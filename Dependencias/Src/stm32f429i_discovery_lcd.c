/**
  ******************************************************************************
  * @file    stm32f429i_discovery_lcd.c
  * @author  MCD Application Team
  * @brief   This file includes the LCD driver for ILI9341 Liquid Crystal 
  *          Display Modules of STM32F429I-Discovery kit (MB1075).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
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


/* Includes ------------------------------------------------------------------*/
#include "stm32f429i_discovery_lcd.h"


/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */
    
/** @defgroup STM32F429I_DISCOVERY_LCD STM32F429I DISCOVERY LCD
  * @brief This file includes the LCD driver for (ILI9341) 
  * @{
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_TypesDefinitions STM32F429I DISCOVERY LCD Private TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Defines STM32F429I DISCOVERY LCD Private Defines
  * @{
  */
#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Macros STM32F429I DISCOVERY LCD Private Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))
/**
  * @}
  */ 
  
/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Variables STM32F429I DISCOVERY LCD Private Variables
  * @{
  */ 
LTDC_HandleTypeDef  LtdcHandler;
static DMA2D_HandleTypeDef Dma2dHandler;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

/* Default LCD configuration with LCD Layer 1 */
static uint32_t ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];
LCD_DrvTypeDef  *LcdDrv;
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_FunctionPrototypes STM32F429I DISCOVERY LCD Private FunctionPrototypes
  * @{
  */ 

static void FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);

/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Functions STM32F429I DISCOVERY LCD Private Functions
  * @{
  */ 

/**
  * @brief  Initializes the LCD.
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void)
{ 
  /* On STM32F429I-DISCO, it is not possible to read ILI9341 ID because */
  /* PIN EXTC is not connected to VDD and then LCD_READ_ID4 is not accessible. */
  /* In this case, ReadID function is bypassed.*/  
  /*if(ili9341_drv.ReadID() == ILI9341_ID)*/

    /* LTDC Configuration ----------------------------------------------------*/
    LtdcHandler.Instance = LTDC;
    
    /* Timing configuration  (Typical configuration from ILI9341 datasheet)
          HSYNC=10 (9+1)
          HBP=20 (29-10+1)
          ActiveW=240 (269-20-10+1)
          HFP=10 (279-240-20-10+1)
    
          VSYNC=2 (1+1)
          VBP=2 (3-2+1)
          ActiveH=320 (323-2-2+1)
          VFP=4 (327-320-2-2+1)
      */
    
    /* Configure horizontal synchronization width */
    LtdcHandler.Init.HorizontalSync = ILI9341_HSYNC;
    /* Configure vertical synchronization height */
    LtdcHandler.Init.VerticalSync = ILI9341_VSYNC;
    /* Configure accumulated horizontal back porch */
    LtdcHandler.Init.AccumulatedHBP = ILI9341_HBP;
    /* Configure accumulated vertical back porch */
    LtdcHandler.Init.AccumulatedVBP = ILI9341_VBP;
    /* Configure accumulated active width */
    LtdcHandler.Init.AccumulatedActiveW = 269;
    /* Configure accumulated active height */
    LtdcHandler.Init.AccumulatedActiveH = 323;
    /* Configure total width */
    LtdcHandler.Init.TotalWidth = 279;
    /* Configure total height */
    LtdcHandler.Init.TotalHeigh = 327;
    
    /* Configure R,G,B component values for LCD background color */
    LtdcHandler.Init.Backcolor.Red= 0;
    LtdcHandler.Init.Backcolor.Blue= 0;
    LtdcHandler.Init.Backcolor.Green= 0;
    
    /* LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_8 = 48/4 = 6Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct); 
    
    /* Polarity */
    LtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    LtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    LtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    LtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    
    BSP_LCD_MspInit();
    HAL_LTDC_Init(&LtdcHandler); 
    
    /* Select the device */
    LcdDrv = &ili9341_drv;

    /* LCD Init */	 
    LcdDrv->Init();

    /* Initialize the SDRAM */
    BSP_SDRAM_Init();

  return LCD_OK;
}  

/**
  * @brief  Gets the LCD X size.  
  * @retval The used LCD X size
  */
uint32_t BSP_LCD_GetXSize(void)
{
  return LcdDrv->GetLcdPixelWidth();
}

