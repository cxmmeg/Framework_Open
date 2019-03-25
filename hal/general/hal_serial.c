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
 * @file       hal_serial.c                                                    *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-01-10                                                      *
 * @brief      hal serial driver component                                     *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.20170110 �����ļ� �ļ���:fw_debug.c                                       *
 * 2.20171106 �ϲ�fw_stream.c������ݣ�ʵ�ִ������أ���ƽ̨����                *
 *            ����ΪHAL_SERIAL.c                                               *
 * 3.20180423 ͳһӲ������㣬�޸�Ӳ���ײ�����(�ο�RT-THREAD)                  *
 *            ֧�ֶ���UARTģʽ                                                 *
 * 4.20180804 �޸��ļ�������HAL_SERIAL.c�޸�ΪHAL_SERIAL.c                     *
 * 5.20180815 ������115200��ͨ��ѹ������(ÿ��10MS����"Hello World!")           *
 *******************************************************************************
 */
 
/**
 * @defgroup hal serial driver component
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_device.h"
#include <string.h>

/* Private define ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       serial�����ز�����
 *******************************************************************************
 */
#if 0
#define Serial_PutChar(a, c) ((!IS_PTR_NULL(DeviceSerial.Ops.PutChar)) ? (DeviceSerial.Ops.PutChar((serial), (c))) : (0))
#define Serial_GetChar(a, c) ((!IS_PTR_NULL(DeviceSerial.Ops.GetChar)) ? (DeviceSerial.Ops.GetChar((serial), (char *)(c))) : (0))
#define Serial_Put(a, b, c)  ((!IS_PTR_NULL(DeviceSerial.Ops.DmaTransmit)) ? (DeviceSerial.Ops.DmaTransmit(serial, (b), (c), HAL_SERIAL_DIR_TX)) : (0))
#else
#define Serial_PutChar(a, c) (DeviceSerial.Ops.PutChar((serial), (c)))
#define Serial_GetChar(a, c) (DeviceSerial.Ops.GetChar((serial), (char *)(c)))
#define Serial_Put(a, b, c)  (DeviceSerial.Ops.DmaTransmit(serial, (b), (c), HAL_SERIAL_DIR_TX))
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef ENABLE_HAL_SERIAL_DRIVER
struct HAL_DRIVER_SERIAL
{
    HAL_Device_t           Parent;
    HAL_Serial_Interface_t Ops;
}DeviceSerial;
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#ifdef ENABLE_HAL_SERIAL_DRIVER
/**
 *******************************************************************************
 * @brief       SERIAL ���ʹ�����
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  void
 * @return      [in/out]  ������
 * @note        None
 *******************************************************************************
 */
__STATIC_INLINE
void Serial_Send_Handle(HAL_Serial_t *serial)
{
    HAL_Atom_Begin();

    FwBuf_t *fifo = &serial->Tx.Fifo;
    uint16_t ch;

    //! ʹ�ܷ��ͻ���
    if (serial->Tx.State == HAL_SERIAL_TX_IDLE)
    {
        ch = FwBuf_ReadByte(fifo);

        if (ch != 0xFFFF)
        {
            Serial_PutChar(serial, ch);
        }
    }

    HAL_Atom_End();
}

/**
 *******************************************************************************
 * @brief       SERIAL �豸ע�ắ��
 * @param       [in/out]  name       �豸����
 * @param       [in/out]  ops        �豸�ײ�����ӿ�
 * @param       [in/out]  flag       �豸���ò���
 * @param       [in/out]  userData   �û�����
 * @return      [in/out]  ���ý��
 * @note        None
 *******************************************************************************
 */
void HAL_Serial_Register(HAL_Serial_Interface_t *ops, void *userData)
{
    memset(&DeviceSerial, 0, sizeof(DeviceSerial));

    DeviceSerial.Ops.Config      = ops->Config;
    DeviceSerial.Ops.Control     = ops->Control;
    DeviceSerial.Ops.DmaTransmit = ops->DmaTransmit;
    DeviceSerial.Ops.GetChar     = ops->GetChar;
    DeviceSerial.Ops.PutChar     = ops->PutChar;

#if defined(ENABLE_HAL_DRIVER_COMPONENT)
    HAL_Interface_t halDevOps;

    halDevOps.Close   = NULL;
    halDevOps.Control = NULL;
    halDevOps.Fini    = NULL;
    halDevOps.Init    = NULL;
    halDevOps.Open    = NULL;
    halDevOps.Read    = NULL;
    halDevOps.Write   = NULL;

    DeviceSerial.Parent.UserData = userData;

    HAL_Device_Register(&DeviceSerial.Parent, HAL_SERIAL_STR, &halDevOps, HAL_Flag_Encode(HAL_DEVICE_SERIAL, HAL_READ_FLAG | HAL_WRITE_FLAG));
#endif
}

