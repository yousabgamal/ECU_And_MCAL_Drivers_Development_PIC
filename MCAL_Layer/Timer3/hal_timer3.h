/* 
 * File:   hal_timer3.h
 * Author: Yousab Gamal
 * Created on March 29, 2025, 1:45 PM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/******************Section: Includes**********************/
#include <xc.h>
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"

/******************Section: Macros Declarations***********/
#define TIMER3_COUNTER_MODE               1
#define TIMER3_TIMER_MODE                 0

#define TIMER3_ASYNC_COUNTER_MODE         1
#define TIMER3_SYNC_COUNTER_MODE          0

#define TIMER3_PRESCALER_DIV_BY_1         0
#define TIMER3_PRESCALER_DIV_BY_2         1
#define TIMER3_PRESCALER_DIV_BY_4         2
#define TIMER3_PRESCALER_DIV_BY_8         3

#define TIMER3_RW_REGESTER_8BIT_MODE      0
#define TIMER3_RW_REGESTER_16BIT_MODE     1

/******************Section: Macros Functions Declarations*/
#define TIMER3_PRESCALER_SELECT(PRESCALER_VALUE)    (T3CONbits.T3CKPS = PRESCALER_VALUE)

#define TIMER3_RW_REGESTER_8BIT_MODE_ENABLE()       (T3CONbits.RD16 = 0)
#define TIMER3_RW_REGESTER_16BIT_MODE_ENABLE()      (T3CONbits.RD16 = 1)

#define TIMER3_MODULE_ENABLE()              (T3CONbits.TMR3ON = 1)
#define TIMER3_MODULE_DISABLE()             (T3CONbits.TMR3ON = 0)

#define TIMER3_TIMER_MODE_ENABLE()          (T3CONbits.TMR3CS = 0)
#define TIMER3_COUNTER_MODE_ENABLE()        (T3CONbits.TMR3CS = 1)

#define TIMER3_ASYNC_COUNTER_MODE_ENABLE()  (T3CONbits.T3SYNC = 1)
#define TIMER3_SYNC_COUNTER_MODE_ENABLE()   (T3CONbits.T3SYNC = 0)

/******************Section: Data Types Declarations*******/
typedef struct{
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void(*TIMER3_InterruptHandler)(void);
    interrupt_priority_cfg priority; 
#endif
    uint8 timer3_reserved      : 3;
    uint8 prescaler_value      : 2;
    uint8 timer3_mode          : 1;
    uint8 timer3_counter_mode  : 1;
    uint8 timer3_reg_wr_mode   : 1;
    uint16 timer3_preload_value;
}timer3_t;

/******************Section: Functions Declarations********/
Std_ReturnType Timer3_Init(const timer3_t * timer);
Std_ReturnType Timer3_DeInit(const timer3_t * timer);
Std_ReturnType Timer3_Write_Value(const timer3_t * timer , uint16 value);
Std_ReturnType Timer3_Read_Value(const timer3_t * timer , uint16  *value);

#endif	/* HAL_TIMER3_H */

