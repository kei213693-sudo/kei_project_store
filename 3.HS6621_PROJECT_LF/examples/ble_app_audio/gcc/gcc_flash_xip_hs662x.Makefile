############################################################################################
#
# OnMicro makefile
#
############################################################################################

# Project
PROJECT = gcc_flash_xip_hs662x

# SDK ROOT dir
SDK_BASE = ../../../

# Defines
DEFINES = -DCONFIG_AUTOCONFG_VERSION -DCONFIG_LOG_OUTPUT

# Optimization: 0, 1, 2, 3, s, 4(ultrasize)
CONFIG_OPTIMIZATION_LEVEL=4

# Build obj files direcotry
BUILDDIR	= .build_gcc_flash_xip_hs662x

# ASM Sources
ASM_SOURCES = \

# Sources (Relative to the SDK root directory: $SDK_BASE)
SOURCES = \
    lib/gcc/sys_vectors.c \
    examples/ble_app_audio/src/app.c \
    examples/ble_app_audio/src/main.c \
    hardware/peripheral/retarget.c \
    examples/ble_app_audio/src/app_event.c \
    stack/plf/hs6621/rwip/rwmem.c \
    examples/common/app_adv.c \
    examples/common/app_privacy.c \
    examples/common/app_sec.c \
    examples/common/app_whl.c \
    examples/common/app_ancsc.c \
    examples/common/app_batt.c \
    examples/common/app_dis.c \
    examples/common/app_hid.c \
    examples/common/app_task.c \
    examples/common/app_tspp_server.c \
    examples/common/app_wechat.c \
    stack/ip/ble/profiles/tspp/tspps/src/tspps.c \
    stack/ip/ble/profiles/tspp/tspps/src/tspps_task.c \
    stack/ip/ble/profiles/bas/bass/src/bass.c \
    stack/ip/ble/profiles/bas/bass/src/bass_task.c \
    stack/ip/ble/profiles/dis/diss/src/diss.c \
    stack/ip/ble/profiles/dis/diss/src/diss_task.c \
    stack/ip/ble/profiles/hogp/hogpd/src/hogpd.c \
    stack/ip/ble/profiles/hogp/hogpd/src/hogpd_task.c \
    examples/common/wechat/AES.c \
    examples/common/wechat/crc32.c \
    examples/common/wechat/epb.c \
    examples/common/wechat/epb_MmBp.c \
    examples/common/wechat/md5.c \
    examples/common/wechat/mpbledemo2.c \
    examples/common/wechat/wechat_util.c \


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
    -I../../../stack/ip/ble/profiles/anp \
    -I../../../stack/ip/ble/profiles/bas \
    -I../../../stack/ip/ble/profiles/bcs \
    -I../../../stack/ip/ble/profiles/blp \
    -I../../../stack/ip/ble/profiles/cpp \
    -I../../../stack/ip/ble/profiles/cscp \
    -I../../../stack/ip/ble/profiles/dis \
    -I../../../stack/ip/ble/profiles/find \
    -I../../../stack/ip/ble/profiles/glp \
    -I../../../stack/ip/ble/profiles/hogp \
    -I../../../stack/ip/ble/profiles/hrp \
    -I../../../stack/ip/ble/profiles/htp \
    -I../../../stack/ip/ble/profiles/lan \
    -I../../../stack/ip/ble/profiles/pasp \
    -I../../../stack/ip/ble/profiles/prox \
    -I../../../stack/ip/ble/profiles/rscp \
    -I../../../stack/ip/ble/profiles/scpp \
    -I../../../stack/ip/ble/profiles/tip \
    -I../../../stack/ip/ble/profiles/uds \
    -I../../../stack/ip/ble/profiles/wscp \
    -I../../../stack/ip/ble/profiles/anp/anpc/api \
    -I../../../stack/ip/ble/profiles/anp/anps/api \
    -I../../../stack/ip/ble/profiles/bas/basc/api \
    -I../../../stack/ip/ble/profiles/bas/bass/api \
    -I../../../stack/ip/ble/profiles/bcs/bcsc/api \
    -I../../../stack/ip/ble/profiles/bcs/bcss/api \
    -I../../../stack/ip/ble/profiles/blp/blpc/api \
    -I../../../stack/ip/ble/profiles/blp/blps/api \
    -I../../../stack/ip/ble/profiles/cpp/cppc/api \
    -I../../../stack/ip/ble/profiles/cpp/cpps/api \
    -I../../../stack/ip/ble/profiles/cscp/cscpc/api \
    -I../../../stack/ip/ble/profiles/cscp/cscps/api \
    -I../../../stack/ip/ble/profiles/dis/disc/api \
    -I../../../stack/ip/ble/profiles/dis/diss/api \
    -I../../../stack/ip/ble/profiles/find/findl/api \
    -I../../../stack/ip/ble/profiles/find/findt/api \
    -I../../../stack/ip/ble/profiles/glp/glpc/api \
    -I../../../stack/ip/ble/profiles/glp/glps/api \
    -I../../../stack/ip/ble/profiles/hogp/hogpbh/api \
    -I../../../stack/ip/ble/profiles/hogp/hogpd/api \
    -I../../../stack/ip/ble/profiles/hogp/hogprh/api \
    -I../../../stack/ip/ble/profiles/hrp/hrpc/api \
    -I../../../stack/ip/ble/profiles/hrp/hrps/api \
    -I../../../stack/ip/ble/profiles/htp/htpc/api \
    -I../../../stack/ip/ble/profiles/htp/htpt/api \
    -I../../../stack/ip/ble/profiles/lan/lanc/api \
    -I../../../stack/ip/ble/profiles/lan/lans/api \
    -I../../../stack/ip/ble/profiles/pasp/paspc/api \
    -I../../../stack/ip/ble/profiles/pasp/pasps/api \
    -I../../../stack/ip/ble/profiles/prox/proxm/api \
    -I../../../stack/ip/ble/profiles/prox/proxr/api \
    -I../../../stack/ip/ble/profiles/rscp/rscpc/api \
    -I../../../stack/ip/ble/profiles/rscp/rscps/api \
    -I../../../stack/ip/ble/profiles/scpp/scppc/api \
    -I../../../stack/ip/ble/profiles/scpp/scpps/api \
    -I../../../stack/ip/ble/profiles/tip/tipc/api \
    -I../../../stack/ip/ble/profiles/tip/tips/api \
    -I../../../stack/ip/ble/profiles/uds/udsc/api \
    -I../../../stack/ip/ble/profiles/uds/udss/api \
    -I../../../stack/ip/ble/profiles/wscp/wscc/api \
    -I../../../stack/ip/ble/profiles/wscp/wscs/api \
    -I../../../stack/ip/ble/profiles/tspp/tspps/api \
    -I../../../stack/ip/ble/profiles/tspp/tsppc/api \
    -I../../../examples/common \
    -I../../../stack/plf/hs6621/bb \
    -I../../../stack/plf/hs6621/sc \
    -I../src \
    -I../../../stack/ip/ble/profiles/tspp \
    -I../../../stack/ip/ble/profiles/tspp/tspps/api \
    -I../../../stack/ip/ble/profiles/onmicro_dfu/api \
    -I../../../stack/ip/ble/profiles/onmicro_dfu/src \
    -I../../../stack/ip/ble/profiles/wechat/api \
    -I../../../stack/ip/ble/profiles/dis/diss/api \
    -I../../../stack/ip/ble/profiles/dis/diss/src \
    -I../../../stack/ip/ble/profiles/bas/bass/src \
    -I../../../stack/ip/ble/profiles/hogp/hogpd/src \
    -I../../../stack/ip/ble/profiles/ancs/src \
    -I../../common/sbc \
    -I../../common/sbc/encoder/include \


# ld file
include $(SDK_BASE)lib/gcc/ldscripts.mk
include $(SDK_BASE)lib/gcc/libs.mk

LDLIBS_ROM+= \
../../common/sbc/libsbc.a \


# rules
include $(SDK_BASE)lib/gcc/rules.mk

