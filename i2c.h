#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/*
 * I2C1 base address
 */
#define I2C1_BASE_ADDR    0x40005400U


/*
 * I2C register structure
 */
typedef struct
{
    uint32_t CR1;        /* 0x00 */
    uint32_t CR2;        /* 0x04 */
    uint32_t OAR1;       /* 0x08 */
    uint32_t OAR2;       /* 0x0C */
    uint32_t DR;         /* 0x10 */
    uint32_t SR1;        /* 0x14 */
    uint32_t SR2;        /* 0x18 */
    uint32_t CCR;        /* 0x1C */
    uint32_t TRISE;      /* 0x20 */
    uint32_t FLTR;       /* 0x24 */
} I2C_RegDef_t;


#define I2C1    ((volatile I2C_RegDef_t *)I2C1_BASE_ADDR)


/*
 * I2C status
 */
#define I2C_OK       0U
#define I2C_ERROR    1U


/*
 * I2C1 pins
 */
#define I2C1_PORT    GPIOB

#define I2C1_SCL_PIN  8U
#define I2C1_SDA_PIN  9U

#define I2C1_AF       4U


/*
 * I2C configuration
 */
#define I2C1_PCLK_HZ     16000000U
#define I2C1_SPEED_HZ    100000U


/*
 * Initialize I2C1
 */
void I2C1_Init(void);


/*
 * Generate START condition
 */
void I2C1_Start(void);


/*
 * Generate STOP condition
 */
void I2C1_Stop(void);


/*
 * Send one byte to I2C slave
 */
uint8_t I2C1_MasterWriteByte(uint8_t slaveAddress,
                             uint8_t data);


/*
 * Write multiple bytes to I2C slave
 *
 * slaveAddress = 7-bit address
 */
uint8_t I2C1_MasterWrite(uint8_t slaveAddress,
                         uint8_t *data,
                         uint32_t length);


/*
 * Wait for a flag with timeout
 */
uint8_t I2C1_WaitForFlag(uint32_t flag);


#endif /* I2C_H */