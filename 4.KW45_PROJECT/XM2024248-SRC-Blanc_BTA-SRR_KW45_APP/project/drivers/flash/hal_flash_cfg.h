/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_flash.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述flash模块配置接口，不建议修改。
 *      文件调用：
 *                  文件应被hal_flash.c调用。
 *                  文件应包含hal_flash_typedef.h。
 * 当前版本：
 * 历史版本：
*/
#ifndef HAL_flash_CFG_H
#define HAL_flash_CFG_H

/*  Include     Begine     */
#include "hal_flash_typedef.h"
/*  Include     End     */

/*  Function    Begine     */
#define HAL_FLASH_SECTOR_SIZE								(0x2000)

////0x01000000 ~ 0x0101FFFF
//#define HAL_DATAFLASH_START_ADDR						(0x01000000)
//#define HAL_DATAFLASH_END_ADDR							(0x0101FFFF)
//#define HAL_DATAFLASH_ALL_SIZE              (128UL << 10U)
//#define HAL_DATAFLASH_ADDR_ARR_LEN          (16U)//存放地址的数组长度
//
//#define HAL_DATAFLASH_ADDR0                 (/*地址自定义*/0x01000000)
//#define HAL_DATAFLASH_ADDR1                 (/*地址自定义*/0x01002000)
//#define HAL_DATAFLASH_ADDR2                 (/*地址自定义*/0x01004000)
//#define HAL_DATAFLASH_ADDR3                 (/*地址自定义*/0x01006000)
//#define HAL_DATAFLASH_ADDR4                 (/*地址自定义*/0x01008000)
//#define HAL_DATAFLASH_ADDR5                 (/*地址自定义*/0x0100A000)
//#define HAL_DATAFLASH_ADDR6                 (/*地址自定义*/0x0100C000)
//#define HAL_DATAFLASH_ADDR7                 (/*地址自定义*/0x0100E000)
//#define HAL_DATAFLASH_ADDR8                 (/*地址自定义*/0x01010000)
//#define HAL_DATAFLASH_ADDR9                 (/*地址自定义*/0x01012000)
//#define HAL_DATAFLASH_ADDR10                (/*地址自定义*/0x01014000)
//#define HAL_DATAFLASH_ADDR11                (/*地址自定义*/0x01016000)
//#define HAL_DATAFLASH_ADDR12                (/*地址自定义*/0x01018000)
//#define HAL_DATAFLASH_ADDR13                (/*地址自定义*/0x0101A000)
//#define HAL_DATAFLASH_ADDR14                (/*地址自定义*/0x0101C000)
//#define HAL_DATAFLASH_ADDR15                (/*地址自定义*/0x0101E000)

//0x00000000 ~ 0x001FFFFF
#define HAL_CODEFLASH_START_ADDR						(0x00000000)
#define HAL_CODEFLASH_END_ADDR							(0x000FFFFF)
#define HAL_CODEFLASH_ALL_SIZE              (1024UL << 10U)
#define HAL_CODEFLASH_ADDR_ARR_LEN          (128U)//存放地址的数组长度
#define HAL_CODEFLASH_ADDR0                 (/*地址自定义*/0x00000000)
#define HAL_CODEFLASH_ADDR1                 (/*地址自定义*/0x00002000)
#define HAL_CODEFLASH_ADDR2                 (/*地址自定义*/0x00004000)
#define HAL_CODEFLASH_ADDR3                 (/*地址自定义*/0x00006000)
#define HAL_CODEFLASH_ADDR4                 (/*地址自定义*/0x00008000)
#define HAL_CODEFLASH_ADDR5                 (/*地址自定义*/0x0000A000)
#define HAL_CODEFLASH_ADDR6                 (/*地址自定义*/0x0000C000)
#define HAL_CODEFLASH_ADDR7                 (/*地址自定义*/0x0000E000)

#define HAL_CODEFLASH_ADDR8                 (/*地址自定义*/0x00010000)
#define HAL_CODEFLASH_ADDR9                 (/*地址自定义*/0x00012000)
#define HAL_CODEFLASH_ADDR10                (/*地址自定义*/0x00014000)
#define HAL_CODEFLASH_ADDR11                (/*地址自定义*/0x00016000)
#define HAL_CODEFLASH_ADDR12                (/*地址自定义*/0x00018000)
#define HAL_CODEFLASH_ADDR13                (/*地址自定义*/0x0001A000)
#define HAL_CODEFLASH_ADDR14                (/*地址自定义*/0x0001C000)
#define HAL_CODEFLASH_ADDR15                (/*地址自定义*/0x0001E000)

