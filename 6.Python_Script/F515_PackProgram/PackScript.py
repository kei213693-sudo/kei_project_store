import re # 导入正则表达式模块
import os # 导入操作系统接口
import sys
import argparse
import subprocess # 导入子进程（即调用外部程序）管理模块
import logging
import winreg  # 导入Windows专用注册表交互模块
import shutil


from typing import Tuple
from datetime import datetime


# 获取脚本所在目录
script_dir = os.path.dirname(os.path.abspath(__file__))
# 获取脚本上一级目录
parent_dir = os.path.dirname(script_dir)


# 确保目录路径以 \ 结尾
if not script_dir.endswith(os.sep):
    script_dir += os.sep 
if not parent_dir.endswith(os.sep):
    parent_dir += os.sep


"""
函数:Get_macro_in_file(file_path, macro_name)
功能:获取该文件某个宏定义的宏名后的所有内容
参数:
    file_path (str): 文件路径
    macro_name (str): 要获取的宏名称
"""
def Get_macro_in_file(file_path, macro_name):
    # 检查文件是否存在
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"文件不存在: {file_path}")
    # 读取文件内容
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()


    # 正则表达式模式：精确匹配#define行，捕获宏名后的所有内容
    pattern = fr'^\s*#\s*define\s+{re.escape(macro_name)}\b(.*)$'
    # 使用多行模式进行匹配，加上re.IGNORECASE可以忽略大小写
    match = re.search(pattern, content, re.MULTILINE)


    if match:
        # 获取完整的匹配行
        full_line = match.group(0)
        # 计算宏名结束位置（包括所有空格）
        define_part = f"#define {macro_name}"
        macro_start = full_line.index(define_part) + len(define_part)
        # 提取宏名后的所有内容（包括前面的空格）
        value_part = full_line[macro_start:].rstrip('\n')
        return value_part
    else:
        print(f"未找到宏定义: {macro_name}")
        return None


"""
函数:Modify_macro_in_file(file_path, macro_name, new_value)
功能:修改C或H文件中的宏定义值
参数:
    file_path (str): 文件路径
    macro_name (str): 要修改的宏名称
    new_value (str): 新的宏值,从宏名称后面包括空格和注释
"""
def Modify_macro_in_file(file_path, macro_name, new_value):
    # 检查文件是否存在
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"文件不存在: {file_path}")
    # 读取文件内容
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 定义正则表达式模式匹配宏定义
    pattern = fr'^\s*#\s*define\s+{re.escape(macro_name)}\s+(\S*)(.*)$'
    # 使用多行模式进行匹配，加上re.IGNORECASE可以忽略大小写
    match = re.search(pattern, content, re.MULTILINE)


    if match:
        # 构建新的宏定义行
        new_line = f"#define {macro_name}{new_value}"
        # 替换内容
        modified_content = re.sub(pattern, new_line, content, flags=re.MULTILINE | re.IGNORECASE)
        
        # 写回文件
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(modified_content)
        # print(f"成功将宏 {macro_name} 的值修改为 {new_string_value}")
    else:
        print(f"未找到宏定义: {macro_name}")


"""
函数:Modify_macro_in_file(file_path, macro_name, new_value)
功能:修改MCU APP和BOOT的版本信息
参数:
"""
def Mcu_version_modify():
    # 定义文件路径,后续需要修改为相对路径
    version_file_path = script_dir + r"Version_info.txt"
    if not os.path.exists(version_file_path):
        raise FileNotFoundError(f"文件Version_info.txt不存在: {version_file_path}")
    
    boot_version_file_path = parent_dir + r"05_SourceCode\IKM_MCU\F515_IKM_MCU_BOOT\Src\CDD_BSP\Sys_Config\Version.c"
    if not os.path.exists(boot_version_file_path):
        raise FileNotFoundError(f"文件Version.c不存在: {boot_version_file_path}")
    
    app_version_file_path = parent_dir + r"05_SourceCode\IKM_MCU\F515_IKM_MCU\Src\CDD_ABS\SysVersion\Version_Cfg.c"
    if not os.path.exists(app_version_file_path):
        raise FileNotFoundError(f"文件Version_Cfg.c不存在: {app_version_file_path}")


    # 待修改的宏定义，共11个
    macros_to_update = {
        "BOOTLOADER_VERSION": boot_version_file_path,
        "SUPP_SOFT_VERSION": app_version_file_path,
        "SW_VER_YEAR": app_version_file_path,
        "SW_VER_MONTH": app_version_file_path,
        "SW_VER_DATE": app_version_file_path,
        "SYS_SUPP_IDENDIFIER_ID": app_version_file_path,
        "REVISE_NUM": app_version_file_path,
        "VEH_MANU_SPAR_PART_NUM": app_version_file_path,
        "ECU_NAME": app_version_file_path,
        "VEH_SUPP_HARD_VERSION": app_version_file_path,
        "SYS_SUPP_SOFT_VERSION": app_version_file_path
    }


    # 批量更新宏定义
    for macro_name, target_file in macros_to_update.items():
        new_value = Get_macro_in_file(version_file_path, macro_name)
        Modify_macro_in_file(target_file, macro_name, new_value)
        # print(f"已更新 {macro_name} => {new_value} 到 {os.path.basename(target_file)}")
    
