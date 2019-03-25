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
 * @file       fw_nano.h                                                       *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-01-10                                                      *
 * @brief      framework core nano version interface head files                *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.20170110    �����ļ�fw_event.h         		                           *
 * 2.20180918    �޸��ļ��ܹ�������Ϊfw_nano.h                                 *
 *******************************************************************************
 */

/**
 * @defgroup framework core interface
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FRAMEWORK_NANO_H__
#define __FRAMEWORK_NANO_H__

/* Add c++ compatibility -----------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "compiler.h"
#include "hal_msp.h"

/* Exported macro ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        �������
 *******************************************************************************
 */
//! �����������״̬
#define FW_TASK_INIT                                                         (0)
#define FW_TASK_READY                                                        (1)
#define FW_TASK_YEILD                                                        (2)
#define FW_TASK_DELAY                                                        (3)
#define FW_TASK_SLEEP                                                        (4)

/**
 *******************************************************************************
 * @brief        �������
 *******************************************************************************
 */
//! ��������������ȼ�
#define FRAMEWORK_MINIMUM_PRIORITY               (FRAMEWORK_TASK_PRIORITY_MAX-1)

//! ��������������ȼ�
#define FRAMEWORK_HIGHEST_PRIORITY                                           (0)

//! framework array link list null
#define FW_MSG_ADDR_NULL                                                  (0xFF)

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        tick block
 *******************************************************************************
 */
#ifdef FRAMEWORK_NANO_EXPAND
typedef uint32_t FwTick_t;
#define FRAMEWORK_TICK_MAX UINT32_MAX
#else
typedef uint16_t FwTick_t;
#define FRAMEWORK_TICK_MAX UINT16_MAX
#endif

/**
 *******************************************************************************
 * @brief        timer block
 *******************************************************************************
 */
typedef struct
{
#ifdef FRAMEWORK_NANO_EXPAND
    union
    {
        void (*Callback)(void *param);
        void *Ptr;
    }Handle;
    
    void *param;
    
    FwTick_t Reload;
#endif
    
    FwTick_t Tick;
}FwTimer_t;

/**
 *******************************************************************************
 * @brief        Framework Core Data Structure
 *******************************************************************************
 */
//! ������
#if defined(USE_KEIL_C51_COMPILER)
typedef uint16_t (__CODE *FwTaskHandle)(uint8_t taskid, uint16_t evt);
typedef FwTaskHandle Fw_Task_t;

typedef void (__CODE *FwTaskInit)(uint8_t taskid);
typedef FwTaskInit Fw_Task_Init_t;
#else
typedef uint16_t(*FwTaskHandle)(uint8_t taskid, uint16_t evt);
typedef FwTaskHandle Fw_Task_t;

typedef void(*FwTaskInit)(uint8_t taskid);
typedef FwTaskInit Fw_Task_Init_t;
#endif

//! ������
typedef struct
{
    FwTimer_t Timer;
    
#ifdef FRAMEWORK_NANO_EXPAND
    uint16_t State;
#endif
    
    uint16_t Event;
}FwTask_t;

//! �ں˾��
typedef struct
{
    volatile FwTick_t Tick;
    
#ifdef FRAMEWORK_NANO_EXPAND
    volatile uint32_t IsrLock;
#endif
    
    volatile uint16_t Event;
    
    uint16_t CurTask;
}FwHandle_t;

/* Exported variables --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        ��������
 *******************************************************************************
 */
//! �����б�
extern __CODE Fw_Task_t      FwTaskList[];
extern __CODE Fw_Task_Init_t FwTaskInitList[];

#ifdef USE_KEIL_C51_COMPILER
extern __DATA FwTask_t FwTask[];
extern __DATA FwHandle_t FwCore;
#else
extern FwTask_t FwTask[];
extern FwHandle_t FwCore;
#endif

/* Exported functions --------------------------------------------------------*/
/**
 * @defgroup framework core interface
 * @{
 */
/**
 *******************************************************************************
 * @brief       ����ȫ���ж�
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
#define Fw_IRQ_Disable()                                     __DISABLE_ALL_ISR()

/**
 *******************************************************************************
 * @brief       ����ȫ���жϺ���
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
#define Fw_IRQ_Enable()                                       __ENABLE_ALL_ISR()

/**
 *******************************************************************************
 * @brief       �ٽ���������
 * @note        �û��ɵ���
 *******************************************************************************
 */
