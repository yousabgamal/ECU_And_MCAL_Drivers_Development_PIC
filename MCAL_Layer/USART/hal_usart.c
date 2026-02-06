/* 
 * File:   hal_usart.c
 * Author: Yousab Gamal
 * Created on April 5, 2025, 2:24 PM
 */

#include "hal_usart.h"

#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*EUSART_TxInterruptHandler)(void) = NULL;
#endif
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*EUSART_RxInterruptHandler)(void) = NULL;
    static void(*EUSART_FramingErrorHandler)(void) = NULL;
    static void(*EUSART_OverrunErrorHandler)(void) = NULL;
#endif

static void EUSART_Baud_Rate_Calc(const usart_t *eusart);
static void EUSART_ASYNCH_TX_Init(const usart_t *eusart);
static void EUSART_ASYNCH_RX_Init(const usart_t *eusart);

/**
 * @brief initialize EUSART.
 * @param eusart pointer points to usart_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType EUSART_ASYNCH_Init(const usart_t *eusart)
{
    Std_ReturnType ret = E_OK;
    if(NULL == eusart)
    {
        ret = E_NOT_OK;
    }
    else
    {
        EUSART_MODULE_DISABLE();
        TRISCbits.RC7 = 1;
        TRISCbits.RC6 = 1;
        EUSART_Baud_Rate_Calc(eusart);
        EUSART_ASYNCH_TX_Init(eusart);
        EUSART_ASYNCH_RX_Init(eusart);
        EUSART_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief de initialize EUSART module.
 * @param eusart pointer points to usart_t data.
 * @return...
 *           E_OK: means function done without any errors.
 *           E_NOT_OK: means there is an error during run the function.
 */
Std_ReturnType EUSART_ASYNCH_DeInit(const usart_t *eusart)
{
    Std_ReturnType ret = E_OK;
    if(NULL == eusart)
    {
        ret = E_NOT_OK;
    }
    else
    {
        EUSART_MODULE_DISABLE();
    }
    return ret;
}

/**
 * @brief read the value in RCREG register and store it in data.
 * @param data is a char.
 */
void EUSART_ASYNCH_ReadByteBlocking(uint8 *data)
{
    while(!PIR1bits.RCIF);//blocking
    *data = RCREG;
}

/**
 * 
 * @param data is a char.
 */
void EUSART_ASYNCH_ReadByteNonBlocking(uint8 *data)
{
    if(1 == PIR1bits.RCIF)//non_blocking
    {
        *data = RCREG;
    }
    else
    {
        /*NOTHING*/
    }
}

/**
 * @brief restart the EUSART module.
 */
void EUSART_ASYNCH_RX_Restart(void)
{
    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
}

/**
 * @brief write data(char) in TXREG register.
 * @param data is a char. 
 */
void EUSART_ASYNCH_WriteByteBlocking(uint8 data)
{
    while(!TXSTAbits.TRMT);//bloking
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    EUSART_TX_InterruptEnable();
#endif
    TXREG = data;
}

/**
 * @brief write data(char) in TXREG register.
 * @param data is a char.
 */
void EUSART_ASYNCH_WriteByteNonBlocking(uint8 data)
{
    if(TXSTAbits.TRMT)
    {
        TXREG = data;
    }
    else
    {
        /*NOTHING*/
    }
}

/**
 * @brief write data(string) char by char in TXREG register.
 * @param data an array of characters.
 * @param string_length the length of the string.
 */
Std_ReturnType EUSART_ASYNCH_WriteStringNonBlocking(uint8 *data , uint16 string_length)
{
    Std_ReturnType ret = E_OK;
    uint16 l_char_counter = 0;
    if(NULL == data)
    {
        ret = E_NOT_OK;
    }
    else
    {
        for(l_char_counter = 0 ; l_char_counter < string_length ; l_char_counter++)
        {
            EUSART_ASYNCH_WriteByteNonBlocking(data[l_char_counter]); 
        }
    }
    return ret;
}