"""
函数:Build_iar_project(workspace_path, config_name="Debug", iarbuild_path="IarBuild")
功能:使用IarBuild工具编译IAR工程
参数:
    workspace_path: IAR工程文件(.ewp)的路径
    config_name: 编译配置名称(如"Debug"或"Release")
"""
def Build_iar_project(workspace_path: str, config_name: str = "Debug") -> Tuple[int, str]:
    # 定位 IarBuild.exe（需根据实际安装路径调整）
    # 从注册列表找IAR路径可能要管理员权限，这里直接写绝对路径
    iar_bin_path = r"C:\Program Files\IAR Systems\Embedded Workbench 9.1\common\bin\iarbuild.exe"
    if not os.path.exists(iar_bin_path):
        raise FileNotFoundError(f"iarbuild.exe 未找到: {iar_bin_path}")


    # 检查工程文件是否存在且有效
    prj_path = os.path.abspath(workspace_path)  # 转换为绝对路径
    if not os.path.exists(prj_path):
        raise FileNotFoundError(f"IAR工程不存在: {workspace_path}")
    
    # 构建命令
    command_clean = [
        str(iar_bin_path),
        str(prj_path),
        "-clean",
        config_name,
        "-log", "warnings"  # 输出详细日志
    ]


    command_build = [
        str(iar_bin_path),
        str(prj_path),
        "-build",
        config_name,
        "-log", "all"  # 输出详细日志
    ]


    try:
        # 执行命令并捕获输出
        subprocess.run(
            command_clean, # 先clean
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="gbk",  # 适配Windows中文输出
            timeout=600      # 10分钟超时
        )


        result = subprocess.run(
            command_build, # 再build
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="gbk",  # 适配Windows中文输出
            timeout=600      # 10分钟超时
        )
        return (result.returncode, result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"编译失败，返回码: {e.returncode}")
        print(f"标准输出:\n{e.stdout}")
        return (e.returncode, e.stdout)
    except subprocess.TimeoutExpired as e:
        return (-1, f"编译超时: {e}")
    except Exception as e:
        return (-2, f"未知错误: {str(e)}")
    
"""
函数:Mcu_build():
功能:编译MCU APP和BOOT的工程
参数:
"""
def Mcu_build():
    projects = [
        {
            "name": "MCU_BOOT",
            "path": parent_dir + r"05_SourceCode\IKM_MCU\F515_IKM_MCU_BOOT\IAR\DigiKey_MB_B.ewp",
            "config": "Debug"
        },
        {
            "name": "MCU_APP",
            "path": parent_dir + r"05_SourceCode\IKM_MCU\F515_IKM_MCU\Compile\DigiKey_AC7840X.ewp",
            "config": "Debug"
        }
    ]


    overall_success = True


    for project in projects:
        print(f"{'='*20} 开始编译 {project['name']} 工程 {'='*20}")
        print(f"配置: {project['config']}")
        return_code, stdout = Build_iar_project(project["path"], project["config"])
        
        if return_code == 0:
            print("\033[32m编译成功!\033[0m")  # 绿色文字
        else:
            print("\033[31m编译失败!\033[0m")  # 红色文字
            overall_success = False


        if return_code != 0:
            print(f"返回码: {return_code}")
            print("\033[31m" + "="*50 + "\n错误详情:\n" + stdout + "\033[0m")
    
    return overall_success