#define HAL_CODEFLASH_ADDR16                (/*地址自定义*/0x00020000)
#define HAL_CODEFLASH_ADDR17                (/*地址自定义*/0x00022000)
#define HAL_CODEFLASH_ADDR18                (/*地址自定义*/0x00024000)
#define HAL_CODEFLASH_ADDR19                (/*地址自定义*/0x00026000)
#define HAL_CODEFLASH_ADDR20                (/*地址自定义*/0x00028000)
#define HAL_CODEFLASH_ADDR21                (/*地址自定义*/0x0002A000)
#define HAL_CODEFLASH_ADDR22                (/*地址自定义*/0x0002C000)
#define HAL_CODEFLASH_ADDR23                (/*地址自定义*/0x0002E000)

#define HAL_CODEFLASH_ADDR24                (/*地址自定义*/0x00030000)
#define HAL_CODEFLASH_ADDR25                (/*地址自定义*/0x00032000)
#define HAL_CODEFLASH_ADDR26                (/*地址自定义*/0x00034000)
#define HAL_CODEFLASH_ADDR27                (/*地址自定义*/0x00036000)
#define HAL_CODEFLASH_ADDR28                (/*地址自定义*/0x00038000)
#define HAL_CODEFLASH_ADDR29                (/*地址自定义*/0x0003A000)
#define HAL_CODEFLASH_ADDR30                (/*地址自定义*/0x0003C000)
#define HAL_CODEFLASH_ADDR31                (/*地址自定义*/0x0003E000)

#define HAL_CODEFLASH_ADDR32                (/*地址自定义*/0x00040000)
#define HAL_CODEFLASH_ADDR33                (/*地址自定义*/0x00042000)
#define HAL_CODEFLASH_ADDR34                (/*地址自定义*/0x00044000)
#define HAL_CODEFLASH_ADDR35                (/*地址自定义*/0x00046000)
#define HAL_CODEFLASH_ADDR36                (/*地址自定义*/0x00048000)
#define HAL_CODEFLASH_ADDR37                (/*地址自定义*/0x0004A000)
#define HAL_CODEFLASH_ADDR38                (/*地址自定义*/0x0004C000)
#define HAL_CODEFLASH_ADDR39                (/*地址自定义*/0x0004E000)

#define HAL_CODEFLASH_ADDR40                (/*地址自定义*/0x00050000)
#define HAL_CODEFLASH_ADDR41                (/*地址自定义*/0x00052000)
#define HAL_CODEFLASH_ADDR42                (/*地址自定义*/0x00054000)
#define HAL_CODEFLASH_ADDR43                (/*地址自定义*/0x00056000)
#define HAL_CODEFLASH_ADDR44                (/*地址自定义*/0x00058000)
#define HAL_CODEFLASH_ADDR45                (/*地址自定义*/0x0005A000)
#define HAL_CODEFLASH_ADDR46                (/*地址自定义*/0x0005C000)
#define HAL_CODEFLASH_ADDR47                (/*地址自定义*/0x0005E000)

#define HAL_CODEFLASH_ADDR48                (/*地址自定义*/0x00060000)
#define HAL_CODEFLASH_ADDR49                (/*地址自定义*/0x00062000)
#define HAL_CODEFLASH_ADDR50                (/*地址自定义*/0x00064000)
#define HAL_CODEFLASH_ADDR51                (/*地址自定义*/0x00066000)
#define HAL_CODEFLASH_ADDR52                (/*地址自定义*/0x00068000)
#define HAL_CODEFLASH_ADDR53                (/*地址自定义*/0x0006A000)
#define HAL_CODEFLASH_ADDR54                (/*地址自定义*/0x0006C000)
#define HAL_CODEFLASH_ADDR55                (/*地址自定义*/0x0006E000)

