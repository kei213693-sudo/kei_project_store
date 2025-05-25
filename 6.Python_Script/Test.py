"""
TestScript.py 用于Python脚本学习调试
编码格式coding: utf-8 适用于处理中文
"""

import re # 正则表达式模块，用于字符串匹配、替换和模式提取（例如验证邮箱、提取URL）
import os # 操作系统接口模块，处理文件路径、目录操作和环境变量（如os.path.join, os.listdir）
import sys # 系统相关功能模块，访问命令行参数、解释器设置及标准输入输出（如sys.argv, sys.exit）
import argparse # 命令行参数解析模块，用于定义和解析用户输入的命令行选项和参数
import subprocess # 子进程管理模块，执行外部命令、管理进程输入输出（如调用系统命令、捕获执行结果）
import logging # 日志记录模块，配置日志级别、格式化和输出（如调试信息、错误跟踪）
import winreg # Windows注册表操作模块，读写注册表键值（仅限Windows系统，如软件配置访问）
import shutil # 高级文件操作模块，实现文件/目录的复制、移动、删除（如shutil.copy, shutil.rmtree）

from typing import Tuple # 类型注解模块，用于明确函数返回值或参数的类型（例如 -> Tuple[int, str]）
from datetime import datetime # 日期时间处理模块，获取当前时间、格式化或解析时间字符串（如 datetime.now(), strftime/%Y-%m-%d）

def main():
    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] Test Start!")

    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] Test End!")

if __name__ == '__main__':
    main()