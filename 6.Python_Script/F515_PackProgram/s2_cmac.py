import logging
import binascii
import re
from Crypto.Hash import CMAC
from Crypto.Cipher import AES
import os
import subprocess

def cmac_aes_128(key, message):
    c = CMAC.new(key, message, ciphermod=AES)
    return c.digest()
    
class FileSign():
    '''
    s19 文件签名类
    '''
    addrLen = {'S1': 4, 'S2': 6, 'S3': 8}  # 不同类型的S19记录的地址位字符长度

    def __init__(self, file: str):
        self.file = file
    def getData(self) -> bytes:
        '''
        从文件中获取需要签名的数据
        :return: 二进制数据值
        '''
        print("...getData")
        if not (self.file.endswith('.s19') or self.file.endswith('.S19')):
            return False
            
        with open(self.file, 'r') as fp:
            
            content = ''
            while True:
                line = fp.readline().strip('\r').strip('\n')         # 删除每行字符串左右的回车符\r和换行符\n
                if line == '':
                    break
                if (line.startswith('S0') or line.startswith('S8')):   
                    continue
                else:
                    stype = line[0:2]
                    addr = line[4:10]
                    dataLen = int(line[2:4], 16) * 2 - self.addrLen[stype]
                    data = line[len(line) - dataLen : -2]
                    
                    if addr >= '020000' and addr <= '0FF7E0':               # code —— app
                    # if addr >= '020000' and addr <= '020020':             # test
                        data = self.reverse(data)
                        content = f'{content}{data}'
                        continue
            
            contentBin = binascii.unhexlify(content.encode('utf-8')) 
            content_len = len(content) // 2
            
        return contentBin, content_len

    def cmac(self, data_: bytes, key: bytes) -> str:
        '''
            CMAC Calculate
        '''
        content_cmac = cmac_aes_128(key, data_)
        content_cmac = binascii.hexlify(content_cmac).decode()
        # print("Encrypted CMAC:", content_cmac)
        return content_cmac
    
    def reverse(self, data: str) -> str:
        '''
            将数据每隔16字节翻转一次
        '''
        chunks = [data[i:i+32] for i in range(0, len(data), 32)]
        reversed_chunks = []
        for chunk in chunks:
            groups = [chunk[i:i+2] for i in range(0, len(chunk), 2)]
            reversed_groups = groups[::-1]
            reversed_chunk = ''.join(reversed_groups)
            reversed_chunks.append(reversed_chunk)
        reversed_data = ''.join(reversed_chunks)
        # print("reversed_data:", reversed_data)
        
        return reversed_data

    def reversed_cmac(self, data: str) -> str:
        '''
            CMAC翻转
        '''
        groups = [data[i:i+2] for i in range(0, len(data), 2)]
        reversed_groups = groups[::-1]
        reversed_chunk = ''.join(reversed_groups)

        return reversed_chunk

    def writeSignS19(self, out_s19_file: str, cmac: str) -> bool:
        '''
            将签名结果按照s19文件格式写入到此文件地址0x000FF800
        '''
        wf = open(out_s19_file, 'w')
  
        with open(self.file, 'r') as f:
            while True:
                line = f.readline()
                if line.startswith('S8'):
                    wf.write(line)
                    break
                if line.startswith('S2240FF800'):
                    stype = line[0:2]
                    length = line[2:4]
                    addr = line[4:10]
                    # print(f"cmacupper:{cmac.upper()}")
                    data = cmac.upper() + line[43:75]
                    newline = stype + length + addr + data + self.checkSum(length, addr, data)+ ('\n')
                    wf.write(newline)
                    continue
                else:
                    wf.write(line)
                    continue
        print("Signature successfully!")
        wf.close()
        f.close()

    @staticmethod
    def checkSum(*strHex: str) -> str:
        '''
            计算s19文件某一行数据的校验值
            :param strHex: s19数据内容，可以是列表
            :return: 16进制检验值
        '''
        str2check = ''
        sum = 0
        for i in strHex:
            str2check = f'{str2check}{i}'.lower()
        if not (re.match('[0-9a-f]', str2check) and len(str2check) % 2 == 0):
            logging.error(str2check)
            raise ValueError('数据错误')
        while True:
            if len(str2check) == 0:
                break
            sum = sum + int(str2check[:2], 16)
            str2check = str2check[2:]
        checkedSum = hex(int('ff', 16) - int(hex(sum)[-2:], 16))[2:]
        return f'00{checkedSum}'[-2:]

def main(source_folder: str, output_folder: str):


    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
        
    for filename in os.listdir(source_folder):
        if filename.endswith('.s19'):
            s2_export_input_filename = f"{os.path.splitext(filename)[0]}_s2_fill_and_export.s19"
            input_file_path = os.path.join(source_folder, s2_export_input_filename)
            output_filename = f"{os.path.splitext(filename)[0]}_cmac.s19"
            output_file_path = os.path.join(output_folder, output_filename)
        
        subprocess.run(['hexview_fill_and_exports2.bat', os.path.join(source_folder, filename), input_file_path])
            
        appkey_default = [0x3A, 0xF7, 0x42, 0x8C, 0x19, 0xE5, 0xD0, 0x2B, 0x6E, 0x91, 0x04, 0x7D, 0xB3, 0xC8, 0x5F, 0xAA]
        fileSign = FileSign(input_file_path)
        contentBin, content_code_len = fileSign.getData()
        # print('content_code_len:%s\n' % (content_code_len))
        content_cmac = fileSign.cmac(contentBin, bytearray(appkey_default))
        cmac = fileSign.reversed_cmac(content_cmac)
        fileSign.writeSignS19(output_file_path, cmac)



if __name__ == '__main__':
    
    print('version: 20250407.')
    source_folder = './source'
    output_folder = './output'
    main(source_folder, output_folder)
    