#define HAL_CODEFLASH_ADDR56                (/*地址自定义*/0x00070000)
#define HAL_CODEFLASH_ADDR57                (/*地址自定义*/0x00072000)
#define HAL_CODEFLASH_ADDR58                (/*地址自定义*/0x00074000)
#define HAL_CODEFLASH_ADDR59                (/*地址自定义*/0x00076000)
#define HAL_CODEFLASH_ADDR60                (/*地址自定义*/0x00078000)
#define HAL_CODEFLASH_ADDR61                (/*地址自定义*/0x0007A000)
#define HAL_CODEFLASH_ADDR62                (/*地址自定义*/0x0007C000)
#define HAL_CODEFLASH_ADDR63                (/*地址自定义*/0x0007E000)

#define HAL_CODEFLASH_ADDR64                (/*地址自定义*/0x00080000)
#define HAL_CODEFLASH_ADDR65                (/*地址自定义*/0x00082000)
#define HAL_CODEFLASH_ADDR66                (/*地址自定义*/0x00084000)
#define HAL_CODEFLASH_ADDR67                (/*地址自定义*/0x00086000)
#define HAL_CODEFLASH_ADDR68                (/*地址自定义*/0x00088000)
#define HAL_CODEFLASH_ADDR69                (/*地址自定义*/0x0008A000)
#define HAL_CODEFLASH_ADDR70                (/*地址自定义*/0x0008C000)
#define HAL_CODEFLASH_ADDR71                (/*地址自定义*/0x0008E000)

#define HAL_CODEFLASH_ADDR72                (/*地址自定义*/0x00090000)
#define HAL_CODEFLASH_ADDR73                (/*地址自定义*/0x00092000)
#define HAL_CODEFLASH_ADDR74                (/*地址自定义*/0x00094000)
#define HAL_CODEFLASH_ADDR75                (/*地址自定义*/0x00096000)
#define HAL_CODEFLASH_ADDR76                (/*地址自定义*/0x00098000)
#define HAL_CODEFLASH_ADDR77                (/*地址自定义*/0x0009A000)
#define HAL_CODEFLASH_ADDR78                (/*地址自定义*/0x0009C000)
#define HAL_CODEFLASH_ADDR79                (/*地址自定义*/0x0009E000)

#define HAL_CODEFLASH_ADDR80                (/*地址自定义*/0x000A0000)
#define HAL_CODEFLASH_ADDR81                (/*地址自定义*/0x000A2000)
#define HAL_CODEFLASH_ADDR82                (/*地址自定义*/0x000A4000)
#define HAL_CODEFLASH_ADDR83                (/*地址自定义*/0x000A6000)
#define HAL_CODEFLASH_ADDR84                (/*地址自定义*/0x000A8000)
#define HAL_CODEFLASH_ADDR85                (/*地址自定义*/0x000AA000)
#define HAL_CODEFLASH_ADDR86                (/*地址自定义*/0x000AC000)
#define HAL_CODEFLASH_ADDR87                (/*地址自定义*/0x000AE000)

#define HAL_CODEFLASH_ADDR88                (/*地址自定义*/0x000B0000)
#define HAL_CODEFLASH_ADDR89                (/*地址自定义*/0x000B2000)
#define HAL_CODEFLASH_ADDR90                (/*地址自定义*/0x000B4000)
#define HAL_CODEFLASH_ADDR91                (/*地址自定义*/0x000B6000)
#define HAL_CODEFLASH_ADDR92                (/*地址自定义*/0x000B8000)
#define HAL_CODEFLASH_ADDR93                (/*地址自定义*/0x000BA000)
#define HAL_CODEFLASH_ADDR94                (/*地址自定义*/0x000BC000)
#define HAL_CODEFLASH_ADDR95                (/*地址自定义*/0x000BE000)

#define HAL_CODEFLASH_ADDR96                (/*地址自定义*/0x000C0000)
#define HAL_CODEFLASH_ADDR97                (/*地址自定义*/0x000C2000)
#define HAL_CODEFLASH_ADDR98                (/*地址自定义*/0x000C4000)
#define HAL_CODEFLASH_ADDR99                (/*地址自定义*/0x000C6000)
#define HAL_CODEFLASH_ADDR100               (/*地址自定义*/0x000C8000)
#define HAL_CODEFLASH_ADDR101               (/*地址自定义*/0x000CA000)
#define HAL_CODEFLASH_ADDR102               (/*地址自定义*/0x000CC000)
#define HAL_CODEFLASH_ADDR103               (/*地址自定义*/0x000CE000)

