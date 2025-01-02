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
 * @file     fs.h
 * @brief    OnMicro filesystem
 * @date     16 July 2021
 * @author   OnMicro SW Team
 *
 * @defgroup OMFS OMFS
 * @ingroup  COMPONENTS
 * @brief    Filesystem for OnMicro OM668x
 * @details  OnMicro filesystem
 *
 * @version:
 * Version 1.0
 *  - Initial release
 *
 * @{
 */


#ifndef __FS_H
#define __FS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>
#include <stdint.h>
#include "ff.h"
#include "fs_port.h"

/*******************************************************************************
 * MACROS
 */
// The maximum buffer size of file data
#define FS_MAX_FILE_BUF_SIZE    1024
// The maximum capacity size of filesystem
#define FS_MAX_CAPACITY         10240000
// The maximum length of file name (Don't change)
#define FS_MAX_FILE_NAME_LENGTH 20
// Get the total size of the specificed file
#define FS_FILE_SIZE(file)      ((file).file_size)
// Get the current value of file size
#define FS_TELL(file)           ((file).file_offset)
// Get tatal number of files under the directory
#define FS_FILE_NUM(dir)        ((dir).file_num)
// Start of the file
#define FS_SEEK_SET             0
// Current position
#define FS_SEEK_CUR             1
// End of the file
#define FS_SEEK_END             2

/*******************************************************************************
 * TYPEDEFS
 */
typedef uint32_t            fs_addr_len_t;
typedef enum FS_FILE_TYPE   fs_file_type_t;
typedef struct fs_file      fs_file_t;
typedef struct fs_dir       fs_dir_t;
typedef struct fs_info      fs_info_t;

/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */
// File type
enum FS_FILE_TYPE
{
    FS_DIR,
    FS_SBC,
    FS_BPM,
    FS_WAV,
    FS_FILE = 0x0F
};

/*
 * STRUCT DEFINITIONS
 ****************************************************************************************
 */
// File object
struct fs_file
{
    fs_addr_len_t   file_addr;
    uint32_t        file_size;
    uint32_t        file_offset;
    fs_file_type_t  file_type;
};
// Directory object
struct fs_dir
{
    fs_addr_len_t   dir_addr;
    fs_addr_len_t   sub_file_addr;
    uint8_t         dir_level;
    uint8_t         file_num;
};
// File information
struct fs_info
{
    uint8_t         name_len;
    char            name[FS_MAX_FILE_NAME_LENGTH];
};

/*******************************************************************************
 * EXTERN FUNCTIONS
 */
/**
 * @brief Mount a filesystem
 *
 * @param[in] base_addr   The filesystem's base address in flash
 *
 * @return errno
 **/
FRESULT omfs_mount(fs_addr_len_t base_addr);

/**
 * @brief Open the file whose name is the string pointed to by the path
 *
 * @param[out] file   Upon successful, return a file pointer
 * @param[in]  path   Point to the path of the specific file
 *
 * @return errno
 **/
FRESULT omfs_open(fs_file_t* file, const char* path);

/**
 * @brief Set the file position
 *
 * @param[out] file   Upon successful, return a file pointer
 * @param[in]  offset Offset bytes to the specified position
 * @param[in]  whence Specify the position
 *
 * @return errno
 **/
FRESULT omfs_seek(fs_file_t* file, uint32_t offset, uint32_t whence);

/**
 * @brief Read the specificed file
 *
 * @param[in]  file    Specified file pointer
 * @param[out] file'   Upon successful, return a file pointer
 * @param[out] buf     A pointer to the buffer allocated by the caller to be filled with
 *                     the DATA part of the file
 * @param[in]  length  Expect read length
 * @param[out] length' Real readed length
 *
 * @return errno
 **/
FRESULT omfs_read(fs_file_t* file, void* buf, uint32_t* length);

/**
 * @brief Unmount current filesystem
 *
 * @return none
 **/
void omfs_unmount(void);

/**
 * @brief Open the directory whose name is the string pointed to by the path
 *
 * @param[out] dir    Upon successful, return a directory pointer
 * @param[in]  path   Point to the path of the specific file
 *
 * @return errno
 **/
FRESULT omfs_opendir(fs_dir_t* dir, const char* path);

/**
 * @brief Read the specificed directory
 *
 * @param[out] dir       Upon successful, return a directory pointer
 * @param[out] info      Point to an information of file that under the directory
 * @param[in]  file_num  Expect read file number
 * @param[out] file_num' Real readed file number
 *
 * @return errno
 **/
FRESULT omfs_readdir(fs_dir_t* dir, fs_info_t* info, uint8_t* file_num);

/// @} FS
#ifdef __cplusplus
}
#endif

#endif // __FS_H_

/** @} */
