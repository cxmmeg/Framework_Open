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
 * @file       hal_veeprom.c                                                   *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-10-27                                                      *
 * @brief      mcu virtual eeprom component server source file                 *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1.Ŀǰ֧��д���������ȡ������                                              *
 * 2. Demoƽ̨ M0516LDN                                                        *
 *******************************************************************************
 * @par update                                                                 *
 * 1. 20171027    �����ļ�"veeprom.c"                                          *
 * 2. 20180829    ��veeprom���Ǩ�Ƶ�hal���У�����Ϊ"hal_veeprom.c"            *
 *******************************************************************************
 */
 
/**
* @defgroup hal virtual eeprom driver component
* @{
*/

/* Includes ------------------------------------------------------------------*/
#include "hal_device.h"

/* Private define ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       ��ز�����
 *******************************************************************************
 */
#define HAL_VEEPROM_PAGE_INFO_SIZE              (sizeof(size_t)/sizeof(uint8_t))
#define HAL_VEEPROM_ALIGNMENT                   (sizeof(size_t)/sizeof(uint8_t))

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef ENABLE_HAL_VIRTUAL_EEPROM_DRIVER
/**
 *******************************************************************************
 * @brief       virtual eeprom device driver handle
 *******************************************************************************
 */
struct
{
    HAL_Device_t            Parent;
    
