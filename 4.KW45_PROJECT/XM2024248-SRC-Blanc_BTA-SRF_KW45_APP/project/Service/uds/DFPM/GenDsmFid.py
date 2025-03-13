#!/usr/bin/python3
# coding: utf-8

import argparse
import sys
import openpyxl

    
    
def print_info(row, f): 
    source = ""
    comment = ""
    tmp = ''
    for i in range(0, 31):        
        if row[i+4].value != None:
            value = 'DFP_'+row[i+4].value + ', ' 
        else:
            value = 'DFP_DFC_UNUSE, '
        tmp = "{:18}".format(value)
        source = source + tmp.upper()  
      
    if row[31+4].value != None:
        value = 'DFP_'+row[31+4].value
    else:
        value = 'DFP_DFC_UNUSE'        
    tmp = "{:18}".format(value)
    source = source + tmp.upper()    
      
      
    comment = row[2].value     
    print("    {{ {} }}, /* {} */".format(source, comment), file= f)

def print_fidfile(InFile, OutFilePath, OutFile):
    wb = openpyxl.load_workbook(InFile)
    #sheet1 = wb.get_sheet_by_name('FaultList')  # get_sheet_by_name('Sheet1')
    sheet1 =  wb["DINH"]

    ''' outpuf C file '''
    of = open(OutFilePath + '\\' + OutFile+'.c', "w")
    print('/* This file was automatically generated */',  file = of)
    print(file = of)
    print('#include "{}.h"'.format(OutFile), file = of)
    print('#include "dfc_type_init.h"'.format(OutFile), file = of)  
    print('#include "DINH.h"'.format(OutFile), file = of)  
    
    print(file = of)
    #print("#pragma section CONST \".dataset\" R", file = of)
    print('const uint16_t FID_inhabitTable[][32] = ', file = of) 
    print('{', file = of)
    print('    /* not use */', file = of)
    print('    { DFP_DFC_UNUSE,   DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,    DFP_DFC_UNUSE,  DFP_DFC_UNUSE,  DFP_DFC_UNUSE      },', file = of)

    fidnum = 1

    for row in range(2, sheet1.max_row+1):
        if sheet1.cell(row, 2).value != "" and sheet1.cell(row, 2).value != None:
            print('    /* {} */'.format(sheet1.cell(row, 2).value), file = of)
            print_info(sheet1[row], of)             
            fidnum = fidnum +1

    print('};', file = of)
    #print("#pragma section CONST", file = of)     
    print('const uint16_t DSM_FID_NUM = {}u;'.format(fidnum), file = of) 
    
    print('void DSM_fidInit(void)', file = of) 
    print('{', file = of) 
    
    fid_var = ''
    fid_enum = ''
    for row in range(2, sheet1.max_row+1): 
        if sheet1.cell(row, 2).value != "" and sheet1.cell(row, 2).value != None:
            fid_var = sheet1.cell(row, 2).value
            fid_enum = 'DSM_' + fid_var.upper()            
            print('    {}.DSM_FIDTYPE_IMPL_IRAM->FIdType = {}; '.format(fid_var, fid_enum), file = of)    
    print('}', file = of) 
    of.close()
    print('output {}'.format(of.name))
    
    
    
    ''' outpuf H file '''
    of = open(OutFilePath + '\\' + OutFile + '.h', "w")
    print('#ifndef '+OutFile.upper()+'_H', file=of)
    print('#define '+OutFile.upper()+'_H', file=of)
    print('#include "typedefs.h"', file=of)
    print(file=of)
    
    print('typedef enum', file = of) 
    print('{', file = of)     
    print('    DSM_FID_UNUSE= 0u,', file = of)
    fid_var = ''
    fid_enum = ''
    for row in range(2, sheet1.max_row+1): 
        if sheet1.cell(row, 2).value != "" and sheet1.cell(row, 2).value != None:
            fid_var = sheet1.cell(row, 2).value
            fid_enum = '    DSM_' + fid_var.upper() +',' 
            print(fid_enum, file = of)
    
    print('}Fid_Type;', file = of)     
    print('extern const uint16_t FID_inhabitTable[{}][32];'.format(fidnum), file=of)
    print('extern const uint16_t DSM_FID_NUM;', file=of)
    print('extern void DSM_fidInit(void);', file=of) 
    
    print(file=of)
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
        default="OBDFaultList.xlsx"
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
        print_fidfile(args.InputFileName, args.OutputFilePath, args.OutputFileName)
        print('Conversion  completed')


if __name__ == "__main__":
    main()

