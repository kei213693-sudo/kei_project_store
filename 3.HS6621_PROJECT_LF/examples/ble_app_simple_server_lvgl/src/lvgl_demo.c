#include "lvgl.h"


static void fs_img_test1()
{
    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_t * img1 = lv_img_create(lv_scr_act());
    lv_obj_t * img2 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, "S:/1.bin");
    lv_img_set_src(img2, "S:/2.bin");
}

static void fs_img_test2()
{
    lv_obj_t * img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, "S:/3.bin");
}

void lv_demo_gif()
{
    LV_IMG_DECLARE(om_gif_213_256);
    lv_obj_t * img;
    img = lv_gif_create(lv_scr_act());
    
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_gif_set_src(img, &om_gif_213_256);
}


void lv_demo_button(void)
{
    lv_obj_t * label1;
    lv_obj_t * label2;
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Button");
    lv_obj_center(label1);

    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Toggle");
    lv_obj_center(label2);
}

void lv_demo_start(void)
{
    lv_demo_gif();
}