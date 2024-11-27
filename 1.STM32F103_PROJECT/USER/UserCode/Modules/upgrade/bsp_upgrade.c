/*
 * bsp_upgrade.c
 *
 *  Created on: 2024年10月23日
 *      Author: lijing
 *	  Abstract: 程序刷写模块，实现多途径程序接收、校验、存储、刷写的流程，app层处理"传输方式、通信协议解析、传输过程加解密"，其他交给bsp层
 */
/*******************************头文件*******************************/
//本组件实现两种升级方式
//1.串口传输+Ymodem协议+单包一致性校验+不验证权限+存储exflash（读写使用明文）
//无法判断软件版本号、型号，默认传什么就刷什么，甚至可以旧刷新

//2.ble传输（传输使用aes算法）+自定义协议（起始+数据+终止+ack+nack+失败重传）+单包一致性校验+数字签名验证权限（md5+rsa）+存储exflash（读写使用aes加解密算法）
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "bsp_upgrade.h"
#include "bsp_crc.h"

/*******************************全局变量定义*******************************/
bsp_upgrade_ctl_t *g_pStBspUpgradeCtl = NULL;//app层负责申请/释放内存
uint8_t *g_pDataCode = NULL;//程序区域一个扇区的数据，大小为BSP_INFLASH_SECTOR_SIZE，太大了不能当作临时变量，必须定义成全局变量

/*******************************内部变量定义*******************************/

/*******************************内部函数的声明*******************************/
static void bsp_upgrade_info_set(uint8_t u8UpFlag);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：初始化控制结构体操作，配置升级方案的详细组成
*函数参数 ：eArea，要操作的升级区域类型
*函数返回值 ：
*备注 ：一次只能处理一个模块
**************************************************************/
void bsp_upgrade_init(BSP_UPGRADE_AREA_E eArea)
{
    if((NULL == g_pStBspUpgradeCtl) || (eArea >= BSP_UPGRADE_AREA_END))
    {
        return;
    }

    uint16_t u16Crc = 0;
    memset(g_pStBspUpgradeCtl,0,sizeof(bsp_upgrade_ctl_t));

    //1.配置程序存储区域和信息区域参数
    if(BSP_UPGRADE_AREA_APP == eArea)
    {//app存储地址
        g_pStBspUpgradeCtl->u32StoreAddr = BSP_UPGRADE_APP_STORE_ADDR_S;
        g_pStBspUpgradeCtl->u32StoreSize = BSP_UPGRADE_APP_STORE_ADDR_E - BSP_UPGRADE_APP_STORE_ADDR_S;
        g_pStBspUpgradeCtl->u32InfoAddr  = BSP_UPGRADE_APP_INFO_ADDR_S;
        g_pStBspUpgradeCtl->u32InfoSize  = BSP_UPGRADE_APP_INFO_ADDR_E - BSP_UPGRADE_APP_INFO_ADDR_S;
        printf("upgrade area param,store(0x%x,%dbytes),info(0x%x,%dbytes)\r\n",g_pStBspUpgradeCtl->u32StoreAddr,g_pStBspUpgradeCtl->u32StoreSize,g_pStBspUpgradeCtl->u32InfoAddr,g_pStBspUpgradeCtl->u32InfoSize);
    }

    //2.处理stInfo，获取已经存储的升级信息
    bsp_upgrade_info_read(g_pStBspUpgradeCtl->u32InfoAddr,(uint32_t)&g_pStBspUpgradeCtl->stInfo,sizeof(bsp_upgrade_info_t));
    u16Crc = bsp_crc16_table((unsigned char *)&g_pStBspUpgradeCtl->stInfo,(sizeof(bsp_upgrade_info_t) - sizeof(g_pStBspUpgradeCtl->stInfo.u16Crc)));
    if((0xffff == g_pStBspUpgradeCtl->stInfo.u16Crc) || (0x0000 == g_pStBspUpgradeCtl->stInfo.u16Crc) || (u16Crc != g_pStBspUpgradeCtl->stInfo.u16Crc))
    {//确保info信息的有效性，异常就重置为全0x00
        printf("upgrade info crc err\r\n");
        memset(&g_pStBspUpgradeCtl->stInfo,0x00,sizeof(bsp_upgrade_info_t));
    }
    else
    {
        printf("************************************\r\nupgrade info param\r\nu8Type:%d\r\nu32Version:0x%x\r\nu8Name:%s\r\nu32TotalLen:%d\r\nu32StoreLen:%d\r\nu8UpFlag:%d\r\n************************************\r\n"
                ,g_pStBspUpgradeCtl->stInfo.u8Type,g_pStBspUpgradeCtl->stInfo.u32Version,g_pStBspUpgradeCtl->stInfo.u8Name,g_pStBspUpgradeCtl->stInfo.u32TotalLen,g_pStBspUpgradeCtl->stInfo.u32StoreLen,g_pStBspUpgradeCtl->stInfo.u8UpFlag);
    }

    //3.处理stMd5Ctx，初始化md5计算结构体
    bsp_md5_init(&g_pStBspUpgradeCtl->stMd5Ctx);

    #if (1 == BSP_UPGRADE_SEC_EN)
    //4.处理stAesCtx，初始化aes计算结构体
    bsp_aes_init(&g_pStBspUpgradeCtl->stAesCtx,NULL);
    #endif
}

