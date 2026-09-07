#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* TIM2 base address */
#define TIM2_BASE_ADDR  0x40000000U

typedef struct
{
    uint32_t CR1;        /* 0x00 */
    uint32_t CR2;        /* 0x04 */
    uint32_t SMCR;       /* 0x08 */
    uint32_t DIER;       /* 0x0C */
    uint32_t SR;         /* 0x10 */
    uint32_t EGR;        /* 0x14 */
    uint32_t CCMR1;      /* 0x18 */
    uint32_t CCMR2;      /* 0x1C */
    uint32_t CCER;       /* 0x20 */
    uint32_t CNT;        /* 0x24 */
    uint32_t PSC;        /* 0x28 */
    uint32_t ARR;        /* 0x2C */
    uint32_t RESERVED1;  /* 0x30 */
    uint32_t CCR1;       /* 0x34 */
    uint32_t CCR2;       /* 0x38 */
    uint32_t CCR3;       /* 0x3C */
    uint32_t CCR4;       /* 0x40 */
    uint32_t RESERVED2;  /* 0x44 */
    uint32_t DCR;        /* 0x48 */
    uint32_t DMAR;       /* 0x4C */
} TIM_RegDef_t;

#define TIM2    ((volatile TIM_RegDef_t *)TIM2_BASE_ADDR)


/*
 * Initialize TIM2 for 1 microsecond timer tick.
 *
 * timer_clock_hz = actual TIM2 peripheral clock.
 *
 * For the initial Nucleo setup we assume
 * TIM2 clock = 16 MHz.
 */
void TIM2_Init(void);


/* Start timer */
void TIM2_Start(void);


/* Stop timer */
void TIM2_Stop(void);


/* Reset counter to zero */
void TIM2_Reset(void);


/* Return current counter value */
uint32_t TIM2_GetCounter(void);


/* Delay in microseconds */
void delay_us(uint32_t us);


/* Delay in milliseconds */
void delay_ms(uint32_t ms);

#endif /* TIMER_H */