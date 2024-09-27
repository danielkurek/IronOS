#include "ST7735.h"
#include "Font.h"
#include "Settings.h"
#include "SPI_Wrapper.h"

static bool         in_left_handed_mode; // Whether the screen is in left or not (used for offsets in GRAM)
static bool         init_done = false;
static bool         onoff;
static int16_t      cursor_x;
static int16_t      cursor_y;
static uint8_t      x_offset;
static uint8_t      y_offset;
static uint8_t      screenBuffer[16 + (LCD_WIDTH * (LCD_HEIGHT / 8)) + 10]; // The data buffer // TODO change size
static uint8_t      madctl_reg;

static void st7735_draw_char(uint16_t charCode, font_style_t  font_style, const uint8_t soft_x_limit); // Draw a character to the current cursor location

/* based on LVGL ST7735 driver */
// TODO: refactor values to defines
static const uint8_t init_cmd_list[] = {
    ST7735_FRM_CTR1, 3, 0x05, 0x3C, 0x3C,
    ST7735_FRM_CTR2, 3, 0x05, 0x3C, 0x3C,
    ST7735_FRM_CTR3, 6, 0x05, 0x3C, 0x3C, 0x05, 0x3C, 0x3C,
    ST7735_INV_CTR,  1, 0x03,
    ST7735_POW_CTR1, 3, 0x28, 0x08, 0x04,
    ST7735_POW_CTR2, 2, 0x0D, 0x00,
    ST7735_POW_CTR3, 2, 0x8D, 0x2A,
    ST7735_POW_CTR4, 2, 0x8D, 0xEE,
    ST7735_POW_CTR5, 1, 0x10,
    ST7735_GAM_CTR_P1, 16, 0x04, 0x22, 0x07, 0x0A, 0x2E, 0x30, 0x25, 0x2A, 0x28, 0x26, 0x2E, 0x3A, 0x00, 0x01, 0x03, 0x13,
    ST7735_GAM_CTR_N1, 16, 0x04, 0x16, 0x06, 0x0D, 0x2D, 0x26, 0x23, 0x27, 0x27, 0x25, 0x2D, 0x3B, 0x00, 0x01, 0x04, 0x13
};

void st7735_init(void){
    cursor_x = 0;
    cursor_y = 0;
    
    // enter sleep mode
    spi_write_cmd(ST7735_SLP_IN, NULL, 0);
    delay_ms(10);
    
    // perform software reset
    spi_write_cmd(ST7735_SWRESET, NULL, 0);
    delay_ms(200);

    // exit sleep mode
    spi_write_cmd(ST7735_SLP_OUT, NULL, 0);
    delay_ms(300);

    // enter normal mode
    spi_write_cmd(ST7735_NORM_ON, NULL, 0);

    spi_write_cmd(ST7735_MAD_CTR, (uint8_t[]) {madctl_reg}, 1);
    spi_write_cmd(ST7735_COLOR_MODE, (uint8_t[]) {colmod_reg}, 1);
    spi_write_cmd(ST7735_DISP_ON, NULL, 0);

    // init LCD panel
    const size_t cmd_list_len = sizeof(init_cmd_list) / sizeof(init_cmd_list[0]);
    size_t idx = 0;

    while(idx < cmd_list_len){
        size_t remaining_cmds = cmd_list_len - idx;
        if(remaining_cmds < 2){
            break; // malformed cmd list - cannot read command and arg_len
        }
        uint8_t cmd = init_cmd_list[idx];
        uint8_t arg_len = init_cmd_list[idx+1];
        if(remaining_cmds < 2 + arg_len){
            break; // missing args in array - missing args or wrong arg_len
        }
        spi_write_cmd(cmd, init_cmd_list+idx+2, arg_len);
        idx += 2 + arg_len;
    }

    init_done = true;
}

bool st7735_init_done(void){
    return init_done;
}

void st7735_onoff(void){
    // TODO: turn off backlight
}

void st7735_set_rotation(bool leftHanded){
    in_left_handed_mode = leftHanded;
    // TODO: set MX and MY bit (hardware X-Y mirror)
}

bool st7735_get_rotation(void){
    return in_left_handed_mode;
}

void st7735_set_brightness(void){
    // TODO
}

int16_t st7735_get_cursor_x(void){
    return cursor_x;
}

int16_t st7735_get_cursor_y(void){
    return cursor_y;
}

void st7735_set_cursor(int16_t x, int16_t y){
    cursor_x = x;
    cursor_y = y;
}

void st7735_print(const char *str, font_style_t font_style, uint8_t length, const uint8_t soft_x_limit){
    const uint8_t *next = (const uint8_t*) str;
    if (next[0] == 0x01) {
        font_style = FONT_LARGE;
        next++;
        length--;
    }
    while (next[0] && length > 0) {
        uint16_t index;
        if (next[0] <= 0xF0) {
            index = next[0];
            next++;
        } else {
            if (!next[1]) {
                return;
            }
            index = (next[0] - 0xF0) * 0xFF - 15 + next[1];
            next += 2;
        }
        st7735_draw_char(index, font_style, soft_x_limit);
        length--;
    }
}

