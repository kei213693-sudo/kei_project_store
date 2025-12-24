"""
TestScript.py 用于Python脚本学习调试
编码格式coding: utf-8 适用于处理中文
"""

import os  # 导入操作系统接口
import re  # 导入正则表达式模块
import shutil
import subprocess  # 导入子进程（即调用外部程序）管理模块
import sys
import tempfile
from datetime import datetime
from pathlib import Path

from file_utils import (
    file_clear,
    file_compression_zipfile,
    file_copy,
    file_delete,
    file_rename,
)

# 获取脚本文件所在路径
script_dir = os.path.dirname(os.path.abspath(__file__))
# 获取脚本文件上一级目录路径
parent_dir = os.path.dirname(script_dir)

# 确保目录路径以 \ 结尾
if not script_dir.endswith(os.sep):
    script_dir += os.sep
if not parent_dir.endswith(os.sep):
    parent_dir += os.sep


def get_macro_in_file(file_path, macro_name) -> str | None:
    """
    功能:获取该文件某个宏定义的宏名的有效字符串内容
    参数:
        file_path (str): 头文件路径
        macro_name (str): 宏名称
    返回:
        str: 清理后的宏值,或None(未找到)
    """
    try:
        with open(file_path, "r", encoding="utf-8") as file:
            content = file.read()
        pattern = re.compile(
            rf'^\s*#\s*define\s+{re.escape(macro_name)}\s*"([^"]*)"(?:\s*//.*)?$',
            re.MULTILINE,
        )
        match = pattern.search(content)
        if match:
            value = re.sub(r"\s+", "", match.group(1))
            # 特殊处理SUPP_SOFT_VERSION：移除冒号
            if macro_name == "SUPP_SOFT_VERSION":
                value = value.replace(":", "")
            return value
        return None
    except Exception:
        return None


def get_macro_all_in_file(file_path, macro_name) -> str | None:
    """
    功能:获取该文件某个宏定义的宏名后的所有内容
    参数:
        file_path (str): 文件路径
        macro_name (str): 要获取的宏名称
    返回:
        str: 宏名后的所有内容
    """
    # 检查文件是否存在
    if not os.path.exists(file_path):
        print(f"文件不存在: {file_path}")
        return None
    # 读取文件内容
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    # 正则表达式模式：精确匹配#define行,捕获宏名后的所有内容
    pattern = rf"^\s*#\s*define\s+{re.escape(macro_name)}\b(.*)$"
    # 使用多行模式进行匹配,加上re.IGNORECASE可以忽略大小写
    match = re.search(pattern, content, re.MULTILINE)

    if match:
        # 获取完整的匹配行
        full_line = match.group(0)
        # 计算宏名结束位置（包括所有空格）
        define_part = f"#define {macro_name}"
        macro_start = full_line.index(define_part) + len(define_part)
        # 提取宏名后的所有内容（包括前面的空格）
        value_part = full_line[macro_start:].rstrip("\n")
        return value_part
    else:
        print(f"未找到宏定义: {macro_name}")
        return None


def find_jflash_path():
    """
    功能:查找JFlash.exe安装路径
    参数:无
    返回:JFlash.exe路径,没找到就返回None
    """
    jflash_path = None

    # 尝试从环境变量获取
    jflash_path = os.environ.get("keil_exe_path")

    # 若环境变量未设置,尝试从PATH环境变量搜索
    if not jflash_path:
        jflash_path = shutil.which("JFlash.exe")

    # 若仍未找到,尝试常见安装位置,或者手动写入安装位置
    if not jflash_path:
        jflash_path_list = [
            r"C:\Program Files\SEGGER\JLink\JFlash.exe",
            r"C:\Program Files (x86)\SEGGER\JLink\JFlash.exe",
            r"C:\Program Files (x86)\SEGGER\JLink_V720\JFlash.exe",
            r"C:\SEGGER\JLink\JFlash.exe",
        ]
        for path in jflash_path_list:
            if os.path.isfile(path):
                jflash_path = path
                break

    return jflash_path


def modify_macro_in_file(file_path, macro_name, new_value) -> bool:
    """
    功能:修改C或H文件中的宏定义值
    参数:
        file_path (str): 文件路径
        macro_name (str): 要修改的宏名称
        new_value (str): 新的宏值,从宏名称后面包括空格和注释
    返回:
        True: 成功
        False: 失败
    """
    # 检查文件是否存在
    if not os.path.exists(file_path):
        print(f"文件不存在: {file_path}")
        return False
    # 读取文件内容
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    # 定义正则表达式模式匹配宏定义
    pattern = rf"^\s*#\s*define\s+{re.escape(macro_name)}\s+(\S*)(.*)$"
    # 使用多行模式进行匹配,加上re.IGNORECASE可以忽略大小写
    match = re.search(pattern, content, re.MULTILINE)

    if match:
        # 构建新的宏定义行
        new_line = f"#define {macro_name}{new_value}"
        # 替换内容
        modified_content = re.sub(
            pattern, new_line, content, flags=re.MULTILINE | re.IGNORECASE
        )

        # 写回文件
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(modified_content)
        # print(f"成功将宏 {macro_name} 的值修改为 {new_string_value}")
        return True
    else:
        print(f"未找到宏定义: {macro_name}")
        return False


