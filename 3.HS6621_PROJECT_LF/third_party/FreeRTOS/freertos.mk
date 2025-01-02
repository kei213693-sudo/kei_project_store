OS_BASE_DIR = $(THIRD_PARTY_DIR)/FreeRTOS

OS_SOURCES = \
    $(OS_BASE_DIR)/list.c \
    $(OS_BASE_DIR)/queue.c \
    $(OS_BASE_DIR)/tasks.c \
    $(OS_BASE_DIR)/timers.c \
    $(OS_BASE_DIR)/event_groups.c \
    $(OS_BASE_DIR)/portable/MemMang/heap_4.c \
    $(OS_BASE_DIR)/portable/GCC/ARM_CM4F/port.c \

OS_INCLUDES = \
    $(OS_BASE_DIR)/include \
    $(OS_BASE_DIR)/portable/GCC/ARM_CM4F/ \

