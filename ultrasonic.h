/*
 * ultrasonic.h
 *
 *  Created on: Sep 7, 2026
 *      Author: raahe
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <stdint.h>
#include "gpio.h"

#define ULTRASONIC_PORT GPIOB

#define ULTRASONIC_TRIG_PIN 1
#define ULTRASONIC_ECHO_PIN 2

#define ULTRASONIC_TIMEOUT_US 30000U

void Ultrasonic_Init(void);

uint32_t Ultrasonic_GetDistanceCm(void);
uint32_t Ultrasonic_GetDistanceMm(void);


#endif /* ULTRASONIC_H_ */
