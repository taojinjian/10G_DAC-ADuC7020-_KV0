#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""枚举所有 CH341 设备号, 并对每台做 0x50/0x51 只读地址探测 (不写数据)"""
import os
import sys
import ctypes

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ch341_erase_fw_tool import CH341Error

BASE = os.path.dirname(os.path.abspath(__file__))
SYSROOT = os.environ.get("SystemRoot", r"C:\Windows")


def main():
    dll = ctypes.WinDLL(os.path.join(SYSROOT, "System32", "CH341DLLA64.DLL"))
    dll.CH341OpenDevice.argtypes = [ctypes.c_ulong]
    dll.CH341OpenDevice.restype = ctypes.c_ulong
    dll.CH341CloseDevice.argtypes = [ctypes.c_ulong]
    dll.CH341GetVerIC.argtypes = [ctypes.c_ulong]
    dll.CH341GetVerIC.restype = ctypes.c_ulong
    dll.CH341GetDeviceName.argtypes = [ctypes.c_ulong]
    dll.CH341GetDeviceName.restype = ctypes.c_char_p
    dll.CH341StreamI2C.argtypes = [ctypes.c_ulong, ctypes.c_ulong,
                                   ctypes.c_void_p,
                                   ctypes.POINTER(ctypes.c_ulong),
                                   ctypes.c_void_p]
    dll.CH341StreamI2C.restype = ctypes.c_int
    dll.CH341GetStatus.argtypes = [ctypes.c_ulong,
                                   ctypes.POINTER(ctypes.c_ulong)]
    dll.CH341GetStatus.restype = ctypes.c_int
    dll.CH341ReadI2C.argtypes = [ctypes.c_ulong, ctypes.c_ubyte, ctypes.c_ubyte,
                                 ctypes.POINTER(ctypes.c_ubyte)]
    dll.CH341ReadI2C.restype = ctypes.c_int

    found_any = False
    for idx in range(8):
        h = dll.CH341OpenDevice(idx)
        if h in (0xFFFFFFFF, 0):
            if idx == 0:
                print("设备号 0 都打不开, 没有任何 CH341 在线?")
            break
        found_any = True
        try:
            ver = dll.CH341GetVerIC(idx)
            name = dll.CH341GetDeviceName(idx)
            name = name.decode("mbcs", "replace") if name else "?"
            rlen = ctypes.c_ulong(0)
            rbuf = (ctypes.c_ubyte * 1)()
            ack = []
            for label, addr8 in (("0x50/A0", 0xA0), ("0x51/A2", 0xA2)):
                wbuf = (ctypes.c_ubyte * 1).from_buffer_copy(bytes([addr8]))
                ok = dll.CH341StreamI2C(idx, 1, wbuf, ctypes.byref(rlen), rbuf)
                ack.append("%s:%s" % (label, "ACK!" if ok else "无应答"))
            print("设备号 %d: 芯片版本=0x%02X  %s  [%s]"
                  % (idx, ver, name, " | ".join(ack)))

            # 引脚静态电平 (位22=SCL, 位23=SDA)
            st = ctypes.c_ulong(0)
            if dll.CH341GetStatus(idx, ctypes.byref(st)):
                v = st.value
                print("    引脚电平: SCL=%d SDA=%d  D7-D0=0x%02X  %s"
                      % (1 if v & 0x00400000 else 0,
                         1 if v & 0x00800000 else 0,
                         v & 0xFF,
                         "(SCL/SDA 应为 1=高电平)" ))
            # 字节级 API: CH341ReadI2C (低7位地址)
            for label, dev7 in (("A0(0x50)", 0x50), ("A2(0x51)", 0x51)):
                b = ctypes.c_ubyte(0)
                ok = dll.CH341ReadI2C(idx, dev7, 0, ctypes.byref(b))
                print("    CH341ReadI2C %s 寄存器0: %s"
                      % (label, ("0x%02X" % b.value) if ok else "无应答"))
        finally:
            dll.CH341CloseDevice(idx)
    if not found_any:
        print("未枚举到任何 CH341 设备")


if __name__ == "__main__":
    main()