/**************************************************************
*函数功能描述 ：数据接收成功，设置标志位
*函数参数 ： u8UpFlag，升级标志
            0x00，不操作直接跳转app
            0x01，程序接收完成，需要刷写app代码，完成后再跳转
            0x02，程序接收完校验失败，擦除升级包存储区域，将现有程序重新拷贝过去，重置该标志信息，完成后再跳转
            0x03，程序接收未完成，意外中断，保存已有数据，等待续传
*函数返回值 ：
*备注 ：
**************************************************************/
static void bsp_upgrade_info_set(uint8_t u8UpFlag)
{
    g_pStBspUpgradeCtl->stInfo.u8UpFlag = u8UpFlag;
    g_pStBspUpgradeCtl->stInfo.u16Crc = bsp_crc16_table((unsigned char *)&g_pStBspUpgradeCtl->stInfo
            ,sizeof(bsp_upgrade_info_t) - sizeof(g_pStBspUpgradeCtl->stInfo.u16Crc));
    bsp_upgrade_info_write(g_pStBspUpgradeCtl->u32InfoAddr,(uint32_t)&g_pStBspUpgradeCtl->stInfo,sizeof(bsp_upgrade_info_t));
}

/**************************************************************
*函数功能描述 ：将接受的数据存储起来，单次调用逐步拼接数据，每满BSP_UPGRADE_BUF_LEN写一次flash，并计算整包校验值（md5或crc）
*函数参数 ： pData，指向要处理的数据，要求是明文
            u16Len，要处理的数据长度
            u8Force，强制写入标志，用于最后一包没有写满的情况，此时传入pData为NULL
*函数返回值 ：
*备注 ：根据方案还要考虑写入前是否需要先加密一遍；
        这个api不关注传输方式和通信协议。
**************************************************************/
void bsp_upgrade_store(uint8_t *pData,uint16_t u16Len,uint8_t u8Force)
{
    if(NULL == g_pStBspUpgradeCtl)
    {
        return;
    }

    uint8_t u8FullFlag = 0;
    uint16_t u16LenTemp = 0;
    uint8_t u8padding = 0;//待填充的数据，最后一组不足16的数值

    if((NULL != pData) || (0 != u16Len))
    {
        //1.拼接数据
        if((u16Len + g_pStBspUpgradeCtl->stBuf.u16BufLen) <= BSP_UPGRADE_BUF_LEN)
        {//没有超过，直接拼接
            memcpy(&g_pStBspUpgradeCtl->stBuf.u8Buf[g_pStBspUpgradeCtl->stBuf.u16BufLen],pData,u16Len);
            g_pStBspUpgradeCtl->stBuf.u16BufLen += u16Len;
            if(BSP_UPGRADE_BUF_LEN == g_pStBspUpgradeCtl->stBuf.u16BufLen)
            {
                u8FullFlag = 1;
            }
        }
        else
        {//超过了，先拼接满存储，再重置缓存，存储剩下的内容
            u16LenTemp = BSP_UPGRADE_BUF_LEN - g_pStBspUpgradeCtl->stBuf.u16BufLen;
            memcpy(&g_pStBspUpgradeCtl->stBuf.u8Buf[g_pStBspUpgradeCtl->stBuf.u16BufLen],pData,u16LenTemp);
            g_pStBspUpgradeCtl->stBuf.u16BufLen = BSP_UPGRADE_BUF_LEN;
            u8FullFlag = 2;
        }
    }
    
    //2.存满或最后一包写入存储区域
    if((0 != u8FullFlag) || (1 == u8Force))
    {
        //3.计算md5数值（可选）
        if(1 == g_pStBspUpgradeCtl->u8ProtocolType)
        {//自定义通信协议才计算md5数值，stm32f103 72Mhz计算2k数据耗时不到1ms
            uint32_t read_len = 0;
            for(uint32_t i = 0; i < (g_pStBspUpgradeCtl->stBuf.u16BufLen - 1); i+= 64)
            {
                read_len = ((i + 64) < (g_pStBspUpgradeCtl->stBuf.u16BufLen - 1)) ? 64 : (g_pStBspUpgradeCtl->stBuf.u16BufLen - 1) - i;//要确定下这里是否有问题
                bsp_md5_update(&g_pStBspUpgradeCtl->stMd5Ctx, (g_pStBspUpgradeCtl->stBuf.u8Buf+i), read_len);
            }
            bsp_md5_final(g_pStBspUpgradeCtl->stInfo.u8Md5,&g_pStBspUpgradeCtl->stMd5Ctx);
        }
        
        //4.加密数据（可选）
        #if (1 == BSP_UPGRADE_SEC_EN)
        u8padding =  g_pStBspUpgradeCtl->stBuf.u16BufLen % BSP_AES_BLOCKLEN;
        if(0 != u8padding)
        {//u8Force为1，最后一组数据不足16的倍数需要手动填充0xff
            for(uint8_t i = 0; i < u8padding; i++)
            {
                g_pStBspUpgradeCtl->stBuf.u8Buf[i + g_pStBspUpgradeCtl->stBuf.u16BufLen] = 0xff;
            }
        }
        bsp_upgrade_en(&g_pStBspUpgradeCtl->stAesCtx,g_pStBspUpgradeCtl->stBuf.u8Buf,(g_pStBspUpgradeCtl->stBuf.u16BufLen + u8padding));
        #endif

        //5.写入存储区域
        bsp_upgrade_store_write(g_pStBspUpgradeCtl->u32StoreAddr,(uint32_t)g_pStBspUpgradeCtl->stBuf.u8Buf,(g_pStBspUpgradeCtl->stBuf.u16BufLen + u8padding));
        
        //6.处理stInfo
        g_pStBspUpgradeCtl->stInfo.u32StoreLen += (g_pStBspUpgradeCtl->stBuf.u16BufLen + u8padding);
        if(0 == g_pStBspUpgradeCtl->u8ProtocolType)
        {//Ymodem协议失败直接擦除
            bsp_upgrade_info_set(0x02);
        }
        else if(1 == g_pStBspUpgradeCtl->u8ProtocolType)
        {//自定义通信协议才进行断电续传
            bsp_upgrade_info_set(0x03);
        }

        //处理stBuf
        g_pStBspUpgradeCtl->u32StoreAddr += BSP_UPGRADE_BUF_LEN;//写入地址递增
        memset(&g_pStBspUpgradeCtl->stBuf,0,sizeof(bsp_upgrade_buf_t));//清空除去addr的其他参数
        if(2 == u8FullFlag)
        {//保存溢出的数据
            memcpy(&g_pStBspUpgradeCtl->stBuf.u8Buf[g_pStBspUpgradeCtl->stBuf.u16BufLen],(pData + u16LenTemp),(u16Len - u16LenTemp));
            g_pStBspUpgradeCtl->stBuf.u16BufLen += u16LenTemp;
        }
    }
}


