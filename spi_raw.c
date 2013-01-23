
/*
 * Copyright (c) 2006-2010 by Roland Riegel <feedback@roland-riegel.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <avr/io.h>
#include <stdint.h>
#include "spi_raw_config.h"
#include "spi_raw.h"


/**
 * \ingroup spi_raw
 * Initializes memory card communication.
 *
 * \returns 0 on failure, 1 on success.
 */
void spi_raw_init()
{
    /* enable outputs for MOSI, SCK, SS, input for MISO */
    configure_pin_mosi();
    configure_pin_sck();
    configure_pin_ss();
    configure_pin_miso();

    unselect_device();

    /* initialize SPI with lowest frequency; max. 400kHz during identification mode of card */
    SPCR = (0 << SPIE) | /* SPI Interrupt Enable */
           (1 << SPE)  | /* SPI Enable */
           (0 << DORD) | /* Data Order: MSB first */
           (1 << MSTR) | /* Master mode */
           (0 << CPOL) | /* Clock Polarity: SCK low when idle */
           (0 << CPHA) | /* Clock Phase: sample on rising SCK edge */
           (1 << SPR1) | /* Clock Frequency: f_OSC / 128 */
           (1 << SPR0);
    SPSR &= ~(1 << SPI2X); /* No doubled clock frequency */

    select_device();
}

/**
 * \ingroup spi_raw
 * Sends a raw byte to the memory card.
 *
 * \param[in] b The byte to sent.
 * \see spi_raw_rec_byte
 */
void spi_raw_send_byte(uint8_t b)
{
    SPDR = b;
    /* wait for byte to be shifted out */
    while(!(SPSR & (1 << SPIF)));
    SPSR &= ~(1 << SPIF);
}

/**
 * \ingroup spi_raw
 * Receives a raw byte from the memory card.
 *
 * \returns The byte which should be read.
 * \see spi_raw_send_byte
 */
uint8_t spi_raw_rec_byte()
{
    /* send dummy data for receiving some */
    SPDR = 0x00;
    while(!(SPSR & (1 << SPIF)));
    SPSR &= ~(1 << SPIF);

    return SPDR;
}
