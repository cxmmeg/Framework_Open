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
 * @file       fw_timer.c                                                      *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2016-07-09                                                      *
 * @brief      timer component source files                                    *
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
* @defgroup framework timer component
* @{
*/

/* Includes ------------------------------------------------------------------*/
#include "fw_core.h"
#include <string.h>

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief        ���嶨ʱ�����
 *******************************************************************************
 */
FwTimerHandle_t FwTimer;

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       ��ʱ�������ʼ������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ��Fw_Core_Init�������ã������û�����
 *******************************************************************************
 */
int FwTimer_Component_Init(void)
{ 
    memset((void *)&FwTimer, 0, sizeof(FwTimer));
    
    FwListInit((FwList_t *)&FwTimer.List);

    return 0;
}
INIT_COMPONENT_EXPORT(FwTimer_Component_Init);

/**
 *******************************************************************************
 * @brief       ��ȡ��ʱ���б�ͷ
 * @param       [in/out]  void
 * @return      [in/out]  FwList_t*    ��ʱ���б�ͷ
 * @note        None
 *******************************************************************************
 */
FwList_t *FwTimer_List_Root(void)
{
	return FwTimer.List.Next;
}

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
__INLINE
void Fw_Timer_Init(FwTimer_t *timer, char *name, FwTimerCallback callback, void *param, uint32_t tick, uint8_t flag)
{
    FwListInit(&timer->List);

    timer->Callback = callback;
    timer->Param    = param;
	timer->Name     = name;

    if (Fw_Flag_Get(flag, FW_TIM_EN_PERIOD))
    {
        Fw_Flag_Set(timer->Flag, FW_TIM_EN_PERIOD);
    }
    else
    {
        Fw_Flag_Clr(timer->Flag, FW_TIM_EN_PERIOD);
    }

    timer->Timeout = tick;
    timer->State   = FW_TIM_SLEEP;
}

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
__INLINE
void Fw_Timer_Set_Callback(FwTimer_t *timer, FwTimerCallback callback, void *param)
{
    timer->Callback = callback;
    timer->Param    = param;
}

/**
 *******************************************************************************
 * @brief       ��ʱ��ע������
 * @param       [in/out]  *timer      ��ʱ�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Timer_Fini(FwTimer_t *timer)
{
    memset(timer, 0, sizeof(FwTimer_t));
}

/**
 *******************************************************************************
 * @brief       ��ʱ����������
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Timer_Start(FwTimer_t *timer)
{
    FwList_t *p;
    FwTimer_t *t;
    
    Fw_Enter_Critical();

#ifdef FW_PUT_TIMER_START_INFO
	Fw_Log("[Begin Process]\r\n");
	Fw_Put_Timer_List();
#endif

    //! ɾ���б�
    FwListRemove(&timer->List);

    //! ����ʱ��
    timer->Tick = Fw_Tick_Get() + timer->Timeout;

    //! �������
    for (p = FwTimer.List.Next; p != &FwTimer.List; p = p->Next)
    {
        t = FwListEntry(p, FwTimer_t, List);
        
        if (t->Tick >= timer->Tick)
        {
            break;
        }
    }

    FwListInsertBefore(p, &timer->List);

    timer->State  = FW_TIM_DOING;

#ifdef FW_PUT_TIMER_START_INFO
	Fw_Log("[End Process]\r\n");
	Fw_Put_Timer_List();
#endif

    Fw_Exit_Critical();
}

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
void Fw_Timer_Restart(FwTimer_t *timer, uint32_t tick, uint8_t flag)
{
    Fw_Enter_Critical();
    
    if (Fw_Flag_Get(flag, FW_TIM_EN_PERIOD))
    {
        Fw_Flag_Set(timer->Flag, FW_TIM_EN_PERIOD);
    }
    else
    {
        Fw_Flag_Clr(timer->Flag, FW_TIM_EN_PERIOD);
    }

    timer->Timeout = tick;
    
    Fw_Timer_Start(timer);
    
    Fw_Exit_Critical();
}

/**
 *******************************************************************************
 * @brief       ��ȡ��ʱ��״̬
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
uint16_t Fw_Timer_State(FwTimer_t *timer)
{
    if (timer->State == FW_TIM_COMPLETE)
    {
        timer->State = FW_TIM_SLEEP;
        
        return FW_TIM_COMPLETE;
    }
    
    return timer->State;
}

/**
 *******************************************************************************
 * @brief       ��ȡ��ʱ�����״̬
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
uint8_t Fw_Timer_Done(FwTimer_t *timer)
{
    if (timer->State == FW_TIM_COMPLETE)
    {
        timer->State = FW_TIM_SLEEP;
        
        return 1;
    }
    
    return 0;
}

/**
 *******************************************************************************
 * @brief       ��ʱ��ֹͣ����
 * @param       [in/out]  timer        ��ʱ�����
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
__INLINE
void Fw_Timer_Stop(FwTimer_t *timer)
{
    Fw_Enter_Critical();
    
    timer->Tick    = 0;
    timer->Timeout = 0;
    timer->State = FW_TIM_SLEEP;

    FwListRemove(&timer->List);
    
    Fw_Exit_Critical();
}

/**
 *******************************************************************************
 * @brief       ��ʱ��������������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ���û�����
 *******************************************************************************
 */
void Fw_Tick_Inc(void)
{
    Fw_Enter_Critical();

    FwTimer.Tick ++;
    
#ifdef FW_PUT_TICK_INFO
	Fw_Core_Log("%s", Fw_Time_To_Str(FW_TIME_2_STR_TIME));
#endif

#ifdef ENABLE_FRAMEWORK_TIMER_REAL
    FwTimer_Component_Handle();
#else
    Fw_Evt_Set(FwCoreEvent, FRAMEWORK_TICK_EVENT);
#endif
    
    Fw_Exit_Critical();
}

/**
 *******************************************************************************
 * @brief       ��ȡ��ǰ����ֵ
 * @param       [in/out]  void
 * @return      [in/out]  uint32_t    ��ǰ����ֵ
 * @note        ���û�����
 *******************************************************************************
 */
uint32_t Fw_Tick_Get(void)
{
    return FwTimer.Tick;
}

/**
 *******************************************************************************
 * @brief       ��������ϸ�ʱ��������ֵ
 * @param       [in/out]  lastTick    �ϸ�ʱ���
 * @return      [in/out]  uint32_t    ���ŵ�ʱ��
 * @note        None
 *******************************************************************************
 */
__INLINE
uint32_t Fw_Tick_Past(uint32_t lastTick)
{
    return (FwTimer.Tick < lastTick) ? (UINT32_MAX - lastTick + FwTimer.Tick) : (FwTimer.Tick - lastTick);
}

/**
 *******************************************************************************
 * @brief       ��ʱ��������
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ��Fw_Core_Start����
 *******************************************************************************
 */
void FwTimer_Component_Handle(void)
{
    uint32_t tick;
    
    FwList_t *p;
    
    FwTimer_t *t;
    
    Fw_Enter_Critical();
    
#ifdef FW_PUT_TIMER_PROCESS_INFO
	Fw_Log("[Begin Timer Process]\r\n");
	Fw_Put_Timer_List();
#endif

    for (p = FwTimer.List.Next; p != &FwTimer.List;)
    {
        t = FwListEntry(p, FwTimer_t, List);
        
        //! ���µ�ǰʱ��
        tick = Fw_Tick_Get();
        
        if ((tick - t->Tick) < FW_TICK_MAX / 2)
        {
            //! �л�����һ���ڵ�
            p = p->Next;
            
            //! �Ƴ���ǰ�ڵ�
            FwListRemove(&t->List);

            Fw_Exit_Critical();
            
            //! ��ʱ����
            if (!IS_PTR_NULL(t->Callback))
            {
                t->Callback(t->Param);
            }
            
            Fw_Enter_Critical();
            
            //! ���±�־λ
            if (Fw_Flag_Get(t->Flag, FW_TIM_EN_PERIOD))
            {
                t->Tick = Fw_Tick_Get() + t->Timeout;
                
                Fw_Timer_Start(t);
            }
            else
            {
                t->State = FW_TIM_SLEEP;
            }
        }
        else
        {
            break;
        }
    }
    
#ifdef FW_PUT_TIMER_PROCESS_INFO
	Fw_Log("[End Timer Process]\r\n");
	Fw_Put_Timer_List();
#endif

    Fw_Exit_Critical();
}

/** @}*/     /** framework timer component */

/**********************************END OF FILE*********************************/