#define HAL_CODEFLASH_ADDR104               (/*地址自定义*/0x000D0000)
#define HAL_CODEFLASH_ADDR105               (/*地址自定义*/0x000D2000)
#define HAL_CODEFLASH_ADDR106               (/*地址自定义*/0x000D4000)
#define HAL_CODEFLASH_ADDR107               (/*地址自定义*/0x000D6000)
#define HAL_CODEFLASH_ADDR108               (/*地址自定义*/0x000D8000)
#define HAL_CODEFLASH_ADDR109               (/*地址自定义*/0x000DA000)
#define HAL_CODEFLASH_ADDR110               (/*地址自定义*/0x000DC000)
#define HAL_CODEFLASH_ADDR111               (/*地址自定义*/0x000DE000)

#define HAL_CODEFLASH_ADDR112               (/*地址自定义*/0x000E0000)
#define HAL_CODEFLASH_ADDR113               (/*地址自定义*/0x000E2000)
#define HAL_CODEFLASH_ADDR114               (/*地址自定义*/0x000E4000)
#define HAL_CODEFLASH_ADDR115               (/*地址自定义*/0x000E6000)
#define HAL_CODEFLASH_ADDR116               (/*地址自定义*/0x000E8000)
#define HAL_CODEFLASH_ADDR117               (/*地址自定义*/0x000EA000)
#define HAL_CODEFLASH_ADDR118               (/*地址自定义*/0x000EC000)
#define HAL_CODEFLASH_ADDR119               (/*地址自定义*/0x000EE000)

#define HAL_CODEFLASH_ADDR120               (/*地址自定义*/0x000F0000)
#define HAL_CODEFLASH_ADDR121               (/*地址自定义*/0x000F2000)
#define HAL_CODEFLASH_ADDR122               (/*地址自定义*/0x000F4000)
#define HAL_CODEFLASH_ADDR123               (/*地址自定义*/0x000F6000)
#define HAL_CODEFLASH_ADDR124               (/*地址自定义*/0x000F8000)
#define HAL_CODEFLASH_ADDR125               (/*地址自定义*/0x000FA000)
#define HAL_CODEFLASH_ADDR126               (/*地址自定义*/0x000FC000)
#define HAL_CODEFLASH_ADDR127               (/*地址自定义*/0x000FE000)

