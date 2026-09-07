#include "ir_sensor.h"
#include "gpio.h"


void IR_Init(void)
{
    /* Enable GPIOB clock */
    GPIO_PeripClockControl(IR_SENSOR_PORT, GPIO_ENABLE);

    /* Configure PB0 as input */
    GPIO_SetPinMode(IR_SENSOR_PORT,
                    IR_SENSOR_PIN,
                    GPIO_INPUT);

    /*
     * Use pull-up because many IR modules
     * have an active-low output.
     */
    GPIO_SetPinPull(IR_SENSOR_PORT,
                    IR_SENSOR_PIN,
                    GPIO_PULLUP);
}


uint8_t IR_Read(void)
{
    return GPIO_ReadFromInputPin(IR_SENSOR_PORT,
                                 IR_SENSOR_PIN);
}


uint8_t IR_IsDetected(void)
{
    uint8_t value;

    value = IR_Read();

    if(value == IR_DETECTED_LEVEL)
    {
        return 1U;
    }

    return 0U;
}


uint8_t IR_IsClear(void)
{
    if(IR_IsDetected())
    {
        return 0U;
    }

    return 1U;
}