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
EXAMPLE_BASE = examples/ble_app_hid_mouse

# Freertos source files direcotry
FREERTOS_BASE = third_party/FreeRTOS

# Profile source files direcotry
PROFILE_BASE = stack/ip/ble/profiles

# Defines
DEFINES = -DCONFIG_AUTOCONFG_VERSION -DCONFIG_LOG_OUTPUT

# Optimization: 0, 1, 2, 3, s, 4(ultrasize)
CONFIG_OPTIMIZATION_LEVEL=3

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
    hardware/peripheral/retarget.c \

# App Sources
APP_SOURCES = \
    $(EXAMPLE_BASE)/src/app.c \
    $(EXAMPLE_BASE)/src/app_hid.c \
    $(EXAMPLE_BASE)/src/app_dis.c \
    $(EXAMPLE_BASE)/src/app_task.c \
    examples/common/app_adv.c \
    examples/common/app_sec.c \

# Profile Sources
PROFILE_SOURCES = \
    $(PROFILE_BASE)/hogp/hogpd/src/hogpd.c \
    $(PROFILE_BASE)/hogp/hogpd/src/hogpd_task.c \
    $(PROFILE_BASE)/dis/diss/src/diss.c \
    $(PROFILE_BASE)/dis/diss/src/diss_task.c \

# Stack Sources
STACK_SOURCES = \
    stack/plf/hs6621/rwip/rwmem.c \

# Sources (Relative to the SDK root directory: $SDK_BASE)
SOURCES += $(BASE_SOURCES) $(MAIN_SOURCES) $(APP_SOURCES) $(PROFILE_SOURCES) $(STACK_SOURCES)

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
    -I../../../$(PROFILE_BASE)/dis/diss/src \
    -I../../../$(PROFILE_BASE)/hogp/hogpd/src \

# ld file
include $(SDK_BASE)lib/gcc/ldscripts.mk
include $(SDK_BASE)lib/gcc/libs.mk

# rules
include $(SDK_BASE)lib/gcc/rules.mk

