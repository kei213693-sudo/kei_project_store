#include <stdint.h>
#include "om_animation.h"
#include "lvgl.h"

/*
    port function
*/

#define OM_LV_EN    0

#if OM_LV_EN
#else
static uint8_t big_buff[280*240*2 + 512 + 1000];
static uint8_t* om_malloc_p = big_buff;
#endif


static void* om_gif_malloc(uint32_t size)
{
    #if OM_LV_EN
    return lv_mem_alloc(size);
    #else
    uint8_t* rp = om_malloc_p;
    om_malloc_p += size;
    memset(rp, 0, size);
    return rp;
    #endif
}

static void om_gif_free(void* ptr)
{
    #if OM_LV_EN
    lv_mem_free(ptr);
    #else
    
    #endif
}

// __attribute__((always_inline)) __inline
static uint32_t om_gif_read(om_gif_t* gif, uint8_t* buf, uint32_t len)
{
    uint8_t* src = gif->data + gif->ptr;
    for(uint32_t i=0; i<len;i++)
        buf[i] = src[i];
    gif->ptr += len;
    return len;
}


uint32_t* i_debug;

 om_gif_t* om_gif_debug;

om_gif_t* om_gif_open(void* data, uint32_t data_len)
{
    om_gif_t* om_gif = om_gif_malloc(sizeof(om_gif_t));
    om_gif_debug = om_gif;
    om_gif->data = data;
    om_gif->data_len = data_len;
    om_gif_read(om_gif, (void*)om_gif, 7);
    
    uint16_t w = om_gif->header.width;
    uint16_t h = om_gif->header.height;
    uint8_t gct_size = om_gif->header.gct_size;
    
    om_gif->frame =  om_gif_malloc(w*h*2);
    om_gif->gct = om_gif_malloc(gct_size*2 + 2);
    om_gif_read(om_gif, (void*)om_gif->gct, gct_size*2 + 2);
    return om_gif;
}


static void om_gif_get_frame_raw(om_gif_t* om_gif)
{
}
uint32_t offset;
static void om_gif_get_frame_index(om_gif_t* om_gif)
{
    
    uint16_t img_w = om_gif->header.width;
    uint16_t img_h = om_gif->header.height;
    
    uint16_t w = om_gif->frame_info.frame_w;
    uint16_t h = om_gif->frame_info.frame_h;
    uint16_t l = om_gif->frame_info.frame_left;
    uint16_t t = om_gif->frame_info.frame_top;
    uint8_t flag = om_gif->frame_info.sign_flag;
    uint8_t sign = om_gif->frame_info.sign;
    uint8_t sign_bytes = om_gif->frame_info.sign_bytes_number;
    uint16_t* color_table  = (uint16_t*)om_gif->gct;
    
    uint8_t* p = om_gif->ptr + om_gif->data;
    uint8_t* dst = om_gif->frame;
    
    uint8_t dispose_method = om_gif->frame_info.dispose_method;
    
    if(dispose_method == 2) {
        for(uint32_t i=0; i<img_w*img_h; i++) {
            ((uint16_t*)dst)[i] = color_table[om_gif->header.bg_color_index - 1];
        }
    }
    
    
    uint32_t i = 0;
    i_debug = &i;
    offset = 0;
    offset = t * img_w + l;
    
    while(i < w * h) {  //  w*h numbers pixel need to be changed
        uint8_t content = *p;
        p++;
        if(content == sign) {
            uint8_t write = *p;
            p++;
            uint8_t size = *p;
            p++;
            for(uint32_t j=0; j<size; j++) {
                ((uint16_t*) dst)[i%w + offset] = color_table[write];
                i++;
                if(i%w==0) {
                    offset += img_w;
                }
            }
        } else {
            ((uint16_t*) dst)[i%w + offset] = color_table[content];
            i++;    // write 1 pixels
            if(i%w==0) {
                offset += img_w;
            }
        }
    }
    

    
    
    
    om_gif->ptr = p - om_gif->data;
}

static void om_gif_get_frame_index_dec(om_gif_t* om_gif)
{
}

static void om_gif_get_frame_index_mix(om_gif_t* om_gif)
{
}




uint8_t om_gif_get_frame(om_gif_t* om_gif, uint16_t* delay)
{
    if(om_gif->ptr == om_gif->data_len) {
        return 1;
    }

    om_gif_read(om_gif, (void*)&(om_gif->frame_info), 14);
    *delay = om_gif->frame_info.frame_delay;

    om_gif_get_frame_index(om_gif);
    
    
    return 0;
}

void om_gif_rewind(om_gif_t* om_gif)
{
    om_gif->ptr = 7 + (om_gif->header.gct_size + 1)*2;
}

void om_gif_close(om_gif_t* om_gif)
{
    om_gif_free(om_gif->frame);
    om_gif_free(om_gif->gct);
    om_gif_free(om_gif);
}