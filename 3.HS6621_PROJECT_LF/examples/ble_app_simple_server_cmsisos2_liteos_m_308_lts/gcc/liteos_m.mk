LITEOSTOPDIR := third_party/kernel_liteos_m_308_lts

SOURCES  += \
            $(LITEOSTOPDIR)/kernel/arch/arm/cortex-m4/gcc/los_dispatch.S \
            $(LITEOSTOPDIR)/kernel/arch/arm/cortex-m4/gcc/los_exc.S

SOURCES  += \
            $(LITEOSTOPDIR)/kernel/arch/arm/cortex-m4/gcc/los_interrupt.c \
            $(LITEOSTOPDIR)/kernel/arch/arm/cortex-m4/gcc/los_context.c \
            $(LITEOSTOPDIR)/kernel/arch/arm/cortex-m4/gcc/los_timer.c \
            $(LITEOSTOPDIR)/kernel/arch/arm/cortex-m4/gcc/los_mpu.c \
            $(LITEOSTOPDIR)/kernel/src/los_event.c \
            $(LITEOSTOPDIR)/kernel/src/los_init.c \
            $(LITEOSTOPDIR)/kernel/src/los_mux.c \
            $(LITEOSTOPDIR)/kernel/src/los_queue.c \
            $(LITEOSTOPDIR)/kernel/src/los_sched.c \
            $(LITEOSTOPDIR)/kernel/src/los_sem.c \
            $(LITEOSTOPDIR)/kernel/src/los_sortlink.c \
            $(LITEOSTOPDIR)/kernel/src/los_swtmr.c \
            $(LITEOSTOPDIR)/kernel/src/los_task.c \
            $(LITEOSTOPDIR)/kernel/src/los_tick.c \
            $(LITEOSTOPDIR)/kernel/src/mm/los_membox.c \
            $(LITEOSTOPDIR)/kernel/src/mm/los_memory.c \
            $(LITEOSTOPDIR)/components/cpup/los_cpup.c \
            $(LITEOSTOPDIR)/components/power/los_pm.c \
            $(LITEOSTOPDIR)/components/backtrace/los_backtrace.c \
            $(LITEOSTOPDIR)/components/exchook/los_exchook.c \
            $(LITEOSTOPDIR)/components/exchook/los_exc_info.c \
            $(LITEOSTOPDIR)/utils/los_debug.c \
            $(LITEOSTOPDIR)/utils/los_error.c \
            $(LITEOSTOPDIR)/utils/los_hook.c \
            $(LITEOSTOPDIR)/kal/cmsis/cmsis_liteos2.c \
            $(LITEOSTOPDIR)/porting/pmu_timer/los_pmutimer.c \
            $(LITEOSTOPDIR)/porting/low_power/los_lowpower.c

INCLUDES  += \
            -I../../../$(LITEOSTOPDIR) \
            -I../../../$(LITEOSTOPDIR)/kernel/arch/include \
            -I../../../$(LITEOSTOPDIR)/kernel/arch/arm/cortex-m4/gcc \
            -I../../../$(LITEOSTOPDIR)/utils \
            -I../../../$(LITEOSTOPDIR)/kal/cmsis \
            -I../../../$(LITEOSTOPDIR)/kernel/include \
            -I../../../$(LITEOSTOPDIR)/components/cpup \
            -I../../../$(LITEOSTOPDIR)/components/power \
            -I../../../$(LITEOSTOPDIR)/components/backtrace \
            -I../../../$(LITEOSTOPDIR)/components/exchook \
            -I../../../$(LITEOSTOPDIR)/components/signal \
            -I../../../$(LITEOSTOPDIR)/porting/include \
            -I../../../$(LITEOSTOPDIR)/porting/pmu_timer \
            -I../../../$(LITEOSTOPDIR)/porting/low_power


ASSFLAGS  += -imacros ../../../$(LITEOSTOPDIR)/kernel/include/los_config.h -DCLZ=CLZ
DEFINES   += -DHalPendSV=PendSV_Handler
# LITEOS_DEFS += OsTickHandler=SysTick_Handler  # it defined in target_config.h
