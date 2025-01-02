#ifndef __OM_ANIMATION_H
#define __OM_ANIMATION_H
#include <stdint.h>
#include "lvgl.h"

typedef enum {
    OM_GIF_TYPE_RAW,
    OM_GIF_TYPE_INDEX,              // only index frame saved
    OM_GIF_TYPE_INDEX_DEC,          // index is be compressed
    OM_GIF_TYPE_INDEX_MIX,          // some frame is index and some frame is compressed.
    OM_GIF_TYPE_UNKNOW,
} om_gif_type_t;

typedef struct {
    uint16_t width;
    uint16_t height;
    om_gif_type_t type;
    uint8_t gct_size;             // raw frame need no gct
    uint8_t bg_color_index;
} om_gif_header_t;

typedef struct {
    uint16_t frame_w;
    uint16_t frame_h;
    uint16_t frame_left;
    uint16_t frame_top;
    uint16_t frame_delay;
    uint8_t dispose_method;
    uint8_t sign_flag;
    uint8_t sign;
    uint8_t sign_bytes_number;
} om_gif_frame_info_t;

typedef struct {
    om_gif_header_t header;
    om_gif_frame_info_t frame_info;
    uint32_t ptr;
    uint32_t data_len;
    uint8_t* gct;
    uint8_t* lct;
    uint8_t* pattler;
    uint8_t* data;
    uint8_t* frame;
} om_gif_t;


extern om_gif_t* om_gif_open(void* data, uint32_t data_len);
extern uint8_t om_gif_get_frame(om_gif_t* om_gif, uint16_t* delay);
extern void om_gif_rewind(om_gif_t* om_gif);
extern void om_gif_close(om_gif_t* om_gif);

#endif /* __OM_ANIMATION_H */
