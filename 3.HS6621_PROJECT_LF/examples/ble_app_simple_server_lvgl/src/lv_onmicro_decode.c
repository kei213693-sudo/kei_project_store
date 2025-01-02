#include "lvgl.h"
#if ONMICRO_POS
#include "co_debug.h"
#include "fs.h"


FRESULT omfs_read_no_copy(fs_file_t* file, void* buf, uint32_t* length);

typedef struct {
    fs_file_t f;
} lv_img_decoder_onmicro_data_t;


static lv_res_t decoder_info(lv_img_decoder_t * decoder, const void * src, lv_img_header_t * header);
static lv_res_t decoder_open(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);
static lv_res_t decoder_read_line(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc,lv_coord_t x, lv_coord_t y, lv_coord_t len, uint8_t * buf);
static void decoder_close(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);


void lv_onmicro_decoder_init(void)
{
    lv_img_decoder_t * dec = lv_img_decoder_create();
    lv_img_decoder_set_info_cb(dec, decoder_info);
    lv_img_decoder_set_open_cb(dec, decoder_open);
    lv_img_decoder_set_read_line_cb(dec, decoder_read_line);
    lv_img_decoder_set_close_cb(dec, decoder_close);
}

static lv_res_t decoder_info(lv_img_decoder_t * decoder, const void * src, lv_img_header_t * header)
{
    LV_UNUSED(decoder); /*Unused*/
    lv_img_src_t src_type = lv_img_src_get_type(src);

    if(src_type == LV_IMG_SRC_FILE) {
        const char * fn = src;
        if(!strcmp(&fn[strlen(fn) - 3], "bin")) {              /*Check the extension*/
            /*Save the data in the header*/
            fs_file_t f;
            lv_fs_res_t res = omfs_open(&f, src+2);
            if(res != LV_FS_RES_OK) return LV_RES_INV;
                        
            uint32_t rn = sizeof(lv_img_header_t);
            res = omfs_read(&f, header,&rn);

#if LV_COLOR_DEPTH == 32
            uint16_t bpp;
            memcpy(&bpp, headers + 28, 2);
            header->cf = bpp == 32 ? LV_IMG_CF_TRUE_COLOR_ALPHA : LV_IMG_CF_TRUE_COLOR;
#else
            header->cf = LV_IMG_CF_TRUE_COLOR;
#endif
            return LV_RES_OK;
        }
    }
    return LV_RES_INV;         /*If didn't succeeded earlier then it's an error*/

}

static lv_res_t decoder_open(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc)
{
    LV_UNUSED(dec);
    if(dsc->src_type == LV_IMG_SRC_FILE) {
        const char * fn = dsc->src;

        if(strcmp(&fn[strlen(fn) - 3], "bin")) return LV_RES_INV;       /*Check the extension*/

        lv_img_decoder_onmicro_data_t b;
        memset(&b, 0x00, sizeof(b));          
        lv_fs_res_t res = omfs_open(&b.f, (dsc->src)+2);
        if(res == LV_RES_OK) return LV_RES_INV;
        dsc->user_data = lv_mem_alloc(sizeof(lv_img_decoder_onmicro_data_t));
        LV_ASSERT_MALLOC(dsc->user_data);
        if(dsc->user_data == NULL) return LV_RES_INV;
        memcpy(dsc->user_data, &b, sizeof(b));

        dsc->img_data = NULL;
        return LV_RES_OK;
    }
    return LV_RES_INV;
};

static lv_res_t decoder_read_line(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc,lv_coord_t x, lv_coord_t y, lv_coord_t len, uint8_t * buf)
{
    // at point x,y read len numbers pixel put them in buf
    LV_UNUSED(decoder);
    lv_img_decoder_onmicro_data_t * b = dsc->user_data;
    // log_debug("dec_dsc,x:%p ",&b->f);
    // log_debug("x:%d,y:%d, len:%d, buf:%p\r\n",x,y,len,buf);
    uint32_t px_size = 16;
    uint32_t pos = ((y * dsc->header.w + x) * px_size) >> 3;
    pos += 4;
    lv_fs_res_t res = omfs_seek(&b->f, pos, FS_SEEK_SET);

    uint32_t btr = len * (px_size >> 3);
    // btr = btr*ONMICRO_DRAW_LINES;
    gpio_toggle(1<<16);
    res = omfs_read_no_copy(&b->f, buf, &btr);
    gpio_toggle(1<<16);
    if(res != LV_FS_RES_OK || btr != (len * (px_size >> 3))) {
        LV_LOG_WARN("onmicro image decoder read failed");
        log_debug("res,%d, btr:%d,len:%d\r\n",res,btr,len);
        return LV_RES_INV;
    }
    return LV_RES_OK;
};

static void decoder_close(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc)
{
    LV_UNUSED(dec);
    lv_img_decoder_onmicro_data_t * b = dsc->user_data;
    // lv_mem_free(&b->f);
    lv_mem_free(dsc->user_data);
};

#endif
