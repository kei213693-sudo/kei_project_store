#include <stdint.h>
#include <stdbool.h>
#include "peripheral.h"
#include "mbr.h"

#define VER_HS6621A3        // Chip version: VER_OM6621A3 / VER_OM6621CA2.
#define FLASH_APP_MBR_TYPE  PART_TYPE_USR2 // 注意！DFU升级时APP打包参数要与此处一致
#define LDR_COPY_BUF_SIZE   1024
#define BOOT_TRY_TIMES      4
#define FLASH_LOADER_DEBUG  0

#if defined(VER_HS6621A3)
#define ROM_ID              0x66210300
#define FLASH_ADDR_MASK     0x12000000
#define FLASH_APP_DEF_ADDR  0x3000
#elif defined(VER_OM6621CA2)
#define ROM_ID              0x6621C200
#define FLASH_ADDR_MASK     0x00400000
#define FLASH_APP_DEF_ADDR  0x3000
#else
#error "Chip version ERROR."
#endif

enum {
    ERR_NO_ERROR          = 1 << 0,
    ERR_NO_NEW_APP        = 1 << 1, // MBR empty, running at defualt address
    ERR_NEW_APP_ADDR      = 1 << 2, // MBR read success, no need to copy APP
    ERR_CRC_ERROR         = 1 << 3,
    ERR_INVALID_IMAGE     = 1 << 4,
};

enum {
    RUN_FLASH_DEF       = 1 << 0,
    RUN_FLASH_SPEC      = 1 << 1,
    RUN_TO_FLASH_MASK   = 0x03,
};
uint32_t m_loader_addr;
uint32_t m_loader_size;
uint32_t m_app_addr;
uint32_t m_app_size;
uint16_t m_app_crc;
uint32_t m_app_msp;
uint32_t m_app_pc;
uint32_t m_loader_result = 0xF10ADE00;
__ALIGNED(16) uint8_t m_copy_buffer[LDR_COPY_BUF_SIZE]; // for flash copy

__WEAK void rwmem_init(void) {}
#if !FLASH_LOADER_DEBUG
#undef log_debug
#define log_debug(...)
#else
#define PIN_UART0_TX                    5
#define DEBUG_UART_BAUDRATE             115200
void system_init_run2sf(void);
static void hardware_init(void)
{
    pmu_pin_mode_set(0xFFFFFFFF, PMU_PIN_MODE_PU);
    pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
    gpio_open();
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}
static void ble_stack_config(void)
{
    // Disable WDT
    wdt_enable(0);
    // Enable DCDC
    pmu_dcdc_enable(true);
    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(false);
    // Select 32k clock for stack
    pmu_select_32k(PMU_32K_SEL_32768HZ_XTAL);
    // xtal32m param
    pmu_xtal32m_change_param(15);
    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(false);
    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
}
#endif

static void ldr_boot_fail_handler(uint32_t err_bit, bool halt)
{
    m_loader_result |= err_bit;
    while (halt);
}

static void loader_data_diff(uint32_t src, uint32_t dst, uint32_t length)
{
#if FLASH_LOADER_DEBUG
    // Dump error data.
    uint8_t local_buffer[LDR_COPY_BUF_SIZE + 16];
    uint8_t* buffer = (uint8_t*)((((uint32_t)local_buffer + 15) % ~15));
    int i, j;
    for (i = 0; i < length; i += LDR_COPY_BUF_SIZE) {
        sfs_read(src + i, m_copy_buffer, LDR_COPY_BUF_SIZE);
        sfs_read(src + i, buffer, LDR_COPY_BUF_SIZE);
        if (memcmp(m_copy_buffer, buffer, LDR_COPY_BUF_SIZE)) {
            log_debug("[BL] Error Offset 0x%08X \n", src + i);
            for (j = 0; j < LDR_COPY_BUF_SIZE; j++) {
                if (!(j % 32)) {
                    log_debug("\n");
                }
                log_debug("%02X ", m_copy_buffer[j]);
            }
            log_debug("\n");
            for (j = 0; j < LDR_COPY_BUF_SIZE; j++) {
                if (!(j % 32)) {
                    log_debug("\n");
                }
                log_debug("%02X ", buffer[j]);
            }
            log_debug("\n");
            break;
        }
    }
    log_debug("[BL] No more diff infomation.\n");
#endif
}