    HAL_Veeprom_Interface_t Ops;
}DeviceVEEPROM;
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifdef ENABLE_HAL_VIRTUAL_EEPROM_DRIVER
/**
 *******************************************************************************
 * @brief       ��ȡveeprom��־λ
 * @param       [in/out]  flag          ��־λ
 * @param       [in/out]  addr          У���ַ
 * @param       [in/out]  vData         У������
 * @return      [in/out]  0             normal
 * @return      [in/out]  1             busy
 * @return      [in/out]  2             error
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
uint32_t veeprom_get_flag(uint32_t flag, uint32_t addr, uint32_t vData)
{
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.GetFlag))
    {
        return DeviceVEEPROM.Ops.GetFlag(flag, addr, vData);
    }
    
    return 0;
}

/**
 *******************************************************************************
 * @brief       ����Ӳ��
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_lock(void)
{
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.Lock))
    {
        DeviceVEEPROM.Ops.Lock();
    }
}

/**
 *******************************************************************************
 * @brief       ����Ӳ��
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_unlock(void)
{
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.Unlock))
    {
        DeviceVEEPROM.Ops.Unlock();
    }
}

/**
 *******************************************************************************
 * @brief       ����veeprom��д����
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_rw_unlock(HAL_Veeprom_t *eeprom)
{
    Fw_Flag_Set(eeprom->Atrribute.Flag, HAL_VEEPROM_LOCK_FLAG);
}

/**
 *******************************************************************************
 * @brief       ����veeprom��д����
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_rw_lock(HAL_Veeprom_t *eeprom)
{
    Fw_Flag_Clr(eeprom->Atrribute.Flag, HAL_VEEPROM_LOCK_FLAG);
}

/**
 *******************************************************************************
 * @brief       ��ȡveeprom��д��������״̬
 * @param       [in/out]  *eeprom    �������
 * @return      [in/out]  0          δ����
 * @return      [in/out]  1          ������
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
uint8_t veeprom_is_rw_lock(HAL_Veeprom_t *eeprom)
{
    return Fw_Flag_Get(eeprom->Atrribute.Flag, HAL_VEEPROM_LOCK_FLAG);
}

/**
 *******************************************************************************
 * @brief       ��������ҳ
 * @param       [in/out]  addr    �����ַ
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_erase_page(uint32_t addr)
{
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.Erase))
    {
        DeviceVEEPROM.Ops.Erase(addr);
    }
}

/**
 *******************************************************************************
 * @brief       ����veeprom���ݿ�
 * @param       [in/out]  addr        ������ʼ��ַ
 * @param       [in/out]  pageSize    ҳ��С
 * @param       [in/out]  num         ҳ����
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_erase_block(uint32_t addr, uint16_t pageSize, uint16_t num)
{
    uint16_t i;
    
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.Erase))
    {
        for (i=0; i<num; i++, addr+=pageSize)
        {
            DeviceVEEPROM.Ops.Erase(addr);
            while (veeprom_get_flag(HAL_VEEPROM_ERASE_FLAG, 0, 0) == 1);
        }
    } 
}

/**
 *******************************************************************************
 * @brief       ��ȡӲ������--1��
 * @param       [in/out]  addr        ������ʼ��ַ
 * @return      [in/out]  data        ��ȡ��������
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
uint32_t veeprom_read_word(uint32_t addr)
{
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.ReadWord))
    {
        return DeviceVEEPROM.Ops.ReadWord(addr);
    }
    
    return 0xFFFFFFFF;
}

/**
 *******************************************************************************
 * @brief       д��Ӳ������--1��
 * @param       [in/out]  addr        ����д���ַ
 * @param       [in/out]  addr        д�������
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_write_word(uint32_t addr, uint32_t wr)
{
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.WriteWord))
    {
        DeviceVEEPROM.Ops.WriteWord(addr, wr);
    }
}

/**
 *******************************************************************************
 * @brief       д��Ӳ������--1��(������ʽ)
 * @param       [in/out]  addr        ����д���ַ
 * @param       [in/out]  addr        д�������
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_write_word_at_once(uint32_t addr, uint32_t wr)
{
    if (!IS_PTR_NULL(DeviceVEEPROM.Ops.WriteWord))
    {
        DeviceVEEPROM.Ops.WriteWord(addr, wr);
    }
    
    while (veeprom_get_flag(HAL_VEEPROM_WRITE_FLAG, 0, 0) == 1);
}

/**
 *******************************************************************************
 * @brief       ����veeprom״̬
 * @param       [in/out]  *eeprom         veeprom���
 * @return      [in/out]  VeepromState    veeprom״̬
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE 
VeepromState veeprom_load_state(HAL_Veeprom_t *eeprom)
{
    uint32_t pageState, page1State;

    //! ��ȡҳ��Ϣ
    pageState  = veeprom_read_word(eeprom->PhyInfo.Page0Base);
    page1State = veeprom_read_word(eeprom->PhyInfo.Page1Base);
    
    //! page0 ��ʹ��
    if(pageState == HAL_VEEPROM_PAGE_USE)
    {
        eeprom->Addr      = eeprom->PhyInfo.Page0Base + HAL_VEEPROM_ALIGNMENT;
        eeprom->PageState = HAL_VEEPROM_USE_PAGE0;
        
        return HAL_VEEPROM_RET_LOAD;
    }
    //! page0 δʹ��
    else if(pageState == HAL_VEEPROM_PAGE_FREE)
    {
        eeprom->Addr      = eeprom->PhyInfo.Page0Base + HAL_VEEPROM_ALIGNMENT;
        eeprom->PageState = HAL_VEEPROM_SET_PAGE0;
        
        return HAL_VEEPROM_RET_NORMAL;
    }
    //! page1 ��ʹ��
    else if(page1State == HAL_VEEPROM_PAGE_USE)
    {
        eeprom->Addr      = eeprom->PhyInfo.Page1Base + HAL_VEEPROM_ALIGNMENT;
        eeprom->PageState = HAL_VEEPROM_USE_PAGE1;
        
        return HAL_VEEPROM_RET_LOAD;
    }
    //! page1 δʹ��
    else if(page1State == HAL_VEEPROM_PAGE_FREE)
    {
        eeprom->Addr      = eeprom->PhyInfo.Page1Base + HAL_VEEPROM_ALIGNMENT;
        eeprom->PageState = HAL_VEEPROM_SET_PAGE1;
        
        return HAL_VEEPROM_RET_NORMAL;
    }
    
    //! ��Ч״̬
    return HAL_VEEPROM_RET_RESET;
}

/**
 *******************************************************************************
 * @brief       ��λveeprom���
 * @param       [in/out]  *eeprom         veeprom���
 * @return      [in/out]  VeepromState    veeprom״̬
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE 
VeepromState veeprom_reset(HAL_Veeprom_t *eeprom)
{
#ifdef ENABLE_VEEPROM_CHECK
    uint32_t load;
#endif
    
    //! ��������
    veeprom_lock();
    veeprom_erase_block(eeprom->PhyInfo.Page0Base, eeprom->PhyInfo.PageSize, eeprom->BlockNum);
    veeprom_erase_block(eeprom->PhyInfo.Page1Base, eeprom->PhyInfo.PageSize, eeprom->BlockNum);

    //! д�뵱ǰҳ״̬
    veeprom_write_word_at_once(eeprom->PhyInfo.Page0Base, HAL_VEEPROM_PAGE_FREE);
    
#ifdef ENABLE_VEEPROM_CHECK
    load = veeprom_read_word(eeprom->PhyInfo.Page0Base);
    
    if(load != HAL_VEEPROM_PAGE_FREE)
    {
        return HAL_VEEPROM_RET_HW_ERROR;
    }
#endif
    
    eeprom->Addr = eeprom->PhyInfo.Page0Base + HAL_VEEPROM_ALIGNMENT;
    
    eeprom->Atrribute.State = 1;
    eeprom->UpdateState = HAL_VEEPROM_IDLE;
    
    veeprom_unlock();
    
    return HAL_VEEPROM_RET_SUCCESS;
}

/**
 *******************************************************************************
 * @brief       ����veeprom����
 * @param       [in/out]  *eeprom         veeprom���
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_load_data(HAL_Veeprom_t *eeprom)
{
    uint32_t rdata;
    uint16_t addr, tmp;
    
    //! ��������
    while(1)
    {
        rdata = veeprom_read_word(eeprom->Addr);
        
        if(rdata != 0xFFFFFFFF)
        {
            tmp   = rdata & 0x0000FFFF;
            addr  = rdata >> 16;
            
            //! ��⵽��Ч���ݵ�ַ
            //! ���������ݵĶ�ȡ��ʹ��ΪĬ����ֵ
            if(addr < eeprom->BufferLen)
            {
                *((uint16_t *)&eeprom->Buffer[addr]) = tmp;
            }
        }
        else
        {
            break;
        }
		
		//! �����ַ�߽籣��
        eeprom->Addr += HAL_VEEPROM_ALIGNMENT;

		if(eeprom->PageState == HAL_VEEPROM_USE_PAGE0 && eeprom->Addr > eeprom->PhyInfo.Page0End)
		{
            break;
		}
        else if(eeprom->PageState == HAL_VEEPROM_USE_PAGE1 && eeprom->Addr > eeprom->PhyInfo.Page1End)
        {
            break;
		}
    }
}

/**
 *******************************************************************************
 * @brief       veeprom���ݳ�ʼ��
 * @param       [in/out]  *eeprom         veeprom���
 * @return      [in/out]  0               �洢����ʹ����������װ��
 * @return      [in/out]  1               �洢��δʹ��
 * @return      [in/out]  2               ����
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
uint8_t veeprom_init_data(HAL_Veeprom_t *eeprom)
{
    uint8_t state;
    uint8_t ret;

    state = veeprom_load_state(eeprom);

    switch(state)
    {
        case HAL_VEEPROM_RET_RESET:
            state = veeprom_reset(eeprom);
        
            if(state == HAL_VEEPROM_RET_HW_ERROR)
            {
                break;
            }
        case HAL_VEEPROM_RET_NORMAL:
            ret = 1;
            break;
        case HAL_VEEPROM_RET_LOAD:
            veeprom_load_data(eeprom);
            ret = 0;
            break;
        default:
            ret = 2;
            break;
    }

    return ret;
}

/**
 *******************************************************************************
 * @brief       ����Ƿ���Ҫ�л��洢��
 * @param       [in/out]  *eeprom         veeprom���
 * @return      [in/out]  0               ����Ҫ�л��ҿ��Լ���ʹ��
 * @return      [in/out]  1               ��Ҫ�л�
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
uint8_t veeprom_block_check(HAL_Veeprom_t *eeprom)
{
    uint8_t ret = 1;
    
    switch (eeprom->PageState)
    {
        case HAL_VEEPROM_SET_PAGE0:
            veeprom_write_word_at_once(eeprom->PhyInfo.Page0Base, HAL_VEEPROM_PAGE_USE);
            eeprom->Addr = eeprom->PhyInfo.Page0Base + HAL_VEEPROM_ALIGNMENT;
        
            eeprom->PageState = HAL_VEEPROM_USE_PAGE0;
        
            ret = 0;
            break;
        case HAL_VEEPROM_USE_PAGE0:
            if (eeprom->Addr <= eeprom->PhyInfo.Page0End - 4)
            {
                ret = 0;
            }
            else
            {
                HAL_Evt_Set(eeprom->Atrribute.Event, HAL_VEEPROM_SWITCH_PAGE1_EVENT);
            }
            break;
        case HAL_VEEPROM_SET_PAGE1:
            veeprom_write_word_at_once(eeprom->PhyInfo.Page1Base, HAL_VEEPROM_PAGE_USE);
            eeprom->Addr = eeprom->PhyInfo.Page1Base + HAL_VEEPROM_ALIGNMENT;
        
            eeprom->PageState = HAL_VEEPROM_USE_PAGE1;
        
            ret = 0;
            break;
        case HAL_VEEPROM_USE_PAGE1:
            if (eeprom->Addr <= eeprom->PhyInfo.Page1End - 4)
            {
                ret = 0;
            }
            else
            {
                HAL_Evt_Set(eeprom->Atrribute.Event, HAL_VEEPROM_SWITCH_PAGE0_EVENT);
            }
            break;
        default:
            break;
    }

    return ret;
}

/**
 *******************************************************************************
 * @brief       д�����ݵ������ַ
 * @param       [in/out]  *eeprom         veeprom���
 * @param       [in/out]  addr            veeprom�����ַ
 * @param       [in/out]  wr              д�������
 * @return      [in/out]  void
 * @note        ˽�У��û�����ֱ�ӷ���
 *******************************************************************************
 */
