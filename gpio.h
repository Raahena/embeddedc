#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* GPIO enable/disable */
#define GPIO_ENABLE     1U
#define GPIO_DISABLE    0U

/* GPIO modes */
#define GPIO_INPUT      0U
#define GPIO_OUTPUT     1U
#define GPIO_ALTFUNC    2U
#define GPIO_ANALOG     3U

/* GPIO pin state */
#define GPIO_SET        1U
#define GPIO_RESET      0U

/* Pull-up / Pull-down */
#define GPIO_NO_PUPD    0U
#define GPIO_PULLUP     1U
#define GPIO_PULLDOWN   2U

/* Output type */
#define GPIO_PUSH_PULL  0U
#define GPIO_OPEN_DRAIN 1U

/* GPIO register structure */
typedef struct
{
    uint32_t MODER;       /* Offset 0x00 */
    uint32_t OTYPER;      /* Offset 0x04 */
    uint32_t OSPEEDR;     /* Offset 0x08 */
    uint32_t PUPDR;       /* Offset 0x0C */
    uint32_t IDR;         /* Offset 0x10 */
    uint32_t ODR;         /* Offset 0x14 */
    uint32_t BSRR;        /* Offset 0x18 */
    uint32_t LCKR;        /* Offset 0x1C */
    uint32_t AFRL;        /* Offset 0x20 */
    uint32_t AFRH;        /* Offset 0x24 */
} GPIO_RegDef_t;

/* RCC register structure */
typedef struct
{
    uint32_t CR;              /* 0x00 */
    uint32_t PLLCFGR;         /* 0x04 */
    uint32_t CFGR;            /* 0x08 */
    uint32_t CIR;             /* 0x0C */
    uint32_t AHB1RSTR;        /* 0x10 */
    uint32_t AHB2RSTR;        /* 0x14 */
    uint32_t AHB3RSTR;        /* 0x18 */
    uint32_t RESERVED0;       /* 0x1C */
    uint32_t APB1RSTR;        /* 0x20 */
    uint32_t APB2RSTR;        /* 0x24 */
    uint32_t RESERVED1[2];    /* 0x28, 0x2C */
    uint32_t AHB1ENR;         /* 0x30 */
    uint32_t AHB2ENR;         /* 0x34 */
    uint32_t AHB3ENR;         /* 0x38 */
    uint32_t RESERVED2;       /* 0x3C */
    uint32_t APB1ENR;         /* 0x40 */
    uint32_t APB2ENR;         /* 0x44 */
} RCC_RegDef_t;


/* GPIO base addresses */

#define GPIOA   ((volatile GPIO_RegDef_t *)0x40020000U)
#define GPIOB   ((volatile GPIO_RegDef_t *)0x40020400U)
#define GPIOC   ((volatile GPIO_RegDef_t *)0x40020800U)
#define GPIOD   ((volatile GPIO_RegDef_t *)0x40020C00U)
#define GPIOE   ((volatile GPIO_RegDef_t *)0x40021000U)
#define GPIOF   ((volatile GPIO_RegDef_t *)0x40021400U)
#define GPIOG   ((volatile GPIO_RegDef_t *)0x40021800U)
#define GPIOH   ((volatile GPIO_RegDef_t *)0x40021C00U)
#define GPIOI   ((volatile GPIO_RegDef_t *)0x40022000U)

/* RCC base address */

#define RCC     ((volatile RCC_RegDef_t *)0x40023800U)


/* Function prototypes */

void GPIO_PeripClockControl(volatile GPIO_RegDef_t *pGPIOx,
                            uint8_t EnorDi);

void GPIO_SetPinMode(volatile GPIO_RegDef_t *pGPIOx,
                     uint8_t pinNum,
                     uint8_t mode);

void GPIO_SetPinPull(volatile GPIO_RegDef_t *pGPIOx,
                     uint8_t pinNum,
                     uint8_t pull);

void GPIO_SetPinOutputType(volatile GPIO_RegDef_t *pGPIOx,
                           uint8_t pinNum,
                           uint8_t type);

void GPIO_WritePin(volatile GPIO_RegDef_t *pGPIOx,
                   uint8_t pinNum,
                   uint8_t value);

uint8_t GPIO_ReadFromInputPin(volatile GPIO_RegDef_t *pGPIOx,
                              uint8_t pinNum);

void GPIO_TogglePin(volatile GPIO_RegDef_t *pGPIOx,
                    uint8_t pinNum);

#endif /* GPIO_H */