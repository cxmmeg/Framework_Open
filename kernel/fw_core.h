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
 * @file       fw_core.h                                                       *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-04-13                                                      *
 * @brief      framework core head files                                       *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.XXXXX                                                                     *
 *******************************************************************************
 */

/**
 * @defgroup framework core interface
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FRAMEWORK_CORE_H__
#define __FRAMEWORK_CORE_H__

/* Add c++ compatibility -----------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "compiler.h"
#include "fw_server.h"

#if defined(FRAMEWORK_VERSION_NANO)
#include "fw_nano.h"
#elif defined(FRAMEWORK_VERSION_FULL)
/**
 *******************************************************************************
 * @brief        ����Э����ؽӿ�
 *******************************************************************************
 */
#ifdef ENABLE_FRAMEWORK_PROTOTHREAD
//! ProtoThread API
#define Fw_PT_Init(task)                           _st((task)->ThreadState = 0;)
//! ProtoThread API
#define Fw_PT_Begin()                  switch (FwCurTask->ThreadState) {case 0:
//! ProtoThread API
#define Fw_PT_End()                                             default: break;}
//! ProtoThread API
#define Fw_PT_Enter()              FwCurTask->ThreadState = (uint16_t)__LINE__;\
                                                                  case __LINE__:
//! ProtoThread API
#define Fw_PT_Wait(state)                                        Fw_PT_Enter();\
                                                            if(state) {return ;}
//! ProtoThread API
#define Fw_PT_Delay(tick)                                  Fw_Task_Delay(tick);\
                                   FwCurTask->ThreadState = (uint16_t)__LINE__;\
                                                                         break;\
                                                                  case __LINE__:
//! ProtoThread API
#define Fw_PT_Exit()                                             Fw_PT_Enter();\
                                                                       return ;
#else
#define Fw_PT_Init(task)
#define Fw_PT_Begin()
#define Fw_PT_End()
#define Fw_PT_Enter()
#define Fw_PT_Wait(state)
#define Fw_PT_Exit()                                  
#define Fw_PT_Delay(tick)
#endif

//! ����ȴ�����
#define Fw_Task_Wait(s)                            _st(if ((s) == 0){return 0;})

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief      ���������Ϣ
 *******************************************************************************
 */
typedef enum
{
    FW_ERR_NONE = 0,
    FW_ERR_FAIL,
    FW_ERR_NOT_READY,
    FW_ERR_WAIT,
    FW_ERR_NOT_SUPPORT,
    FW_ERR_UNINIT,
    FW_ERR_LOCKED,
    FW_ERR_INVAILD_PTR,
    FW_ERR_INVAILD_PARAM,
    FW_ERR_IO,
    FW_ERR_BUG,
    FW_ERR_UNKNOW,
    FW_ERR_OVERFLOW,
    FW_ERR_TIMEOUT,
    FW_ERR_EMPTY,
}FwErr_t;

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

//! �û���������ѡ��
#define FW_TASK_EN_PROTOTHREAD                                              0x01
#define FW_TASK_EN_TIM                                                      0x02

//! ���񴴽�����
#if USE_FRAMEWORK_VERSION == FRAMEWORK_NANO_VERSION
#define FW_TASK_CREATE(name) 
#elif USE_FRAMEWORK_VERSION == FRAMEWORK_GENERAL_VERSION
#define FW_TASK_CREATE(name) FwTask_t name;
#endif

//! ��������������ȼ�
#define FRAMEWORK_MINIMUM_PRIORITY               (FRAMEWORK_TASK_PRIORITY_MAX-1)

//! ��������������ȼ�
#define FRAMEWORK_HIGHEST_PRIORITY                                           (0)

/**
 *******************************************************************************
 * @brief        ��ʱ�����
 *******************************************************************************
 */
//! ��ʱ�����в���       λƫ�Ƶ�ַ
//! = 1: ��������
//! = 0: ��������
#define FW_TIM_EN_PERIOD                                                  _bv(0)

//! ��ʱ������״̬
#define FW_TIM_SLEEP                                                         (0)
#define FW_TIM_DOING                                                         (1)
#define FW_TIM_COMPLETE                                                      (2)

#define FW_TICK_MAX                                                 (0xFFFFFFFF)

//! ��ʱ����������
#define FW_TIM_CREATE(name) FwTimer_t name;

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        timer block
 *******************************************************************************
 */
typedef void (*FwTimerCallback)(void *param);

typedef struct
{
    //! �ص�����
    FwTimerCallback Callback;
    void            *Param;
    
    //! ��ʱ������ֵ
    uint32_t    Timeout;
    uint32_t    Tick;
    
    FwList_t  List;
    
    char        *Name;
    
    //! ��ʱ��״̬����
    uint16_t     Flag;
    uint16_t     State;
}FwTimer_t;

