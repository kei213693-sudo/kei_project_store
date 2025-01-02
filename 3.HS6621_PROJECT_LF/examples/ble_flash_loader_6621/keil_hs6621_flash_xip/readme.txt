说明：
	若flash loader存放地址不是默认运行地址(一般是0x3000),则程序烧录时应先烧录客户APP（手段不限），
	再用ISP工具烧录flash loader程序（ISP会自动更新MBR PART_TYPE_APP区的启动地址，以确保先启动flash loader程序）
	Debug时要注意由于APP的默认flash烧录地址为0x3000，新烧录的APP地址与其对应的MBR区（FLASH_APP_MBR_TYPE）地址可能不同导致启动失败，因此建议烧录新APP时先将flash全部擦除

loader工作流程如下：
	1 在MBR中（FLASH_APP_MBR_TYPE）读取客户APP存放地址
	2 若客户APP地址读取成功且不在客户APP默认存放基地址(FLASH_APP_DEF_ADDR),则将客户APP拷贝到APP基地址(FLASH_APP_DEF_ADDR),并更新MBR(FLASH_APP_MBR_TYPE)
	3 跳转到客户APP基地址(FLASH_APP_DEF_ADDR)运行客户代码
	

以下为可能修改的参数：
	修改 loader 程序的 flash 地址:(startup_hs221_flash.sct), [注意]该地址为flash对应的memory地址
	具体位置可参考相应芯片的Document->user_guides->memory. 比如flash地址是0x3000，6621对应的地址是0x12003000, 6621c是0x00403000.
	#define FLASH_LOADER_ADDR        0x1203f000
	
	修改 客户APP默认存放基地址:(main.c)
	#define FLASH_APP_DEF_ADDR 0x3000

	修改 客户APP地址在MBR存放的位置:(main.c)
	#define FLASH_APP_MBR_TYPE  PART_TYPE_USR1
	
	修改 loader 打印:(main.c)
	#define FLASH_LOADER_DEBUG  0