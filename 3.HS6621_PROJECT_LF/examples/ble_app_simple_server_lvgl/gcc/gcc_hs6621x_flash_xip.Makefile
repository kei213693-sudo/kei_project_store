############################################################################################
#
# OnMicro makefile
#
############################################################################################

# Project
PROJECT = gcc_hs6621c_flash_xip

# SDK ROOT dir
SDK_BASE = ../../../

# Example source files direcotry
EXAMPLE_BASE = examples/ble_app_simple_server_lvgl

# Freertos source files direcotry
FREERTOS_BASE = third_party/FreeRTOS

# Profile source files direcotry
PROFILE_BASE = stack/ip/ble/profiles

# LVGL source files direcotry
LVGL_BASE = third_party/LittleVGL

# Defines
DEFINES = -DCONFIG_AUTOCONFG_VERSION -DCONFIG_LOG_OUTPUT

# Optimization: 0, 1, 2, 3, s, 4(ultrasize)
CONFIG_OPTIMIZATION_LEVEL=4

# Build obj files direcotry
BUILDDIR	= .build_gcc_hs6621c_flash_xip

# ASM Sources
ASM_SOURCES = \

# Base Sources
BASE_SOURCES = \
    lib/gcc/sys_vectors.c \

# Main Sources
MAIN_SOURCES = \
    $(EXAMPLE_BASE)/src/main.c \
    $(EXAMPLE_BASE)/src/ble_main.c \
    $(EXAMPLE_BASE)/src/ctp_hynitron_ext.c \
    $(EXAMPLE_BASE)/src/ctp_hynitron_iic.c \
    $(EXAMPLE_BASE)/src/glcd_jd5858.c \
    $(EXAMPLE_BASE)/src/lvgl_demo.c \
    hardware/peripheral/retarget.c \

# App Sources
APP_SOURCES = \
    $(EXAMPLE_BASE)/src/app.c \
    $(EXAMPLE_BASE)/src/app_task.c \
    $(EXAMPLE_BASE)/src/app_simple_server.c \
    examples/common/app_adv.c \
    examples/common/app_sec.c \

# Profile Sources
PROFILE_SOURCES = \
    $(PROFILE_BASE)/simple_server/src/simple_server.c \
    $(PROFILE_BASE)/simple_server/src/simple_server_task.c \

# Stack Sources
STACK_SOURCES = \
    stack/plf/hs6621/rwip/rwmem.c \

# Freertos Sources
FREERTOS_SOURCES = \
    $(FREERTOS_BASE)/event_groups.c \
    $(FREERTOS_BASE)/list.c \
    $(FREERTOS_BASE)/queue.c \
    $(FREERTOS_BASE)/tasks.c \
    $(FREERTOS_BASE)/timers.c \
    $(FREERTOS_BASE)/portable/MemMang/heap_4.c \
    $(FREERTOS_BASE)/portable/GCC/ARM_CM4F/port.c \

