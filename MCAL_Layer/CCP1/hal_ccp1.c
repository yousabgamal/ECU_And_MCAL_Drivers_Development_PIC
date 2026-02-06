/* 
 * File:   hal_ccp1.c
 * Author: Yousab Gamal
 * Created on March 31, 2025, 4:42 PM
 */

#include "hal_ccp1.h"

static void CCP_InterruptConfig(const ccp_t *ccp_obj);
static void CCP_CaptureModeTimerselect(const ccp_t *ccp_obj);

#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*CCP1_InterruptHandler)(void);
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*CCP2_InterruptHandler)(void);
#endif

/**
 * @brief initialize CCP1 moduleby writing the desired value in CCP1CON register. 
 * @param ccp_obj pointer points to ccp1_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_Init(const ccp_t *ccp_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == ccp_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == ccp_obj -> ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if(CCP2_INST == ccp_obj -> ccp_inst)
        {
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
        }
        else
        {
            /*NOTHING*/
        }
        if(CCP_CAPTURE_MODE_SELECT == ccp_obj -> CCP_mode)
        {
            if(CCP1_INST == ccp_obj -> ccp_inst)
            {
                switch(ccp_obj -> CCP_mode_variant)
                {
                    case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                        CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                        break;
                    case CCP_CAPTURE_MODE_1_RISING_EDGE:
                        CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                        break;
                    case CCP_CAPTURE_MODE_4_RISING_EDGE:
                        CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                        break;
                    case CCP_CAPTURE_MODE_16_RISING_EDGE:
                        CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                        break;   
                    default:
                        ret = E_NOT_OK;
                        break;
                }
            }
            else if(CCP2_INST == ccp_obj -> ccp_inst)
            {
                switch(ccp_obj -> CCP_mode_variant)
                {
                    case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                        CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                        break;
                    case CCP_CAPTURE_MODE_1_RISING_EDGE:
                        CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                        break;
                    case CCP_CAPTURE_MODE_4_RISING_EDGE:
                        CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                        break;
                    case CCP_CAPTURE_MODE_16_RISING_EDGE:
                        CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                        break;   
                    default:
                        ret = E_NOT_OK;
                        break;
                }
            }
            else
            {
                /*NOTHING*/
            }
            CCP_CaptureModeTimerselect(ccp_obj);
        }
        else if(CCP_COMPARE_MODE_SELECT == ccp_obj -> CCP_mode)
        {
            if(CCP1_INST == ccp_obj -> ccp_inst)
            {
                switch(ccp_obj -> CCP_mode_variant)
                {
                    case CCP_COMPARE_MODE_SET_PIN_LOW:
                        CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
                        break;
                    case CCP_COMPARE_MODE_SET_PIN_HIGH:
                        CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
                        break;
                    case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
                        CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                        break;
                    case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
                        CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                        break; 
                    case CCP_COMPARE_MODE_GEN_EVENT:
                        CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                        break;    
                    default:
                        ret = E_NOT_OK;
                        break;
                }
            }
            else if(CCP2_INST == ccp_obj -> ccp_inst)
            {
                switch(ccp_obj -> CCP_mode_variant)
                {
                    case CCP_COMPARE_MODE_SET_PIN_LOW:
                        CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
                        break;
                    case CCP_COMPARE_MODE_SET_PIN_HIGH:
                        CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
                        break;
                    case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
                        CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                        break;
                    case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
                        CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                        break; 
                    case CCP_COMPARE_MODE_GEN_EVENT:
                        CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                        break;    
                    default:
                        ret = E_NOT_OK;
                        break;
                }
            }
            else
            {
                /*NOTHING*/
            }
            CCP_CaptureModeTimerselect(ccp_obj);
        }
        else if(CCP_PWM_MODE_SELECT == ccp_obj -> CCP_mode)
        {
#if CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED
            switch(ccp_obj -> ccp_inst)
            {
                case CCP1_INST:
                    CCP1_SET_MODE(CCP_PWM_MODE);
                    break;
                case CCP2_INST:
                    CCP2CONbits.CCP2M = CCP_PWM_MODE;
                    break;
                default:
                    ret = E_NOT_OK;
                    break;    
            }
            PR2 = (uint8)((_XTAL_FREQ / (ccp_obj -> PWM_frequency * 4.0 * 
                        ccp_obj -> postscaler_value * ccp_obj -> prescaler_value))-1);
#endif        
        }
        else
        {
            /*NOTHING*/
        }
        ret = gpio_pin_intialize(&ccp_obj -> pin_init);
        CCP_InterruptConfig(ccp_obj);
    }
    return ret;
}

