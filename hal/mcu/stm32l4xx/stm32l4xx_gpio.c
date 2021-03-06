/**
 *******************************************************************************
 * @file       stm32l4xx_gpio.c                                                *
 * @author     yearnext                                                        *
 * @version    1.0.0                                                           *
 * @date       2018-12-18                                                      *
 * @brief      stm32l476 gpio driver component                                 *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         * 
 *******************************************************************************
 * @note                                                                       *
 * 1.XXXXX                                                                     *
 *******************************************************************************
 */
 
/**
 * @defgroup stm32l4xx gpio driver component
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_msp.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       GPIO 时钟设置函数
 * @param       [in/out]  port         GPIO 端口
 * @param       [in/out]  cmd          0:关闭 1：开启
 * @return      void
 * @note        None
 *******************************************************************************
 */
__STATIC_INLINE
void MSP_GPIO_CLK_CMD(GPIO_TypeDef *port, uint8_t cmd)
{
    uint32_t mask;

    if (port == GPIOA)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOA;
    }
    else if (port == GPIOB)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOB;
    }
    else if (port == GPIOC)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOC;
    }
#ifdef GPIOD
    else if (port == GPIOD)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOD;
    }
#endif
    
#ifdef GPIOE
    else if (port == GPIOE)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOE;
    }
#endif

#ifdef GPIOF
    else if (port == GPIOF)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOF;
    }
#endif

#ifdef GPIOG
    else if (port == GPIOG)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOG;
    }
#endif

#ifdef GPIOH
    else if (port == GPIOH)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOH;
    }
#endif

#ifdef GPIOI
    else if (port == GPIOI)
    {
        mask = LL_AHB2_GRP1_PERIPH_GPIOI;
    }
#endif

    if (cmd)
    {
        LL_AHB2_GRP1_EnableClock(mask);
    }
    else
    {
        LL_AHB2_GRP1_DisableClock(mask);
    }
}

/**
 *******************************************************************************
 * @brief       GPIO 基地址获取函数
 * @param       [in/out]  port         GPIO 端口
 * @return      GPIO_TypeDef*          基地址指针
 * @note        None
 *******************************************************************************
 */
__STATIC_INLINE
GPIO_TypeDef *MSP_GPIO_Base_Get(uint16_t port)
{
    return (GPIO_TypeDef *)(AHB2PERIPH_BASE + port * 0x400UL);
}

/**
 *******************************************************************************
 * @brief       GPIO 引脚掩码获取函数
 * @param       [in/out]  pin          GPIO 引脚
 * @return      uint32_t               引脚掩码
 * @note        None
 *******************************************************************************
 */
__STATIC_INLINE
uint32_t MSP_GPIO_PinMask_Get(uint16_t pin)
{
    return _bv(pin);
}

/**
 *******************************************************************************
 * @brief       GPIO 模式掩码获取函数
 * @param       [in/out]  pin          GPIO 模式
 * @return      uint32_t               模式掩码
 * @note        None
 *******************************************************************************
 */
__STATIC_INLINE
uint32_t MSP_GPIO_ModeMask_Get(uint16_t mode)
{
    const uint32_t modeParam[] = {LL_GPIO_MODE_INPUT, LL_GPIO_MODE_OUTPUT, 0, LL_GPIO_MODE_INPUT, LL_GPIO_MODE_ANALOG};
    
    return modeParam[mode];
}

#ifdef ENABLE_HAL_GPIO_DRIVER
/**
 *******************************************************************************
 * @brief       GPIO 端口配置函数
 * @param       [in/out]  dev          设备句柄
 * @param       [in/out]  pin          GPIO 引脚
 * @param       [in/out]  mode         GPIO 端口初始化状态
 * @return      void
 * @note        None
 *******************************************************************************
 */
