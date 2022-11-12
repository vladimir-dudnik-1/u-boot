/**
  ******************************************************************************
  * @file    bl808_glb_pll.c
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

#include <bl808/cci_reg.h>
#include "bl808_glb.h"

/* uhs PLL 2000 Config*/
const GLB_MU_PLL_CFG_BASIC_Type uhsPll2000BasicCfg_24M = {
    .clkpllRefdivRatio = 1,     /*!< pll_refdiv_ratio */
    .clkpllSelSampleClk = 2,    /*!< pll_sel_sample_clk */
    .clkpllVcoSpeed = 7,        /*!< pll_vco_speed */
    .clkpllEvenDivEn = 1,       /*!< pll_even_div_en */
    .clkpllEvenDivRatio = 2000/50, /*!< pll_even_div_ratio */
};
const GLB_MU_PLL_CFG_BASIC_Type uhsPll2000BasicCfg_32M = {
    .clkpllRefdivRatio = 2,     /*!< pll_refdiv_ratio */
    .clkpllSelSampleClk = 2,    /*!< pll_sel_sample_clk */
    .clkpllVcoSpeed = 7,        /*!< pll_vco_speed */
    .clkpllEvenDivEn = 1,       /*!< pll_even_div_en */
    .clkpllEvenDivRatio = 2000/50, /*!< pll_even_div_ratio */
};
const GLB_MU_PLL_CFG_BASIC_Type uhsPll2000BasicCfg_38P4M = {
    .clkpllRefdivRatio = 2,     /*!< pll_refdiv_ratio */
    .clkpllSelSampleClk = 2,    /*!< pll_sel_sample_clk */
    .clkpllVcoSpeed = 7,        /*!< pll_vco_speed */
    .clkpllEvenDivEn = 1,       /*!< pll_even_div_en */
    .clkpllEvenDivRatio = 2000/50, /*!< pll_even_div_ratio */
};
const GLB_MU_PLL_CFG_BASIC_Type uhsPll2000BasicCfg_40M = {
    .clkpllRefdivRatio = 2,     /*!< pll_refdiv_ratio */
    .clkpllSelSampleClk = 2,    /*!< pll_sel_sample_clk */
    .clkpllVcoSpeed = 7,        /*!< pll_vco_speed */
    .clkpllEvenDivEn = 1,       /*!< pll_even_div_en */
    .clkpllEvenDivRatio = 2000/50, /*!< pll_even_div_ratio */
};
const GLB_MU_PLL_CFG_BASIC_Type uhsPll2000BasicCfg_26M = {
    .clkpllRefdivRatio = 1,     /*!< pll_refdiv_ratio */
    .clkpllSelSampleClk = 1,    /*!< pll_sel_sample_clk */
    .clkpllVcoSpeed = 7,        /*!< pll_vco_speed */
    .clkpllEvenDivEn = 1,       /*!< pll_even_div_en */
    .clkpllEvenDivRatio = 2000/50, /*!< pll_even_div_ratio */
};
const GLB_MU_PLL_Cfg_Type uhsPllCfg_2000M[GLB_XTAL_MAX] = {
    { NULL, 0x0 },                      /*!< XTAL is None */
    { &uhsPll2000BasicCfg_24M, 0x29AAA },   /*!< XTAL is 24M */
    { &uhsPll2000BasicCfg_32M, 0x3E800 },   /*!< XTAL is 32M */
    { &uhsPll2000BasicCfg_38P4M, 0x34155 }, /*!< XTAL is 38.4M */
    { &uhsPll2000BasicCfg_40M, 0x32000 },   /*!< XTAL is 40M */
    { &uhsPll2000BasicCfg_26M, 0x26762 },   /*!< XTAL is 26M */
    { &uhsPll2000BasicCfg_32M, 0x3E800 },   /*!< XTAL is RC32M */
};