#ifdef FRAMEWORK_NANO_EXPAND
#define Fw_Atom_Begin(x)                    _st((x) = (x); Fw_Enter_Critical();)
#define Fw_Atom_End(x)                       _st((x) = (x); Fw_Exit_Critical();)
#else
#define Fw_Atom_Begin(x)       _st((x) = __GET_ISR_FLAG(); __DISABLE_ALL_ISR();)
#define Fw_Atom_End(x)                                   _st(__SET_ISR_FLAG(x);)
#endif

/**
 *******************************************************************************
 * @brief       �����ٽ�㺯��
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Enter_Critical(void);

/**
 *******************************************************************************
 * @brief       �˳��ٽ�㺯��
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Exit_Critical(void);

/**
 *******************************************************************************
 * @brief       Ӳ�������ʼ��
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û�ʵ��
 *******************************************************************************
 */
extern void Fw_Hardware_Init(void);

/**
 *******************************************************************************
 * @brief       Ӳ����ѯ����
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û�ʵ��
 *******************************************************************************
 */
extern void Fw_Hardware_Poll(void);

/**
 *******************************************************************************
 * @brief       ˯�ߴ�����
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û�ʵ��
 *******************************************************************************
 */
extern void Fw_Sleep_Handle(void);

/**
 *******************************************************************************
 * @brief       ���������¼�
 * @param       [in/out]  id      ����ID
 * @param       [in/out]  evt     �����¼�
 * @return      [in/out]  void
 * @note        �û�ʵ��
 *******************************************************************************
 */
#define Fw_Task_Evt_Set(id, evt)             Fw_Evt_Set(FwTask[(id)].Event, evt)

/**
 *******************************************************************************
 * @brief       ��������¼�
 * @param       [in/out]  evt     �����¼�
 * @note        �û�ʵ��
 *******************************************************************************
 */
#define Fw_Task_Evt_Clr(evt)       Fw_Evt_Clr(FwTask[FwCore.CurTask].Event, evt)

/**
 *******************************************************************************
 * @brief       ��λ�����¼�
 * @param       [in/out]  id       ����ID
 * @note        �û�ʵ��
 *******************************************************************************
 */
#define Fw_Task_Evt_Rst(id)               _st(FwTask[FwCore.CurTask].Event = 0;)

/**
 *******************************************************************************
 * @brief       ��ȡ�����¼�
 * @param       [in/out]  evt     �����¼�
 * @note        �û�ʵ��
 *******************************************************************************
 */
#define Fw_Task_Evt_Get(evt)       Fw_Evt_Get(FwTask[FwCore.CurTask].Event, evt)

/**
 *******************************************************************************
 * @brief       ��Ϣ���ͺ���
 * @param       [in/out]  id        ����ID
 * @param       [in/out]  evt       �����¼�
 * @return      [in/out]  0         ���ͳɹ�
 * @return      [in/out]  1         ����ʧ��
 * @note        ���û�����
 *******************************************************************************
 */
extern uint8_t Fw_Msg_Send(uint8_t id, uint8_t evt);

/**
 *******************************************************************************
 * @brief       ��Ϣ���պ���
 * @param       [in/out]  id         ����ID
 * @return      [in/out]  0          ��Ϣ����������Ϣ
 * @return      [in/out]  else       ��Ϣ����������Ϣ
 * @note        ���û�����
 *******************************************************************************
 */
extern uint8_t Fw_Msg_Recv(uint8_t id);

/**
 *******************************************************************************
 * @brief       �¼����ɺ���
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ������
 *******************************************************************************
 */
extern void Fw_Event_General(void);

/**
 *******************************************************************************
 * @brief       ������Ϣ���պ���
 * @param       [in/out]  id         ����ID
 * @return      [in/out]  0          ��Ϣ����������Ϣ
 * @return      [in/out]  else       ��Ϣ����������Ϣ
 * @note        ���û�����
 *******************************************************************************
 */
#define Fw_Task_Msg_Recv()                           Fw_Msg_Recv(FwCore.CurTask)

