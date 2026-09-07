#include "led.h"
#include "gpio.h"

void LED_Init(void)
{
    /* Enable GPIOA clock */
    GPIO_PeripClockControl(LED_PORT, GPIO_ENABLE);

    /* Configure PA5 as output */
    GPIO_SetPinMode(LED_PORT, LED_PIN, GPIO_OUTPUT);

    /* Push-pull output */
    GPIO_SetPinOutputType(LED_PORT, LED_PIN, GPIO_PUSH_PULL);

    /* No pull-up / pull-down */
    GPIO_SetPinPull(LED_PORT, LED_PIN, GPIO_NO_PUPD);

    /* LED initially OFF */
    LED_Off();
}


void LED_On(void)
{
    GPIO_WritePin(LED_PORT, LED_PIN, GPIO_SET);
}


void LED_Off(void)
{
    GPIO_WritePin(LED_PORT, LED_PIN, GPIO_RESET);
}


void LED_Toggle(void)
{
    GPIO_TogglePin(LED_PORT, LED_PIN);
}


void LED_Write(uint8_t state)
{
    if(state == GPIO_SET)
    {
        LED_On();
    }
    else
    {
        LED_Off();
    }
}