/**
 *******************************************************************************
 * @brief        Framework Core Data Structure
 *******************************************************************************
 */
//! ����������
typedef void (*FwTaskCallback)(void *Param);
typedef struct __FW_TASK
{
    //! ����ص�
    union
    {
        FwTaskCallback  TaskHandle;
        void            *Handle;
    };

    void *Param;

    //! �����б�
    FwList_t List;

	//! ��ѯ��ʱ��
	FwTimer_t Timer;
    
	//! ��������
    char *Name;
    
    //! ��ѯʱ��
    uint32_t Tick;
    
    //! ��������
    uint16_t Flag;

    //! Э��״̬
#ifdef ENABLE_FRAMEWORK_PROTOTHREAD
    uint16_t ThreadState;
#endif

    //! ����״̬
    uint8_t State;
    
    //! �������ȼ�
    uint8_t Priority;
}FwTask_t;

//! �����Ϣ�������
//! ��Ϣ����
typedef struct
{
    uint32_t Event;
    
    union
    {
        void     *Param;
        size_t   Expand;
    };
}FwMsg_t;

//! ��Ϣ����
//! ��Ϣ����ֻ���ɵ�������������Ӧ
typedef struct
{
    FwBuf_t Buffer;
    
    FwTask_t *Task;
}FwMQ_t;

//! ����¼����
//! �¼�����
//! �¼������ɶ������������Ӧ
typedef struct
{ 
    uint32_t   Event;
    
    FwList_t List;
}FwEvt_t;

//! ���嶨ʱ�����
typedef struct
{
	//! ��ʱ������
	FwList_t List;

	//! ����ϵͳ����
	volatile uint32_t Tick;
}FwTimerHandle_t;

//! �ں˾��
typedef struct
{
	//! ��ǰ�����
	FwTask_t *CurTask;

	//! ����������
	void(*IdleHook)(void);

	//! �����б�
	FwList_t ActiveTable[FRAMEWORK_TASK_PRIORITY_MAX];

	//! ���������
	uint8_t ReadyTable[(FRAMEWORK_TASK_PRIORITY_MAX % 8) ? (FRAMEWORK_TASK_PRIORITY_MAX / 8 + 1) : (FRAMEWORK_TASK_PRIORITY_MAX / 8)];

	//! ����¼�
	volatile uint16_t Event;

	//! �ж���
	volatile uint16_t IsrLock;
}FwCore_t;

/* Exported variables --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        �����ں�������
 *******************************************************************************
 */
extern FwCore_t FwCore;

//! ���ڼ����ϴ���
#define FwCurTask                                               (FwCore.CurTask)
#define FwIdleHook                                             (FwCore.IdleHook)
#define FwIsrLock                                               (FwCore.IsrLock)
#define FwCoreEvent                                               (FwCore.Event)
#define FwTaskReadyTable                                     (FwCore.ReadyTable)
#define FwActiveTaskTable                                   (FwCore.ActiveTable)
#define FwCurTaskHandle(evt)             FwCurTask->TaskHandle(FwCurTask->Param)

/**
 *******************************************************************************
 * @brief        ���嶨ʱ��������
 *******************************************************************************
 */
extern FwTimerHandle_t FwTimer;

/* Exported functions --------------------------------------------------------*/
/**
 * @defgroup framework core interface
 * @{
 */
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
 * @brief       ���HAL��ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern int Fw_HAL_Init(void);

