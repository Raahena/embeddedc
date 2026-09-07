#ifndef LED_H
#define LED_H

#include <stdint.h>

/* LED connected to PA5 */
#define LED_PORT    GPIOA
#define LED_PIN     5U

void LED_Init(void);

void LED_On(void);

void LED_Off(void);

void LED_Toggle(void);

void LED_Write(uint8_t state);

#endif /* LED_H */