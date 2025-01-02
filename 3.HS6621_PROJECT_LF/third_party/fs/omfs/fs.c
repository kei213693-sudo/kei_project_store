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
 * @file     fs.c
 * @brief    Filesystem for OnMicro OM668x
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

#include <string.h>
#include "fs.h"

/*******************************************************************************
 * MACROS
 */

// Define magic length
#define FS_MAGIC_NUMBER_LENGTH       4
// Define flash page size
#define FS_FLASH_PAGE_SIZE           0x100
// Define invalid flash address
#define FS_INVALID_ADDRESS           0xFFFFFFFF
// Get the file type
#define FS_GET_FILE_TYPE(file)      (fs_file_type_t)(file & 0x0F)
// Get the directory level of the current file
#define FS_GET_DIR_LEVEL(file)      ((file >> 4)& 0x0F)
// Define file header length
#define FS_FILE_HEADER_LENGTH       (sizeof(struct fs_header))
// Define MBR information length
#define FS_MBR_LENGTH               (sizeof(struct fs_mbr))
// Check invalid character in path
#define FS_CHECK_CHAR(c)     \
((c == '\\') || (c == '"') || (c == '|') || (c == ':') ||   \
(c == '*') || (c == '<') || (c == '>') || (c == '?'))
// Read flash interface for porting
#define FS_READ(address, buf, length)       fs_flash_read(address, length, buf)
/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

/*
 * STRUCT DEFINITIONS
 ****************************************************************************************
 */
// MBR information
struct fs_mbr
{
    uint32_t fs_date; /* lint -e754*/
    uint32_t fs_ver; /* lint -e754*/
    uint32_t fs_capacity;
};
// OMFS configuration
struct fs_config
{
    bool            is_mounted;
    fs_addr_len_t   start_addr;
    struct fs_mbr   info;
};
// File header information
struct fs_header
{
    uint8_t     file_prop;
    uint8_t     file_name_len; /* lint -e754*/
    uint16_t    reserved; /* lint -e754*/
    char        file_name[FS_MAX_FILE_NAME_LENGTH];
    uint32_t    file_size;
    uint32_t    next_addr;
};

/*******************************************************************************
 * CONST & VARIABLES
 */
// OMFS configuration
static struct fs_config fs_cfg;
// OMFS magic number
static const int32_t fs_magic_number = 0x53464d4f; /* OMFS */

/*******************************************************************************
 * LOCAL FUNCTIONS
 */
static inline void _fs_disk_read(uint32_t address,
                                 uint8_t* buf,
                                 uint32_t length)
{
    FS_READ(address, buf, length);
    while(!fs_flash_is_read_done());
}

static FRESULT _fs_validate(void* obj)
{
    fs_file_t* file;

    // Makesure filesystem was mounted
    if (!fs_cfg.is_mounted) {
        return FR_NO_FILESYSTEM;
    }

    if (obj == NULL) {
        return FR_INVALID_PARAMETER;
    }

    file = (fs_file_t*)obj;
    // If file was opened, check file address
    if (file->file_addr) {
        // The file address must be within valid range
        if (file->file_addr < fs_cfg.start_addr ||
            file->file_addr > (fs_cfg.start_addr + fs_cfg.info.fs_capacity)) {
            return FR_NO_FILE;
        }
    }

    return FR_OK;
}

static uint8_t _fs_get_dir_level(const char* path)
{
    uint8_t dir_level = 0, path_len, idx;

    path_len = (uint8_t)strlen(path);

    // Check if it is root directory
    if (path_len == 1) {
        return dir_level;
    }
    for (idx = 0; idx < path_len; idx++) {
        if (*(path + idx) == '/') {
            dir_level++;
        }
    }

    return dir_level;
}

