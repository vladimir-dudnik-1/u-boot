/**
  ******************************************************************************
  * @file    bl808_ef_ctrl.c
  * @version V1.0
  * @date
  * @brief   This file is the standard driver c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020 Bouffalo Lab</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of Bouffalo Lab nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "string.h"
#include "bl808_ef_ctrl.h"
#include <bl808/ef_data_0_reg.h>
#include <bl808/ef_data_1_reg.h>

/** @addtogroup  BL808_Peripheral_Driver
 *  @{
 */

/** @addtogroup  SEC_EF_CTRL
 *  @{
 */

/** @defgroup  SEC_EF_CTRL_Private_Macros
 *  @{
 */
#define EF_CTRL_EFUSE_CYCLE_PROTECT (0xbf << 24)
#define EF_CTRL_EFUSE_CTRL_PROTECT  (0xbf << 8)
#define EF_CTRL_DFT_TIMEOUT_VAL     (320 * 1000)
#ifndef BOOTROM
#define EF_CTRL_LOAD_BEFORE_READ_R0 EF_Ctrl_Load_Efuse_R0()
#define EF_CTRL_LOAD_BEFORE_READ_R1 EF_Ctrl_Load_Efuse_R1()
#else
#define EF_CTRL_LOAD_BEFORE_READ_R0
#define EF_CTRL_LOAD_BEFORE_READ_R1
#endif
#define EF_CTRL_DATA0_CLEAR EF_Ctrl_Clear(0, 0, EF_CTRL_EFUSE_R0_SIZE / 4)
#define EF_CTRL_DATA1_CLEAR EF_Ctrl_Clear(1, 0, EF_CTRL_EFUSE_R1_SIZE / 4)

/*@} end of group SEC_EF_CTRL_Private_Macros */

/** @defgroup  SEC_EF_CTRL_Private_Types
 *  @{
 */

/*@} end of group SEC_EF_CTRL_Private_Types */

/** @defgroup  SEC_EF_CTRL_Private_Variables
 *  @{
 */

/*@} end of group SEC_EF_CTRL_Private_Variables */

/** @defgroup  SEC_EF_CTRL_Global_Variables
 *  @{
 */

/*@} end of group SEC_EF_CTRL_Global_Variables */

/** @defgroup  SEC_EF_CTRL_Private_Fun_Declaration
 *  @{
 */

/*@} end of group SEC_EF_CTRL_Private_Fun_Declaration */

/** @defgroup  SEC_EF_CTRL_Private_Functions
 *  @{
 */

/****************************************************************************/ /**
 * @brief  Switch efuse region 0 control to AHB clock
 *
 * @param  None
 *
 * @return None
 *
*******************************************************************************/
#ifndef BFLB_USE_ROM_DRIVER
void EF_Ctrl_Sw_AHB_Clk_0(void)
{
    uint32_t tmpVal;
    uint32_t timeout = EF_CTRL_DFT_TIMEOUT_VAL;

    while (EF_Ctrl_Busy() == SET) {
        timeout--;

        if (timeout == 0) {
            break;
        }
    }

    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_0_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_0_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_AUTO_RD_EN_POS) |
             (0 << EF_CTRL_EF_IF_POR_DIG_POS) |
             (1 << EF_CTRL_EF_IF_0_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_0_RW_POS) |
             (0 << EF_CTRL_EF_IF_0_TRIG_POS);

    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0, tmpVal);
}
#endif

/****************************************************************************/ /**
 * @brief  Switch efuse region 1 control to AHB clock
 *
 * @param  None
 *
 * @return None
 *
*******************************************************************************/
#ifndef BFLB_USE_ROM_DRIVER
void EF_Ctrl_Sw_AHB_Clk_1(void)
{
    uint32_t tmpVal;
    uint32_t timeout = EF_CTRL_DFT_TIMEOUT_VAL;

    while (EF_Ctrl_Busy() == SET) {
        timeout--;

        if (timeout == 0) {
            break;
        }
    }

    /* Note:ef_if_ctrl_1 has no EF_CTRL_EF_CLK_SAHB_DATA_SEL_POS bit as ef_if_ctrl_0,
	   so we select it(them) in ef_if_ctrl_0 */
    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_0_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_0_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_AUTO_RD_EN_POS) |
             (0 << EF_CTRL_EF_IF_POR_DIG_POS) |
             (1 << EF_CTRL_EF_IF_0_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_0_RW_POS) |
             (0 << EF_CTRL_EF_IF_0_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0, tmpVal);

    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_1_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_1_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_1_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_1_RW_POS) |
             (0 << EF_CTRL_EF_IF_1_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_1, tmpVal);
}
#endif

/****************************************************************************/ /**
 * @brief  Load efuse region 0
 *
 * @param  None
 *
 * @return None
 *
*******************************************************************************/
#ifndef BFLB_USE_ROM_DRIVER
void EF_Ctrl_Load_Efuse_R0(void)
{
    uint32_t tmpVal;
    uint32_t timeout = EF_CTRL_DFT_TIMEOUT_VAL;

    EF_CTRL_DATA0_CLEAR;

    /* Trigger read */
    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_0_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_0_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_AUTO_RD_EN_POS) |
             (0 << EF_CTRL_EF_IF_POR_DIG_POS) |
             (1 << EF_CTRL_EF_IF_0_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_0_RW_POS) |
             (0 << EF_CTRL_EF_IF_0_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0, tmpVal);

    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_0_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_0_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_AUTO_RD_EN_POS) |
             (0 << EF_CTRL_EF_IF_POR_DIG_POS) |
             (1 << EF_CTRL_EF_IF_0_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_0_RW_POS) |
             (1 << EF_CTRL_EF_IF_0_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0, tmpVal);

    arch_delay_us(10);

    /* Wait for efuse control idle */
    do {
        tmpVal = BL_RD_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0);
        timeout--;

        if (timeout == 0) {
            break;
        }
    } while (BL_IS_REG_BIT_SET(tmpVal, EF_CTRL_EF_IF_0_BUSY) ||

             (!BL_IS_REG_BIT_SET(tmpVal, EF_CTRL_EF_IF_0_AUTOLOAD_DONE)));

    /* Switch to AHB clock */
    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_0_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_0_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_AUTO_RD_EN_POS) |
             (0 << EF_CTRL_EF_IF_POR_DIG_POS) |
             (1 << EF_CTRL_EF_IF_0_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_0_RW_POS) |
             (0 << EF_CTRL_EF_IF_0_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0, tmpVal);
}
#endif