/**************************************************************
*函数功能描述 ：权限认证
*函数参数 ： pData，签名值
            u16Len，签名值长度
*函数返回值 ：0，权限认证成功；1，权限认证失败
*备注 ：
**************************************************************/
uint8_t bsp_upgrade_auth(uint8_t *pData,uint16_t u16Len)
{
    uint8_t u8ret = 0;
    if(NULL == g_pStBspUpgradeCtl)
    {
        u8ret = 1;
        return u8ret;
    }

    #if (1 == BSP_UPGRADE_AUTH_EN)
    uint8_t u8Md5Temp[BSP_MD5_OUT_LEN] = {0};
    uint16_t u16Outlen = 0;
    if((NULL != pData) && (0 != u16Len))
    {
        //bsp_log_print_hex(BSP_LOG_MODULE_UPGRADE,"sig e",pData,u16Len);
        u16Outlen = bsp_upgrade_sign_verify(pData,u16Len,u8Md5Temp,g_RsaPublicKey_exponent,g_RsaModulus);
        //bsp_log_print_hex(BSP_LOG_MODULE_UPGRADE,"sig d",u8Md5Temp,u16Outlen);

        if(0 != strncmp(u8Md5Temp,(const char *)g_pStBspUpgradeCtl->stInfo.u8Md5,BSP_MD5_OUT_LEN))
        {
            u8ret = 1;
        }        
    }
    else
    {
        u8ret = 1;
    }
    #endif

    return u8ret;
}

