"""
file_utils.py Python脚本的文件处理函数组件
已实现接口：
    单个文件的“复制、删除、重命名、剪切”；
    某个文件夹的所有内容清空
    文件的压缩和解压缩；
待实现接口：
    单个文件的加解密
    两个文件特定内容的替换或修改；
    多个文件的批量“复制、删除、重命名、剪切”，判定条件可选“名称前缀、文件类型、修改时间、文本内容”等；
编码格式coding: utf-8 适用于处理中文
"""

import os  # 导入操作系统接口
import shutil  # 主要用于实现文件和目录的复制、移动、删除、归档等操作
import subprocess
import zipfile
from typing import List, Optional


def file_copy(source_path, dest_path, file_name) -> bool:
    """
    功能:将某一个路径内的“某个文件”复制到另外一个路径，文件名称相同就全覆盖
    参数:
        source_path (str): 源文件或目录路径
        dest_path (str): 目标目录路径
        file_name (str): 文件名
    返回:
        True: 成功
        False: 失败
    """
    try:
        # 规范化Windows路径（处理正反斜杠问题）
        source_dir = os.path.normpath(source_path)
        target_dir = os.path.normpath(dest_path)
        src_file = os.path.join(source_dir, file_name)
        dst_file = os.path.join(target_dir, file_name)

        # 验证源文件是否存在
        if not os.path.isfile(src_file):
            print(f"源文件不存在: {src_file}")
            return False

        # 创建目标目录（递归创建）
        os.makedirs(target_dir, exist_ok=True)

        # 执行文件复制（强制覆盖）
        shutil.copy2(src_file, dst_file)
        # print(f"成功复制: {src_file} -> {dst_file}")
        return True
    except PermissionError as pe:
        print(f"权限拒绝错误: {pe}")
        return False
    except Exception as e:
        print(f"操作失败: {str(e)}")
        return False


def file_delete(dest_path, file_name) -> bool:
    """
    功能:将目标目录路径下的“某个文件”删除
    参数:
        dest_path (str): 目标目录路径
        file_name (str): 要删除的文件名
    返回:
        True: 成功
        False: 失败
    """
    file_path = os.path.join(dest_path, file_name)
    if os.path.isdir(file_path):
        print(f"错误: {file_name} 是目录，无法使用此函数删除")
        return False

    # 检查文件是否存在
    if not os.path.exists(file_path):
        print(f"错误: 文件不存在 - {file_path}")
        return False

    try:
        # 执行文件删除
        os.remove(file_path)
        # print(f"成功删除文件: {file_path}")
        return True
    except Exception as e:
        print(f"删除失败: {str(e)}")
        return False


def file_rename(dest_path, file_name, new_name) -> bool:
    """
    功能:将目标目录路径下的“某个文件”重命名
    参数:
        dest_path (str): 目标目录路径
        file_name (str): 原文件名
        new_name (str): 新文件名
    返回:
        True: 成功
        False: 失败
    """
    source_path = os.path.join(dest_path, file_name)
    target_path = os.path.join(dest_path, new_name)

    # 检查源文件是否存在
    if not os.path.exists(source_path):
        print(f"错误：源文件不存在 - {source_path}")
        return False

    # 检查目标文件是否已存在
    if os.path.exists(target_path):
        print(f"错误：目标文件已存在 - {target_path}")
        return False

    try:
        # 执行文件重命名
        os.rename(source_path, target_path)
        # print(f"成功重命名：{source_path} → {target_path}")
        return True
    except Exception as e:
        print(f"重命名失败：{str(e)}")
        return False


def file_cut(source_path, dest_path, file_name) -> bool:
    """
    功能:将某一个路径内的“某个文件”剪切到另外一个路径，文件名称相同就全覆盖
    参数:
        source_path (str): 源文件或目录路径
        dest_path (str): 目标目录路径
        file_name (str): 文件名
    返回:
        True: 成功
        False: 失败
    """
    try:
        # 规范化Windows路径（处理正反斜杠问题）
        source_dir = os.path.normpath(source_path)
        target_dir = os.path.normpath(dest_path)
        src_file = os.path.join(source_dir, file_name)
        dst_file = os.path.join(target_dir, file_name)

        # 验证源文件是否存在
        if not os.path.isfile(src_file):
            print(f"源文件不存在: {src_file}")
            return False

        # 创建目标目录（递归创建）
        os.makedirs(target_dir, exist_ok=True)

        # 如果目标文件已存在，则先删除
        if os.path.exists(dst_file):
            if not file_delete(target_dir, file_name):
                return False

        # 执行文件移动（剪切）
        shutil.move(src_file, dst_file)
        # print(f"成功移动: {src_file} -> {dst_file}")
        return True
    except PermissionError as pe:
        print(f"权限拒绝错误: {pe}")
        return False
    except Exception as e:
        print(f"操作失败: {str(e)}")
        return False


