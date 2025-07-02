此脚本适用于数字钥匙APP S19文件CMAC生成

环境：python3.6+  
运行方式：将待计算的S19放入source文件夹下，输出的S19的文件在out文件夹下；
	Windows -  python s2_cmac.py
	Linux   -  python3 s2_cmac.py

备注：还要下载pycryptodome加密库，去cmd中输入指令“pip install pycryptodome”即可

CMAC存放地址0x000FF800
APP CMAC计算地址范围 0x00020000-0x000FF7FF, (确保这个范围是连贯的且未有内容的位置进行全FF填充)

！！！

此脚本的APP_KEY为
	[0x3A, 0xF7, 0x42, 0x8C, 0x19, 0xE5, 0xD0, 0x2B, 0x6E, 0x91, 0x04, 0x7D, 0xB3, 0xC8, 0x5F, 0xAA]
如需修改 在s2_cmac.py脚本的155行（appkey_default = []）处进行替换。

！！！
验证secure_app()函数时需提前load进APP_KEY  且此KEY 和此脚本计算的KEY保持一致。
