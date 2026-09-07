#include "oled.h"
#include "i2c.h"
#include "timer.h"


/*
 * Frame buffer
 *
 * 128 x 64 pixels
 *
 * 128 * 64 / 8 = 1024 bytes
 */
static uint8_t OLED_Buffer[OLED_WIDTH * OLED_HEIGHT / 8U];


/*
 * Current cursor position
 *
 * x = column
 *
 * y = page
 *
 * One page = 8 vertical pixels
 */
static uint8_t OLED_CurrentX = 0U;
static uint8_t OLED_CurrentY = 0U;


/*
 * 5x7 font
 *
 * Characters supported:
 *
 * Space
 * A-Z
 * 0-9
 * :
 * -
 * *
 * .
 * /
 */
static const uint8_t Font5x7[][5] =
{
    /* Space */
    {0x00,0x00,0x00,0x00,0x00},

    /* A */
    {0x7E,0x11,0x11,0x11,0x7E},

    /* B */
    {0x7F,0x49,0x49,0x49,0x36},

    /* C */
    {0x3E,0x41,0x41,0x41,0x22},

    /* D */
    {0x7F,0x41,0x41,0x22,0x1C},

    /* E */
    {0x7F,0x49,0x49,0x49,0x41},

    /* F */
    {0x7F,0x09,0x09,0x09,0x01},

    /* G */
    {0x3E,0x41,0x49,0x49,0x7A},

    /* H */
    {0x7F,0x08,0x08,0x08,0x7F},

    /* I */
    {0x00,0x41,0x7F,0x41,0x00},

    /* J */
    {0x20,0x40,0x41,0x3F,0x01},

    /* K */
    {0x7F,0x08,0x14,0x22,0x41},

    /* L */
    {0x7F,0x40,0x40,0x40,0x40},

    /* M */
    {0x7F,0x02,0x0C,0x02,0x7F},

    /* N */
    {0x7F,0x04,0x08,0x10,0x7F},

    /* O */
    {0x3E,0x41,0x41,0x41,0x3E},

    /* P */
    {0x7F,0x09,0x09,0x09,0x06},

    /* Q */
    {0x3E,0x41,0x51,0x21,0x5E},

    /* R */
    {0x7F,0x09,0x19,0x29,0x46},

    /* S */
    {0x46,0x49,0x49,0x49,0x31},

    /* T */
    {0x01,0x01,0x7F,0x01,0x01},

    /* U */
    {0x3F,0x40,0x40,0x40,0x3F},

    /* V */
    {0x1F,0x20,0x40,0x20,0x1F},

    /* W */
    {0x3F,0x40,0x38,0x40,0x3F},

    /* X */
    {0x63,0x14,0x08,0x14,0x63},

    /* Y */
    {0x07,0x08,0x70,0x08,0x07},

    /* Z */
    {0x61,0x51,0x49,0x45,0x43},

    /* 0 */
    {0x3E,0x51,0x49,0x45,0x3E},

    /* 1 */
    {0x00,0x42,0x7F,0x40,0x00},

    /* 2 */
    {0x42,0x61,0x51,0x49,0x46},

    /* 3 */
    {0x21,0x41,0x45,0x4B,0x31},

    /* 4 */
    {0x18,0x14,0x12,0x7F,0x10},

    /* 5 */
    {0x27,0x45,0x45,0x45,0x39},

    /* 6 */
    {0x3C,0x4A,0x49,0x49,0x30},

    /* 7 */
    {0x01,0x71,0x09,0x05,0x03},

    /* 8 */
    {0x36,0x49,0x49,0x49,0x36},

    /* 9 */
    {0x06,0x49,0x49,0x29,0x1E},

    /* : */
    {0x00,0x36,0x36,0x00,0x00},

    /* - */
    {0x08,0x08,0x08,0x08,0x08},

    /* * */
    {0x14,0x08,0x3E,0x08,0x14},

    /* . */
    {0x00,0x60,0x60,0x00,0x00},

    /* / */
    {0x20,0x10,0x08,0x04,0x02}
};


/*
 * Send one command to SSD1306
 */
static void OLED_WriteCommand(uint8_t command)
{
    uint8_t data[2];

    /*
     * 0x00 = command control byte
     */
    data[0] = 0x00U;
    data[1] = command;

    I2C1_MasterWrite(OLED_I2C_ADDRESS,
                     data,
                     2U);
}


