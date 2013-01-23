#ifndef __UART_H_
#define __UART_H_


void init_sys_uart(void);

#include "config.h"
#ifdef ENABLE_SHELL_SUPPORT
# include <stdint.h>
	uint8_t read_cmd_line(char* buffer, uint8_t buffer_length);
#endif /* ENABLE_SHELL_SUPPORT */

char getch(void);
void putch(char c);


#endif	/* uart.h */