/**
 *******************************************************************************
 * @brief       ������ʱ����
 * @param       [in/out]  tick    ��ʱʱ��
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
#ifdef FRAMEWORK_NANO_EXPAND
#define Fw_Task_Delay(tick)     _st(FwTask[FwCore.CurTask].Timer.Reload = 0;   \
                                    FwTask[FwCore.CurTask].Timer.Tick = (tick);)
#else
#define Fw_Task_Delay(tick)     _st(FwTask[FwCore.CurTask].Timer.Tick = (tick);)
#endif

/**
 *******************************************************************************
 * @brief       ��������ʱ��
 * @param       [in/out]  tick    ��ʱʱ��
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
#ifdef FRAMEWORK_NANO_EXPAND
#define Fw_TaskTimer_Start(id, tick)        _st(FwTask[id].Timer.Reload = 0;   \
                                                FwTask[id].Timer.Tick = (tick);)
#else
#define Fw_TaskTimer_Start(id, tick)        _st(FwTask[id].Timer.Tick = (tick);)
#endif

#define Fw_TaskTimer_Stop(id)                  _st(FwTask[id].Timer.Tick = (0);)

 /**
  *******************************************************************************
  * @brief       ����Task����
  * @param       [in/out]  void
  * @return      [in/out]  num    Task����
  * @note        None
  *******************************************************************************
  */
extern uint16_t Fw_Task_Num(void);

/**
 *******************************************************************************
 * @brief       ����Timer����
 * @param       [in/out]  void
 * @return      [in/out]  num    Timer����
 * @note        None
 *******************************************************************************
 */
extern uint16_t Fw_Timer_Num(void);

#ifdef FRAMEWORK_NANO_EXPAND
/**
 *******************************************************************************
 * @brief       �������
 * @param       [in/out]  id     ����ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Task_Ready(uint8_t id);

/**
 *******************************************************************************
 * @brief       ��������
 * @param       [in/out]  id     ����ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Task_Sleep(uint8_t id);

/**
 *******************************************************************************
 * @brief       ��������
 * @param       [in/out]  id     ����ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Task_Yeild(uint8_t id);

/**
 *******************************************************************************
 * @brief       ����״̬
 * @param       [in/out]  id       ����ID
 * @return      [in/out]  state    ����״̬
 * @note        None
 *******************************************************************************
 */
extern uint8_t Fw_Task_State(uint8_t id);
#endif

/**
 *******************************************************************************
 * @brief       ��ʼ����ʱ��
 * @param       [in/out]  id       ��ʱ��ID
 * @param       [in/out]  callback ��ʱ���ص�
 * @param       [in/out]  param    �ص�����
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Timer_Init(uint8_t id, void *callback, void *param);

/**
 *******************************************************************************
 * @brief       ������ʱ��
 * @param       [in/out]  id       ��ʱ��ID
 * @param       [in/out]  tick     ��ʱ��ʱ��
 * @param       [in/out]  flag     ��ʱ������
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Timer_Start(uint8_t id, FwTick_t tick, uint16_t flag);

/**
 *******************************************************************************
 * @brief       ֹͣ��ʱ��
 * @param       [in/out]  id       ��ʱ��ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Timer_Stop(uint8_t id);

/**
 *******************************************************************************
 * @brief       ��ȡ������ʱ״̬
 * @param       [in/out]  evt     ��ǰ�����¼�
 * @return      [in/out]  1       ��ʱ����
 * @return      [in/out]  0       ��ʱ��
 * @note        ���û�����
 *******************************************************************************
 */
#define Fw_Delay_Done(evt)                Fw_Evt_Get(evt, FRAMEWORK_DELAY_EVENT)
     
 /**
 *******************************************************************************
 * @brief       ���������ʱ״̬
 * @param       [in/out]  evt     ��ǰ�����¼�
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
#define Fw_Delay_Clr(evt)                 Fw_Task_Evt_Clr(FRAMEWORK_DELAY_EVENT)

/**
 *******************************************************************************
 * @brief       ��ȡ��ǰ����ֵ
 * @note        ���û�����
 *******************************************************************************
 */ 
#define Fw_Tick_Get()                                              (FwCore.Tick)
  
/**
 *******************************************************************************
 * @brief       ����Tickʱ��
 * @note        None
 *******************************************************************************
 */
#define Fw_Tick_Past(in) (((in) > (FwCore.Tick)) ?                             \
                                  ((FRAMEWORK_TICK_MAX - (in) + FwCore.Tick))  \
                                  : (FwCore.Tick - (in)))