"""
函数:File_copy(source_path,dest_path,file_name)
功能:将某一个路径内的文件复制到另外一个路径，文件名称相同就全覆盖
参数:
    source_path (str): 源文件或目录路径
    dest_path (str): 目标目录路径
    file_name (str): 文件名
返回:
    True: 成功
    False: 失败
"""
def File_copy(source_path,dest_path,file_name):
    try:
        # 规范化Windows路径（处理正反斜杠问题）
        source_dir = os.path.normpath(source_path)
        target_dir = os.path.normpath(dest_path)
        src_file = os.path.join(source_dir, file_name)
        dst_file = os.path.join(target_dir, file_name)

        # 验证源文件是否存在
        if not os.path.isfile(src_file):
            raise FileNotFoundError(f"源文件不存在: {src_file}")

        # 创建目标目录（递归创建）
        os.makedirs(target_dir, exist_ok=True)

        # 执行文件复制（强制覆盖）
        shutil.copy2(src_file, dst_file)
        # print(f"成功复制: {src_file} -> {dst_file}")
        return True

    except PermissionError as pe:
        print(f"权限拒绝错误: {pe}")
    except Exception as e:
        print(f"操作失败: {str(e)}")
    return False


"""
函数:File_clear(dest_path):
功能:将目标目录路径下的文件清空
参数:
    dest_path (str): 目标目录路径
返回:
    True: 成功
    False: 失败
"""
def File_clear(dest_path):
    if dest_path in ['/', '\\', 'C:\\', 'D:\\']:
        raise ValueError("禁止删除根目录，这可能导致系统崩溃！")
    if not os.path.exists(dest_path):
        raise FileNotFoundError(f"目标目录不存在: {dest_path}")
    if not os.path.isdir(dest_path):
        raise NotADirectoryError(f"指定路径不是目录: {dest_path}")
    
    dest_path = os.path.abspath(dest_path) # 转换为绝对路径以便调试
    
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
        return True  # 删除成功
    except Exception as e:
        print(f"删除过程中发生错误: {str(e)}")
        return False
    
"""
函数:File_delete(dest_path,file_name)
功能:将目标目录路径下的某个文件删除
参数:
    dest_path (str): 目标目录路径
    file_name (str): 要删除的文件名
返回:
    True: 成功
    False: 失败
"""
def File_delete(dest_path,file_name):
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
        print(f"成功删除文件: {file_path}")
        return True
    except Exception as e:
        print(f"删除失败: {str(e)}")
        return False


"""
函数:ile_rename(dest_path,file_name,new_name)
功能:将目标目录路径下的文件重命名
参数:
    dest_path (str): 目标目录路径
    file_name (str): 原文件名
    new_name (str): 新文件名
返回:
    True: 成功
    False: 失败
"""
def File_rename(dest_path,file_name,new_name):
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
        print(f"成功重命名：{source_path} → {target_path}")
        return True
    except Exception as e:
        print(f"重命名失败：{str(e)}")
        return False


