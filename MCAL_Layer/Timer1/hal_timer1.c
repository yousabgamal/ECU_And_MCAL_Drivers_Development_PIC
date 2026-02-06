/* 
 * File:   hal_timer1.c
 * Author: Yousab Gamal
 * Created on March 28, 2025, 7:33 PM
 */

#include "hal_timer1.h"

static inline void Timer1_Mode_Select(const timer1_t * timer);

#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*TMR1_InterruptHandler)(void);
#endif

static uint16 timer1_preload = 0;

/**
 * @brief initialize timer1 module by accessing the register(T1CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer1_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer1_Init(const timer1_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER1_MODULE_DISABLE();
        TIMER1_PRESCALER_SELECT(timer -> prescaler_value);
        Timer1_Mode_Select(timer);
        TMR1H = (timer -> timer1_preload_value) >> 8;
        TMR1L = (uint8)(timer -> timer1_preload_value);
        timer1_preload = timer -> timer1_preload_value;
#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable();
        TIMER1_InterruptFlagClear();
        TMR1_InterruptHandler = timer -> TIMER1_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_LowPrioritySet();
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
        TIMER1_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief de initialize timer1 module by accessing the register(T1CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer1_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer1_DeInit(const timer1_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER1_MODULE_DISABLE();
#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
#endif
    }
    return ret;
}

/**
 * @brief write the value in TMR1L and TMR1H.
 * @param timer pointer points to timer1_t data.
 * @param value the data that should be written in 
 *        TMR1L and TMR1H.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer1_Write_Value(const timer1_t * timer , uint16 value)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR1H = value >> 8;
        TMR1L = (uint8)value;
    }
    return ret;
}

/**
 * @brief read the value written in TMR1L and TMR1H.
 * @param timer pointer points to timer1_t data.
 * @param value pointer points to data location that
 *              should be read from it.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer1_Read_Value(const timer1_t * timer , uint16  *value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmr1_low = 0 , l_tmr1_high = 0;
    if((NULL == timer) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        l_tmr1_low = TMR1L;
        l_tmr1_high = TMR1H;
        *value = (uint16)((l_tmr1_high << 8) + l_tmr1_low);
    }
    return ret;
}

/**
 * @brief select timer mode / counter mode in TIMER1.
 * @param timer pointer points to timer1_t data.
 */
static inline void Timer1_Mode_Select(const timer1_t * timer)
{
    if(TIMER1_TIMER_MODE == timer -> timer1_mode)
    {
        TIMER1_TIMER_MODE_ENABLE();
    }
    else if(TIMER1_COUNTER_MODE == timer -> timer1_mode)
    {
        TIMER1_COUNTER_MODE_ENABLE();
        if(TIMER1_ASYNC_COUNTER_MODE == timer -> timer1_counter_mode)
        {
            TIMER1_ASYNC_COUNTER_MODE_ENABLE();
        }
        else if(TIMER1_SYNC_COUNTER_MODE == timer -> timer1_counter_mode)
        {
            TIMER1_SYNC_COUNTER_MODE_ENABLE();
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
 *        occurs in TIMER1 module.
 */
void TMR1_ISR(void)
{
    TIMER1_InterruptFlagClear();
    TMR1H = timer1_preload >> 8;
    TMR1L = (uint8)timer1_preload;
    if(TMR1_InterruptHandler)
    {
        TMR1_InterruptHandler();
    }
}