__STATIC_INLINE
void veeprom_to_phy(HAL_Veeprom_t *eeprom, uint16_t addr, uint16_t wr)
{
    if (addr % 2)
    {
        addr --;
    }
    
    if (veeprom_block_check(eeprom) == 0)
    {
        uint32_t wrData = addr << 16 | wr;
        
        veeprom_write_word_at_once(eeprom->Addr, wrData);
        
        eeprom->Addr += HAL_VEEPROM_ALIGNMENT;
    }
}
#endif

/* Exported functions --------------------------------------------------------*/
#ifdef ENABLE_HAL_VIRTUAL_EEPROM_DRIVER
/**
 *******************************************************************************
 * @brief       veeprom�豸ע��
 * @param       [in/out]  *ops            veeprom��������
 * @param       [in/out]  *userData       �û�����
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void HAL_Veeprom_Register(HAL_Veeprom_Interface_t *ops, void *userData)
{
    HAL_Interface_t halDevOps;
    
    memset(&DeviceVEEPROM, 0, sizeof(DeviceVEEPROM));
    
    DeviceVEEPROM.Parent.Type     = HAL_DEVICE_VEEPROM;
    DeviceVEEPROM.Parent.UserData = userData;
    
    halDevOps.Init                = NULL;
    halDevOps.Fini                = NULL;
    halDevOps.Open                = NULL;
    halDevOps.Close               = NULL;
    halDevOps.Write               = NULL;
    halDevOps.Read                = NULL;
    halDevOps.Control             = NULL;
    
    DeviceVEEPROM.Ops.Erase       = ops->Erase;
    DeviceVEEPROM.Ops.Lock        = ops->Lock;
    DeviceVEEPROM.Ops.Unlock      = ops->Unlock;
    DeviceVEEPROM.Ops.ReadWord    = ops->ReadWord;
    DeviceVEEPROM.Ops.WriteWord   = ops->WriteWord;
    
    HAL_Device_Register(&DeviceVEEPROM.Parent, HAL_VEEPROM_STR, &halDevOps, HAL_Flag_Encode(HAL_DEVICE_VEEPROM, HAL_READ_FLAG | HAL_WRITE_FLAG));
}

/**
 *******************************************************************************
 * @brief       veeprom��ʼ��
 * @param       [in/out]  *eeprom         veeprom���
 * @param       [in/out]  *config         veeprom����
 * @return      [in/out]  0               �洢����ʹ����������װ��
 * @return      [in/out]  1               �洢��δʹ��
 * @return      [in/out]  2               ����
 * @note        None
 *******************************************************************************
 */
