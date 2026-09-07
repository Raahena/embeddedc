/*
 * ultrasonic.c
 *
 * HC-SR04 ultrasonic sensor driver
 */

#include "ultrasonic.h"
#include "timer.h"

void Ultrasonic_Init(void)
{
    /* Enable GPIOB clock */
    GPIO_PeripClockControl(ULTRASONIC_PORT, GPIO_ENABLE);

    /* TRIG -> Output */
    GPIO_SetPinMode(ULTRASONIC_PORT,
                    ULTRASONIC_TRIG_PIN,
                    GPIO_OUTPUT);



    GPIO_SetPinPull(ULTRASONIC_PORT,
                    ULTRASONIC_TRIG_PIN,
                    GPIO_NO_PUPD);

    /* ECHO -> Input */
    GPIO_SetPinMode(ULTRASONIC_PORT,
                    ULTRASONIC_ECHO_PIN,
                    GPIO_INPUT);

    GPIO_SetPinPull(ULTRASONIC_PORT,
                    ULTRASONIC_ECHO_PIN,
                    GPIO_NO_PUPD);

    /* Keep TRIG LOW initially */
    GPIO_WritePin(ULTRASONIC_PORT,
                  ULTRASONIC_TRIG_PIN,
                  GPIO_RESET);
}

uint32_t Ultrasonic_GetDistanceCm(void)
{
    uint32_t start_time;
    uint32_t end_time;
    uint32_t echo_time;

    /*
     * Make sure TRIG is LOW
     */
    GPIO_WritePin(ULTRASONIC_PORT,
                  ULTRASONIC_TRIG_PIN,
                  GPIO_RESET);

    delay_us(2);

    /*
     * Send 10 us trigger pulse
     */
    GPIO_WritePin(ULTRASONIC_PORT,
                  ULTRASONIC_TRIG_PIN,
                  GPIO_SET);

    delay_us(10);

    GPIO_WritePin(ULTRASONIC_PORT,
                  ULTRASONIC_TRIG_PIN,
                  GPIO_RESET);

    /*
     * Wait for ECHO to become HIGH.
     */
    TIM2_Reset();

    while (GPIO_ReadFromInputPin(ULTRASONIC_PORT,
                                 ULTRASONIC_ECHO_PIN) == GPIO_RESET)
    {
        if (TIM2_GetCounter() >= ULTRASONIC_TIMEOUT_US)
        {
            return 0;
        }
    }

    /*
     * ECHO is HIGH.
     * Start measuring the pulse width.
     */
    TIM2_Reset();

    start_time = TIM2_GetCounter();

    /*
     * Wait for ECHO to become LOW.
     */
    while (GPIO_ReadFromInputPin(ULTRASONIC_PORT,
                                 ULTRASONIC_ECHO_PIN) == GPIO_SET)
    {
        if (TIM2_GetCounter() >= ULTRASONIC_TIMEOUT_US)
        {
            return 0;
        }
    }

    end_time = TIM2_GetCounter();

    /*
     * Calculate ECHO pulse width.
     */
    echo_time = end_time - start_time;

    /*
     * HC-SR04:
     *
     * Distance in cm = echo time in us / 58
     */
    return echo_time / 58U;
}

uint32_t Ultrasonic_GetDistanceMm(void)
{
    uint32_t distance_cm;

    distance_cm = Ultrasonic_GetDistanceCm();

    return distance_cm * 10U;
}
