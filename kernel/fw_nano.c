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
 * @file       fw_nano.c                                                       *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-01-10                                                      *
 * @brief      framework core nano version interface source files              *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.20170110    �����ļ�fw_event.c         		                           *
 * 2.20180918    �޸��ļ��ܹ�������Ϊfw_nano.c                                 *
 * 2.20180920    �޸�������ȷ�ʽ�������¼����������޸�Ϊ�����������ȼ���      *
 *               �¼��������ȷ�ʽ                                              *
 *******************************************************************************
 */

/**
 * @defgroup framework nano version interface
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "fw_core.h"
#include <string.h>

/* Exported macro ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        ������������
 *******************************************************************************
 */
#define FRAMEWORK_TASK_MAX                                  (_dimof(FwTaskList))
#define FRAMEWORK_TASK_INIT_MAX                         (_dimof(FwTaskInitList))

/**
 *******************************************************************************
 * @brief        ��ʱ����������
 *******************************************************************************
 */
#define FRAMEWORK_TIMER_MAX                                   FRAMEWORK_TASK_MAX

/**
 *******************************************************************************
 * @brief        ��Ϣ������������
 *******************************************************************************
 */
#define FRAMEWORK_MSG_QUEUE_MAX     (FRAMEWORK_TASK_MAX*FRAMEWORK_MQ_SIZE_PARAM)

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        ��Ϣ�����ڴ涨��
 *******************************************************************************
 */
typedef struct
{
    uint8_t Msg;
    uint8_t Next;
}FwQueue_Mem_t;

/**
 *******************************************************************************
 * @brief        ��Ϣ�����ڴ�����������
 *******************************************************************************
 */
typedef struct
{
    uint8_t Head;
    uint8_t Tail;
}FwQueue_Mgr_t;

/* Private variables ---------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        ����λͼ 16Bit
 *******************************************************************************
 */
__CONST uint8_t FwNanoMap[] = {255, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};

/* Exported variables --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        �ں˾��
 *******************************************************************************
 */
#ifdef USE_KEIL_C51_COMPILER
__DATA FwHandle_t FwCore;
#else
FwHandle_t FwCore;
#endif

/**
 *******************************************************************************
 * @brief        ��������
 *******************************************************************************
 */
//! �����б�
__CODE FwTaskInit FwTaskInitList[] = {Fw_Task_Init_Array};
__CODE FwTaskHandle FwTaskList[]   = {Fw_Task_Handle_Array};

//! ������
#ifdef USE_KEIL_C51_COMPILER
FwTask_t __DATA FwTask[FRAMEWORK_TASK_MAX];
#else
FwTask_t FwTask[FRAMEWORK_TASK_MAX];
#endif

/**
 *******************************************************************************
 * @brief        �����¼�����
 *******************************************************************************
 */
//! ���л�����
FwQueue_Mem_t __DATA FwMsgMemory[FRAMEWORK_MSG_QUEUE_MAX];
//! ���л���������
FwQueue_Mgr_t __DATA FwMsgMgr[FRAMEWORK_TASK_MAX];
//! ״̬
uint8_t __DATA FwMsgMemFlag[(FRAMEWORK_MSG_QUEUE_MAX % 8) ? (FRAMEWORK_MSG_QUEUE_MAX/8 + 1) : (FRAMEWORK_MSG_QUEUE_MAX/8)];