uint8_t HAL_Veeprom_Init(HAL_Veeprom_t *eeprom, HAL_Veeprom_Config_t *config)
{
    size_t block;

    memset(eeprom, 0, sizeof(HAL_Veeprom_t));

    //! ���ò���
    eeprom->PhyInfo.Page0Base = config->Atrribute.Page0Base;
    eeprom->PhyInfo.Page0End  = config->Atrribute.Page0End;
    eeprom->PhyInfo.Page1Base = config->Atrribute.Page1Base;
    eeprom->PhyInfo.Page1End  = config->Atrribute.Page1End;
    eeprom->PhyInfo.PageSize  = config->Atrribute.PageSize;
    
    eeprom->Buffer     = config->Buffer;
    eeprom->BufferLen  = config->BufferLen;
    
    eeprom->UpdateState = HAL_VEEPROM_INIT;
    
    if (IS_PTR_NULL(eeprom->Buffer))
    {
        return HAL_VEEPROM_RET_ERROR;
    }
    
    memset((void *)eeprom->Buffer, 0, eeprom->BufferLen);
    
    //! �������ݿ���ռ�õĿռ�(PAGE1 + PAGE2)
    block = eeprom->PhyInfo.Page0End - eeprom->PhyInfo.Page0Base + 1;
    //! ����EEPROM���ݿ�ռ������ҳ������
    eeprom->BlockNum = block / eeprom->PhyInfo.PageSize;
    //! �Ƴ�״ָ̬ʾ��
    block = block - HAL_VEEPROM_ALIGNMENT;
    
    //! ���EEPROM��ַ�����Ƿ����
    if (block < eeprom->BufferLen || block % eeprom->PhyInfo.PageSize)
    {
        return HAL_VEEPROM_RET_HW_ERROR;
    }

    eeprom->Atrribute.State = veeprom_init_data(eeprom);

    return eeprom->Atrribute.State;
}

