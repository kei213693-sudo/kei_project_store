/**
 ****************************************************************************************
 *
 * @file shell.c
 *
 * @brief Simple CLI shell application
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"            // SW configuration

#include <stdio.h>
#include <string.h>

#if (CONFIG_APP_SHELL)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "ke_timer.h"
#include "co_utils.h"               // Common functions
#include "co_timer_pmu.h"
#include "co_debug.h"
#include "uart.h"
#include "shell.h"
#include "peripheral.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/**
 * @brief   Shell maximum arguments per command.
 */
#define SHELL_RX_CHECK_INTERVAL         20

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

struct app_shell_env_tag app_shell_env;
static co_timer_t rx_time;

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
#define BACKSPACE(n) do { \
    uint8_t back[3] = {0x8, 0x20, 0x8}; \
    int i = n; \
    while(i--) { \
        uart_send_block(app_shell_env.config->sc_channel, back, sizeof(back)); \
    } \
} while (0)

#define OUTSHELL do { \
    uint8_t shell[] = "shell> "; \
    uart_send_block(app_shell_env.config->sc_channel, shell, sizeof(shell)); \
} while (0)

static char *_strtok(char *str, const char *delim, char **saveptr) {
    char *token;
    if (str)
        *saveptr = str;
    token = *saveptr;

    if (!token)
        return NULL;

    token += strspn(token, delim);
    *saveptr = strpbrk(token, delim);
    if (*saveptr)
        *(*saveptr)++ = '\0';

    return *token ? token : NULL;
}

static void usage(char *p) {

    log_debug("Usage: %s\r\n", p);
}

static void list_commands(const ShellCommand *scp) {

    while (scp->sc_name != NULL) {
        log_debug("%s ", scp->sc_name);
        scp++;
    }
}

static void cmd_info(int argc, char *argv[]) {

    (void)argv;
	  (void)argc;
    //if (argc > 0) {
		log_debug("version=0x%08x, chipid=0x%08x\r\n", HS_SYS->REV_ID, HS_SYS->CHIP_ID);
		return;
    //}
}

void cmd_mem(int argc, char *argv[])
{
    uint32_t *pu32Ptr, val;
    uint32_t address,cnt,i;
    if (argc == 0) {
print_help:
        log_debug(
                "Usage: mem [options]...\r\n"
                "    r address count:               read count from address\r\n"
                "    w address value:          write value to address\r\n"
                "\r\n"
                "Note:\r\n"
                "    1.address, count, value use hex\r\n"
                "\r\n"
                "Example:      \r\n"
                "    mem r 40000000 4\r\n"
                "    mem w 40000000 FFFFFFFF \r\n"
                );

        return;
    }
    else if (argc == 3) {
         if (strcmp(argv[0], "r") == 0) {
           address = strtoul(argv[1], NULL, 16) & 0xfffffffc;
           cnt     = strtoul(argv[2], NULL, 16);
           pu32Ptr = (uint32_t *)address;
           for(i=0; i<cnt; i++)
           {
             if((i%4) == 0) {
               log_debug("\r\n%08X:\t", address+i*4);
             }
             log_debug("%08X ", pu32Ptr[i]);
           }
           log_debug("\r\n\r\n");
         }
         else if (strcmp(argv[0], "w") == 0) {
           address = strtoul(argv[1], NULL, 16) & 0xfffffffc;
           val     = strtoul(argv[2], NULL, 16);
           pu32Ptr = (uint32_t *)address;
           *pu32Ptr = val;
           if(*pu32Ptr == val)
             log_debug("[0x%08X]=0x%08X write success!\r\n\r\n", address, val);
           else
             log_debug("[0x%08X]=0x%08X write fail!\r\n\r\n", address, *pu32Ptr);
         }
         else
         {
            goto print_help;
         }
    }
    else
    {
        goto print_help;
    }
}

static ShellCommand local_commands[] = {
    {"info", cmd_info},
    {NULL, NULL}
};

