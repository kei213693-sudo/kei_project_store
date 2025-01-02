
#ifndef __GLCD_H
#define __GLCD_H

#include <stdint.h>

typedef void(*glcd_cb_t)(void);

extern void glcd_init(void);
extern void glcd_set_disp_window(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
extern void glcd_disp_flush(const void *color, uint32_t color_len);
extern void glcd_disp_flush_finish(void);
extern void glcd_register_cb(glcd_cb_t cb);


#if (GLCD_JD5858)
#include "glcd_jd5858.h"
#endif

#endif  /* __GLCD_H */


/* End Of File -------------------------------------------------------------- */