static FRESULT _fs_check_path(const char* path)
{
    FRESULT ret = FR_INVALID_NAME;
    uint8_t path_len, name_len = 0, idx;

    path_len = (uint8_t)strlen(path);

    // In the path, '/' must be set to first and the last byte should not end with '/',
    // unless the path is points to root directory
    if (path_len == 0 || *path != '/' ||
        (*(path + path_len -1) == '/' && path_len !=  1)) {
        return ret;
    }

    for (idx = 1; idx < path_len; idx++) {
        // Disallow empty file name
        if (*(path + idx) == '/') {
            if (name_len == 0) {
                break;
            }
            // Entry a new directory, so clear file name
            name_len = 0;
            // Disallow invalid char
        } else if (FS_CHECK_CHAR(*(path + idx))) {
            break;
        } else {
            name_len++;
            // Check file name length
            if (name_len > FS_MAX_FILE_NAME_LENGTH) {
                break;
            }
        }
    }

    if (idx == path_len) {
        ret = FR_OK;
    }

    return ret;
}

static FRESULT _fs_get_file_info(const char* path,
                                 uint8_t dir_level,
                                 fs_info_t* info)
{
    FRESULT ret = FR_OK;
    uint8_t idx, path_len, cur_level = 0;

    info->name_len = 0;
    path_len = (uint8_t)strlen(path);

    for (idx = 0; idx < path_len; idx++) {
        if (*(path + idx) == '/') {
            cur_level++;
            if (cur_level <= dir_level) {
                continue;
            } else {
                break;
            }
        } else if (cur_level == dir_level) {
            info->name[info->name_len] = (char)(*(path + idx));
            info->name_len++;
        }
    }
    // If name length is 0, that means current directory level is overflow
    if (info->name_len == 0) {
        ret = FR_NO_FILE;
    }
    return ret;
}

static FRESULT _fs_browse_file(const char* path,
                               struct fs_header* file_hdr)
{
    FRESULT     ret = FR_NO_FILE;
    uint8_t     max_dir_level, cur_dir_level = 1;
    uint32_t    cur_addr;
    fs_info_t   info;

    max_dir_level = _fs_get_dir_level(path);
    // If directory level is 0, that means it's not a file
    if (max_dir_level == 0) {
        return ret;
    }

    if (_fs_get_file_info(path, cur_dir_level, &info) != FR_OK) {
        return ret;
    }
    cur_addr = fs_cfg.start_addr;

