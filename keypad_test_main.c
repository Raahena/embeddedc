#include <stdint.h>

#include "gpio.h"
#include "timer.h"
#include "led.h"
#include "keypad.h"


int main(void)
{
    char key;

    /*
     * Timer
     */
    TIM2_Init();
    TIM2_Start();


    /*
     * LED
     */
    LED_Init();


    /*
     * Keypad
     */
    Keypad_Init();


    while(1)
    {
        key = Keypad_GetKey();

        if(key != '\0')
        {
            /*
             * A key was pressed
             */
            LED_Toggle();
        }
    }
}