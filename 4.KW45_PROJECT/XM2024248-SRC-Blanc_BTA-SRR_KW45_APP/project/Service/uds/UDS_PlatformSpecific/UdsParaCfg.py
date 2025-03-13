#!/usr/bin/python3

import argparse
import sys
import openpyxl

class DidInfo(object):
    def __init__(self):
        self.did = ''
        self.size = ''
        self.AccessAttribute = ''
        self.pDataBuf = ''
        self.pFuncPtrToReadData = ''
        self.pFuncPtrToWriteData = ''
        self.pFuncPtrToWriteCheck = ''
        self.desc = ''

    def clear(self):
        self.did = ''
        self.size = ''
        self.AccessAttribute = ''
        self.pDataBuf = ''
        self.pFuncPtrToReadData = ''
        self.pFuncPtrToWriteData = ''
        self.pFuncPtrToWriteCheck = ''
        self.desc = ''

    def parse_info(self, row):

        if row[4].value == 'Y' or row[5].value == 'Y':
            attribute = 0x0000
            self.did = '0x' + row[1].value + 'u'
            self.desc = row[2].value
            self.size = str(row[3].value) + 'u'
            self.pDataBuf = 'rub_DidDataOperBuf'
            if row[4].value == 'Y':
                self.pFuncPtrToReadData = 'UDS_itf_readDid_'+ row[1].value
                attribute = 0x0001
                # session mode
                if row[6].value == 'Y':
                    attribute |= 0x0000
                if row[7].value == 'Y':
                    attribute |= 0x0200
                if row[8].value == 'Y':
                    attribute |= 0x0400
                if row[9].value == 'Y':
                    attribute |= 0x0800
                # Security Leve
                if row[10].value == 'Y':
                    attribute |= 0x0000
                if row[11].value == 'Y':
                    attribute |= 0x0008
                if row[12].value == 'Y':
                    attribute |= 0x0010
                if row[13].value == 'Y':
                    attribute |= 0x0020
            else:
                self.pFuncPtrToReadData = ''

            if row[5].value == 'Y':
                self.pFuncPtrToWriteData = 'UDS_itf_writeDid_'+ row[1].value
                self.pFuncPtrToWriteCheck = 'UDS_itf_writeCheck_'+ row[1].value
                attribute |= 0x0002
                # session mode
                if row[14].value == 'Y':
                    attribute |= 0x0000
                if row[15].value == 'Y':
                    attribute |= 0x1000
                if row[16].value == 'Y':
                    attribute |= 0x2000
                if row[17].value == 'Y':
                    attribute |= 0x4000
                # Security Leve
                if row[18].value == 'Y':
                    attribute |= 0x0000
                if row[19].value == 'Y':
                    attribute |= 0x0040
                if row[20].value == 'Y':
                    attribute |= 0x0080
                if row[21].value == 'Y':
                    attribute |= 0x0100
            else:
                self.pFuncPtrToWriteData = ''
                self.pFuncPtrToWriteCheck = ''
            self.AccessAttribute =  str('0x%04X' % attribute)
        else:
            self.clear()

    def print_info(self, f):
        if self.did != '':
            print("    {{ {}, {:4}, {}, {}, {} }}, /* {} */"\
                        .format(self.did,
                        self.size,
                        self.AccessAttribute,
                        self.pDataBuf,
                        self.pFuncPtrToReadData,
                        self.desc), file= f)

