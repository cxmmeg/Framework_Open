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
 * @file       hal_veeprom.h                                                   *
 * @author     Accumulate Team                                                 *
 * @version    1.0.0                                                           *
 * @date       2017-10-27                                                      *
 * @brief      mcu virtual eeprom component server head file                   *
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
 * 1. 20171027    �����ļ�"veeprom.h"                                          *
 * 2. 20180829    ��veeprom���Ǩ�Ƶ�hal���У�����Ϊ"hal_veeprom.h"            *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_VIRTUAL_EEPROM_H__
#define __HAL_VIRTUAL_EEPROM_H__

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//! ����ҳ״̬
#define HAL_VEEPROM_PAGE_INVAILD                                    (0x5A8888A5)
#define HAL_VEEPROM_PAGE_INIT                                       (0xFFFFFFFF)
#define HAL_VEEPROM_PAGE_FREE                                       (0x5AFFFFA5)
#define HAL_VEEPROM_PAGE_USE                                        (0x5ACCCCA5)

//! Ӳ���쳣״̬
#define HAL_VEEPROM_NORMAL                                                   (0)
#define HAL_VEEPROM_FIX                                                      (1)
#define HAL_VEEPROM_EXCEPTION                                                (2)

//! ����ҳ״̬
#define HAL_VEEPROM_SET_PAGE0                                                (0)
#define HAL_VEEPROM_USE_PAGE0                                                (1)
#define HAL_VEEPROM_SET_PAGE1                                                (2)
#define HAL_VEEPROM_USE_PAGE1                                                (3)

//! VEEPROM�¼�
#define HAL_VEEPROM_SWITCH_PAGE0_EVENT                                       (0)
#define HAL_VEEPROM_SWITCH_PAGE1_EVENT                                       (1)

//! VEEPROM ��ʶ
#define HAL_VEEPROM_LOCK_FLAG                                           (0x0001)
#define HAL_VEEPROM_VERIFY_FLAG                                         (0x0002)
#define HAL_VEEPROM_ERASE_FLAG                                          (0x0004)
#define HAL_VEEPROM_WRITE_FLAG                                          (0x0008)

//! define flash state
enum __HAL_VEEPROM_STATE
{
    HAL_VEEPROM_INIT = 0,
    
    HAL_VEEPROM_IDLE,
    
    HAL_VEEPROM_ERASE,
    HAL_VEEPROM_ERASE_WAIT,
    HAL_UPDATE_INIT,
    HAL_UPDATE_WAIT,
    HAL_VEEPROM_WRITE,
    HAL_VEEPROM_WRITE_WAIT,
    
    HAL_VEEPROM_HW_ERROR,
};

/* Exported types ------------------------------------------------------------*/
//! veeprom update ����ֵ
typedef enum
{
    HAL_VEEPROM_RET_NONE,
    HAL_VEEPROM_RET_NORMAL,
    HAL_VEEPROM_RET_ERROR,
    HAL_VEEPROM_RET_SUCCESS,
    HAL_VEEPROM_RET_SWITCH,
    HAL_VEEPROM_RET_TRANSFER,
    HAL_VEEPROM_RET_UPDATE,
    HAL_VEEPROM_RET_RESET,
    HAL_VEEPROM_RET_LOAD,
    HAL_VEEPROM_RET_FIX,
    HAL_VEEPROM_RET_FILL_BLOCK0,
    HAL_VEEPROM_RET_FILL_BLOCK1,
    HAL_VEEPROM_RET_HW_ERROR,
}VeepromState;

//! Veeprom Ӳ�������ӿ�
typedef struct 
{
    uint32_t (*ReadWord)(uint32_t addr);
    void     (*WriteWord)(uint32_t addr, uint32_t wdata);
    void     (*Erase)(uint32_t addr);
    void     (*Lock)(void);
    void     (*Unlock)(void);
    uint32_t (*GetFlag)(uint32_t flag, uint32_t addr, uint32_t vData);
}HAL_Veeprom_Interface_t;

typedef struct
{
    //! ҳ�����ַ
    size_t Page0Base;
    size_t Page0End;
    size_t Page1Base;
    size_t Page1End;
    //! ����ҳ��С
    size_t PageSize;
}VeepromInfo;

typedef struct
{
    VeepromInfo Atrribute;
    
    //! RAM������
    volatile uint8_t *Buffer;
    
    //! EEPROM�ռ�
    uint16_t BufferLen;
}HAL_Veeprom_Config_t;

typedef struct
{
    HAL_Atrribute_t   Atrribute;
    VeepromInfo PhyInfo;

    //! ��ǰҳ��ַ
    size_t Addr;
    
    //! RAM������
    volatile uint8_t *Buffer;
    
    //! EEPROM�ռ�
    uint16_t BufferLen;
    
    //! ҳ״̬
    uint8_t PageState;
    
    //! ���С
    uint16_t BlockNum; 

    //! ���ݷ����л�״̬
    uint8_t UpdateState; 
}HAL_Veeprom_t;

/* Exported constants --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void     HAL_Veeprom_Register(HAL_Veeprom_Interface_t *ops, void *userData);
extern uint8_t  HAL_Veeprom_Init(HAL_Veeprom_t *eeprom, HAL_Veeprom_Config_t *config);
extern uint8_t  HAL_Veeprom_WriteByte(HAL_Veeprom_t *eeprom, uint16_t addr, uint8_t ch);
extern uint16_t HAL_Veeprom_Write(HAL_Veeprom_t *eeprom, uint8_t *buf, uint16_t addr, uint16_t len);
extern uint8_t  HAL_Veeprom_ReadByte(HAL_Veeprom_t *eeprom, uint32_t addr);
extern uint16_t HAL_Veeprom_Read(HAL_Veeprom_t *eeprom, uint8_t *buf, uint16_t addr, uint16_t len);

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif      /** prevent recursive inclusion */

/**********************************END OF FILE*********************************/
