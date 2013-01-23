#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "uart.h"
#include <util/setbaud.h>


static int uart_putchar(char c, FILE *stream){
	if (c == '\n')
		uart_putchar('\r', stream);

	loop_until_bit_is_set(UCSRA, UDRE);

	UDR = c;

	return 0;
}

static int uart_getchar(FILE *stream){
	loop_until_bit_is_set(UCSRA, RXC);

	return UDR;
}

FILE istream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

static void init_uart(void){
	/* set baud rate */
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	/* set frame format: 8bit, 1stopbit */
	UCSRC = _BV(URSEL)|_BV(UCSZ0)|_BV(UCSZ1);
	/* enable receiver and transmitter */
	UCSRB = _BV(RXEN) | _BV(TXEN);
}

void init_sys_uart(void){
	init_uart();

	stdout = &istream;
	stdin = &istream;
}

char getch(){
	loop_until_bit_is_set(UCSRA, RXC);

	return UDR;
}

void putch(char c){
	loop_until_bit_is_set(UCSRA, UDRE);

	UDR = c;
}

#ifdef ENABLE_SHELL_SUPPORT
/* read_cmd_line
 * @ return argc of cmd_line
 */
uint8_t read_cmd_line(char* buffer, uint8_t buffer_size) {
    memset(buffer, 0, buffer_size);

    uint8_t read_length = 0;
    uint8_t read_argc = 0;
    while(read_length < buffer_size - 1)
    {
        uint8_t c = (uint8_t)uart_getchar(stdin);


	switch(c) {
		case 0x08:
		case 0x7f: /* delete a char */
			if(read_length < 1) break;

			--read_length;
			if (buffer[read_length] == '\0') {
				--read_argc;
			} else {
				buffer[read_length] = '\0';
			}

			uart_putchar(0x08, stdout);
			uart_putchar(' ', stdout);
			uart_putchar(0x08, stdout);

			break;
		case 0x03: /* if received ^C */
			uart_putchar('\n', stdout);
			buffer[0] = 0; 
			return 0;

		case 27: /* if received [ESC], ignore. for arrow keys */
			break;

		case 0x0d:
		case '\n':
			c = '\n';
        		uart_putchar(c, stdout);
            		buffer[read_length] = '\0';
			if ((read_length > 0) && (buffer[read_length-1] != '\0')) ++read_argc;
    			return read_argc;

		case 0x20: 
		case 0x09: /* space/tab, as delimter of cmd and options */
			if ((read_length > 0) && (buffer[read_length-1] != '\0')) {
				uart_putchar(' ', stdout);
				c = '\0';
				buffer[read_length] = c;
				++read_length;
				++read_argc;
			}
			break;
		case 0x0c: /* ^L clear screen */
			if (read_length == 0) { /* only need refresh prompt */
				uart_putchar(c, stdout);
				uart_putchar('>', stdout);
				uart_putchar(' ', stdout);
			}
			break;
		default:
        		if ((c >= 0x20)&&(c < 0x7F)) {
				uart_putchar(c, stdout);
				buffer[read_length] = c;
				++read_length;
			}
			break;
    	}
    }

    uart_putchar('\n', stdout);
    return (read_argc+1);
}
#endif /* ENABLE_SHELL_SUPPORT */