def convert_bin_with_jflash(
    bin_file, base_address, output_format="hex", output_file=None
) -> bool:
    """
    功能:使用J-Flash将BIN文件转换为HEX或S19格式
    参数:param bin_file: 输入的BIN文件路径
        param base_address: 起始地址 (十六进制值，如0x08000000)
        param output_format: 输出格式 ("hex" 或 "s19")
        param output_file: 输出文件路径
    返回:
        True: 成功
        False: 失败
    备注:Jfalsh命令行说明参考：https://kb.segger.com/UM08003_JFlash#Batch_processing
    """
    # 验证输入文件是否存在
    if not os.path.isfile(bin_file):
        print(f"[ERROR] BIN文件不存在: {bin_file}")
        return False

    # 验证输出格式
    output_format = output_format.lower()
    if output_format not in ["hex", "s19"]:
        print(f"[ERROR] 不支持的输出格式: '{output_format}'，必须是 'hex' 或 's19'")
        return False

    # 设置输出文件名
    if output_file is None:
        bin_stem = os.path.splitext(os.path.basename(bin_file))[0]
        output_file = f"{bin_stem}_{hex(base_address)[2:].upper()}.{output_format}"

    # 确保输出目录存在
    output_dir = os.path.dirname(output_file)
    if output_dir and not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 查找JFlash.exe路径
    jflash_exe_path = find_jflash_path()
    if jflash_exe_path is None:
        print("[ERROR] 未找到JFlash.exe，请安装SEGGER J-Flash软件")
        return False

    try:
        # 构建J-Flash命令行
        cmd = [
            f'"{jflash_exe_path}"',  # 双引号包裹路径处理空格
            # f'-openprj"{project_file}"', # 打开某个指定的.jflash的工程文件，这里我们只需要转换文件格式，不需要connect芯片去烧录，所以不指定，这样运行时会打开一个默认项目，兼容不同的芯片
            f'-open"{bin_file},{hex(base_address)}"',  # 逗号分隔地址
            f'-saveas"{output_file}"',
            "-hide",  # 隐藏GUI界面
            "-exit",  # 退出 J-Flash
        ]
        full_cmd = " ".join(cmd)

        # 执行命令
        result = subprocess.run(
            full_cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=30,  # 30秒超时
        )

        # 验证结果
        if os.path.isfile(output_file):
            # print(f"[SUCCESS] 转换成功! 输出文件: {output_file}")
            return True
        else:
            print("[ERROR] 转换失败，输出文件未生成")
            print(f"[CMD] 执行命令: {full_cmd}")
            # 错误诊断
            if "Invalid base address" in result.stdout:
                print("[DIAG] 无效的起始地址")
            elif "could not open file" in result.stdout:
                print("[DIAG] 无法打开输入文件")
            elif "Unsupported file format" in result.stdout:
                print("[DIAG] 不支持的文件格式")
            return False

    except subprocess.TimeoutExpired:
        print("[ERROR] 命令执行超时")
        return False
    except Exception as e:
        print(f"[ERROR] 执行命令时出错: {e}")
        return False


def find_iarbuild_path():
    """
    功能:查找iarbuild.exe安装路径
    参数:无
    返回:iarbuild.exe路径,没找到就返回None
    """
    iarbuild_path = None
    # 从环境变量获取IAR_PATH
    iarbuild_path = os.environ.get("IAR_PATH")

    # 若环境变量未设置,从PATH搜索
    if not iarbuild_path:
        iarbuild_path = shutil.which("iarbuild.exe")

    # 若仍未找到,尝试常见安装位置,或者手动写入安装位置
    if not iarbuild_path:
        iarbuild_path_list = [
            r"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe",
            r"C:\Program Files (x86)\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe",
        ]
        for path in iarbuild_path_list:
            if os.path.exists(path):
                iarbuild_path = path
                break

    return iarbuild_path


