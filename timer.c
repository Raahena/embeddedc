#include "timer.h"
#include "gpio.h"


void TIM2_Init(void)
{
    /*
     * TIM2 is connected to APB1.
     *
     * RCC APB1 peripheral clock enable register:
     *
     * RCC->APB1ENR bit 0 = TIM2EN
     */
    RCC->APB1ENR |= (1U << 0);

    /*
     * Assuming TIM2 clock = 16 MHz.
     *
     * We want:
     *
     * 16 MHz / (15999 + 1)
     * = 1000 Hz
     *
     * That would give a 1 ms tick.
     *
     * But for ultrasonic measurement we want
     * a 1 microsecond tick.
     *
     * Therefore:
     *
     * 16 MHz / (15 + 1)
     * = 1 MHz
     *
     * 1 MHz = 1 tick per microsecond.
     */
    TIM2->PSC = 15U;

    /*
     * Maximum counting period.
     *
     * TIM2 is a 32-bit timer.
     */
    TIM2->ARR = 0xFFFFFFFFU;

    /*
     * Generate an update event so that the
     * prescaler value is loaded immediately.
     */
    TIM2->EGR = (1U << 0);

    /*
     * Reset counter.
     */
    TIM2->CNT = 0U;

    /*
     * Timer initially stopped.
     *
     * CR1 bit 0 = CEN
     */
    TIM2->CR1 &= ~(1U << 0);
}


void TIM2_Start(void)
{
    TIM2->CR1 |= (1U << 0);
}


void TIM2_Stop(void)
{
    TIM2->CR1 &= ~(1U << 0);
}


void TIM2_Reset(void)
{
    TIM2->CNT = 0U;
}


uint32_t TIM2_GetCounter(void)
{
    return TIM2->CNT;
}


void delay_us(uint32_t us)
{
    uint32_t start;

    start = TIM2_GetCounter();

    while((TIM2_GetCounter() - start) < us)
    {
        /* Wait */
    }
}


void delay_ms(uint32_t ms)
{
    while(ms--)
    {
        delay_us(1000U);
    }
}