/**************************************************************
*函数功能描述 ：数据接收完成操作，根据成功或失败分别设置参数
*函数参数 ： u8Flag，0，校验成功；1，校验失败
*函数返回值 ：
*备注 ：
**************************************************************/
void bsp_upgrade_finish(uint8_t u8Flag)
{
    if(NULL == g_pStBspUpgradeCtl)
    {
        return;
    }
    
    printf("bsp_upgrade_finish TotalLen:%d,StorageLen:%d,u8Flag:%d\r\n",g_pStBspUpgradeCtl->stInfo.u32TotalLen,g_pStBspUpgradeCtl->stInfo.u32StoreLen,u8Flag);
    if(0 == u8Flag)
    {//校验成功，准备升级
        bsp_upgrade_info_set(0x01);
    }
    else
    {//校验失败，擦除已接收数据，备份当前代码
        bsp_upgrade_info_set(0x02);
    }
    
    //软件重启，boot处理app程序区域
    NVIC_SystemReset();
}

/**************************************************************
*函数功能描述 ：将存储区域代码数据拷贝到程序区域
*函数参数 ：u32CodeAddr，程序区域地址
            u32CodeSize，程序区域大小，以扇区为最小单位，bytes
*函数返回值 ：
*备注 ：boot调用，如果需要给多个区域升级，需要重新调用初始化函数bsp_upgrade_init（）重置控制结构体变量；
        然后传入对应的程序地址以供分别处理；
        BSP_UPGRADE_BUF_LEN和BSP_INFLASH_SECTOR_SIZE必须满足倍数关系。
**************************************************************/
void bsp_upgrade_copy_area_code(uint32_t u32CodeAddr,uint32_t u32CodeSize)
{
    if((NULL == g_pStBspUpgradeCtl) || (NULL == g_pDataCode))
    {
        return;
    }
    printf("copy_area_code u32CodeAddr:0x%x,u32CodeSize:%d\r\n",u32CodeAddr,u32CodeSize);

    if((u32CodeSize < g_pStBspUpgradeCtl->stInfo.u32StoreLen)
		|| (0 == g_pStBspUpgradeCtl->stInfo.u32StoreLen))
    {//程序区域小于已存储的数据
        printf("out area code err:%d < %d\r\n",u32CodeSize,g_pStBspUpgradeCtl->stInfo.u32StoreLen);
        return;
    }
    
    uint32_t u32CodeLen = 0;//已写入程序区域的明文数据长度
    uint32_t u32ReadLen = 0;//此次已经读取的数据长度
    uint32_t u32WriteLen = 0;//此次要写入的数据长度
    uint32_t u32SplicIndex = 0;//用于拼接的索引

    while(u32CodeLen < g_pStBspUpgradeCtl->stInfo.u32StoreLen)
    {
        //1.读取存储区域
        memset(&g_pStBspUpgradeCtl->stBuf,0,sizeof(bsp_upgrade_buf_t));//清空除去addr的其他参数
        u32ReadLen = ((g_pStBspUpgradeCtl->stInfo.u32StoreLen - u32CodeLen) > BSP_UPGRADE_BUF_LEN) ? BSP_UPGRADE_BUF_LEN : (g_pStBspUpgradeCtl->stInfo.u32StoreLen - u32CodeLen);
        bsp_upgrade_store_read(g_pStBspUpgradeCtl->u32StoreAddr,(uint32_t)g_pStBspUpgradeCtl->stBuf.u8Buf,u32ReadLen);
        g_pStBspUpgradeCtl->u32StoreAddr += u32ReadLen;

        //2.解密数据（可选
        #if (1 == BSP_UPGRADE_SEC_EN)
        bsp_upgrade_de(&g_pStBspUpgradeCtl->stAesCtx,g_pStBspUpgradeCtl->stBuf.u8Buf,u32ReadLen);//单次加解密单位是存储区域的一个扇区或最后一包
        #endif

        //3.写入程序区域
        if(u32ReadLen < BSP_INFLASH_SECTOR_SIZE)
        {//读多次，写一次，拼接
            memcpy(&g_pDataCode[u32SplicIndex],g_pStBspUpgradeCtl->stBuf.u8Buf,u32ReadLen);
            u32SplicIndex += u32ReadLen;
            if((BSP_INFLASH_SECTOR_SIZE == u32SplicIndex) //存满写入
                || (BSP_UPGRADE_BUF_LEN != u32ReadLen))//最后一包写入
            {
                hw_inflash_write(u32CodeAddr,(uint32_t)g_pDataCode,u32SplicIndex);
                u32CodeAddr += u32SplicIndex;
                u32CodeLen  += u32SplicIndex;
                u32SplicIndex = 0;
            }
        }
        else if(u32ReadLen == BSP_INFLASH_SECTOR_SIZE)
        {//读一次，写一次
            hw_inflash_write(u32CodeAddr,(uint32_t)&g_pStBspUpgradeCtl->stBuf.u8Buf[0],u32ReadLen);
            u32CodeAddr += u32ReadLen;
            u32CodeLen  += u32ReadLen;
        }
        else if(u32ReadLen > BSP_INFLASH_SECTOR_SIZE)
        {//读一次，写多次，分块
            for(uint32_t i = 0; i <= (u32ReadLen - 1); i+=BSP_INFLASH_SECTOR_SIZE)
            {
                u32WriteLen = ((i + BSP_INFLASH_SECTOR_SIZE) < u32ReadLen) ? BSP_INFLASH_SECTOR_SIZE : (u32ReadLen - i);
                hw_inflash_write(u32CodeAddr,(uint32_t)&g_pStBspUpgradeCtl->stBuf.u8Buf[i],u32WriteLen);
                u32CodeAddr += u32WriteLen;
            }
            u32CodeLen  += u32ReadLen;
        }

        /*//这种方式无法处理BSP_UPGRADE_BUF_LEN < BSP_INFLASH_SECTOR_SIZE的情况（读多次，写一次，拼接）
        //最后一包不足BSP_UPGRADE_BUF_LEN的读取差值，其他都读取BSP_UPGRADE_BUF_LEN
        u32ReadLen = ((g_pStBspUpgradeCtl->stInfo.u32StoreLen - u32CodeLen) > BSP_UPGRADE_BUF_LEN) ? BSP_UPGRADE_BUF_LEN : (g_pStBspUpgradeCtl->stInfo.u32StoreLen - u32CodeLen);
        bsp_upgrade_store_read(g_pStBspUpgradeCtl->u32StoreAddr,(uint32_t)g_pStBspUpgradeCtl->stBuf.u8Buf,u32ReadLen);
        g_pStBspUpgradeCtl->u32StoreAddr += u32ReadLen;

        #if (1 == BSP_UPGRADE_SEC_EN)//需要解密数据
        bsp_upgrade_de(&g_pStBspUpgradeCtl->stAesCtx,g_pStBspUpgradeCtl->stBuf.u8Buf,u32ReadLen);
        #endif

        for(uint32_t i = 0; i <= (u32ReadLen - 1); i+=BSP_INFLASH_SECTOR_SIZE)
        {//每次最多写入BSP_INFLASH_SECTOR_SIZE，如果超过要分批写入
            u32WriteLen = ((i + BSP_INFLASH_SECTOR_SIZE) < u32ReadLen) ? BSP_INFLASH_SECTOR_SIZE : (u32ReadLen - i);
            hw_inflash_write(u32CodeAddr,&g_pStBspUpgradeCtl->stBuf.u8Buf[i],u32WriteLen);
            u32CodeAddr += u32WriteLen;
        }
        u32CodeLen += u32ReadLen;*/
    }

    //4.擦除程序区域后一个扇区
    if(u32CodeLen < u32CodeSize)
    {//没有超过程序区域
        u32CodeAddr = ((u32CodeAddr/BSP_INFLASH_SECTOR_SIZE) + 1) * BSP_INFLASH_SECTOR_SIZE;
        hw_inflash_erase(u32CodeAddr);
    }

    //5.重置参数
    bsp_upgrade_info_set(0);//刷写完成，更新升级标志
    printf("copy_area_code end\r\n");
}

