#!/usr/bin/python3
# coding: utf-8

import argparse
import sys
import openpyxl

class FaultInfo(object):
    def __init__(self):
        self.confirmCycle = '0u'
        self.healingCycle = '0u'
        self.reconfirmCycle = '0u'
        self.faultclass = 'DFPM_Class_C'
        self.dtc = '0u'
        self.desc = ''
        self.snapshotGroup = '0u'
        self.spn = '0u'
        self.fmi = '0u'
        self.enable = 'FALSE'
		
    def clear(self):
        self.confirmCycle = '0u'
        self.healingCycle = '0u'
        self.reconfirmCycle = '0u'
        self.faultclass = 'DFPM_Class_C'
        self.dtc = '0u'
        self.snapshotGroup = '0u'
        self.spn = '0u'
        self.fmi = '0u'
        self.enable = 'FALSE'

    def parse_info(self, row):        

        if row[6].value == '立即点亮MIL':
            self.confirmCycle = '1u'
        elif row[6].value == '第二个驾驶循环点亮MIL':
            self.confirmCycle = '2u'
        else:
            self.confirmCycle = '2u'

        self.healingCycle = str(row[7].value)+'u'
        self.reconfirmCycle = str(row[8].value)+'u'

        if row[9].value == 'A类故障':
            self.faultclass = 'DFPM_Class_A'
        elif row[9].value == 'B1类故障':
            self.faultclass = 'DFPM_Class_B1'
        elif row[9].value == 'B2类故障':
            self.faultclass = 'DFPM_Class_B2'
        elif row[9].value == 'C类故障':
            self.faultclass = 'DFPM_Class_C'
        else:
            self.faultclass = 'DFPM_Class_C'

        self.desc = row[4].value
        dtc = row[2].value + str(row[3].value)

        if dtc[0] == 'P':   
            block = int(dtc[1]) + 0
        elif dtc[0] == 'C': 
            block = int(dtc[1]) + 4
        elif dtc[0] == 'B':
            block = int(dtc[1]) + 8
        elif dtc[0] == 'U':
            block = int(dtc[1]) + 12

        block_str = hex(block) 
        self.dtc = block_str.upper() + dtc[2:].upper() + 'u'  
        self.snapshotGroup = str(row[10].value)+'u'
        self.spn = str(row[11].value)+'u'
        self.fmi = str(row[12].value)+'u'  
        
        if row[13].value == 'Enable':
            self.enable = 'TRUE'
        else:
            self.enable = 'FALSE'        
        

    def print_info(self, dfp, f):  
        print("    {{ {:8}, {:8}, {:8}, {:15}, {:12}, {:3}, {:5}, {:3}, {:7}}}, /* Dfp = {:3}, {} */"\
                    .format(self.confirmCycle,\
                    self.healingCycle, \
                    self.reconfirmCycle,\
                    self.faultclass, \
                    self.dtc,\
                    self.snapshotGroup,\
                    self.spn,\
                    self.fmi,\
                    self.enable,\
                    dfp,\
                    self.desc), file= f)

