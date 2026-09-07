#include "gpio.h"

void GPIO_PeripClockControl(volatile GPIO_RegDef_t *pGPIOx,
                            uint8_t EnorDi)
{
    if(EnorDi == GPIO_ENABLE)
    {
        if(pGPIOx == GPIOA)
        {
            RCC->AHB1ENR |= (1U << 0);
        }
        else if(pGPIOx == GPIOB)
        {
            RCC->AHB1ENR |= (1U << 1);
        }
        else if(pGPIOx == GPIOC)
        {
            RCC->AHB1ENR |= (1U << 2);
        }
        else if(pGPIOx == GPIOD)
        {
            RCC->AHB1ENR |= (1U << 3);
        }
        else if(pGPIOx == GPIOE)
        {
            RCC->AHB1ENR |= (1U << 4);
        }
        else if(pGPIOx == GPIOF)
        {
            RCC->AHB1ENR |= (1U << 5);
        }
        else if(pGPIOx == GPIOG)
        {
            RCC->AHB1ENR |= (1U << 6);
        }
        else if(pGPIOx == GPIOH)
        {
            RCC->AHB1ENR |= (1U << 7);
        }
        else if(pGPIOx == GPIOI)
        {
            RCC->AHB1ENR |= (1U << 8);
        }
    }
    else
    {
        if(pGPIOx == GPIOA)
        {
            RCC->AHB1ENR &= ~(1U << 0);
        }
        else if(pGPIOx == GPIOB)
        {
            RCC->AHB1ENR &= ~(1U << 1);
        }
        else if(pGPIOx == GPIOC)
        {
            RCC->AHB1ENR &= ~(1U << 2);
        }
        else if(pGPIOx == GPIOD)
        {
            RCC->AHB1ENR &= ~(1U << 3);
        }
        else if(pGPIOx == GPIOE)
        {
            RCC->AHB1ENR &= ~(1U << 4);
        }
        else if(pGPIOx == GPIOF)
        {
            RCC->AHB1ENR &= ~(1U << 5);
        }
        else if(pGPIOx == GPIOG)
        {
            RCC->AHB1ENR &= ~(1U << 6);
        }
        else if(pGPIOx == GPIOH)
        {
            RCC->AHB1ENR &= ~(1U << 7);
        }
        else if(pGPIOx == GPIOI)
        {
            RCC->AHB1ENR &= ~(1U << 8);
        }
    }
}


void GPIO_SetPinMode(volatile GPIO_RegDef_t *pGPIOx,
                     uint8_t pinNum,
                     uint8_t mode)
{
    /* Clear the existing 2 bits */
    pGPIOx->MODER &= ~(3U << (2U * pinNum));

    /* Set new mode */
    pGPIOx->MODER |= (mode << (2U * pinNum));
}


void GPIO_SetPinPull(volatile GPIO_RegDef_t *pGPIOx,
                     uint8_t pinNum,
                     uint8_t pull)
{
    /* Clear existing pull configuration */
    pGPIOx->PUPDR &= ~(3U << (2U * pinNum));

    /* Set new pull configuration */
    pGPIOx->PUPDR |= (pull << (2U * pinNum));
}


void GPIO_SetPinOutputType(volatile GPIO_RegDef_t *pGPIOx,
                           uint8_t pinNum,
                           uint8_t type)
{
    /* Clear existing output type */
    pGPIOx->OTYPER &= ~(1U << pinNum);

    /* Set output type */
    pGPIOx->OTYPER |= (type << pinNum);
}


void GPIO_WritePin(volatile GPIO_RegDef_t *pGPIOx,
                   uint8_t pinNum,
                   uint8_t value)
{
    if(value == GPIO_SET)
    {
        /* Set pin using BSRR */
        pGPIOx->BSRR = (1U << pinNum);
    }
    else
    {
        /* Reset pin using BSRR */
        pGPIOx->BSRR = (1U << (pinNum + 16U));
    }
}


uint8_t GPIO_ReadFromInputPin(volatile GPIO_RegDef_t *pGPIOx,
                              uint8_t pinNum)
{
    uint8_t value;

    value = (uint8_t)((pGPIOx->IDR >> pinNum) & 0x01U);

    return value;
}


void GPIO_TogglePin(volatile GPIO_RegDef_t *pGPIOx,
                    uint8_t pinNum)
{
    pGPIOx->ODR ^= (1U << pinNum);
}