def build_iar_project(workspace_path: str, config_name: str = "Debug") -> bool:
    """
    功能:使用IarBuild工具编译IAR工程
    参数:
        workspace_path: IAR工程文件(.ewp)的路径
        config_name: 编译配置名称(如"Debug"或"Release")
    返回:
        True: 成功
        False: 失败
    """
    iarbuild_exe_path = find_iarbuild_path()
    if iarbuild_exe_path is None:
        print("[ERROR] 未找到iarbuild.exe，请安装iar软件")
        return False

    # 检查工程文件是否存在且有效
    prj_path = os.path.abspath(workspace_path)  # 转换为绝对路径
    if not os.path.exists(prj_path):
        print(f"IAR工程不存在: {workspace_path}")
        return False

    # 构建命令
    command_clean = [
        str(iarbuild_exe_path),
        str(prj_path),
        "-clean",
        config_name,
        "-log",
        "warnings",
    ]

    command_build = [
        str(iarbuild_exe_path),
        str(prj_path),
        "-build",
        config_name,
        "-log",
        "all",  # 输出详细日志
    ]

    try:
        # 执行命令并捕获输出
        subprocess.run(
            command_clean,  # 先clean
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="gbk",  # 适配Windows中文输出
            timeout=60,  # 超时时间,s
        )
        subprocess.run(
            command_build,  # 再build
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="gbk",  # 适配Windows中文输出
            timeout=600,  # 超时时间,s
        )

        return True
    except subprocess.CalledProcessError as e:
        print("编译失败！")
        print(f"返回码: {e.returncode}")
        print(f"错误详情:\n{e.stdout}")
        return False
    except subprocess.TimeoutExpired as e:
        print("编译超时！")
        print(f"错误详情: {e}")
        return False
    except Exception as e:
        print("发生未知错误！")
        print(f"错误详情: {str(e)}")
        return False


def find_UV4_path():
    """
    功能:查找UV4.exe安装路径
    参数:无
    返回:UV4.exe路径,没找到就返回None
    """
    UV4_path = None
    # 从环境变量获取IAR_PATH
    UV4_path = os.environ.get("keil_exe_path")

    # 若环境变量未设置,从PATH搜索
    if not UV4_path:
        UV4_path = shutil.which("UV4.exe")

    # 若仍未找到,尝试常见安装位置,或者手动写入安装位置
    if not UV4_path:
        UV4_path_list = [
            r"C:\Keil_v5\UV4\UV4.exe",  # Keil MDK5默认安装位置
            r"D:\SoftWare\Keil_v5\UV4\UV4.exe",
        ]
        for path in UV4_path_list:
            if os.path.exists(path):
                UV4_path = path
                break

    return UV4_path


def build_keil_project(uvprojx_path: str, target_name: str = None) -> bool:  # type: ignore
    """
    功能:使用UV4.exe工具编译keil uVision工程
    参数:
        uvprojx_path: keil工程文件(.uvprojx)的路径
        target_name: 编译目标名称（可选），对应 Keil 项目中的 Targets,如果工程中只有一个目标可以省略此参数（默认为None）
    返回:
        True: 成功
        False: 失败
    备注:命令行格式为“UV4 〚command〛 〚projectfile〛 〚options〛”，具体描述参考：https://developer.arm.com/documentation/101407/0542/Command-Line
    """
    keil_exe_path = find_UV4_path()
    if keil_exe_path is None:
        print("[ERROR] 未找到UV4.exe，请安装keil MDK软件")

    # 检查工程文件
    project_file = Path(uvprojx_path).resolve()
    if not project_file.exists():
        print(f"Keil工程不存在: {uvprojx_path}")
        return False

    # 构建命令参数
    command_base = [
        keil_exe_path,
        "-cr",  # clean后再build项目
        str(project_file),
        "-j0",  # 隐藏μ Vision GUI
    ]
    # 添加目标名称参数（如果指定）
    if target_name:
        command_base.extend(["-t", target_name])

    # 执行编译过程
    try:
        subprocess.run(
            command_base,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="gbk",  # Keil输出通常是GBK编码（中文系统）
            timeout=600,  # 超时时间,s
        )

        return True
    except subprocess.CalledProcessError as e:
        print("编译失败！")
        print(f"返回码: {e.returncode}")
        print(f"执行命令: {' '.join(command_base)}")
        return False
    except subprocess.TimeoutExpired as e:
        print("编译超时！")
        print(f"错误详情: {e}")
        return False
    except Exception as e:
        print("发生未知错误！")
        print(f"错误详情: {str(e)}")
        return False


