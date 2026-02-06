/* 
 * File:   hal_ccp1.h
 * Author: Yousab Gamal
 * Created on March 31, 2025, 4:42 PM
 */

#ifndef HAL_CCP1_H
#define	HAL_CCP1_H

/******************Section: Includes**********************/
#include "ccp1_cfg.h"
#include <xc.h>
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"

/******************Section: Macros Declarations***********/
#define CCP_TIMER2_POSTSCALER_DIV_BY_1         1
#define CCP_TIMER2_POSTSCALER_DIV_BY_2         2
#define CCP_TIMER2_POSTSCALER_DIV_BY_3         3
#define CCP_TIMER2_POSTSCALER_DIV_BY_4         4
#define CCP_TIMER2_POSTSCALER_DIV_BY_5         5
#define CCP_TIMER2_POSTSCALER_DIV_BY_6         6
#define CCP_TIMER2_POSTSCALER_DIV_BY_7         7
#define CCP_TIMER2_POSTSCALER_DIV_BY_8         8
#define CCP_TIMER2_POSTSCALER_DIV_BY_9         9
#define CCP_TIMER2_POSTSCALER_DIV_BY_10        10
#define CCP_TIMER2_POSTSCALER_DIV_BY_11        11
#define CCP_TIMER2_POSTSCALER_DIV_BY_12        12
#define CCP_TIMER2_POSTSCALER_DIV_BY_13        13
#define CCP_TIMER2_POSTSCALER_DIV_BY_14        14
#define CCP_TIMER2_POSTSCALER_DIV_BY_15        15
#define CCP_TIMER2_POSTSCALER_DIV_BY_16        16

#define CCP_TIMER2_PRESCALER_DIV_BY_1          1
#define CCP_TIMER2_PRESCALER_DIV_BY_4          4
#define CCP_TIMER2_PRESCALER_DIV_BY_16         16

#define CCP_MODULE_DISABLE                 ((uint8)0x00)
#define CCP_CAPTURE_MODE_1_FALLING_EDGE    ((uint8)0x04)
#define CCP_CAPTURE_MODE_1_RISING_EDGE     ((uint8)0x05)
#define CCP_CAPTURE_MODE_4_RISING_EDGE     ((uint8)0x06)
#define CCP_CAPTURE_MODE_16_RISING_EDGE    ((uint8)0x07)
#define CCP_COMPARE_MODE_SET_PIN_LOW       ((uint8)0x08)
#define CCP_COMPARE_MODE_SET_PIN_HIGH      ((uint8)0x09)
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH   ((uint8)0x02)
#define CCP_COMPARE_MODE_GEN_SW_INTERRUPT  ((uint8)0x0A)
#define CCP_COMPARE_MODE_GEN_EVENT         ((uint8)0x0B)
#define CCP_PWM_MODE                       ((uint8)0X0C)

#define CCP1_CAPTURE_NOT_READY              0x00
#define CCP1_CAPTURE_READY                  0x01

#define CCP1_COMPARE_NOT_READY              0x00
#define CCP1_COMPARE_READY                  0x01

/******************Section: Macros Functions Declarations*/
#define CCP1_SET_MODE(_CONFIG)              (CCP1CONbits.CCP1M = _CONFIG)
#define CCP2_SET_MODE(_CONFIG)              (CCP2CONbits.CCP2M = _CONFIG)

/******************Section: Data Types Declarations*******/
typedef enum{
    CCP_CAPTURE_MODE_SELECT = 0,
    CCP_COMPARE_MODE_SELECT,
    CCP_PWM_MODE_SELECT        
}ccp1_mode_t;

typedef union{
    struct{
        uint8 ccpr_low;
        uint8 ccpr_high;
    };
    struct{
        uint16 ccpr_16bit;
    };
}ccp_reg_t;

typedef enum {
    CCP1_INST,
    CCP2_INST,        
}ccp_inst;

typedef enum {
    CCP1_CCP2_TIMER3 = 0,
    CCP1_TIMER1_CCP2_TIMER3,
    CCP1_CCP2_TIMER1
}ccp_capture_timer;

typedef struct{
#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void(*CCP1_InterruptHandler)(void);
    interrupt_priority_cfg ccp1_priority; 
#endif
    #if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void(*CCP2_InterruptHandler)(void);
    interrupt_priority_cfg ccp2_priority; 
#endif
#if CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED
    uint32 PWM_frequency;
    uint8 postscaler_value     : 4;
    uint8 prescaler_value      : 2;
    uint8 reserved             : 2;
#endif
    ccp_inst ccp_inst;
    ccp1_mode_t CCP_mode;
    uint8 CCP_mode_variant;
    pin_config_t pin_init;
    ccp_capture_timer ccp_capture_timer;
}ccp_t;

/******************Section: Functions Declarations********/
Std_ReturnType CCP_Init(const ccp_t *ccp_obj); 
Std_ReturnType CCP_DeInit(const ccp_t *ccp_obj); 

#if CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED
Std_ReturnType CCP_PWM_SetDuty(const ccp_t *ccp_obj , const uint8 duty);
Std_ReturnType CCP_PWM_Start(const ccp_t *ccp_obj);
Std_ReturnType CCP_PWM_Stop(const ccp_t *ccp_obj);
#endif

#if CCP_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED
Std_ReturnType CCP_IsCompareComplete(uint8 *compare_status);
Std_ReturnType CCP_CompareModeSetValue(const ccp_t *ccp_obj ,uint16 compare_value);
#endif

#if CCP_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED
Std_ReturnType CCP_IsCaptureReady(uint8 *capture_status);
Std_ReturnType CCP_CaptureModeReadValue(uint16 *capture_value);
#endif

#endif	/* HAL_CCP1_H */

