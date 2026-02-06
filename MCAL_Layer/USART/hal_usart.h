/* 
 * File:   hal_usart.h
 * Author: Yousab Gamal
 * Created on April 5, 2025, 2:24 PM
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/******************Section: Includes**********************/
#include <xc.h>
#include "hal_usart_cfg.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"

/******************Section: Macros Declarations***********/
#define EUSART_SYNCH_MODE              1
#define EUSART_ASYNCH_MODE             0

#define EUSART_ASYNCH_HIGH_SPEED       1
#define EUSART_ASYNCH_LOW_SPEED        0

#define EUSART_16BIT_BAUDRATE_GEN      1
#define EUSART_8BIT_BAUDRATE_GEN       0

#define EUSART_ASYNCH_TX_ENABLE             1
#define EUSART_ASYNCH_TX_DISABLE            0
#define EUSART_ASYNCH_9BIT_TX_ENABLE        1
#define EUSART_ASYNCH_9BIT_TX_DISABLE       0
#define EUSART_ASYNCH_INTERRUPT_TX_ENABLE   1
#define EUSART_ASYNCH_INTERRUPT_TX_DISABLE  0

#define EUSART_ASYNCH_RX_ENABLE             1
#define EUSART_ASYNCH_RX_DISABLE            0
#define EUSART_ASYNCH_9BIT_RX_ENABLE        1
#define EUSART_ASYNCH_9BIT_RX_DISABLE       0
#define EUSART_ASYNCH_INTERRUPT_RX_ENABLE   1
#define EUSART_ASYNCH_INTERRUPT_RX_DISABLE  0

#define EUSART_FRAMING_ERROR_DETECTED       1
#define EUSART_FRAMING_ERROR_CLEARED        0

#define EUSART_OVERRUN_ERROR_DETECTED       1
#define EUSART_OVERRUN_ERROR_CLEARED        0

/******************Section: Macros Functions Declarations*/
#define EUSART_MODULE_ENABLE()       (RCSTAbits.SPEN = 1)
#define EUSART_MODULE_DISABLE()      (RCSTAbits.SPEN = 0)

/******************Section: Data Types Declarations*******/
typedef enum{
    BAUDRATE_ASYNCH_8BIT_LOW_SPEED,
    BAUDRATE_ASYNCH_8BIT_HIGH_SPEED,
    BAUDRATE_ASYNCH_16BIT_LOW_SPEED,
    BAUDRATE_ASYNCH_16BIT_HIGH_SPEED,
    BAUDRATE_SYNCH_8BIT,
    BAUDRATE_SYNCH_16BIT,
}baudrate_gen_t;

typedef struct{
    uint8 usart_tx_reserved         : 5;
    uint8 usart_tx_enable           : 1;
    uint8 usart_tx_9bit_enable      : 1;
    uint8 usart_tx_interrupt_enable : 1;
    interrupt_priority_cfg usart_tx_priority;
}usart_tx_cfg_t;

typedef struct{
    uint8 usart_rx_reserved         : 5;
    uint8 usart_rx_enable           : 1;
    uint8 usart_rx_9bit_enable      : 1;
    uint8 usart_rx_interrupt_enable : 1;
    interrupt_priority_cfg usart_rx_priority;
}usart_rx_cfg_t;

typedef union{
    struct{
        uint8 usart_rx_reserved : 6;
        uint8 usart_ferr        : 1;
        uint8 usart_oerr        : 1;
    };
    uint8 status;
}usart_error_status_t;

typedef struct{
    uint32 baudrate;
    baudrate_gen_t baudrate_gen_cfg;
    usart_tx_cfg_t usart_tx_cfg;
    usart_rx_cfg_t usart_rx_cfg;
    usart_error_status_t error_status;
    void(*EUSART_TxInterruptHandler)(void);
    void(*EUSART_RxInterruptHandler)(void);
    void(*EUSART_FramingErrorHandler)(void);
    void(*EUSART_OverrunErrorHandler)(void);
}usart_t;

/******************Section: Functions Declarations********/
Std_ReturnType EUSART_ASYNCH_Init(const usart_t *eusart);
Std_ReturnType EUSART_ASYNCH_DeInit(const usart_t *eusart);
void EUSART_ASYNCH_ReadByteBlocking(uint8 *data);
void EUSART_ASYNCH_ReadByteNonBlocking(uint8 *data);
void EUSART_ASYNCH_RX_Restart(void);
void EUSART_ASYNCH_WriteByteBlocking(uint8 data);
void EUSART_ASYNCH_WriteByteNonBlocking(uint8 data);
Std_ReturnType EUSART_ASYNCH_WriteStringNonBlocking(uint8 *data , uint16 string_length);
Std_ReturnType EUSART_ASYNCH_WriteStringBlocking(uint8 *data , uint16 string_length);

#endif	/* HAL_USART_H */