# Lvgl Sources
LVGL_SOURCES = \
    $(LVGL_BASE)/port/lv_port_disp.c \
    $(LVGL_BASE)/port/lv_port_indev.c \
    $(LVGL_BASE)/port/lv_port_fs.c \
    $(LVGL_BASE)/port/lv_port_hs662x.c \
    $(LVGL_BASE)/src/lv_widgets/lv_line.c \
    $(LVGL_BASE)/src/lv_widgets/lv_keyboard.c \
    $(LVGL_BASE)/src/lv_widgets/lv_cont.c \
    $(LVGL_BASE)/src/lv_widgets/lv_img.c \
    $(LVGL_BASE)/src/lv_widgets/lv_objmask.c \
    $(LVGL_BASE)/src/lv_widgets/lv_linemeter.c \
    $(LVGL_BASE)/src/lv_widgets/lv_spinner.c \
    $(LVGL_BASE)/src/lv_widgets/lv_label.c \
    $(LVGL_BASE)/src/lv_widgets/lv_objx_templ.c \
    $(LVGL_BASE)/src/lv_widgets/lv_led.c \
    $(LVGL_BASE)/src/lv_widgets/lv_btn.c \
    $(LVGL_BASE)/src/lv_widgets/lv_tileview.c \
    $(LVGL_BASE)/src/lv_widgets/lv_gauge.c \
    $(LVGL_BASE)/src/lv_widgets/lv_msgbox.c \
    $(LVGL_BASE)/src/lv_widgets/lv_calendar.c \
    $(LVGL_BASE)/src/lv_widgets/lv_win.c \
    $(LVGL_BASE)/src/lv_widgets/lv_spinbox.c \
    $(LVGL_BASE)/src/lv_widgets/lv_tabview.c \
    $(LVGL_BASE)/src/lv_widgets/lv_list.c \
    $(LVGL_BASE)/src/lv_widgets/lv_arc.c \
    $(LVGL_BASE)/src/lv_widgets/lv_btnmatrix.c \
    $(LVGL_BASE)/src/lv_widgets/lv_table.c \
    $(LVGL_BASE)/src/lv_widgets/lv_imgbtn.c \
    $(LVGL_BASE)/src/lv_widgets/lv_canvas.c \
    $(LVGL_BASE)/src/lv_widgets/lv_bar.c \
    $(LVGL_BASE)/src/lv_widgets/lv_page.c \
    $(LVGL_BASE)/src/lv_widgets/lv_dropdown.c \
    $(LVGL_BASE)/src/lv_widgets/lv_switch.c \
    $(LVGL_BASE)/src/lv_widgets/lv_roller.c \
    $(LVGL_BASE)/src/lv_widgets/lv_checkbox.c \
    $(LVGL_BASE)/src/lv_widgets/lv_cpicker.c \
    $(LVGL_BASE)/src/lv_widgets/lv_slider.c \
    $(LVGL_BASE)/src/lv_widgets/lv_chart.c \
    $(LVGL_BASE)/src/lv_widgets/lv_textarea.c \
    $(LVGL_BASE)/src/lv_core/lv_style.c \
    $(LVGL_BASE)/src/lv_core/lv_group.c \
    $(LVGL_BASE)/src/lv_core/lv_refr.c \
    $(LVGL_BASE)/src/lv_core/lv_indev.c \
    $(LVGL_BASE)/src/lv_core/lv_obj.c \
    $(LVGL_BASE)/src/lv_core/lv_disp.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_32.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_10.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_38.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_16.c \
    $(LVGL_BASE)/src/lv_font/lv_font_dejavu_16_persian_hebrew.c \
    $(LVGL_BASE)/src/lv_font/lv_font_simsun_16_cjk.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_30.c \
    $(LVGL_BASE)/src/lv_font/lv_font_fmt_txt.c \
    $(LVGL_BASE)/src/lv_font/lv_font.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_42.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_36.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_20.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_46.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_18.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_40.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_28_compressed.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_12.c \
    $(LVGL_BASE)/src/lv_font/lv_font_unscii_16.c \
    $(LVGL_BASE)/src/lv_font/lv_font_unscii_8.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_34.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_22.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_12_subpx.c \
    $(LVGL_BASE)/src/lv_font/lv_font_loader.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_28.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_44.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_26.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_14.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_24.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_48.c \
    $(LVGL_BASE)/src/lv_font/lv_font_montserrat_8.c \
    $(LVGL_BASE)/src/lv_themes/lv_theme_empty.c \
    $(LVGL_BASE)/src/lv_themes/lv_theme_template.c \
    $(LVGL_BASE)/src/lv_themes/lv_theme_mono.c \
    $(LVGL_BASE)/src/lv_themes/lv_theme_material.c \
    $(LVGL_BASE)/src/lv_themes/lv_theme.c \
    $(LVGL_BASE)/src/lv_misc/lv_math.c \
    $(LVGL_BASE)/src/lv_misc/lv_async.c \
    $(LVGL_BASE)/src/lv_misc/lv_ll.c \
    $(LVGL_BASE)/src/lv_misc/lv_color.c \
    $(LVGL_BASE)/src/lv_misc/lv_mem.c \
    $(LVGL_BASE)/src/lv_misc/lv_fs.c \
    $(LVGL_BASE)/src/lv_misc/lv_area.c \
    $(LVGL_BASE)/src/lv_misc/lv_log.c \
    $(LVGL_BASE)/src/lv_misc/lv_txt_ap.c \
    $(LVGL_BASE)/src/lv_misc/lv_bidi.c \
    $(LVGL_BASE)/src/lv_misc/lv_txt.c \
    $(LVGL_BASE)/src/lv_misc/lv_anim.c \
    $(LVGL_BASE)/src/lv_misc/lv_utils.c \
    $(LVGL_BASE)/src/lv_misc/lv_task.c \
    $(LVGL_BASE)/src/lv_misc/lv_templ.c \
    $(LVGL_BASE)/src/lv_misc/lv_gc.c \
    $(LVGL_BASE)/src/lv_misc/lv_debug.c \
    $(LVGL_BASE)/src/lv_misc/lv_printf.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_mask.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_triangle.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_label.c \
    $(LVGL_BASE)/src/lv_draw/lv_img_cache.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_arc.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_rect.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_img.c \
    $(LVGL_BASE)/src/lv_draw/lv_img_decoder.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_blend.c \
    $(LVGL_BASE)/src/lv_draw/lv_img_buf.c \
    $(LVGL_BASE)/src/lv_draw/lv_draw_line.c \
    $(LVGL_BASE)/src/lv_hal/lv_hal_tick.c \
    $(LVGL_BASE)/src/lv_hal/lv_hal_indev.c \
    $(LVGL_BASE)/src/lv_hal/lv_hal_disp.c \
    $(LVGL_BASE)/src/lv_gpu/lv_gpu_nxp_pxp_osa.c \
    $(LVGL_BASE)/src/lv_gpu/lv_gpu_nxp_pxp.c \
    $(LVGL_BASE)/src/lv_gpu/lv_gpu_nxp_vglite.c \
    $(LVGL_BASE)/src/lv_gpu/lv_gpu_stm32_dma2d.c \

