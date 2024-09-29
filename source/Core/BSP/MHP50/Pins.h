/*
 * Pins.h
 *
 *  Created on: 20 Sep 2024
 *      Author: Daniel Kurek
 */

#ifndef BSP_MINIWARE_PINS_H_
#define BSP_MINIWARE_PINS_H_
#include "configuration.h"

// MHP50 pin map
#define KEY_A_Pin                     GPIO_Pin_8
#define KEY_A_GPIO_Port               GPIOC
#define KEY_B_Pin                     GPIO_Pin_9
#define KEY_B_GPIO_Port               GPIOC
// #define TMP36_INPUT_Pin               GPIO_PIN_1
// #define TMP36_INPUT_GPIO_Port         GPIOB
// #define TMP36_ADC1_CHANNEL            ADC_CHANNEL_9
// #define TMP36_ADC2_CHANNEL            ADC_CHANNEL_9
// #define TIP_TEMP_Pin                  GPIO_PIN_2
// #define TIP_TEMP_GPIO_Port            GPIOA
// #define TIP_TEMP_ADC1_CHANNEL         ADC_CHANNEL_2
// #define TIP_TEMP_ADC2_CHANNEL         ADC_CHANNEL_2
// #define VIN_Pin                       GPIO_PIN_1
// #define VIN_GPIO_Port                 GPIOA
// #define VIN_ADC1_CHANNEL              ADC_CHANNEL_1
// #define VIN_ADC2_CHANNEL              ADC_CHANNEL_1
#define SPI_MISO_PIN                  GPIO_Pin_6
#define SPI_MISO_PORT                 GPIOA
#define SPI_MOSI_PIN                  GPIO_Pin_7
#define SPI_MOSI_PORT                 GPIOA
#define SPI_SCLK_PIN                  GPIO_Pin_5
#define SPI_SCLK_PORT                 GPIOA
#define FLASH_SPI_CS_PIN              GPIO_Pin_14
#define FLASH_SPI_CS_PORT             GPIOB
#define LCD_SPI_CS_PIN                GPIO_Pin_4
#define LCD_SPI_CS_PORT               GPIOA
#define LCD_SPI_DC_PIN                GPIO_Pin_5
#define LCD_SPI_DC_PORT               GPIOD
#define LCD_SPI_RES_PIN               GPIO_Pin_6
#define LCD_SPI_RES_PORT              GPIOD
// #define OLED_RESET_Pin                GPIO_PIN_4
// #define OLED_RESET_GPIO_Port          GPIOB
#define PWM_Out_Pin                   GPIO_Pin_1
#define PWM_Out_GPIO_Port             GPIOB
#define PWM_Out_CHANNEL               TIM_CHANNEL_4
#define BUZZER_Pin                    GPIO_Pin_8
#define BUZZER_GPIO_Port              GPIOB
#define BUZZER_CHANNEL                TIM_CHANNEL_3
// #define SCL_Pin                       GPIO_PIN_6
// #define SCL_GPIO_Port                 GPIOB
// #define SDA_Pin                       GPIO_PIN_7
// #define SDA_GPIO_Port                 GPIOB
// #define SCL2_Pin                      GPIO_PIN_3
// #define SCL2_GPIO_Port                GPIOB
// #define SDA2_Pin                      GPIO_PIN_15
// #define SDA2_GPIO_Port                GPIOA
// #define INT_PD_Pin                    GPIO_PIN_5
// #define INT_PD_GPIO_Port              GPIOB
#define HEAT_EN_Pin                   GPIO_Pin_13
#define HEAT_EN_GPIO_Port             GPIOB
// #define PLATE_SENSOR_PULLUP_Pin       GPIO_PIN_1
// #define PLATE_SENSOR_PULLUP_GPIO_Port GPIOD

// #define PLATE_SENSOR_Pin          GPIO_PIN_5
// #define PLATE_SENSOR_GPIO_Port    GPIOA
// #define PLATE_SENSOR_ADC1_CHANNEL ADC_CHANNEL_5
// #define PLATE_SENSOR_ADC2_CHANNEL ADC_CHANNEL_5

#define WS2812_Pin       GPIO_Pin_15 // TODO: check if it is WS2812 compatible LED
#define WS2812_GPIO_Port GPIOB
#endif /* BSP_MINIWARE_PINS_H_ */
