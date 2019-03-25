/**
 *******************************************************************************
 *                 Copyright (C) 2017 - 2019  Accumulate Team                  *
 *                                                                             *
 *      Licensed under the Apache License, Version 2.0 (the "License");        *
 *      you may not use this file except in compliance with the License.       *
 *      You may obtain a copy of the License at                                *
 *                 http://www.apache.org/licenses/LICENSE-2.0                  *
 *                                                                             *
 *   Unless required by applicable law or agreed to in writing, software       *
 *   distributed under the License is distributed on an "AS IS" BASIS,         *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *   See the License for the specific language governing permissions and       *
 *   limitations under the License.                                            *
 *******************************************************************************
 * @file       hal_def.h                                                       *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2019-03-09                                                      *
 * @brief      hal device define head file                                     *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1. 20190309    �����ļ�"hal_def.h"                                          *
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_DEVICE_DEFINE_H__
#define __HAL_DEVICE_DEFINE_H__

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "compiler.h"
#include "fw_server.h"

/* Exported macro ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        HAL �豸����
 *******************************************************************************
 */
//! �����豸����
enum HAL_DEVICE_TYPE
{
    HAL_DEVICE_MCU = 0,
    HAL_DEVICE_PIN,
    HAL_DEVICE_ADC,
    HAL_DEVICE_TIMER,
    HAL_DEVICE_PWM,
    HAL_DEVICE_SERIAL,
    HAL_DEVICE_IIC,
    HAL_DEVICE_SPI,
    HAL_DEVICE_HALL,
    HAL_DEVICE_FLASH,
    HAL_DEVICE_VEEPROM,
    HAL_DEVICE_MAX,
};

/**
 *******************************************************************************
 * @brief        HAL �豸������
 *******************************************************************************
 */
//! �����豸�����ַ���
#define HAL_PIN_STR                                                       "GPIO"
#define HAL_ADC_STR                                                        "ADC"
#define HAL_TIMER_STR                                                    "TIMER"
#define HAL_PWM_STR                                                        "PWM"
#define HAL_SERIAL_STR                                                  "SERIAL"
#define HAL_IIC_STR                                                        "IIC"
#define HAL_SPI_STR                                                        "SPI"
#define HAL_HALL_STR                                                      "HALL"
#define HAL_FLASH_STR                                                    "FLASH"
#define HAL_VEEPROM_STR                                                "VEEPROM"

/**
 *******************************************************************************
 * @brief        HAL �豸״̬
 *******************************************************************************
 */
enum __HAL_DEVICE_STATE
{
    HAL_DEVICE_INIT = 0,
    HAL_DEVICE_IDLE,
    HAL_DEVICE_READY,
    HAL_DEVICE_BUSY,
    HAL_DEVICE_SLEEP,
};

/**
 *******************************************************************************
 * @brief        HAL IRQ�����������
 *******************************************************************************
 */
typedef void (*HAL_IrqHandle)(void *param);
typedef struct
{
    HAL_IrqHandle Handle;
    void          *Param;
}HAL_IRQ_t;

/**
 *******************************************************************************
 * @brief        HAL ͨ���豸�����ӿ�
 *******************************************************************************
 */
struct HAL_DEVICE;
typedef struct
{
    uint16_t (*Init)(struct HAL_DEVICE *dev, size_t param);
    uint16_t (*Fini)(struct HAL_DEVICE *dev);

    uint16_t (*Open)(struct HAL_DEVICE *dev, uint16_t flag);
    uint16_t (*Close)(struct HAL_DEVICE *dev);
    
    uint16_t (*Write)(struct HAL_DEVICE *dev, uint16_t pos, uint8_t *buf, uint16_t len);
    uint16_t (*Read)(struct HAL_DEVICE *dev, uint16_t pos, uint8_t *buf, uint16_t len);
    
    uint16_t (*Control)(struct HAL_DEVICE *dev, uint8_t cmd, void *args);
}HAL_Interface_t;

/**
 *******************************************************************************
 * @brief        HAL �豸���
 *******************************************************************************
 */
typedef struct HAL_DEVICE
{
    //! �豸����
    uint8_t Type;
    //! �豸״̬
    uint8_t State;
    //! �豸��ʶ
    uint16_t Flag;
    //! �豸����
    FwList_t List;
    //! �豸�����ӿ�
    HAL_Interface_t Ops;
    //! �豸����
    char *Name;
    //! �û�����
    void *UserData;
}HAL_Device_t;

