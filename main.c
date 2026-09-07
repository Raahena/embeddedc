
#include "timer.h"
#include "led.h"
#include "ir_sensor.h"
#include "keypad.h"
#include "i2c.h"
#include "oled.h"
#include "ultrasonic.h"

#define ACCESS_DISTANCE_CM 100U
#define PASSWORD_LENGTH 4U

static const char PASSWORD[] = "1234";

static void OLED_ShowMessage(const char *line1,
                             const char *line2,
                             const char *line3)
{
    OLED_Clear();

    OLED_SetCursor(0, 0);
    OLED_WriteString(line1);

    OLED_SetCursor(0, 2);
    OLED_WriteString(line2);

    OLED_SetCursor(0, 4);
    OLED_WriteString(line3);

    OLED_UpdateScreen();
}

static uint8_t CheckPassword(void)
{
    char entered[PASSWORD_LENGTH + 1];
    uint8_t index = 0;
    char key;

    while (index < PASSWORD_LENGTH)
    {
        key = Keypad_GetKey();

        if (key != 0)
        {
            if (key >= '0' && key <= '9')
            {
                entered[index] = key;
                index++;

                OLED_SetCursor(index * 6, 6);
                OLED_WriteString("*");
                OLED_UpdateScreen();
            }
        }
    }

    entered[PASSWORD_LENGTH] = '\0';

    for (index = 0; index < PASSWORD_LENGTH; index++)
    {
        if (entered[index] != PASSWORD[index])
        {
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    uint32_t distance;
    uint8_t password_ok;

    /* Initialize peripherals */
    TIM2_Init();
    LED_Init();
    IR_Init();
    Keypad_Init();
    I2C1_Init();
    OLED_Init();
    Ultrasonic_Init();

    /* Initial screen */
    OLED_ShowMessage("HAZARDOUS ZONE",
                     "ACCESS CONTROL",
                     "SYSTEM READY");

    delay_ms(1000);

    while (1)
    {
        /* Check IR sensor */
        if (IR_IsDetected())
        {
            /* Check distance */
            distance = Ultrasonic_GetDistanceCm();

            if (distance != 0 &&
                distance <= ACCESS_DISTANCE_CM)
            {
                /* Ask for PIN */
                OLED_Clear();

                OLED_SetCursor(0, 0);
                OLED_WriteString("PERSON DETECTED");

                OLED_SetCursor(0, 2);
                OLED_WriteString("ENTER PIN:");

                OLED_SetCursor(0, 4);
                OLED_WriteString("____");

                OLED_UpdateScreen();

                /* Check password */
                password_ok = CheckPassword();

                if (password_ok)
                {
                    /* Access granted */
                    LED_On();

                    OLED_ShowMessage("ACCESS GRANTED",
                                     "WELCOME",
                                     "ZONE ENTRY OK");

                    delay_ms(3000);

                    LED_Off();
                }
                else
                {
                    /* Access denied */
                    LED_Off();

                    OLED_ShowMessage("ACCESS DENIED",
                                     "WRONG PIN",
                                     "TRY AGAIN");

                    delay_ms(2000);
                }

                /* Return to ready state */
                OLED_ShowMessage("HAZARDOUS ZONE",
                                 "ACCESS CONTROL",
                                 "SYSTEM READY");

                delay_ms(500);
            }
        }

        delay_ms(100);
    }
}