def mcu_version_modify() -> bool:
    """
    功能:修改MCU APP和BOOT的版本信息
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("1、开始修改MCU APP和BOOT的版本信息")
    version_file_path = script_dir + r"Version_info.txt"
    if not os.path.exists(version_file_path):
        print(f"找不到输入文件: {version_file_path}")
        return False

    boot_version_file_path = (
        parent_dir
        + r"05_SourceCode\IKM_MCU\F515_IKM_MCU_BOOT\Src\CDD_BSP\Sys_Config\Version.c"
    )
    if not os.path.exists(boot_version_file_path):
        print(f"找不到输入文件: {boot_version_file_path}")
        return False

    app_version_file_path = (
        parent_dir
        + r"05_SourceCode\IKM_MCU\F515_IKM_MCU\Src\CDD_ABS\SysVersion\Version_Cfg.c"
    )
    if not os.path.exists(app_version_file_path):
        print(f"找不到输入文件: {app_version_file_path}")
        return False

    # 待修改的宏定义,共11个
    macros_to_update = {
        # "BOOTLOADER_VERSION": boot_version_file_path,#暂时不修改boot
        "SUPP_SOFT_VERSION": app_version_file_path,
        "SW_VER_YEAR": app_version_file_path,
        "SW_VER_MONTH": app_version_file_path,
        "SW_VER_DATE": app_version_file_path,
        "SYS_SUPP_IDENDIFIER_ID": app_version_file_path,
        "REVISE_NUM": app_version_file_path,
        "VEH_MANU_SPAR_PART_NUM": app_version_file_path,
        "ECU_NAME": app_version_file_path,
        "VEH_SUPP_HARD_VERSION": app_version_file_path,
        "SYS_SUPP_SOFT_VERSION": app_version_file_path,
    }

    # 批量更新宏定义
    for macro_name, target_file in macros_to_update.items():
        new_value = get_macro_all_in_file(version_file_path, macro_name)
        if not modify_macro_in_file(target_file, macro_name, new_value):
            print(
                f"更新 {macro_name} => {new_value} 到 {os.path.basename(target_file)}失败！"
            )
            return False  # 失败时终止
        # else:
        # print(f"已更新 {macro_name} => {new_value} 到 {os.path.basename(target_file)}")

    return True


def mcu_build() -> bool:
    """
    功能:编译MCU APP和BOOT的工程
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("2、开始编译MCU的工程")
    projects = [
        {
            "name": "MCU_BOOT",
            "path": parent_dir
            + r"05_SourceCode\IKM_MCU\F515_IKM_MCU_BOOT\IAR\DigiKey_MB_B.ewp",
            "config": "Debug",
        },
        {
            "name": "MCU_APP",
            "path": parent_dir
            + r"05_SourceCode\IKM_MCU\F515_IKM_MCU\Compile\DigiKey_AC7840X.ewp",
            "config": "Debug",
        },
    ]

    for project in projects:
        print(
            f"{'=' * 10} [{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] {project['name']}工程编译开始 {'=' * 10}"
        )
        print(f"配置: {project['config']}")
        ret = build_iar_project(project["path"], project["config"])
        print(
            f"{'=' * 10} [{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] {project['name']}工程编译结束 {'=' * 10}"
        )

        if ret:
            print("编译成功！")
        else:
            print("编译失败！")
            return False

    return True


