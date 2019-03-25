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
 * @file       fw_core.c                                                       *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-04-13                                                      *
 * @brief      framework core interface source files                           *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.XXXXX                  						                           *
 *******************************************************************************
 */

/**
 * @defgroup framework core interface
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "fw_core.h"
#include <string.h>

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       bit map
 *******************************************************************************
 */
static uint8_t __CONST FwBitMap[] =
{
	/* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
};

/* Exported variables --------------------------------------------------------*/
//! ����¼����
FwCore_t FwCore;

/* Private functions ---------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       ����ʱ��Ƭ��ѯ������
 * @param       [in/out]  *param       ����ص�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void FwTask_Timer_Handle(void *param)
{
    FwTask_t *task = (FwTask_t *)param;
    
    Fw_Task_Ready(task);
}

/**
 *******************************************************************************
 * @brief       ������ʱ������
 * @param       [in/out]  *param       ����ص�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
static 
void FwTask_Delay_Handle(void *param)
{
    FwTask_t *task = (FwTask_t *)param;
    
    Fw_Task_Ready(task);
}

/**
 *******************************************************************************
 * @brief       ������������
 * @param       [in/out]  *param       ����ص�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
static 
void FwTask_Idle_Handle(void)
{
#ifdef FW_PUT_IDLE_INFO
	Fw_Core_Log("Framework Is Idle");
#endif

	if (!IS_PTR_NULL(FwIdleHook))
	{
		FwIdleHook();
	}
}

/**
 *******************************************************************************
 * @brief       ��Ϣ���䶨ʱ��������
 * @param       [in/out]  *param      ����
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
static
void FwMQ_Timer_Callback(void *param)
{
    FwMQ_t *queue = (FwMQ_t *)param;
    
    if (queue->Task->State == FW_TASK_YEILD)
    {
        //! ��Ϣ�Ѹ���
        if (FwBuf_Used(&queue->Buffer) >= sizeof(FwMsg_t))
        {
            Fw_Task_Ready(queue->Task);
            
            queue->Task = NULL;
        }
        else
        {
            queue->Task->State = FW_TASK_SLEEP;
        }
    }
}

/* Exported functions --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       �����ٽ�㺯��
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Enter_Critical(void)
{
    if (FwIsrLock == 0)
    {
        Fw_IRQ_Disable();

        FwIsrLock++;
    }
    else 
    {
        if (FwIsrLock < 0xFFFF)
        {
            FwIsrLock++;
        }
    }
}

/**
 *******************************************************************************
 * @brief       �˳��ٽ�㺯��
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Exit_Critical(void)
{
    if (FwIsrLock)
    {
        FwIsrLock --;
        
        if (FwIsrLock == 0)
        {
            Fw_IRQ_Enable();
        }
    }
}

/**
 *******************************************************************************
 * @brief       �����ʼ������
 * @param       [in/out]  *task        ������
 * @param       [in/out]  *name        ��������
 * @param       [in/out]  handle       ������ص�
 * @param       [in/out]  *param       ����ص�����
 * @param       [in/out]  priority     �������ȼ�
 * @param       [in/out]  priority     �������ȼ�
 * @param       [in/out]  flag         ��������
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Task_Init(FwTask_t *task, char *name, void *handle, void *param, uint8_t priority, uint8_t flag)
{
	memset(task, 0, sizeof(FwTask_t));
    
	if (priority >= FRAMEWORK_TASK_PRIORITY_MAX)
	{
#ifdef FW_PUT_INIT_INFO
		Fw_Core_Error("[FWERR-001]Task Name:%s, Set Priority:%d, New Priority:%d! \r\n", name, priority, (FRAMEWORK_TASK_PRIORITY_MAX - 1));
#endif

		priority = FRAMEWORK_TASK_PRIORITY_MAX - 1;
	}

    task->Handle   = handle;
    task->Param    = param;
    task->Priority = priority;
    task->Flag     = flag;
	task->Name     = name;

	//! ����ʱ����ʼ��
	Fw_Timer_Init(&task->Timer, task->Name, FwTask_Delay_Handle, task, 0, 0);

    //! ������������״̬ģʽ
#ifdef ENABLE_FRAMEWORK_PROTOTHREAD
    if (Fw_Flag_Get(task->Flag, FW_TASK_EN_PROTOTHREAD))
    {
        Fw_PT_Init(task);
    }
#endif
    
    FwList_Init(&task->List);
}

/**
 *******************************************************************************
 * @brief       ������������
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Task_Start(FwTask_t *task)
{
    Fw_Task_Ready(task);
}

/**
 *******************************************************************************
 * @brief       ������ʱ����
 * @param       [in/out]  tick    ������ʱʱ��
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Task_Delay(uint32_t tick)
{
    //! ��������
    FwCurTask->State = FW_TASK_DELAY;

    //! ���ó�ʱ��ʱ��
    Fw_Timer_Set_Callback(&FwCurTask->Timer, FwTask_Delay_Handle, (void *)FwCurTask);
    Fw_Timer_Restart(&FwCurTask->Timer, tick, 0);
}

/**
 *******************************************************************************
 * @brief       ������������
 * @param       [in/out]  tick       ������ʱʱ��
 * @param       [in/out]  *handle    ������ʱ������
 * @param       [in/out]  *param     ������ʱ�������
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Task_Yeild(uint32_t timeout, void *handle, void *param)
{
    FwCurTask->State = FW_TASK_YEILD;
    
    Fw_Timer_Set_Callback(&FwCurTask->Timer, (FwTimerCallback)handle, param);
    Fw_Timer_Restart(&FwCurTask->Timer, timeout, 0);
}

/**
 *******************************************************************************
 * @brief       �������ߺ���
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Task_Sleep(FwTask_t *task)
{
    task->State = FW_TASK_SLEEP;

    FwList_Remove(&task->List);
}

/**
 *******************************************************************************
 * @brief       ����������Ϊ����״̬
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__INLINE
void Fw_Task_Ready(FwTask_t *task)
{
    FwList_t *p;
    FwList_t *head;
    
	Fw_Enter_Critical();

#ifdef FW_PUT_TASK_READY_INFO
	Fw_Log("[Begin Task Process]\r\n");
	Fw_Put_Task_List();
#endif

    //! ��������
    if (task->State == FW_TASK_INIT     \
		|| task->State == FW_TASK_DELAY \
        || task->State == FW_TASK_YEILD \
        || task->State == FW_TASK_SLEEP)
    {
        task->State = FW_TASK_READY;
    }

    //! �Ƴ�����
    FwList_Remove(&task->List);
    
    //! ���Ҳ���
    for (head = &FwActiveTaskTable[task->Priority], p = head->Next; p != head; p = p->Next);
    
    //! ��ĩβ����
	FwList_InsertBefore(p, &task->List);
	
	//! ��������������������
    _mem_bit_set(FwTaskReadyTable, 8, task->Priority);

#ifdef FW_PUT_TASK_READY_INFO
	Fw_Log("[End Task Process]\r\n");
	Fw_Put_Task_List();
#endif

	Fw_Exit_Critical();
}

/**
 *******************************************************************************
 * @brief       �����Ѻ���
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Task_Wakeup(FwTask_t *task)
{
    Fw_Task_Ready(task);
}

/**
 *******************************************************************************
 * @brief       ����ע������
 * @param       [in/out]  *task        ������
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Task_Fini(FwTask_t *task)
{
    //! todo: �����
}

/**
 *******************************************************************************
 * @brief       ��ȡ��ǰ��������λ
 * @param       [in/out]  flag      ��Ҫ��ȡ������λ
 * @return      [in/out]  return    ��ȡ��������λ
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
uint32_t Fw_Self_Flag_Get(uint32_t flag)
{
    return Fw_Flag_Get(FwCurTask->Flag, flag);
}

/**
 *******************************************************************************
 * @brief       ��Ϣ��ʼ������
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @param       [in/out]  *buf        ����
 * @param       [in/out]  len         ����������
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_MQ_Init(FwMQ_t *queue, void *buf, uint16_t len)
{
    FwBuf_Init(&queue->Buffer, (uint8_t *)buf, len);
    
    queue->Task = NULL;
}

/**
 *******************************************************************************
 * @brief       ��Ϣ����ע������
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_MQ_Fini(FwMQ_t *queue)
{
    FwBuf_Fini(&queue->Buffer);
}

/**
 *******************************************************************************
 * @brief       ��Ϣ���ͺ���
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @param       [in/out]  *msg        ������Ϣ
 * @param       [in/out]  size        ������Ϣ����
 * @return      [in/out]  size        ������Ϣ����
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
uint16_t Fw_MQ_Send(FwMQ_t *queue, void *msg, uint16_t size)
{
    uint16_t result = FwBuf_Write(&queue->Buffer, (uint8_t *)msg, size);
    
    if (!IS_PTR_NULL(queue->Task))
    {
        if (queue->Task->State != FW_TASK_DELAY)
        {
            Fw_Task_Ready(queue->Task);
            queue->Task = NULL;
        }
    }
    
    return result;
}

/**
 *******************************************************************************
 * @brief       ��Ϣ���պ���
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @param       [in/out]  *msg        ��Ϣ���ջ�����
 * @param       [in/out]  size        ��Ϣ��С
 * @param       [in/out]  tick        ����ʱ��
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
FwErr_t Fw_MQ_Recv(FwMQ_t *queue, void *msg, uint16_t size, size_t tick)
{
    if (FwBuf_Read(&queue->Buffer, (uint8_t *)msg, size) != size)
    {
        //! ���ö��еȴ�����
        queue->Task = FwCurTask;

        if (tick > 0)
        {
            Fw_Task_Yeild(tick, (void *)FwMQ_Timer_Callback, (void *)queue);
        }
        else
        {
            Fw_Task_Sleep(FwCurTask);
        }
        
        return FW_ERR_EMPTY;
    }
    else
    {
        return FW_ERR_NONE;
    }
}

/**
 *******************************************************************************
 * @brief       ��Ϣ���պ���
 * @param       [in/out]  *queue      ��Ϣ���о��
 * @param       [in/out]  *msg        ��Ϣ���ջ�����
 * @param       [in/out]  size        ��Ϣ��С
 * @param       [in/out]  tick        ����ʱ��
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
FwErr_t Fw_MQ_Recv_Mirror(FwMQ_t *queue, void *msg, uint16_t size)
{
    if (FwBuf_ReadMirror(&queue->Buffer, (uint8_t *)msg, size) != size)
    {       
        //! ���ö��еȴ�����
        queue->Task = FwCurTask;
        
        Fw_Task_Sleep(FwCurTask);

        return FW_ERR_EMPTY;
    }
    else
    {
        return FW_ERR_NONE;
    }
}

/**
 *******************************************************************************
 * @brief       ����������й���
 * @param       [in/out]  *hook       ������й��Ӻ���
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void FwHook_Set_Idle(void *hook)
{
	FwIdleHook = (void(*)(void))hook;
}

/**
 *******************************************************************************
 * @brief       ����ں˳�ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Core_Init(void)
{
    uint16_t i;
    
    //! ��ʼ���ж���
    FwIsrLock = 0;

    Fw_Enter_Critical();
    
    //! ��ʼ��ϵͳ�¼�
    FwCoreEvent = 0;
    
    //! ��ʼ�����������
    for (i = 0; i<_dimof(FwActiveTaskTable); i++)
    {
        FwList_Init(&FwActiveTaskTable[i]);
    }

    for (i=0; i<_dimof(FwTaskReadyTable); i++)
    {
        FwTaskReadyTable[i] = 0;
    }

    //! ��ʼ����ǰ����ָ��
    FwCurTask = NULL;

	//! ���ϵͳ��Ϣ
	Fw_Put_Info();

    //! ��������ʼ��
    Fw_Component_Init();

    //! HAL��ʼ��
    Fw_HAL_Init();

	//! Ӳ����ʼ��
    Fw_Board_Init();

	//! Ӧ�������ʼ��
    Fw_App_Init();

    Fw_Exit_Critical();
}

/**
 *******************************************************************************
 * @brief       ϵͳ�ں���������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Core_Startup(void)
{
    uint16_t prio;
    uint16_t i;
    FwList_t *list;
    
    while (1)
    {
#ifndef ENABLE_FRAMEWORK_TIMER_REAL
        //! ����ʱ������
        if (Fw_Evt_Get(FwCoreEvent, FRAMEWORK_TICK_EVENT))
        {
            Fw_Enter_Critical();
        
            Fw_Evt_Clr(FwCoreEvent, FRAMEWORK_TICK_EVENT);
            
            Fw_Exit_Critical();
            
            FwTimer_Component_Handle();
        }
#endif

        //! 1. ��ȡ��������
        for (i=0; i<_dimof(FwTaskReadyTable); i++)
        {
            if (FwTaskReadyTable[i])
            {
                prio = FwBitMap[FwTaskReadyTable[i]] + (i * 8);
                
                if (prio >= FRAMEWORK_TASK_PRIORITY_MAX)
                {
                    goto __CLEAR_TASK_READY_FLAG;
                }
                
                //! 2. ��ȡ����������
                list = FwList_DeleteAfter(&FwActiveTaskTable[prio]);
                
                if (IS_PTR_NULL(list))
                {
                    goto __CLEAR_TASK_READY_FLAG;
                }
                
                FwCurTask = (FwTask_t *)FwListEntry(list, FwTask_t, List);
                
                //! 3. ִ�о�������
                if (!IS_PTR_NULL(FwCurTask))
                {
                    if (!IS_PTR_NULL(FwCurTask->Handle))
                    {
                        FwCurTask->TaskHandle(FwCurTask->Param);
                    }
                    
                    FwCurTask = NULL;
                }
                
                //! 4. �������������־
                if (FwList_IsEmpty(&FwActiveTaskTable[prio]))
                {
                    goto __CLEAR_TASK_READY_FLAG;
                }
                
                break;
                
                //! 5. ������������־
__CLEAR_TASK_READY_FLAG:
                _mem_bit_clr(FwTaskReadyTable, 8, prio);
                
                break;
            }
        }

		//! ����������
		FwTask_Idle_Handle();
    }
}

/**
 *******************************************************************************
 * @brief       Ӳ�������ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        �ɿ�ܵ���
 *******************************************************************************
 */
#ifdef ENABLE_FRAMEWORK_COMPONENT_INIT
/*
 * Components Initialization will initialize some driver and components as following
 * order:
 * rti_start                --> 0
 * FW_COMPONENT_EXPORT      --> 1
 * rti_fw_component_end     --> 1.end
 *
 * DEVICE_EXPORT            --> 2
 * COMPONENT_EXPORT         --> 3
 * FS_EXPORT                --> 4
 * ENV_EXPORT               --> 5
 * APP_EXPORT               --> 6
 *
 * rti_end                  --> 6.end
 *
 * These automatically initializaiton, the driver or component initial function must
 * be defined with:
 * INIT_BOARD_EXPORT(fn);
 * INIT_DEVICE_EXPORT(fn);
 * ...
 * INIT_APP_EXPORT(fn);
 * etc.
 */
static int fw_init_start(void)
{
    return 0;
}
INIT_EXPORT(fw_init_start, "0");

static int fw_component_init_start(void)
{
    return 0;
}
INIT_EXPORT(fw_component_init_start, "0.end");

static int fw_component_init_end(void)
{
    return 0;
}
INIT_EXPORT(fw_component_init_end, "1.end");

static int fw_hal_init_end(void)
{
    return 0;
}
INIT_EXPORT(fw_hal_init_end, "2.end");

static int fw_board_init_end(void)
{
    return 0;
}
INIT_EXPORT(fw_board_init_end, "3.end");

static int fw_app_init_end(void)
{
    return 0;
}
INIT_EXPORT(fw_app_init_end, "4.end");

static int fw_init_end(void)
{
    return 0;
}
INIT_EXPORT(fw_init_end, "6.end");

int Fw_Component_Init(void)
{
    const init_fn_t *fn;

    for (fn = &f_fw_component_init_start; fn<&f_fw_component_init_end; fn++)
    {
        (*fn)();
    }
    
    return 0;
}

int Fw_HAL_Init(void)
{
    const init_fn_t *fn;

    for (fn = &f_fw_component_init_end; fn<&f_fw_hal_init_end; fn++)
    {
        (*fn)();
    }
    
    return 0;
}

int Fw_Board_Init(void)
{
    const init_fn_t *fn;

    for (fn = &f_fw_hal_init_end; fn<&f_fw_board_init_end; fn++)
    {
        (*fn)();
    }
    
    return 0;
}

int Fw_App_Init(void)
{
    const init_fn_t *fn;

    for (fn = &f_fw_board_init_end; fn<&f_fw_app_init_end; fn++)
    {
        (*fn)();
    }
    
    return 0;
}
#else

__WEAK
int Fw_HAL_Init(void)
{
	return 0;
}

__WEAK
int Fw_Component_Init(void)
{
	FwTimer_Component_Init();

#ifdef ENABLE_FRAMEWORK_DEBUG
	Fwdbg_Component_Init();
#endif

	return 0;
}

#endif

/** @}*/     /** framework core interface */

/**********************************END OF FILE*********************************/
