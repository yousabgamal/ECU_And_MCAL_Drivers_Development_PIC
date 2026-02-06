#include "hal_i2c.h"

                    /*Static functions declaration*/
static inline void MSSP_I2C_Mode_GPIO_CFG(void);
static inline void I2C_Master_Mode_clock_Congigrations(const mssp_i2c_t * i2c_obj);
static inline void I2C_Slew_Control(const mssp_i2c_t * i2c_obj);
static inline void I2C_SMBus_Enable_Or_Disable(const mssp_i2c_t * i2c_obj);
static inline void MSSP_I2C_Interrupt_Configrations(const mssp_i2c_t * i2c_obj);
#if MSSP_I2C_INTERRUPT_ENABLE_FEATURE == INTERRUPT_FEATURE_ENABLE
    static void(*I2C_Report_Write_Collision_InterruptHandler)(void);
    static void(*I2C_Deafult_Interrupthandler)(void);
    static void(*I2C_Report_Receive_Overflow_InterruptHandler)(void);
#endif                    

                    /*Other functions declaration*/
/**
 * @brief Initialize I2C communication 
 * @param i2c_obj : Pointer points to data(mssp_i2c_t type)
 *                  that includes all the specifications of I2C mode
 * @return E_OK if the function implements successfully  
 *         E_NOT_OK if the retrieved pointer is NULL  
 */
Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t * i2c_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        MSSP_MODULE_DISABLE_CFG();
        if(MSSP_I2C_MASTER_MODE == i2c_obj->i2c_config.i2c_mode)
        {
            I2C_Master_Mode_clock_Congigrations(i2c_obj);
        }
        else if(MSSP_I2C_SLAVE_MODE == i2c_obj->i2c_config.i2c_mode)
        {
            if(I2C_GENERAL_CALL_ENABLE == i2c_obj->i2c_config.i2c_general_call)
            {
                I2C_GENERAL_CALL_ENABLE_CFG();
            }
            else if(I2C_GENERAL_CALL_DISABLE == i2c_obj->i2c_config.i2c_general_call)
            {
                I2C_GENERAL_CALL_DISABLE_CFG();
            }
            else
            {
                /*Nothing*/
            }
        }
        else
        {
            /*Nothing*/
        }
        MSSP_I2C_Mode_GPIO_CFG();
        I2C_Slew_Control(i2c_obj);
        I2C_SMBus_Enable_Or_Disable(i2c_obj);
#if MSSP_I2C_INTERRUPT_ENABLE_FEATURE == INTERRUPT_FEATURE_ENABLE        
        MSSP_I2C_Interrupt_Configrations(i2c_obj);
#endif        
        MSSP_MODULE_ENABLE_CFG();
    }
    return ret;
}

/**
 * @brief Deinitialize I2C communication 
 * @param i2c_obj : Pointer points to data(mssp_i2c_t type)
 *                  that includes all the specifications of I2C mode
 * @return E_OK if the function implements successfully  
 *         E_NOT_OK if the retrieved pointer is NULL  
 */
Std_ReturnType MSSP_I2C_Deinit(const mssp_i2c_t * i2c_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        MSSP_MODULE_DISABLE_CFG();
#if MSSP_I2C_INTERRUPT_ENABLE_FEATURE == INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_InterruptDisable();
        MSSP_I2C_BUS_COL_InterruptDisable();     
#endif 
    }
    return ret;
}

/**
 * @brief Initiate the start condition on the SCL and SDA
 * @param i2c_obj : Pointer points to data(mssp_i2c_t type)
 *                  that includes all the specifications of I2C mode
 * @return E_OK if the function implements successfully  
 *         E_NOT_OK if the retrieved pointer is NULL  
 */