static bool cmdexec(const ShellCommand *scp, char *name, int argc, char *argv[]) {

    while (scp->sc_name != NULL) {
        if (strcasecmp(scp->sc_name, name) == 0) {
            scp->sc_function(argc, argv);
            return false;
        }
        scp++;
    }
    return true;
}

static void app_shell_rx_timer_handler(co_timer_t *timer, void *param)
{
    int n;
    char *lp, *cmd, *tokp, *line;
    char *args[SHELL_MAX_ARGUMENTS + 1];
    const ShellCommand *scp = app_shell_env.config->sc_commands;

    line = (char*)app_shell_env.rx_buffer;

    lp = _strtok(line, " \t", &tokp);
    cmd = lp;
    n = 0;
    while ((lp = _strtok(NULL, " \t", &tokp)) != NULL) {
        if (n >= SHELL_MAX_ARGUMENTS) {
            log_debug("too many arguments\r\n");
            cmd = NULL;
            break;
        }
        args[n++] = lp;
    }
    args[n] = NULL;
    if (cmd != NULL) {
        if (strcasecmp(cmd, "exit") == 0) {
            if (n > 0) {
                usage("exit");
            }
            OUTSHELL;
            return;
        }
        else if (strcasecmp(cmd, "help") == 0) {
            if (n > 0) {
                usage("help");
                OUTSHELL;
                return;
            }
            log_debug("Commands: help exit ");
            list_commands(local_commands);
            if (scp != NULL)
                list_commands(scp);
            log_debug("\r\n");
        }
        else if (cmdexec(local_commands, cmd, n, args) &&
                ((scp == NULL) || cmdexec(scp, cmd, n, args))) {
            log_debug("%s", cmd);
            log_debug(" ?\r\n");
        }
    }
    OUTSHELL;
}

void app_shell_init(const ShellConfig* config)
{
    ShellCommand *scp = NULL;
    if (config != NULL) scp = (ShellCommand *)config->sc_commands;

    memset(&app_shell_env, 0, sizeof(struct app_shell_env_tag));
    app_shell_env.config = (ShellConfig*)config;

    log_debug("shell> Commands: help exit ");
    list_commands(local_commands);

    if (scp != NULL)
        list_commands(scp);
    log_debug("\r\n");

    OUTSHELL;
}

/**
 ****************************************************************************************
 * @brief recevice the data
 ****************************************************************************************
 */
void app_shell_rx(uint8_t data)
{
    if (data == 4) {
        log_debug("^D");
        app_shell_env.fifo_len = 0;
        return;
    }

    if ((data== 8) || (data == 127)) {
        if (app_shell_env.fifo_len != 0) {
            app_shell_env.fifo_len -= 1;
            app_shell_env.rx_fifo[app_shell_env.fifo_len] = 0;
            BACKSPACE(1);
        }
        return;
    }

    if (data == '\r') {
        log_debug("\r\n");
        memset(app_shell_env.rx_buffer, 0, app_shell_env.rx_len);
        memcpy(app_shell_env.rx_buffer, app_shell_env.rx_fifo, app_shell_env.fifo_len);
        app_shell_env.rx_len = app_shell_env.fifo_len;
        app_shell_env.fifo_len = 0;
        memset(app_shell_env.rx_fifo, 0, app_shell_env.rx_len);

        if (app_shell_env.rx_len > 0)
            co_timer_set(&rx_time,  SHELL_RX_CHECK_INTERVAL,  TIMER_ONE_SHOT,   app_shell_rx_timer_handler, NULL);
        else
        {
         OUTSHELL;
        }
        return;
    }

    if (data < 0x20)
        return;

    if (app_shell_env.fifo_len >= SHELL_MAX_LINE_LENGTH-1) {
        log_debug("\r\n");
        app_shell_env.fifo_len = 0;
    }

    app_shell_env.config->sc_channel->THR=data;

    app_shell_env.rx_fifo[app_shell_env.fifo_len++] = data;
}
#endif //(CONFIG_APP_SHELL)

/// @} APP

