/* 
 * File:   hal_i2c.h
 * Author: Yousab Gamal(yous.gmm@gmail.com)
 *
 * Created on December 8, 2025, 1:39 PM
 */

#ifndef HAL_I2C_H
#define	HAL_I2C_H

/******************Section: Includes**********************/
#include "../GPIO/hal_gpio.h"
#include "../interrupt/mcal_internal_interrupt.h"
#include "../../MCAL_Layer/mcal_std_types.h"
#include "pic18f4620.h"

/******************Section: Macros Declarations***********/
                    /*Slew Rate Enable/Disable*/
#define I2C_SLEW_RATE_ENABLE    0
#define I2C_SLEW_RATE_DISABLE   1
                    /*SMBus Enable/Disable*/
#define I2C_SMBUS_ENABLE    1
#define I2C_SMBUS_DISABLE   0
                    /*Slave Mode Data/Address Indication*/
#define I2C_LAST_BYTE_DATA    1
#define I2C_LAST_BYTE_ADDRESS 0
                    /*Stop Condition Indication*/
#define I2C_STOP_BIT_DETECTED     1
#define I2C_STOP_BIT_NOT_DETECTED 0
                    /*Start Condition Indication*/
#define I2C_START_BIT_DETECTED     1 
#define I2C_START_BIT_NOT_DETECTED 0
                    /*I2C Master/Slave Mode*/
#define MSSP_I2C_MASTER_MODE 1
#define MSSP_I2C_SLAVE_MODE  0 
                    /*Master Synch Serial Port Mode Select*/
#define I2C_SLAVE_MODE_7BIT_ADDRESS                 0X06U
#define I2C_SLAVE_MODE_10BIT_ADDRESS                0X07U
#define I2C_SLAVE_MODE_7BIT_ADDRESS_SS_INT_ENABLE   0X0EU
#define I2C_SLAVE_MODE_10BIT_ADDRESS_SS_INT_ENABLE  0X0FU
#define I2C_MASTER_MODE_DEFINED_CLOCK               0X08U
#define I2C_MASTER_MODE_FIRMWARE_CONTROLLED         0X0BU
                    /*General Call Enable/Disable*/
#define I2C_GENERAL_CALL_ENABLE  1
#define I2C_GENERAL_CALL_DISABLE 0
                    /*Master Mode Receive Enable/Disable*/
#define I2C_MASTER_RECEIVE_ENABLE  1
#define I2C_MASTER_RECEIVE_DISABLE 0
                    /*Acknowledge Status bit (Master Transmit mode only)*/
#define I2C_ACK_RECEIVE_FROM_SLAVE     0
#define I2C_ACK_NOT_RECEIVE_FROM_SLAVE 1
                    /*Acknowledge Status bit (Master Receive mode only)*/
#define I2C_MASTER_SEND_ACK      0
#define I2C_MASTER_SEND_NOT_ACK  1

/******************Section: Macros Functions Declarations*/
                    /*Slew Rate Enable/Disable*/
#define I2C_SLEW_RATE_ENABLE_CFG()  (SSPSTATbits.SMP = I2C_SLEW_RATE_ENABLE)
#define I2C_SLEW_RATE_DISABLE_CFG() (SSPSTATbits.SMP = I2C_SLEW_RATE_DISABLE)
                    /*SMBus Enable/Disable*/
#define I2C_SMBUS_ENABLE_CFG()      (SSPSTATbits.CKE = I2C_SMBUS_ENABLE)
#define I2C_SMBUS_DISABLE_CFG()     (SSPSTATbits.CKE = I2C_SMBUS_DISABLE)
                    /*General Call Enable/Disable*/
#define I2C_GENERAL_CALL_ENABLE_CFG()   (SSPCON2bits.GCEN = I2C_GENERAL_CALL_ENABLE)
#define I2C_GENERAL_CALL_DISABLE_CFG()  (SSPCON2bits.GCEN = I2C_GENERAL_CALL_DISABLE)
                    /*Master Mode Receive Enable/Disable*/
#define I2C_MASTER_RECEIVE_ENABLE_CFG() (SSPCON2bits.RCEN = I2C_MASTER_RECEIVE_ENABLE)
#define I2C_MASTER_RECEIVE_DISABLE_CFG()(SSPCON2bits.RCEN = I2C_MASTER_RECEIVE_DISABLE)
                    /*Master Synch Serial Port Enable/Disable*/
#define MSSP_MODULE_ENABLE_CFG()     (SSPCON1bits.SSPEN = 1)
#define MSSP_MODULE_DISABLE_CFG()    (SSPCON1bits.SSPEN = 0)

/******************Section: Data Types Declarations*******/
typedef struct{
    uint8 i2c_mode_cfg;           /*Master synch serial port mode select*/
    uint8 i2c_mode : 1;           /*Master mode or slave mode*/
    uint8 i2c_slew_rate : 1;      /*Slew rate enable or disable(I2C speed)*/
    uint8 i2c_SMBus_control : 1;  /*SMBus enable or disable*/
    uint8 i2c_general_call : 1;   /*General call enable or disable*/
    uint8 i2c_master_rec_mode : 1;/*Master receive enable or disable*/
    uint8 i2c_reserved : 3;
#if MSSP_I2C_INTERRUPT_ENABLE_FEATURE == INTERRUPT_FEATURE_ENABLE    
    interrupt_priority_cfg mssp_i2c_priority;  
    interrupt_priority_cfg mssp_i2c_bus_col_priority;  
#endif 
}i2c_configs_t;

typedef struct{
    uint32 i2c_clock; /*Master clock frequency*/
#if MSSP_I2C_INTERRUPT_ENABLE_FEATURE == INTERRUPT_FEATURE_ENABLE    
    void (*I2C_Report_Write_Collision)(void);
    void (*I2C_Deafult_Interrupthandler)(void);  
    void (*I2C_Report_Receive_Overflow)(void);
#endif    
    i2c_configs_t i2c_config;
}mssp_i2c_t;

/******************Section: Functions Declarations********/
Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2C_Deinit(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Start_Blocking(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start_Blocking(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Stop_Blocking(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSSP_I2C_Master_Write_blocking(const mssp_i2c_t * i2c_obj , uint8 i2c_data , uint8 * ack);
Std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t * i2c_obj , uint8 ack , uint8 * i2c_data);

#endif	/* HAL_I2C_H */