def mcu_program_cmac() -> bool:
    """
    功能:操作编译生成的程序文件,先复制到相应路径,再调用s2_camc.py脚本签名
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("3、开始给MCU APP程序签名")
    try:
        # copy mcu app代码到s2_cmac_20250407\source文件夹
        mcu_app_src_path = (
            parent_dir + r"05_SourceCode\IKM_MCU\F515_IKM_MCU\Compile\Debug\Exe"
        )
        mcu_app_dest_path = script_dir + r"s2_cmac_20250407\source"
        if not file_clear(mcu_app_dest_path):  # 先清空文件夹,再复制
            print(f"清空: {mcu_app_dest_path}失败")
            return False
        if not file_clear(script_dir + r"s2_cmac_20250407\output"):
            print(f"清空: {script_dir + r's2_cmac_20250407\output'}失败！")
            return False
        if not file_copy(mcu_app_src_path, mcu_app_dest_path, "DigiKey_AC7840X.s19"):
            print("复制DigiKey_AC7840X.s19到{mcu_app_dest_path}失败！")
            return False

        # 调用s2_camc.py脚本签名
        script_path = script_dir + r"s2_cmac_20250407\s2_cmac.py"
        if not os.path.exists(script_path):
            print(f"未找到脚本: {script_path}")
            return False

        result = subprocess.run(
            [
                sys.executable,
                script_path,
                script_dir + r"source",
                script_dir + r"output",
            ],  # 传递参数
            capture_output=True,
            text=True,
            cwd=script_dir + r"s2_cmac_20250407",  # 设置工作目录为s2_cmac.py路径
        )

        if result.returncode == 0:
            # print("s2_cmac.py 执行成功")
            # 将生成的签名文件,复制到input文件夹中
            mcu_app_mac_src_path = script_dir + r"s2_cmac_20250407\output"
            mcu_app_mac_dest_path = script_dir + r"input"
            if not file_copy(
                mcu_app_mac_src_path, mcu_app_mac_dest_path, "DigiKey_AC7840X_cmac.s19"
            ):
                print("复制DigiKey_AC7840X_cmac.s19失败！")
                return False
            else:
                return True
        else:
            print(f"s2_cmac.py 执行失败,返回代码: {result.returncode}")
            if result.stderr:
                print(f"错误输出:{result.stderr}")
            return False

    except Exception as e:
        print(f"执行过程中发生异常: {str(e)}")
        return False


def mcu_merge_file_hexview(dest_path) -> bool:
    """
    功能:使用HexView合并两个文件,在特定区域填充0x5A,并导出为新的hex文件
    参数:
        dest_path (str): 输出文件路径(.hex)
    返回:
        True: 成功
        False: 失败
    """
    hexview_path = script_dir + r"s2_cmac_20250407\HexView\hexview.exe"
    file_1_path = (
        parent_dir
        + r"05_SourceCode\IKM_MCU\F515_IKM_MCU_BOOT\IAR\Debug\Exe\DigiKey_MB_B.hex"
    )
    file_2_path = script_dir + r"input\DigiKey_AC7840X_cmac.s19"

    if not os.path.exists(hexview_path):
        print(f"找不到可执行文件: {hexview_path}")
        return False
    if not os.path.exists(file_1_path):
        print(f"找不到输入文件: {file_1_path}")
        return False
    if not os.path.exists(file_2_path):
        print(f"找不到输入文件: {file_2_path}")
        return False

    # 构造指令
    command = (
        f"/s "  # 静默模式
        f"/MT:{file_1_path};0x00000000+{file_2_path};0x00000000 "  # 合并两个文件
        f"/FR:0x0001F800-0x0001F803 "  # 定义区域
        f"/FP:0x5A "  # 设置填充值
        f"/XI:32 "  # 导出为Intel-Hex,每行32字节（自动选择记录类型,Rectype=0）
        f"-o {dest_path}"  # 输出路径,改为-o方式
    )

    # 执行HexView命令
    try:
        subprocess.run(
            [hexview_path] + command.split(),  # 将命令拆分为参数列表
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=30,  # 设置超时时间,s
        )
        # print(f"HexView执行成功,输出文件: {dest_path}")
        return True
    except subprocess.CalledProcessError as e:
        print(f"命令执行失败: {e.stderr}")
        # 打印完整的命令行,便于调试
        print(f"执行的命令: {' '.join([hexview_path] + command.split())}")
        return False
    except subprocess.TimeoutExpired:
        print("HexView执行超时,请检查文件大小和复杂度")
        return False
    except Exception as e:
        print(f"发生未知错误: {str(e)}")
        return False


def ble_version_modify() -> bool:
    """
    功能:修改BLE APP和BOOT的版本信息
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("4、开始修改BLE APP和BOOT的版本信息")
    version_file_path = script_dir + r"Version_info.txt"
    if not os.path.exists(version_file_path):
        print(f"找不到输入文件: {version_file_path}")
        return False

    ble_version_file_path = (
        parent_dir + r"05_SourceCode\IKM_BLE\F515_IKM_BLE\src\flash_operation.h"
    )
    if not os.path.exists(version_file_path):
        print(f"找不到输入文件: {version_file_path}")
        return False

    # 待修改的宏定义,共11个
    macros_to_update = {
        "BLE_SUPP_SOFT_VERSION": ble_version_file_path,
        "BLE_PROCOL_STACH_VERSION": ble_version_file_path,
    }

    # 批量更新宏定义
    for macro_name, target_file in macros_to_update.items():
        new_value = get_macro_all_in_file(version_file_path, macro_name)
        if not modify_macro_in_file(target_file, macro_name, new_value):
            print(
                f"更新 {macro_name} => {new_value} 到 {os.path.basename(target_file)}失败！"
            )
            return False  # 失败时终止
    return True