def print_DidInfoFile(InFile):
    wb = openpyxl.load_workbook(InFile)
    sheet1 =  wb["DIDList"]
    didtable = []

    ''' outpuf C file '''
    of = open('DidList.c', "w")
    print('/* This file was automatically generated */',  file = of)
    print(file=of)
    print('#include "DidList.h"', file = of)
    print('#include "UdsItf_ReadDid.h"', file=of)
    print('#include "UdsItf_WriteDid.h"', file=of)
    print(file=of)
    print('uint8_t rub_DidDataOperBuf[UDS_C_DID_DATA_OPERATE_BUF_SIZE];', file=of)
    print(file=of)
    print('#ifdef UDS_ENABLE_READDATABYIDENTIFIER', file=of)
    print(file=of)
    print('const UDS_DidDatMngRecd_t Uds_g_DidDataTable[] =', file = of)
    print('{', file = of)

    for row in range(4, sheet1.max_row+1):
        info = DidInfo()
        info.parse_info(sheet1[row])
        info.print_info(of)
        if info.did != '':
            didtable.append(info)

    print('    {UDS_C_DID_END_OF_TBL_FFFF, 0x00u, 0x0000u, NULL, NULL}/*This Line indicate the end of the data table */', file = of)
    print('};', file = of)
    print('#endif /* #ifdef UDS_ENABLE_READDATABYIDENTIFIER */', file=of)

    print(file=of)
    print('#ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER', file=of)
    print(file=of)
    print('const UDS_DidEepromDatMngRecd_t Uds_g_DidEepromDataTable[] =', file=of)
    print('{', file=of)

    for did in didtable:
        ''' writeable '''
        if (did.pFuncPtrToWriteData != ""):
            print("    {{ {}, {:4}, {}, 0, {}, {}, {} }}, /* {} */" \
                  .format(did.did,
                          did.size,
                          did.AccessAttribute,
                          did.pDataBuf,
                          did.pFuncPtrToWriteCheck,
                          did.pFuncPtrToWriteData,
                          did.desc), file=of)

    print('    {UDS_C_DID_END_OF_TBL_FFFF, 0x00u, 0x0000u, 0x00u, NULL, NULL, NULL}/*This Line indicate the end of the data table */', file=of)
    print('};', file=of)
    print('#endif /* #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER */', file=of)
    of.close()
    print('DidList.c')

    ''' outpuf H file '''
    of = open('DidList.h', "w")
    print('/* This file was automatically generated */', file=of)
    print(file=of)
    print('#ifndef DIDLIST_H', file=of)
    print('#define DIDLIST_H', file=of)
    print(file=of)
    print('#include "uds_confpriv.h"', file = of)
    print(file=of)
    print('extern uint8_t rub_DidDataOperBuf[];', file=of)
    print(file=of)

    print('#ifdef UDS_ENABLE_READDATABYIDENTIFIER', file=of)
    print(file=of)
    print('extern const UDS_DidDatMngRecd_t Uds_g_DidDataTable[];', file = of)
    print(file=of)
    print('#endif  /* #ifdef UDS_ENABLE_READDATABYIDENTIFIER  */', file=of)
    print(file=of)

    print('#ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER', file=of)
    print(file=of)
    print('extern const UDS_DidEepromDatMngRecd_t Uds_g_DidEepromDataTable[];', file = of)
    print(file=of)
    print('#endif  /* #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER  */', file=of)
    print(file=of)

    print('#endif', file=of)
    of.close()
    print('DidList.h')

    ''' outpuf C file '''
    of = open('UdsItf_ReadDid.c.tmp', "w")
    print(file=of)
    print('#include "string.h"', file=of)
    print('#include "uds_confpriv.h"', file=of)
    print(file=of)
    for did in didtable:
        print('/* {} */'.format(did.desc), file=of)
        print('void {}(uint8_t *i_pDataBufPtr)'.format(did.pFuncPtrToReadData), file=of)
        print('{', file=of)
        print('    (void) i_pDataBufPtr;', file=of)
        print('}', file=of)
        print(file=of)
    of.close()
    print('UdsItf_ReadDid.c.tmp')

    ''' outpuf H file '''
    of = open('UdsItf_ReadDid.h', "w")
    print(file=of)
    print('#ifndef UDSITF_READDID_H_', file=of)
    print('#define UDSITF_READDID_H_', file=of)
    print('#include "typedefs.h"', file=of)
    print(file=of)
    for did in didtable:
        print('extern void {}(uint8_t *i_pDataBufPtr);'.format(did.pFuncPtrToReadData), file=of)
    print(file=of)
    print('#endif', file=of)
    of.close()
    print('UdsItf_ReadDid.h')


    ''' outpuf C file '''
    of = open('UdsItf_WriteDid.c.tmp', "w")
    print(file=of)
    print('#include "string.h"', file=of)
    print('#include "uds_confpriv.h"', file=of)
    print(file=of)
    for did in didtable:
        if (did.pFuncPtrToWriteData != ""):
            print('/* {} */'.format(did.desc), file=of)
            print('uint8_t {}(uint8_t *i_pDataBufPtr)'.format(did.pFuncPtrToWriteCheck), file=of)
            print('{', file=of)
            print('    (void) i_pDataBufPtr;', file=of)
            print('    return UDS_POSITIVE_RESPONSE;', file=of)            
            print('}', file=of)
            print(file=of)

            print('void {}(uint8_t *i_pDataBufPtr)'.format(did.pFuncPtrToWriteData), file=of)
            print('{', file=of)
            print('    (void) i_pDataBufPtr;', file=of)
            print('}', file=of)
            print(file=of)
    of.close()
    print('UdsItf_WriteDid.c.tmp')

    ''' outpuf H file '''
    of = open('UdsItf_WriteDid.h', "w")
    print(file=of)
    print('#ifndef UDSITF_WRITEDID_H_', file=of)
    print('#define UDSITF_WRITEDID_H_', file=of)
    print('#include "typedefs.h"', file=of)
    print(file=of)
    for did in didtable:
        if (did.pFuncPtrToWriteData != ""):
            print('uint8_t {}(uint8_t *i_pDataBufPtr);'.format(did.pFuncPtrToWriteCheck), file=of)
            print('extern void {}(uint8_t *i_pDataBufPtr);'.format(did.pFuncPtrToWriteData), file=of)
            print(file=of)
    print(file=of)
    print('#endif', file=of)

    of.close()
    print('UdsItf_WriteDid.h')


