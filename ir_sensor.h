#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>

/* IR sensor configuration */
#define IR_SENSOR_PORT    GPIOB
#define IR_SENSOR_PIN     0U

/*
 * Most IR obstacle sensor modules have:
 *
 * OUT = LOW  -> Object detected
 * OUT = HIGH -> No object
 *
 * If your module behaves opposite to this,
 * change IR_DETECTED_LEVEL to GPIO_SET.
 */
#define IR_DETECTED_LEVEL GPIO_RESET

void IR_Init(void);

uint8_t IR_Read(void);

uint8_t IR_IsDetected(void);

uint8_t IR_IsClear(void);

#endif /* IR_SENSOR_H */