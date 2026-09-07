#include "i2c.h"
#include "gpio.h"


/*
 * I2C SR1 flags
 */
#define I2C_SR1_SB       (1U << 0)
#define I2C_SR1_ADDR     (1U << 1)
#define I2C_SR1_BTF      (1U << 2)
#define I2C_SR1_RXNE     (1U << 6)
#define I2C_SR1_TXE      (1U << 7)
#define I2C_SR1_BERR     (1U << 8)
#define I2C_SR1_ARLO     (1U << 9)
#define I2C_SR1_AF       (1U << 10)
#define I2C_SR1_OVR      (1U << 11)


/*
 * I2C CR1 bits
 */
#define I2C_CR1_PE       (1U << 0)
#define I2C_CR1_START    (1U << 8)
#define I2C_CR1_STOP     (1U << 9)
#define I2C_CR1_ACK      (1U << 10)
#define I2C_CR1_SWRST    (1U << 15)


/*
 * Timeout value
 */
#define I2C_TIMEOUT      100000U


void I2C1_Init(void)
{
    /*
     * Enable GPIOB clock
     */
    GPIO_PeripClockControl(GPIOB, GPIO_ENABLE);


    /*
     * Configure PB8 as Alternate Function
     */
    GPIO_SetPinMode(GPIOB,
                    I2C1_SCL_PIN,
                    GPIO_ALTFUNC);

    /*
     * Configure PB9 as Alternate Function
     */
    GPIO_SetPinMode(GPIOB,
                    I2C1_SDA_PIN,
                    GPIO_ALTFUNC);


    /*
     * I2C1 uses Alternate Function 4
     */
    GPIO_SetPinAlternateFunction(GPIOB,
                                 I2C1_SCL_PIN,
                                 I2C1_AF);

    GPIO_SetPinAlternateFunction(GPIOB,
                                 I2C1_SDA_PIN,
                                 I2C1_AF);


    /*
     * I2C requires open-drain outputs.
     */
    GPIO_SetPinOutputType(GPIOB,
                          I2C1_SCL_PIN,
                          GPIO_OPEN_DRAIN);

    GPIO_SetPinOutputType(GPIOB,
                          I2C1_SDA_PIN,
                          GPIO_OPEN_DRAIN);


    /*
     * Pull-up configuration.
     *
     * External I2C pull-up resistors are recommended.
     */
    GPIO_SetPinPull(GPIOB,
                    I2C1_SCL_PIN,
                    GPIO_PULLUP);

    GPIO_SetPinPull(GPIOB,
                    I2C1_SDA_PIN,
                    GPIO_PULLUP);


    /*
     * Enable I2C1 peripheral clock.
     *
     * RCC APB1ENR:
     *
     * Bit 21 = I2C1EN
     */
    RCC->APB1ENR |= (1U << 21);


    /*
     * Reset I2C peripheral.
     */
    I2C1->CR1 |= I2C_CR1_SWRST;

    I2C1->CR1 &= ~I2C_CR1_SWRST;


    /*
     * I2C peripheral clock frequency.
     *
     * APB1 = 16 MHz
     *
     * CR2 FREQ[5:0] = 16
     */
    I2C1->CR2 &= ~(0x3FU);
    I2C1->CR2 |= 16U;


    /*
     * Own address.
     *
     * We are operating as master, so this is
     * not important for our current application.
     *
     * Bit 14 must be kept at 1 according to
     * the STM32 I2C peripheral requirements.
     */
    I2C1->OAR1 = (1U << 14);


    /*
     * Standard mode = 100 kHz
     *
     * CCR = Fpclk / (2 * Fscl)
     *
     * CCR = 16 MHz / (2 * 100 kHz)
     *     = 80
     */
    I2C1->CCR = 80U;


    /*
     * Maximum rise time for Standard Mode:
     *
     * TRISE = FREQ(MHz) + 1
     *
     * = 16 + 1
     * = 17
     */
    I2C1->TRISE = 17U;


    /*
     * Enable I2C peripheral.
     */
    I2C1->CR1 |= I2C_CR1_PE;
}


