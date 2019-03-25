/**
 *******************************************************************************
 * @file       hal_tm1628.h                                                    *
 * @author     yearnext                                                        *
 * @version    1.0.0                                                           *
 * @date       2018-09-25                                                      *
 * @brief      tmm1628 driver component head file                              *
 * @par        work platform                                                   *
 *                 Windows                                                     *
 * @par        compiler                                                        *
 *                 GCC                                                         *
 *******************************************************************************
 * @note                                                                       *
 * 1. 20180925 �����ļ�                                                        *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_TM1628_H__
#define __HAL_TM1628_H__

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       TM1628 GRID��������
 *******************************************************************************
 */
#define TM1628_GRID_MAX          (7)

/**
 *******************************************************************************
 * @brief       TM1628 �����
 *******************************************************************************
 */
#define TM1628_CMD_ADDR_MASK     (0xC0)
#define TM1628_CMD_DISP_MASK     (0x80)
#define TM1628_DISP_LUMIN_MASK   (0x08)
//! �ر���ʾ����
#define TM1628_CMD_DISP_OFF      (TM1628_CMD_DISP_MASK)
//! ���ȵȼ�����
#define TM1628_CMD_LUMIN_LV1     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x00)
#define TM1628_CMD_LUMIN_LV2     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x01)
#define TM1628_CMD_LUMIN_LV3     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x02)
#define TM1628_CMD_LUMIN_LV4     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x03)
#define TM1628_CMD_LUMIN_LV5     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x04)
#define TM1628_CMD_LUMIN_LV6     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x05)
#define TM1628_CMD_LUMIN_LV7     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x06)
#define TM1628_CMD_LUMIN_LV8     (TM1628_CMD_DISP_MASK|TM1628_DISP_LUMIN_MASK|0x07)
//! ���ݶ�д����
#define TM1628_CMD_READ          (0x42)
#define TM1628_CMD_WRITE         (0x40)
//! ��ַ����
#define TM1628_CMD_ADDR_INC      (0x40)
#define TM1628_CMD_ADDR_FIX      (0x44)
//! ������ʾģʽ
#define TM1628_CMD_DRV_4BIT13SEG (0x00)
#define TM1628_CMD_DRV_5BIT13SEG (0x01)
#define TM1628_CMD_DRV_6BIT13SEG (0x02)
#define TM1628_CMD_DRV_7BIT13SEG (0x03)

/**
 *******************************************************************************
 * @brief       TM1628 ָʾ����ʾλ�ö���
 *******************************************************************************
 */
//! ������ͼ�� λ�ö���
#define LIGHT_BIT_POS       5
#define LIGHT_SEG_POS       10
//! ����� λ�ö���
#define TEMP_HIGH_A_BIT_POS 1
#define TEMP_HIGH_A_SEG_POS 10
#define TEMP_HIGH_B_BIT_POS 1
#define TEMP_HIGH_B_SEG_POS 9
#define TEMP_HIGH_C_BIT_POS 1
#define TEMP_HIGH_C_SEG_POS 8
#define TEMP_HIGH_D_BIT_POS 1
#define TEMP_HIGH_D_SEG_POS 5
#define TEMP_HIGH_E_BIT_POS 1
#define TEMP_HIGH_E_SEG_POS 4
#define TEMP_HIGH_F_BIT_POS 3
#define TEMP_HIGH_F_SEG_POS 10
#define TEMP_HIGH_G_BIT_POS 3
#define TEMP_HIGH_G_SEG_POS 9
#define TEMP_LOW_A_BIT_POS  2
#define TEMP_LOW_A_SEG_POS  10
#define TEMP_LOW_B_BIT_POS  2
#define TEMP_LOW_B_SEG_POS  9
#define TEMP_LOW_C_BIT_POS  2
#define TEMP_LOW_C_SEG_POS  8
#define TEMP_LOW_D_BIT_POS  2
#define TEMP_LOW_D_SEG_POS  5
#define TEMP_LOW_E_BIT_POS  2
#define TEMP_LOW_E_SEG_POS  4
#define TEMP_LOW_F_BIT_POS  4
#define TEMP_LOW_F_SEG_POS  10
#define TEMP_LOW_G_BIT_POS  4
#define TEMP_LOW_G_SEG_POS  9
//! �¶�ͼ�� λ�ö���
#define TEMP_ICON_BIT_POS   4
#define TEMP_ICON_SEG_POS   8
//! ����ͼ�� λ�ö���
#define FAN_BIT_POS         3
#define FAN_SEG_POS         4
//! ���ͼ�� λ�ö���
#define HOT_BIT_POS         4
#define HOT_SEG_POS         4
//! ����ͼ�� λ�ö���
#define DISINFECT_BIT_POS   6
#define DISINFECT_SEG_POS   10
//! �ȼ�ͼ�� λ�ö���
#define LEVEL_1_BIT_POS     5
#define LEVEL_1_SEG_POS     9
#define LEVEL_2_BIT_POS     5
#define LEVEL_2_SEG_POS     8
#define LEVEL_3_BIT_POS     5
#define LEVEL_3_SEG_POS     5
#define LEVEL_4_BIT_POS     5
#define LEVEL_4_SEG_POS     4
#define LEVEL_5_BIT_POS     6
#define LEVEL_5_SEG_POS     4
#define LEVEL_6_BIT_POS     6
#define LEVEL_6_SEG_POS     5
#define LEVEL_7_BIT_POS     6
#define LEVEL_7_SEG_POS     8
#define LEVEL_8_BIT_POS     6
#define LEVEL_8_SEG_POS     9

/* Exported types ------------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief      TM1628�����ӿ�
 *******************************************************************************
 */
typedef struct
{
    void (*STB_Port)(uint8_t flag);
    void (*CLK_Port)(uint8_t flag);
    
    uint8_t (*DIO_Port)(uint8_t flag);
}TM1628_Interface_t;

typedef struct
{
    HAL_Atrribute_t Attribute;

    TM1628_Interface_t Ops;

    uint16_t Cache[TM1628_GRID_MAX];
}HAL_TM1628_t;

/* Exported constants --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void    HAL_TM1628_Init(HAL_TM1628_t *dev, TM1628_Interface_t *ops);
extern void    HAL_TM1628_WriteBit(HAL_TM1628_t *dev, uint8_t bitPos, uint8_t segPos, uint8_t flag);
extern uint8_t HAL_TM1628_ReadBit(HAL_TM1628_t *dev, uint8_t bitPos, uint8_t segPos);
extern void    HAL_TM1628_Update(void *param);

/* Add c++ compatibility------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif      /** prevent recursive inclusion */

/**********************************END OF FILE*********************************/
