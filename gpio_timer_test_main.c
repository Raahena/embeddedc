#include <stdint.h>

#include "gpio.h"
#include "timer.h"

int main(void)
{
    /* Enable GPIOA clock */
    GPIO_PeripClockControl(GPIOA, GPIO_ENABLE);

    /* PA5 as output */
    GPIO_SetPinMode(GPIOA, 5, GPIO_OUTPUT);

    /* Push-pull */
    GPIO_SetPinOutputType(GPIOA, 5, GPIO_PUSH_PULL);

    /* No pull-up/pull-down */
    GPIO_SetPinPull(GPIOA, 5, GPIO_NO_PUPD);

    /* Initialize TIM2 */
    TIM2_Init();

    /* Start TIM2 */
    TIM2_Start();

    /* LED initially OFF */
    GPIO_WritePin(GPIOA, 5, GPIO_RESET);

    while(1)
    {
        GPIO_TogglePin(GPIOA, 5);

        delay_ms(500);
    }
}