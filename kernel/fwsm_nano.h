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
 * @file       fw_sm.h                                                         *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-01-10                                                      *
 * @brief      framework state machine component head file                     *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.20170110    创建文件fw_sm.h                                               *
 *******************************************************************************
 */

/**
 * @defgroup framework test component
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FRAMEWORK_STATE_MACHINE_NANO_H__
#define __FRAMEWORK_STATE_MACHINE_NANO_H__

/* Add c++ compatibility -----------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "compiler.h"

/* Exported macro ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief      定义SM事件队列使能状态
 *******************************************************************************
 */
#define FW_SM_EVENT_QUEUE_ENABLE

/**
 *******************************************************************************
 * @brief      定义SM状态表使能状态
 *******************************************************************************
 */
#define FW_SM_EVENT_TABLE_ENABLE

/**
 *******************************************************************************
 * @brief      定义SM相关处理功能
 *******************************************************************************
 */
#define Fw_Sm_Tran(me, state)         (((FwSm_t *)me)->Temp = (SmStateHandle)(state), \
                                       FW_SM_RET_TRAN)
#define Fw_Sm_Handle()                (FW_SM_RET_HANDLED)
#define Fw_Sm_Super(me, super)        (((FwSm_t *)me)->SuperId = (super), FW_SM_RET_SUPER)
#define Fw_Sm_Back(me, state)         (((FwSm_t *)me)->Temp = (SmStateHandle)(state), \
                                       FW_SM_RET_BACK)
#define Fw_Sm_ExitSub()               (FW_SM_RET_EXIT_SUB)
#define Fw_Sm_Unhandle()              (FW_SM_RET_UNHANDLED)
#define Fw_Sm_Wait(me, wait)          (((FwSm_t *)me)->Wait = (wait), FW_SM_RET_WAIT)

/**
 *******************************************************************************
 * @brief      定义SM过滤等级
 *******************************************************************************
 */
#define FW_SM_NONE_LEVEL      0xFFFF
#define FW_SM_SUPER_LEVEL     0xF000
#define FW_SM_SUB_LEVEL       0x0F00
#define FW_SM_SUBSUB_LEVEL    0x00F0
#define FW_SM_SUBSUBSUB_LEVEL 0x000F

/**
 *******************************************************************************
 * @brief      定义空状态机
 *******************************************************************************
 */
#define FW_SM_NULL 0xFF

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief      定义状态机数据类型
 *******************************************************************************
 */
enum FW_SM_RET
{
    FW_SM_RET_SUPER,
    FW_SM_RET_SUPER_SUB,
    FW_SM_RET_UNHANDLED,

    FW_SM_RET_WAIT,
    
    FW_SM_RET_CALL,
    FW_SM_RET_BACK,
    FW_SM_RET_EXIT_SUB,
    
    FW_SM_RET_HANDLED,
    FW_SM_RET_IGNORED,

    FW_SM_RET_ENTRY,
    FW_SM_RET_EXIT,

    FW_SM_RET_NULL,

    FW_SM_RET_TRAN,
    FW_SM_RET_TRAN_INIT,
    FW_SM_RET_TRAN_HIST,
};

//! 定义状态机状态
typedef uint8_t FwSmState_t;

//! 定义状态机事件
typedef struct FW_SM_EVT
{
    uint16_t Sig;
    uint8_t  Id;
    uint8_t  Next;
}FwSmMsg_t;

typedef struct
{
	uint16_t NowState;
    uint16_t NowLevel;
	uint16_t InputEvent;
	uint16_t NextState;
    uint16_t NextLevel;
	uint16_t OutEvent;
}FwSmTab_t;

//! 定义状态机处理函数
struct FW_SM;
#ifdef USE_KEIL_C51_COMPILER
typedef uint8_t (__CODE *SmStateHandle)(struct FW_SM *me, FwSmMsg_t *evt) reentrant;
#else
typedef uint8_t (*SmStateHandle)(struct FW_SM *me, FwSmMsg_t *evt);
#endif

/**
 *******************************************************************************
 * @brief      FSM 数据结构
 *******************************************************************************
 */
//! FSM处理句柄
typedef struct FW_SM
{
    SmStateHandle State;
    SmStateHandle Temp;	
    
    uint16_t Wait;
	uint16_t StateCode;

    uint8_t TimerId;
	uint8_t SmId;
    uint8_t SuperId;
    uint8_t SubSmId;
}FwSm_t;

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void FwSm_Task_Init(uint8_t taskId);
extern uint16_t FwSm_Task_Handle(uint8_t taskid, uint16_t evt);
extern void Fw_Sm_Init(uint8_t smId, uint8_t timerId, SmStateHandle init);
extern uint16_t FwSm_Release_Wait(FwSm_t *me);
extern void FwSm_Clear_Wait(FwSm_t *me);
extern void FwSm_Post_Msg(uint16_t sm, uint16_t sig);
extern void FwSm_Timer_Start(FwSm_t *me, uint32_t tick);
extern void FwSm_Timer_Stop(FwSm_t *me);
extern uint32_t FwSm_Timer_Tick(FwSm_t *me);
extern uint16_t Fw_Sm_Code(FwSm_t *me);
extern void Fw_Sm_SetCode(FwSm_t *me, uint16_t sCode);
extern void *Fw_Sm_State(uint16_t smId);

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
	
#endif       /** end include define */

/** @}*/     /** framework state machine component */

/**********************************END OF FILE*********************************/
