#include <stdint.h>

#include "gpio.h"
#include "timer.h"
#include "led.h"
#include "i2c.h"
#include "oled.h"


int main(void)
{
    /*
     * Initialize timer
     */
    TIM2_Init();
    TIM2_Start();

    /*
     * Initialize LED
     */
    LED_Init();

    /*
     * Initialize I2C
     */
    I2C1_Init();

    /*
     * Initialize OLED
     */
    OLED_Init();

    /*
     * Display message
     */
    OLED_Clear();

    OLED_SetCursor(0U, 0U);
    OLED_WriteString("HAZARDOUS ZONE");

    OLED_SetCursor(0U, 2U);
    OLED_WriteString("SYSTEM READY");

    OLED_SetCursor(0U, 4U);
    OLED_WriteString("ACCESS CONTROL");

    OLED_SetCursor(0U, 6U);
    OLED_WriteString("STM32F446RE");

    /*
     * Send framebuffer to OLED
     */
    OLED_UpdateScreen();


    while(1)
    {
        /*
         * Keep LED blinking to show that
         * MCU is running.
         */
        LED_Toggle();

        delay_ms(500U);
    }
}