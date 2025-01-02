/**
 * @file lv_gif.h
 *
 */

#ifndef LV_OM_GIF_H
#define LV_OM_GIF_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lvgl.h"
#if LV_USE_GIF

#include "om_animation.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_img_t img;
    om_gif_t *gif;
    lv_timer_t * timer;
    lv_img_dsc_t imgdsc;
    uint32_t last_call;
}lv_om_gif_t;

extern const lv_obj_class_t lv_om_gif_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t * lv_om_gif_create(lv_obj_t * parent);
void lv_om_gif_set_src(lv_obj_t * obj, const void * src, uint32_t data_len);
void lv_om_gif_restart(lv_obj_t * gif);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_GIF*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_GIF_H*/
