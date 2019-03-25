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
 * @file       hal_key.c                                                       *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2015-06-15                                                      *
 * @brief      key driver component                                            *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1. 2015-06-15 �����ļ� ԭ����bsp_key.c�� ʵ�ְ����ļ�⴦��ͨ��FSMʵ��      *
 * 2. 2015-08-25 ͨ����������ʵ�ֵײ��״̬Ǩ�ƣ��ӿ�ִ���ٶ�                  *
 * 3. 2016-03-02 ���������������                                              *
 * 4. 2017-02-23 �޸��ļ���Ϊ��fw_signal.c��,�ع������ʹ�ÿ��APIʵ��           *
 * 5. 2018-05-16 �޸��ļ���Ϊ��HAL_key.c��,�ع������ʹ��HAL��APIʵ��            *
 * 6. 2018-06-28 �޸��ļ���Ϊ��hal_key.c��,ͳһHAL��ܹ�                         *
 *******************************************************************************
 */

/**
 * @defgroup key driver component
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_device.h"

/* Private define ------------------------------------------------------------*/
//! ����״̬Ǩ��
#define KEY_STATE_TRNASFER(now_key, new_state) _st((now_key)->State = (new_state);)

//! ������Ϣ���ͺ���
#define KEY_MSG_Send(msg, dev) _st((msg).MsgType  = HAL_DEVICE_KEY_MSG; \
                                   (msg).KeyState = (dev)->State; \
                                   (msg).KeyTick  = (dev)->Tick; \
                                   (msg).KeyValue = (dev)->Value; \
                                   HAL_Msg_Send(&(msg));)
  
/* Private typedef -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void HAL_Key_Init(HAL_Key_t *key, HALKeyScan scan, uint16_t tick, uint16_t flag)
{
    key->Scan = scan;
    key->Flag = flag;

    key->FilterTick = 0;
    key->FilterValue = 0;
    key->State = KEY_RELEASE_STATE;
    key->Tick  = 0;
    key->Value = 0;
    
    HAL_SuperTimer_Init(key->Timer, "Key Driver", HAL_Key_Scan, key);
    HAL_SuperTimer_Start(key->Timer, tick/FRAMEWORK_TICK_TIME, FW_TIM_EN_PERIOD);
}

void HAL_Key_Fini(HAL_Key_t *key)
{
    HAL_SuperTimer_Stop(key->Timer);
}

void HAL_Key_Scan(void *param)
{
    HAL_Key_t *key = (HAL_Key_t *)param;
    
    HAL_MSG_t msg;
    
    uint16_t nowValue = key->Scan();
    
    uint8_t change = 0;

    //! 1. ������ƽ���
    if (nowValue != key->FilterValue)
    {
        key->FilterValue = nowValue;
        key->FilterTick = 0;
        
        change = 1;
    }

    //! 2. ���°���ʱ��
    if (key->FilterTick < 0xFFFF)
    {
        key->FilterTick ++;
    }
    
    //! 3. ����״̬������
    switch (key->State)
    {
        case KEY_RELEASE_STATE:
            if (change)
            {
                //! ����״̬Ǩ��
                //! Ǩ�������²����˲�״̬
                KEY_STATE_TRNASFER(key, KEY_RELEASE_FILTER_STATE);
            }
            break;
        case KEY_RELEASE_FILTER_STATE:
            if (change)
            {
                //! ����״̬Ǩ��
                //! Ǩ���������ɿ�״̬
                KEY_STATE_TRNASFER(key, KEY_RELEASE_STATE);
                
                //! ȷ��Ϊ��������
                if (key->Value == key->FilterValue)
                {
                    KEY_STATE_TRNASFER(key, KEY_RELEASE_STATE);
                }
            }
            else
            {   
                //! �˲���ʱ
                if (key->FilterTick >= HAL_KEY_FILTER_TICK)
                {
                    //! ����tick
                    key->Tick = key->FilterTick;
                    
                    //! ���¼�ֵ
                    key->Value = key->FilterValue;
                    
                    //! ����״̬Ǩ��
                    //! Ǩ�����������±���״̬
                    KEY_STATE_TRNASFER(key, KEY_PRESS_EDGE_STATE);
                    
                    //! ����״̬Ǩ��
                    //! Ǩ����������ס״̬
                    //! ���������Ϣ���¼�
                    KEY_MSG_Send(msg, key);
                    
                    //! ����״̬Ǩ��
                    //! Ǩ����������ס״̬
                    KEY_STATE_TRNASFER(key, KEY_PRESS_STATE);
                }
            }
            break;
        case KEY_PRESS_STATE:
            if (change)
            {
                //! ����״̬Ǩ��
                //! Ǩ���������ɿ��˲�״̬
                KEY_STATE_TRNASFER(key, KEY_PRESS_FILTER_STATE);
            }
            else
            {   
                key->Tick = key->FilterTick;
                
                //! ʹ���˳������
                if (HAL_Flag_Get(key->Flag, HAL_KEY_LONG_SCAN))
                {
                    //! ÿ�뷢��һ����Ϣ
                    if ((key->Tick % HAL_KEY_SECOND_TICK) == 0)
                    {
                        //! ����״̬Ǩ��
                        //! ���������Ϣ���¼�
                        KEY_MSG_Send(msg, key);
                    }
                }
            }
            break;
        case KEY_PRESS_FILTER_STATE:
            if (change)
            {
                //! ����״̬Ǩ��
                //! Ǩ�����������±�����Ӧ״̬
                key->FilterTick = key->Tick;
                
                //! ȷ��Ϊ��������
                if (key->Value == key->FilterValue)
                {
                    KEY_STATE_TRNASFER(key, KEY_PRESS_STATE);
                }
            }
            else
            {   
                //! �˲���ʱ
                if (key->FilterTick >= HAL_KEY_FILTER_TICK)
                {
                    //! ����״̬Ǩ��
                    //! Ǩ���������ɿ�����״̬
                    KEY_STATE_TRNASFER(key, KEY_RELEASE_EDGE_STATE);
                
                    //! ���������Ϣ���¼�
                    KEY_MSG_Send(msg, key);
                    
                    //! ��ռ���ֵ
                    key->Tick = 0;

                    //! ���¼�ֵ
                    key->Value = key->FilterValue;
                    
                    //! Ǩ�����������±���״̬
                    if (key->Value == HAL_KEY_VALUE_NOP)
                    {
                        //! ����״̬Ǩ��
                        //! Ǩ���������ɿ�״̬
                        KEY_STATE_TRNASFER(key, KEY_RELEASE_STATE);
                    }
                    else
                    {
                        //! ����״̬Ǩ��
                        //! Ǩ���������ɿ�״̬
                        KEY_STATE_TRNASFER(key, KEY_RELEASE_FILTER_STATE);
                    }
                }
            }
            break;
//            case KEY_RELEASE_EDGE_STATE:
//                break;
        default:
            break;
    }
}

/** @}*/     /** key driver component */

/**********************************END OF FILE*********************************/