/**
 * @brief write data(string) char by char in TXREG register.
 * @param data an array of characters.
 * @param string_length the length of the string.
 */
Std_ReturnType EUSART_ASYNCH_WriteStringBlocking(uint8 *data , uint16 string_length)
{
    Std_ReturnType ret = E_OK;
    uint16 l_char_counter = 0;
    if(NULL == data)
    {
        ret = E_NOT_OK;
    }
    else
    {
        for(l_char_counter = 0 ; l_char_counter < string_length ; l_char_counter++)
        {
            EUSART_ASYNCH_WriteByteBlocking(data[l_char_counter]); 
        }
    }
    return ret;
}

/**
 * @brief calculate and initialize baud rate in the two registers...
 *        TXSTA(SYNC bit & BRGH bit) and BAUDCON(BRG16 bit).
 * @param eusart pointer points to usart_t data.
 */
static void EUSART_Baud_Rate_Calc(const usart_t *eusart)
{
    float Baud_Rate_Temp = 0;
    switch(eusart -> baudrate_gen_cfg)
    {
        case BAUDRATE_ASYNCH_8BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCH_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCH_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = (((_XTAL_FREQ / (float)eusart -> baudrate)/ 64)- 1);
            break;
        case BAUDRATE_ASYNCH_8BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCH_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCH_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = (((_XTAL_FREQ / (float)eusart -> baudrate)/ 16)- 1);
            break;
        case BAUDRATE_ASYNCH_16BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCH_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCH_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = (((_XTAL_FREQ / (float)eusart -> baudrate)/ 16)- 1);
            break;
        case BAUDRATE_ASYNCH_16BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCH_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCH_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = (((_XTAL_FREQ / (float)eusart -> baudrate)/ 4)- 1);
            break;  
        case BAUDRATE_SYNCH_8BIT:
            TXSTAbits.SYNC = EUSART_SYNCH_MODE;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = (((_XTAL_FREQ / (float)eusart -> baudrate)/ 4)- 1);
            break; 
        case BAUDRATE_SYNCH_16BIT:
            TXSTAbits.SYNC = EUSART_SYNCH_MODE;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = (((_XTAL_FREQ / (float)eusart -> baudrate)/ 4)- 1);
            break;   
        default:
            /*NOTHING*/
            break;
    }
    SPBRG = (uint8)((uint32)Baud_Rate_Temp );
    SPBRGH = (uint8)((uint32)Baud_Rate_Temp >> 8);
}

/**
 * @brief initialize TX with the desired values.
 * @param eusart pointer points to usart_t data.
 */