def print_SnapshotInfoFile(InFile):
    wb = openpyxl.load_workbook(InFile, data_only=True)
    #sheet1 = wb["Snapshot_group0"]

    ''' outpuf C file '''
    of = open('SnapshotTable.c', "w")
    print('/* This file was automatically generated */', file=of)
    print(file=of)
    print('#include "string.h"', file=of)
    print('#include "SnapshotTable.h"', file=of)
    print(file=of)

    group_name = []
    count = []
    size = []

    for num in range(0,10):
        sheet_name = "Snapshot_group" + str(num)
        try:
            sheet = wb[sheet_name]
        except:
            group_name.append('NULL')
            count.append(0)
            size.append(0)
        else:
            print(file=of)
            print('static const uint16_t Uds_g_'+sheet_name+'[]=', file=of)
            print('{', file=of)
            totalSize = 0
            for row in range(2, sheet.max_row + 1):
                print('    0x{}u, /* {} */'.format(sheet[row][1].value, sheet[row][2].value), file=of)
                totalSize += int(sheet[row][3].value)
            print('};', file=of)
            group_name.append('Uds_g_' + sheet_name)
            count.append(sheet.max_row-1)
            size.append(totalSize)

    print(file=of)
    print('const Snapshot_Info snapshot_group_map[] =', file=of)
    print('{', file=of)
    for num in range(0, 10):
        print("    {{ {}, {}u, {}u }},".format(group_name[num], count[num], size[num]), file=of)
    print('};', file=of)
    of.close()

    ''' outpuf H file '''
    of = open('SnapshotTable.h', "w")
    print('/* This file was automatically generated */', file=of)
    print(file=of)
    print('#ifndef SNAPSHOTTABLE_H_', file=of)
    print('#define SNAPSHOTTABLE_H_', file=of)
    print('#include "typedefs.h"', file=of)
    print(file=of)
    content = '''\
    typedef  struct
    {
        const uint16_t  *group;
        uint16_t        count;
        uint32_t        length;
    }Snapshot_Info;
    '''
    print(content, file=of)
    print('    #define SNAPSHOT_TOTAL_GROUP_NUM {}u'.format(10), file=of)
    print('    extern const Snapshot_Info snapshot_group_map[];', file=of)
    print('    extern uint16_t *pSnapshot;', file=of)
    print(file=of)
    print('#endif', file=of)
    of.close()
    print('SnapshotTable.h')


