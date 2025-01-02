#include <stdio.h>
#include "om_animation.h"
#include "lv_om_animation.h"
#include "glcd.h"



//extern unsigned char om_gif_1[937539];



//static om_gif_t* om_gif;

//void om_gifdec_demo()
//{
//    om_gif = om_gif_open(om_gif_1, sizeof(om_gif_1));
//    
//    uint16_t delay;
//    uint8_t result;
//    while(1) {
//        result = om_gif_get_frame(om_gif, &delay);
//        if(result) {
//            om_gif_rewind(om_gif);
//            continue;
//        }
//        uint16_t offset = 100;
//        uint16_t w = om_gif->frame_info.frame_w;
//        uint16_t h = om_gif->frame_info.frame_h;
//        uint16_t x = om_gif->frame_info.frame_left;
//        uint16_t y = om_gif->frame_info.frame_top;
//        
//        uint16_t x1 = x + offset;
//        uint16_t y1 = y + offset;
//        uint16_t x2 = x1 + w - 1;
//        uint16_t y2 = y1 + h - 1;
//        
//        uint8_t* dst = om_gif->frame;
//        glcd_set_disp_window(x1, y1, x2, y2);
//        glcd_disp_flush(dst, w * h *2);
//        glcd_disp_flush_finish();
//    }
//}


 






/**
 * @file lv_gifenc.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#if LV_USE_GIF



/*********************
 *      DEFINES
 *********************/
#define MY_CLASS    &lv_om_gif_class

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_om_gif_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_om_gif_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void next_frame_task_cb(lv_timer_t * t);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_om_gif_class = {
    .constructor_cb = lv_om_gif_constructor,
    .destructor_cb = lv_om_gif_destructor,
    .instance_size = sizeof(lv_om_gif_t),
    .base_class = &lv_img_class
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_om_gif_create(lv_obj_t * parent)
{

    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

void lv_om_gif_set_src(lv_obj_t * obj, const void * src, uint32_t data_len)
{
    lv_om_gif_t * gifobj = (lv_om_gif_t *) obj;

    /*Close previous gif if any*/
    if(gifobj->gif) {
        lv_img_cache_invalidate_src(&gifobj->imgdsc);
        om_gif_close(gifobj->gif);
        gifobj->gif = NULL;
        gifobj->imgdsc.data = NULL;
    }

//    if(lv_img_src_get_type(src) == LV_IMG_SRC_VARIABLE) {
//        const lv_img_dsc_t * img_dsc = src;
//        gifobj->gif = gd_open_gif_data(img_dsc->data);
//    }
//    else if(lv_img_src_get_type(src) == LV_IMG_SRC_FILE) {
//       gifobj->gif = gd_open_gif_file(src);
//    }
//    if(gifobj->gif == NULL) {
//        LV_LOG_WARN("Could't load the source");
//        return;
//    }

   gifobj->gif = om_gif_open((void*)src, data_len);
    
   gifobj->imgdsc.data = gifobj->gif->frame;
   gifobj->imgdsc.header.always_zero = 0;

   gifobj->imgdsc.header.cf = LV_IMG_CF_TRUE_COLOR;

    
   gifobj->imgdsc.header.h = gifobj->gif->header.height;
   gifobj->imgdsc.header.w = gifobj->gif->header.width;
   gifobj->last_call = lv_tick_get();

   lv_img_set_src(obj, &gifobj->imgdsc);

   lv_timer_resume(gifobj->timer);
   lv_timer_reset(gifobj->timer);

   next_frame_task_cb(gifobj->timer);

}

void lv_om_gif_restart(lv_obj_t * obj)
{
    lv_om_gif_t * gifobj = (lv_om_gif_t *) obj;
    om_gif_rewind(gifobj->gif);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_om_gif_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    lv_om_gif_t * gifobj = (lv_om_gif_t *) obj;

    gifobj->timer = lv_timer_create(next_frame_task_cb, 10, obj);
    lv_timer_pause(gifobj->timer);
}

static void lv_om_gif_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_om_gif_t * gifobj = (lv_om_gif_t *) obj;
    lv_img_cache_invalidate_src(&gifobj->imgdsc);
    om_gif_close(gifobj->gif);
    lv_timer_del(gifobj->timer);
}

static void next_frame_task_cb(lv_timer_t * t)
{
    lv_obj_t * obj = t->user_data;
    lv_om_gif_t * gifobj = (lv_om_gif_t *) obj;
    uint32_t elaps = lv_tick_elaps(gifobj->last_call);
    if(elaps < gifobj->gif->frame_info.frame_delay * 10) return;

    gifobj->last_call = lv_tick_get();

    uint16_t delay;
    int has_next = om_gif_get_frame(gifobj->gif, &delay);
    if(has_next == 1) {
        om_gif_rewind(gifobj->gif);
        return;
    }

    lv_img_cache_invalidate_src(lv_img_get_src(obj));
    lv_obj_invalidate(obj);
}

#endif /*LV_USE_GIF*/