Std_ReturnType MSSP_I2C_Master_Send_Start_Blocking(const mssp_i2c_t * i2c_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        SSPCON2bits.SEN = 1;/*Automatically cleared by hardware*/
        while(SSPCON2bits.SEN);/*Wait for the completion of the start condition*/
        PIR1bits.SSPIF = 0;
        if(I2C_START_BIT_DETECTED == SSPSTATbits.S)
        {
            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief Initiate repeated start condition on the SCL and SDA
 * @param i2c_obj : Pointer points to data(mssp_i2c_t type)
 *                  that includes all the specifications of I2C mode
 * @return E_OK if the function implements successfully  
 *         E_NOT_OK if the retrieved pointer is NULL  
 */
Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start_Blocking(const mssp_i2c_t * i2c_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        SSPCON2bits.RSEN = 1;/*Automatically cleared by hardware*/
        while(SSPCON2bits.RSEN);
        PIR1bits.SSPIF = 0;
    }
    return ret;
}

/**
 * @brief Initiate the stop condition on the SCL and SDA
 * @param i2c_obj : Pointer points to data(mssp_i2c_t type)
 *                  that includes all the specifications of I2C mode
 * @return E_OK if the function implements successfully  
 *         E_NOT_OK if the retrieved pointer is NULL  
 */
Std_ReturnType MSSP_I2C_Master_Send_Stop_Blocking(const mssp_i2c_t * i2c_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        SSPCON2bits.PEN = 1;/*Automatically cleared by hardware*/
        while(SSPCON2bits.PEN);
        PIR1bits.SSPIF = 0;
        if(I2C_STOP_BIT_DETECTED == SSPSTATbits.P)
        {
            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}
/**
 * @brief Write a single byte of data to a specific I2C device 
 * @param i2c_obj : Pointer points to data(mssp_i2c_t type)
 *                  that includes all the specifications of I2C mode
 * @param i2c_data : The data wrote on the bus 
 * @param ack : Pointer to get the state of received data from the slave
 * @return E_OK if the function implements successfully  
 *         E_NOT_OK if the retrieved pointer is NULL  
 */
Std_ReturnType MSSSP_I2C_Master_Write_blocking(const mssp_i2c_t * i2c_obj , uint8 i2c_data , uint8 * ack)
{
    Std_ReturnType ret = E_OK;
    if((NULL == i2c_obj) || (NULL == ack))
    {
        ret = E_NOT_OK;
    }
    else
    {
        SSPBUF = i2c_data;
        while(!PIR1bits.SSPIF);
        PIR1bits.SSPIF = 0;
        if(I2C_ACK_RECEIVE_FROM_SLAVE == SSPCON2bits.ACKSTAT)
        {
            *ack = I2C_ACK_RECEIVE_FROM_SLAVE;
        }
        else
        {
            *ack = I2C_ACK_NOT_RECEIVE_FROM_SLAVE;
        }
    }
    return ret;
}

/**
 * @brief Read a single byte of data to a specific I2C device 
 * @param i2c_obj : Pointer points to data(mssp_i2c_t type)
 *                  that includes all the specifications of I2C mode
 * @param ack : The state that determines if data received 
 *              successfully or not
 * @param i2c_data : Pointer points to the data(uint8 type) retrieved 
 *                   from the slave 
 * @return E_OK if the function implements successfully  
 *         E_NOT_OK if any of the retrieved pointers are NULL  
 */
Std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t * i2c_obj , uint8 ack , uint8 * i2c_data)
{
    Std_ReturnType ret = E_OK;
    if((NULL == i2c_obj) || (NULL == i2c_data))
    {
        ret = E_NOT_OK;
    }
    else
    {
        I2C_MASTER_RECEIVE_ENABLE_CFG();
        while(!SSPSTATbits.BF);
        *i2c_data = SSPBUF;
        if(I2C_MASTER_SEND_ACK == ack)
        {
            SSPCON2bits.ACKDT = 0;
        }
        else if(I2C_MASTER_SEND_NOT_ACK == ack)
        {
            SSPCON2bits.ACKDT = 1;
        }
        else
        {
            /*Nothing*/
        }
        SSPCON2bits.ACKEN = 1;/*Automatically cleared by hardware*/
    }
    return ret;
}

void MSSP_I2C_ISR(void)
{
    MSSP_I2C_InterruptFlagClear();
    if(NULL != I2C_Deafult_Interrupthandler)
    {
        I2C_Deafult_Interrupthandler();
    }
    else
    {
        /*Nothing*/
    }
}
void MSSP_I2C_BUS_COL_ISR(void)
{
    MSSP_I2C_BUS_COL_InterruptFlagClear();
    if(NULL != I2C_Report_Write_Collision_InterruptHandler)
    {
        I2C_Report_Write_Collision_InterruptHandler();
    }
    else
    {
        /*Nothing*/
    }
}
static inline void MSSP_I2C_Mode_GPIO_CFG(void)
{
    TRISCbits.TRISC3 = 1;/*Serial clock (SCL) is Input*/
    TRISCbits.TRISC4 = 1;/*Serial data (SDA) is Input*/
}
static inline void I2C_Master_Mode_clock_Congigrations(const mssp_i2c_t * i2c_obj)
{
    SSPCON1bits.SSPM = I2C_MASTER_MODE_DEFINED_CLOCK;
    SSPADD = (uint8)(((_XTAL_FREQ / 4.0) / i2c_obj->i2c_clock) - 1);
}
static inline void I2C_Slew_Control(const mssp_i2c_t * i2c_obj)
{
    if(I2C_SLEW_RATE_ENABLE == i2c_obj->i2c_config.i2c_slew_rate)
    {
        I2C_SLEW_RATE_ENABLE_CFG();
    }
    else if(I2C_SLEW_RATE_DISABLE == i2c_obj->i2c_config.i2c_slew_rate)
    {
        I2C_SLEW_RATE_DISABLE_CFG();
    }
    else
    {
        /*Nothing*/
    }
}
static inline void I2C_SMBus_Enable_Or_Disable(const mssp_i2c_t * i2c_obj)
{
    if(I2C_SMBUS_ENABLE == i2c_obj->i2c_config.i2c_SMBus_control)
    {
        I2C_SMBUS_ENABLE_CFG();
    }
    else if(I2C_SMBUS_DISABLE == i2c_obj->i2c_config.i2c_SMBus_control)
    {
        I2C_SMBUS_DISABLE_CFG();
    }
    else
    {
        /*Nothing*/
    }
}
static inline void MSSP_I2C_Interrupt_Configrations(const mssp_i2c_t * i2c_obj)
{
#if MSSP_I2C_INTERRUPT_ENABLE_FEATURE == INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_InterruptEnable();
        MSSP_I2C_BUS_COL_InterruptEnable();
        MSSP_I2C_InterruptFlagClear();
        MSSP_I2C_BUS_COL_InterruptFlagClear();
        I2C_Report_Write_Collision_InterruptHandler = i2c_obj->I2C_Report_Write_Collision;
        I2C_Deafult_Interrupthandler = i2c_obj->I2C_Deafult_Interrupthandler;
        I2C_Report_Receive_Overflow_InterruptHandler = i2c_obj->I2C_Report_Receive_Overflow;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_config.mssp_i2c_priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            MSSP_I2C_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_config.mssp_i2c_priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            MSSP_I2C_LowPrioritySet();
        }
        else
        {
            /*NOTHING*/
        }
        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_config.mssp_i2c_bus_col_priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            MSSP_I2C_BUS_COL_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_config.mssp_i2c_bus_col_priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            MSSP_I2C_BUS_COL_LowPrioritySet();
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