/**
 *******************************************************************************
 * @brief       veeprom д���ֽ�����
 * @param       [in/out]  *eeprom         veeprom���
 * @param       [in/out]  addr            д��veeprom�����ַ
 * @param       [in/out]  ch              д������
 * @return      [in/out]  0               д��ʧ��
 * @return      [in/out]  1               д��ɹ�
 * @note        None
 *******************************************************************************
 */
uint8_t HAL_Veeprom_WriteByte(HAL_Veeprom_t *eeprom, uint16_t addr, uint8_t ch)
{
    uint8_t ret;
    
    if (addr > eeprom->BufferLen)
    {
        return 0;
    }
    
    if (veeprom_is_rw_lock(eeprom))
    {
        return 0;
    }
    else
    {
        veeprom_rw_lock(eeprom);
    }
    
    if (eeprom->Atrribute.State)
    {
        if (eeprom->Buffer[addr] != ch)
        {
            eeprom->Buffer[addr] = ch;
            veeprom_to_phy(eeprom, addr, ch);
        }
        
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    
    veeprom_rw_unlock(eeprom);
    
    return ret;
}

/**
 *******************************************************************************
 * @brief       veeprom д������
 * @param       [in/out]  *eeprom         veeprom���
 * @param       [in/out]  *buf            д�뻺����
 * @param       [in/out]  addr            д�����ַ
 * @param       [in/out]  len             д�����ݳ���
 * @return      [in/out]  0               д��ʧ��
 * @return      [in/out]  len             д��ɹ�
 * @note        None
 *******************************************************************************
 */
uint16_t HAL_Veeprom_Write(HAL_Veeprom_t *eeprom, uint8_t *buf, uint16_t addr, uint16_t len)
{
    if (addr > eeprom->BufferLen)
    {
        return 0;
    }
    
    if (veeprom_is_rw_lock(eeprom))
    {
        return 0;
    }
    else
    {
        veeprom_rw_lock(eeprom);
    }
    
    if (eeprom->Atrribute.State)
    {
        uint16_t endAddr = addr + len;
        uint16_t *pEEPROM = (uint16_t *)&eeprom->Buffer[addr];
        uint16_t *pWrite  = (uint16_t *)&buf[0];
        
        //! ��ַд�����
        if (addr % 2)
        {
            if(eeprom->Buffer[addr] != buf[0])
            {
                eeprom->Buffer[addr] = buf[0];
                veeprom_to_phy(eeprom, addr, buf[0]);
            }
            
            pEEPROM = (uint16_t *)&eeprom->Buffer[addr+1];
            pWrite  = (uint16_t *)&buf[1];
            
            addr++;
        }

        for (; addr < endAddr; addr+=2)
        {
            if (*pEEPROM != *pWrite)
            {
                *pEEPROM = *pWrite;
                veeprom_to_phy(eeprom, addr, *pWrite);
            }
            
            pEEPROM ++;
            pWrite  ++;
        }
        
        //! ����д�����
        if (addr > endAddr)
        {
            if(eeprom->Buffer[addr-1] != buf[addr-1])
            {
                eeprom->Buffer[addr-1] = buf[addr-1];
                veeprom_to_phy(eeprom, addr, buf[len-1]);
            }
        }
    }
    else
    {
        len = 0;
    }
    
    veeprom_rw_unlock(eeprom);
    
    return len;
}

/**
 *******************************************************************************
 * @brief       veeprom ��ȡ�ֽ�
 * @param       [in/out]  *eeprom         veeprom���
 * @param       [in/out]  addr            ��ȡ��ַ
 * @return      [in/out]  0xFF            ��ȡʧ��
 * @return      [in/out]  data            ��ȡ�ɹ�
 * @note        None
 *******************************************************************************
 */
uint8_t HAL_Veeprom_ReadByte(HAL_Veeprom_t *eeprom, uint32_t addr)
{
    uint8_t read;
    
    if (addr > eeprom->BufferLen)
    {
        return 0xFF;
    }
    
    if (veeprom_is_rw_lock(eeprom))
    {
        return 0;
    }
    else
    {
        veeprom_rw_lock(eeprom);
    }
    
    if (eeprom->Atrribute.State)
    {
        read = eeprom->Buffer[addr];
    }
    else
    {
        read = 0xFF;
    }
    
    veeprom_rw_unlock(eeprom);
    
    return read;
}

/**
 *******************************************************************************
 * @brief       veeprom ��ȡ����
 * @param       [in/out]  *eeprom         veeprom���
 * @param       [in/out]  *buf            ��ȡ������
 * @param       [in/out]  addr            ��ȡ����ַ
 * @param       [in/out]  len             ��ȡ���ݳ���
 * @return      [in/out]  0               ��ȡʧ��
 * @return      [in/out]  len             ��ȡ�ɹ�
 * @note        None
 *******************************************************************************
 */
uint16_t HAL_Veeprom_Read(HAL_Veeprom_t *eeprom, uint8_t *buf, uint16_t addr, uint16_t len)
{
    if (addr > eeprom->BufferLen)
    {
        return 0;
    }
    
    if (veeprom_is_rw_lock(eeprom))
    {
        return 0;
    }
    else
    {
        veeprom_rw_lock(eeprom);
    }
    
    if (eeprom->Atrribute.State)
    {
        uint16_t i;
        uint8_t *p = (uint8_t *)&eeprom->Buffer[addr];
        
        for (i=0; i<len; i++)
        {
            buf[i] = p[i];
        }
    }
    else
    {
        len = 0;
    }
    
    veeprom_rw_unlock(eeprom);
    
    return len;
}

/**
 *******************************************************************************
 * @brief       veeprom ��������(�л����ݿ�)
 * @param       [in/out]  *param         veeprom���
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void HAL_Veeprom_Update(void *param)
{
    HAL_Veeprom_t *eeprom = (HAL_Veeprom_t *)param;
    
    if (veeprom_is_rw_lock(eeprom))
    {
        return ;
    }
    
    switch (eeprom->UpdateState)
    {
        case HAL_VEEPROM_INIT:
        {
            if (HAL_Evt_Get(eeprom->Atrribute.Event, HAL_VEEPROM_SWITCH_PAGE0_EVENT))
            {
                HAL_Evt_Clr(eeprom->Atrribute.Event, HAL_VEEPROM_SWITCH_PAGE0_EVENT);
                
                veeprom_unlock();
                eeprom->Addr = eeprom->PhyInfo.Page0Base;
                
                eeprom->UpdateState = HAL_VEEPROM_ERASE;
            }
            else if (HAL_Evt_Get(eeprom->Atrribute.Event, HAL_VEEPROM_SWITCH_PAGE1_EVENT))
            {
                HAL_Evt_Clr(eeprom->Atrribute.Event, HAL_VEEPROM_SWITCH_PAGE1_EVENT);
                
                veeprom_unlock();
                eeprom->Addr = eeprom->PhyInfo.Page1Base;
                
                eeprom->UpdateState = HAL_VEEPROM_ERASE;
            }
            break;
        }
        case HAL_VEEPROM_ERASE:
        {
            veeprom_erase_page(eeprom->Addr);
            eeprom->UpdateState = HAL_VEEPROM_ERASE_WAIT;
            break;
        }
        case HAL_VEEPROM_ERASE_WAIT:
        {
            uint8_t state = veeprom_get_flag(HAL_VEEPROM_ERASE_FLAG, 0, 0);
            uint8_t page  = (eeprom->Addr > eeprom->PhyInfo.Page1Base) ? (1) : (0);
            
            //! д��ɹ�
            if (state == 0)
            {
                eeprom->Addr += eeprom->PhyInfo.PageSize;
                
                if (page == 0)
                {
                    if (eeprom->Addr > eeprom->PhyInfo.Page0End)
                    {
                        eeprom->Addr = eeprom->PhyInfo.Page1Base;
                        eeprom->UpdateState = HAL_UPDATE_INIT;
                    }
                    else
                    {
                        eeprom->UpdateState = HAL_VEEPROM_ERASE;
                    }
                }
                else
                {
                    if (eeprom->Addr > eeprom->PhyInfo.Page1End)
                    {
                        eeprom->Addr = eeprom->PhyInfo.Page0Base;
                        eeprom->UpdateState = HAL_UPDATE_INIT;
                    }
                    else
                    {
                        eeprom->UpdateState = HAL_VEEPROM_ERASE;
                    }
                }
            }
            //! д��ʧ��
            else if (state == 2)
            {
                eeprom->UpdateState = HAL_VEEPROM_HW_ERROR;
            }
            break;
        }
        case HAL_UPDATE_INIT:
        {
            veeprom_write_word(eeprom->Addr, HAL_VEEPROM_PAGE_USE);
            eeprom->UpdateState = HAL_UPDATE_WAIT;
            break;
        }
        case HAL_UPDATE_WAIT:
        {
            uint8_t state = veeprom_get_flag(HAL_VEEPROM_WRITE_FLAG, 0, 0);

            if (state == 0)
            {
                //! У��ɹ�
                if (veeprom_get_flag(HAL_VEEPROM_VERIFY_FLAG, eeprom->Addr, HAL_VEEPROM_PAGE_USE) == 0)
                {
                    eeprom->Addr += HAL_VEEPROM_ALIGNMENT;
                    eeprom->UpdateState = HAL_VEEPROM_WRITE;
                }
                //! У��ʧ��
                else
                {
                    eeprom->UpdateState = HAL_UPDATE_INIT;
                }
            }
            //! д��ʧ��
            else if (state == 2)
            {
                eeprom->UpdateState = HAL_VEEPROM_HW_ERROR;
            }
            break;
        }
        case HAL_VEEPROM_WRITE:
        {
            uint8_t page  = (eeprom->Addr > eeprom->PhyInfo.Page1Base) ? (1) : (0);
            uint32_t pos = (page) ? (eeprom->Addr - eeprom->PhyInfo.Page1Base) : (eeprom->Addr - eeprom->PhyInfo.Page0Base);
            uint32_t wrData = *((uint32_t *)&eeprom->Buffer[pos]);
            
            veeprom_write_word(eeprom->Addr, wrData);
            
            eeprom->UpdateState = HAL_VEEPROM_WRITE_WAIT;
            break;
        }
        case HAL_VEEPROM_WRITE_WAIT:
        {
            uint8_t state = veeprom_get_flag(HAL_VEEPROM_WRITE_FLAG, 0, 0);
            uint8_t page  = (eeprom->Addr > eeprom->PhyInfo.Page1Base) ? (1) : (0);
            uint32_t pos = (page) ? (eeprom->Addr - eeprom->PhyInfo.Page1Base) : (eeprom->Addr - eeprom->PhyInfo.Page0Base);
            uint32_t wrData = *((uint32_t *)&eeprom->Buffer[pos]);
            
            if (state == 0)
            {
                //! У��ɹ�
                if (veeprom_get_flag(HAL_VEEPROM_VERIFY_FLAG, eeprom->Addr, wrData) == 0)
                {
                    eeprom->Addr += HAL_VEEPROM_ALIGNMENT;

                    if (eeprom->Addr > eeprom->BufferLen)
                    {
                        veeprom_lock();
                        eeprom->UpdateState = HAL_VEEPROM_IDLE;
                    }
                    else
                    {
                        eeprom->UpdateState = HAL_VEEPROM_WRITE;
                    }
                }
                //! У��ʧ��
                else
                {
                    eeprom->UpdateState = HAL_FLASH_WRITE;
                }
            }
            //! д��ʧ��
            else if (state == 2)
            {
                eeprom->UpdateState = HAL_VEEPROM_HW_ERROR;
            }
            break;
        }
        default:
            break;
    }
}
#endif

/** @}*/     /** virtual eeprom component */

/**********************************END OF FILE*********************************/
