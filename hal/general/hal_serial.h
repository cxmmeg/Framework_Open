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
 * @file       hal_serial.h                                                    *
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_DRIVER_SERIAL_H__
#define __HAL_DRIVER_SERIAL_H__

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        HAL SERIAL PORT
 *******************************************************************************
 */
enum HAL_SERIAL_PORT
{
    HAL_SERIAL_0,
    HAL_SERIAL_1,
    HAL_SERIAL_2,
    HAL_SERIAL_3,
    HAL_SERIAL_4,
    HAL_SERIAL_5,
    HAL_SERIAL_6,
    HAL_SERIAL_7,
    HAL_SERIAL_8,
    
    HAL_SERIAL_MAX,
};

/**
 *******************************************************************************
 * @brief      ���� SERIAL ���������ò���
 *******************************************************************************
 */
#define HAL_SERIAL_BAUD_2400              2400
#define HAL_SERIAL_BAUD_4800              4800
#define HAL_SERIAL_BAUD_9600              9600
#define HAL_SERIAL_BAUD_19200             19200
#define HAL_SERIAL_BAUD_38400             38400
#define HAL_SERIAL_BAUD_57600             57600
#define HAL_SERIAL_BAUD_115200            115200
#define HAL_SERIAL_BAUD_230400            230400
#define HAL_SERIAL_BAUD_460800            460800
#define HAL_SERIAL_BAUD_921600            921600
#define HAL_SERIAL_BAUD_2000000           2000000
#define HAL_SERIAL_BAUD_3000000           3000000

/**
 *******************************************************************************
 * @brief      ���� SERIAL ����λ���ò���
 *******************************************************************************
 */
#define HAL_SERIAL_DATA_BITS_5            5
#define HAL_SERIAL_DATA_BITS_6            6
#define HAL_SERIAL_DATA_BITS_7            7
#define HAL_SERIAL_DATA_BITS_8            8
#define HAL_SERIAL_DATA_BITS_9            9

/**
 *******************************************************************************
 * @brief      ���� SERIAL ֹͣλ���ò���
 *******************************************************************************
 */
#define HAL_SERIAL_STOP_BITS_1            1
#define HAL_SERIAL_STOP_BITS_2            2
#define HAL_SERIAL_STOP_BITS_3            3
#define HAL_SERIAL_STOP_BITS_4            4

/**
 *******************************************************************************
 * @brief      ���� SERIAL У��λ���ò���
 *******************************************************************************
 */
#define HAL_SERIAL_PARITY_NONE            0
#define HAL_SERIAL_PARITY_ODD             1                           //! ��У��
#define HAL_SERIAL_PARITY_EVEN            2                           //! żУ��

/**
 *******************************************************************************
 * @brief      ���� SERIAL ���뷽ʽ���ò���
 *******************************************************************************
 */
#define HAL_SERIAL_NRZ_NORMAL             0       /* Non Return to Zero : normal mode */
#define HAL_SERIAL_NRZ_INVERTED           1       /* Non Return to Zero : inverted mode */

/**
 *******************************************************************************
 * @brief      SERIAL ���ͻ���������
 *******************************************************************************
 */
#define HAL_SERIAL_CACHE_SIZE            64

/**
 *******************************************************************************
 * @brief      SERIAL �¼�����
 *******************************************************************************
 */
//! Ӳ���¼�
#define HAL_SERIAL_EVENT_RX_IND          0x01       /* Rx indication */
#define HAL_SERIAL_EVENT_TX_DONE         0x02       /* Tx complete   */
#define HAL_SERIAL_EVENT_RX_FIFO_DONE    0x03       /* Rx fifo transfer done */
#define HAL_SERIAL_EVENT_TX_FIFO_DONE    0x04       /* Tx fifo transfer complete */
#define HAL_SERIAL_EVENT_RX_DMADONE      0x05       /* Rx DMA transfer done */
#define HAL_SERIAL_EVENT_TX_DMADONE      0x06       /* Tx DMA transfer done */
#define HAL_SERIAL_EVENT_RX_TIMEOUT      0x07       /* Rx timeout    */

//! ����¼�
#define HAL_SERIAL_TX_DONE               0x08
#define HAL_SERIAL_TX_START              0x09
#define HAL_SERIAL_TX_TIMEOUT            0x0A
#define HAL_SERIAL_RX_ONCE               0x0B
#define HAL_SERIAL_RX_DONE               0x0C
#define HAL_SERIAL_RX_TIMEOUT            0x0D
#define HAL_SERIAL_RX_OVERFLOW           0x0E

