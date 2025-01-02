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
 ******************************************************************************
 *
 * $Project:     OnMicro OM662x
 * $Author:      OnMicro SW Team
 * $Date:        13. Nov 2020
 *
 * $Revision:    V1.0
 * $Brief:       lvgl port for OnMicro OM662x platform
 *
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    - Initial release
 */


/* Include ------------------------------------------------------------------ */
#include <stdint.h>
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_fs.h"
#include "lv_port_indev.h"
#include "lv_conf.h"
#include "co_debug.h"


#if (LV_USE_LOG)
static void lv_log_print_cb(lv_log_level_t level, const char *file, uint32_t line, const char *func, const char *desc) {
    size_t p;
    static const char * lvl_prefix[] = {"Trace", "Info", "Warn", "Error", "User"};
    
    //log_debug("%s: %s \t(%s #%d %s())\n", lvl_prefix[level], desc, &file[p], line, func);
}
#endif  /* LV_USE_LOG */

#if ONMICRO_POS
    void lv_onmicro_decoder_init(void);
#endif

/* Export Function ---------------------------------------------------------- */
void lv_port_init(void) {
    lv_init();
#if (LV_USE_LOG)
    lv_log_register_print_cb(lv_log_print_cb);
#endif
    lv_port_disp_init();
    lv_port_indev_init();
    lv_port_fs_init();
#if ONMICRO_POS
    lv_onmicro_decoder_init();
#endif
}


/* End Of File -------------------------------------------------------------- */

