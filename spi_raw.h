
/*
 * Copyright (c) 2006-2010 by Roland Riegel <feedback@roland-riegel.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef SPI_RAW_H
#define SPI_RAW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


void spi_raw_init();
void spi_raw_send_byte(uint8_t b);
uint8_t spi_raw_rec_byte();


#ifdef __cplusplus
}
#endif

#endif

