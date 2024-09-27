#include "ST7735.h"
#include "Font.h"

static bool         in_left_handed_mode; // Whether the screen is in left or not (used for offsets in GRAM)
static bool         init_done = false;
static bool         onoff;
static int16_t      cursor_x;
static int16_t      cursor_y;
static uint8_t      display_offset;
static uint8_t      screenBuffer[16 + (LCD_WIDTH * (LCD_HEIGHT / 8)) + 10]; // The data buffer // TODO change size

static void st7735_draw_char(uint16_t charCode, font_style_t  font_style, const uint8_t soft_x_limit); // Draw a character to the current cursor location


void st7735_init(void){
    cursor_x = 0;
    cursor_y = 0;
    // TODO init driver
    init_done = true;
}

void st7735_init_done(void){
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

void st7735_print(const char *string, font_style_t font_style, uint8_t lenght, const uint8_t soft_x_limit){
    // TODO
}

void st7735_print_whole_screen(const char *string){
    // TODO
}

void st7735_print_symbol_deg(font_style_t fontStyle){
    // TODO
}

void st7735_draw_image(const uint8_t *buffer, uint8_t x, uint8_t width){
    // TODO
}


void st7735_print_number(uint16_t number, uint8_t places, font_style_t fontStyle, bool noLeaderZeros){
    // TODO
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

void st7735_debug_number(int32_t val, FontStyle fontStyle){
    // TODO
}

void st7735_draw_hex(uint32_t x, FontStyle fontStyle, uint8_t digits){
    // TODO
}

void st7735_draw_symbol(uint8_t symbolID){
    // TODO
} 

void st7735_draw_area(int16_t x, int8_t y, uint8_t wide, uint8_t height, const uint8_t *ptr){
    // TODO
}

void st7735_fill_area(int16_t x, int8_t y, uint8_t wide, uint8_t height, const uint8_t value){
    // TODO
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

void st7735_draw_char(uint16_t charCode, FontStyle fontStyle, const uint8_t soft_x_limit){
    // TODO
}