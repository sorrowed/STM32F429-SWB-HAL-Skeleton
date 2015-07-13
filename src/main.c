#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"

static void RCC_Init( void );

int main( void )
{
	RCC_Init();
	HAL_Init();

	BSP_LCD_Init();
	BSP_LCD_DisplayOff();
	BSP_LCD_LayerDefaultInit( 0, LCD_FRAME_BUFFER );
	BSP_LCD_SetLayerVisible( 0, ENABLE );
	BSP_LCD_SelectLayer( 0 );
	BSP_LCD_DisplayOn();

	BSP_LCD_SetBackColor( LCD_COLOR_BLUE );
	BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
	BSP_LCD_Clear( LCD_COLOR_BLUE );

	BSP_LCD_SetFont( &Font16 );
	BSP_LCD_DisplayStringAtLine( 1, (uint8_t*) "Test" );

	for( ;; )
		;
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 *         The LTDC Clock is configured as follow :
 *            PLLSAIN                        = 192
 *            PLLSAIR                        = 4
 *            PLLSAIDivR                     = 8
 * @param  None
 * @retval None
 *
 * COPYRIGHT(c) 2014 STMicroelectronics
 */
void RCC_Init( void )
{
	__PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );

	RCC_OscInitTypeDef OscCfg;
	OscCfg.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OscCfg.HSEState = RCC_HSE_ON;
	OscCfg.PLL.PLLState = RCC_PLL_ON;
	OscCfg.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	OscCfg.PLL.PLLM = 8;
	OscCfg.PLL.PLLN = 360;
	OscCfg.PLL.PLLP = RCC_PLLP_DIV2;
	OscCfg.PLL.PLLQ = 7;
	HAL_RCC_OscConfig( &OscCfg );

	HAL_PWREx_ActivateOverDrive();

	RCC_ClkInitTypeDef ClkCfg;
	ClkCfg.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	ClkCfg.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	ClkCfg.AHBCLKDivider = RCC_SYSCLK_DIV1;
	ClkCfg.APB1CLKDivider = RCC_HCLK_DIV4;
	ClkCfg.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig( &ClkCfg, FLASH_LATENCY_5 );

	/*##-2- LTDC Clock Configuration ###########################################*/
	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDIVR_8 = 48/8 = 6 Mhz */
	RCC_PeriphCLKInitTypeDef PeriphClkCfg;
	PeriphClkCfg.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkCfg.PLLSAI.PLLSAIN = 192;
	PeriphClkCfg.PLLSAI.PLLSAIR = 4;
	PeriphClkCfg.PLLSAIDivR = RCC_PLLSAIDIVR_8;
	HAL_RCCEx_PeriphCLKConfig( &PeriphClkCfg );
}
