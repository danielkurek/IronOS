/*
 * SPI_Wrapper.h
 *
 *  Created on: 2024-09-27
 *      Author: Daniel Kurek
 *  
 */

#ifndef SPI_WRAPPER_H_
#define SPI_WRAPPER_H_

#include <inttypes.h>

void spi_init(void);

void spi_write_cmd(uint8_t cmd, const uint8_t *args, size_t args_len);
// void spi_read_data();

#endif // SPI_WRAPPER_H_