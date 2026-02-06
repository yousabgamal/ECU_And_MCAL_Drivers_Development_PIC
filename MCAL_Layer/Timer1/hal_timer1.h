/* 
 * File:   hal_timer1.h
 * Author: Yousab Gamal
 * Created on March 28, 2025, 7:33 PM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/******************Section: Includes**********************/
#include <xc.h>
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"

/******************Section: Macros Declarations***********/
#define TIMER1_COUNTER_MODE               1
#define TIMER1_TIMER_MODE                 0

#define TIMER1_ASYNC_COUNTER_MODE         1
#define TIMER1_SYNC_COUNTER_MODE          0

#define TIMER1_OSC_ENABLE                 1
#define TIMER1_OSC_DISABLE                0

#define TIMER1_PRESCALER_DIV_BY_1         0
#define TIMER1_PRESCALER_DIV_BY_2         1
#define TIMER1_PRESCALER_DIV_BY_4         2
#define TIMER1_PRESCALER_DIV_BY_8         3

#define TIMER1_RW_REGESTER_8BIT_MODE      0
#define TIMER1_RW_REGESTER_16BIT_MODE     1

/******************Section: Macros Functions Declarations*/
#define TIMER1_PRESCALER_SELECT(PRESCALER_VALUE)    (T1CONbits.T1CKPS = PRESCALER_VALUE)

#define TIMER1_RW_REGESTER_8BIT_MODE_ENABLE()       (T1CONbits.RD16 = 0)
#define TIMER1_RW_REGESTER_16BIT_MODE_ENABLE()      (T1CONbits.RD16 = 1)

#define TIMER1_MODULE_ENABLE()              (T1CONbits.TMR1ON = 1)
#define TIMER1_MODULE_DISABLE()             (T1CONbits.TMR1ON = 0)

#define TIMER1_TIMER_MODE_ENABLE()          (T1CONbits.TMR1CS = 0)
#define TIMER1_COUNTER_MODE_ENABLE()        (T1CONbits.TMR1CS = 1)

#define TIMER1_ASYNC_COUNTER_MODE_ENABLE()  (T1CONbits.T1SYNC = 1)
#define TIMER1_SYNC_COUNTER_MODE_ENABLE()   (T1CONbits.T1SYNC = 0)

#define TIMER1_OSC_HW_ENABLE()              (T1CONbits.T1OSCEN = 1)
#define TIMER1_OSC_HW_DISABLE()             (T1CONbits.T1OSCEN = 0)

#define TIMER1_SYSTEM_CLK_STATUS()          (T1CONbits.T1RUN)

/******************Section: Data Types Declarations*******/
typedef struct{
#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void(*TIMER1_InterruptHandler)(void);
    interrupt_priority_cfg priority; 
#endif
    uint8 prescaler_value      : 2;
    uint8 timer1_reserved      : 2;
    uint8 timer1_mode          : 1;
    uint8 timer1_counter_mode  : 1;
    uint8 timer1_osc_cfg       : 1;
    uint8 timer1_reg_wr_mode   : 1;
    uint16 timer1_preload_value;
}timer1_t;

/******************Section: Functions Declarations********/
Std_ReturnType Timer1_Init(const timer1_t * timer);
Std_ReturnType Timer1_DeInit(const timer1_t * timer);
Std_ReturnType Timer1_Write_Value(const timer1_t * timer , uint16 value);
Std_ReturnType Timer1_Read_Value(const timer1_t * timer , uint16  *value);

#endif	/* HAL_TIMER1_H */