/**
 *******************************************************************************
 * @brief       UART ������ʼ��
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  config    ���ò���
 * @return      [in/out]  ���ý��
 * @note        None
 *******************************************************************************
 */
void HAL_Serial_Init(HAL_Serial_t *serial, HAL_Serial_Config_t *config)
{
    memset(serial, 0, sizeof(HAL_Serial_t));

    FwBuf_Init(&serial->Tx.Fifo, config->Buffer.Tx, config->Buffer.TxLen);
    FwBuf_Init(&serial->Rx.Fifo, config->Buffer.Rx, config->Buffer.RxLen);
    
    serial->Attribute.Port = config->Attribute.Port;

    serial->Tx.State   = HAL_SERIAL_INIT;
    serial->Rx.State   = HAL_SERIAL_INIT;

    serial->Super      = config->Super;
    serial->SuperParam = config->SuperParam;
    
    serial->Dev        = &DeviceSerial.Parent;
    
    //! ��ʼ��Ӳ��
    if (!IS_PTR_NULL(DeviceSerial.Ops.Config))
    {
        DeviceSerial.Ops.Config(serial, config);
    }
    
    //! ע���豸�����ص�    
    Serial_Callback callback = {.Callback = HAL_Serial_Isr_Handle, .Param = (void *)serial};
    
    if (!IS_PTR_NULL(DeviceSerial.Ops.Control))
    {
        DeviceSerial.Ops.Control(serial, HAL_SET_CALLBACK_CMD, (void *)&callback);
    }

    //! ����λ���
    HAL_Serial_EnableTx(serial);
    HAL_Serial_EnableRx(serial);
}

/**
 *******************************************************************************
 * @brief       UART ʹ�ܷ������
 * @param       [in/out]  serial     �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void HAL_Serial_EnableTx(HAL_Serial_t *serial)
{
    serial->Tx.State = HAL_SERIAL_TX_IDLE;
    
    FwBuf_SetEmpty(&serial->Tx.Fifo);
}

/**
 *******************************************************************************
 * @brief       UART ���÷������
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void HAL_Serial_DisableTx(HAL_Serial_t *serial)
{
    serial->Tx.State = HAL_SERIAL_TX_SLEEP;
    
    FwBuf_SetEmpty(&serial->Tx.Fifo);
}

/**
 *******************************************************************************
 * @brief       UART ʹ�ܽ������
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void HAL_Serial_EnableRx(HAL_Serial_t *serial)
{
    serial->Rx.State = HAL_SERIAL_RX_IDLE;
    
    FwBuf_SetEmpty(&serial->Rx.Fifo);
}

/**
 *******************************************************************************
 * @brief       UART ���ý������
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void HAL_Serial_DisableRx(HAL_Serial_t *serial)
{
    serial->Rx.State = HAL_SERIAL_RX_SLEEP;
    
    FwBuf_SetEmpty(&serial->Rx.Fifo);
}

/**
 *******************************************************************************
 * @brief       ��� UART �����Ƿ����
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  0         ������
 * @return      [in/out]  1         �������
 * @note        None
 *******************************************************************************
 */
uint16_t HAL_Serial_IsTxDone(HAL_Serial_t *serial)
{
    if (serial->Tx.State == HAL_SERIAL_TX_SLEEP)
    {
        serial->Tx.State = HAL_SERIAL_TX_IDLE;
        
        return 1;
    }
    
    return 0;
}

/**
 *******************************************************************************
 * @brief       ��� UART �����Ƿ����
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  0         ������
 * @return      [in/out]  1         �������
 * @note        None
 *******************************************************************************
 */
uint16_t HAL_Serial_IsRxDone(HAL_Serial_t *serial)
{
    if (serial->Rx.State == HAL_SERIAL_RX_BUSY)
    {
        serial->Rx.State = HAL_SERIAL_RX_IDLE;
        
        return 1;
    }
    
    return 0;
}

/**
 *******************************************************************************
 * @brief       UART ����ַ�
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  c         ���͵��ַ�
 * @return      [in/out]  0         ���ʧ��
 * @return      [in/out]  1         ����ɹ�
 * @note        None
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Serial_PutChar(HAL_Serial_t *serial, uint8_t c)
{
    return Serial_PutChar(serial, c);
}

/**
 *******************************************************************************
 * @brief       UART �����ַ�
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  *c        ���յ��ַ�
 * @return      [in/out]  0         ����ʧ��
 * @return      [in/out]  1         ����ɹ�
 * @note        None
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Serial_GetChar(HAL_Serial_t *serial, uint8_t *c)
{
    return Serial_GetChar(serial, c);
}

/**
 *******************************************************************************
 * @brief       UART ����ַ���
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  buf       ���ͻ���
 * @param       [in/out]  pos       ���͵�ַƫ����
 * @param       [in/out]  len       �������ݳ���
 * @return      [in/out]  ʵ��д�����ݵ����ݳ���
 * @note        None
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Serial_Put(HAL_Serial_t *serial, uint8_t *buf, uint16_t pos, uint16_t len)
{
    return Serial_Put(serial, &buf[pos], len);
}

/**
 *******************************************************************************
 * @brief       UART д��
 * @param       [in/out]  drv       �豸���
 * @param       [in/out]  buf       ���ͻ���
 * @param       [in/out]  len       �������ݳ���
 * @return      [in/out]  ʵ��д�����ݵ����ݳ���
 * @note        None
 *******************************************************************************
 */
