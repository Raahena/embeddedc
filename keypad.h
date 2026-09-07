#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

/* Keypad port */
#define KEYPAD_PORT GPIOC

/* Rows */
#define KEYPAD_R0   0U
#define KEYPAD_R1   1U
#define KEYPAD_R2   2U
#define KEYPAD_R3   3U

/* Columns */
#define KEYPAD_C0   4U
#define KEYPAD_C1   5U
#define KEYPAD_C2   6U
#define KEYPAD_C3   7U

/* Number of rows and columns */
#define KEYPAD_ROWS 4U
#define KEYPAD_COLS 4U

void Keypad_Init(void);

char Keypad_GetKey(void);

#endif /* KEYPAD_H */