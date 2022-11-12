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
#ifndef __BL808_EF_CTRL_H__
#define __BL808_EF_CTRL_H__

#include <bl808/ef_ctrl_reg.h>
#include "bl808_common.h"

/**
 *  @brief Efuse Ctrl clock type definition
 */
typedef enum {
    EF_CTRL_PARA_DFT,    /*!< Select default cyc parameter */
    EF_CTRL_PARA_MANUAL, /*!< Select manual cyc parameter */
} EF_Ctrl_CYC_PARA_Type;

/**
 *  @brief Efuse Ctrl clock type definition
 */
typedef enum {
    EF_CTRL_OP_MODE_AUTO,   /*!< Select efuse program auto mode */
    EF_CTRL_OP_MODE_MANUAL, /*!< Select efuse program manual mode */
} EF_Ctrl_OP_MODE_Type;

/**
 *  @brief Efuse analog device info type definition
 */
typedef struct
{
    uint32_t rsvd       : 22; /*!< Reserved */
    uint32_t deviceInfo : 3;  /*!< Efuse device information */
    uint32_t psramInfo  : 2;  /*!< Efuse psram info 0:no psram, 1:BW 4MB, 2:UHS 64MB */
    uint32_t memoryInfo : 2;  /*!< Efuse memory info 0:no memory, 8:1MB flash */
    uint32_t chipInfo   : 3;  /*!< Efuse chip revision */
} Efuse_Device_Info_Type;

#define EF_CTRL_EFUSE_R0_SIZE 128
#define EF_CTRL_EFUSE_R1_SIZE 128

void EF_Ctrl_Load_Efuse_R0(void);
void EF_Ctrl_Load_Efuse_R1(void);
BL_Sts_Type EF_Ctrl_Busy(void);
void EF_Ctrl_Clear(uint8_t region, uint32_t index, uint32_t len);
void EF_Ctrl_Sw_AHB_Clk_0(void);
void EF_Ctrl_Sw_AHB_Clk_1(void);

#endif /* __BL808_EF_CTRL_H__ */