void st7735_print_whole_screen(const char *str){
    st7735_set_cursor(0, 0);
    st7735_print(str, FONT_SMALL);
}

void st7735_print_symbol_deg(font_style_t font_style){
    switch (font_style) {
        case FONT_EXTRAS:
            // Picks *F or *C in ExtraFontChars[] from Font.h
            st7735_draw_symbol(getSettingValue(TemperatureInF) ? 0 : 1);
            break;
        case FONT_LARGE:
        case FONT_SMALL:
        default:
            st7735_print(getSettingValue(TemperatureInF) ? LargeSymbolDegF : LargeSymbolDegC, font_style);
            break;
    }
}

void st7735_draw_image(const uint8_t *buffer, uint8_t x, uint8_t width){
    // TODO
}

static inline void strip_leading_zeros(char *buffer, uint8_t places) {
  // Removing the leading zero's by swapping them to SymbolSpace
  // Stop 1 short so that we dont blank entire number if its zero
  for (int i = 0; i < (places - 1); i++) {
    if (buffer[i] == 2) {
      buffer[i] = LargeSymbolSpace[0];
    } else {
      return;
    }
  }
}


void st7735_print_number(uint16_t number, uint8_t places, font_style_t font_style, bool no_leading_zeros){
    char buffer[7] = {0};

    if (places >= 5) {
        buffer[5] = 2 + number % 10;
        number /= 10;
    }
    if (places > 4) {
        buffer[4] = 2 + number % 10;
        number /= 10;
    }

    if (places > 3) {
        buffer[3] = 2 + number % 10;
        number /= 10;
    }

    if (places > 2) {
        buffer[2] = 2 + number % 10;
        number /= 10;
    }

    if (places > 1) {
        buffer[1] = 2 + number % 10;
        number /= 10;
    }

    buffer[0] = 2 + number % 10;
    if (no_leading_zeros) {
        strip_leading_zeros(buffer, places);
    }
    print(buffer, font_style);
}

void st7735_draw_hex(uint32_t x, font_style_t font_style, uint8_t digits){
    // print number to hex
    for (uint_fast8_t i = 0; i < digits; i++) {
        uint16_t value = (x >> (4 * (7 - i))) & 0b1111;
        drawChar(value + 2, font_style, 0);
    }
}

void st7735_clear_screen(void){
    // TODO
}

void st7735_draw_battery(uint8_t state){
    st7735_draw_symbol(3 + (state > 10 ? 10 : state));
}

void st7735_draw_checkbox(bool state){
    st7735_draw_symbol((state) ? 16 : 17);
}

void st7735_draw_unavailable_icon(void){
    st7735_draw_area(LCD_WIDTH - LCD_HEIGHT - 2, 0, LCD_HEIGHT, LCD_HEIGHT, UnavailableIcon);
}

void st7735_debug_number(int32_t val, font_style_t font_style){
    // TODO
}

void st7735_draw_symbol(uint8_t symbolID){
    // TODO
} 

void st7735_draw_area(int16_t x, int8_t y, uint8_t width, uint8_t height, const uint8_t *ptr){
    // TODO
}

void st7735_fill_area(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint16_t value){
    if(x > LCD_WIDTH || y > LCD_HEIGHT){
        return; // out of bounds
    }
    // clip area to LCD dimensions
    if((uint16_t) x + width > LCD_WIDTH){
        width = LCD_WIDTH - x;
    }
    if((uint16_t) y + height > LCD_HEIGHT){
        height = LCD_HEIGHT - x;
    }
    size_t buf_len = ((size_t) width) * height
    uint16_t data[buf_len];
    for(size_t i = 0; i < buf_len; i++){
        data[i] = value;
    }
    spi_write_cmd(ST7735_COLUMN_ADDR_SET, (uint8_t []){0, x, 0, x+width}, 4);
    spi_write_cmd(ST7735_ROW_ADDR_SET, (uint8_t []){0, y, 0, y+height}, 4);
    spi_write_cmd(ST7735_MEM_WRITE, (uint8_t *) data, buf_len*2);
}

void st7735_draw_filled_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool clear){
    // TODO
}

void st7735_draw_heat_symbol(uint8_t state){
    // TODO
}

void st7735_draw_scroll_indicator(uint8_t p, uint8_t h){
    // TODO
}

void st7735_transition_scroll_down(const TickType_t viewEnterTime){
    // TODO
}

void st7735_transition_scroll_up(const TickType_t viewEnterTime){
    // TODO
}

void st7735_draw_char(uint16_t charCode, font_style_t font_style, const uint8_t soft_x_limit){
    // TODO
}