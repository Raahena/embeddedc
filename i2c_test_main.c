#include <stdint.h>

#include "gpio.h"
#include "timer.h"
#include "led.h"
#include "i2c.h"

int main(void)
{
    TIM2_Init();
    TIM2_Start();

    LED_Init();

    I2C1_Init();

    while(1)
    {
        /*
         * I2C has been initialized.
         *
         * Blink LED to indicate that the
         * program reached this point.
         */
        LED_Toggle();

        delay_ms(500);
    }
}