static void pin_ops_init(HAL_Device_t *dev, uint32_t pin, uint16_t mode)
{
    GPIO_TypeDef *port = MSP_GPIO_Base_Get(HAL_PORT(pin));
    uint32_t pinMask = MSP_GPIO_PinMask_Get(HAL_PIN(pin));
    uint32_t modeParam = MSP_GPIO_ModeMask_Get(mode);
    
    MSP_GPIO_Config(port, pinMask, modeParam);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口写入函数
 * @param       [in/out]  dev          设备句柄
 * @param       [in/out]  pin          GPIO 引脚
 * @param       [in/out]  value        写入值
 * @return      void
 * @note        None
 *******************************************************************************
 */
static void pin_ops_write(HAL_Device_t *dev, uint32_t pin, uint32_t value)
{
    GPIO_TypeDef *port = MSP_GPIO_Base_Get(HAL_PORT(pin));
    uint32_t pinMask = MSP_GPIO_PinMask_Get(HAL_PIN(pin));

    if (value)
    {
        WRITE_REG(port->BSRR, pinMask);
    }
    else
    {
        WRITE_REG(port->BRR, pinMask);
    }
}

/**
 *******************************************************************************
 * @brief       GPIO 端口电平翻转函数
 * @param       [in/out]  dev          设备句柄
 * @param       [in/out]  pin          GPIO 引脚
 * @param       [in/out]  value        写入值
 * @return      void
 * @note        None
 *******************************************************************************
 */
static void pin_ops_toggle(HAL_Device_t *dev, uint32_t pin)
{
    GPIO_TypeDef *port = MSP_GPIO_Base_Get(HAL_PORT(pin));
    uint32_t pinMask = MSP_GPIO_PinMask_Get(HAL_PIN(pin));

    WRITE_REG(port->ODR, READ_REG(port->ODR) ^ pinMask);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口读取函数
 * @param       [in/out]  dev          设备句柄
 * @param       [in/out]  pin          GPIO 引脚
 * @return      uint32_t               读取值
 * @note        None
 *******************************************************************************
 */
static uint32_t pin_ops_read(HAL_Device_t *dev, uint32_t pin)
{
    GPIO_TypeDef *port = MSP_GPIO_Base_Get(HAL_PORT(pin));
    uint32_t pinMask = MSP_GPIO_PinMask_Get(HAL_PIN(pin));

    return (READ_BIT(port->ODR, pinMask) == pinMask);
}
#endif

/* Exported functions --------------------------------------------------------*/
__INLINE
void MSP_Pin_Init(uint16_t port, uint32_t pin, uint16_t mode)
{
    GPIO_TypeDef *handle = MSP_GPIO_Base_Get(port);
    uint32_t pinMask = MSP_GPIO_PinMask_Get(pin);
    uint32_t modeParam = MSP_GPIO_ModeMask_Get(mode);
    
    MSP_GPIO_Config(handle, pinMask, modeParam);
}

__INLINE
void MSP_Pin_Set(uint16_t port, uint32_t pin)
{
    GPIO_TypeDef *handle = MSP_GPIO_Base_Get(port);
    uint32_t pinMask = MSP_GPIO_PinMask_Get(pin);

    _set_reg(handle->BSRR, pinMask);
}

__INLINE
void MSP_Pin_Clr(uint16_t port, uint32_t pin)
{
    GPIO_TypeDef *handle = MSP_GPIO_Base_Get(port);
    uint32_t pinMask = MSP_GPIO_PinMask_Get(pin);

    _clr_reg(handle->BRR, pinMask);
}

__INLINE
void MSP_Pin_Toggle(uint16_t port, uint32_t pin)
{
    GPIO_TypeDef *handle = MSP_GPIO_Base_Get(port);
    uint32_t pinMask = MSP_GPIO_PinMask_Get(pin);
    
    if (_get_reg(handle->IDR, pinMask))
    {
        _clr_reg(handle->BRR, pinMask);
    }
    else
    {
        _set_reg(handle->BSRR, pinMask);
    }
}

__INLINE
uint16_t MSP_Pin_Read(uint16_t port, uint32_t pin)
{
    GPIO_TypeDef *handle = MSP_GPIO_Base_Get(port);
    uint32_t pinMask = MSP_GPIO_PinMask_Get(pin);
    
    return _read_reg(handle->IDR & pinMask) ? (1) : (0);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口置位函数
 * @param       [in/out]  port         GPIO 端口
 * @param       [in/out]  pin          GPIO 引脚
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void MSP_GPIO_Set(GPIO_TypeDef *port, uint32_t pin)
{
    LL_GPIO_SetOutputPin(port, pin);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口复位函数
 * @param       [in/out]  port         GPIO 端口
 * @param       [in/out]  pin          GPIO 引脚
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void MSP_GPIO_Clr(GPIO_TypeDef *port, uint32_t pin)
{
    LL_GPIO_ResetOutputPin(port, pin);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口状态获取函数
 * @param       [in/out]  port         GPIO 端口
 * @param       [in/out]  pin          GPIO 引脚
 * @return      [in/out]  state        GPIO 引脚状态
 * @note        None
 *******************************************************************************
 */
uint32_t MSP_GPIO_Get(GPIO_TypeDef *port, uint32_t pin)
{
    return LL_GPIO_IsInputPinSet(port, pin);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口写入函数
 * @param       [in/out]  port         GPIO 端口
 * @param       [in/out]  wr           GPIO 端口写入数据
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void MSP_GPIO_Write(GPIO_TypeDef *port, uint32_t wr)
{
    LL_GPIO_WriteOutputPort(port, wr);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口读取函数
 * @param       [in/out]  port         GPIO 端口
 * @return      [in/out]  rd           GPIO 端口读取数据
 * @note        None
 *******************************************************************************
 */
uint32_t MSP_GPIO_Read(GPIO_TypeDef *port)
{
    return LL_GPIO_ReadInputPort(port) ? (1) : (0);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口配置函数
 * @param       [in/out]  port         GPIO 端口
 * @param       [in/out]  pin          GPIO 引脚
 * @param       [in/out]  mode         GPIO 端口初始化状态
 * @return      void
 * @note        None
 *******************************************************************************
 */
void MSP_GPIO_Config(GPIO_TypeDef *port, uint32_t pin, uint32_t mode)
{
    LL_GPIO_InitTypeDef Init;

    MSP_GPIO_CLK_CMD(port, 1);
    
    LL_GPIO_ResetOutputPin(port, pin);

    Init.Pin        = pin;
    Init.Mode       = mode;
    Init.Speed      = LL_GPIO_SPEED_FREQ_LOW;
    Init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    Init.Pull       = LL_GPIO_PULL_NO;
    
    LL_GPIO_Init(port, &Init);
}

/**
 *******************************************************************************
 * @brief       GPIO 端口初始化函数
 * @param       void
 * @return      void
 * @note        None
 *******************************************************************************
 */
void MSP_GPIO_Init(void)
{
#ifdef ENABLE_HAL_GPIO_DRIVER
    HAL_Port_Interface_t PinOps;

    PinOps.Init   = pin_ops_init;
    PinOps.Read   = pin_ops_read;
    PinOps.Write  = pin_ops_write;
    PinOps.Toggle = pin_ops_toggle;
    
    HAL_Pin_Register(&PinOps, NULL);
#endif
}

/** @}*/     /** stm32l4xx gpio driver component */

/**********************************END OF FILE*********************************/
