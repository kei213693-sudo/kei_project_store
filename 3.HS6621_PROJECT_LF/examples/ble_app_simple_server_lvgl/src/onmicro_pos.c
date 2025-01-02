#include "lvgl.h"
#ifdef ONMICRO_POS
#include "onmicro_pos.h"
#include "co_debug.h"

typedef struct {

    lv_area_t* coord;
    lv_area_t* clip;
    lv_img_decoder_dsc_t* dsc;
} onmicro_pos_read_t;

static onmicro_pos_read_t onmicro_pos_read;


lv_res_t onmicro_pos_read_line(lv_img_decoder_dsc_t * dsc, lv_area_t* coord, lv_area_t* clip)
{
    onmicro_pos_read.coord = coord;
    onmicro_pos_read.clip = clip;
    onmicro_pos_read.dsc = dsc;
    return LV_RES_OK;
}

void onmicro_pos_copy(lv_color_t * buf, lv_coord_t buf_w, const lv_color_t * map, lv_coord_t map_w, lv_coord_t copy_w, lv_coord_t copy_h)
{
    // if(onmicro_pos_read.multi_read & onmicro_pos_read.back_read)
        //if(width == onmicro_pos_read.dsc.header.w)
    int32_t width = lv_area_get_width(onmicro_pos_read.clip);
    lv_coord_t x = onmicro_pos_read.clip->x1;
    lv_coord_t y = onmicro_pos_read.clip->y1 - onmicro_pos_read.coord->y1;
    uint32_t read_lines = onmicro_pos_read.clip->y2-onmicro_pos_read.clip->y1+1;
    
    
    if(copy_w == buf_w)
        lv_img_decoder_read_line(onmicro_pos_read.dsc, x, y, read_lines*width, (void*)buf);
    else{
        for(uint32_t i=0;i<copy_h;i++) {
            lv_img_decoder_read_line(onmicro_pos_read.dsc, x, y++, copy_w, (void*)buf);
            // log_debug("buf:%p\r\n",buf);
            buf += buf_w;
            
        }
    }
    
    
    // log_debug("parm:%d, %d, %d, %d\r\n",buf_w,map_w,copy_w,copy_h);
    // log_debug("coor:%d, %d, %d, %d\r\n",onmicro_pos_read.coord->x1,onmicro_pos_read.coord->x2,onmicro_pos_read.coord->y1,onmicro_pos_read.coord->y2);
    // log_debug("clip:%d, %d, %d, %d\r\n",onmicro_pos_read.clip->x1,onmicro_pos_read.clip->x2,onmicro_pos_read.clip->y1,onmicro_pos_read.clip->y2);
    // log_debug("read:%d, %d, %d, %d\r\n",x,y,read_lines,width);

}






























#endif
