/* 
 * File:   hal_timer3.c
 * Author: Yousab Gamal
 * Created on March 29, 2025, 1:45 PM
 */

#include "hal_timer3.h"

static inline void Timer3_Mode_Select(const timer3_t * timer);

#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*TMR3_InterruptHandler)(void);
#endif

static uint16 timer3_preload = 0;

/**
 * @brief initialize timer3 module by accessing the register(T3CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer3_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer3_Init(const timer3_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER3_MODULE_DISABLE();
        TIMER3_PRESCALER_SELECT(timer -> prescaler_value);
        Timer3_Mode_Select(timer);
        TMR3H = (timer -> timer3_preload_value) >> 8;
        TMR3L = (uint8)(timer -> timer3_preload_value);
        timer3_preload = timer -> timer3_preload_value;
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptEnable();
        TIMER3_InterruptFlagClear();
        TMR3_InterruptHandler = timer -> TIMER3_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER3_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER3_LowPrioritySet();
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
        TIMER3_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief de initialize timer3 module by accessing the register(T3CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer3_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer3_DeInit(const timer3_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER3_MODULE_DISABLE();
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptDisable();
#endif
    }
    return ret;
}

/**
 * @brief write the value in TMR3L and TMR3H.
 * @param timer pointer points to timer3_t data.
 * @param value the data that should be written in 
 *        TMR3L and TMR3H.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer3_Write_Value(const timer3_t * timer , uint16 value)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR3H = value >> 8;
        TMR3L = (uint8)value;
    }
    return ret;
}

/**
 * @brief read the value written in TMR3L and TMR3H.
 * @param timer pointer points to timer3_t data.
 * @param value pointer points to data location that
 *              should be read from it.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer3_Read_Value(const timer3_t * timer , uint16  *value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmr3_low = 0 , l_tmr3_high = 0;
    if((NULL == timer) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        l_tmr3_low = TMR3L;
        l_tmr3_high = TMR3H;
        *value = (uint16)((l_tmr3_high << 8) + l_tmr3_low);
    }
    return ret;
}

/**
 * @brief select timer mode / counter mode in TIMER3.
 * @param timer pointer points to timer3_t data.
 */
static inline void Timer3_Mode_Select(const timer3_t * timer)
{
    if(TIMER3_TIMER_MODE == timer -> timer3_mode)
    {
        TIMER3_TIMER_MODE_ENABLE();
    }
    else if(TIMER3_COUNTER_MODE == timer -> timer3_mode)
    {
        TIMER3_COUNTER_MODE_ENABLE();
        if(TIMER3_ASYNC_COUNTER_MODE == timer -> timer3_counter_mode)
        {
            TIMER3_ASYNC_COUNTER_MODE_ENABLE();
        }
        else if(TIMER3_SYNC_COUNTER_MODE == timer -> timer3_counter_mode)
        {
            TIMER3_SYNC_COUNTER_MODE_ENABLE();
        }
        else
        {
            /*NOTHING*/
        }
    }
    else
    {
        /*NOTHING*/
    }
}

/**
 * @brief void function called when an interrupt 
 *        occurs in TIMER3 module.
 */
void TMR3_ISR(void)
{
    TIMER3_InterruptFlagClear();
    TMR3H = timer3_preload >> 8;
    TMR3L = (uint8)timer3_preload;
    if(TMR3_InterruptHandler)
    {
        TMR3_InterruptHandler();
    }
}