/**
 * @brief de initialize CCP1 module. 
 * @param ccp_obj pointer points to ccp_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_DeInit(const ccp_t *ccp_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == ccp_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == ccp_obj -> ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
         CCP1_InterruptDisable();
#endif
        }
        else if(CCP2_INST == ccp_obj -> ccp_inst)
        {
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        CCP2_InterruptDisable();
#endif     
        }
        else
        {
            /*NOTHING*/
        }
    }
    return ret;
}

#if CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED
/**
 * @breif calculate the value that should be store in... 
 *        -if CCP1 enabled: CCPR1L register and DC1B(two bits).
 *        -if CCP2 enabled: CCPR2L register and DC2B(two bits).
 * @param duty hold the duty cycle in percents.
 * @param ccp_obj pointer points to ccp_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_PWM_SetDuty(const ccp_t *ccp_obj , const uint8 duty)
{
    Std_ReturnType ret = E_OK;
    if(NULL == ccp_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        uint16 l_duty_temp = 0;
        l_duty_temp = (uint16)(4 * (PR2 + 1) * (duty / 100.0));  
        if(CCP1_INST == ccp_obj -> ccp_inst)
        {
            CCP1CONbits.DC1B = (uint8)(l_duty_temp & 0x03);
            CCPR1L = (uint8)(l_duty_temp >> 2);
        }
        else if(CCP2_INST == ccp_obj -> ccp_inst)
        {
            CCP2CONbits.DC2B = (uint8)(l_duty_temp & 0x03);
            CCPR2L = (uint8)(l_duty_temp >> 2);
        }
        else
        {
            /*NOTHING*/
        }
    }
    return ret;
}

/**
 * @brief enable CCP PWM module.
 * @param ccp_obj pointer points to ccp_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_PWM_Start(const ccp_t *ccp_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == ccp_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == ccp_obj -> ccp_inst)
        {
            CCP1_SET_MODE(CCP_PWM_MODE);
        }
        else if(CCP2_INST == ccp_obj -> ccp_inst)
        {
            CCP2CONbits.CCP2M = CCP_PWM_MODE;
        }
        else
        {
            /*NOTHING*/
        }
    }
    return ret;
}

/**
 * @brief disable the whole CCP module.
 * @param ccp_obj pointer points to ccp_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function. 
 */
Std_ReturnType CCP_PWM_Stop(const ccp_t *ccp_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == ccp_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == ccp_obj -> ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if(CCP2_INST == ccp_obj -> ccp_inst)
        {
            CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
        }
        else
        {
            /*NOTHING*/
        }
    }
    return ret;
}
#endif

#if CCP_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED
/**
 * @breif check CCP1IF if (CCP1IF = 1): CCP1IF = 0.
 * @param compare_status pointer to store CCP1IF status.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_IsCompareComplete(uint8 *compare_status)
{
    Std_ReturnType ret = E_OK;
    if(NULL == compare_status)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_COMPARE_READY == PIR1bits.CCP1IF)
        {
            *compare_status = CCP1_COMPARE_READY;
            CCP1_InterruptFlagClear();
        }
        else if(CCP1_COMPARE_NOT_READY == PIR1bits.CCP1IF)
        {
            *compare_status = CCP1_COMPARE_NOT_READY;
        }
        else
        {
            /*NOTHING*/
        }
    }
    return ret;
}

