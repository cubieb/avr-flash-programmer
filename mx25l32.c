#include <stdint.h>
#include <avr/io.h>
#include "mx25l32.h"
#include "spi_raw.h"
#include "spi_raw_config.h"

#define STATUS_SRWD	7
#define STATUS_BP3	5
#define STATUS_BP2	4
#define STATUS_BP1	3
#define STATUS_BP0	2
#define STATUS_WEL	1
#define STATUS_WIP	0

#define CMD_WREN	0x06
#define CMD_WRDI	0x04
#define CMD_WRSR	0x01
#define CMD_RDID	0x9F
#define CMD_RDSR	0x05
#define CMD_READ	0x03
#define CMD_FREAD	0x0B
#define CMD_RDSFDP	0x5A
#define CMD_RES		0xAB
#define CMD_REMS	0x90
#define CMD_DREAD	0x3B
#define CMD_SE		0x20 /* 4K */
#define CMD_BE		0xD8 /* or 0x52, 64K */
#define CMD_CE		0x60 /* or 0xC7 */
#define CMD_PP		0x02
#define CMD_DP		0xB9
#define CMD_RDP		0xAB
#define CMD_RDSCUR	0x2B
#define CMD_WRSCUR	0x2F
#define CMD_ENSO	0xB1
#define CMD_EXSO	0xC1

#define TIMEOUT_CHIP_ERASE		((uint32_t)50*1000*1000) /* Max. 50s */
#define TIMEOUT_BLOCK_ERASE		((uint32_t)2*1000*1000) /* Max. 2s */
#define TIMEOUT_SECTOR_ERASE	((uint32_t)300*1000) /* Max. 300ms */
#define TIMEOUT_WRITE_STATUS	((uint32_t)40*1000) /* Max. 40ms */
#define TIMEOUT_PAGE_PROGRAM	((uint32_t)5*1000) /* Max. 5ms */
#define TIMEOUT_BYTE_PROGRAM	((uint32_t)300) /* Max. 300us */


/* WREN
 * write enable */
void mx25l32_write_enable() {
	select_device();
	spi_raw_send_byte(CMD_WREN);
	unselect_device();
}

/* WRDI
 * write disable */
void mx25l32_write_disable() {
	select_device();
	spi_raw_send_byte(CMD_WRDI);
	unselect_device();
}

/* RDID
 * read id */
uint32_t mx25l32_read_id(void) {
	select_device();
	spi_raw_send_byte(CMD_RDID);
	uint8_t c0 = spi_raw_rec_byte();
	uint8_t c1 = spi_raw_rec_byte();
	uint8_t c2 = spi_raw_rec_byte();
	unselect_device();

	uint32_t id = c0;
	id = c1 | (id << 8);
	id = c2 | (id << 8);

	return id;
}

/* RDSR
 * read status register */
uint8_t mx25l32_read_status() {
	select_device();
	spi_raw_send_byte(CMD_RDSR);
	uint8_t val = spi_raw_rec_byte();
	unselect_device();

	return val;
}

char mx25l32_is_busy() {
	uint8_t status = mx25l32_read_status();

	return (status & (1<<STATUS_WIP));
}

/* if timeout, return -1 */
char mx25l32_wait_flash_ready(uint32_t timeout) {
	//uint32_t temp = 0;

	while(mx25l32_is_busy()) {
		//if (temp > timeout) return -1;
		//temp++;
		while(timeout > 100){
			timeout -= 100;
		};
	}

	return 0;
}

/* WRSR
 * write status register */
uint8_t mx25l32_write_status(char status) {
	if (mx25l32_is_busy()) return -2;

	mx25l32_write_enable();

	select_device();
	spi_raw_send_byte(CMD_WRSR);
	spi_raw_send_byte(status);
	unselect_device();

	return mx25l32_wait_flash_ready(TIMEOUT_WRITE_STATUS);
}

/* READ
 * read byte */
void mx25l32_read_byte(uint32_t addr, char *pdata, uint32_t length) {
	select_device();
	spi_raw_send_byte(CMD_READ);
	spi_raw_send_byte((addr >> 16) & 0xFF);
	spi_raw_send_byte((addr >>  8) & 0xFF);
	spi_raw_send_byte((addr >>  0) & 0xFF);
	while(length--) {
		*pdata++ = spi_raw_rec_byte();
	}
	unselect_device();
}

/* CE
 * chip erase */
char mx25l32_chip_erase() {
	if (mx25l32_is_busy()) return -2;

	mx25l32_write_enable();

	select_device();
	spi_raw_send_byte(CMD_CE);
	unselect_device();

	return mx25l32_wait_flash_ready(TIMEOUT_CHIP_ERASE);
}

/* BE
 * block erase */
char mx25l32_block_erase(uint32_t addr) {
	if (mx25l32_is_busy()) return -2;

	mx25l32_write_enable();

	select_device();
	spi_raw_send_byte(CMD_BE);
	spi_raw_send_byte((addr >> 16) & 0xFF);
	spi_raw_send_byte((addr >>  8) & 0xFF);
	spi_raw_send_byte((addr >>  0) & 0xFF);
	unselect_device();

	return mx25l32_wait_flash_ready(TIMEOUT_BLOCK_ERASE);
}

/* SE
 * sector erase */
char mx25l32_sector_erase(uint32_t addr) {
	if (mx25l32_is_busy()) return -2;

	mx25l32_write_enable();

	select_device();
	spi_raw_send_byte(CMD_SE);
	spi_raw_send_byte((addr >> 16) & 0xFF);
	spi_raw_send_byte((addr >>  8) & 0xFF);
	spi_raw_send_byte((addr >>  0) & 0xFF);
	unselect_device();

	return mx25l32_wait_flash_ready(TIMEOUT_SECTOR_ERASE);
}

/* PP
 * page program */
char mx25l32_page_program(uint32_t addr, char *pdata, uint32_t length) {
	if (mx25l32_is_busy()) return -2;

	mx25l32_write_enable();

	select_device();
	spi_raw_send_byte(CMD_PP);
	spi_raw_send_byte((addr >> 16) & 0xFF);
	spi_raw_send_byte((addr >>  8) & 0xFF);
	spi_raw_send_byte((addr >>  0) & 0xFF);
	while(length--) {
		spi_raw_send_byte(*pdata++);
	}
	unselect_device();

	return mx25l32_wait_flash_ready(TIMEOUT_PAGE_PROGRAM);
}
