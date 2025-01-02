LITEOSTOPDIR := third_party/kernel_liteos_m

SOURCES +=   $(LITEOSTOPDIR)/kernel/src/los_event.c        \
                $(LITEOSTOPDIR)/kernel/src/los_init.c         \
                $(LITEOSTOPDIR)/kernel/src/los_mux.c          \
                $(LITEOSTOPDIR)/kernel/src/los_queue.c        \
                $(LITEOSTOPDIR)/kernel/src/los_sched.c            \
                $(LITEOSTOPDIR)/kernel/src/los_sem.c              \
                $(LITEOSTOPDIR)/kernel/src/los_sortlink.c         \
                $(LITEOSTOPDIR)/kernel/src/los_swtmr.c            \
                $(LITEOSTOPDIR)/kernel/src/los_task.c             \
                $(LITEOSTOPDIR)/kernel/src/los_tick.c             \
                $(LITEOSTOPDIR)/kernel/src/mm/los_membox.c        \
                $(LITEOSTOPDIR)/kernel/src/mm/los_memory.c

SOURCES +=  $(LITEOSTOPDIR)/utils/los_debug.c                 \
                $(LITEOSTOPDIR)/utils/los_error.c                 \
                $(LITEOSTOPDIR)/utils/los_hook.c

SOURCES +=  $(LITEOSTOPDIR)/kal/cmsis/cmsis_liteos2.c

SOURCES +=  $(LITEOSTOPDIR)/components/power/los_pm.c         \
                $(LITEOSTOPDIR)/components/signal/los_signal.c \
                $(LITEOSTOPDIR)/components/backtrace/los_backtrace.c \
                $(LITEOSTOPDIR)/components/exchook/los_exc_info.c \
                $(LITEOSTOPDIR)/components/exchook/los_exchook.c

SOURCES +=  $(LITEOSTOPDIR)/porting/pmu_timer/los_pmutimer.c  \
                $(LITEOSTOPDIR)/porting/low_power/los_lowpower.c

SOURCES +=  $(LITEOSTOPDIR)/arch/arm/cortex-m4/gcc/los_context.c \
                $(LITEOSTOPDIR)/arch/arm/cortex-m4/gcc/los_dispatch.S \
                $(LITEOSTOPDIR)/arch/arm/cortex-m4/gcc/los_exc.S \
                $(LITEOSTOPDIR)/arch/arm/cortex-m4/gcc/los_interrupt.c \
                $(LITEOSTOPDIR)/arch/arm/cortex-m4/gcc/los_timer.c

INCLUDES  += \
            -I../../../third_party/kernel_liteos_m \
            -I../../../third_party/kernel_liteos_m/arch/include \
            -I../../../third_party/kernel_liteos_m/arch/arm/cortex-m4/gcc \
            -I../../../third_party/kernel_liteos_m/utils \
            -I../../../third_party/kernel_liteos_m/kal/cmsis \
            -I../../../third_party/kernel_liteos_m/kernel/include \
            -I../../../third_party/kernel_liteos_m/components/cpup \
            -I../../../third_party/kernel_liteos_m/components/power \
            -I../../../third_party/kernel_liteos_m/components/backtrace \
            -I../../../third_party/kernel_liteos_m/components/exchook \
            -I../../../third_party/kernel_liteos_m/components/signal \
            -I../../../third_party/kernel_liteos_m/porting/low_power \
            -I../../../third_party/kernel_liteos_m/porting/pmu_timer \
            -I../../../third_party/CMSIS_RTOS2/Include

ASSFLAGS  += -imacros ../../../third_party/kernel_liteos_m/kernel/include/los_config.h -DCLZ=CLZ
DEFINES   += -DHalPendSV=PendSV_Handler
# LITEOS_DEFS += OsTickHandler=SysTick_Handler  # it defined in target_config.h
