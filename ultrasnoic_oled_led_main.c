#include "timer.h"
#include "led.h"
#include "i2c.h"
#include "oled.h"
#include "ultrasonic.h"

int main(void)
{
    uint32_t distance;

    /* Initialize peripherals */
    TIM2_Init();
    LED_Init();
    I2C1_Init();
    OLED_Init();
    Ultrasonic_Init();

    while (1)
    {
        /* Read ultrasonic distance */
        distance = Ultrasonic_GetDistanceCm();

        /* Clear OLED */
        OLED_Clear();

        /* Display title */
        OLED_SetCursor(0, 0);
        OLED_WriteString("ULTRASONIC");

        OLED_SetCursor(0, 2);
        OLED_WriteString("DISTANCE:");

        if (distance == 0)
        {
            /* Sensor timeout/error */
            OLED_SetCursor(0, 4);
            OLED_WriteString("SENSOR ERROR");

            LED_Off();
        }
        else
        {
            /* Display distance */
            OLED_SetCursor(0, 4);
            OLED_WriteNumber(distance);
            OLED_WriteString(" CM");

            LED_On();
        }

        /* Send framebuffer to OLED */
        OLED_UpdateScreen();

        /* Read again after 200 ms */
        delay_ms(200);
    }
}