/**
 *******************************************************************************
 * @brief      SERIAL ���䷽����
 *******************************************************************************
 */
#define HAL_SERIAL_DIR_TX                0x00
#define HAL_SERIAL_DIR_RX                0x01
    
/**
 *******************************************************************************
 * @brief      SERIAL ʱ�����
 *******************************************************************************
 */
#define HAL_SERIAL_RX_TICK              10

/**
 *******************************************************************************
 * @brief      SERIAL ������־λ
 *******************************************************************************
 */
#define HAL_SERIAL_TRANSFER_ISR                                           0x0001
#define HAL_SERIAL_TRANSFER_FIFO                                          0x0002
#define HAL_SERIAL_TRANSFER_DMA                                           0x0004

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief      SERIAL DRIVER STATUS
 *******************************************************************************
 */
typedef enum
{
    HAL_SERIAL_INIT,
    HAL_SERIAL_IDLE,
    HAL_SERIAL_BUSY,
    HAL_SERIAL_SLEEP,

    HAL_SERIAL_TX_INIT  = HAL_SERIAL_INIT,
    HAL_SERIAL_TX_IDLE  = HAL_SERIAL_IDLE,
    HAL_SERIAL_TX_BUSY  = HAL_SERIAL_BUSY,
    HAL_SERIAL_TX_SLEEP = HAL_SERIAL_SLEEP,
    
    HAL_SERIAL_RX_INIT  = HAL_SERIAL_INIT,
    HAL_SERIAL_RX_IDLE  = HAL_SERIAL_IDLE,
    HAL_SERIAL_RX_BUSY  = HAL_SERIAL_BUSY,
    HAL_SERIAL_RX_SLEEP = HAL_SERIAL_SLEEP,
}Serial_Status;

/**
 *******************************************************************************
 * @brief      SERIAL SUPER CALLBACK TYPE
 *******************************************************************************
 */
typedef uint32_t (*SerialSuper_t)(void *super, uint32_t evt, uint16_t len);

/**
 *******************************************************************************
 * @brief      SERIAL HANDLE INIT TYPE
 *******************************************************************************
 */
typedef struct
{
    struct
    {
        uint8_t *Tx;
        uint8_t *Rx;
        
        uint16_t TxLen;
        uint16_t RxLen;
    }Buffer;
    
    struct
    {
        uint32_t Baud;

        uint32_t DataBits :4;
        uint32_t StopBits :2;
        uint32_t Parity   :2;
        uint32_t BitOrder :1;
        uint32_t Invert   :1;
        uint32_t BufSize  :16;
        uint32_t Port     :6;
    }Attribute;
    
    SerialSuper_t Super;
    void          *SuperParam;
}HAL_Serial_Config_t;

/**
 *******************************************************************************
 * @brief      SERIAL PIPELINE COMPONENT
 *******************************************************************************
 */
typedef struct
{
    FwBuf_t Fifo;

    Serial_Status State;
}Serial_Pipe_t;

/**
 *******************************************************************************
 * @brief      SERIAL COMPONENT HANDLE
 * @note       todo ��ӶԶ�̬�ڴ��֧�֣���Ҫ����LIBC���
 *******************************************************************************
 */
typedef struct
{
    HAL_Atrribute_t Attribute;
    
    //! �ϲ�ص�
    SerialSuper_t Super;
    void          *SuperParam;
    
    //! �ײ�������
    void *Param;
    
    //! �豸�������
    HAL_Device_t *Dev;
    
    //! ͨѶ�ܵ�
    Serial_Pipe_t Tx;
    Serial_Pipe_t Rx;
}HAL_Serial_t;

/**
 *******************************************************************************
 * @brief      SERIAL INTERFACE
 *******************************************************************************
 */
typedef struct
{
    uint16_t (*Config)(HAL_Serial_t *serial, HAL_Serial_Config_t *config);
    uint32_t (*Control)(HAL_Serial_t *serial, uint32_t cmd, void *arg);

    uint16_t (*PutChar)(HAL_Serial_t *serial, char c);
    uint16_t (*GetChar)(HAL_Serial_t *serial, char *c);

    uint16_t (*DmaTransmit)(HAL_Serial_t *serial, uint8_t *buf, uint16_t size, uint8_t dir);
}HAL_Serial_Interface_t;