def file_clear(dest_path) -> bool:
    """
    功能:将目标目录路径下的“所有文件、文件夹”清空
    参数:
        dest_path (str): 目标目录路径
    返回:
        True: 成功
        False: 失败
    """
    if dest_path in ["/", "\\", "C:\\", "D:\\"]:
        print("禁止删除根目录，这可能导致系统崩溃！")
        return False
    if not os.path.exists(dest_path):
        print(f"目标目录不存在: {dest_path}")
        return False
    if not os.path.isdir(dest_path):
        print(f"指定路径不是目录: {dest_path}")
        return False

    dest_path = os.path.abspath(dest_path)  # 转换为绝对路径以便调试

    try:
        # 遍历目录内容
        for item in os.listdir(dest_path):
            item_path = os.path.join(dest_path, item)
            # 删除文件
            if os.path.isfile(item_path):
                os.remove(item_path)
            # 递归删除子目录
            elif os.path.isdir(item_path):
                shutil.rmtree(item_path)
        return True  # 删除成功
    except Exception as e:
        print(f"删除过程中发生错误: {str(e)}")
        return False


def file_compression_zipfile(
    files: list[str], output_zip: str, compression_mode: str = "store"
) -> bool:
    """
    功能：使用Python标准库zipfile将多个文件压缩为ZIP格式
    参数:
        files: 待压缩的文件路径列表
        output_zip: 输出ZIP文件的完整路径
        compression_mode: 压缩级别，可选值：
            "store"   - 仅存储（不压缩）
            "normal"  - 标准压缩
            "high"    - 高压缩率
    返回:
        True: 压缩成功
        False: 压缩失败
    备注:
        1. 基于Python标准库zipfile实现，不支持加密功能
        2. 输出格式固定为ZIP
    """
    missing_files = [f for f in files if not os.path.exists(f)]
    if missing_files:
        print("错误: 以下文件不存在\n" + "\n".join(missing_files))
        return False

    # 压缩模式映射
    COMPRESSION_MODES = {
        "store": (zipfile.ZIP_STORED, None),
        "normal": (zipfile.ZIP_DEFLATED, 6),
        "high": (zipfile.ZIP_DEFLATED, 9),
    }
    compression_mode = compression_mode.lower()
    if compression_mode not in COMPRESSION_MODES:
        print(f"无效压缩模式: {compression_mode}，使用默认值store")
        compression_mode = "store"
    compression_type, compress_level = COMPRESSION_MODES[compression_mode]

    # 准备输出目录
    output_dir = os.path.dirname(output_zip)
    os.makedirs(output_dir, exist_ok=True)

    try:
        with zipfile.ZipFile(
            output_zip, "w", compression=compression_type, compresslevel=compress_level
        ) as zipf:
            for f in files:
                zipf.write(f, os.path.basename(f))
        return True
    except Exception as e:
        print(f"zipfile压缩失败: {e}")
        return False


def file_decompression_zipfile(file_zip_path: str, output_path: str) -> bool:
    """
    功能：使用Python标准库zipfile解压ZIP文件
    参数:
        file_zip_path: 待解压的ZIP文件路径
        output_path: 解压目标路径
    返回:
        True: 解压成功
        False: 解压失败
    备注:
        1. 基于Python标准库zipfile实现，不支持加密文件
        2. 自动创建解压目标目录（如果不存在）
    """
    if not os.path.exists(file_zip_path):
        print(f"错误: ZIP文件不存在 - {file_zip_path}")
        return False

    os.makedirs(output_path, exist_ok=True)

    try:
        with zipfile.ZipFile(file_zip_path) as zipf:
            zipf.extractall(output_path)
        return True
    except RuntimeError as e:
        if "encrypted" in str(e).lower():
            print("错误: ZIP文件已加密，请使用7-Z或其他软件解压并提供密码")
        else:
            print(f"zipfile解压失败: {e}")
        return False


def Find_seven_zip_path() -> Optional[str]:
    """查找7-Zip可执行文件路径"""
    locations = [
        lambda: os.environ.get("SEVEN_ZIP_PATH"),
        lambda: shutil.which("7z.exe"),
        lambda: r"C:\Program Files\7-Zip\7z.exe",
        lambda: r"C:\Program Files (x86)\7-Zip\7z.exe",
    ]

    for loc in locations:
        try:
            path = loc()
            if path and os.path.exists(path):
                return path
        except Exception:
            continue

    return None