/**
 *******************************************************************************
 * @brief       ���MCU֧�ְ���ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern int Fw_MSP_Init(void);

/**
 *******************************************************************************
 * @brief       ���BSP��ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern int Fw_Board_Init(void);

/**
 *******************************************************************************
 * @brief       ��������ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern int Fw_Component_Init(void);

/**
 *******************************************************************************
 * @brief       ���Ӧ�������ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern int Fw_App_Init(void);

/**
 *******************************************************************************
 * @brief       ����������й���
 * @param       [in/out]  *hook       ������й��Ӻ���
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
extern void FwHook_Set_Idle(void *hook);

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

/**
 *******************************************************************************
 * @brief       �����ʼ������
 * @param       [in/out]  *task        ������
 * @param       [in/out]  *name        ��������
 * @param       [in/out]  handle       ������ص�
 * @param       [in/out]  *param       ����ص�����
 * @param       [in/out]  priority     �������ȼ�
 * @param       [in/out]  flag         ��������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Init(FwTask_t *task, char *name, void *handle, void *param, uint8_t priority, uint8_t flag);

/**
 *******************************************************************************
 * @brief       ������������
 * @param       [in/out]  *task                     ������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Start(FwTask_t *task);

/**
 *******************************************************************************
 * @brief       ������ʱ����
 * @param       [in/out]  tick    ������ʱʱ��
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Delay(uint32_t tick);

/**
 *******************************************************************************
 * @brief       ������������
 * @param       [in/out]  tick       ������ʱʱ��
 * @param       [in/out]  *handle    ������ʱ������
 * @param       [in/out]  *param     ������ʱ�������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Yeild(uint32_t timeout, void *handle, void *param);

/**
 *******************************************************************************
 * @brief       �������ߺ���
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Sleep(FwTask_t *task);

/**
 *******************************************************************************
 * @brief       ����������Ϊ����״̬
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Ready(FwTask_t *task);

/**
 *******************************************************************************
 * @brief       �����Ѻ���
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Wakeup(FwTask_t *task);

/**
 *******************************************************************************
 * @brief       ����ע������
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Task_Fini(FwTask_t *task);

/**
 *******************************************************************************
 * @brief       ��Ϣ��ʼ������
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @param       [in/out]  *buf        ����
 * @param       [in/out]  len         ����������
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_MQ_Init(FwMQ_t *queue, void *buf, uint16_t len);

/**
 *******************************************************************************
 * @brief       ��Ϣ����ע������
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_MQ_Fini(FwMQ_t *queue);

/**
 *******************************************************************************
 * @brief       ��Ϣ���ͺ���
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @param       [in/out]  *msg        ������Ϣ
 * @param       [in/out]  size        ������Ϣ����
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern uint16_t Fw_MQ_Send(FwMQ_t *queue, void *msg, uint16_t size);

/**
 *******************************************************************************
 * @brief       ��Ϣ���պ���
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @param       [in/out]  *msg        ��Ϣ���ջ�����
 * @param       [in/out]  size        ��Ϣ��С
 * @param       [in/out]  tick        ����ʱ��
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
//! �ɿ��API����
extern FwErr_t Fw_MQ_Recv(FwMQ_t *queue, void *msg, uint16_t size, size_t tick);
extern FwErr_t Fw_MQ_Recv_Mirror(FwMQ_t *queue, void *msg, uint16_t size);

//! ���û��������
#define Fw_MQ_Read(queue, msg, size) _st(if(Fw_MQ_Recv((queue), (void *)(msg), (size), 0) == FW_ERR_EMPTY) {break;})
#define Fw_MQ_Read_Mirror(queue, msg, size) _st(if(Fw_MQ_Recv_Mirror((queue), (void *)(msg), (size)) == FW_ERR_EMPTY) {break;})

/** @}*/     /** framework core interface */

/**
 * @defgroup framework timer interface
 * @{
 */

/**
 *******************************************************************************
 * @brief       ��ʱ�������ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ��Fw_Core_Init�������ã������û�����
 *******************************************************************************
 */
extern int FwTimer_Component_Init(void);

/**
 *******************************************************************************
 * @brief       ��ȡ��ʱ���б�ͷ
 * @param       [in/out]  void
 * @return      [in/out]  FwList_t*    ��ʱ���б�ͷ
 * @note        None
 *******************************************************************************
 */
extern FwList_t *FwTimer_List_Root(void);

/**
 *******************************************************************************
 * @brief       ��ʱ�������ʼ������
 * @param       [in/out]  timer        ��ʱ�����
 * @param       [in/out]  name         ��ʱ������
 * @param       [in/out]  callback     ��ʱ���ص�
 * @param       [in/out]  param        �ص�����
 * @param       [in/out]  tick         ��ʱ����ʱʱ��
 * @param       [in/out]  flag         ��ʱ��������־
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Timer_Init(FwTimer_t *timer, char *name, FwTimerCallback callback, void *param, uint32_t tick, uint8_t flag);

/**
 *******************************************************************************
 * @brief       ���ö�ʱ���ص�����
 * @param       [in/out]  *timer       ��ʱ�����
 * @param       [in/out]  callback     ��ʱ���ص�
 * @param       [in/out]  *param       �ص�����
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Timer_Set_Callback(FwTimer_t *timer, FwTimerCallback callback, void *param);

/**
 *******************************************************************************
* @brief        ��ʱ��ע������
 * @param       [in/out]  *timer      ��ʱ�����
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Timer_Fini(FwTimer_t *timer);

/**
 *******************************************************************************
 * @brief       ��ʱ����������
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Timer_Start(FwTimer_t *timer);

/**
 *******************************************************************************
 * @brief       ��ʱ����������
 * @param       [in/out]  timer        ��ʱ�����
 * @param       [in/out]  tick         ��ʱ����ʱʱ��
 * @param       [in/out]  flag         ��ʱ������ģʽ
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
extern void Fw_Timer_Restart(FwTimer_t *timer, uint32_t tick, uint8_t flag);
    
/**
 *******************************************************************************
 * @brief       ��ȡ��ʱ��״̬
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern uint16_t Fw_Timer_State(FwTimer_t *timer);

/**
 *******************************************************************************
 * @brief       ��ȡ��ʱ�����״̬
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern uint8_t Fw_Timer_Done(FwTimer_t *timer);

/**
 *******************************************************************************
 * @brief       ��ʱ��ֹͣ����
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Timer_Stop(FwTimer_t *timer);

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
												
/**
 *******************************************************************************
 * @brief       ��ʱ��������������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern void Fw_Tick_Inc(void);

/**
 *******************************************************************************
 * @brief       ��ȡ��ǰ����ֵ
 * @param       [in/out]  void
 * @return      [in/out]  uint32_t    ��ǰ����ֵ
 * @note        �û��ɵ���
 *******************************************************************************
 */