uint8_t I2C1_WaitForFlag(uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT;

    while((I2C1->SR1 & flag) == 0U)
    {
        /*
         * Check for common I2C errors.
         */
        if(I2C1->SR1 & I2C_SR1_BERR)
        {
            return I2C_ERROR;
        }

        if(I2C1->SR1 & I2C_SR1_ARLO)
        {
            return I2C_ERROR;
        }

        if(I2C1->SR1 & I2C_SR1_AF)
        {
            return I2C_ERROR;
        }

        if(timeout == 0U)
        {
            return I2C_ERROR;
        }

        timeout--;
    }

    return I2C_OK;
}


void I2C1_Start(void)
{
    /*
     * Generate START condition.
     */
    I2C1->CR1 |= I2C_CR1_START;
}


void I2C1_Stop(void)
{
    /*
     * Generate STOP condition.
     */
    I2C1->CR1 |= I2C_CR1_STOP;
}


uint8_t I2C1_MasterWriteByte(uint8_t slaveAddress,
                             uint8_t data)
{
    /*
     * Generate START
     */
    I2C1_Start();


    /*
     * Wait for SB flag.
     */
    if(I2C1_WaitForFlag(I2C_SR1_SB) != I2C_OK)
    {
        I2C1_Stop();
        return I2C_ERROR;
    }


    /*
     * Send 7-bit slave address + WRITE bit.
     *
     * Write bit = 0
     *
     * Address goes into bits [7:1].
     */
    I2C1->DR = ((uint32_t)slaveAddress << 1);


    /*
     * Wait for ADDR flag.
     */
    if(I2C1_WaitForFlag(I2C_SR1_ADDR) != I2C_OK)
    {
        I2C1_Stop();
        return I2C_ERROR;
    }


    /*
     * Clear ADDR flag.
     *
     * On STM32F4 this is done by reading SR1
     * followed by reading SR2.
     */
    (void)I2C1->SR1;
    (void)I2C1->SR2;


    /*
     * Wait until data register is empty.
     */
    if(I2C1_WaitForFlag(I2C_SR1_TXE) != I2C_OK)
    {
        I2C1_Stop();
        return I2C_ERROR;
    }


    /*
     * Send data.
     */
    I2C1->DR = data;


    /*
     * Wait until transfer is complete.
     */
    if(I2C1_WaitForFlag(I2C_SR1_BTF) != I2C_OK)
    {
        I2C1_Stop();
        return I2C_ERROR;
    }


    /*
     * Generate STOP.
     */
    I2C1_Stop();


    return I2C_OK;
}


uint8_t I2C1_MasterWrite(uint8_t slaveAddress,
                         uint8_t *data,
                         uint32_t length)
{
    uint32_t i;


    if(data == 0)
    {
        return I2C_ERROR;
    }


    if(length == 0U)
    {
        return I2C_ERROR;
    }


    /*
     * START
     */
    I2C1_Start();


    /*
     * Wait for START condition.
     */
    if(I2C1_WaitForFlag(I2C_SR1_SB) != I2C_OK)
    {
        I2C1_Stop();
        return I2C_ERROR;
    }


    /*
     * Send slave address + WRITE.
     */
    I2C1->DR = ((uint32_t)slaveAddress << 1);


    /*
     * Wait for address acknowledge.
     */
    if(I2C1_WaitForFlag(I2C_SR1_ADDR) != I2C_OK)
    {
        I2C1_Stop();
        return I2C_ERROR;
    }


    /*
     * Clear ADDR flag.
     */
    (void)I2C1->SR1;
    (void)I2C1->SR2;


    /*
     * Send all bytes.
     */
    for(i = 0U; i < length; i++)
    {
        /*
         * Wait until DR is empty.
         */
        if(I2C1_WaitForFlag(I2C_SR1_TXE) != I2C_OK)
        {
            I2C1_Stop();
            return I2C_ERROR;
        }


        /*
         * Send byte.
         */
        I2C1->DR = data[i];
    }


    /*
     * Wait for final byte transfer.
     */
    if(I2C1_WaitForFlag(I2C_SR1_BTF) != I2C_OK)
    {
        I2C1_Stop();
        return I2C_ERROR;
    }


    /*
     * STOP
     */
    I2C1_Stop();


    return I2C_OK;
}