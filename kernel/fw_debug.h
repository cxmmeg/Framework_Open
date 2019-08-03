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
 * @file       fw_debug.h                                                      *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2019-03-22                                                      *
 * @brief      framework server component head file                            *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1. 2019-03-22    Create File                                                *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FRAMEWORK_DEBUG_H__
#define __FRAMEWORK_DEBUG_H__

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "compiler.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
//! ����̨��ʼ��
extern void Fw_Console_Init(void);

#ifdef ENABLE_FRAMEWORK_DEBUG
//! ��־��� API
extern void Fw_Console_Put(char *str, ...);
extern void Fw_Core_Log(char *str, ...);
extern void Fw_Core_Error(char *str, ...);
extern void Fw_Log(char *str, ...);
extern void Fw_Error(char *str, ...);
extern void Fw_Put_Info(void);
//! ʱ����� API
extern void Fw_TimeTest_Begin(void);
extern void Fw_TimeTest_End(void);
//! ��ʱ��������� API
extern void Fw_Put_Timer_List(void);
extern void Fw_Timer_Test(void);
//! ����������� API
extern void Fw_Put_Task_List(void);
//! �¼�������� API
extern void Fw_Event_General(void);
#else
#ifndef USE_KEIL_C51_COMPILER
#define Fw_Console_Put(str, ...)
#define Fw_Core_Log(str, ...)  
#define Fw_Core_Error(str, ...)
#define Fw_Log(str, ...)       
#define Fw_Error(str, ...)
#define Fw_Put_Info()
#define Fw_TimeTest_Begin()
#define Fw_TimeTest_End()
#define Fw_Timer_Test()
#define Fw_Put_Timer_List()
#define Fw_Timer_Test()
#define Fw_Put_Task_List()
#define Fw_Event_General()
#endif
#endif

/**
 *******************************************************************************
 * @brief       ��ǰʱ��ת�ַ���API
 * @param       [in/out]  level    ת���ȼ�
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
#define FW_TIME_2_STR_ALL  0
#define FW_TIME_2_STR_DATE 1
#define FW_TIME_2_STR_TIME 0x10

extern char *Fw_Time_To_Str(uint8_t level);

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif      /** prevent recursive inclusion */

/**********************************END OF FILE*********************************/