static void EUSART_ASYNCH_TX_Init(const usart_t *eusart)
{
    if(EUSART_ASYNCH_TX_ENABLE == eusart -> usart_tx_cfg.usart_tx_enable)
    {
        TXSTAbits.TXEN = EUSART_ASYNCH_TX_ENABLE;
        /*EUSART transmitter interrupt configuration*/
        if(EUSART_ASYNCH_INTERRUPT_TX_ENABLE == eusart -> usart_tx_cfg.usart_tx_interrupt_enable)
        {
            PIE1bits.TXIE = EUSART_ASYNCH_INTERRUPT_TX_ENABLE;
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            EUSART_TX_InterruptEnable();
            EUSART_TxInterruptHandler = eusart -> EUSART_TxInterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelEnable();
            if(INTERRUPT_HIGH_PRIORITY == eusart -> usart_tx_cfg.usart_tx_priority)
            {
                INTERRUPT_GlobalInterruptHighEnable();
                EUSART_TX_HighPrioritySet();
            }
            else if(INTERRUPT_LOW_PRIORITY == eusart -> usart_tx_cfg.usart_tx_priority)
            {
                INTERRUPT_GlobalInterruptLowEnable();
                EUSART_RX_LowPrioritySet();
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
        else if(EUSART_ASYNCH_INTERRUPT_TX_DISABLE == eusart -> usart_tx_cfg.usart_tx_interrupt_enable)
        {
            PIE1bits.TXIE = EUSART_ASYNCH_INTERRUPT_TX_DISABLE;
        }
        else
        {
            /*NOTHING*/
        }
        /*EUSART 9-bit transmitter configuration*/
        if(EUSART_ASYNCH_9BIT_TX_ENABLE == eusart -> usart_tx_cfg.usart_tx_9bit_enable)
        {
            TXSTAbits.TX9 = EUSART_ASYNCH_9BIT_TX_ENABLE;
        }
        else if(EUSART_ASYNCH_9BIT_TX_DISABLE == eusart -> usart_tx_cfg.usart_tx_9bit_enable)
        {
            TXSTAbits.TX9 = EUSART_ASYNCH_9BIT_TX_DISABLE;
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
 * @brief initialize RX with the desired values.
 * @param eusart
 */
static void EUSART_ASYNCH_RX_Init(const usart_t *eusart)
{
    if(EUSART_ASYNCH_RX_ENABLE == eusart -> usart_rx_cfg.usart_rx_enable)
    {
        RCSTAbits.CREN = EUSART_ASYNCH_RX_ENABLE;
        /*EUSART receiver interrupt configuration*/
        if(EUSART_ASYNCH_INTERRUPT_RX_ENABLE == eusart -> usart_rx_cfg.usart_rx_interrupt_enable)
        {
            PIE1bits.RCIE = EUSART_ASYNCH_INTERRUPT_RX_ENABLE;
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            EUSART_RX_InterruptEnable();
            EUSART_RxInterruptHandler = eusart -> EUSART_RxInterruptHandler;
            EUSART_FramingErrorHandler = eusart -> EUSART_FramingErrorHandler;
            EUSART_OverrunErrorHandler = eusart -> EUSART_OverrunErrorHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelEnable();
            if(INTERRUPT_HIGH_PRIORITY == eusart -> usart_rx_cfg.usart_rx_priority)
            {
                INTERRUPT_GlobalInterruptHighEnable();
                EUSART_RX_HighPrioritySet();
            }
            else if(INTERRUPT_LOW_PRIORITY == eusart -> usart_rx_cfg.usart_rx_priority)
            {
                INTERRUPT_GlobalInterruptLowEnable();
                EUSART_RX_LowPrioritySet();
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
        else if(EUSART_ASYNCH_INTERRUPT_RX_DISABLE == eusart -> usart_rx_cfg.usart_rx_interrupt_enable)
        {
            PIE1bits.RCIE = EUSART_ASYNCH_INTERRUPT_RX_DISABLE;
        }
        else
        {
            /*NOTHING*/
        }
        /*EUSART 9-bit receiver configuration*/
        if(EUSART_ASYNCH_9BIT_RX_ENABLE == eusart -> usart_rx_cfg.usart_rx_9bit_enable)
        {
            RCSTAbits.RX9 = EUSART_ASYNCH_9BIT_RX_ENABLE;
        }
        else if(EUSART_ASYNCH_9BIT_RX_DISABLE == eusart -> usart_rx_cfg.usart_rx_9bit_enable)
        {
            RCSTAbits.RX9 = EUSART_ASYNCH_9BIT_RX_DISABLE;
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

void EUSART_TX_ISR()
{
    EUSART_TX_InterruptDisable();
    if(EUSART_TxInterruptHandler)
    {
       EUSART_TxInterruptHandler();
    }
    else
    {
        /*NOTHING*/
    }
}

void EUSART_RX_ISR()
{
    if(EUSART_RxInterruptHandler)
    {
       EUSART_RxInterruptHandler();
    }
    else
    {
        /*NOTHING*/
    }
    if(EUSART_FramingErrorHandler)
    {
       EUSART_FramingErrorHandler();
    }
    else
    {
        /*NOTHING*/
    }
    if(EUSART_OverrunErrorHandler)
    {
       EUSART_OverrunErrorHandler();
    }
    else
    {
        /*NOTHING*/
    }
}