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
 * @file       hal_blink.h                                                     *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2018-01-16                                                      *
 * @brief      blink driver component                                          *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.2018-01-16 �����ļ�������LED��˸����                                      *
 * 1.2018-06-28 �޸��ļ���Ϊ��hal_blink.c�������Ͽ�ܣ�ͳһHAL��                 *
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_BLINK_H__
#define __HAL_BLINK_H__

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif


/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//! BLINK ״̬����
#define HAL_BLINK_INIT_STATE  0
#define HAL_BLINK_ON_STATE    1
#define HAL_BLINK_OFF_STATE   2
#define HAL_BLINK_DOING_STATE 3

//! BLINK �����
#define HAL_BLINK_INIT_CMD    0
#define HAL_BLINK_ON_CMD      1
#define HAL_BLINK_OFF_CMD     2
    
/* Exported types ------------------------------------------------------------*/
typedef int (__CODE* HALBlinkCallback)(void *handle, uint8_t cmd);

typedef struct HAL_BLINK
{
    //! ��ʱ�����
    HalDevTim_t Timer;
    
    //! ���ûص�����
    HALBlinkCallback Callback;
    
    //! �ص�����
    void *Param;
    
    //! �豸����
    char *Name;
    
    //! ��ǰ״̬
    uint8_t State;
    //! ��˸����
    int16_t Count;
    //! ��˸��ʱʱ��
    uint16_t Timeout;
}HAL_Blink_t;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void HAL_Blink_Init(HAL_Blink_t *blink, char *name, HALBlinkCallback callback, void *param);
extern void HAL_Blink_On(HAL_Blink_t *blink);
extern void HAL_Blink_Off(HAL_Blink_t *blink);
extern void HAL_Blink_Control(HAL_Blink_t *blink, int16_t count, uint16_t tick);

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif      /** prevent recursive inclusion */

/**********************************END OF FILE*********************************/
