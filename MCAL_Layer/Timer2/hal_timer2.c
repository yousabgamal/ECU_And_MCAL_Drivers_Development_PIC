/* 
 * File:   hal_timer2.c
 * Author: Yousab Gamal
 * Created on March 29, 2025, 1:39 PM
 */

#include "hal_timer2.h"

static uint8 timer2_preload = 0;

#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*TMR2_InterruptHandler)(void);
#endif

/**
 * @brief initialize timer2 module by accessing the register(T2CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer2_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer2_Init(const timer2_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER2_MODULE_DISABLE();
        TIMER2_PRESCALER_SELECT(timer -> prescaler_value);
        TIMER2_POSTSCALER_SELECT(timer -> postscaler_value);
        TMR2 = timer -> timer2_preload_value;
        timer2_preload = timer -> timer2_preload_value;
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptEnable();
        TIMER2_InterruptFlagClear();
        TMR2_InterruptHandler = timer -> TIMER2_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER2_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER2_LowPrioritySet();
        }
        else
        {
            /*NOTHING*/
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        TIMER2_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief de initialize timer2 module by accessing the register(T2CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer2_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer2_DeInit(const timer2_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER2_MODULE_DISABLE();
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptDisable();
#endif
    }
    return ret;
}

/**
 * @brief write the value in TMR2.
 * @param timer pointer points to timer2_t data.
 * @param value the data that should be written in TMR2.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer2_Write_Value(const timer2_t * timer , uint8 value)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR2 = value;
    }
    return ret;
}

/**
 * @brief read the value written in TMR2.
 * @param timer pointer points to timer2_t data.
 * @param value pointer points to data location that
 *              should be read from it.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer2_Read_Value(const timer2_t * timer , uint8  *value)
{
    Std_ReturnType ret = E_OK;
    if((NULL == timer) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        *value = TMR2;
    }
    return ret;
}

/**
 * @brief void function called when an interrupt 
 *        occurs in TIMER2 module.
 */
void TMR2_ISR(void)
{
    TIMER2_InterruptFlagClear();
    TMR2 = timer2_preload;
    if(TMR2_InterruptHandler)
    {
        TMR2_InterruptHandler();
    }
}