//! �����湦��
extern FwTick_t Fw_Tick_PastWithUpdate(FwTick_t *last);

/**
 *******************************************************************************
 * @brief       �ں�����������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
extern void Fw_Tick_Handle(void);

/**
 *******************************************************************************
 * @brief       �ں�������������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
#ifdef ENABLE_FRAMEWORK_TIMER_REAL
#define Fw_Tick_Inc()                                                          \
                                           _st(FwCore.Tick++; Fw_Tick_Handle();)
#else
#define Fw_Tick_Inc()                                                          \
             _st(FwCore.Tick++; Fw_Evt_Set(FwCore.Event, FRAMEWORK_TICK_EVENT);)
#endif

/**
 *******************************************************************************
 * @brief       ������ʱ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
#define Fw_GenTimer_Start(tim, tick)            _st((tim) = (tick);)
#define Fw_GenTimer_Stop(tim)                   _st((tim) = 0;)
#define Fw_GenTimer_Done(tim)                   ((tim) == 0)
#define Fw_GenTimer_Handle(tim)                 _st(if((tim) > 0)             \
                                                {                             \
                                                    --(tim);                  \
                                                })
#define Fw_GenTimer_HandleWithMsg(tim, id, evt) _st(if((tim) > 0)             \
                                                {                             \
                                                    if(!--(tim))              \
                                                    {                         \
                                                        Fw_Msg_Send(id, evt); \
                                                    }                         \
                                                })
                                                
#define Fw_GenTimer_HandleWithBSPMsg(tim, evt) _st(if((tim) > 0)             \
                                               {                             \
                                                   if(!--(tim))              \
                                                   {                         \
                                                       BSP_Msg_Send(evt);    \
                                                   }                         \
                                               })
/**
 *******************************************************************************
 * @brief       ����ں˳�ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Core_Init(void);

/**
 *******************************************************************************
 * @brief       ������к���
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Core_Startup(void);

#ifdef ENABLE_FRAMEWORK_DEBUG
extern void Fw_Log(char *str, ...);
#endif

/**
 *******************************************************************************
 * @brief        ����Э����ؽӿ�
 *******************************************************************************
 */
#ifdef ENABLE_FRAMEWORK_PROTOTHREAD
//! ProtoThread API
#define Fw_PT_Init(task)                            static uint16_t ptState = 0;
//! ProtoThread API
#define Fw_PT_Begin()                                 switch (ptState) { case 0:
//! ProtoThread API
#define Fw_PT_End()                                             default: break;}
//! ProtoThread API
#define Fw_PT_Enter()                             ptState = (uint16_t)__LINE__;\
                                                                  case __LINE__:
//! ProtoThread API
#define Fw_PT_Wait(state)                                        Fw_PT_Enter();\
                                                 if(state) {return FW_ERR_WAIT;}
//! ProtoThread API
#define Fw_PT_Delay(tick)                                  Fw_Task_Delay(tick);\
                                                   evt = FRAMEWORK_EMPTY_EVENT;\
                                       Fw_PT_Wait(evt != FRAMEWORK_DELAY_EVENT);
//! ProtoThread API
#define Fw_PT_Exit()                                             Fw_PT_Enter();\
                                                             return FW_ERR_NONE;
#else
#define Fw_PT_Init(task)
#define Fw_PT_Begin()
#define Fw_PT_End()
#define Fw_PT_Enter()
#define Fw_PT_Wait(state)
#define Fw_PT_Exit()                                  
#define Fw_PT_Delay(tick)
#endif

/**
 *******************************************************************************
 * @brief        ������־��ؽӿ�
 *******************************************************************************
 */
extern void Fw_Core_Log(char *str, ...);
extern void Fw_Core_Error(char *str, ...);
extern void Fw_Log(char *str, ...);
extern void Fw_Error(char *str, ...);

/**
 *******************************************************************************
 * @brief        ����״̬����ؽӿ�
 *******************************************************************************
 */
enum FSM_RET_STATE
{
	SM_RET_HANDLE = 0,
	SM_RET_SUPER,
	SM_RET_IGNORED,

	SM_RET_DOING,
	SM_RET_TRAN,
	SM_RET_COMPLETE,
	SM_RET_SLEEP,
};

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
	
#endif       /** end include define */

/** @}*/     /** framework core interface  */

/**********************************END OF FILE*********************************/