# ble app工程路径：F515\05_SourceCode\IKM_BLE\F515_IKM_BLE\DigiKey_Master_V2.uvprojx
# ble app生成代码路径：F515\05_SourceCode\IKM_BLE\F515_IKM_BLE\output\DigiKey_Master_V2.hex
# ble boot工程路径：F515\05_SourceCode\IKM_BLE\F515_IKM_BLE_BOOT\F2A_boot_loader.uvprojx
# ble boot生成代码路径：F515\05_SourceCode\IKM_BLE\F515_IKM_BLE_BOOT\output\F2A_boot_loader.hex
def ble_build() -> bool:
    """
    功能:编译BLE APP和BOOT的工程
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("5、开始编译BLE的工程")
    projects = [
        {
            "name": "BLE_BOOT",
            "path": parent_dir
            + r"05_SourceCode\IKM_BLE\F515_IKM_BLE_BOOT\F2A_boot_loader.uvprojx",
            "target_name": None,
        },
        {
            "name": "BLE_APP",
            "path": parent_dir
            + r"05_SourceCode\IKM_BLE\F515_IKM_BLE\DigiKey_Master_V2.uvprojx",
            "target_name": None,
        },
        {
            "name": "BLE_BQB",
            "path": parent_dir + r"05_SourceCode\IKM_BLE\F515_BQB\F515_BQB.uvprojx",
            "target_name": None,
        },
    ]

    for project in projects:
        print(
            f"{'=' * 10} [{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] 编译开始 {project['name']} 工程 {'=' * 10}"
        )
        print(f"配置: {project['target_name']}")
        ret = build_keil_project(project["path"])
        print(
            f"{'=' * 10} [{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] 编译结束 {project['name']} 工程 {'=' * 10}"
        )

        if ret:
            print("编译成功！")
        else:
            print("编译失败！")
            return False

    return True


def ble_program_cmac() -> bool:
    """
    功能:操作编译生成的程序文件,先复制到相应路径,再调用Tool.exe脚本签名
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("6、开始给BLE APP程序签名")
    # copy ble app代码到tool文件夹
    ble_app_src_path = parent_dir + r"05_SourceCode\IKM_BLE\F515_IKM_BLE"
    ble_app_dest_path = parent_dir + r"05_SourceCode\Tools\BLE_F515_app_Fill_Tool\tool"

    if not file_copy(ble_app_src_path, ble_app_dest_path, "DigiKey_Master_V2.bin"):
        print("复制DigiKey_Master_V2.bin到{ble_app_dest_path}失败！")
        return False

    if os.path.exists(ble_app_dest_path + r"\ble_app.bin"):
        file_delete(ble_app_dest_path, "ble_app.bin")

    if not file_rename(ble_app_dest_path, "DigiKey_Master_V2.bin", "ble_app.bin"):
        print("重命名{ble_app_dest_path}的DigiKey_Master_V2.bin失败！")
        return False

    # 调用Tool.exe脚本签名
    script_path = (
        parent_dir + r"05_SourceCode\Tools\BLE_F515_app_Fill_Tool\tool\app_fill_tool.exe"
    )
    if not os.path.exists(script_path):
        print(f"未找到脚本: {script_path}")
        return False

    try:
        # 执行命令
        result = subprocess.run(
            [script_path],
            capture_output=True,
            text=True,
            cwd=ble_app_dest_path,  # 设置工作目录为tool.exe路径
        )

        # 检查返回码
        if result.returncode == 0:
            # print(f"成功执行: {script_path}")
            # 将生成的签名文件,复制到input文件夹中
            ble_app_mac_src_path = ble_app_dest_path
            ble_app_mac_dest_path = script_dir + r"input"
            if not file_copy(
                ble_app_mac_src_path, ble_app_mac_dest_path, "out_ble_app.bin"
            ):
                print("复制DigiKey_AC7840X_cmac.s19失败！")
                return False
            else:
                return True
        else:
            print(f"Tool.exe执行失败，返回码: {result.returncode}")
            if result.stderr:
                print(f"标准错误:\n{result.stderr}")
            return False

    except Exception as e:
        print(f"执行exe时发生错误: {e}")
        return False


def ble_merge_file_jflash(dest_path) -> bool:
    """
    功能:使用jflash合并5个文件,并导出为新的hex文件
    参数:
        dest_path (str): 输出文件路径(.hex)
    返回:
        True: 成功
        False: 失败
    """
    # 查找JFlash.exe路径
    jflash_exe_path = find_jflash_path()
    if jflash_exe_path is None:
        print("[ERROR] 未找到JFlash.exe，请安装SEGGER J-Flash软件")
        return False
        
    # ble app签名bin程序生成ble_app.hex和ble_app_back.hex
    convert_bin_with_jflash(
        bin_file=script_dir + r"input\out_ble_app.bin",
        base_address=0x2C000,
        output_format="hex",
        output_file=script_dir + r"input\ble_app.hex",
    )

    convert_bin_with_jflash(
        bin_file=script_dir + r"input\out_ble_app.bin",
        base_address=0x54000,
        output_format="hex",
        output_file=script_dir + r"input\ble_app_back.hex",
    )

    file_1_path = (
        parent_dir
        + r"05_SourceCode\IKM_BLE\F515_IKM_BLE\sdk\bundles\exp\ING9188xx\platform_entry.hex"
    )
    file_2_path = (
        parent_dir
        + r"05_SourceCode\IKM_BLE\F515_IKM_BLE_BOOT\output\F2A_boot_loader.hex"
    )
    file_3_path = script_dir + r"input\ble_app.hex"
    file_4_path = parent_dir + r"05_SourceCode\IKM_BLE\F515_BQB\output\F515_BQB.hex"
    file_5_path = script_dir + r"input\ble_app_back.hex"

    if not os.path.exists(file_1_path):
        print(f"找不到输入文件: {file_1_path}")
        return False
    if not os.path.exists(file_2_path):
        print(f"找不到输入文件: {file_2_path}")
        return False
    if not os.path.exists(file_3_path):
        print(f"找不到输入文件: {file_3_path}")
        return False
    if not os.path.exists(file_4_path):
        print(f"找不到输入文件: {file_4_path}")
        return False
    if not os.path.exists(file_5_path):
        print(f"找不到输入文件: {file_5_path}")
        return False
    
    try:
        # 构造指令
        cmd = [
            f'"{jflash_exe_path}"',  # 双引号包裹路径处理空格
            # f'-openprj"{project_file}"', # 打开某个指定的.jflash的工程文件，这里我们只需要转换文件格式，不需要connect芯片去烧录，所以不指定，这样运行时会打开一个默认项目，兼容不同的芯片
            f'-open"{file_1_path}"',  # 逗号分隔地址
            f'-merge"{file_2_path}"',  # 合并第2个文件
            f'-merge"{file_3_path}"',  # 合并第3个文件
            f'-merge"{file_4_path}"',  # 合并第4个文件
            f'-merge"{file_5_path}"',  # 合并第5个文件
            f'-saveas"{dest_path}"',
            "-hide",  # 隐藏GUI界面
            "-exit",  # 退出 J-Flash
        ]
        full_cmd = " ".join(cmd)
        
        # 执行命令
        result = subprocess.run(
            full_cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=30,  # 30秒超时
        )
        
        # 验证结果
        if os.path.isfile(dest_path):
            # print(f"[SUCCESS] 转换成功! 输出文件: {dest_path}")
            return True
        else:
            print("[ERROR] 转换失败，输出文件未生成")
            print(f"[CMD] 执行命令: {full_cmd}")
            # 错误诊断
            if "could not open file" in result.stdout:
                print("[DIAG] 无法打开输入文件")
            elif "Unsupported file format" in result.stdout:
                print("[DIAG] 不支持的文件格式")
            return False

    except subprocess.TimeoutExpired:
        print("[ERROR] 命令执行超时")
        return False
    except Exception as e:
        print(f"[ERROR] 执行命令时出错: {e}")
        return False


