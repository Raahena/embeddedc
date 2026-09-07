#ifndef OLED_H
#define OLED_H

#include <stdint.h>

/* OLED configuration */
#define OLED_WIDTH          128U
#define OLED_HEIGHT         64U

#define OLED_I2C_ADDRESS    0x3CU

#define OLED_PAGES          (OLED_HEIGHT / 8U)

/* OLED commands */
#define OLED_DISPLAY_OFF            0xAEU
#define OLED_DISPLAY_ON             0xAFU

#define OLED_SET_DISPLAY_CLOCK      0xD5U
#define OLED_SET_MULTIPLEX          0xA8U
#define OLED_SET_DISPLAY_OFFSET     0xD3U
#define OLED_SET_START_LINE         0x40U

#define OLED_CHARGE_PUMP            0x8DU

#define OLED_MEMORY_MODE            0x20U
#define OLED_SEGMENT_REMAP          0xA1U
#define OLED_COM_SCAN_DEC           0xC8U

#define OLED_SET_COM_PINS           0xDAU
#define OLED_SET_CONTRAST           0x81U

#define OLED_SET_PRECHARGE          0xD9U
#define OLED_SET_VCOM_DETECT        0xDBU

#define OLED_NORMAL_DISPLAY         0xA6U

#define OLED_COLUMN_ADDR            0x21U
#define OLED_PAGE_ADDR              0x22U


void OLED_Init(void);

void OLED_Clear(void);

void OLED_UpdateScreen(void);

void OLED_SetCursor(uint8_t x, uint8_t y);

void OLED_WriteChar(char c);

void OLED_WriteString(const char *str);

void OLED_WriteNumber(uint32_t number);

void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color);

#endif /* OLED_H */