/****************************************************************************/ /**
 * @brief  Load efuse region 1
 *
 * @param  None
 *
 * @return None
 *
*******************************************************************************/
#ifndef BFLB_USE_ROM_DRIVER
void EF_Ctrl_Load_Efuse_R1(void)
{
    uint32_t tmpVal;

    EF_CTRL_DATA1_CLEAR;

    /* Trigger read */
    /* Note:ef_if_ctrl_1 has no EF_CTRL_EF_CLK_SAHB_DATA_SEL_POS bit as ef_if_ctrl_0,
	so we select it(them) in ef_if_ctrl_0 */
    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_0_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_0_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_AUTO_RD_EN_POS) |
             (0 << EF_CTRL_EF_IF_POR_DIG_POS) |
             (1 << EF_CTRL_EF_IF_0_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_0_RW_POS) |
             (0 << EF_CTRL_EF_IF_0_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0, tmpVal);

    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_1_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_1_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_1_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_1_RW_POS) |
             (0 << EF_CTRL_EF_IF_1_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_1, tmpVal);

    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_1_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_1_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_1_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_1_RW_POS) |
             (1 << EF_CTRL_EF_IF_1_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_1, tmpVal);

    arch_delay_us(10);

    /* Wait for efuse control idle */
    do {
        tmpVal = BL_RD_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_1);
    } while (BL_IS_REG_BIT_SET(tmpVal, EF_CTRL_EF_IF_1_BUSY));

    do {
        tmpVal = BL_RD_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0);
    } while (!BL_IS_REG_BIT_SET(tmpVal, EF_CTRL_EF_IF_0_AUTOLOAD_DONE));

    /* Switch to AHB clock since often read efuse data after load */
    /* Note:ef_if_ctrl_1 has no EF_CTRL_EF_CLK_SAHB_DATA_SEL_POS bit as ef_if_ctrl_0,
	   so we select it(them) in ef_if_ctrl_0 */
    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_0_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_0_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_AUTO_RD_EN_POS) |
             (0 << EF_CTRL_EF_IF_POR_DIG_POS) |
             (1 << EF_CTRL_EF_IF_0_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_0_RW_POS) |
             (0 << EF_CTRL_EF_IF_0_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0, tmpVal);

    tmpVal = (EF_CTRL_EFUSE_CTRL_PROTECT) |
             (EF_CTRL_OP_MODE_AUTO << EF_CTRL_EF_IF_1_MANUAL_EN_POS) |
             (EF_CTRL_PARA_DFT << EF_CTRL_EF_IF_1_CYC_MODIFY_POS) |
             (1 << EF_CTRL_EF_IF_1_INT_CLR_POS) |
             (0 << EF_CTRL_EF_IF_1_RW_POS) |
             (0 << EF_CTRL_EF_IF_1_TRIG_POS);
    BL_WR_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_1, tmpVal);
}
#endif

/****************************************************************************/ /**
 * @brief  Check efuse busy status
 *
 * @param  None
 *
 * @return SET or RESET
 *
*******************************************************************************/
#ifndef BFLB_USE_ROM_DRIVER
BL_Sts_Type EF_Ctrl_Busy(void)
{
    if (BL_IS_REG_BIT_SET(BL_RD_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_0), EF_CTRL_EF_IF_0_BUSY) ||
        BL_IS_REG_BIT_SET(BL_RD_REG(EF_CTRL_BASE, EF_CTRL_EF_IF_CTRL_1), EF_CTRL_EF_IF_1_BUSY)) {
        return SET;
    }

    return RESET;
}
#endif

/****************************************************************************/ /**
 * @brief  Clear efuse data register
 *
 * @param  region: index efuse region
 * @param  index: index of efuse in word
 * @param  len: data length
 *
 * @return None
 *
*******************************************************************************/
#ifndef BFLB_USE_ROM_DRIVER
void EF_Ctrl_Clear(uint8_t region, uint32_t index, uint32_t len)
{
    uint32_t *pEfuseStart0 = (uint32_t *)(EF_DATA_BASE + 0x00);
    uint32_t *pEfuseStart1 = (uint32_t *)(EF_DATA_BASE + 0x80);
    uint32_t i = 0;

    if (region == 0) {
        /* Switch to AHB clock */
        EF_Ctrl_Sw_AHB_Clk_0();

        /* Clear data */
        for (i = 0; i < len; i++) {
            pEfuseStart0[index + i] = 0;
        }
    } else if (region == 1) {
        /* Switch to AHB clock */
        EF_Ctrl_Sw_AHB_Clk_1();

        /* Clear data */
        for (i = 0; i < len; i++) {
            pEfuseStart1[index + i] = 0;
        }
    }
}
#endif