/* Private functions ---------------------------------------------------------*/
#ifdef FRAMEWORK_NANO_EXPAND
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
    Fw_IRQ_Disable();
    
    if (FwCore.IsrLock == 0)
    {
        FwCore.IsrLock++;
    }
    else 
    {
        if (FwCore.IsrLock < 0xFFFFFFFF)
        {
            FwCore.IsrLock++;
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
    if (FwCore.IsrLock)
    {
        FwCore.IsrLock --;
        
        if (FwCore.IsrLock == 0)
        {
            Fw_IRQ_Enable();
        }
    }
}
#endif

/**
 *******************************************************************************
 * @brief       ��Ϣ���г�ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Msg_Init(void)
{
    uint8_t i = 0;
    
    //! ��ʼ����Ϣ����
    for (i=0; i<FRAMEWORK_MSG_QUEUE_MAX; i++)
    {
        FwMsgMemory[i].Next = FW_MSG_ADDR_NULL;
        FwMsgMemory[i].Msg  = 0;
    }
    
    //! ��ʼ��������Ϣ����
    for (i=0; i<FRAMEWORK_TASK_MAX; i++)
    {
        FwMsgMgr[i].Head = FW_MSG_ADDR_NULL;
        FwMsgMgr[i].Tail = FW_MSG_ADDR_NULL;
    }
    
    //! ��ʼ����Ϣ�ڴ�״̬
    for (i=0; i<_dimof(FwMsgMemFlag); i++)
    {
        FwMsgMemFlag[i] = 0xFF;
    }
}

/**
 *******************************************************************************
 * @brief       ��Ϣ�����ڴ���亯��
 * @param       [in/out]  void
 * @return      [in/out]  FW_MSG_ADDR_NULL    ����ʧ��
 * @return      [in/out]  addr                �����ַ
 * @note        ���û�����
 *******************************************************************************
 */
uint8_t Fw_Msg_Alloc(void)
{
    uint8_t isr  = 0;

    uint8_t addr = FW_MSG_ADDR_NULL;
    uint8_t i = 0;
    uint8_t low, high;
    
    Fw_Atom_Begin(isr);
    
    //! �����ڴ������ڴ��ַ
    do
    {
        if (FwMsgMemFlag[i])
        {
            low  = FwMsgMemFlag[i] & 0x0F;
            high = FwMsgMemFlag[i] >> 4;
            
            if ((addr = FwNanoMap[low]) <= 3)
            {
                //! do nothing!
            }
            else if ((addr = FwNanoMap[high]) <= 3)
            {
                addr += 4;
            }
            else
            {
                continue;
            }

            _clr_bit(FwMsgMemFlag[i], addr);
            
            addr += i * 8;
            
            if (addr < FRAMEWORK_MSG_QUEUE_MAX)
            {
                break;
            }
			else
			{
				addr = FW_MSG_ADDR_NULL;
			}
        }
    }while(++i < _dimof(FwMsgMemFlag));

#ifdef FW_PUT_MSG_ALLOC_INFO
	if (addr >= FRAMEWORK_MSG_QUEUE_MAX)
	{
		Fw_Core_Log("Msg Memory Alloc Addr Is Overflow");
	}
	else
	{
		Fw_Core_Log("Msg Memory Alloc Addr Is %d", (uint8_t)addr);
	}
#endif

    Fw_Atom_End(isr);
    
    return addr;
}

/**
 *******************************************************************************
 * @brief       ��Ϣ�����ڴ��ͷź���
 * @param       [in/out]  id     �ڴ��ַ
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
#define Fw_Msg_Free(addr)         _set_bit(FwMsgMemFlag[(addr) / 8], (addr) % 8)

/**
  *******************************************************************************
  * @brief       �����Ϣ�б���Ϣ
  * @param       [in/out]  void
  * @return      [in/out]  void
  * @note        ���û�����
  *******************************************************************************
  */
#ifdef FW_PUT_MSG_CMPT_INFO
void Fw_Put_Msg_List(void)
{
	for (int i = 0; i < _dimof(FwMsgMgr); i++)
	{
		int head = FwMsgMgr[i].Head;
		int tail = FwMsgMgr[i].Tail;
		int now  = head;
		int len;

		if (head == 0xFF)
		{
			printf("Task %d Msg Head: Empty\r\n", (uint8_t)i);
		}
		else
		{
			printf("Task %d Msg Head: %d\r\n", (uint8_t)i, (uint8_t)head);
		}
		
		if (tail == 0xFF)
		{
			printf("Task %d Msg Tail: Empty\r\n", (uint8_t)i);
		}
		else
		{
			printf("Task %d Msg Tail: %d\r\n", (uint8_t)i, (uint8_t)tail);
		}

		printf("Put Message List:");

		len = 0;

		while (1)
		{
			if (now == 0xFF)
			{
				printf("\r\nEND, Msg List Len��%d\r\n\r\n", (uint8_t)len);

				break;
			}
			else
			{
				printf("Mem[%d]", (uint8_t)now);

				len++;
			}

			now = FwMsgMemory[now].Next;

			if (now != 0xFF)
			{
				printf(" -> ");
			}
		}
	}
}
#endif

/* Exported functions --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       ��Ϣ���ͺ���
 * @param       [in/out]  id        ����ID
 * @param       [in/out]  msg       ��Ϣ
 * @return      [in/out]  0         ���ͳɹ�
 * @return      [in/out]  1         ����ʧ��
 * @note        ���û�����
 *******************************************************************************
 */
uint8_t Fw_Msg_Send(uint8_t id, uint8_t msg)
{
    uint8_t isr  = 0;
    uint8_t mem  = Fw_Msg_Alloc();
    uint8_t head = FwMsgMgr[id].Head;
    uint8_t tail = FwMsgMgr[id].Tail;
    
    if (mem >= FRAMEWORK_MSG_QUEUE_MAX || id >= FRAMEWORK_TASK_MAX)
    {
		return 1;
    }

#ifdef FW_PUT_MSG_CMPT_INFO
	Fw_Core_Log("\r\n********************Alloc Message Memory Begin, Dst Task Id:%d, Message:%d********************", (uint8_t)id, (uint8_t)msg);
	Fw_Put_Msg_List();
#endif

    Fw_Atom_Begin(isr);
    
    //! ������Ϣ����
    if(head == FW_MSG_ADDR_NULL && tail == FW_MSG_ADDR_NULL)
    {
        FwMsgMgr[id].Head = mem;
        FwMsgMgr[id].Tail = mem;
    }
    else
    {
        FwMsgMemory[tail].Next = mem;
        FwMsgMgr[id].Tail      = mem;
    }
    
    //! д����Ϣ
    FwMsgMemory[mem].Msg  = msg;
    FwMsgMemory[mem].Next = FW_MSG_ADDR_NULL;
    
#ifdef FW_PUT_MESSAGE_CHECK_INFO
	if (msg == 0)
	{
		Fw_Core_Error("Task %d Send Message Error", (uint8_t)id);
	}
#endif

    Fw_Evt_Set(FwTask[id].Event, FRAMEWORK_MSG_EVENT);
    
#ifdef FW_PUT_MSG_CMPT_INFO
	Fw_Core_Log("\r\nAlloc Message Memory End");
	Fw_Put_Msg_List();
	printf("********************************************************************************\r\n\r\n\r\n");
#endif

    Fw_Atom_End(isr);
    
    return 0;
}

/**
 *******************************************************************************
 * @brief       ��Ϣ���պ���
 * @param       [in/out]  id         ����ID
 * @return      [in/out]  0          ��Ϣ����������Ϣ
 * @return      [in/out]  else       ��Ϣ����������Ϣ
 * @note        ���û�����
 *******************************************************************************
 */
uint8_t Fw_Msg_Recv(uint8_t id)
{
    uint8_t isr  = 0;
    uint8_t mem  = FwMsgMgr[id].Head;
    uint8_t head = FwMsgMgr[id].Head;
    uint8_t msg;

    if(id >= FRAMEWORK_TASK_MAX || head == FW_MSG_ADDR_NULL || mem >= FRAMEWORK_MSG_QUEUE_MAX)
    {
#ifdef FW_PUT_MSG_FREE_INFO
		Fw_Core_Error("Read Task��%d Message Error", (uint8_t)id);
#endif
        return 0;
    }

    Fw_Atom_Begin(isr);

#ifdef FW_PUT_MSG_CMPT_INFO
	Fw_Core_Log("\r\n********************Free Message Memory Begin********************");
	Fw_Put_Msg_List();
#endif

	//! ��ȡ��Ϣ
	msg = FwMsgMemory[mem].Msg;

#ifdef FW_PUT_MESSAGE_CHECK_INFO
	if (msg == 0)
	{
		Fw_Core_Error("Task %d Recv Message Error", (uint8_t)id);
	}
#endif

    //! ���þ��״̬
    FwMsgMgr[id].Head = FwMsgMemory[head].Next;

    if (FwMsgMgr[id].Head == FW_MSG_ADDR_NULL)
    {
        FwMsgMgr[id].Tail = FW_MSG_ADDR_NULL;
        
        Fw_Evt_Clr(FwTask[id].Event, FRAMEWORK_MSG_EVENT);
    }

    FwMsgMemory[mem].Next = FW_MSG_ADDR_NULL;

    Fw_Msg_Free(mem);

#ifdef FW_PUT_MSG_CMPT_INFO
	Fw_Core_Log("\r\nFree Message Memory End, Src Task Id:%d, Message:%d", (uint8_t)id, (uint8_t)msg);
	Fw_Put_Msg_List();
	printf("********************************************************************************\r\n\r\n\r\n");
#endif

    Fw_Atom_End(isr);
    
    return msg;
}

/**
 *******************************************************************************
 * @brief       �����ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���ں˵���
 *******************************************************************************
 */
void Fw_Task_Init(void)
{
    for (FwCore.CurTask=0; FwCore.CurTask<FRAMEWORK_TASK_MAX; FwCore.CurTask++)
    {
        FwTaskInitList[FwCore.CurTask](FwCore.CurTask);

#ifdef FRAMEWORK_NANO_EXPAND
        FwTask[FwCore.CurTask].State = FW_TASK_READY;
#endif
    }
    
    FwCore.CurTask = 0;
}

#ifdef FRAMEWORK_NANO_EXPAND
/**
 *******************************************************************************
 * @brief       �������
 * @param       [in/out]  id     ����ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
__INLINE
void Fw_Task_Ready(uint8_t id)
{
    FwTask[id].State = FW_TASK_READY;
}

/**
 *******************************************************************************
 * @brief       ��������
 * @param       [in/out]  id     ����ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
__INLINE
void Fw_Task_Sleep(uint8_t id)
{
    FwTask[id].State = FW_TASK_SLEEP;
}

/**
 *******************************************************************************
 * @brief       ��������
 * @param       [in/out]  id     ����ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
__INLINE
void Fw_Task_Yeild(uint8_t id)
{
    FwTask[id].State = FW_TASK_YEILD;
}

/**
 *******************************************************************************
 * @brief       ����״̬
 * @param       [in/out]  id       ����ID
 * @return      [in/out]  state    ����״̬
 * @note        None
 *******************************************************************************
 */
__INLINE
uint8_t Fw_Task_State(uint8_t id)
{
    return  FwTask[id].State;
}
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
void Fw_Timer_Init(uint8_t id, void *callback, void *param)
{
    FwTimer_t *timer = &FwTask[id].Timer;
    
    timer->Tick = 0;

#ifdef FRAMEWORK_NANO_EXPAND
    timer->Reload = 0;

    timer->Handle.Ptr = callback;
    timer->param      = param;
#else
    _unused(callback);
    _unused(param);
#endif
}

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
void Fw_Timer_Start(uint8_t id, FwTick_t tick, uint16_t flag)
{
    FwTask[id].Timer.Tick = tick;

#ifdef FRAMEWORK_NANO_EXPAND
    if (flag)
    {
        FwTask[id].Timer.Reload = tick;
    }
    else
    {
        FwTask[id].Timer.Reload = 0;
    }
#else
    _unused(flag);
#endif
}

/**
 *******************************************************************************
 * @brief       ֹͣ��ʱ��
 * @param       [in/out]  id       ��ʱ��ID
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void Fw_Timer_Stop(uint8_t id)
{
    FwTask[id].Timer.Tick = 0;

#ifdef FRAMEWORK_NANO_EXPAND
    FwTask[id].Timer.Reload = 0;
#endif
}

/**
 *******************************************************************************
 * @brief       ����Task����
 * @param       [in/out]  void
 * @return      [in/out]  num    Task����
 * @note        None
 *******************************************************************************
 */
uint16_t Fw_Task_Num(void)
{
	return FRAMEWORK_TASK_MAX;
}

/**
 *******************************************************************************
 * @brief       ����Timer����
 * @param       [in/out]  void
 * @return      [in/out]  num    Timer����
 * @note        None
 *******************************************************************************
 */
uint16_t Fw_Timer_Num(void)
{
	return FRAMEWORK_TIMER_MAX;
}

/**
 *******************************************************************************
 * @brief       ����Tickʱ��
 * @param       [in/out]  uint16_t    ��һ��TICK
 * @return      [in/out]  uint16_t    PAST TICK
 * @note        None
 *******************************************************************************
 */
FwTick_t Fw_Tick_PastWithUpdate(FwTick_t *last)
{
    uint8_t isr = 0;
    FwTick_t past;
    
    Fw_Atom_Begin(isr);
    
    past = Fw_Tick_Past(*last);

    *last = FwCore.Tick;
    
    Fw_Atom_End(isr);
    
    return past;
}

/**
 *******************************************************************************
 * @brief       �ں�����������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Tick_Handle(void)
{
	static FwTick_t tick = 0;
	FwTick_t past = Fw_Tick_Past(tick);
    uint8_t i;
    FwTimer_t *timer;
    
    for (i=0; i<FRAMEWORK_TASK_MAX; i++)
    {
        timer = &FwTask[i].Timer;
        
        if (timer->Tick)
        {
            if (past >= timer->Tick)
            {
#ifdef FRAMEWORK_NANO_EXPAND
                if (!IS_PTR_NULL(timer->Handle.Callback))
                {
                    timer->Handle.Callback(timer->param);
                }
                else
                {
                    Fw_Task_Evt_Set(i, FRAMEWORK_TICK_EVENT);
                }
                
                if (timer->Reload)
                {
                    timer->Tick = timer->Reload;
                }
#else
                Fw_Task_Evt_Set(i, FRAMEWORK_TICK_EVENT);
                
                timer->Tick = 0;
#endif
                
#ifdef FW_PUT_TICK_HANDLE_INFO
				Fw_Core_Log("Task��%d Timer Timeout \r\n", i);
#endif
            }
            else
            {
                timer->Tick -= past;
            }
        }
    }
    
    tick = Fw_Tick_Get();
}

/**
 *******************************************************************************
 * @brief       ����ں˳�ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Core_Init(void)
{
    uint8_t isr = 0;
    
    //! ��ʼ��ϵͳ���ѱ���
    memset((void *)&FwCore,    0, sizeof(FwCore));
    memset((void *)&FwTask[0], 0, sizeof(FwTask));
    
    Fw_Atom_Begin(isr);

    //! ��Ϣ���г�ʼ��
    Fw_Msg_Init();

    //! Ӳ�������ʼ��
    //! ���û�ʵ��
    Fw_Hardware_Init();
    
    //! ���������ʼ��
    Fw_Task_Init();
    
    Fw_Atom_End(isr);
    
    //! ����ȫ���ж�
    Fw_IRQ_Enable();
}

/**
 *******************************************************************************
 * @brief       ϵͳ�ں���������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Core_Startup(void)
{
    uint8_t isr   = 0;
    uint16_t task = 0;
    
    while (1)
    {
        Fw_Atom_Begin(isr);
        
        //! ʱ��������
#ifndef ENABLE_FRAMEWORK_TIMER_REAL
        if (Fw_Evt_Get(FwCore.Event, FRAMEWORK_TICK_EVENT))
        {
            Fw_Tick_Handle();
            Fw_Evt_Clr(FwCore.Event, FRAMEWORK_TICK_EVENT);
        }
#endif

        //! �������
        FwCore.CurTask = 0;
        
        do
        {
#ifdef FRAMEWORK_NANO_EXPAND
            if (FwTask[FwCore.CurTask].Event && FwTask[FwCore.CurTask].State == FW_TASK_READY)
#else
            if (FwTask[FwCore.CurTask].Event)
#endif
            {
                break;
            }
        }while(++FwCore.CurTask < FRAMEWORK_TASK_MAX);
        
        task = FwCore.CurTask;
        
        Fw_Atom_End(isr);
        
        if (task < FRAMEWORK_TASK_MAX)
        {
//            Fw_Atom_Begin(isr);
            FwTaskList[task](task, FwTask[task].Event);
//            Fw_Atom_End(isr);
        }
        else
        {
            Fw_Sleep_Handle();
        }
        
        Fw_Hardware_Poll();
    }
}

/** @}*/     /** framework core interface */

/**********************************END OF FILE*********************************/
