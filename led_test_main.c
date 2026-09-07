#include <stdint.h>

#include "gpio.h"
#include "timer.h"
#include "led.h"

int main(void)
{
    /* Initialize timer */
    TIM2_Init();
    TIM2_Start();

    /* Initialize LED */
    LED_Init();

    while(1)
    {
        LED_On();
        delay_ms(500);

        LED_Off();
        delay_ms(500);
    }
}