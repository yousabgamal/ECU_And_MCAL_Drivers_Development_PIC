/* 
 * File:   hal_timer0.c
 * Author: Yousab Gamal
 * Created on March 25, 2025, 2:45 PM
 */

#include "hal_timer0.h"

static inline void Timer0_Prescaler_Config(const timer0_t * timer);
static inline void Timer0_Mode_Select(const timer0_t * timer);
static inline void Timer0_Register_Size_Select(const timer0_t * timer);

#if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*TMR0_InterruptHandler)(void);
#endif

static uint16 timer0_preload = 0;
    
/**
 * @brief initialize timer0 module by accessing the register(T0CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer0_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer0_Init(const timer0_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER0_MODULE_DISABLE();
        Timer0_Prescaler_Config(timer);
        Timer0_Mode_Select(timer);
        Timer0_Register_Size_Select(timer);
        TMR0H = (timer -> timer0_preload_value) >> 8;
        TMR0L = (uint8)(timer -> timer0_preload_value);
        timer0_preload = timer -> timer0_preload_value;
#if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptEnable();
        TIMER0_InterruptFlagClear();
        TMR0_InterruptHandler = timer -> TIMER0_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER0_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer -> priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER0_LowPrioritySet();
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
        TIMER0_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief de initialize timer0 module by accessing the register(T0CON)
 *        and write the desired values in it's bits.
 * @param timer pointer points to timer0_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer0_DeInit(const timer0_t * timer)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TIMER0_MODULE_DISABLE();
        #if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptDisable();
        #endif
    }
    return ret;
}

/**
 * @brief write the value in TMR0L and TMR0H(if 16-bit).
 * @param timer pointer points to timer0_t data.
 * @param value the data that should be written in 
 *        TMR0L and TMR0H(if 16-bit).
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer0_Write_Value(const timer0_t * timer , uint16 value)
{
    Std_ReturnType ret = E_OK;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR0H = value >> 8;
        TMR0L = (uint8)value;
    }
    return ret;
}

/**
 * @brief read the value written in TMR0L and TMR0H(if 16-bit).
 * @param timer pointer points to timer0_t data.
 * @param value pointer points to data location that
 *              should be read from it.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType Timer0_Read_Value(const timer0_t * timer , uint16  *value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmr0_low = 0 , l_tmr0_high = 0;
    if(NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        l_tmr0_low = TMR0L;
        l_tmr0_high = TMR0H;
        *value = (uint16)((l_tmr0_high << 8) + l_tmr0_low);
    }
    return ret;
}

/**
 * @brief void function called when an interrupt 
 *        occurs in TIMER0 module.
 */
void TMR0_ISR(void)
{
    TIMER0_InterruptFlagClear();
    TMR0H = timer0_preload >> 8;
    TMR0L = (uint8)timer0_preload;
    if(TMR0_InterruptHandler)
    {
        TMR0_InterruptHandler();
    }
}

/**
 * @brief a helper function used to assign user Prescaler choice
 *        in it's 3-bits.  
 * @param timer pointer points to timer0_t data.
 */
static inline void Timer0_Prescaler_Config(const timer0_t * timer)
{
    if(TIMER0_PRESCALER_ENABLE_CGF == timer -> prescaler_enable)
    {
        TIMER0_PRESCALER_ENABLE();
        T0CONbits.T0PS = timer -> prescaler_value;
    }
    else if(TIMER0_PRESCALER_DISABLE_CGF == timer -> prescaler_enable)
    {
        TIMER0_PRESCALER_DISABLE();
    }
    else
    {
        /*NOTHING*/
    }
}

/**
 * @brief select timer mode / counter mode in TIMER0.
 * @param timer pointer points to timer0_t data.
 */
static inline void Timer0_Mode_Select(const timer0_t * timer)
{
    if(TIMER0_TIMER_MODE == timer -> timer0_mode)
    {
        TIMER0_TIMER_MODE_ENABLE();
    }
    else if(TIMER0_COUNTER_MODE == timer -> timer0_mode)
    {
        TIMER0_COUNTER_MODE_ENABLE();
        if(TIMER0_RISING_EDGE_CGF == timer -> timer0_counter_edge)
        {
            TIMER0_RISING_EDGE_ENABLE();
        }
        else if(TIMER0_FALLING_EDGE_CGF == timer -> timer0_counter_edge)
        {
            TIMER0_FALLING_EDGE_ENABLE();
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
 * @brief select register size 8-bit / 16-bit in TIMER0.
 * @param timer pointer points to timer0_t data.
 */
static inline void Timer0_Register_Size_Select(const timer0_t * timer)
{
    if(TIMER0_8BIT_REGISTER_MODE == timer -> timer0_register_size)
    {
        TIMER0_8BIT_REGISTER_MODE_ENABLE();
    }
    else if(TIMER0_16BIT_REGISTER_MODE == timer -> timer0_register_size)
    {
        TIMER0_16BIT_REGISTER_MODE_ENABLE();
    }
    else
    {
        /*NOTHING*/
    }
}