/**************************************************************
*函数功能描述 ：将程序区域代码数据拷贝到存储区域
*函数参数 ：u32CodeAddr，程序区域地址
            u32CodeSize，程序区域大小，以扇区为最小单位，bytes
*函数返回值 ：
*备注 ：boot调用，用于“数据接收未完成，但是无法续传”的情况，此时升级信息已经被重置；
        所以需要把当前运行的app代码重新拷贝到存储区域备份，再重置升级信息;
        BSP_UPGRADE_BUF_LEN和BSP_INFLASH_SECTOR_SIZE必须满足倍数关系。
**************************************************************/
void bsp_upgrade_copy_area_store(uint32_t u32CodeAddr,uint32_t u32CodeSize)
{
    if((NULL == g_pStBspUpgradeCtl) || (NULL == g_pDataCode))
    {
        return;
    }
    printf("copy_area_store u32CodeAddr:0x%x,u32CodeSize:%d\r\n",u32CodeAddr,u32CodeSize);

    g_pStBspUpgradeCtl->stInfo.u32StoreLen = 0;//已写入存储区域的明文数据长度
    uint32_t u32ReadLen = BSP_INFLASH_SECTOR_SIZE;//此次要读取的数据长度，固定值
    uint32_t u32WriteLen = BSP_UPGRADE_BUF_LEN;//此次要写入的数据长度，固定值
    uint32_t u32SplicIndex = 0;//用于拼接的索引

    uint8_t  u8Data[32] = {0};//用于判定app数据是否读完
    memset(u8Data,0xff,sizeof(u8Data));

    while(1)
    {
        //1.读取程序区域
        memset(g_pDataCode,0,BSP_INFLASH_SECTOR_SIZE);
        hw_inflash_read(u32CodeAddr,g_pDataCode,u32ReadLen);
        u32CodeAddr += u32ReadLen;
        if(g_pStBspUpgradeCtl->stInfo.u32StoreLen > g_pStBspUpgradeCtl->u32StoreSize)
        {//超过存储区域错误，直接返回
            printf("out area store err!\r\n");
            return;
        }
        else if(0 == strncmp((const char *)g_pDataCode,(const char *)u8Data,sizeof(u8Data)))
        {//有效数据读完了，准备退出
            if(0 != u32SplicIndex)
            {//最后一包写入
                //2.加密数据（可选
                #if (1 == BSP_UPGRADE_SEC_EN)
                bsp_upgrade_en(&g_pStBspUpgradeCtl->stAesCtx,g_pStBspUpgradeCtl->stBuf.u8Buf,u32SplicIndex);//单次加解密单位是存储区域的一个扇区或最后一包
                #endif
                //3.写入存储区域
                bsp_upgrade_store_write(g_pStBspUpgradeCtl->u32StoreAddr,(uint32_t)g_pStBspUpgradeCtl->stBuf.u8Buf,u32SplicIndex);
                g_pStBspUpgradeCtl->u32StoreAddr += u32SplicIndex;
                g_pStBspUpgradeCtl->stInfo.u32StoreLen += u32SplicIndex;
                u32SplicIndex = 0;
            }
            break;
        }

        if(BSP_UPGRADE_BUF_LEN < u32ReadLen)
        {//读一次，写多次，分块
            for(uint32_t i = 0; i <= (u32ReadLen - 1); i+=BSP_UPGRADE_BUF_LEN)
            {
                //2.加密数据（可选
                #if (1 == BSP_UPGRADE_SEC_EN)
                bsp_upgrade_en(&g_pStBspUpgradeCtl->stAesCtx,&g_pDataCode[i],u32WriteLen);//单次加解密单位是存储区域的一个扇区
                #endif
                //3.写入存储区域
                bsp_upgrade_store_write(g_pStBspUpgradeCtl->u32StoreAddr,(uint32_t)&g_pDataCode[i],u32WriteLen);
                g_pStBspUpgradeCtl->u32StoreAddr += u32WriteLen;
            }
            g_pStBspUpgradeCtl->stInfo.u32StoreLen += u32ReadLen;
        }
        else if(BSP_UPGRADE_BUF_LEN == u32ReadLen)
        {//读一次，写一次
            //2.加密数据（可选
            #if (1 == BSP_UPGRADE_SEC_EN)
            bsp_upgrade_en(&g_pStBspUpgradeCtl->stAesCtx,g_pDataCode,u32WriteLen);//单次加解密单位是存储区域的一个扇区
            #endif
            //3.写入存储区域
            bsp_upgrade_store_write(g_pStBspUpgradeCtl->u32StoreAddr,(uint32_t)g_pDataCode,u32ReadLen);
            g_pStBspUpgradeCtl->u32StoreAddr += u32ReadLen;
            g_pStBspUpgradeCtl->stInfo.u32StoreLen += u32ReadLen;
        }
        else if(BSP_UPGRADE_BUF_LEN > u32ReadLen)
        {//读多次，写一次，拼接
            memcpy(&g_pStBspUpgradeCtl->stBuf.u8Buf[u32SplicIndex],g_pDataCode,u32ReadLen);
            u32SplicIndex += u32ReadLen;
            if(BSP_UPGRADE_BUF_LEN == u32SplicIndex)//存满写入
            {
                //2.加密数据（可选
                #if (1 == BSP_UPGRADE_SEC_EN)
                bsp_upgrade_en(&g_pStBspUpgradeCtl->stAesCtx,g_pStBspUpgradeCtl->stBuf.u8Buf,u32SplicIndex);//单次加解密单位是存储区域的一个扇区
                #endif
                //3.写入存储区域
                bsp_upgrade_store_write(g_pStBspUpgradeCtl->u32StoreAddr,(uint32_t)g_pStBspUpgradeCtl->stBuf.u8Buf,u32SplicIndex);
                g_pStBspUpgradeCtl->u32StoreAddr += u32SplicIndex;
                g_pStBspUpgradeCtl->stInfo.u32StoreLen += u32SplicIndex;
                u32SplicIndex = 0;
            }
        }
    }

    //4.重置参数
	if(0 != g_pStBspUpgradeCtl->stInfo.u32StoreLen)
	{
		memset(&g_pStBspUpgradeCtl->stInfo,0x00,sizeof(bsp_upgrade_info_t));
		//从系统参数或系统信息存储区域获取当前程序信息
		// g_pStBspUpgradeCtl->stInfo.u8Type = stSysCtl.u8Type;
		// g_pStBspUpgradeCtl->stInfo.u32Version = stSysCtl.u32Version;
		// memcpy(g_pStBspUpgradeCtl->stInfo.u8Name,tSysCtl.u8Name,sizeof(g_pStBspUpgradeCtl->stInfo.u8Name));
		g_pStBspUpgradeCtl->stInfo.u32TotalLen = g_pStBspUpgradeCtl->stInfo.u32StoreLen;
		g_pStBspUpgradeCtl->stInfo.u8UpFlag = 0;
		 g_pStBspUpgradeCtl->stInfo.u16Crc = bsp_crc16_table((unsigned char *)&g_pStBspUpgradeCtl->stInfo
			,sizeof(bsp_upgrade_info_t) - sizeof(g_pStBspUpgradeCtl->stInfo.u16Crc));
		bsp_upgrade_info_write(g_pStBspUpgradeCtl->u32InfoAddr,(uint32_t)&g_pStBspUpgradeCtl->stInfo,sizeof(bsp_upgrade_info_t));
		printf("copy_area_store end\r\n");
	}
}