def factory_file_output() -> bool:
    """
    功能:输出工厂刷写包
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("7、开始打包factory程序")

    # 合并mcu的boot和app(camc签名过的)
    if not mcu_merge_file_hexview(script_dir + r"output\factory_mcu_boot_and_app.hex"):
        print("factory_mcu_boot_and_app.hex程序打包失败！")
        return False

    # 合并ble的boot和app(crc32签名过的)
    if not ble_merge_file_jflash(script_dir + r"output\factory_ble_boot_app_bqb.hex"):
        print("factory_ble_boot_app_bqb.hex程序打包失败！")
        return False

    # 将factory_ble_boot_app_bqb.hex和factory_mcu_boot_and_app打包成一个压缩包
    macros = {  # 待获取的宏信息
        "SUPP_SOFT_VERSION": "",
        "SW_VER_YEAR": "",
        "SW_VER_MONTH": "",
        "SW_VER_DATE": "",
        "SYS_SUPP_IDENDIFIER_ID": "",
        "REVISE_NUM": "",
    }

    for macro_name in macros:  # 依次提取宏值
        value = get_macro_in_file(script_dir + r"Version_info.txt", macro_name)
        if value:
            macros[macro_name] = value
        else:
            print(f"警告: 未找到宏定义 {macro_name}")
            return False

    zip_name = (  # 拼接zip文件名
        "factory_BLE_"
        + macros["SUPP_SOFT_VERSION"]
        + "_"
        + macros["SW_VER_YEAR"]
        + macros["SW_VER_MONTH"]
        + macros["SW_VER_DATE"]
        + "_"
        + macros["SYS_SUPP_IDENDIFIER_ID"]
        + "_"
        + macros["REVISE_NUM"]
        + ".zip"
    )
    if not file_compression_zipfile(
        files=[
            script_dir + r"output\factory_mcu_boot_and_app.hex",
            script_dir + r"output\factory_ble_boot_app_bqb.hex",
        ],
        output_zip=os.path.join(script_dir, "output", zip_name),
        compression_mode="store",  # 不压缩、不加密
    ):
        print("打包zip失败！")
        return False
    else:
        return True

def ota_file_output_mcu() -> bool:
    """
    功能:输出OTA素材包(用于生成OTA升级包)
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("8、开始打包ota mcu程序")

    # 将DigiKey_AC7840X_cmac.s19复制到OTA素材包内,并重命名2_BLE_APP_0MCU_B01.s19
    if not file_copy(
        script_dir + r"input",
        script_dir + r"output\OTA素材包",
        "DigiKey_AC7840X_cmac.s19",
    ):
        print("复制DigiKey_AC7840X_cmac.s19失败！")
        return False

    if os.path.exists(  # 源文件存在就先删除
        script_dir + r"output\OTA素材包\2_BLE_APP_0MCU_B01.s19"
    ):
        file_delete(script_dir + r"output\OTA素材包", "2_BLE_APP_0MCU_B01.s19")

    if not file_rename(
        script_dir + r"output\OTA素材包",
        "DigiKey_AC7840X_cmac.s19",
        "2_BLE_APP_0MCU_B01.s19",
    ):
        print("重命名2_BLE_APP_0MCU_B01.s19失败！")
        return False
    else:
        return True