/**
 *******************************************************************************
 * @brief        HAL Flag Encode
 *******************************************************************************
 */
#define HAL_Flag_Encode(type, flag)                          (type << 16 | flag)

/**
 *******************************************************************************
 * @brief        HAL �豸����
 *******************************************************************************
 */
typedef struct
{
    //! �豸�˿�
    uint16_t Port;
    //! �豸״̬
    uint16_t State;
    //! �豸��־
    uint16_t Flag;
    //! �豸�¼�
    uint16_t Event;
}HAL_Atrribute_t;

/**
 *******************************************************************************
 * @brief      HAL ��Чλ����
 *******************************************************************************
 */
#define HAL_DEVICE_MSB                                                       (0)
#define HAL_DEVICE_LSB                                                       (1)

/**
 *******************************************************************************
 * @brief      HAL �豸������ʶ
 *******************************************************************************
 */
#define HAL_READ_FLAG                                                     _bv(0)
#define HAL_WRITE_FLAG                                                    _bv(1)

/**
 *******************************************************************************
 * @brief      HAL �豸��������
 *******************************************************************************
 */
enum HAL_DEVICE_CMDS
{
    HAL_INIT_CMD = 0,
    HAL_FINI_CMD,
    HAL_REQUEST_CMD,
    HAL_SET_CALLBACK_CMD,
    
    HAL_SPECIAL_CMD,
};

/**
 *******************************************************************************
 * @brief      HAL �豸�ж��¼�
 *******************************************************************************
 */
enum HAL_DEVICE_EVENT
{
    HAL_EVENT_NONE = 0,
    //! Rx indication
    HAL_EVENT_RX_IND,
    //! Rx fifo transfer done
    HAL_EVENT_RX_FIFO_DONE,
    //! Rx DMA transfer done
    HAL_EVENT_RX_DMADONE,
    //! Rx timeout
    HAL_EVENT_RX_TIMEOUT,
    //! Tx complete
    HAL_EVENT_TX_DONE,
    //! Tx fifo transfer complete
    HAL_EVENT_TX_FIFO_DONE,
    //! Tx DMA transfer done
    HAL_EVENT_TX_DMADONE,
};

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief      HAL �豸��Ϣ
 *******************************************************************************
 */
#define HAL_DEVICE_NORMAL_MSG 0
#define HAL_DEVICE_KEY_MSG    1
#define HAL_DEVICE_BLINK_MSG  2
#define HAL_DEVICE_UART_MSG   3

typedef union
{
    //! ������Ϣ
    struct
    {
        uint16_t MsgType;
        uint16_t Event;
        
        union
        {
            void     *Param;
            uint32_t Extend;
        };
    };
    
    //! ������Ϣ
    struct
    {
        uint16_t : 16;
        uint16_t KeyState;
        uint16_t KeyValue;
        uint16_t KeyTick;
    };
}HAL_MSG_t;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        HAL��ʱ������API
 *******************************************************************************
 */
#define HAL_CMD_Nop(t)
#define HAL_Delay_Us(t)
#define HAL_Delay_Ms(t)

//! HAL ��ʱ�������ӿ�
//! �����ײ�ʹ�ô˽ӿ�
#define HAL_GenTimer_Start(tim, tick) _st((tim) = (tick);)
#define HAL_GenTimer_Stop(tim)        _st((tim) = 0;)
#define HAL_GenTimer_Handle(tim)      _st(if(tim > 0)        \
                                         {                  \
                                             if(!--tim)     \
                                             {              \
                                                 tim = -1;  \
                                             }              \
                                         })
#define HAL_GenTimer_Done(tim)       (((tim) == -1) ? ((tim) = 0, 1) : (0))

/**
 *******************************************************************************
 * @brief        HAL�¼�����API
 *******************************************************************************
 */
#define HAL_Evt_Set(p, e)                         _set_bit((p), (e))
#define HAL_Evt_Clr(p, e)                         _clr_bit((p), (e))
#define HAL_Evt_Get(p, e)                         _read_bit((p), (e))
 
/**
 *******************************************************************************
 * @brief        HAL Flag����API
 *******************************************************************************
 */
#define HAL_Flag_Set(p, e)                        _set_reg((p), (e))
#define HAL_Flag_Clr(p, e)                        _clr_reg((p), (e))
#define HAL_Flag_Get(p, e)                        _get_reg((p), (e))

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif      /** prevent recursive inclusion */

/**********************************END OF FILE*********************************/