/**
  * @brief  Gets the LCD Y size.  
  * @retval The used LCD Y size
  */
uint32_t BSP_LCD_GetYSize(void)
{
  return LcdDrv->GetLcdPixelHeight();
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: the layer foreground or background. 
  * @param  FB_Address: the layer frame buffer.
  */
void BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{     
  LCD_LayerCfgTypeDef   Layercfg;

 /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = BSP_LCD_GetXSize();
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = BSP_LCD_GetYSize(); 
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = FB_Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = BSP_LCD_GetXSize();
  Layercfg.ImageHeight = BSP_LCD_GetYSize();
  
  HAL_LTDC_ConfigLayer(&LtdcHandler, &Layercfg, LayerIndex); 

  DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;

  DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK; 

  /* Dithering activation */
  HAL_LTDC_EnableDither(&LtdcHandler);
}

/**
  * @brief  Selects the LCD Layer.
  * @param  LayerIndex: the Layer foreground or background.
  */
void BSP_LCD_SelectLayer(uint32_t LayerIndex)
{
  ActiveLayer = LayerIndex;
}

/**
  * @brief  Sets a LCD Layer visible.
  * @param  LayerIndex: the visible Layer.
  * @param  state: new state of the specified layer.
  *    This parameter can be: ENABLE or DISABLE.  
  */
void BSP_LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState state)
{
  if(state == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&LtdcHandler, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&LtdcHandler, LayerIndex);
  }
  __HAL_LTDC_RELOAD_CONFIG(&LtdcHandler);
}

/**
  * @brief  Sets an LCD Layer visible without reloading.
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE 
  * @retval None
  */
void BSP_LCD_SetLayerVisible_NoReload(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&LtdcHandler, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&LtdcHandler, LayerIndex);
  }
  /* Do not Sets the Reload  */
}

/**
  * @brief  Configures the Transparency.
  * @param  LayerIndex: the Layer foreground or background.
  * @param  Transparency: the Transparency, 
  *    This parameter must range from 0x00 to 0xFF.
  */
void BSP_LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{     
  HAL_LTDC_SetAlpha(&LtdcHandler, Transparency, LayerIndex);
}

/**
  * @brief  Configures the transparency without reloading.
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF 
  * @retval None
  */
void BSP_LCD_SetTransparency_NoReload(uint32_t LayerIndex, uint8_t Transparency)
{    
  HAL_LTDC_SetAlpha_NoReload(&LtdcHandler, Transparency, LayerIndex);
}

/**
  * @brief  Sets a LCD layer frame buffer address.
  * @param  LayerIndex: specifies the Layer foreground or background
  * @param  Address: new LCD frame buffer value      
  */
void BSP_LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{     
  HAL_LTDC_SetAddress(&LtdcHandler, Address, LayerIndex);
}

/**
  * @brief  Sets an LCD layer frame buffer address without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value      
  * @retval None
  */
void BSP_LCD_SetLayerAddress_NoReload(uint32_t LayerIndex, uint32_t Address)
{
  HAL_LTDC_SetAddress_NoReload(&LtdcHandler, Address, LayerIndex);
}

/**
  * @brief  Sets the Display window.
  * @param  LayerIndex: layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height  
  */
void BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* reconfigure the layer size */
  HAL_LTDC_SetWindowSize(&LtdcHandler, Width, Height, LayerIndex);
  
  /* reconfigure the layer position */
  HAL_LTDC_SetWindowPosition(&LtdcHandler, Xpos, Ypos, LayerIndex);
}

/**
  * @brief  Sets display window without reloading.
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height  
  * @retval None
  */
void BSP_LCD_SetLayerWindow_NoReload(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Reconfigure the layer size */
  HAL_LTDC_SetWindowSize_NoReload(&LtdcHandler, Width, Height, LayerIndex);
  
  /* Reconfigure the layer position */
  HAL_LTDC_SetWindowPosition_NoReload(&LtdcHandler, Xpos, Ypos, LayerIndex); 
}

/**
  * @brief  Configures and sets the color Keying.
  * @param  LayerIndex: the Layer foreground or background
  * @param  RGBValue: the Color reference
  */