# Sources (Relative to the SDK root directory: $SDK_BASE)
SOURCES += $(BASE_SOURCES) $(MAIN_SOURCES) $(APP_SOURCES) $(PROFILE_SOURCES) $(STACK_SOURCES) $(FREERTOS_SOURCES) $(LVGL_SOURCES)

# Includes (Relative to the Makefile directory)
INCLUDES = \
    -I../../../lib \
    -I../../../lib/gcc \
    -I../../../coroutine \
    -I../../../include \
    -I../../../main \
    -I../../../hardware/sys \
    -I../../../hardware/cmsis \
    -I../../../hardware/cfg \
    -I../../../hardware/peripheral \
    -I../../../hardware/rw_port \
    -I../../../hardware/patch \
    -I../../../hardware/loader \
    -I../../../hardware/mbr \
    -I../../../stack/ip/ahi/api \
    -I../../../stack/ip/hci/api \
    -I../../../stack/ip/sch/api \
    -I../../../stack/ip/em/api \
    -I../../../stack/modules/aes/api \
    -I../../../stack/modules/common/api \
    -I../../../stack/modules/dbg/api \
    -I../../../stack/modules/display/api \
    -I../../../stack/modules/ecc_p256/api \
    -I../../../stack/modules/h4tl/api \
    -I../../../stack/modules/ke/api \
    -I../../../stack/modules/nvds/api \
    -I../../../stack/modules/rf/api \
    -I../../../stack/modules/rwip/api \
    -I../../../stack/modules/rwip/src \
    -I../../../stack/plf/hs6621/arch \
    -I../../../stack/plf/hs6621/reg \
    -I../../../stack/ip/ble/hl/api \
    -I../../../stack/ip/ble/hl/inc \
    -I../../../stack/ip/ble/hl/src/rwble_hl \
    -I../../../stack/ip/ble/ll/api \
    -I../../../stack/ip/ble/ll/src \
    -I../../../stack/ip/ble/mesh/api \
    -I../../../stack/ip/ble/mesh/inc \
    -I../../../stack/ip/ble/mesh/inc_al \
    -I../../../stack/ip/ble/mesh/src/common/inc \
    -I../../../stack/ip/ble/mesh/src/inc \
    -I../../../stack/ip/ble/mesh/src/models \
    -I../../../stack/ip/ble/mesh/src/models/inc \
    -I../../../stack/ip/ble/mesh/src/models/src \
    -I../../../stack/ip/ble/mesh/src/prf \
    -I../../../stack/ip/ble/mesh/src/prf/inc \
    -I../../../$(PROFILE_BASE)/anp \
    -I../../../$(PROFILE_BASE)/bas \
    -I../../../$(PROFILE_BASE)/bcs \
    -I../../../$(PROFILE_BASE)/blp \
    -I../../../$(PROFILE_BASE)/cpp \
    -I../../../$(PROFILE_BASE)/cscp \
    -I../../../$(PROFILE_BASE)/dis \
    -I../../../$(PROFILE_BASE)/find \
    -I../../../$(PROFILE_BASE)/glp \
    -I../../../$(PROFILE_BASE)/hogp \
    -I../../../$(PROFILE_BASE)/hrp \
    -I../../../$(PROFILE_BASE)/htp \
    -I../../../$(PROFILE_BASE)/lan \
    -I../../../$(PROFILE_BASE)/pasp \
    -I../../../$(PROFILE_BASE)/prox \
    -I../../../$(PROFILE_BASE)/rscp \
    -I../../../$(PROFILE_BASE)/scpp \
    -I../../../$(PROFILE_BASE)/tip \
    -I../../../$(PROFILE_BASE)/uds \
    -I../../../$(PROFILE_BASE)/wscp \
    -I../../../$(PROFILE_BASE)/anp/anpc/api \
    -I../../../$(PROFILE_BASE)/anp/anps/api \
    -I../../../$(PROFILE_BASE)/bas/basc/api \
    -I../../../$(PROFILE_BASE)/bas/bass/api \
    -I../../../$(PROFILE_BASE)/bcs/bcsc/api \
    -I../../../$(PROFILE_BASE)/bcs/bcss/api \
    -I../../../$(PROFILE_BASE)/blp/blpc/api \
    -I../../../$(PROFILE_BASE)/blp/blps/api \
    -I../../../$(PROFILE_BASE)/cpp/cppc/api \
    -I../../../$(PROFILE_BASE)/cpp/cpps/api \
    -I../../../$(PROFILE_BASE)/cscp/cscpc/api \
    -I../../../$(PROFILE_BASE)/cscp/cscps/api \
    -I../../../$(PROFILE_BASE)/dis/disc/api \
    -I../../../$(PROFILE_BASE)/dis/diss/api \
    -I../../../$(PROFILE_BASE)/find/findl/api \
    -I../../../$(PROFILE_BASE)/find/findt/api \
    -I../../../$(PROFILE_BASE)/glp/glpc/api \
    -I../../../$(PROFILE_BASE)/glp/glps/api \
    -I../../../$(PROFILE_BASE)/hogp/hogpbh/api \
    -I../../../$(PROFILE_BASE)/hogp/hogpd/api \
    -I../../../$(PROFILE_BASE)/hogp/hogprh/api \
    -I../../../$(PROFILE_BASE)/hrp/hrpc/api \
    -I../../../$(PROFILE_BASE)/hrp/hrps/api \
    -I../../../$(PROFILE_BASE)/htp/htpc/api \
    -I../../../$(PROFILE_BASE)/htp/htpt/api \
    -I../../../$(PROFILE_BASE)/lan/lanc/api \
    -I../../../$(PROFILE_BASE)/lan/lans/api \
    -I../../../$(PROFILE_BASE)/pasp/paspc/api \
    -I../../../$(PROFILE_BASE)/pasp/pasps/api \
    -I../../../$(PROFILE_BASE)/prox/proxm/api \
    -I../../../$(PROFILE_BASE)/prox/proxr/api \
    -I../../../$(PROFILE_BASE)/rscp/rscpc/api \
    -I../../../$(PROFILE_BASE)/rscp/rscps/api \
    -I../../../$(PROFILE_BASE)/scpp/scppc/api \
    -I../../../$(PROFILE_BASE)/scpp/scpps/api \
    -I../../../$(PROFILE_BASE)/tip/tipc/api \
    -I../../../$(PROFILE_BASE)/tip/tips/api \
    -I../../../$(PROFILE_BASE)/uds/udsc/api \
    -I../../../$(PROFILE_BASE)/uds/udss/api \
    -I../../../$(PROFILE_BASE)/wscp/wscc/api \
    -I../../../$(PROFILE_BASE)/wscp/wscs/api \
    -I../../../$(PROFILE_BASE)/tspp/tspps/api \
    -I../../../$(PROFILE_BASE)/tspp/tsppc/api \
    -I../../../examples/common \
    -I../../../stack/plf/hs6621/bb \
    -I../../../stack/plf/hs6621/sc \
    -I../src \
    -I../../../$(PROFILE_BASE)/simple_server/api \
    -I../../../$(PROFILE_BASE)/simple_server/src \
    -I../../../third_party/FreeRTOS/include \
    -I../../../third_party/FreeRTOS/portable/GCC/ARM_CM4F \
    -I../../../third_party/LittleVGL \
    -I../../../third_party/LittleVGL/src \
    -I../../../third_party/LittleVGL/port \
    -I../../../examples/ble_app_simple_server_lvgl \

# ld file
include $(SDK_BASE)lib/gcc/ldscripts.mk
include $(SDK_BASE)lib/gcc/libs.mk

# rules
include $(SDK_BASE)lib/gcc/rules.mk