/**
 * @brief write the desired value in CCPR1H and CCPR1L;
 * @param compare_value hold the desired value.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_CompareModeSetValue(const ccp_t *ccp_obj ,uint16 compare_value)
{
    Std_ReturnType ret = E_OK;
    ccp_reg_t capture_temp_value ={.ccpr_low = 0 , .ccpr_high = 0};
    capture_temp_value.ccpr_16bit = compare_value;
    if(NULL == ccp_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == ccp_obj -> ccp_inst)
        {
            CCPR1L = capture_temp_value.ccpr_low;
            CCPR1H = capture_temp_value.ccpr_high;
        }
        else if(CCP2_INST == ccp_obj -> ccp_inst)
        {
            CCPR2L = capture_temp_value.ccpr_low;
            CCPR2H = capture_temp_value.ccpr_high;
        }
        else
        {
            /*NOTHING*/
        }
    }
    return ret;
}
#endif

#if CCP_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED
/**
 * @breif check CCP1IF if (CCP1IF = 1): CCP1IF = 0.
 * @param capture_status pointer to store CCP1IF status.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_IsCaptureReady(uint8 *capture_status)
{
    Std_ReturnType ret = E_OK;
    if(NULL == capture_status)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_CAPTURE_READY == PIR1bits.CCP1IF)
        {
            *capture_status = CCP1_CAPTURE_READY;
            CCP1_InterruptFlagClear();
        }
        else if(CCP1_CAPTURE_NOT_READY == PIR1bits.CCP1IF)
        {
            *capture_status = CCP1_CAPTURE_NOT_READY;
        }
        else
        {
            /*NOTHING*/
        }
    }
    return ret;
}

/**
 * @breif read and store CCPR1H + CCPR1L in a pointer variable.
 * @param capture_value pointer to store CCPR1H + CCPR1L.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType CCP_CaptureModeReadValue(uint16 *capture_value)
{
    Std_ReturnType ret = E_OK;
    ccp_reg_t capture_temp_value ={.ccpr_low = 0 , .ccpr_high = 0};
    if(NULL == capture_value)
    {
        ret = E_NOT_OK;
    }
    else
    {
        capture_temp_value.ccpr_low = CCPR1L;
        capture_temp_value.ccpr_high = CCPR1H;
        *capture_value = capture_temp_value.ccpr_16bit;
    }
    return ret;
}
#endif


void CCP1_ISR(void)
{
    CCP1_InterruptFlagClear();
    if(CCP1_InterruptHandler)
    {
        CCP1_InterruptHandler();
    }
    else
    {
        /*NOTHING*/
    }
}

void CCP2_ISR(void)
{
    CCP2_InterruptFlagClear();
    if(CCP2_InterruptHandler)
    {
        CCP2_InterruptHandler();
    }
    else
    {
        /*NOTHING*/
    }
}

/**
 * @breif enable interrupt for CCP1 or CCP2.
 * @param ccp_obj pointer points to ccp_t data.
 */
static void CCP_InterruptConfig(const ccp_t *ccp_obj)
{
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    CCP2_InterruptEnable();
    CCP2_InterruptFlagClear();
    CCP2_InterruptHandler = ccp_obj -> CCP2_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelEnable();
    if(INTERRUPT_HIGH_PRIORITY == ccp_obj -> ccp2_priority)
    {
        INTERRUPT_GlobalInterruptHighEnable();
        CCP2_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == ccp_obj -> ccp2_priority)
    {
        INTERRUPT_GlobalInterruptLowEnable();
        CCP2_LowPrioritySet();
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
        
#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    CCP1_InterruptEnable();
    CCP1_InterruptFlagClear();
    CCP1_InterruptHandler = ccp_obj -> CCP1_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelEnable();
    if(INTERRUPT_HIGH_PRIORITY == ccp_obj -> ccp1_priority)
    {
        INTERRUPT_GlobalInterruptHighEnable();
        CCP1_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == ccp_obj -> ccp1_priority)
    {
        INTERRUPT_GlobalInterruptLowEnable();
        CCP1_LowPrioritySet();
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
}

/**
 * @brief select which timer should be enabled 
 *        according to the choosen CCP module(CCP1 or CCP2).
 * @param ccp_obj pointer points to ccp_t data.
 */
static void CCP_CaptureModeTimerselect(const ccp_t *ccp_obj)
{
    if(CCP1_CCP2_TIMER3 == ccp_obj -> ccp_capture_timer)
    {
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == ccp_obj -> ccp_capture_timer)
    {
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == ccp_obj -> ccp_capture_timer)
    {
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else
    {
        /*NOTHING*/
    }
}