    while (cur_addr < (fs_cfg.start_addr + fs_cfg.info.fs_capacity)) {
        _fs_disk_read(cur_addr, (uint8_t*)file_hdr, FS_FILE_HEADER_LENGTH);
        // There is no specified file in current directory
        if (cur_dir_level != FS_GET_DIR_LEVEL(file_hdr->file_prop)) {
            break;
        } else if (!strncmp((char*)file_hdr->file_name,
                            info.name, info.name_len) &&
                    info.name_len == file_hdr->file_name_len) {
            // Found it
            if (cur_dir_level == max_dir_level &&
                FS_GET_FILE_TYPE(file_hdr->file_prop) != FS_DIR) {
                return FR_OK;
            // Go to the next directory
            } else if (cur_dir_level < max_dir_level &&
                       FS_GET_FILE_TYPE(file_hdr->file_prop) == FS_DIR) {
                cur_addr += FS_FILE_HEADER_LENGTH;
                cur_dir_level++;
                if (_fs_get_file_info(path, cur_dir_level, &info) != FR_OK) {
                    break;
                }
                continue;
            }
        }
        // Browse next file in current directory
        if (FS_GET_FILE_TYPE(file_hdr->file_prop) == FS_DIR) {
            cur_addr = file_hdr->next_addr;
        } else {
            cur_addr += FS_FILE_HEADER_LENGTH;
        }
    }
    return ret;
}
static FRESULT _fs_browse_dir(const char* path,
                              struct fs_header* file_hdr,
                              fs_addr_len_t* file_hdr_addr)
{
    FRESULT     ret = FR_NO_FILE;
    uint8_t     cur_dir_level = 1, max_dir_level;
    uint32_t    cur_addr, file_num;
    fs_info_t   info;

    max_dir_level = _fs_get_dir_level(path);
    cur_addr = fs_cfg.start_addr;

    if (max_dir_level == 0) {
        _fs_disk_read((cur_addr - FS_FLASH_PAGE_SIZE) +
                     FS_MAGIC_NUMBER_LENGTH + FS_MBR_LENGTH,
                     (uint8_t*)&file_num, 1);
        file_hdr->file_size = file_num;
        // directory level is 0 and file type is FS_DIR(0)
        file_hdr->file_prop = 0;
        *file_hdr_addr = fs_cfg.start_addr;
        return FR_OK;
    }

    if (_fs_get_file_info(path, cur_dir_level, &info) != FR_OK) {
        return ret;
    }

    while (cur_addr < (fs_cfg.start_addr + fs_cfg.info.fs_capacity)) {
        _fs_disk_read(cur_addr, (uint8_t*)file_hdr, FS_FILE_HEADER_LENGTH);
        // There is no specified file in current directory
        if (cur_dir_level != FS_GET_DIR_LEVEL(file_hdr->file_prop)) {
            break;
        } else if (!strncmp((char*)file_hdr->file_name,
                            info.name, info.name_len) &&
                    info.name_len == file_hdr->file_name_len) {
            if (FS_GET_FILE_TYPE(file_hdr->file_prop) == FS_DIR) {
                if (cur_dir_level == max_dir_level) {
                    *file_hdr_addr = cur_addr;
                    return FR_OK;
                } else {
                    cur_addr += FS_FILE_HEADER_LENGTH;
                    cur_dir_level++;
                    if (_fs_get_file_info(path, cur_dir_level, &info) != FR_OK) {
                        break;
                    }
                    continue;
                }
            }
         }
        // Browse next file in current directory
        if (FS_GET_FILE_TYPE(file_hdr->file_prop) == FS_DIR) {
            cur_addr = file_hdr->next_addr;
        } else {
            cur_addr += FS_FILE_HEADER_LENGTH;
        }
    }
    return ret;
}
/*******************************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief Mount a filesystem
 *
 * @param[in] base_addr   The filesystem's base address in flash
 *
 * @return errno
 **/
FRESULT omfs_mount(fs_addr_len_t base_addr)
{
    int32_t     magic = 0;
    uint32_t    buf[FS_MBR_LENGTH/4] = {0};

    // Check if already mounted
    if (fs_cfg.is_mounted == true &&
        base_addr == fs_cfg.start_addr - FS_FLASH_PAGE_SIZE) {
            return FR_OK;
    }

    fs_flash_init();

    // Check if filesystem is existed
    _fs_disk_read(base_addr, (uint8_t*)&magic, FS_MAGIC_NUMBER_LENGTH);
    if (magic != fs_magic_number) {
        return FR_NO_FILESYSTEM;
    }

    fs_cfg.is_mounted = true;
    fs_cfg.start_addr = base_addr + FS_FLASH_PAGE_SIZE;

    // Get MBR information
    _fs_disk_read(base_addr + FS_MAGIC_NUMBER_LENGTH,
                  (uint8_t*)buf, FS_MBR_LENGTH);

    memcpy(&fs_cfg.info, buf, FS_MBR_LENGTH);

    if (fs_cfg.info.fs_capacity > FS_MAX_CAPACITY) {
        fs_cfg.info.fs_capacity = FS_MAX_CAPACITY;
    }

    return FR_OK;
}

/**
 * @brief Open the file whose name is the string pointed to by the path
 *
 * @param[out] file   Upon successful, return a file pointer
 * @param[in]  path   Point to the path of the specific file
 *
 * @return errno
 **/