def print_IODIDInfoFile(InFile):
    wb = openpyxl.load_workbook(InFile, data_only=True)
    of = open('Uds_IODIDCtl.c.tmp', "w")
    print('/* This file was automatically generated */', file=of)
    print('#include "uds_confpriv.h"', file=of)
    print('#include "Uds_IODIDCtl.h"', file=of)
    print('#include "uds_priv.h"', file=of)
    print(file=of)
    print('extern uint8_t rub_DidDataOperBuf[UDS_C_DID_DATA_OPERATE_BUF_SIZE];', file=of)
    print(file=of)
    print('#ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER', file=of)
    content = '''\
    /* 0x2F InputOutputControlByIdentifier 
       Note : this table is located in the rom, can not be changed dynamically
              if a new did which is need to control InputOutput , then must update this table 
              before build the whole of project again;
                     
     */
    ''';
    print(content, file=of);
    print('UDS_CONFIG_TYPE UDS_IoCtrlIdxTblRcd_t Uds_g_IoCtrlByDidIdxTable[] = {', file=of);
    #print('/*      pDataBuf                    IoCtrlBitmapMask               pFuncPtrToIoCtrl               */', file=of)
    print('/*      dataIdentifier             DataSize            AccessAttribute      StartIndex,  NumOfEntry    */ ', file=of);
    sheet = wb['IODID'];
    NumOfEntry =0;
    attribute = 4;
    StartIndex = 0;
    DID=0;
    DIDS=0;
    IODidDataOperBuf=0;
    for Line in range(2, sheet.max_row+1):
        StartIndex = StartIndex+NumOfEntry;
        NumOfEntry=0;
        attribute=4;
        for row1 in range(10,17):
            if(sheet[Line][row1].value != 'NULL'):
                NumOfEntry=NumOfEntry+1
        if sheet[Line][4].value == 'Y':
            attribute |= 0x0000
        if sheet[Line][5].value == 'Y':
            attribute |= 0x1000
        # Security Leve
        if sheet[Line][6].value == 'Y':
            attribute |= 0x0000
        if sheet[Line][7].value == 'Y':
            attribute |= 0x0040
        if sheet[Line][8].value == 'Y':
            attribute |= 0x0080
        if sheet[Line][9].value == 'Y':
            attribute |= 0x0100
        DID= sheet[Line][1].value;
        #length=sheet[Line][3].value;
        length = hex(int(sheet[Line][3].value))
        attribute1 = hex(int(attribute))
        StartIndex1 = hex(int(StartIndex))
        NumOfEntry1 = hex(int(NumOfEntry))
        IODidDataOperBuf=IODidDataOperBuf+int(sheet[Line][3].value);
        print('/*'+str(Line-2)+'u */{'+'0x'+str(DID)+'                    ,'+str(length)+'                 ,'+str(attribute1)+'               ,'+str(StartIndex1)+'         ,'+str(NumOfEntry1)+'},', file=of)

    print('/*This Line indicate the end of the data table , do not delete */', file=of)
    print('/*FFu*/{UDS_C_DID_END_OF_TBL_FFFF, 0x00u,              0x0000u,             0x00u,       0x00u     }', file=of)
    print('}; /* end of table : UDS_CONFIG_TYPE UDS_IoCtrlIdxTblRcd_t Uds_g_IoCtrlByDidIdxTable[]  */', file=of)
    
    print(file=of)
    print(file=of)
    print(file=of)
    MAX_NUM = StartIndex+1;
    print(MAX_NUM)
    
    print('UDS_CONFIG_TYPE UDS_IoCtrlMngtDatTblRcd_t Uds_g_IoCtrlByDidMngtTable[UDS_2FIOCTRL_MNGT_RCD_MAX_NUM] = {',file=of)
    print('/*      pDataBuf                    IoCtrlBitmapMask               pFuncPtrToIoCtrl               */', file=of)
    IoCtrlByDidMngtTableNum=0;
    for Line in range(2, sheet.max_row+1):
        NumOfEntry=0;
        for row1 in range(10,17):
            if(sheet[Line][row1].value != 'NULL'):
                NumOfEntry=NumOfEntry+1;
                print('/* '+str(IoCtrlByDidMngtTableNum)+'u */{rub_DidDataOperBuf,        0x'+sheet[Line][row1].value+',                          '+'UDS_ITF_IMP_IOCTRL_'+sheet[Line][1].value+'_'+sheet[Line][row1].value+'},',file=of)
                IoCtrlByDidMngtTableNum=IoCtrlByDidMngtTableNum+1;
                #if(IoCtrlByDidMngtTableNum==(MAX_NUM-1)):
                    #print('/* '+str(IoCtrlByDidMngtTableNum)+'u */{rub_DidDataOperBuf,        0x'+sheet[Line][row1].value+',                          '+'UDS_ITF_IMP_IOCTRL_'+sheet[Line][1].value+'_'+sheet[Line][row1].value+'}',file=of)
                if(IoCtrlByDidMngtTableNum==MAX_NUM):
                    print('}; /* end of table : UDS_CONFIG_TYPE UDS_IoCtrlMngtDatTblRcd_t Uds_g_IoCtrlByDidMngtTable[]  */',file=of)
    print(file=of)
    print(file=of)
    print(file=of)
    print('uint8_t rub_IODidDataBuf['+str(IODidDataOperBuf)+'];',file=of)
    print('uint8_t rub_IODidDataOperBuf['+str(MAX_NUM)+'];',file=of)
    print(file=of)
    print(file=of)
    print(file=of)
    print('#endif // end of #ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER', file=of)
    print(file=of)
    print(file=of)
    print(file=of)
    IoCtrlByDidMngtTableNum=0;
    rub_IODidDataOperBufNum=0;
    for Line in range(2, sheet.max_row+1):
        NumOfEntry=0;
        for row1 in range(10,17):
            if(sheet[Line][row1].value != 'NULL'):
                NumOfEntry=NumOfEntry+1;
                print(file=of)
                print(file=of)
                print('/****'+sheet[Line][2].value+'****/',file=of)
                print('void '+'UDS_ITF_IMP_IOCTRL_'+sheet[Line][1].value+'_'+sheet[Line][row1].value+'(uint8_t i_IoCtrlOption, uint8_t *iP_IoCtrlParam)',file=of)
                print('{',file=of)
                print('    rub_IODidDataOperBuf['+str(rub_IODidDataOperBufNum)+'] = i_IoCtrlOption;',file=of)
                for row2 in range(0,int(sheet[Line][3].value)):
                    IoCtrlByDidMngtTableNum=IoCtrlByDidMngtTableNum+row2
                    print('    rub_IODidDataBuf['+str(IoCtrlByDidMngtTableNum)+'] = iP_IoCtrlParam['+str(row2)+'];',file=of)
                print('}',file=of)
        IoCtrlByDidMngtTableNum=IoCtrlByDidMngtTableNum+1;
        rub_IODidDataOperBufNum = rub_IODidDataOperBufNum+1;
    print(file=of)
    print(file=of)
    print(file=of)
    of.close()
    print('automatically generated Uds_IODIDCtl.c is ok!')

    of = open('Uds_IODIDCtl.h', "w")
    print('/* This file was automatically generated */', file=of)
    print(file=of)
    print('#ifndef UDS_IODIDCTL_H_', file=of)
    print('#define UDS_IODIDCTL_H_', file=of)
    print('#include "typedefs.h"', file=of)
    print(file=of)
    print(file=of)
    print(file=of)
    print('#define UDS_2FIOCTRL_MNGT_RCD_MAX_NUM  '+str(MAX_NUM),file=of)
    print(file=of)
    print(file=of)
    print('extern UDS_CONFIG_TYPE UDS_IoCtrlIdxTblRcd_t Uds_g_IoCtrlByDidIdxTable[];',file=of)
    print('extern UDS_CONFIG_TYPE UDS_IoCtrlMngtDatTblRcd_t Uds_g_IoCtrlByDidMngtTable[UDS_2FIOCTRL_MNGT_RCD_MAX_NUM];',file=of)
    IoCtrlByDidMngtTableNum=0;
    for Line in range(2, sheet.max_row+1):
        NumOfEntry=0;
        for row1 in range(10,17):
            if(sheet[Line][row1].value != 'NULL'):
                NumOfEntry=NumOfEntry+1;
                print('extern void '+'UDS_ITF_IMP_IOCTRL_'+sheet[Line][1].value+'_'+sheet[Line][row1].value+'(uint8_t i_IoCtrlOption, uint8_t *iP_IoCtrlParam);',file=of)
                IoCtrlByDidMngtTableNum=IoCtrlByDidMngtTableNum+1;
    print(file=of)
    print('#endif', file=of)
    of.close()
    print('automatically generated Uds_IODIDCtl.h is ok!')



