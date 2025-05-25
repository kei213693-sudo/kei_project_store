"""
文件名称：File_opt
功能：处理文件基础操作，复制、删除、重命名、剪切、清空
作者：lijing
"""
import os
import shutil

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



