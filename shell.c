#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include "shell.h"
#include "uart.h"
#include "xmodem.h"
#include "crctab.h"
#include "mx25l32.h"

#include "config.h"
#include <util/delay.h>


int show_help(void *argv) {
	printf_P(PSTR("Usage: cmd [options]\n"
		 "       echo            print string\n"
		 "       stat            read mx25l32 status reg\n"
		 "       dump            dump mx25l32 flash\n"
		 "       serase          erase sector of flash\n"
		 "       berase          erase block of flash\n"
		 "       cerase          erase all of flash\n"
		 "       upload          upload firmware to server\n"
		 "       download        download firmware to board\n"
		 "       help            show this msg\n"
		 "\nBuild time: %s %s @Kicer\n"), __DATE__, __TIME__);


	return 0;
}

int echo_string(void *argv) {
	struct cmd_argv *pargv = (struct cmd_argv *)argv;

	printf_P(PSTR("argc: %d\n"), pargv->argc);

	int i;
	char *pnext = pargv->argv;

	for(i=0; i<pargv->argc; i++) {
		printf_P(PSTR("argv[%d]: %s\n"), i, pnext);
		pnext += strlen(pnext) + 1;
	}


	return pargv->argc;
}

void xmodem_wait() {
	_delay_ms(1);
}

/* for flash r/w addr */
uint32_t g_waddr = 0;
uint32_t g_raddr = 0;
uint32_t g_rlen = 0;

char proc_rec_block(struct xmodem_block_st *rec) {
#if 1 /* flash LED */
	DDRA = 0xFF;
	PORTA = rec->blk;
#endif
#if 1
	char buf[256];

	if ((g_waddr & 0xFF) == 0) {
			mx25l32_page_program(g_waddr, rec->data, 128);
	} else if ((g_waddr & 0x7F) == 0) {
			mx25l32_read_byte(g_waddr-128, buf, 128);
			memcpy(buf+128, rec->data, 128);
			mx25l32_page_program(g_waddr-128, buf, 256);
	} else {
			return -1;
	}
#endif

#if 1 /* check value */
	memset(buf, 0, 128);
	mx25l32_read_byte(g_waddr, buf, 128);
	uint8_t i;
	for(i=0; i<128; i++) {
			if (rec->data[i] != buf[i])
					return -2;
	}
#endif

	g_waddr += 128;

	return 0;
}

/* > download [delay ms]
 */
int file_download(void *argv) {
	struct cmd_argv *pargv = (struct cmd_argv *)argv;

	if (pargv->argc != 3) {
		printf_P(PSTR("Usage: %s [addr] [delay sec]\n"), pargv->argv);
		return -1;
	}

	char *phex = pargv->argv;
	phex = phex + strlen(phex) + 1;
	g_waddr = (uint32_t)strtol(phex, NULL, 0);
	phex = phex + strlen(phex) + 1;
	unsigned char sec = (unsigned char)strtol(phex, NULL, 0);

	printf_P(PSTR("rx: flash firmware: [sA] %04x%04x\n"),
			(uint16_t)(g_waddr>>16), (uint16_t)(g_waddr)); 
	printf_P(PSTR("rx: delay %d s\n"), sec);
	while(sec--) { _delay_ms(1000); }

	if (xmodem_rec(proc_rec_block) == 0) {
		printf_P(PSTR("rx: flash firmware: [eA] %04x%04x\n"),
			(uint16_t)(g_waddr>>16), (uint16_t)(g_waddr)); 
		printf_P(PSTR("rx: transfer finished\n"));
	} else {
		printf_P(PSTR("rx: transfer error\n"));
	}


	return 0;
}

char send_block(struct xmodem_block_st *send) {
	  static uint32_t addr = 0;

	  if (send->soh == XMODEM_CRC) { /* send 1st */
		    addr = g_raddr;
	  } else if (send->soh == XMODEM_ACK) { /* send next */
		    addr += sizeof(send->data);
		    if (addr >= (g_raddr+g_rlen)) return 1; /* end */
	  } else if (send->soh == XMODEM_NAK) { /* re-send */
		    /* addr = addr; */
	  }

	  mx25l32_read_byte(addr, send->data, sizeof(send->data));

	  return 0;
}