def print_infofile(InFile, OutFilePath, OutFile):
    wb = openpyxl.load_workbook(InFile)
    #sheet1 = wb.get_sheet_by_name('FaultList')  # get_sheet_by_name('Sheet1')
    sheet1 =  wb["FaultList"]

    ''' outpuf C file '''
    of = open(OutFilePath + '\\' + OutFile+'.c', "w")
    print('/* This file was automatically generated */',  file = of)
    print(file = of)
    print('#include "dfpm_priv.h"', file = of)
    print(file = of)
    print("#pragma section CONST \".dataset\" R", file = of)
    print('const DFPM_FaultInfo    FaultList[] = ', file = of) 
    print('{', file = of)
    print('/* confirm   healing   reconfirm        class          dtc     snapshot spn    fmi   enable           */', file = of)
    print('    { 0u      , 0u      , 0u      , DFPM_Class_C   , 0X000000u   , 0u , 0u   , 0u , FALSE}, /* unuse item */', file = of)
    info = FaultInfo()
    dfpnum = 1

    for row in range(2, sheet1.max_row+1):
        if sheet1.cell(row, 2).value != "" and sheet1.cell(row, 2).value != None and \
           sheet1.cell(row, 3).value != "" and sheet1.cell(row, 3).value != None and \
           sheet1.cell(row, 4).value != "" and sheet1.cell(row, 4).value != None: 
            info.parse_info(sheet1[row])  
            info.print_info(dfpnum,of)
            dfpnum = dfpnum +1
            info.clear()

    print('};', file = of)
    print("#pragma section CONST", file = of)
    '''
    if sheet1.cell(2, 13).value == '有差别系统':
        print('const uint8_t   MI_DISPLY_STRATEGY = 1u;', end ='', file = of)
    else:
        print('const uint8_t   MI_DISPLY_STRATEGY = 0u;', end ='', file = of)

    print(' /*!< display strategy, 0: Non-Discriminatory,  1:Discriminatory*/', file = of)

    print('uint32_t  TIME_ACC_FACTOR = {}u;'.format(sheet1.cell(2, 14).value),end ='', file = of)
    print('   /*!< time acceleration factor */', file = of)
    '''
    
    of.close()
    print('output {}'.format(of.name))

    ''' outpuf H file '''
    of = open(OutFilePath + '\\' + OutFile + '.h', "w")
    print('#ifndef '+OutFile.upper()+'_H', file=of)
    print('#define '+OutFile.upper()+'_H', file=of)
    #print('#include "dfpm_priv.h"', file=of)
    print(file=of)
    print('#define DFPM_DFP_NUM {}u  /*!< dfp total number */'.format(dfpnum), file=of)
    print(file=of)
    print('#endif', file=of)
    of.close()
    print('output {}'.format(of.name))
    
    
    ''' dfc file  '''
    of = open(OutFilePath + '\\' +'dfc_type_init.c', "w")    
    print('/* This file was automatically generated */',  file = of)
    print(file = of)
    print('#include "dfc_type_init.h"', file = of) 
    print('#include "DFP.h"', file = of) 
    print(file = of)
    print('void dfc_type_init(void)', file = of) 
    print('{', file = of)  

    
    dfc_var = ''
    dfc_enum = ''
    for row in range(2, sheet1.max_row+1): 
        if sheet1.cell(row, 2).value != "" and sheet1.cell(row, 2).value != None and \
           sheet1.cell(row, 3).value != "" and sheet1.cell(row, 3).value != None and \
           sheet1.cell(row, 4).value != "" and sheet1.cell(row, 4).value != None: 
            dfc_var = sheet1.cell(row, 2).value
            dfc_enum = 'DFP_' + dfc_var.upper()            
            print('    {}.DSM_DFCTYPE_IMPL_IRAM->DFCType = {}; '.format(dfc_var, dfc_enum), file = of)
    
    print('}', file = of) 
    of.close()
    print('output {}'.format(of.name))
    

    of = open(OutFilePath + '\\' +'dfc_type_init.h', "w")  
    print('/* This file was automatically generated */',  file = of)
    print(file = of)
    print('#ifndef DFC_TYPE_INIT_H', file=of)
    print('#define DFC_TYPE_INIT_H', file=of)
    print(file=of)    

    print('typedef enum', file = of) 
    print('{', file = of)     
    print('    DFP_DFC_UNUSE= 0u,', file = of)
    dfc_var = ''
    dfc_enum = ''
    for row in range(2, sheet1.max_row+1): 
        if sheet1.cell(row, 2).value != "" and sheet1.cell(row, 2).value != None and \
           sheet1.cell(row, 3).value != "" and sheet1.cell(row, 3).value != None and \
           sheet1.cell(row, 4).value != "" and sheet1.cell(row, 4).value != None:   
            dfc_var = sheet1.cell(row, 2).value
            dfc_enum = '    DFP_' + dfc_var.upper() +',' 
            print(dfc_enum, file = of)
    
    print('}DFC_Type;', file = of) 
    
    print('extern void dfc_type_init(void);', file=of) 
    print('#endif', file=of)   
    of.close()
    print('output {}'.format(of.name))


def main():
    parser = argparse.ArgumentParser(
        prog="Parse_FaultInfo.py",
        usage="%(prog)s [options] [command]",
        description="Generate fault infomation configuration file"
    )
    parser.add_argument(
        "-if",
        dest="InputFileName",
        help="Specifies the alternate function file for the chip",
        default="FaultInfoCfg.xlsx"
    )

    parser.add_argument(
        "-of",
        dest="OutputFileName",
        help="Specifies the alternate function file for the chip",
        default="dfpm_dfpInfo"
    )
    
    parser.add_argument(
        "-ofPath",
        dest="OutputFilePath",
        help="Specifies the alternate function file for the chip",
        default=".\\"
    )

    args = parser.parse_args(sys.argv[1:])
    if args.InputFileName and args.OutputFileName:
        print_infofile(args.InputFileName, args.OutputFilePath, args.OutputFileName)
        print('Conversion  completed')


if __name__ == "__main__":
    main()

