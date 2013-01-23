/*
 * use interrupt mode maybe more userful!
 */
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "uart.h"
#include "spi_raw.h"

/* auto build by gperf */
#include "shell_hash.h"


void init_system_io(void){
	/* init all io to a fixed power level 
	 * 	all port enable pull-up res
	 */
	DDRA = 0x00;
	PORTA = 0xff;
	DDRB = 0x00;
	PORTB = 0xff;
	DDRC = 0x00;
	PORTC = 0xff;
	DDRD = 0x00;
	PORTD = 0xff;
}

int main(void){
	init_system_io();	/* pull-up *Res* enable */
	init_sys_uart();	/* init uart for printf */
	spi_raw_init();

	char buffer[32];
	struct cmd_argv txdata;

	while(1) {
		putchar('>');
		putchar(' ');

		char *cmd_line = buffer;
		if ((txdata.argc = read_cmd_line(cmd_line, sizeof(buffer))) <= 0)
			continue;
		const struct cmd_config *val;

		val = shell_lookup(cmd_line, strlen(cmd_line));
		if (val != NULL) {
			txdata.argv = cmd_line;	/* point to options */
			int ret = (*(val->func))(&txdata);
			printf_P(PSTR("[I] Exec %s: %d\n"), cmd_line, ret);
		} else {
			printf_P(PSTR("[X] invalid cmd: %s\n"), cmd_line);
		}
	}


	return 0;
}
