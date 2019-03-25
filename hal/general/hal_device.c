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
 * @file       hal_device.c                                                    *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-08-28                                                      *
 * @brief      hal device component                                            *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1. 20170828    �����ļ�"hal_device.c"                                       *
 * 2. 20180127    �޸�HAL��ܹ�                                                *
 *******************************************************************************
 */
 
/**
 * @defgroup hal device component
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_device.h"
#include "fw_core.h"
#include <string.h>

/* Private define ------------------------------------------------------------*/
//! HAL Flag decode
#define HAL_Flag_Decode_Type(flag)                                  (flag >> 16)
#define HAL_Flag_Decode_Flag(flag)                         (flag & 0x0000FFFFUL)

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief      HAL DEVICE ר�ñ���
 *******************************************************************************
 */
#ifdef ENABLE_HAL_DRIVER_COMPONENT
struct 
{
    FwList_t List;

    uint16_t   Num;
}DeviceList;
#endif

/* Private functions ---------------------------------------------------------*/
#ifdef ENABLE_HAL_DRIVER_COMPONENT
/**
 *******************************************************************************
 * @brief       �豸�����ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  0
 * @note        ���ں˵���
 *******************************************************************************
 */
//! ��ʼ��HAL������
int HAL_Component_Init(void)
{
    //! ��ʼ���豸�б�
    FwList_Init(&DeviceList.List);
    DeviceList.Num = 0;

    return 0;
}
INIT_COMPONENT_EXPORT(HAL_Component_Init);

/**
 *******************************************************************************
 * @brief       �豸ע�ắ��
 * @param       [in/out]  *dev     �豸���
 * @param       [in/out]  *name    �豸������
 * @param       [in/out]  *ops     �豸�����ӿ�
 * @param       [in/out]  flag     ��������
 * @return      [in/out]  0        ע��ɹ�
 * @return      [in/out]  1        ע��ʧ��
 * @note        ���ں˵���
 *******************************************************************************
 */
uint16_t HAL_Device_Register(HAL_Device_t *dev, char *name, HAL_Interface_t *ops, uint32_t flag)
{
    dev->Name = name;

    dev->Flag = HAL_Flag_Decode_Flag(flag);
    dev->Type = HAL_Flag_Decode_Type(flag);

    dev->Ops.Init    = ops->Init;
    dev->Ops.Fini    = ops->Fini;
    dev->Ops.Open    = ops->Open;
    dev->Ops.Close   = ops->Close;
    dev->Ops.Read    = ops->Read;
    dev->Ops.Write   = ops->Write;
    dev->Ops.Control = ops->Control;
    
    dev->State = 1;
    DeviceList.Num ++;
    
    FwList_Init(&dev->List);
    FwList_InsertAfter(&DeviceList.List, &dev->List);

    return 0;
}

/**
 *******************************************************************************
 * @brief       �豸ע������
 * @param       [in/out]  *dev     �豸���
 * @return      [in/out]  0        ע��ɹ�
 * @return      [in/out]  1        ע��ʧ��
 * @note        ���ں˵���
 *******************************************************************************
 */
uint16_t HAL_Device_Unregister(HAL_Device_t *dev)
{
    if (dev->State)
    {
        FwList_Remove(&dev->List);
        
        dev->State = 0;
        
        DeviceList.Num --;
    }
    
    return 0;
}

/**
 *******************************************************************************
 * @brief       �豸���Һ���
 * @param       [in/out]  type             �豸����
 * @return      [in/out]  HAL_Device_t *   �豸���
 * @note        ���ں˵���
 *******************************************************************************
 */
HAL_Device_t *HAL_Device_Find(uint16_t type)
{
    HAL_Device_t *dev = NULL;
    FwList_t *list = &DeviceList.List;
    uint16_t i;
    
    for (i=0; i<DeviceList.Num; i++)
    {
        list = list->Next;
        
        dev = FwListEntry(list, HAL_Device_t, List);
        
        if (dev->Type == type)
        {
            return dev;
        }
    }

    return NULL;
}