extern uint32_t Fw_Tick_Get(void);

/**
 *******************************************************************************
 * @brief       ��������ϸ�ʱ��������ֵ
 * @param       [in/out]  lastTick    �ϸ�ʱ���
 * @return      [in/out]  uint32_t    ���ŵ�ʱ��
 * @note        None
 *******************************************************************************
 */
extern uint32_t Fw_Tick_Past(uint32_t lastTick);

/**
 *******************************************************************************
 * @brief       ��ʱ��������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ��Fw_Core_Start����
 *******************************************************************************
 */
extern void FwTimer_Component_Handle(void);
/** @}*/     /** framework timer interface */

/**
 * @defgroup framework debug interface
 * @{
 */

/**
 *******************************************************************************
 * @brief       ���������ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ��Fw_Core_Start����
 *******************************************************************************
 */
extern int Fwdbg_Component_Init(void);

#ifdef ENABLE_FRAMEWORK_DEBUG
/**
 *******************************************************************************
 * @brief       ����ʱ�������ʼ����
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_TimeTest_Begin(void);

/**
 *******************************************************************************
 * @brief       ����ʱ����Խ�������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_TimeTest_End(void);

/**
 *******************************************************************************
 * @brief       ��ʱ�����Ժ�������ѹ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Timer_Test(void);

/**
 *******************************************************************************
 * @brief       �ں������־API
 * @param       [in/out]  str     ��־��Ϣ
 * @param       [in/out]  ...     ��־����
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Core_Log(char *str, ...);

/**
 *******************************************************************************
 * @brief       �ں��������API
 * @param       [in/out]  str     ��־��Ϣ
 * @param       [in/out]  ...     ��־����
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Core_Error(char *str, ...);

/**
 *******************************************************************************
 * @brief       ��־���API
 * @param       [in/out]  str     ��־��Ϣ
 * @param       [in/out]  ...     ��־����
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Log(char *str, ...);

/**
 *******************************************************************************
 * @brief       �������API
 * @param       [in/out]  str     ��־��Ϣ
 * @param       [in/out]  ...     ��־����
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Error(char *str, ...);

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

/**
 *******************************************************************************
 * @brief       ����ں���ϢAPI
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Put_Info(void);

/**
 *******************************************************************************
 * @brief       ��������б�API
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Put_Task_List(void);

/**
 *******************************************************************************
 * @brief       �����ʱ���б�API
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Put_Timer_List(void);

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
 * @brief       ��ʱ�����Ժ�������ѹ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
extern void Fw_Timer_Test(void);
#else
#define Fw_TimeTest_Begin()
#define Fw_TimeTest_End()
#define Fw_Timer_Test()
#define Fw_Core_Log(s, ...)
#define Fw_Core_Error(s, ...)
#define Fw_Log(s, ...)
#define Fw_Error(s, ...)
#define Fw_Put_Info()
#define Fw_Event_General()
#define Fw_Timer_Test()
#endif

/** @}*/     /** framework debug interface */

#endif

/* Exported macro ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       ����������汾
 *******************************************************************************
 */
#define FRAMEWORK_SOFT_VERSION                                           "1.3.3"

/**
 *******************************************************************************
 * @brief       �����ܱ�������
 *******************************************************************************
 */
#define FRAMEWORK_BUILD_DATE                                        "2019-03-22"

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
 * @brief      MACRO
 *******************************************************************************
 */
#define Fw_Evt_Set(b, pos)                                      _set_bit(b, pos)
#define Fw_Evt_Clr(b, pos)                                      _clr_bit(b, pos)
#define Fw_Evt_Get(b, pos)                                     _read_bit(b, pos)

#define Fw_Flag_Set(b, flag)                                   _set_reg(b, flag)
#define Fw_Flag_Clr(b, flag)                                   _clr_reg(b, flag)
#define Fw_Flag_Get(b, flag)                                   _get_reg(b, flag)

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
	
#endif       /** end include define */

/** @}*/     /** framework core interface  */

/**********************************END OF FILE*********************************/
