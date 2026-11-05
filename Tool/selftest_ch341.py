#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
CH341 + 模块连通性自检 (只读, 不做任何写入)

用法:  python selftest_ch341.py [设备号]
默认:  python selftest_ch341.py 0

检查项:
  1. CH341 DLL 位数匹配加载
  2. 打开设备 + I2C 速度设置
  3. 只读探测: 读模块 A2 寄存器 0-3 (擦除协议全程只涉及 A2, 0x51)
"""

import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ch341_erase_fw_tool import CH341, CH341Error, A2_ADDR_W


def main():
    index = int(sys.argv[1]) if len(sys.argv) > 1 else 0

    dev = CH341(index=index)
    print("[1/3] DLL 已加载 (位数匹配): %s" % dev.dll_path)

    dev.open()
    print("[2/3] 设备 %d 已打开, %s"
          % (index, dev.info() or "无版本信息"))

    try:
        data = dev.read_regs(0, 4)
        print("[3/4] [0x%02X] 寄存器 0-3 读到: %s  -> 模块在线, ACK 正常"
              % (A2_ADDR_W, " ".join("%02X" % b for b in data)))
    except CH341Error as e:
        print("[3/4] A2 读取无应答: %s" % e)

    print("[4/4] 扫描总线 0x08-0x77 ...")
    found = dev.scan_bus()
    if found:
        print("      ACK 地址: " + ", ".join("0x%02X" % a for a in found))
        if 0x51 in found:
            print("结论: 模块 A2 (0x51) 在线, 可以运行擦除工具。")
        else:
            print("结论: 总线有设备但无 0x51, 核对地址/固件版本。")
    else:
        print("      无任何应答")
        print("结论: CH341/驱动正常, 总线空 —— 检查模块 3.3V 供电、SCL/SDA/GND 接线、"
              "上拉电阻(4.7k-10k 到 3.3V);")
        print("      若接线确认无误, 则模块可能未上电或固件已空。")
    dev.close()


if __name__ == "__main__":
    main()