FRESULT omfs_open(fs_file_t* file, const char* path)
{
    FRESULT             ret;
    uint32_t            buf[FS_FILE_HEADER_LENGTH/4] = {0};
    struct fs_header*   file_hdr = (struct fs_header*)((uint8_t*)buf);

    // Makesure filesystem was mounted
    if (!fs_cfg.is_mounted) {
        return FR_NO_FILESYSTEM;
    }

    if (file == NULL || path == NULL) {
        return FR_INVALID_PARAMETER;
    }

    // Get directory level and check path format
    ret = _fs_check_path(path);
    if (ret != FR_OK) {
        return ret;
    }
    // Browse directory to find out specified file
    ret = _fs_browse_file(path, file_hdr);
    if (ret == FR_OK) {
        if (FS_GET_FILE_TYPE(file_hdr->file_prop) != FS_DIR) {
            file->file_addr = file_hdr->next_addr;
            file->file_size = file_hdr->file_size;
            file->file_offset = 0;
            file->file_type = FS_GET_FILE_TYPE(file_hdr->file_prop);
        } else {
            ret = FR_INVALID_NAME;
        }
    }

    return ret;
}

/**
 * @brief Set the file position
 *
 * @param[out] file   Upon successful, return a file pointer
 * @param[in]  offset Offset bytes to the specified position 
 * @param[in]  whence Specify the position
 *
 * @return errno
 **/
FRESULT omfs_seek(fs_file_t* file, uint32_t offset, uint32_t whence)
{
    FRESULT ret;

    ret = _fs_validate(file);
    if (ret != FR_OK) {
        return ret;
    }

    switch (whence) {
        case FS_SEEK_SET:
            if (offset <= file->file_size) {
                file->file_offset = offset;
            } else {
                ret = FR_INVALID_PARAMETER;
            }
            break;
        case FS_SEEK_CUR:
             if ((file->file_offset + offset) <= file->file_size) {
                file->file_offset += offset;
             } else {
                ret = FR_INVALID_PARAMETER;
             }
             break;
        case FS_SEEK_END:
            if (offset == 0) {
                file->file_offset = file->file_size;
            } else {
                ret = FR_INVALID_PARAMETER;
            }
            break;
        default:
            ret = FR_INVALID_PARAMETER;
            break;
    }
    return ret;
}

/**
 * @brief Set the file position
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
FRESULT omfs_read(fs_file_t* file, void* buf, uint32_t* length)
{
    FRESULT ret;

    ret = _fs_validate(file);
    if (ret != FR_OK) {
        return ret;
    }
    // Shall not input invalid parameters
    if (buf == NULL || *length == 0) {
        return FR_INVALID_PARAMETER;
    }

    // Makesure the real read length does not out of range
    if ((file->file_offset + *length) > file->file_size) {
        *length = file->file_size - file->file_offset;
        if (*length > FS_MAX_FILE_BUF_SIZE) {
            *length = FS_MAX_FILE_BUF_SIZE;
        }
    }

    //_fs_disk_read(file->file_addr + file->file_offset, (uint8_t*)buf, *length);
    fs_flash_read(file->file_addr + file->file_offset, *length, (uint8_t*)buf);
    file->file_offset += *length;

    return FR_OK;
}

void fs_flash_read_no_copy(uint32_t address, uint32_t length, uint8_t *buf);
FRESULT omfs_read_no_copy(fs_file_t* file, void* buf, uint32_t* length)
{
    FRESULT ret;

    ret = _fs_validate(file);
    if (ret != FR_OK) {
        return ret;
    }
    // Shall not input invalid parameters
    if (buf == NULL || *length == 0) {
        return FR_INVALID_PARAMETER;
    }

    // Makesure the real read length does not out of range
    if ((file->file_offset + *length) > file->file_size) {
        *length = file->file_size - file->file_offset;
        if (*length > FS_MAX_FILE_BUF_SIZE) {
            *length = FS_MAX_FILE_BUF_SIZE;
        }
    }

    //_fs_disk_read(file->file_addr + file->file_offset, (uint8_t*)buf, *length);
    fs_flash_read_no_copy(file->file_addr + file->file_offset, *length, (uint8_t*)buf);
    file->file_offset += *length;

    return FR_OK;
}


/**
 * @brief Unmount current filesystem
 *
 * @return none
 **/
