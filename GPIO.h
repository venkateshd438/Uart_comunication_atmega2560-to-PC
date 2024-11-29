/*  ==========================
atmega 2560

Created on: 28/11/2024
Author: Venkatesh 

===============================*/


/*------------------- includes section: -------------------------*/
#include <avr/io.h>
#include "typedef.h"
/*============================ end ==============================*/

/*_________________________________

// Peripheral register definition

_____________________________________*/

typedef struct {
    volatile uint8 PIN;  // Pin Input Register
    volatile uint8 DDR;  // Data Direction Register
    volatile uint8 DR;   // Data Register
} GPIO_RegDef;
 

/*_________________________________

// Peripheral definition  based on address

_____________________________________*/

#define GPIOA   ((GPIO_RegDef*)0x20)
#define GPIOB   ((GPIO_RegDef*)0x23)
#define GPIOC   ((GPIO_RegDef*)0x26)
#define GPIOD   ((GPIO_RegDef*)0x29) // major focus on PD1 and PD0.
#define GPIOE   ((GPIO_RegDef*)0x2C)
#define GPIOF   ((GPIO_RegDef*)0x2F)
#define GPIOG   ((GPIO_RegDef*)0x32)

