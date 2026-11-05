#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
================================================================================
 SFP(10G DAC) 固件擦除 Host 工具 —— CH341A USB 转 I2C
================================================================================
 对应固件 : 10G_DAC-ADuC7020 (KV0) 工程 (SFP 封装, SFF-8472 双地址 A0h/A2h)
 模块地址 : A0 页 = 0xA0 (7-bit 0x50) / A2 页 = 0xA2 (7-bit 0x51)
            擦除流程只使用 A2 地址
 I2C 访问 : 字节级 API CH341ReadI2C/CH341WriteI2C (每字节独立事务)。
            CH341A3(芯片版本0x30)+新版驱动组合上 CH341StreamI2C 实测不工作,
            故不使用。

 擦除协议 (与固件 code/inttrupt.c / code/pubcom.h 对应):
   步骤1  A2 寄存器 123~126 依次写入 'u','u','a','a'  密码解锁 (0x75 75 61 61)
   步骤2  A2 寄存器 127     写入 0x00                 选择上页 Page00
   步骤3  A2 寄存器 250~253 依次写入 'e','r','a','s'  擦除口令 (0x65 72 61 73)
          —— 写入 253 的最后一个字节 0x73('s') 时, 固件在中断里立即关闭全部
             中断并反复整片擦除 flash, 模块从此不再响应 I2C。

 擦除结果判定: 擦除命令后再读 A2 寄存器, 无 ACK(传输失败) = 擦除成功。

 依赖: 仅 Python 标准库 (tkinter + ctypes) + WCH 官方 CH341DLL.dll
       DLL 获取: 安装 WCH "CH341PAR" 驱动, 或把对应位数的 CH341DLL.dll
       放到本文件同目录 (64 位 Python 需 64 位 DLL, 32 位 Python 需 32 位 DLL)。

 接线: CH341A 的 SCL/SDA/GND 接 QSFP 模块的 SCL/SDA/GND, 模块供 3.3V。
       本固件 A2 地址不受 ModSelL 影响, ModSelL 可不接 (A0 才会被屏蔽)。
