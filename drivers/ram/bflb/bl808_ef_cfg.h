/**
  ******************************************************************************
  * @file    bl808_ef_ctrl.h
  * @version V1.0
  * @date
  * @brief   This file is the standard driver header file
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
#ifndef __BL808_EF_CFG_H__
#define __BL808_EF_CFG_H__

#include <bl808/ef_ctrl_reg.h>
#include "bl808_common.h"

/**
 *  @brief Efuse analog device info type definition
 */
typedef struct
{
    uint8_t chipInfo;   /*!< Efuse chip revision */
    uint8_t memoryInfo; /*!< Efuse memory info 0:no memory, 8:1MB flash */
    uint8_t psramInfo;  /*!< Efuse psram info 0:no psram, 1:WB 4MB, 2:UHS 32MB, 3:UHS 64MB, 4:WB 32MB */
    uint8_t deviceInfo; /*!< Efuse device information */
} Efuse_Chip_Info_Type;

/**
 *  @brief Efuse psram trim type definition
 */
typedef struct
{
    uint32_t psramTrim       : 11; /*!< Efuse analog trim:psram trim date */
    uint32_t psramTrimParity : 1;  /*!< Efuse analog trim:psram trim_parity */
    uint32_t psramTrimEn     : 1;  /*!< Efuse analog trim:psram trim_en */
    uint32_t reserved        : 19; /*!< Efuse analog trim:reserved */
} Efuse_Psram_Trim_Type;

void EF_Ctrl_Get_Chip_Info(Efuse_Chip_Info_Type *chipInfo);
void EF_Ctrl_Read_Psram_Trim(Efuse_Psram_Trim_Type *trim);

#endif /* __BL808_EF_CFG_H__ */