void omfs_unmount(void)
{
    memset((uint8_t*)&fs_cfg, 0, FS_MBR_LENGTH);
}

/**
 * @brief Open the directory whose name is the string pointed to by the path
 *
 * @param[out] dir    Upon successful, return a directory pointer
 * @param[in]  path   Point to the path of the specific file
 *
 * @return errno
 **/
FRESULT omfs_opendir(fs_dir_t* dir, const char* path)
{
    FRESULT             ret;
    fs_addr_len_t       file_hdr_addr = 0;
    uint32_t            buf[FS_FILE_HEADER_LENGTH/4] = {0};
    struct fs_header*   file_hdr = (struct fs_header*)((uint8_t*)buf);

    // Makesure filesystem was mounted
    if (!fs_cfg.is_mounted) {
        return FR_NO_FILESYSTEM;
    }

    if (dir == NULL || path == NULL) {
        return FR_INVALID_PARAMETER;
    }

    // Get directory level and check path format
    ret = _fs_check_path(path);
    if (ret != FR_OK) {
        return ret;
    }

    // clear directory object
    memset((uint8_t*)dir, 0, sizeof(fs_dir_t));

    // Browse directory to find out specified file
    ret = _fs_browse_dir(path, file_hdr, &file_hdr_addr);
    if (ret == FR_OK) {
        if (FS_GET_FILE_TYPE(file_hdr->file_prop) == FS_DIR) {
            dir->dir_addr = file_hdr_addr;
            // If file type is FS_DIR, file_size is file number under the directory
            dir->file_num = (uint8_t)file_hdr->file_size;
            dir->dir_level = FS_GET_DIR_LEVEL(file_hdr->file_prop);
            memset((uint8_t*)buf, 0, FS_FILE_HEADER_LENGTH);
            // Get the sub file address under the directory
            if (dir->file_num) {
                // It is not root directory
                if (dir->dir_level) {
                    dir->sub_file_addr = file_hdr_addr + FS_FILE_HEADER_LENGTH;
                } else {
                    dir->sub_file_addr = file_hdr_addr;
                }
            } else {
                dir->sub_file_addr = 0;
            }
        } else {
            ret = FR_INVALID_NAME;
        }
    }

    return ret;
}

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
FRESULT omfs_readdir(fs_dir_t* dir, fs_info_t* info, uint8_t* file_num)
{
    FRESULT             ret;
    uint8_t             num = 0;
    uint32_t            buf[FS_FILE_HEADER_LENGTH/4] = {0};
    struct fs_header*   file_hdr = (struct fs_header*)((uint8_t*)buf);

    ret = _fs_validate(dir);
    if (ret != FR_OK) {
        return ret;
    }
    // Shall not input invalid parameters
    if (info == NULL || file_num == NULL) {
        return FR_INVALID_PARAMETER;
    }

    while(num < *file_num) {
        _fs_disk_read(dir->sub_file_addr,
                      (uint8_t*)file_hdr,
                      FS_FILE_HEADER_LENGTH);

        if (file_hdr->file_name_len <= FS_MAX_FILE_NAME_LENGTH &&
            FS_GET_DIR_LEVEL(file_hdr->file_prop) == (dir->dir_level + 1)) {
            info[num].name_len = file_hdr->file_name_len;
            memcpy(info[num].name, file_hdr->file_name, info[num].name_len);
            num++;
            if (file_hdr->next_addr == FS_INVALID_ADDRESS) {
                break;
            } else if(FS_GET_FILE_TYPE(file_hdr->file_prop) == FS_DIR) {
                dir->sub_file_addr = file_hdr->next_addr;
            } else {
                dir->sub_file_addr += FS_FILE_HEADER_LENGTH;
            }
        } else {
            break;
        }
    }
    *file_num = num;
    return ret;
}

/** @} */
