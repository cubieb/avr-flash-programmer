#ifndef MX25L32_H_
#define MX25L32_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define MX25L32_FLASH_SIZE	0x400000
#define MX25L32_BLOCK_SIZE	0x10000
#define MX25L32_SECTOR_SIZE	0x1000
#define MX25L32_PAGE_SIZE	0x100

char mx25l32_page_program(uint32_t addr, char *pdata, uint32_t length);
char mx25l32_sector_erase(uint32_t addr);
char mx25l32_block_erase(uint32_t addr);
char mx25l32_chip_erase();
void mx25l32_read_byte(uint32_t addr, char *pdata, uint32_t length);
uint8_t mx25l32_read_status();
uint32_t mx25l32_read_id(void);


#ifdef __cplusplus
}
#endif

#endif /* MX25L32_H_ */