int file_upload(void *argv) {
	struct cmd_argv *pargv = (struct cmd_argv *)argv;

	if (pargv->argc != 3) {
		printf_P(PSTR("Usage: %s [addr] [len]\n"), pargv->argv);
		return -1;
	}

	char *phex = pargv->argv;
	phex = phex + strlen(phex) + 1;
	g_raddr = (uint32_t)strtol(phex, NULL, 0);
	phex = phex + strlen(phex) + 1;
	g_rlen = (uint32_t)strtol(phex, NULL, 0);

	printf_P(PSTR("tx: sending firmware: [A] 0x%04x%04x, [L] 0x%04x%04x\n"),
					(uint16_t)(g_raddr>>16), (uint16_t)(g_raddr), 
					(uint16_t)(g_rlen>>16), (uint16_t)(g_rlen)); 

	if (xmodem_send(send_block) == 0)
		printf_P(PSTR("tx: send finished\n"));
	else
		printf_P(PSTR("tx: send error\n"));

	return 0;
}

int flash_read_status(void *argv) {
	printf_P(PSTR("mx25L32 status reg: %02x\n"), mx25l32_read_status());

	uint32_t id = mx25l32_read_id();
	printf_P(PSTR("mx25L32 manufactory id: %02x\n"), (uint8_t)(id>>16));
	printf_P(PSTR("mx25L32 device id: %04x\n"), (uint16_t)id);

	return 0;
}

int flash_berase(void *argv) {
	struct cmd_argv *pargv = (struct cmd_argv *)argv;

	if (pargv->argc != 3) {
		printf_P(PSTR("Usage: %s [addr] [len]\n"), pargv->argv);
		return -1;
	}

	char *phex = pargv->argv;
	phex = phex + strlen(phex) + 1;
	uint32_t addr = (uint32_t)strtol(phex, NULL, 0);
	phex = phex + strlen(phex) + 1;
	uint32_t len = (uint32_t)strtol(phex, NULL, 0);

	if ((len < MX25L32_BLOCK_SIZE) || (len > MX25L32_FLASH_SIZE)) {
		printf_P(PSTR("erase length invalid\n"));
		return -1;
	}

	if (0 != (addr & 0xFFFF)) {
		printf_P(PSTR("erase addr invalid\n"));
		return -1;
	}

	while(len >= MX25L32_BLOCK_SIZE) {
		if (0 != mx25l32_block_erase(addr)) {
			printf_P(PSTR("erase block error\n"));
			return -1;
		}
		addr += MX25L32_BLOCK_SIZE;
		len -= MX25L32_BLOCK_SIZE;
	}

	printf_P(PSTR("erase block finished\n"));

	return 0;
}

int flash_serase(void *argv) {
	struct cmd_argv *pargv = (struct cmd_argv *)argv;

	if (pargv->argc != 3) {
		printf_P(PSTR("Usage: %s [addr] [len]\n"), pargv->argv);
		return -1;
	}

	char *phex = pargv->argv;
	phex = phex + strlen(phex) + 1;
	uint32_t addr = (uint32_t)strtol(phex, NULL, 0);
	phex = phex + strlen(phex) + 1;
	uint32_t len = (uint32_t)strtol(phex, NULL, 0);

	if ((len < MX25L32_SECTOR_SIZE) || (len > MX25L32_FLASH_SIZE)) {
		printf_P(PSTR("erase length invalid\n"));
		return -1;
	}

	if (0 != (addr & 0x3FFF)) {
		printf_P(PSTR("erase addr invalid\n"));
		return -1;
	}

	while(len >= MX25L32_SECTOR_SIZE) {
		if (0 != mx25l32_sector_erase(addr)) {
			printf_P(PSTR("erase sector error\n"));
			return -1;
		}
		addr += MX25L32_SECTOR_SIZE;
		len -= MX25L32_SECTOR_SIZE;
	}

	printf_P(PSTR("erase sector finished\n"));

	return 0;
}

int flash_cerase(void *argv) {
	if (0 != mx25l32_chip_erase()) {
		printf_P(PSTR("erase chip error\n"));
		return -1;
	}

	printf_P(PSTR("erase chip finished\n"));

	return 0;
}

int flash_dump(void *argv) {
	struct cmd_argv *pargv = (struct cmd_argv *)argv;

	if (pargv->argc != 2) {
		printf_P(PSTR("Usage: %s [addr]\n"), pargv->argv);
		return -1;
	}

	char data[128] = "";
	char *phex = pargv->argv;
	phex = phex + strlen(phex) + 1;
	uint32_t addr = (uint32_t)strtol(phex, NULL, 0);
	mx25l32_read_byte(addr, data, 128);

	int i;
	for(i=0; i<128; i++) {
		if (0 == (i % 16)) {
			putchar('\n');
			printf_P(PSTR("%04x%04x  "), (uint16_t)((addr+i)>>16), (uint16_t)(addr+i));
		} else if (0 == (i % 8)) {
			putchar(' ');
		}
		printf_P(PSTR("%02x "), (unsigned char)data[i]);
	}
	putchar('\n');

	return 0;
}
