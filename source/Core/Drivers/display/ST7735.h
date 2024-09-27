/*
 * ST7735.h
 *
 *  Created on: 2024-09-26
 *      Author: Daniel Kurek
 *  
 *  Based on OLED library by Ben V. Brown <Ralim>
 *  Inspired by:
 *    - https://github.com/lvgl/lvgl/
 *    - https://github.com/adafruit/Adafruit-ST7735-Library/
 */

#ifndef ST7735_H_
#define ST7735_H_
#include "Font.h"
#include "cmsis_os.h"
#include "configuration.h"
#include <BSP.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

#include "SPI_Wrapper.hpp"

#define LCD_WIDTH  160
#define LCD_HEIGHT 80

#define RGB_TO_RGB565 (r, g, b) (((r)&0b11111000) << 8 | (((g)&0b11111100) << 3) | (b >> 3))
#define RGB_TO_BGR565 (r, g, b) (((b)&0b11111000) << 8 | (((g)&0b11111100) << 3) | (r >> 3))

#if defined(LCD_BGR565)
#define RGB_TO_LCD RGB_TO_BGR565
#else
#define RGB_TO_LCD RGB_TO_RGB565
#endif

typedef enum {
  FONT_SMALL  = 0,
  FONT_LARGE  = 1,
  FONT_EXTRAS = 2
} font_style_t;

typedef enum {
  ST7735_SWRESET = 0x01,
  ST7735_READ_ID = 0x04,
  ST7735_READ_STATUS = 0x09,
  ST7735_READ_DP_POWER = 0x0A,
  ST7735_READ_DP = 0x0B,
  ST7735_READ_DP_IMG = 0x0D,
  ST7735_SLP_IN = 0x10,
  ST7735_SLP_OUT = 0x11,
  ST7735_NORM_ON = 0x13,
  ST7735_INV_OFF = 0x20,
  ST7735_INV_ON = 0x21,
  ST7735_DISP_OFF = 0x28,
  ST7735_DISP_ON = 0x29,
  ST7735_COLUMN_ADDR_SET = 0x2A,
  ST7735_ROW_ADDR_SET = 0x2B,
  ST7735_MEM_WRITE = 0x2C,
  ST7735_MEM_READ = 0x2E,
  ST7735_PART_ADDR_SET = 0x30,
  ST7735_MAD_CTR = 0x36,
  ST7735_COLOR_MODE = 0x3A,
  ST7735_FRM_CTR1 = 0xB1,
  ST7735_FRM_CTR2 = 0xB2,
  ST7735_FRM_CTR3 = 0xB3,
  ST7735_INV_CTR  = 0xB4,
  ST7735_POW_CTR1 = 0xC0,
  ST7735_POW_CTR2 = 0xC1,
  ST7735_POW_CTR3 = 0xC2,
  ST7735_POW_CTR4 = 0xC3,
  ST7735_POW_CTR5 = 0xC4,
  ST7735_VCOM_CTR = 0xC5,
  ST7735_GAM_CTR_P1 = 0xE0,
  ST7735_GAM_CTR_N1 = 0xE1,
  ST7735_READ_ID1 = 0xDA,
  ST7735_READ_ID2 = 0xDB,
  ST7735_READ_ID3 = 0xDC,
} st7735_cmd_t;

void st7735_init(void);
bool st7735_init_done(void);
void st7735_onoff(void);
void st7735_set_rotation(bool leftHanded);
bool st7735_get_rotation(void);
void st7735_set_brightness(void); // TODO: add BSP function to set PWM
int16_t st7735_get_cursor_x(void);
int16_t st7735_get_cursor_y(void);
// Set the cursor location by pixels
void st7735_set_cursor(int16_t x, int16_t y);
void st7735_print(const char *str, font_style_t font_style, uint8_t length = 255, const uint8_t soft_x_limit = 0);
void st7735_print_whole_screen(const char *str);
// Print *F or *C - in font style of Small, Large (by default) or Extra based on input arg
void st7735_print_symbol_deg(font_style_t fontStyle = FONT_LARGE);
// Draws an image to the buffer, at x offset from top to bottom (fixed height renders)
void st7735_draw_image(const uint8_t *buffer, uint8_t x, uint8_t width);

// Draws a number at the current cursor location
void st7735_print_number(uint16_t number, uint8_t places, font_style_t font_style, bool no_leading_zeros = true);
// Clears the buffer
void st7735_clear_screen(void);
// Draws the battery level symbol
void st7735_draw_battery(uint8_t state);
// Draws a checkbox
void st7735_draw_checkbox(bool state);
void st7735_draw_unavailable_icon(void);
void st7735_debug_number(int32_t val, font_style_t font_style);
void st7735_draw_hex(uint32_t x, font_style_t font_style, uint8_t digits);
void st7735_draw_symbol(uint8_t symbolID); // Used for drawing symbols of a predictable width
void st7735_draw_area(int16_t x, int8_t y, uint8_t width, uint8_t height, const uint8_t *ptr);
void st7735_fill_area(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint16_t value);
void st7735_draw_filled_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool clear);
void st7735_draw_heat_symbol(uint8_t state);
void st7735_draw_scroll_indicator(uint8_t p, uint8_t h); // Draws a scrolling position indicator
void st7735_transition_scroll_down(const TickType_t viewEnterTime);
void st7735_transition_scroll_up(const TickType_t viewEnterTime);

#ifdef __cplusplus
}
#endif

#endif /* ST7735_HPP_ */
