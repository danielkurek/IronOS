#include "SPI_Wrapper.h"
#include <inttypes.h>
#include "Pins.h"
#include "ch32f20x_gpio.h"
#include "ch32f20x_spi.h"

// TODO: add FreeRTOS semaphore

void spi_init(void){
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure  = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = SPI_SCLK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_SCLK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_SPI_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_SPI_DC_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_SPI_DC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_SPI_RES_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_SPI_RES_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = FLASH_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
#if SPI_SPEED_MODE == SPI_FAST_MODE
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#else
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
#endif

    SPI_InitStructure.SPI_FirstBit      = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);

#if SPI_SPEED_MODE == SPI_FAST_MODE
    SPI1->HSCR = 1;
#endif
    GPIO_WriteBit(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN, 1);
    GPIO_WriteBit(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, 1);
}

static void spi_write_blocking(SPI_TypeDef *SPIx, const uint8_t data){
    while(SPI_I2S_GetFlagStatus( SPIx, SPI_I2S_FLAG_TXE ) == RESET);
    SPI_I2S_SendData(SPIx, data);
}

void spi_write_cmd(uint8_t cmd, const uint8_t *args, size_t args_len){
    // select LCD
    GPIO_WriteBit(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN, 0);
    // set CMD mode
    GPIO_WriteBit(LCD_SPI_DC_PORT, LCD_SPI_DC_PIN, 1);
    spi_write_blocking(SPI1, cmd);
    // set DATA mode
    GPIO_WriteBit(LCD_SPI_DC_PORT, LCD_SPI_DC_PIN, 0);
    for(size_t i = 0; i < args_len; i++){
        spi_write_blocking(SPI1, args[i]);
    }
    // deselect LCD
    GPIO_WriteBit(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN, 1);
}