/**
 *******************************************************************************
 * @brief       �豸��ʼ������
 * @param       [in/out]  *dev             �豸����
 * @param       [in/out]  param            �豸����
 * @return      [in/out]  state            ״̬
 * @note        ���ں˵���
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Device_Init(struct HAL_DEVICE *dev, size_t param)
{
    if (!IS_PTR_NULL(dev->Ops.Init))
    {
        return dev->Ops.Init(dev, param);
    }
    
    return 1;
}

/**
 *******************************************************************************
 * @brief       �豸����ʼ������
 * @param       [in/out]  *dev             �豸����
 * @return      [in/out]  state            ״̬
 * @note        ���ں˵���
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Device_Fini(struct HAL_DEVICE *dev)
{
    if (!IS_PTR_NULL(dev->Ops.Fini))
    {
        return dev->Ops.Fini(dev);
    }
    
    return 1;
}

/**
 *******************************************************************************
 * @brief       �豸�򿪺���
 * @param       [in/out]  *dev             �豸���
 * @return      [in/out]  state            �豸״̬
 * @note        ���ں˵���
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Device_Open(struct HAL_DEVICE *dev, uint16_t flag)
{
    if (!IS_PTR_NULL(dev->Ops.Open))
    {
        return dev->Ops.Open(dev, flag);
    }
    
    return 1;
}

/**
 *******************************************************************************
 * @brief       �豸�رպ���
 * @param       [in/out]  *dev             �豸���
 * @return      [in/out]  state            �豸״̬
 * @note        ���ں˵���
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Device_Close(struct HAL_DEVICE *dev)
{
    if (!IS_PTR_NULL(dev->Ops.Close))
    {
        return dev->Ops.Close(dev);
    }
    
    return 1;
}

/**
 *******************************************************************************
 * @brief       �豸����д�뺯��
 * @param       [in/out]  *dev             �豸���
 * @param       [in/out]  pos              ������ƫ����
 * @param       [in/out]  *buf             ������
 * @param       [in/out]  len              д�볤��
 * @return      [in/out]  num              ʵ��д������
 * @note        ���ں˵���
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Device_Write(struct HAL_DEVICE *dev, uint16_t pos, uint8_t *buf, uint16_t len)
{
    if (!IS_PTR_NULL(dev->Ops.Write))
    {
        return dev->Ops.Write(dev, pos, buf, len);
    }
    
    return 0; 
}

/**
 *******************************************************************************
 * @brief       �豸���ݶ�ȡ����
 * @param       [in/out]  *dev             �豸���
 * @param       [in/out]  pos              ������ƫ����
 * @param       [in/out]  *buf             ������
 * @param       [in/out]  len              ��ȡ����
 * @return      [in/out]  num              ʵ�ʶ�ȡ����
 * @note        ���ں˵���
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Device_Read(struct HAL_DEVICE *dev, uint16_t pos, uint8_t *buf, uint16_t len)
{
    if (!IS_PTR_NULL(dev->Ops.Read))
    {
        return dev->Ops.Read(dev, pos, buf, len);
    }
    
    return 0;
}

/**
 *******************************************************************************
 * @brief       �豸���ƺ���
 * @param       [in/out]  *dev             �豸���
 * @param       [in/out]  cmd              ��������
 * @param       [in/out]  *args            ���Ʋ���
 * @return      [in/out]  state            ״̬
 * @note        ���ں˵���
 *******************************************************************************
 */
__INLINE
uint16_t HAL_Device_Control(struct HAL_DEVICE *dev, uint8_t cmd, void *args)
{
    if (!IS_PTR_NULL(dev->Ops.Control))
    {
        return dev->Ops.Control(dev, cmd, args);
    }
    
    return 0xFFFF;
}
#endif

/**
 *******************************************************************************
 * @brief       ����ԭ�Ӳ���
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void HAL_Atom_Begin(void)
{
    Fw_Enter_Critical();
}

/**
 *******************************************************************************
 * @brief       �˳�ԭ�Ӳ���
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void HAL_Atom_End(void)
{
    Fw_Exit_Critical();
}

/** @}*/     /** hal device component */

/**********************************END OF FILE*********************************/