def print_RtDidInfoFile(InFile):
    wb = openpyxl.load_workbook(InFile, data_only=True)
    of = open('Uds_RtDIDCtl.c.tmp', "w")
    print('/* This file was automatically generated */', file=of)
    print('#include "uds_confpriv.h"', file=of)
    print('#include "Uds_RtDIDCtl.h"', file=of)
    print('#include "uds_priv.h"', file=of)
    print(file=of)
    print(file=of)
    print('#ifdef UDS_ENABLE_ROUTINECONTROL', file=of)
    content = '''\
    /* 0x31 RoutineControl service data Managing table 
        Note : this table is located in the rom, can not be changed dynamically
               if a new did which is need to perform routine control  , then must update this table 
               before build the whole of project again 
               for this table, due to the number of attributes is greater than 10 , so split as two rows. 
    */
    ''';
    print(content, file=of);
    print('UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t Uds_g_RtnCtrlMngtDataTable[] = {', file=of);
    #print('/*      pDataBuf                    IoCtrlBitmapMask               pFuncPtrToIoCtrl               */', file=of)
    print('/* routineIdentifier  AccessAttribute  ValidArgumentNum  RtnRunLimitTime   ExpectedPosResult  pFuncPtrStart                pFuncPtrStop                   pFuncPtrGetResult              pFuncPtrRtnCheck                pFuncPtrIsRtnFinished */ ', file=of);
    sheet = wb['Routine_DID'];
    NumOfEntry =0;
    attribute = 4;
    StartIndex = 0;
    DID=0;
    DIDS=0;
    IODidDataOperBuf=0;
    for Line in range(2, sheet.max_row+1):
        StartIndex = StartIndex+NumOfEntry;
        NumOfEntry=0;
        attribute=1;
        if sheet[Line][5].value == 'Y':
            attribute |= 0x0000
        if sheet[Line][6].value == 'Y':
            attribute |= 0x0100
        # Security Leve
        if sheet[Line][7].value == 'Y':
            attribute |= 0x0000
        if sheet[Line][8].value == 'Y':
            attribute |= 0x0010
        if sheet[Line][9].value == 'Y':
            attribute |= 0x0020
        if sheet[Line][10].value == 'Y':
            attribute |= 0x0040
        DID= sheet[Line][1].value;
        #length=sheet[Line][3].value;
        ValidArgumentNum = int(sheet[Line][3].value)
        RtnRunLimitTime = int(sheet[Line][4].value)
        attribute1 = hex(int(attribute))
        StartIndex1 = hex(int(StartIndex))
        NumOfEntry1 = hex(int(NumOfEntry))
        ExpectedPosResult=1;
        IODidDataOperBuf=IODidDataOperBuf+int(sheet[Line][3].value);
        print('/*'+str(Line-2)+'u */{'+'0x'+str(DID)+'        ,'+str(attribute1)+'             ,'+str(ValidArgumentNum)+'              ,'+str(RtnRunLimitTime)+'             ,'+str(ExpectedPosResult)+'                ,UDS_C_RID_FP_START_RTN_'+str(DID)+',  UDS_C_RID_FP_STOP_RTN_'+str(DID)+',  UDS_C_RID_FP_RESULT_RTN_'+str(DID)+',  UDS_C_RID_FP_CHECK_RTN_'+str(DID)+',  UDS_C_RID_FP_FINISH_RTN_'+str(DID)+'},', file=of)
    print('/* These last one Lines indicate the end of the data table , do not delete*/', file=of)
    print('/*FFu*/{0xFFFFu       , 0x0000u        , 0x00u         ,0x0000u         ,              0x00u,NULL                          ,NULL                         ,             NULL,      NULL,       NULL  }', file=of)
    print('}; /* end of table : UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t Uds_g_RtnCtrlMngtDataTable[]  */', file=of)
    print('#endif /* end of #ifdef UDS_ENABLE_ROUTINECONTROL */', file=of)
    for Line in range(2, sheet.max_row+1):
        DID= sheet[Line][1].value;
        print(file=of)
        print('uint8_t UDS_C_RID_FP_START_RTN_'+str(DID)+'(uint8_t *p_RtnCtrlOpt)', file=of)
        print('{', file=of)
        print(' (void)p_RtnCtrlOpt;', file=of)
        print(' return ;', file=of)
        print('}', file=of)
        print(file=of)
        print('void UDS_C_RID_FP_STOP_RTN_'+str(DID)+'(void)', file=of)
        print('{', file=of)
        print(' return ;', file=of)
        print('}', file=of)
        print(file=of)
        print('uint8_t UDS_C_RID_FP_RESULT_RTN_'+str(DID)+'(void)', file=of)
        print('{', file=of)
        print(' return ;', file=of)
        print('}', file=of)
        print(file=of)
        print('uint8_t UDS_C_RID_FP_CHECK_RTN_'+str(DID)+'(uint8_t *p_RtnCtrlOpt)', file=of)
        print('{', file=of)
        print(' (void)p_RtnCtrlOpt;', file=of)
        print(' return ;', file=of)
        print('}', file=of)
        print(file=of)
        print('uint8_t UDS_C_RID_FP_FINISH_RTN_'+str(DID)+'(void)', file=of)
        print('{', file=of)
        print(' return ;', file=of)
        print('}', file=of)
        print(file=of)
    of.close()
    print('automatically generated Uds_RtDIDCtl.c.tmp is ok!')

    of = open('Uds_RtDIDCtl.h', "w")
    print('/* This file was automatically generated */', file=of)
    print(file=of)
    print('#ifndef UDS_RTDIDCTL_H_', file=of)
    print('#define UDS_RTDIDCTL_H_', file=of)
    print('#include "typedefs.h"', file=of)
    print(file=of)
    print(file=of)
    print(file=of)
    print('extern UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t Uds_g_RtnCtrlMngtDataTable[];', file=of);
    for Line in range(2, sheet.max_row+1):
        DID= sheet[Line][1].value;
        print(file=of)
        print('extern uint8_t UDS_C_RID_FP_START_RTN_'+str(DID)+'(uint8_t *p_RtnCtrlOpt);', file=of)
        print('extern void UDS_C_RID_FP_STOP_RTN_'+str(DID)+'(void);', file=of)
        print('extern uint8_t UDS_C_RID_FP_RESULT_RTN_'+str(DID)+'(void);', file=of)
        print('extern uint8_t UDS_C_RID_FP_CHECK_RTN_'+str(DID)+'(uint8_t *p_RtnCtrlOpt);', file=of)
        print('extern uint8_t UDS_C_RID_FP_FINISH_RTN_'+str(DID)+'(void);', file=of)
        print(file=of)

    print('#endif', file=of)
    of.close()
    print('automatically generated Uds_RtDIDCtl.h is ok!')










def main():
    parser = argparse.ArgumentParser(
        prog="UdsParaCfg.py",
        usage="%(prog)s [options] [command]",
        description="Generate fault infomation configuration file"
    )
    parser.add_argument(
        "-if",
        dest="InputFileName",
        help="Specifies the alternate function file for the chip",
        default="UdsParaCfg.xlsx"
    )

    args = parser.parse_args(sys.argv[1:])
    if args.InputFileName:
        print_DidInfoFile(args.InputFileName)
        print_SnapshotInfoFile(args.InputFileName)
        print_IODIDInfoFile(args.InputFileName)
        print_RtDidInfoFile(args.InputFileName)


        print('Conversion  completed')

if __name__ == "__main__":
    main()

