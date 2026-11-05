#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
CH341 I2C 只读诊断: 排列组合测试 DLL版本 × SetStream用法, 定位读不到的原因。
不向任何寄存器写入数据 (纯地址探测 + 读)。
用法: python diag_ch341.py
"""

import os
import sys
import ctypes

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ch341_erase_fw_tool import CH341Error

BASE = os.path.dirname(os.path.abspath(__file__))
SYSROOT = os.environ.get("SystemRoot", r"C:\Windows")

DLLS = [
    ("System32安装版(WIN1X)", os.path.join(SYSROOT, "System32", "CH341DLLA64.DLL")),
    ("包内WIN1X", os.path.join(BASE, "CH341PAR", "CH341PAR", "WIN 1X", "CH341DLLA64.DLL")),
    ("包内根目录(旧)", os.path.join(BASE, "CH341PAR", "CH341PAR", "CH341DLLA64.DLL")),
]

# (说明, Stream模式)  None=不调用SetStream, 其余为CH341SetStream第二参数
STREAM_MODES = [
    ("不调SetStream", None),
    ("SetStream=1(100k)", 1),
    ("SetStream=0(20k)", 0),
    ("SetStream=2(400k)", 2),
]

PROBES = [
    ("A0 读寄存器0-3 ", [0xA0, 0x00], 4),
    ("A2 读寄存器0-3 ", [0xA2, 0x00], 4),
    ("0x50 纯地址探测", [0xA0], 0),
    ("0x51 纯地址探测", [0xA2], 0),
]


def bind(dll):
    dll.CH341OpenDevice.argtypes = [ctypes.c_ulong]
    dll.CH341OpenDevice.restype = ctypes.c_ulong
    dll.CH341CloseDevice.argtypes = [ctypes.c_ulong]
    dll.CH341StreamI2C.argtypes = [ctypes.c_ulong, ctypes.c_ulong,
                                   ctypes.c_void_p,
                                   ctypes.POINTER(ctypes.c_ulong),
                                   ctypes.c_void_p]
    dll.CH341StreamI2C.restype = ctypes.c_int
    dll.CH341SetStream.argtypes = [ctypes.c_ulong, ctypes.c_ulong]
    dll.CH341SetStream.restype = ctypes.c_int
    dll.CH341GetVerIC.argtypes = [ctypes.c_ulong]
    dll.CH341GetVerIC.restype = ctypes.c_ulong


def stream_i2c(dll, wb, rlen):
    wbuf = (ctypes.c_ubyte * len(wb)).from_buffer_copy(bytes(wb))
    rlen_v = ctypes.c_ulong(rlen)
    rbuf = (ctypes.c_ubyte * max(rlen, 1))()
    ok = dll.CH341StreamI2C(0, len(wb), wbuf, ctypes.byref(rlen_v), rbuf)
    if not ok:
        raise CH341Error("NAK/失败")
    return bytes(rbuf[:rlen_v.value]) if rlen else b""


def main():
    for dll_name, dll_path in DLLS:
        if not os.path.isfile(dll_path):
            print("== %s: 文件不存在, 跳过" % dll_name)
            continue
        print("== DLL: %s (%d 字节)" % (dll_name, os.path.getsize(dll_path)))
        try:
            dll = ctypes.WinDLL(dll_path)
        except OSError as e:
            print("   加载失败: %s" % e)
            continue
        bind(dll)
        for mode_name, mode in STREAM_MODES:
            h = dll.CH341OpenDevice(0)
            if h in (0xFFFFFFFF, 0):
                print("   [%s] 打开设备失败" % mode_name)
                continue
            if mode is not None:
                dll.CH341SetStream(0, mode)
            results = []
            for label, wb, rlen in PROBES:
                try:
                    data = stream_i2c(dll, wb, rlen)
                    results.append("%s: OK %s" % (label, " ".join("%02X" % b for b in data)))
                except CH341Error as e:
                    results.append("%s: 无应答" % label)
            print("   [%-18s] %s" % (mode_name, " | ".join(results)))
            dll.CH341CloseDevice(0)
    print("\n请对比其它能正常读 A0/A2 的工具所用适配器是否同一块 CH341A 小板;")
    print("若不是同一块, 请把能用的那块接到本工具再试。")


if __name__ == "__main__":
    main()
