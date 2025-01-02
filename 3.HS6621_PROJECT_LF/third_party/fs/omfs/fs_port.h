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
 * @file     fs_port.h
 * @brief 
 * @date     16 July 2021
 * @author   OnMicro SW Team
 *
 * @defgroup OMFS OMFS
 * @ingroup  COMPONENTS
 * @brief    Porting for OnMicro filesystem
 * @details  OnMicro filesystem 
 *
 * @version:
 * Version 1.0
 *  - Initial release
 *
 * @{
 */
#ifndef __FS_PORT_H
#define __FS_PORT_H

extern void fs_flash_init(void);
extern uint8_t fs_flash_is_read_done(void);
extern void fs_flash_read(uint32_t address, uint32_t length, uint8_t *buf);
extern void fs_flash_write(uint32_t address, uint32_t length, uint8_t *buf);
extern void fs_flash_erase(uint32_t address, uint32_t length);

#endif  // _FS_PORT_H_


/* End Of File -------------------------------------------------------------- */