def file_compression_7z(
    files: List[str],
    output_path: str,
    compression_mode: str = "store",
    password: str | bytes | None = None,
    archive_format: str = "zip",
) -> bool:
    """
    功能：使用7-Zip将多个文件压缩为指定格式，支持加密和多种压缩格式
    参数:
        files: 待压缩的文件路径列表
        output_file: 输出压缩文件的完整路径
        compression_mode: 压缩级别，可选值：
            "store"   - 仅存储（不压缩）
            "normal"  - 标准压缩
            "high"    - 高压缩率
        password: 加密密码（可选）
        archive_format: 压缩格式，可选值：
            "zip"     - ZIP格式
            "7z"      - 7Z格式（压缩率更高）
            "tar"     - TAR格式（仅打包不压缩）
            "wim"     - WIM格式（Windows映像）
    返回:
        True: 压缩成功
        False: 压缩失败
    备注:加密功能仅在提供密码时启用
    调用例子：
    file_compression_7z(
        files=[
            script_dir + r"output\factory_mcu_boot_and_app.hex",
            script_dir + r"output\factory_ble_boot_app_bqb.hex",
        ],
        output_path=script_dir + r"output\factory_BLE_SW2.03.B_240602_1452_01.zip",
        compression_mode="store",
        password="Aa123456",
        archive_format="7z",
    )
    """
    missing_files = [f for f in files if not os.path.exists(f)]
    if missing_files:
        print("以下文件不存在:\n" + "\n".join(missing_files))
        return False

    # 压缩模式映射
    COMPRESSION_MODES = {
        "store": 0,
        "normal": 5,
        "high": 9,
    }
    compression_mode = compression_mode.lower()
    if compression_mode not in COMPRESSION_MODES:
        print(f"无效压缩模式: {compression_mode}，使用默认值store")
        compression_mode = "store"
    compression_level = COMPRESSION_MODES[compression_mode]

    # 准备输出目录
    output_dir = os.path.dirname(output_path)
    os.makedirs(output_dir, exist_ok=True)

    # 获取7z.exe路径
    seven_zip_path = Find_seven_zip_path()
    if not seven_zip_path:
        print("错误: 未找到7z.exe")
        return False

    # 根据archive_format自动修正扩展名
    FORMAT_EXTENSIONS = {
        "zip": ".zip",
        "7z": ".7z",
        "tar": ".tar",
        "wim": ".wim",
    }
    current_ext = os.path.splitext(output_path)[1].lower()
    target_ext = FORMAT_EXTENSIONS.get(archive_format.lower(), ".zip")
    if current_ext != target_ext:
        output_path = os.path.splitext(output_path)[0] + target_ext
        print(f"注意: 自动将输出文件扩展名修正为 {target_ext}")

    # 构造指令
    cmd = [
        f'"{seven_zip_path}"',  # 7-Zip路径加引号
        "a",
        f"-t{archive_format}",  # 指定格式
        f"-mx={compression_level}",
        f'"{output_path}"',  # 输出路径加引号
    ]

    # 添加密码参数（如果有）
    if password:
        password_str = (
            password.decode("utf-8") if isinstance(password, bytes) else password
        )
        cmd.append(f"-p{password_str}")

    # 添加要压缩的文件
    cmd.extend([f'"{f}"' for f in files])

    try:
        subprocess.run(
            " ".join(cmd),
            shell=True,  # 处理引号和路径中的空格
            capture_output=True,
            text=True,
            check=True,
            timeout=300,
        )
        return True
    except subprocess.CalledProcessError as e:
        print(f"7-Zip压缩错误 (代码 {e.returncode}): {e.stderr.strip()}")
        return False
    except Exception as e:
        print(f"未知错误: {str(e)}")
        return False


def file_decompression_7z(
    file_path: str,
    output_path: str,
    password: str | bytes | None = None,
) -> bool:
    """
    功能：使用7-Zip解压文件，支持多种格式和加密文件
    参数:
        file_path: 待解压的文件路径
        output_path: 解压目标路径
        password: 解压密码（可选，未加密则传None）
    返回:
        True: 解压成功
        False: 解压失败
    备注:无
    调用例子：
    file_decompression_7z(
        file_path=script_dir + r"output\factory_BLE_SW2.03.B_240602_1452_01.zip",
        output_path=script_dir
        + r"output\OTA素材包\factory_BLE_SW2.03.B_240602_1452_01",
        password="Aa123456",
    )
    """
    if not os.path.exists(file_path):
        print(f"错误: 压缩包文件不存在 - {file_path}")
        return False

    # 准备目标目录
    os.makedirs(output_path, exist_ok=True)

    seven_zip_path = Find_seven_zip_path()
    if not seven_zip_path:
        print("错误: 未找到7z.exe")
        return False

    password_str = password.decode("utf-8") if isinstance(password, bytes) else password
    cmd = [
        f'"{seven_zip_path}"',
        "x",
        f'"{file_path}"',
        f'-o"{output_path}"',
        "-y",
    ]

    # 添加密码参数（如果有）
    if password:
        password_str = (
            password.decode("utf-8") if isinstance(password, bytes) else password
        )
        cmd.append(f"-p{password_str}")

    try:
        subprocess.run(
            " ".join(cmd),
            shell=True,
            check=True,
            capture_output=True,
            text=True,
            timeout=300,
        )
        return True
    except subprocess.CalledProcessError as e:
        print(f"7-Zip解压错误 (代码 {e.returncode}): {e.stderr.strip()}")
        return False
    except Exception as e:
        print(f"未知错误: {e}")
        return False