static void ldr_copy_app_in_sflash(uint32_t src, uint32_t dst, uint32_t length, uint16_t crc16)
{
    int try_time;
    for (try_time = 0; try_time < BOOT_TRY_TIMES; ++try_time) {
        uint32_t i;
        uint32_t length_align = (length / LDR_COPY_BUF_SIZE) * LDR_COPY_BUF_SIZE;
        uint32_t length_left = length % LDR_COPY_BUF_SIZE;
        uint16_t app_crc16;
        // Erase app
        sfs_erase(dst, length);
        // Copy
        for (i = 0; i < length_align; i += LDR_COPY_BUF_SIZE) {
            log_debug("[BL] Src:0x%08x Dst:0x%08x, Len:%d \n", src + i, dst + i, LDR_COPY_BUF_SIZE);
            sfs_read(src + i, m_copy_buffer, LDR_COPY_BUF_SIZE);
            sfs_write(dst + i, m_copy_buffer, LDR_COPY_BUF_SIZE);
        }
        if (length_left) {
            log_debug("[BL] Src:0x%08x Dst:0x%08x, Len:%d \n", src + i, dst + i, length_left);
            sfs_read(src + i, m_copy_buffer, length_left);
            sfs_write(dst + i, m_copy_buffer, length_left);
        }
        // CRC
        app_crc16 = 0;
        for (i = 0; i < length_align; i += LDR_COPY_BUF_SIZE) {
            sfs_read(dst + i, m_copy_buffer, LDR_COPY_BUF_SIZE);
            app_crc16 = co_crc16_ccitt(app_crc16, m_copy_buffer, LDR_COPY_BUF_SIZE);
        }
        if (length_left) {
            sfs_read(dst + i, m_copy_buffer, length_left);
            app_crc16 = co_crc16_ccitt(app_crc16, m_copy_buffer, length_left);
        }
        // Enable
        if (app_crc16 == crc16) {
            mbr_write_part(FLASH_APP_MBR_TYPE, dst, length, crc16);
            log_debug("[BL] MBR updated from  0x%08X to 0x%08X\n", src, dst);
            return;
        } else {
            log_debug("[BL] Crc not match 0x%08X vs. 0x%08X\n", crc16, app_crc16);
        }
    }
    loader_data_diff(src, dst, length);
    ldr_boot_fail_handler(ERR_CRC_ERROR, true);
}
static int ldr_get_run_target(uint32_t* addr, uint32_t* size, uint16_t* crc, uint32_t* load_addr)
{
    bool is_def_addr = true;
    int res = mbr_read_part(FLASH_APP_MBR_TYPE, addr, size, crc);
    if (res >= 0) { // App in specified address
        is_def_addr = false;
    } else { // Read failed, App in default address
        *addr = FLASH_APP_DEF_ADDR;
        *size = 0xFFFFFFFF;
        *crc = 0xFFFF;
    }
#define VEC_RUN2SF_OFFSET 9
#define VEC_RUN2SF_MAGIC 10
#define VEC_APP_MAGIC    13
#define RUN2SF_MAGIC_FLAG   0x46533252
    uint32_t vector[16] = {0};
    sfs_read(*addr, vector, sizeof(vector));
    if (vector[VEC_APP_MAGIC] == ROM_ID) {
        if (vector[VEC_RUN2SF_MAGIC] == RUN2SF_MAGIC_FLAG) {
            *load_addr = vector[VEC_RUN2SF_OFFSET] & SFS_OFFSET_MASK;
            if (*load_addr == SFS_OFFSET_MASK) {
                *load_addr = FLASH_APP_DEF_ADDR;
            }
            if (is_def_addr) {
                return RUN_FLASH_DEF;
            } else {
                return RUN_FLASH_SPEC;
            }
        }
    }
    log_debug("[BL] Error Image! MAGIC:0x%08X, ROM_ID:0x%08X\n", vector[VEC_RUN2SF_MAGIC], vector[VEC_APP_MAGIC]);
    ldr_boot_fail_handler(ERR_INVALID_IMAGE, true);
    return 0; // Can't reach here.
}
void flash_loader_run(void)
{
#if FLASH_LOADER_DEBUG
    system_init_run2sf();
    // ble_stack_config();
    hardware_init();
#endif
    log_debug("\n\n[BL] Running Loader %d\n", pmu_reboot_reason());
    int res, run_to;
    uint32_t app_load_addr, app_exec_addr;
    sfs_enable();
    res = mbr_read_part(PART_TYPE_APP, &m_loader_addr, &m_loader_size, NULL);
    if (res >= 0) {
        log_debug("[BL] Bootloader address: 0x%08x, size: 0x%08x(%d)\n", m_loader_addr, m_loader_size, m_loader_size);
    }
    run_to = ldr_get_run_target(&m_app_addr, &m_app_size, &m_app_crc, &app_load_addr);
    if (run_to & RUN_TO_FLASH_MASK) { // run to sflash
        if (run_to & RUN_FLASH_SPEC) {
            log_debug("[BL] App address(FLASH): 0x%08x, size: 0x%08x(%d), crc: 0x%04x\n",
                      m_app_addr, m_app_size, m_app_size, m_app_crc);
            if (m_app_addr != app_load_addr) {
                if (app_load_addr >= FLASH_APP_DEF_ADDR && app_load_addr != SFS_OFFSET_MASK) {
                    ldr_copy_app_in_sflash(m_app_addr, app_load_addr, m_app_size, m_app_crc);
                } else {
                    log_debug("[BL] Error Image! App load addr: 0x%08x\n", app_load_addr);
                }
            } else {
                ldr_boot_fail_handler(ERR_NEW_APP_ADDR, false);
            }
        } else {
            log_debug("[BL] App address(FLASH): 0x%08X(default), size: unknown\n", m_app_addr);
            ldr_boot_fail_handler(ERR_NO_NEW_APP, false);
        }
        app_exec_addr = FLASH_ADDR_MASK | app_load_addr;
    }
    // Get APP MSP & PC
    m_app_msp = *(uint32_t*)(app_exec_addr);
    m_app_pc = *(uint32_t*)(app_exec_addr + sizeof(uint32_t));
    log_debug("[BL] msp=0x%08X, pc=0x%08X\n", m_app_msp, m_app_pc);
    // Jump to APP
    ldr_boot_fail_handler(ERR_NO_ERROR, false);
    __set_MSP(m_app_msp);
    (*((void (*)())(m_app_pc)))();
    while (1);
}

int main(void)
{
    flash_loader_run();
}
