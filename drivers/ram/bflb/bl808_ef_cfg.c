/**
  ******************************************************************************
  * @file    bl808_ef_cfg.c
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
#include "bl808_ef_cfg.h"
#include <bl808/ef_data_0_reg.h>
#include <bl808/ef_data_1_reg.h>

#define EF_CTRL_LOAD_BEFORE_READ_R0 EF_Ctrl_Load_Efuse_R0()
#define EF_CTRL_LOAD_BEFORE_READ_R1 EF_Ctrl_Load_Efuse_R1()

/****************************************************************************/ /**
 * @brief  Efuse get chip info
 *
 * @param  chipInfo: info pointer
 *
 * @return None
 *
*******************************************************************************/
void EF_Ctrl_Get_Chip_Info(Efuse_Chip_Info_Type *chipInfo)
{
    uint32_t tmpVal;

    /* Trigger read data from efuse */
    EF_CTRL_LOAD_BEFORE_READ_R0;

    tmpVal = BL_RD_REG(EF_DATA_BASE, EF_DATA_0_EF_WIFI_MAC_HIGH);
    chipInfo->chipInfo = (tmpVal>>29)&0x7;
    chipInfo->memoryInfo = (tmpVal>>27)&0x3;
    chipInfo->psramInfo = (tmpVal>>25)&0x3;
    chipInfo->deviceInfo = (tmpVal>>22)&0x7;

    tmpVal = BL_RD_REG(EF_DATA_BASE, EF_DATA_0_EF_CFG_0);
    chipInfo->psramInfo |= ((tmpVal>>20)&0x1) << 2;
}

/****************************************************************************/ /**
 * @brief  Efuse read psram trim configuration
 *
 * @param  trim: Trim data pointer
 *
 * @return None
 *
*******************************************************************************/
void EF_Ctrl_Read_Psram_Trim(Efuse_Psram_Trim_Type *trim)
{
    uint32_t tmpVal;

    /* Trigger read data from efuse */
    EF_CTRL_LOAD_BEFORE_READ_R1;

    tmpVal = BL_RD_REG(EF_DATA_BASE, EF_DATA_1_EF_KEY_SLOT_10_W2);
    trim->psramTrim = (tmpVal >> 0) & 0x7ff;
    trim->psramTrimParity = (tmpVal >> 11) & 0x01;
    trim->psramTrimEn = (tmpVal >> 12) & 0x01;
}