"""
函数:Mcu_program_opt()
功能:操作编译生成的程序文件,先复制到相应路径,再调用s2_camc.py脚本签名
参数:
"""
def Mcu_program_opt():
    # 1、copy mcu boot代码到input文件夹
    mcu_boot_src_path = parent_dir + r"05_SourceCode\IKM_MCU\F515_IKM_MCU_BOOT\IAR\Debug\Exe"
    mcu_boot_dest_path = script_dir + r"input"
    File_clear(mcu_boot_dest_path) # 先清空文件夹，再复制
    File_copy(mcu_boot_src_path,mcu_boot_dest_path,"DigiKey_MB_B.hex")


    # 2、copy mcu app代码到s2_cmac_20250407文件夹
    mcu_app_src_path = parent_dir + r"05_SourceCode\IKM_MCU\F515_IKM_MCU\Compile\Debug\Exe"
    mcu_app_dest_path = script_dir + r"s2_cmac_20250407\source"
    File_clear(mcu_app_dest_path) # 先清空文件夹，再复制
    File_clear(script_dir + r"s2_cmac_20250407\output")
    # input("按回车键继续...")
    File_copy(mcu_app_src_path,mcu_app_dest_path,"DigiKey_AC7840X.s19")


    # 3、调用s2_camc.py脚本给签名
    script_path = script_dir + r"s2_cmac_20250407\s2_cmac.py"
    if not os.path.exists(script_path):
        raise FileNotFoundError(f"未找到脚本: {script_path}")
    
    result = subprocess.run(
        [sys.executable, script_path, script_dir + r"source", script_dir + r"output"],  # 传递参数
        capture_output=True,
        text=True,
        cwd=script_dir + r"s2_cmac_20250407"  # 设置工作目录为s2_cmac.py路径
    )


    if result.returncode == 0:
        print("s2_cmac.py 执行成功")
        # 4、将生产的签名文件，复制到input文件夹中
        mcu_app_mac_src_path = script_dir + r"s2_cmac_20250407\output"
        mcu_app_mac_dest_path = script_dir + r"input"
        File_copy(mcu_app_mac_src_path,mcu_app_mac_dest_path,"DigiKey_AC7840X_cmac.s19")
    else:
        print(f"s2_cmac.py 执行失败，返回代码: {result.returncode}")
        if result.stderr:
            print(f"错误输出:{result.stderr}")
    
"""
函数:Hexview_merge_file(dest_path)
功能:使用HexView合并两个文件,在特定区域填充5A,并导出为新的hex文件
参数:
    dest_path (str): 输出文件路径(.hex)
返回:
    True: 成功
    False: 失败
"""
def Hexview_merge_file(dest_path):


    file_1_path = script_dir + r"input\DigiKey_MB_B.hex"
    file_2_path = script_dir + r"input\DigiKey_AC7840X_cmac.s19"
    hexview_path = script_dir + r"s2_cmac_20250407\HexView\hexview.exe"


    if not os.path.exists(file_1_path):
        raise FileNotFoundError(f"找不到输入文件: {file_1_path}")
    if not os.path.exists(file_2_path):
        raise FileNotFoundError(f"找不到输入文件: {file_2_path}")
    if not os.path.exists(hexview_path):
        raise FileNotFoundError(f"找不到输入文件: {hexview_path}")
    
    # 先合并两个文件
    temp_file = dest_path + ".temp"
    cmd_merge = [
        hexview_path,
        "-i", file_1_path,
        "-i", file_2_path,
        "-o", temp_file,
        "-m"  # 合并选项
    ]
    print(f"执行合并命令: {' '.join(cmd_merge)}")
    try:
        # 执行合并命令
        result = subprocess.run(
            cmd_merge, 
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        print(f"合并命令执行成功: {result.stdout}")
        
        if not os.path.exists(temp_file):
            print("警告: 临时文件未生成,可能HexView命令参数需要调整")
            print(f"HexView标准错误: {result.stderr}")
            return False
        
        # 在特定区域填充5A (0x5A)
        # 地址范围: 0x0001f800-0x0001f803 (共4个字节)
        cmd_fill = [
            hexview_path,
            "-i", temp_file,
            "-o", dest_path,
            "-e", "fill 0x0001f800 0x0001f803 0x5A"  # 填充命令
        ]
        print(f"执行填充命令: {' '.join(cmd_fill)}")
        # 执行填充命令
        result = subprocess.run(
            cmd_fill, 
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        print(f"填充命令执行成功: {result.stdout}")
        
        if os.path.exists(dest_path):
            print(f"合并并填充后的文件已保存到: {dest_path}")
            # 删除临时文件
            File_delete(os.path.dirname(temp_file), os.path.basename(temp_file))
            return True
        else:
            print("警告: 输出文件未生成,可能HexView命令参数需要调整")
            print(f"HexView标准错误: {result.stderr}")
            return False
            
    except subprocess.CalledProcessError as e:
        print(f"HexView执行失败: {e.stderr}")
        return False
    except Exception as e:
        print(f"发生错误: {e}")
        return False
    
def main():
    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] Test Start!")


    #Mcu_version_modify()    # 修改版本信息
    #Mcu_build()         # 编译工程
    #Mcu_program_opt() # 处理程序文件
    Hexview_merge_file(script_dir + r"output\factory_mcu_boot_and_app.hex")


    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]}] Test End!")


if __name__ == "__main__":
    main()