/*
 * Send multiple commands
 */
static void OLED_WriteCommands(const uint8_t *commands,
                               uint32_t length)
{
    uint8_t buffer[16];
    uint32_t i;

    /*
     * Each command needs 0x00 control byte.
     *
     * This function is therefore limited to
     * 7 commands at a time.
     */
    if(length > 7U)
    {
        length = 7U;
    }

    buffer[0] = 0x00U;

    for(i = 0U; i < length; i++)
    {
        buffer[i + 1U] = commands[i];
    }

    I2C1_MasterWrite(OLED_I2C_ADDRESS,
                     buffer,
                     length + 1U);
}


/*
 * Initialize SSD1306
 */
void OLED_Init(void)
{
    /*
     * Give OLED time after power-up.
     */
    delay_ms(100U);

    /*
     * Display OFF
     */
    OLED_WriteCommand(OLED_DISPLAY_OFF);

    /*
     * Set display clock divide ratio
     */
    OLED_WriteCommand(OLED_SET_DISPLAY_CLOCK);
    OLED_WriteCommand(0x80U);

    /*
     * Set multiplex ratio
     *
     * 0x3F = 63
     *
     * For 64 rows.
     */
    OLED_WriteCommand(OLED_SET_MULTIPLEX);
    OLED_WriteCommand(0x3FU);

    /*
     * Set display offset
     */
    OLED_WriteCommand(OLED_SET_DISPLAY_OFFSET);
    OLED_WriteCommand(0x00U);

    /*
     * Set display start line
     */
    OLED_WriteCommand(OLED_SET_START_LINE);

    /*
     * Enable charge pump
     */
    OLED_WriteCommand(OLED_CHARGE_PUMP);
    OLED_WriteCommand(0x14U);

    /*
     * Memory addressing mode
     *
     * 0x00 = horizontal addressing mode
     */
    OLED_WriteCommand(OLED_MEMORY_MODE);
    OLED_WriteCommand(0x00U);

    /*
     * Segment remap
     */
    OLED_WriteCommand(OLED_SEGMENT_REMAP);

    /*
     * COM output scan direction
     */
    OLED_WriteCommand(OLED_COM_SCAN_DEC);

    /*
     * COM pins hardware configuration
     *
     * For 128x64 OLED:
     *
     * 0x12
     */
    OLED_WriteCommand(OLED_SET_COM_PINS);
    OLED_WriteCommand(0x12U);

    /*
     * Contrast
     */
    OLED_WriteCommand(OLED_SET_CONTRAST);
    OLED_WriteCommand(0x7FU);

    /*
     * Pre-charge period
     */
    OLED_WriteCommand(OLED_SET_PRECHARGE);
    OLED_WriteCommand(0xF1U);

    /*
     * VCOMH deselect level
     */
    OLED_WriteCommand(OLED_SET_VCOM_DETECT);
    OLED_WriteCommand(0x40U);

    /*
     * Normal display
     */
    OLED_WriteCommand(OLED_NORMAL_DISPLAY);

    /*
     * Display ON
     */
    OLED_WriteCommand(OLED_DISPLAY_ON);

    /*
     * Clear framebuffer
     */
    OLED_Clear();

    /*
     * Send framebuffer to OLED
     */
    OLED_UpdateScreen();
}


/*
 * Clear framebuffer
 */
void OLED_Clear(void)
{
    uint32_t i;

    for(i = 0U;
        i < sizeof(OLED_Buffer);
        i++)
    {
        OLED_Buffer[i] = 0x00U;
    }

    OLED_CurrentX = 0U;
    OLED_CurrentY = 0U;
}


/*
 * Update complete display
 */
void OLED_UpdateScreen(void)
{
    uint8_t page;
    uint8_t data[129];
    uint16_t i;

    /*
     * Each page contains 128 bytes.
     *
     * 8 pages for 64-pixel display.
     */
    for(page = 0U;
        page < OLED_PAGES;
        page++)
    {
        /*
         * Set page address.
         */
        OLED_WriteCommand(OLED_PAGE_ADDR);
        OLED_WriteCommand(page);
        OLED_WriteCommand(0x07U);

        /*
         * Set column address.
         */
        OLED_WriteCommand(OLED_COLUMN_ADDR);
        OLED_WriteCommand(0x00U);
        OLED_WriteCommand(0x7FU);

        /*
         * 0x40 = display data control byte.
         */
        data[0] = 0x40U;

        /*
         * Copy page data.
         */
        for(i = 0U; i < OLED_WIDTH; i++)
        {
            data[i + 1U] =
                OLED_Buffer[(uint16_t)page * OLED_WIDTH + i];
        }

        /*
         * Send 128 display bytes.
         */
        I2C1_MasterWrite(OLED_I2C_ADDRESS,
                         data,
                         129U);
    }
}


