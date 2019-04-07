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
 * @file       hal_key.h                                                       *
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
 * 1. 2015-06-15 �����ļ� ԭ����bsp_key.h�� ʵ�ְ����ļ�⴦��ͨ��FSMʵ��      *
 * 2. 2015-08-25 ͨ����������ʵ�ֵײ��״̬Ǩ�ƣ��ӿ�ִ���ٶ�                  *
 * 3. 2016-03-02 ���������������                                              *
 * 4. 2017-02-23 �޸��ļ���Ϊ��fw_signal.h��,�ع������ʹ�ÿ��APIʵ��           *
 * 5. 2018-05-16 �޸��ļ���Ϊ��HAL_key.h��,�ع������ʹ��HAL��APIʵ��            *
 * 6. 2018-06-28 �޸��ļ���Ϊ��hal_key.h��,ͳһHAL��ܹ�                         *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "hal_def.h"

/* Exported macro ------------------------------------------------------------*/
//! ���尴��ɨ������(���ڣ�1MS)
#define HAL_KEY_SCAN_PERIOD                             (10/FRAMEWORK_TICK_TIME)
#define HAL_KEY_FILTER_TICK                             (40/HAL_KEY_SCAN_PERIOD)
#define HAL_KEY_SECOND_TICK                           (1000/HAL_KEY_SCAN_PERIOD)

//! ������־
#define HAL_KEY_LOW_TRIG                                                  (0x00)
#define HAL_KEY_HIGH_TRIG                                                 (0x01)
#define HAL_KEY_LONG_SCAN                                                 (0x02)
#define HAL_KEY_USE_MESSAGE_QUEUE                                         (0x04)
#define HAL_KEY_USE_EVENT                                                 (0x08)
    
//! ���尴���ռ�ֵ
#define HAL_KEY_VALUE_NOP                                                    (0)                

//! ���尴�����״̬
#define KEY_RELEASE_STATE                                                    (0)
#define KEY_RELEASE_FILTER_STATE                                             (1)
#define KEY_PRESS_EDGE_STATE                                                 (2)
#define KEY_PRESS_STATE                                                      (3)
#define KEY_PRESS_FILTER_STATE                                               (4)
#define KEY_PRESS_EDGE_FILTER_STATE                                          (5)
#define KEY_RELEASE_EDGE_STATE                                               (6)

/* Exported types ------------------------------------------------------------*/
typedef uint16_t (__CODE *HAL_KeyValueGet_f)(void);

//! �����������
typedef struct 
{
    HAL_KeyValueGet_f Scan;

    uint16_t Tick;
    uint16_t FilterTick;

    uint16_t Value;
    uint16_t FilterValue;

    uint16_t State;

    //! |7     |6     |5     |4     |3                 |2                 |1                 |0   |
    //! |δʹ��|δʹ��|δʹ��|δʹ��|�¼�����ʹ�ܱ�־λ|��Ϣ����ʹ�ܱ�־λ|�������ʹ�ܱ�־λ|����|
    uint16_t Flag;
}HAL_Key_t;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void HAL_Key_Init(HAL_Key_t *key, HAL_KeyValueGet_f scan, uint16_t tick, uint16_t flag);
extern void HAL_Key_Fini(HAL_Key_t *key);
extern void HAL_Key_Scan(void *param);

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif      /** prevent recursive inclusion */

/**********************************END OF FILE*********************************/