/**
 *******************************************************************************
 * @brief      SERIAL ISR CALLBACK
 *******************************************************************************
 */
typedef void (*SerialIsrCallback)(void *port, uint16_t evt);
typedef struct
{
    SerialIsrCallback Callback;
    void *Param;
}Serial_Callback;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
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
extern void HAL_Serial_Register(HAL_Serial_Interface_t *ops, void *userData);

/**
 *******************************************************************************
 * @brief       SERIAL ������ʼ��
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  config    ���ò���
 * @return      [in/out]  ���ý��
 * @note        None
 *******************************************************************************
 */
extern void HAL_Serial_Init(HAL_Serial_t *serial, HAL_Serial_Config_t *config);

/**
 *******************************************************************************
 * @brief       SERIAL ʹ�ܷ������
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void HAL_Serial_EnableTx(HAL_Serial_t *serial);

/**
 *******************************************************************************
 * @brief       SERIAL ���÷������
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void HAL_Serial_DisableTx(HAL_Serial_t *serial);

/**
 *******************************************************************************
 * @brief       SERIAL ʹ�ܽ������
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void HAL_Serial_EnableRx(HAL_Serial_t *serial);

/**
 *******************************************************************************
 * @brief       SERIAL ���ý������
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void HAL_Serial_DisableRx(HAL_Serial_t *serial);

/**
 *******************************************************************************
 * @brief       SERIAL ����ַ�
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  c         ���͵��ַ�
 * @return      [in/out]  0         ���ʧ��
 * @return      [in/out]  1         ����ɹ�
 * @note        None
 *******************************************************************************
 */
extern uint16_t HAL_Serial_PutChar(HAL_Serial_t *serial, uint8_t c);

/**
 *******************************************************************************
 * @brief       SERIAL �����ַ�
 * @param       [in/out]  serial    �豸���
 * @return      [in/out]  *c        ���յ��ַ�
 * @return      [in/out]  0         ����ʧ��
 * @return      [in/out]  1         ����ɹ�
 * @note        None
 *******************************************************************************
 */
extern uint16_t HAL_Serial_GetChar(HAL_Serial_t *serial, uint8_t *c);

/**
 *******************************************************************************
 * @brief       SERIAL ����ַ���
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  buf       ���ͻ���
 * @param       [in/out]  pos       ���͵�ַƫ����
 * @param       [in/out]  len       �������ݳ���
 * @return      [in/out]  ʵ��д�����ݵ����ݳ���
 * @note        None
 *******************************************************************************
 */
extern uint16_t HAL_Serial_Put(HAL_Serial_t *serial, uint8_t *buf, uint16_t pos, uint16_t len);

/**
 *******************************************************************************
 * @brief       SERIAL д��
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  buf       ���ͻ���
 * @param       [in/out]  len       �������ݳ���
 * @return      [in/out]  ʵ��д�����ݵ����ݳ���
 * @note        None
 *******************************************************************************
 */
extern uint16_t HAL_Serial_Write(HAL_Serial_t *serial, uint8_t *buf, uint16_t len);

/**
 *******************************************************************************
 * @brief       SERIAL ��ȡ
 * @param       [in/out]  serial    �豸���
 * @param       [in/out]  buf       ���ջ���
 * @param       [in/out]  len       �������ݳ���
 * @return      [in/out]  ʵ�ʶ�ȡ�������ݳ���
 * @note        None
 *******************************************************************************
 */
extern uint16_t HAL_Serial_Read(HAL_Serial_t *serial, uint8_t *buf, uint16_t len);

/**
 *******************************************************************************
 * @brief       SERIAL Ӳ���¼�����
 * @param       [in/out]  *port     �豸���
 * @param       [in/out]  event     �����¼�
 * @return      [in/out]  void
 * @note        ֧��DMA���͡�DMA���ա�UART���͡�UART����
 *******************************************************************************
 */
extern void HAL_Serial_Isr_Handle(void *port, uint16_t event);

/**
 *******************************************************************************
 * @brief       SERIAL ʱ���¼�����
 * @param       [in/out]  drv       �豸���
 * @return      [in/out]  void
 * @note        void
 *******************************************************************************
 */
extern void HAL_Serial_Tick(void *param);

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif      /** prevent recursive inclusion */

/**********************************END OF FILE*********************************/