def ota_file_output_ble() -> bool:
    """
    功能:输出BLE OTA素材包(用于生成OTA升级包)
    参数:无
    返回:
        True: 成功
        False: 失败
    """
    print("9、开始打包ota ble程序")

    # copy ble app、协议栈、bqb三个bin文件代码到tool文件夹
    ble_app_src_path = parent_dir + r"05_SourceCode\IKM_BLE\F515_IKM_BLE"
    ble_bqb_src_path = parent_dir + r"05_SourceCode\IKM_BLE\F515_BQB"
    ble_platform_src_path = parent_dir + r"05_SourceCode\IKM_BLE\F515_IKM_BLE\sdk\bundles\exp\ING9188xx"
    ble_app_dest_path = parent_dir + r"05_SourceCode\Tools\BLE_F515_app_Fill_Tool\tool"
    if not file_copy(ble_app_src_path, ble_app_dest_path, "DigiKey_Master_V2.bin"):
        print("复制DigiKey_Master_V2.bin到{ble_app_dest_path}失败！")
        return False
        
    # 存在就删掉
    if os.path.exists(ble_app_dest_path + r"\ble_app.bin"):
        file_delete(ble_app_dest_path, "ble_app.bin")
        
    # 重命名
    if not file_rename(ble_app_dest_path, "DigiKey_Master_V2.bin", "ble_app.bin"):
        print("重命名{ble_app_dest_path}的DigiKey_Master_V2.bin失败！")
        return False
        
    if not file_copy(ble_bqb_src_path, ble_app_dest_path, "F515_BQB.bin"):
        print("F515_BQB.bin到{ble_app_dest_path}失败！")
        return False
        
    if not file_copy(ble_platform_src_path, ble_app_dest_path, "platform.bin"):
        print("platform.bin到{ble_app_dest_path}失败！")
        return False
        
    # 调用platform_up_tool.exe脚本签名
    script_path = (
        parent_dir + r"05_SourceCode\Tools\BLE_F515_app_Fill_Tool\tool\platform_up_tool.exe"
    )
    if not os.path.exists(script_path):
        print(f"未找到脚本: {script_path}")
        return False
        
    try:
        # 执行命令
        result = subprocess.run(
            [script_path],
            capture_output=True,
            text=True,
            cwd=ble_app_dest_path,  # 设置工作目录为tool.exe路径
        )

        # 检查返回码
        if result.returncode == 0:
            # print(f"成功执行: {script_path}")
            if os.path.exists(  # 源文件存在就先删除
                    parent_dir + r"05_SourceCode\Tools\BLE_F515_app_Fill_Tool\tool\out_ble_app_ota.bin"
                ):
                    file_delete(parent_dir + r"05_SourceCode\Tools\BLE_F515_app_Fill_Tool\tool", "out_ble_app_ota.bin")
                    
            if not file_rename(ble_app_dest_path, "out_ble_app.bin", "out_ble_app_ota.bin"):
                print("重命名{ble_app_dest_path}out_ble_app.bin失败！")
                return False
        
            # 将生成的签名文件,复制到input文件夹中
            ble_app_mac_src_path = ble_app_dest_path
            ble_app_mac_dest_path = script_dir + r"input"
            if not file_copy(
                ble_app_mac_src_path, ble_app_mac_dest_path, "out_ble_app_ota.bin"
            ):
                print("复制out_ble_app_ota.bin失败！")
                return False
            else:
                # 将ble的out_ble_app.bin签名文件，2_BLE_APP_1BT_B01.s19文件，地址从0x700000开始，然后再将2_BLE_APP_1BT_B01.s19文件复制到OTA素材包内
                if os.path.exists(  # 源文件存在就先删除
                    script_dir + r"output\OTA素材包\2_BLE_APP_1BT_B01.s19"
                ):
                    file_delete(script_dir + r"output\OTA素材包", "2_BLE_APP_1BT_B01.s19")

                if not convert_bin_with_jflash(
                    bin_file=script_dir + r"input\out_ble_app_ota.bin",
                    base_address=0x700000,
                    output_format="s19",
                    output_file=script_dir + r"output\OTA素材包\2_BLE_APP_1BT_B01.s19",
                ):
                    print("生成2_BLE_APP_1BT_B01_test.s19失败！")
                    return False
                else:
                    return True
        else:
            print(f"Tool.exe执行失败，返回码: {result.returncode}")
            if result.stderr:
                print(f"标准错误:\n{result.stderr}")
            return False

    except Exception as e:
        print(f"执行exe时发生错误: {e}")
        return False


def main():
    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] Run Start！")

    result_value = True
    
    if result_value is True:
        result_value = mcu_version_modify()  # 修改mcu的版本信息

    if result_value is True:
        result_value = mcu_build()  # 编译mcu工程

    if result_value is True:
        result_value = mcu_program_cmac()  # 给mcu app程序签名

    if result_value is True:
        result_value = ble_version_modify()  # 修改ble的版本信息

    if result_value is True:
        result_value = ble_build()  # 编译ble工程

    if result_value is True:
        result_value = ble_program_cmac()  # 给ble app程序签名
    
    if result_value is True:
        result_value = factory_file_output()  # 生成工厂刷写压缩包

    if result_value is True:
        result_value = ota_file_output_mcu()  # 生成OTA包素材文件
        
    if result_value is True:
        result_value = ota_file_output_ble()  # 生成OTA包素材文件
    
    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] Run End！")

    # 退出python解释器
    if result_value is True:
        return True
    else:
        input("打包失败,请查看日志信息检查原因！！！")
        return False


if __name__ == "__main__":
    main()