void BSP_LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue)
{  
  /* Configure and Enable the color Keying for LCD Layer */
  HAL_LTDC_ConfigColorKeying(&LtdcHandler, RGBValue, LayerIndex);
  HAL_LTDC_EnableColorKeying(&LtdcHandler, LayerIndex);
}

/**
  * @brief  Configures and sets the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
void BSP_LCD_SetColorKeying_NoReload(uint32_t LayerIndex, uint32_t RGBValue)
{  
  /* Configure and Enable the color Keying for LCD Layer */
  HAL_LTDC_ConfigColorKeying_NoReload(&LtdcHandler, RGBValue, LayerIndex);
  HAL_LTDC_EnableColorKeying_NoReload(&LtdcHandler, LayerIndex);
}

/**
  * @brief  Disables the color Keying.
  * @param  LayerIndex: the Layer foreground or background
  */
void BSP_LCD_ResetColorKeying(uint32_t LayerIndex)
{
  /* Disable the color Keying for LCD Layer */
  HAL_LTDC_DisableColorKeying(&LtdcHandler, LayerIndex);
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void BSP_LCD_ResetColorKeying_NoReload(uint32_t LayerIndex)
{   
  /* Disable the color Keying for LCD Layer */
  HAL_LTDC_DisableColorKeying_NoReload(&LtdcHandler, LayerIndex);
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  ReloadType: can be one of the following values
  *         - LCD_RELOAD_IMMEDIATE
  *         - LCD_RELOAD_VERTICAL_BLANKING
  * @retval None
  */
void BSP_LCD_Relaod(uint32_t ReloadType)
{
  HAL_LTDC_Relaod (&LtdcHandler, ReloadType);
}


/**
  * @brief  Gets the LCD Background color. 
  * @retval Background color  
  */
uint32_t BSP_LCD_GetBackColor(void)
{
  return DrawProp[ActiveLayer].BackColor;
}


/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background
  */
void BSP_LCD_Clear(uint32_t Color) {
  /* Clear the LCD */ 
  FillBuffer(ActiveLayer, (uint32_t *)(LtdcHandler.LayerCfg[ActiveLayer].FBStartAdress), BSP_LCD_GetXSize(), BSP_LCD_GetYSize(), 0, Color);
}



/*******************************************************************************
                       LTDC and DMA2D BSP Routines
*******************************************************************************/

/**
  * @brief  Initializes the LTDC MSP.
  */
__weak void BSP_LCD_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable the LTDC and DMA2D Clock */
  __HAL_RCC_LTDC_CLK_ENABLE();
  __HAL_RCC_DMA2D_CLK_ENABLE(); 
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* GPIOs Configuration */
  /*
   +------------------------+-----------------------+----------------------------+
   +                       LCD pins assignment                                   +
   +------------------------+-----------------------+----------------------------+
   |  LCD_TFT R2 <-> PC.10  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
   |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
   |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
   |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
   |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
   |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
   -------------------------------------------------------------------------------
            |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
            |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
             -----------------------------------------------------
  */

  /* GPIOA configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 |
                           GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Alternate= GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* GPIOB configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_8 | \
                           GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

 /* GPIOC configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

 /* GPIOD configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  
 /* GPIOF configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);     

 /* GPIOG configuration */  
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | \
                           GPIO_PIN_11;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
 
  /* GPIOB configuration */  
  GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStructure.Alternate= GPIO_AF9_LTDC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* GPIOG configuration */  
  GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Fills buffer.
  * @param  LayerIndex: layer index
  * @param  pDst: output color
  * @param  xSize: buffer width
  * @param  ySize: buffer height
  * @param  OffLine: offset
  * @param  ColorIndex: color Index  
  */
static void FillBuffer(uint32_t LayerIndex, void * pDst, uint32_t xSize, uint32_t ySize,
		uint32_t OffLine, uint32_t ColorIndex)
{
  
  /* Register to memory mode with ARGB8888 as color Mode */ 
  Dma2dHandler.Init.Mode         = DMA2D_R2M;
  Dma2dHandler.Init.ColorMode    = DMA2D_ARGB8888;
  Dma2dHandler.Init.OutputOffset = OffLine;      
  
  Dma2dHandler.Instance = DMA2D; 
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&Dma2dHandler) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2dHandler, LayerIndex) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&Dma2dHandler, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&Dma2dHandler, 10);
      }
    }
  } 
}



/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
