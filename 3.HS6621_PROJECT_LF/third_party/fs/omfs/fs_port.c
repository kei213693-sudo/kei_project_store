/* ----------------------------------------------------------------------------
 * Copyright (c) 2020-2030 OnMicro Limited. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of OnMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -------------------------------------------------------------------------- */

/**
 * @file     fs_port.c
 * @brief    Porting for OnMicro filesystem
 * @date     16 July 2021
 * @author   OnMicro SW Team
 *
 * @version:
 * Version 1.0
 *  - Initial release
 *
 * @{
 */

/*******************************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include "peripheral.h"

static CO_ALIGN(16) uint8_t fs_temp_mem[928];    //454*2+15+16

typedef struct {
    void *buf;
    uint32_t address;
    uint16_t length;
    volatile uint8_t  is_reading;
    volatile uint32_t need_copy;
    uint8_t* buf_before;
    uint8_t* buf_after;
    uint8_t len_before;
    uint8_t len_after;
} fs_port_env_t;
volatile fs_port_env_t fs_port_env;


void fs_callback(HS_SF_Type *sf)
{
    if(fs_port_env.need_copy){
        memcpy(fs_port_env.buf, (uint8_t *)((uint32_t)fs_temp_mem + (fs_port_env.address & 0x0F)), fs_port_env.length);
    } else {
        if(fs_port_env.len_before)
            memcpy(fs_port_env.buf_before,fs_temp_mem,fs_port_env.len_before);
        if(fs_port_env.len_after)
            memcpy(fs_port_env.buf_after,fs_temp_mem+fs_port_env.len_before,fs_port_env.len_after);
        fs_port_env.len_before = 0;
        fs_port_env.len_after = 0;
    }
    fs_port_env.is_reading = 0;
}

/*******************************************************************************
 * PUBLIC FUNCTIONS
 */
void fs_flash_init(void)
{
    HS_SF_Type *sf = HS_SF1;
    
    fs_port_env.is_reading = 0;
    sfb_dma_done_event_register(sf, fs_callback);
}

uint8_t fs_flash_is_read_done(void)
{
    return !(fs_port_env.is_reading);
}

void fs_flash_read(uint32_t address, uint32_t length, uint8_t *buf)
{
//#ifdef CONFIG_OMFS_IN_FLASH
//    flash_read(OM_FLASH0, address, buf, length);
//#endif

//#ifdef CONFIG_OMFS_OUT_FLASH
//    flash_read(OM_FLASH1, address, buf, length);
//#endif
    while(fs_port_env.is_reading);
    uint32_t addr_align16;
    uint32_t len_align16;

    addr_align16 = address & 0xFFFFFFF0;
    len_align16 = ((address - addr_align16) + length + 15) & 0xFFFFFFF0;
    if (len_align16 <= sizeof(fs_temp_mem)) {
        fs_port_env.need_copy = 1;
        fs_port_env.buf = buf;
        fs_port_env.is_reading = 1;
        fs_port_env.address = address;
        fs_port_env.length = length;
        sf_read_fast_quad_naked_dma(HS_SF1, 0, addr_align16, fs_temp_mem, len_align16);
    } else {
        co_assert(0);
    }
}

// suppose the buf address before and after has 15 bytes to extened
void fs_flash_read_no_copy(uint32_t address, uint32_t length, uint8_t *buf)
{
    while(fs_port_env.is_reading);
    
    
    
    uint32_t buf_aligned16 = (uint32_t)buf & 0xFFFFFFF0; 
    uint32_t fixed_address = address - ((uint32_t)buf - buf_aligned16);
    uint32_t fixed_length = length + ((uint32_t)buf - buf_aligned16);
    uint32_t len_align16 = (fixed_length + 15) & 0xFFFFFFF0;
    
    uint32_t len_add_before = (uint32_t)buf - buf_aligned16;
    uint32_t len_add_after  = len_align16 - fixed_length;
    if(len_add_before) {
        memcpy(fs_temp_mem,(void*)buf_aligned16,len_add_before);
        fs_port_env.buf_before = (void*)buf_aligned16;
        fs_port_env.len_before = len_add_before;
    }
    if(len_add_after) {
        memcpy(fs_temp_mem+len_add_before,(uint8_t*)buf+length,len_add_after);
        fs_port_env.buf_after = (uint8_t*)buf+length;
        fs_port_env.len_after = len_add_after;
    }
    fs_port_env.is_reading=1;
    fs_port_env.need_copy = 0;
    sf_read_fast_quad_naked_dma(HS_SF1, 0, fixed_address, (void*)buf_aligned16, len_align16);
}


void fs_flash_write(uint32_t address, uint32_t length, uint8_t *buf)
{
//#ifdef CONFIG_OMFS_IN_FLASH
//    flash_write(OM_FLASH0, address, buf, length);
//#endif

//#ifdef CONFIG_OMFS_OUT_FLASH
//    flash_write(OM_FLASH1, address, buf, length);
//#endif
    HS_SF_Type *sf = HS_SF1;
    sf_write(sf, 0, address, buf, length);
}

void fs_flash_erase(uint32_t address, uint32_t length)
{
//#ifdef CONFIG_OMFS_IN_FLASH
//    flash_erase(OM_FLASH0, FLASH_ERASE_TYPE_SECTOR, address);
//#endif

//#ifdef CONFIG_OMFS_OUT_FLASH
//    flash_erase(OM_FLASH1, FLASH_ERASE_TYPE_SECTOR, address);
//#endif
}


/** @} */