================================================================================
"""

import os
import sys
import time
import ctypes
import threading
import traceback

import tkinter as tk
from tkinter import ttk, messagebox
from tkinter.scrolledtext import ScrolledText

APP_TITLE = "SFP 模块固件擦除工具 (CH341A I2C) — 10G DAC ADuC7020 KV0"

# ---------------- 协议参数 (与固件对应) ----------------
A2_ADDR_W      = 0xA2          # A2 页 8-bit 写地址 (7-bit 0x51)
REG_PWD        = 123           # PUBCOM_PAGE_OFFSET_PWD_START
PWD_BYTES      = b"uuaa"       # glb_passwrd[] = {'u','u','a','a'}
REG_PAGESEL    = 127           # 上页选择寄存器
PAGE_00        = 0x00          # PUBCOM_UPPAGE_00
REG_ERASE      = 250           # PUBCOM_UPMEM_PAGE00_OFFSET_ERASE_START
ERASE_BYTES    = b"eras"       # glb_erase[] = {'e','r','a','s'}
REG_DETECT     = 0             # 检测用: 读 A2 寄存器 0 起
DETECT_LEN     = 4
STEP_DELAY_S   = 0.10          # 步骤间延时
VERIFY_DELAY_S = 0.50          # 擦除后等待复测延时


# =====================================================================
#  CH341 DLL 封装
# =====================================================================
class CH341Error(Exception):
    """CH341 操作异常"""


class CH341:
    """基于 WCH 官方 CH341DLL 的最小 I2C 封装 (CH341A 硬件 I2C)

    DLL 命名 (CH341PAR 驱动包):
      32 位 = CH341DLL.DLL   64 位 = CH341DLLA64.DLL
    加载时按 PE 头位数自动挑选匹配当前 Python 的那一个。
    """

    INVALID_HANDLE = 0xFFFFFFFF

    def __init__(self, index=0):
        self.index = index
        self.is_open = False
        self.dll = self._load_dll()
        self.dll_path = CH341.last_loaded_path
        self._bind()

    # ---------- DLL 加载 ----------
    @staticmethod
    def _pe_machine(path):
        """读 PE 头 Machine 字段: 0x14C=x86, 0x8664=x64, 0xAA64=arm64"""
        try:
            with open(path, "rb") as f:
                if f.read(2) != b"MZ":
                    return None
                f.seek(0x3C)
                e_lfanew = int.from_bytes(f.read(4), "little")
                f.seek(e_lfanew + 4)          # 跳过 "PE\0\0"
                return int.from_bytes(f.read(2), "little")
        except OSError:
            return None

    @classmethod
    def _expected_machine(cls):
        import platform
        if platform.machine() == "ARM64":
            return 0xAA64
        return 0x8664 if ctypes.sizeof(ctypes.c_void_p) == 8 else 0x14C

    @staticmethod
    def _machine_name(m):
        return {0x14C: "32位(x86)", 0x8664: "64位(x64)",
                0xAA64: "ARM64"}.get(m, "0x%X" % m if m else "未知")

    @classmethod
    def _load_dll(cls):
        base = os.path.dirname(os.path.abspath(__file__))
        sysroot = os.environ.get("SystemRoot", r"C:\Windows")
        # 新版 CH341PAR: 64 位 DLL 名为 CH341DLLA64.DLL
        search_dirs = [
            base,
            os.path.join(base, "CH341PAR", "CH341PAR"),            # 本工具自带驱动包
            os.path.join(base, "CH341PAR", "CH341PAR", "WIN 1X"),
            os.path.join(sysroot, "System32"),                     # 64 位驱动安装位置
            os.path.join(sysroot, "SysWOW64"),                     # 32 位驱动安装位置
        ]
        candidates = []
        for d in search_dirs:
            for n in ("CH341DLLA64.DLL", "CH341DLL.dll"):
                candidates.append(os.path.join(d, n))
        candidates += ["CH341DLLA64.DLL", "CH341DLL.dll"]          # 系统搜索路径兜底

        want = cls._expected_machine()
        found_mismatch = []        # (路径, 位数) 已找到但位数不符
        for path in candidates:
            if not os.path.isfile(path):
                continue
            if cls._pe_machine(path) != want:
                found_mismatch.append(
                    "  %s  [%s]" % (path, cls._machine_name(cls._pe_machine(path))))
                continue
            dll = ctypes.WinDLL(path)
            cls.last_loaded_path = path
            return dll

        if found_mismatch:
            raise CH341Error(
                "找到 CH341 DLL 但位数都与当前 Python (%s) 不匹配:\n%s\n"
                "请从 WCH CH341PAR 驱动包 (Tool\\CH341PAR) 取 %s 版本的 DLL "
                "放到工具目录, 或用匹配位数的 Python 运行。"
                % (cls._machine_name(want), "\n".join(found_mismatch),
                   cls._machine_name(want)))
        raise CH341Error(
            "找不到 CH341 DLL (CH341DLL.DLL / CH341DLLA64.DLL) ——\n"
            "请安装 WCH CH341PAR 驱动, 或将对应位数的 DLL 放到本工具目录 (%s)。" % base)

    # ---------- 函数原型 ----------
    def _bind(self):
        d = self.dll
        d.CH341OpenDevice.argtypes  = [ctypes.c_ulong]
        d.CH341OpenDevice.restype   = ctypes.c_ulong
        d.CH341CloseDevice.argtypes = [ctypes.c_ulong]
        d.CH341CloseDevice.restype  = None
        # 字节级 I2C API —— 在 CH341A3(版本0x30)+新版驱动上实测可用
        # (CH341StreamI2C 在该组合上连地址探测都会失败, 故不使用)
        # BOOL CH341ReadI2C (idx, iDevice[低7位地址], iAddr, *oByte)
        d.CH341ReadI2C.argtypes = [ctypes.c_ulong, ctypes.c_ubyte, ctypes.c_ubyte,
                                   ctypes.POINTER(ctypes.c_ubyte)]
        d.CH341ReadI2C.restype = ctypes.c_int
        # BOOL CH341WriteI2C(idx, iDevice[低7位地址], iAddr, iByte)
        d.CH341WriteI2C.argtypes = [ctypes.c_ulong, ctypes.c_ubyte, ctypes.c_ubyte,
                                    ctypes.c_ubyte]
        d.CH341WriteI2C.restype = ctypes.c_int
        for fn in ("CH341GetVersion", "CH341GetVerIC"):   # 可选信息函数
            f = getattr(d, fn, None)
            if f is not None:
                f.argtypes = [ctypes.c_ulong]
                f.restype = ctypes.c_ulong

    # ---------- 打开 / 关闭 ----------
    def open(self):
        h = self.dll.CH341OpenDevice(self.index)
        if h == self.INVALID_HANDLE or h == 0:
            raise CH341Error(
                "CH341OpenDevice(%d) 失败: 未找到 CH341 设备或驱动未安装" % self.index)
        self.is_open = True
        return h

    def close(self):
        if self.is_open:
            self.dll.CH341CloseDevice(self.index)
            self.is_open = False

    def info(self):
        """返回 (驱动版本, 芯片版本) 字符串, 供日志显示"""
        out = []
        ver = getattr(self.dll, "CH341GetVersion", None)
        if ver is not None:
            try:
                out.append("驱动版本=0x%02X" % ver(self.index))
            except Exception:
                pass
        ic = getattr(self.dll, "CH341GetVerIC", None)
        if ic is not None:
            try:
                v = ic(self.index)
                name = {0x10: "CH341", 0x20: "CH341A", 0x30: "CH341A3"}.get(
                    v, "未知(0x%02X)" % v)
                out.append("芯片=%s" % name)
            except Exception:
                pass
        return " ".join(out)

    # ---------- I2C 传输 (字节级 API) ----------
    DEV7 = None   # 占位, 实例化后由模块地址派生

    def write_byte(self, reg, val):
        """单字节写: START + 0xA2 + reg + val + STOP (独立事务)"""
        if not self.is_open:
            raise CH341Error("CH341 设备未打开")
        if not self.dll.CH341WriteI2C(self.index, A2_ADDR_W >> 1, reg, val):
            raise CH341Error(
                "I2C 写失败 (从机无 ACK 或总线错误): 寄存器 %d ← 0x%02X" % (reg, val))

    def read_byte(self, reg):
        """单字节读: START + 0xA2 + reg + 重复START + 读 + STOP"""
        if not self.is_open:
            raise CH341Error("CH341 设备未打开")
        b = ctypes.c_ubyte(0)
        if not self.dll.CH341ReadI2C(self.index, A2_ADDR_W >> 1, reg,
                                     ctypes.byref(b)):
            raise CH341Error("I2C 读失败 (从机无 ACK 或总线错误): 寄存器 %d" % reg)
        return b.value

    # ---------- 器件层封装 ----------
    def write_regs(self, start_reg, data):
        """逐字节写入(每字节独立事务, 寄存器地址显式携带, 与固件中断处理一致)"""
        for i, b in enumerate(bytes(data)):
            self.write_byte(start_reg + i, b)

    def read_regs(self, start_reg, n):
        """逐字节读回 n 个寄存器"""
        return bytes(self.read_byte(start_reg + i) for i in range(n))

    def scan_bus(self, lo=0x08, hi=0x77):
        """地址扫描: 逐地址读寄存器0。字节级 API 的返回值不作 ACK 判断
        (全地址都返回成功), 故以 数据 != 0xFF (总线空闲填充值) 判定在线。"""
        found = []
        b = ctypes.c_ubyte(0)
        for a in range(lo, hi + 1):
            self.dll.CH341ReadI2C(self.index, a, 0, ctypes.byref(b))
            if b.value != 0xFF:
                found.append(a)
        return found


# =====================================================================
#  UI
# =====================================================================
class App:
    def __init__(self, root):
        self.root = root
        root.title(APP_TITLE)
        root.geometry("780x560")
        root.minsize(700, 480)

        self.dev = None                 # CH341 实例
        self.busy = False
        self.action_btns = []
        self.logq = []                  # 主线程取日志 (锁保护)
        self.log_lock = threading.Lock()
        self.popup = None               # (kind, title, msg) 由主线程弹窗

        self._build_ui()
        self.root.after(60, self._poll)

    # ---------------- 界面 ----------------
    def _build_ui(self):
        pad = {"padx": 6, "pady": 4}

        # 标题
        top = tk.Frame(self.root)
        top.pack(fill="x", **pad)
        tk.Label(top, text="SFP 模块固件擦除工具",
                 font=("Microsoft YaHei UI", 14, "bold")).pack(side="left")
        tk.Label(top, text="(CH341A → I2C, 模块 A2 地址 0x51)",
                 fg="#666666").pack(side="left", padx=8)

        # ---- 连接区 ----
        conn = ttk.LabelFrame(self.root, text=" 1. 连接 CH341 ")
        conn.pack(fill="x", **pad)
        tk.Label(conn, text="设备号:").grid(row=0, column=0, padx=(8, 2), pady=5)
        self.var_index = tk.IntVar(value=0)
        ttk.Spinbox(conn, from_=0, to=15, width=4, textvariable=self.var_index)\
            .grid(row=0, column=1, padx=2)
        self.btn_open = ttk.Button(conn, text="打开设备", command=self.on_open)
        self.btn_open.grid(row=0, column=2, padx=6)
        self.btn_close = ttk.Button(conn, text="关闭设备", command=self.on_close,
                                    state="disabled")
        self.btn_close.grid(row=0, column=3, padx=6)
        tk.Label(conn, text="状态:").grid(row=0, column=4, padx=(16, 2))
        self.lbl_status = tk.Label(conn, text="● 未连接", fg="gray")
        self.lbl_status.grid(row=0, column=5)

        # ---- 操作区 ----
        ops = ttk.LabelFrame(self.root, text=" 2. 操作 ")
        ops.pack(fill="x", **pad)

        row1 = tk.Frame(ops)
        row1.pack(fill="x", padx=8, pady=(6, 2))
        self.btn_detect = ttk.Button(row1, text="检测模块 (读 A2 寄存器 0-3)",
                                     command=self.on_detect)
        self.btn_detect.pack(side="left")
        self.btn_scan = ttk.Button(row1, text="扫描总线 (0x08-0x77)",
                                   command=self.on_scan)
        self.btn_scan.pack(side="left", padx=6)

        row2 = tk.Frame(ops)
        row2.pack(fill="x", padx=8, pady=2)
        tk.Label(row2, text="分步调试:").pack(side="left")
        self.btn_step1 = ttk.Button(row2, text="① 写密码 uuaa → A2:123-126",
                                    command=lambda: self.on_step(1))
        self.btn_step1.pack(side="left", padx=4)
        self.btn_step2 = ttk.Button(row2, text="② 选页 Page00 → A2:127=0x00",
                                    command=lambda: self.on_step(2))
        self.btn_step2.pack(side="left", padx=4)
        self.btn_step3 = ttk.Button(row2, text="③ 写擦除口令 eras → A2:250-253",
                                    command=lambda: self.on_step(3))
        self.btn_step3.pack(side="left", padx=4)

        row3 = tk.Frame(ops)
        row3.pack(fill="x", padx=8, pady=(2, 6))
        self.btn_erase = tk.Button(
            row3, text="⚠  一键擦除固件 (密码 → 选页 → 擦除, 不可恢复)  ⚠",
            font=("Microsoft YaHei UI", 11, "bold"),
            fg="white", bg="#C62828", activebackground="#B71C1C",
            activeforeground="white", relief="raised", cursor="hand2",
            command=self.on_erase)
        self.btn_erase.pack(fill="x", ipady=6)

        self.action_btns = [self.btn_detect, self.btn_scan, self.btn_step1,
                            self.btn_step2, self.btn_step3, self.btn_erase]

        # ---- 日志区 ----
        logf = ttk.LabelFrame(self.root, text=" 3. 日志 ")
        logf.pack(fill="both", expand=True, **pad)
        self.txt = ScrolledText(logf, height=14, state="disabled",
                                font=("Consolas", 9))
        self.txt.pack(fill="both", expand=True, padx=6, pady=6)
        self.txt.tag_config("tx",  foreground="#1565C0")   # 事务
        self.txt.tag_config("ok",  foreground="#2E7D32")   # 成功
        self.txt.tag_config("err", foreground="#C62828")   # 失败
        self.txt.tag_config("warn", foreground="#E65100")  # 警告

        bar = tk.Frame(logf)
        bar.pack(fill="x", padx=6, pady=(0, 6))
        ttk.Button(bar, text="清空日志", command=self.clear_log).pack(side="left")
        ttk.Button(bar, text="帮助", command=self.show_help).pack(side="left", padx=6)

        # ---- 底部说明 ----
        tk.Label(self.root, fg="#888888", justify="left", text=(
            "提示: 擦除命令写入 A2 寄存器 253 的最后一个字节时固件立即自毁, 模块不再响应任何 I2C;\n"
            "擦除后只能通过 JTAG 或 UART 下载模式重新烧写。I2C 访问采用字节级 API(CH341ReadI2C/WriteI2C)。"),
        ).pack(fill="x", padx=10, pady=(0, 6))

    # ---------------- 日志 / 弹窗 (线程安全) ----------------
    def log(self, msg, tag=None):
        with self.log_lock:
            self.logq.append((msg, tag))

    def _poll(self):
        with self.log_lock:
            items = self.logq
            self.logq = []
        if items:
            self.txt.config(state="normal")
            for msg, tag in items:
                t = time.strftime("%H:%M:%S")
                self.txt.insert("end", "[%s] " % t, tag)
                if tag:
                    self.txt.insert("end", msg + "\n", tag)
                else:
                    self.txt.insert("end", msg + "\n")
            self.txt.see("end")
            self.txt.config(state="disabled")
        if self.popup:
            kind, title, msg = self.popup
            self.popup = None
            if kind == "error":
                messagebox.showerror(title, msg)
            elif kind == "warn":
                messagebox.showwarning(title, msg)
            else:
                messagebox.showinfo(title, msg)
        self.root.after(60, self._poll)

    def clear_log(self):
        self.txt.config(state="normal")
        self.txt.delete("1.0", "end")
        self.txt.config(state="disabled")

    def set_busy(self, busy):
        self.busy = busy
        state = "disabled" if busy else "normal"
        for b in self.action_btns:
            b.config(state=state)
        self.btn_open.config(state=state)
        self.btn_close.config(state=state)

    def set_status(self, text, color):
        self.lbl_status.config(text=text, fg=color)

    # ---------------- 后台线程辅助 ----------------
    def run_worker(self, fn):
        if self.busy:
            return

        def wrap():
            try:
                fn()
            except CH341Error as e:
                self.log("错误: %s" % e, "err")
                self.popup = ("error", "操作失败", str(e))
            except Exception:
                self.log("异常:\n%s" % traceback.format_exc(), "err")
                self.popup = ("error", "未处理的异常", traceback.format_exc(limit=3))
            finally:
                self.root.after(0, lambda: self.set_busy(False))

        self.set_busy(True)
        threading.Thread(target=wrap, daemon=True).start()

    def ensure_open(self):
        """返回已打开的设备; 未打开则按当前设备号自动打开"""
        if self.dev is not None and self.dev.is_open:
            return self.dev
        self.log("正在打开 CH341 (设备号 %d) ..." % self.var_index.get())
        dev = CH341(index=self.var_index.get())
        dev.open()
        info = dev.info()
        self.log("CH341 已打开 [DLL: %s] [字节级 I2C API] %s"
                 % (os.path.basename(dev.dll_path or "?"), info), "ok")
        self.dev = dev
        self.root.after(0, lambda: self.set_status("● 已连接", "#2E7D32"))
        self.root.after(0, lambda: self.btn_close.config(state="normal"))
        return dev

    # ---------------- 事务日志辅助 ----------------
    def log_write(self, start, data, desc):
        hexs = " ".join("%02X" % b for b in data)
        self.log("→ [0x%02X] 寄存器 %d..%d ← %s  (%s)" %
                 (A2_ADDR_W, start, start + len(data) - 1, hexs, desc), "tx")

    def write_regs(self, dev, start, data, desc):
        """逐字节写入并记录日志 (每字节一笔独立 I2C 事务, 寄存器地址显式携带)"""
        dev.write_regs(start, bytes(data))
        self.log_write(start, bytes(data), desc + " [逐字节]")

    # ================= 按钮动作 =================
    def on_open(self):
        self.run_worker(lambda: (self.ensure_open()))

    def on_close(self):
        if self.dev is not None:
            self.dev.close()
            self.log("CH341 已关闭")
        self.set_status("● 未连接", "gray")
        self.btn_close.config(state="disabled")

    def on_scan(self):
        def fn():
            dev = self.ensure_open()
            self.log("扫描 I2C 总线 (7-bit 地址 0x08-0x77) ...")
            found = dev.scan_bus()
            if not found:
                self.log("总线上无任何设备应答 —— 检查模块 3.3V 供电、SCL/SDA/GND 接线、"
                         "上拉电阻, 或模块固件已空。", "err")
                return
            names = {0x50: "模块 A0 页", 0x51: "模块 A2 页 (擦除用)",
                     0x57: "常见 EEPROM"}
            for a in found:
                self.log("  0x%02X ACK  %s" % (a, names.get(a, "")), "ok")
            if 0x51 in found:
                self.log("模块 A2 地址在线, 可执行检测/擦除。", "ok")
            else:
                self.log("未发现 0x51 (模块 A2 页)。", "warn")
        self.run_worker(fn)

    def module_alive(self, dev):
        """读 A2 寄存器 0-3。字节级 API 无 ACK 判断, 以"非全 0xFF"判定在线"""
        data = dev.read_regs(REG_DETECT, DETECT_LEN)
        hexs = " ".join("%02X" % b for b in data)
        return any(b != 0xFF for b in data), hexs

    def on_detect(self):
        def fn():
            dev = self.ensure_open()
            self.log("检测模块: 读 A2 寄存器 %d 起 %d 字节 ..." % (REG_DETECT, DETECT_LEN))
            alive, hexs = self.module_alive(dev)
            if alive:
                self.log("← [0x%02X] 寄存器 %d..%d → %s   模块在线" %
                         (A2_ADDR_W, REG_DETECT, REG_DETECT + DETECT_LEN - 1,
                          hexs), "ok")
            else:
                self.log("← 读到全 0xFF —— 模块无应答 (未上电/接线断/固件已空)", "err")
        self.run_worker(fn)

    def on_step(self, step):
        def fn():
            dev = self.ensure_open()
            if step == 1:
                self.log("步骤① 写密码 \"uuaa\" ...")
                self.write_regs(dev, REG_PWD, PWD_BYTES, "密码")
                time.sleep(STEP_DELAY_S)
                self.log("密码已写入 (写入126时固件自动校验)", "ok")
            elif step == 2:
                self.log("步骤② 选择上页 Page00 ...")
                self.write_regs(dev, REG_PAGESEL, [PAGE_00], "页选择")
                time.sleep(STEP_DELAY_S)
                self.log("页选择已写入, 当前上页 = Page00", "ok")
            elif step == 3:
                self.log("步骤③ 写擦除口令 \"eras\" —— 写完即触发整片擦除!", "warn")
                try:
                    self.write_regs(dev, REG_ERASE, ERASE_BYTES, "擦除口令")
                except CH341Error as e:
                    # 最后一个字节落笔后固件立刻关中断, STOP 阶段可能无应答, 属正常
                    self.log("写口令阶段无应答(%s) —— 固件可能已开始自擦, 属正常现象" % e, "warn")
                time.sleep(VERIFY_DELAY_S)
                self._verify_erased(dev)
        self.run_worker(fn)

    def on_erase(self):
        if not messagebox.askyesno(
                "危险操作确认",
                "即将永久擦除模块内全部固件!\n\n"
                "• 擦除立即执行且不可恢复\n"
                "• 擦除后模块不再响应任何 I2C 命令\n"
                "• 只能通过 JTAG 或 UART 下载模式重新烧写\n\n"
                "确认已连接目标模块并继续?", icon="warning"):
            return

        def fn():
            dev = self.ensure_open()
            self.log("========== 开始一键擦除 ==========", "warn")

            # 0. 预检测
            self.log("[0/4] 预检测模块 ...")
            alive, hexs = self.module_alive(dev)
            if not alive:
                self.log("读到 %s —— 模块无应答, 中止 (避免对空总线误报成功)" % hexs, "err")
                self.popup = ("error", "模块无应答",
                              "A2 寄存器 0-3 读到全 0xFF, 模块不在线。\n"
                              "请先检查供电/接线, 或用\"检测模块\"按钮排查。")
                return
            self.log("模块在线, 寄存器 0-3 = %s" % hexs, "ok")

            # 1. 密码
            self.log("[1/4] 写密码 \"uuaa\" → A2 寄存器 123-126 ...")
            self.write_regs(dev, REG_PWD, PWD_BYTES, "密码")
            time.sleep(STEP_DELAY_S)

            # 2. 页选择
            self.log("[2/4] 选择上页 Page00 → A2 寄存器 127 = 0x00 ...")
            self.write_regs(dev, REG_PAGESEL, [PAGE_00], "页选择")
            time.sleep(STEP_DELAY_S)

            # 3. 擦除口令
            self.log("[3/4] 写擦除口令 \"eras\" → A2 寄存器 250-253 (写完253立即自擦) ...", "warn")
            try:
                self.write_regs(dev, REG_ERASE, ERASE_BYTES, "擦除口令")
                self.log("口令写入完成", "ok")
            except CH341Error as e:
                self.log("写口令阶段无应答(%s) —— 固件可能已开始自擦, 属正常现象" % e, "warn")

            # 4. 复测
            time.sleep(VERIFY_DELAY_S)
            self.log("[4/4] 复测模块响应 ...")
            self._verify_erased(dev)

        self.run_worker(fn)

    def _verify_erased(self, dev):
        """擦除后复测: 读回全 0xFF (总线无应答) => 擦除成功"""
        alive, hexs = self.module_alive(dev)
        self.log("复测读 A2 寄存器 0-3: %s" % hexs, "tx")
        if not alive:
            self.log("模块已无应答 —— 固件擦除成功!", "ok")
            self.log("========== 擦除流程结束 ==========", "ok")
            self.popup = ("info", "擦除成功",
                          "模块固件已被擦除, 模块不再响应 I2C。\n"
                          "请通过 JTAG 或 UART 下载模式重新烧写固件。")
            return
        self.log("警告: 模块仍有数据返回, 擦除可能未生效!", "err")
        self.log("========== 擦除流程结束(结果存疑) ==========", "err")
        self.popup = ("warn", "结果存疑",
                      "擦除命令已发出, 但模块仍然响应 I2C。\n"
                      "请检查: ①步骤顺序与寄存器地址 ②固件版本是否匹配 ③更换模块重试。")

    # ---------------- 帮助 ----------------
    def show_help(self):
        messagebox.showinfo("帮助", (
            "【擦除协议】(模块 A2 页, 7-bit 地址 0x51, 写地址 0xA2)\n"
            "  ① 寄存器 123-126 ← 75 75 61 61 (\"uuaa\" 密码)\n"
            "  ② 寄存器 127 ← 00 (选择上页 Page00)\n"
            "  ③ 寄存器 250-253 ← 65 72 61 73 (\"eras\" 擦除口令)\n"
            "     写入 253 的最后一个字节时固件立即关中断并整片自擦。\n\n"
            "【接线】CH341A: SCL/SDA/GND ↔ 模块 SCL/SDA/GND, 模块供电 3.3V。\n\n"
            "【DLL】WCH CH341PAR 驱动包 (Tool\\CH341PAR 目录自带):\n"
            "  32 位名为 CH341DLL.DLL, 64 位名为 CH341DLLA64.DLL,\n"
            "  工具按 PE 位数自动匹配当前 Python, 无需手动放置。\n\n"
            "【判定】字节级 API 无 ACK 判断, 以读回数据区分:\n"
            "  有效数据 = 模块在线; 全 0xFF = 无应答(已擦除/不在位)。\n"
            "  擦除成功 = 复测读回全 0xFF; 之后需 JTAG/UART 重烧。"))


def main():
    root = tk.Tk()
    try:
        ttk.Style().theme_use("vista")
    except Exception:
        pass
    App(root)
    root.mainloop()


if __name__ == "__main__":
    main()