uint16_t HAL_Serial_Write(HAL_Serial_t *serial, uint8_t *buf, uint16_t len)
{
    HAL_Atom_Begin();
    
    switch (serial->Tx.State)
    {
        case HAL_SERIAL_TX_IDLE:
            FwBuf_Write(&serial->Tx.Fifo, &buf[0], len);
            Serial_Send_Handle(serial);
            serial->Tx.State = HAL_SERIAL_TX_BUSY;
            break;
        case HAL_SERIAL_TX_SLEEP:
        case HAL_SERIAL_TX_BUSY:
            len = FwBuf_Write(&serial->Tx.Fifo, &buf[0], len);
            break;
        default:
            len = 0;
            break;
    }

    HAL_Atom_End();
    
    return len;
}

/**
 *******************************************************************************
 * @brief       UART ��ȡ
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  buf       ���ջ���
 * @param       [in/out]  len       �������ݳ���
 * @return      [in/out]  ʵ�ʶ�ȡ�������ݳ���
 * @note        None
 *******************************************************************************
 */
uint16_t HAL_Serial_Read(HAL_Serial_t *serial, uint8_t *buf, uint16_t len)
{
    uint16_t state;
    
    HAL_Atom_Begin();
    
    state = FwBuf_Read(&serial->Rx.Fifo, &buf[0], len);
    
    HAL_Atom_End();
    
    return state;
}

/**
 *******************************************************************************
 * @brief       UART �¼�����
 * @param       [in/out]  *port     �豸���
 * @param       [in/out]  event     �����¼�
 * @return      [in/out]  void
 * @note        ֧��DMA���͡�DMA���ա�UART���͡�UART���� ��������жϺ����д���
 *******************************************************************************
 */
void HAL_Serial_Isr_Handle(void *port, uint16_t event)
{
    HAL_Serial_t *serial = (HAL_Serial_t *)port;

    switch (event)
    {
        /* Rx indication */
        case HAL_SERIAL_EVENT_RX_IND:
        {
            uint8_t ch;
            int16_t state;
            FwBuf_t *fifo = &serial->Rx.Fifo;
            
            while (1)
            {
                state = Serial_GetChar(serial, &ch);
                
                if (state == 0)
                {
                    break;
                }
                
                //! ���������½��յ�����
                fifo->Buffer[fifo->Tail++] = ch;
                
                if(fifo->Tail >= fifo->Len)
                {
                    fifo->Tail = 0;
                }

                if(fifo->Tail == fifo->Head)
                {
                    if (++fifo->Head >= fifo->Len)
                    {
                        fifo->Head = 0;
                    }
                }
            }
            
            if (!IS_PTR_NULL(serial->Super))
            {
                serial->Super(serial->SuperParam, HAL_SERIAL_EVENT_RX_IND, FwBuf_Used(&serial->Rx.Fifo));
            }
            break;
        }
        case HAL_SERIAL_EVENT_RX_TIMEOUT:
            HAL_Evt_Set(serial->Attribute.Event, HAL_SERIAL_RX_TIMEOUT);
            break;
        /* Tx complete   */
        case HAL_SERIAL_EVENT_TX_DONE:
        {
            FwBuf_t *fifo = &serial->Tx.Fifo;
            uint16_t ch = FwBuf_ReadByte(fifo);
            
            if (ch != 0xFFFF)
            {
                Serial_PutChar(serial, ch);
            }
            else
            {
                serial->Tx.State = HAL_SERIAL_TX_IDLE;

                if (!IS_PTR_NULL(serial->Super))
                {
                    serial->Super(serial->SuperParam, HAL_SERIAL_EVENT_TX_DONE, NULL);
                }
            }
            break;
        }
        /* Rx DMA transfer done */
        case HAL_SERIAL_EVENT_RX_DMADONE:
            //! ������������¼�
            HAL_Evt_Set(serial->Attribute.Event, HAL_SERIAL_RX_DONE);
            break;
        /* Tx DMA transfer done */
        case HAL_SERIAL_EVENT_TX_DMADONE:
            //! ������ɴ���
            HAL_Evt_Set(serial->Attribute.Event, HAL_SERIAL_TX_DONE);
            break;
        default:
            break;
    }
}

#endif

/** @}*/     /** hal serial driver component */

/**********************************END OF FILE*********************************/