/****************************************************************************/ /**
 * @brief  GLB power off mipi uhs PLL
 *
 * @param  pllType: PLL XTAL type
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
BL_Err_Type GLB_Power_Off_MU_PLL(GLB_MU_PLL_Type pllType)
{
    uint32_t REG_PLL_BASE_ADDRESS = 0;
    uint32_t tmpVal = 0;

    CHECK_PARAM(IS_GLB_Power_Off_MU_TYPE(pllType));

    switch (pllType) {
        case GLB_MU_PLL_MIPIPLL:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_MIPI_PLL_CFG0_OFFSET;
            break;
        case GLB_MU_PLL_UHSPLL:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_UHS_PLL_CFG0_OFFSET;
            break;
        default:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_MIPI_PLL_CFG0_OFFSET;
            break;
    }

    /* cfg0 : pu_aupll=0 */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_PU_AUPLL, 0);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);

    /* cfg0 : pu_aupll_sfreg=0 */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_PU_AUPLL_SFREG, 0);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GLB mipi uhs PLL ref clock select
 *
 * @param  pllType: PLL XTAL type
 * @param  refClk: PLL ref clock select
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
BL_Err_Type GLB_MU_PLL_Ref_Clk_Sel(GLB_MU_PLL_Type pllType, GLB_PLL_REF_CLK_Type refClk)
{
    uint32_t REG_PLL_BASE_ADDRESS = 0;
    uint32_t tmpVal = 0;

    CHECK_PARAM(IS_GLB_WAC_PLL_TYPE(pllType));
    CHECK_PARAM(IS_GLB_PLL_REF_CLK_TYPE(refClk));

    switch (pllType) {
        case GLB_MU_PLL_MIPIPLL:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_MIPI_PLL_CFG0_OFFSET;
            break;
        case GLB_MU_PLL_UHSPLL:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_UHS_PLL_CFG0_OFFSET;
            break;
        default:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_MIPI_PLL_CFG0_OFFSET;
            break;
    }

    /* xxxpll_refclk_sel */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 1);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_REFCLK_SEL, refClk);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 1, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GLB power on PLL
 *
 * @param  pllType: PLL XTAL type
 * @param  cfg: GLB PLL configuration
 * @param  waitStable: wait PLL stable
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
BL_Err_Type GLB_Power_On_MU_PLL(GLB_MU_PLL_Type pllType, const GLB_MU_PLL_Cfg_Type *const cfg, uint8_t waitStable)
{
    uint32_t REG_PLL_BASE_ADDRESS = 0;
    uint32_t tmpVal = 0;

    /* unknown */
    CHECK_PARAM(IS_GLB_WAC_PLL_TYPE(pllType));

    switch (pllType) {
        case GLB_MU_PLL_MIPIPLL:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_MIPI_PLL_CFG0_OFFSET;
            break;
        case GLB_MU_PLL_UHSPLL:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_UHS_PLL_CFG0_OFFSET;
            break;
        default:
            REG_PLL_BASE_ADDRESS = GLB_BASE + GLB_MIPI_PLL_CFG0_OFFSET;
            break;
    }

    /* Step1:config parameter */
    /* cfg1:Set aupll_refclk_sel and aupll_refdiv_ratio */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 1);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_REFDIV_RATIO, cfg->basicCfg->clkpllRefdivRatio);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 1, tmpVal);

    /* cfg4:Set aupll_sel_sample_clk */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 4);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_SEL_SAMPLE_CLK, cfg->basicCfg->clkpllSelSampleClk);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 4, tmpVal);

    /* cfg5:Set aupll_vco_speed */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 5);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_VCO_SPEED, cfg->basicCfg->clkpllVcoSpeed);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 5, tmpVal);

    /* cfg1: uhspll_even_div_en and uhspll_even_div_ratio */
    if (GLB_MU_PLL_UHSPLL == pllType) {
        tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 1);
        tmpVal = BL_SET_REG_BITS_VAL(tmpVal, GLB_UHSPLL_EVEN_DIV_EN, cfg->basicCfg->clkpllEvenDivEn);
        tmpVal = BL_SET_REG_BITS_VAL(tmpVal, GLB_UHSPLL_EVEN_DIV_RATIO, cfg->basicCfg->clkpllEvenDivRatio);
        BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 1, tmpVal);
    }

    /* cfg6:Set aupll_sdm_bypass,aupll_sdmin */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 6);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_SDMIN, cfg->clkpllSdmin);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 6, tmpVal);

    /* Step2:config pu */
    /* cfg0 : pu_aupll_sfreg=1 */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_PU_AUPLL_SFREG, 1);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);

    /* delay > 2us */
    arch_delay_us(3);

    /* cfg0 : pu_wifipll=1 */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_PU_AUPLL, 1);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);

    /* delay > 2us */
    arch_delay_us(3);

    /* toggle sdm_reset (pulse 0 > 1us) */
    /* cfg0 : aupll_sdm_reset */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_SDM_RSTB, 1);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);
    arch_delay_us(2);
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_SDM_RSTB, 0);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);
    arch_delay_us(2);
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_SDM_RSTB, 1);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);

    /* Step3:reset pll */
    /* cfg0 : toggle aupll_reset_fbdv, pulse 0 > 1us */
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_FBDV_RSTB, 1);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);
    arch_delay_us(2);
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_FBDV_RSTB, 0);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);
    arch_delay_us(2);
    tmpVal = BL_RD_WORD(REG_PLL_BASE_ADDRESS + 4 * 0);
    tmpVal = BL_SET_REG_BITS_VAL(tmpVal, CCI_AUPLL_FBDV_RSTB, 1);
    BL_WR_WORD(REG_PLL_BASE_ADDRESS + 4 * 0, tmpVal);

    if (waitStable) {
        /* Wait 1.5*30us    */
        arch_delay_us(45);
    }

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  reconfigure UHSPLL clock
 *
 * @param  xtalType: XTAL frequency type
 * @param  pllCfg: PLL configuration
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
BL_Err_Type GLB_Config_UHS_PLL(GLB_XTAL_Type xtalType, const GLB_MU_PLL_Cfg_Type * pllCfgList)
{
    GLB_PLL_REF_CLK_Type refClk;

    if (xtalType == GLB_XTAL_RC32M) {
        refClk = GLB_PLL_REFCLK_RC32M;
    } else {
        refClk = GLB_PLL_REFCLK_XTAL;
    }

    GLB_Power_Off_MU_PLL(GLB_MU_PLL_UHSPLL);
    GLB_MU_PLL_Ref_Clk_Sel(GLB_MU_PLL_UHSPLL, refClk);
    GLB_Power_On_MU_PLL(GLB_MU_PLL_UHSPLL, &(pllCfgList[xtalType]), 1);

    return SUCCESS;
}