//#define HAL_CODEFLASH_ADDR128               (/*地址自定义*/0x00100000)
//#define HAL_CODEFLASH_ADDR129               (/*地址自定义*/0x00102000)
//#define HAL_CODEFLASH_ADDR130               (/*地址自定义*/0x00104000)
//#define HAL_CODEFLASH_ADDR131               (/*地址自定义*/0x00106000)
//#define HAL_CODEFLASH_ADDR132               (/*地址自定义*/0x00108000)
//#define HAL_CODEFLASH_ADDR133               (/*地址自定义*/0x0010A000)
//#define HAL_CODEFLASH_ADDR134               (/*地址自定义*/0x0010C000)
//#define HAL_CODEFLASH_ADDR135               (/*地址自定义*/0x0010E000)
//
//#define HAL_CODEFLASH_ADDR136               (/*地址自定义*/0x00110000)
//#define HAL_CODEFLASH_ADDR137               (/*地址自定义*/0x00112000)
//#define HAL_CODEFLASH_ADDR138               (/*地址自定义*/0x00114000)
//#define HAL_CODEFLASH_ADDR139               (/*地址自定义*/0x00116000)
//#define HAL_CODEFLASH_ADDR140               (/*地址自定义*/0x00118000)
//#define HAL_CODEFLASH_ADDR141               (/*地址自定义*/0x0011A000)
//#define HAL_CODEFLASH_ADDR142               (/*地址自定义*/0x0011C000)
//#define HAL_CODEFLASH_ADDR143               (/*地址自定义*/0x0011E000)
//
//#define HAL_CODEFLASH_ADDR144               (/*地址自定义*/0x00120000)
//#define HAL_CODEFLASH_ADDR145               (/*地址自定义*/0x00122000)
//#define HAL_CODEFLASH_ADDR146               (/*地址自定义*/0x00124000)
//#define HAL_CODEFLASH_ADDR147               (/*地址自定义*/0x00126000)
//#define HAL_CODEFLASH_ADDR148               (/*地址自定义*/0x00128000)
//#define HAL_CODEFLASH_ADDR149               (/*地址自定义*/0x0012A000)
//#define HAL_CODEFLASH_ADDR150               (/*地址自定义*/0x0012C000)
//#define HAL_CODEFLASH_ADDR151               (/*地址自定义*/0x0012E000)
//
//#define HAL_CODEFLASH_ADDR152               (/*地址自定义*/0x00130000)
//#define HAL_CODEFLASH_ADDR153               (/*地址自定义*/0x00132000)
//#define HAL_CODEFLASH_ADDR154               (/*地址自定义*/0x00134000)
//#define HAL_CODEFLASH_ADDR155               (/*地址自定义*/0x00136000)
//#define HAL_CODEFLASH_ADDR156               (/*地址自定义*/0x00138000)
//#define HAL_CODEFLASH_ADDR157               (/*地址自定义*/0x0013A000)
//#define HAL_CODEFLASH_ADDR158               (/*地址自定义*/0x0013C000)
//#define HAL_CODEFLASH_ADDR159               (/*地址自定义*/0x0013E000)
//
//#define HAL_CODEFLASH_ADDR160               (/*地址自定义*/0x00140000)
//#define HAL_CODEFLASH_ADDR161               (/*地址自定义*/0x00142000)
//#define HAL_CODEFLASH_ADDR162               (/*地址自定义*/0x00144000)
//#define HAL_CODEFLASH_ADDR163               (/*地址自定义*/0x00146000)
//#define HAL_CODEFLASH_ADDR164               (/*地址自定义*/0x00148000)
//#define HAL_CODEFLASH_ADDR165               (/*地址自定义*/0x0014A000)
//#define HAL_CODEFLASH_ADDR166               (/*地址自定义*/0x0014C000)
//#define HAL_CODEFLASH_ADDR167               (/*地址自定义*/0x0014E000)
//
//#define HAL_CODEFLASH_ADDR168               (/*地址自定义*/0x00150000)
//#define HAL_CODEFLASH_ADDR169               (/*地址自定义*/0x00152000)
//#define HAL_CODEFLASH_ADDR170               (/*地址自定义*/0x00154000)
//#define HAL_CODEFLASH_ADDR171               (/*地址自定义*/0x00156000)
//#define HAL_CODEFLASH_ADDR172               (/*地址自定义*/0x00158000)
//#define HAL_CODEFLASH_ADDR173               (/*地址自定义*/0x0015A000)
//#define HAL_CODEFLASH_ADDR174               (/*地址自定义*/0x0015C000)
//#define HAL_CODEFLASH_ADDR175               (/*地址自定义*/0x0015E000)
//
//#define HAL_CODEFLASH_ADDR176               (/*地址自定义*/0x00160000)
//#define HAL_CODEFLASH_ADDR177               (/*地址自定义*/0x00162000)
//#define HAL_CODEFLASH_ADDR178               (/*地址自定义*/0x00164000)
//#define HAL_CODEFLASH_ADDR179               (/*地址自定义*/0x00166000)
//#define HAL_CODEFLASH_ADDR180               (/*地址自定义*/0x00168000)
//#define HAL_CODEFLASH_ADDR181               (/*地址自定义*/0x0016A000)
//#define HAL_CODEFLASH_ADDR182               (/*地址自定义*/0x0016C000)
//#define HAL_CODEFLASH_ADDR183               (/*地址自定义*/0x0016E000)
//
//#define HAL_CODEFLASH_ADDR184               (/*地址自定义*/0x00170000)
//#define HAL_CODEFLASH_ADDR185               (/*地址自定义*/0x00172000)
//#define HAL_CODEFLASH_ADDR186               (/*地址自定义*/0x00174000)
//#define HAL_CODEFLASH_ADDR187               (/*地址自定义*/0x00176000)
//#define HAL_CODEFLASH_ADDR188               (/*地址自定义*/0x00178000)
//#define HAL_CODEFLASH_ADDR189               (/*地址自定义*/0x0017A000)
//#define HAL_CODEFLASH_ADDR190               (/*地址自定义*/0x0017C000)
//#define HAL_CODEFLASH_ADDR191               (/*地址自定义*/0x0017E000)
//
//#define HAL_CODEFLASH_ADDR192               (/*地址自定义*/0x00180000)
//#define HAL_CODEFLASH_ADDR193               (/*地址自定义*/0x00182000)
//#define HAL_CODEFLASH_ADDR194               (/*地址自定义*/0x00184000)
//#define HAL_CODEFLASH_ADDR195               (/*地址自定义*/0x00186000)
//#define HAL_CODEFLASH_ADDR196               (/*地址自定义*/0x00188000)
//#define HAL_CODEFLASH_ADDR197               (/*地址自定义*/0x0018A000)
//#define HAL_CODEFLASH_ADDR198               (/*地址自定义*/0x0018C000)
//#define HAL_CODEFLASH_ADDR199               (/*地址自定义*/0x0018E000)
//
//#define HAL_CODEFLASH_ADDR200               (/*地址自定义*/0x00190000)
//#define HAL_CODEFLASH_ADDR201               (/*地址自定义*/0x00192000)
//#define HAL_CODEFLASH_ADDR202               (/*地址自定义*/0x00194000)
//#define HAL_CODEFLASH_ADDR203               (/*地址自定义*/0x00196000)
//#define HAL_CODEFLASH_ADDR204               (/*地址自定义*/0x00198000)
//#define HAL_CODEFLASH_ADDR205               (/*地址自定义*/0x0019A000)
//#define HAL_CODEFLASH_ADDR206               (/*地址自定义*/0x0019C000)
//#define HAL_CODEFLASH_ADDR207               (/*地址自定义*/0x0019E000)
//
//#define HAL_CODEFLASH_ADDR208               (/*地址自定义*/0x001A0000)
//#define HAL_CODEFLASH_ADDR209               (/*地址自定义*/0x001A2000)
//#define HAL_CODEFLASH_ADDR210               (/*地址自定义*/0x001A4000)
//#define HAL_CODEFLASH_ADDR211               (/*地址自定义*/0x001A6000)
//#define HAL_CODEFLASH_ADDR212               (/*地址自定义*/0x001A8000)
//#define HAL_CODEFLASH_ADDR213               (/*地址自定义*/0x001AA000)
//#define HAL_CODEFLASH_ADDR214               (/*地址自定义*/0x001AC000)
//#define HAL_CODEFLASH_ADDR215               (/*地址自定义*/0x001AE000)
//
//#define HAL_CODEFLASH_ADDR216               (/*地址自定义*/0x001B0000)
//#define HAL_CODEFLASH_ADDR217               (/*地址自定义*/0x001B2000)
//#define HAL_CODEFLASH_ADDR218               (/*地址自定义*/0x001B4000)
//#define HAL_CODEFLASH_ADDR219               (/*地址自定义*/0x001B6000)
//#define HAL_CODEFLASH_ADDR220               (/*地址自定义*/0x001B8000)
//#define HAL_CODEFLASH_ADDR221               (/*地址自定义*/0x001BA000)
//#define HAL_CODEFLASH_ADDR222               (/*地址自定义*/0x001BC000)
//#define HAL_CODEFLASH_ADDR223               (/*地址自定义*/0x001BE000)
//
//#define HAL_CODEFLASH_ADDR224               (/*地址自定义*/0x001C0000)
//#define HAL_CODEFLASH_ADDR225               (/*地址自定义*/0x001C2000)
//#define HAL_CODEFLASH_ADDR226               (/*地址自定义*/0x001C4000)
//#define HAL_CODEFLASH_ADDR227               (/*地址自定义*/0x001C6000)
//#define HAL_CODEFLASH_ADDR228               (/*地址自定义*/0x001C8000)
//#define HAL_CODEFLASH_ADDR229               (/*地址自定义*/0x001CA000)
//#define HAL_CODEFLASH_ADDR230               (/*地址自定义*/0x001CC000)
//#define HAL_CODEFLASH_ADDR231               (/*地址自定义*/0x001CE000)
//
//#define HAL_CODEFLASH_ADDR232               (/*地址自定义*/0x001D0000)
//#define HAL_CODEFLASH_ADDR233               (/*地址自定义*/0x001D2000)
//#define HAL_CODEFLASH_ADDR234               (/*地址自定义*/0x001D4000)
//#define HAL_CODEFLASH_ADDR235               (/*地址自定义*/0x001D6000)
//#define HAL_CODEFLASH_ADDR236               (/*地址自定义*/0x001D8000)
//#define HAL_CODEFLASH_ADDR237               (/*地址自定义*/0x001DA000)
//#define HAL_CODEFLASH_ADDR238               (/*地址自定义*/0x001DC000)
//#define HAL_CODEFLASH_ADDR239               (/*地址自定义*/0x001DE000)
//
//#define HAL_CODEFLASH_ADDR240               (/*地址自定义*/0x001E0000)
//#define HAL_CODEFLASH_ADDR241               (/*地址自定义*/0x001E2000)
//#define HAL_CODEFLASH_ADDR242               (/*地址自定义*/0x001E4000)
//#define HAL_CODEFLASH_ADDR243               (/*地址自定义*/0x001E6000)
//#define HAL_CODEFLASH_ADDR244               (/*地址自定义*/0x001E8000)
//#define HAL_CODEFLASH_ADDR245               (/*地址自定义*/0x001EA000)
//#define HAL_CODEFLASH_ADDR246               (/*地址自定义*/0x001EC000)
//#define HAL_CODEFLASH_ADDR247               (/*地址自定义*/0x001EE000)
//
//#define HAL_CODEFLASH_ADDR248               (/*地址自定义*/0x001F0000)
//#define HAL_CODEFLASH_ADDR249               (/*地址自定义*/0x001F2000)
//#define HAL_CODEFLASH_ADDR250               (/*地址自定义*/0x001F4000)
//#define HAL_CODEFLASH_ADDR251               (/*地址自定义*/0x001F6000)
//#define HAL_CODEFLASH_ADDR252               (/*地址自定义*/0x001F8000)
//#define HAL_CODEFLASH_ADDR253               (/*地址自定义*/0x001FA000)
//#define HAL_CODEFLASH_ADDR254               (/*地址自定义*/0x001FC000)
//#define HAL_CODEFLASH_ADDR255               (/*地址自定义*/0x001FE000)

