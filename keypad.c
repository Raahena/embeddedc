#include "keypad.h"
#include "gpio.h"
#include "timer.h"


/*
 * Keypad layout
 *
 *       C0  C1  C2  C3
 *
 * R0    1   2   3   A
 * R1    4   5   6   B
 * R2    7   8   9   C
 * R3    *   0   #   D
 */
static const char keypad_map[KEYPAD_ROWS][KEYPAD_COLS] =
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


/*
 * Row pins
 */
static const uint8_t row_pins[KEYPAD_ROWS] =
{
    KEYPAD_R0,
    KEYPAD_R1,
    KEYPAD_R2,
    KEYPAD_R3
};


/*
 * Column pins
 */
static const uint8_t col_pins[KEYPAD_COLS] =
{
    KEYPAD_C0,
    KEYPAD_C1,
    KEYPAD_C2,
    KEYPAD_C3
};


void Keypad_Init(void)
{
    uint8_t i;

    /*
     * Enable GPIOC clock
     */
    GPIO_PeripClockControl(KEYPAD_PORT, GPIO_ENABLE);


    /*
     * Configure rows as outputs
     */
    for(i = 0U; i < KEYPAD_ROWS; i++)
    {
        GPIO_SetPinMode(KEYPAD_PORT,
                        row_pins[i],
                        GPIO_OUTPUT);

        GPIO_SetPinOutputType(KEYPAD_PORT,
                              row_pins[i],
                              GPIO_PUSH_PULL);

        GPIO_SetPinPull(KEYPAD_PORT,
                        row_pins[i],
                        GPIO_NO_PUPD);

        /*
         * Rows are normally HIGH
         */
        GPIO_WritePin(KEYPAD_PORT,
                      row_pins[i],
                      GPIO_SET);
    }


    /*
     * Configure columns as inputs with pull-up
     */
    for(i = 0U; i < KEYPAD_COLS; i++)
    {
        GPIO_SetPinMode(KEYPAD_PORT,
                        col_pins[i],
                        GPIO_INPUT);

        GPIO_SetPinPull(KEYPAD_PORT,
                        col_pins[i],
                        GPIO_PULLUP);
    }
}


char Keypad_GetKey(void)
{
    uint8_t row;
    uint8_t col;

    /*
     * Scan every row
     */
    for(row = 0U; row < KEYPAD_ROWS; row++)
    {
        /*
         * Set all rows HIGH
         */
        GPIO_WritePin(KEYPAD_PORT,
                      KEYPAD_R0,
                      GPIO_SET);

        GPIO_WritePin(KEYPAD_PORT,
                      KEYPAD_R1,
                      GPIO_SET);

        GPIO_WritePin(KEYPAD_PORT,
                      KEYPAD_R2,
                      GPIO_SET);

        GPIO_WritePin(KEYPAD_PORT,
                      KEYPAD_R3,
                      GPIO_SET);


        /*
         * Make current row LOW
         */
        GPIO_WritePin(KEYPAD_PORT,
                      row_pins[row],
                      GPIO_RESET);


        /*
         * Small settling time
         */
        delay_us(50U);


        /*
         * Check each column
         *
         * Because of pull-up:
         *
         * No key pressed -> HIGH
         * Key pressed     -> LOW
         */
        for(col = 0U; col < KEYPAD_COLS; col++)
        {
            if(GPIO_ReadFromInputPin(KEYPAD_PORT,
                                     col_pins[col]) == GPIO_RESET)
            {
                /*
                 * Debounce
                 */
                delay_ms(20U);


                /*
                 * Check again
                 */
                if(GPIO_ReadFromInputPin(KEYPAD_PORT,
                                         col_pins[col]) == GPIO_RESET)
                {
                    /*
                     * Wait until key is released
                     *
                     * This prevents one long press
                     * from generating multiple keys.
                     */
                    while(GPIO_ReadFromInputPin(KEYPAD_PORT,
                                                col_pins[col]) == GPIO_RESET)
                    {
                        delay_ms(10U);
                    }


                    /*
                     * Restore all rows HIGH
                     */
                    GPIO_WritePin(KEYPAD_PORT,
                                  KEYPAD_R0,
                                  GPIO_SET);

                    GPIO_WritePin(KEYPAD_PORT,
                                  KEYPAD_R1,
                                  GPIO_SET);

                    GPIO_WritePin(KEYPAD_PORT,
                                  KEYPAD_R2,
                                  GPIO_SET);

                    GPIO_WritePin(KEYPAD_PORT,
                                  KEYPAD_R3,
                                  GPIO_SET);


                    /*
                     * Return detected key
                     */
                    return keypad_map[row][col];
                }
            }
        }
    }

    /*
     * No key pressed
     */
    return '\0';
}