/*
 * Set text cursor.
 *
 * x = character column
 *
 * y = page
 */
void OLED_SetCursor(uint8_t x, uint8_t y)
{
    OLED_CurrentX = x;
    OLED_CurrentY = y;
}


/*
 * Convert character into font index.
 */
static uint8_t OLED_GetFontIndex(char c)
{
    if(c == ' ')
    {
        return 0U;
    }

    /*
     * A-Z
     */
    if(c >= 'A' && c <= 'Z')
    {
        return (uint8_t)(1U + (c - 'A'));
    }

    /*
     * 0-9
     */
    if(c >= '0' && c <= '9')
    {
        return (uint8_t)(27U + (c - '0'));
    }

    /*
     * :
     */
    if(c == ':')
    {
        return 37U;
    }

    /*
     * -
     */
    if(c == '-')
    {
        return 38U;
    }

    /*
     * *
     */
    if(c == '*')
    {
        return 39U;
    }

    /*
     * .
     */
    if(c == '.')
    {
        return 40U;
    }

    /*
     * /
     */
    if(c == '/')
    {
        return 41U;
    }

    /*
     * Unsupported character
     */
    return 0U;
}


/*
 * Write one character.
 */
void OLED_WriteChar(char c)
{
    uint8_t index;
    uint8_t i;

    /*
     * Move to next line if necessary.
     */
    if(OLED_CurrentX >= 21U)
    {
        OLED_CurrentX = 0U;
        OLED_CurrentY++;
    }

    /*
     * Only 8 pages available.
     */
    if(OLED_CurrentY >= OLED_PAGES)
    {
        OLED_CurrentY = 0U;
    }

    index = OLED_GetFontIndex(c);

    /*
     * Each character:
     *
     * 5 columns + 1 blank column
     */
    for(i = 0U; i < 5U; i++)
    {
        OLED_Buffer[(uint16_t)OLED_CurrentY * OLED_WIDTH
                    + ((uint16_t)OLED_CurrentX * 6U)
                    + i] =
                    Font5x7[index][i];
    }

    /*
     * Blank spacing column.
     */
    OLED_Buffer[(uint16_t)OLED_CurrentY * OLED_WIDTH
                + ((uint16_t)OLED_CurrentX * 6U)
                + 5U] = 0x00U;

    OLED_CurrentX++;
}


/*
 * Write string.
 */
void OLED_WriteString(const char *str)
{
    if(str == 0)
    {
        return;
    }

    while(*str != '\0')
    {
        OLED_WriteChar(*str);
        str++;
    }
}


/*
 * Write unsigned integer.
 */
void OLED_WriteNumber(uint32_t number)
{
    char buffer[11];
    uint8_t i = 0U;
    uint8_t j;

    /*
     * Special case for zero.
     */
    if(number == 0U)
    {
        OLED_WriteChar('0');
        return;
    }

    /*
     * Convert number to characters in reverse.
     */
    while(number > 0U && i < sizeof(buffer))
    {
        buffer[i++] = (char)('0' + (number % 10U));
        number /= 10U;
    }

    /*
     * Write in correct order.
     */
    for(j = i; j > 0U; j--)
    {
        OLED_WriteChar(buffer[j - 1U]);
    }
}


/*
 * Draw one pixel.
 */
void OLED_DrawPixel(uint8_t x,
                    uint8_t y,
                    uint8_t color)
{
    uint16_t index;
    uint8_t bit;

    if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }

    index = ((uint16_t)(y / 8U) * OLED_WIDTH) + x;
    bit = (uint8_t)(y % 8U);

    if(color)
    {
        OLED_Buffer[index] |= (1U << bit);
    }
    else
    {
        OLED_Buffer[index] &= ~(1U << bit);
    }
}