//#define HAL_NVM_ALL_SIZE              (0UL << 10U)//空间大小64K
//
//#define HAL_NVM_ADDR_ARR_LEN          (1U)//存放地址的数组长度
//#define HAL_NVM_ADDR0                 (/*地址自定义*/0xffffffffUL)
//#define HAL_NVM_ADDR1                 (/*地址自定义*/0xffffffffUL)
//#define HAL_NVM_ADDR2                 (/*地址自定义*/0xffffffffUL)
//#define HAL_NVM_ADDR3                 (/*地址自定义*/0xffffffffUL)
//#define HAL_NVM_ADDR4                 (/*地址自定义*/0xffffffffUL)
//#define HAL_NVM_ADDR5                 (/*地址自定义*/0xffffffffUL)

///*dataflash存放地址的数组定义*/
//const  uint32_t g_u32_dataflash_addr_a[HAL_DATAFLASH_ADDR_ARR_LEN];
///*codeflash存放地址的数组定义*/
//const  uint32_t g_u32_codeflash_addr_a[HAL_CODEFLASH_ADDR_ARR_LEN];
///*nvm存放地址的数组定义*/
//const  uint32_t g_u32_nvm_addr_a[HAL_NVM_ADDR_ARR_LEN];
///*  Function    End     */

/*
 * 函 数 介 绍: 获得dflash配置表
 * 参       数
 *
 * 返   回  值:
 * 备       注:
 */
void hal_dflash_get_cfg(uint32_t **dflashcfg , uint32_t *len);
/*
 * 函 数 介 绍: 获得cflash配置表
 * 参       数
 *
 * 返   回  值:
 * 备       注:
 */
void hal_cflash_get_cfg(uint32_t **cflashcfg , uint32_t *len);
/*
 * 函 数 介 绍: 获得nvm配置表
 * 参       数
 *
 * 返   回  值:
 * 备       注:
 */
void hal_nvm_get_cfg(uint32_t **nvm , uint32_t *len);

#endif
