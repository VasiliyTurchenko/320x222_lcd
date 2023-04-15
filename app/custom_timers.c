/** @file custom_timers.c
 *  @brief
 *
 *  @author Vasiliy Turchenko
 *  @bug
 *  @date 03-Apr-2023
 */

#include "custom_timers.h"

TIM_HandleTypeDef htim_pixclock;
TIM_HandleTypeDef htim_gating_vsync;
TIM_HandleTypeDef htim_h_sync;
DMA_HandleTypeDef hdma_tim1_ch1;

static void clock_timer_msp_init(void);
static void gating_timer_msp_init(void);
static void sync_timer_msp_init(void);

/* for TIM1 */
static const uint32_t PCLK_TO_PIX_PERIOD = 71U;
static const uint32_t DMARQ_CH			 = TIM_CHANNEL_1;
static const uint32_t DMARQ_PULSE		 = 8U;
static const uint32_t PIX_CLK_CH		 = TIM_CHANNEL_2;
static const uint32_t PIX_CLK_PULSE		 = 48U;
static const uint32_t NUM_OF_PIX_CLK	 = 99U;
static const uint32_t FRAME_PULSE_CH	 = TIM_CHANNEL_1;

/**
 * @brief clock_timer_setup - TIM1
 */
void clock_timer_setup()
{
    TIM_ClockConfigTypeDef		   sClockSourceConfig	= { 0 };
    TIM_SlaveConfigTypeDef		   sSlaveConfig			= { 0 };
    TIM_MasterConfigTypeDef		   sMasterConfig		= { 0 };
    TIM_OC_InitTypeDef			   sConfigOC			= { 0 };
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

    htim_pixclock.Instance				 = TIM1;
    htim_pixclock.Init.Prescaler		 = 0;
    htim_pixclock.Init.CounterMode		 = TIM_COUNTERMODE_UP;
    htim_pixclock.Init.Period			 = PCLK_TO_PIX_PERIOD;
    htim_pixclock.Init.ClockDivision	 = TIM_CLOCKDIVISION_DIV1;
    htim_pixclock.Init.RepetitionCounter = 0;
    htim_pixclock.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim_pixclock) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim_pixclock, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim_pixclock) != HAL_OK) {
        Error_Handler();
    }
#if (1)
    sSlaveConfig.SlaveMode	  = TIM_SLAVEMODE_GATED;
    sSlaveConfig.InputTrigger = TIM_TS_ITR1;
    if (HAL_TIM_SlaveConfigSynchro(&htim_pixclock, &sSlaveConfig) != HAL_OK) {
        Error_Handler();
    }
#endif
    sMasterConfig.MasterOutputTrigger  = TIM_TRGO_OC1REF;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode	   = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim_pixclock, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode	   = TIM_OCMODE_PWM1;
    sConfigOC.Pulse		   = DMARQ_PULSE * 2U;
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim_pixclock, &sConfigOC, DMARQ_CH) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.Pulse	 = 40U; //PIX_CLK_PULSE;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_LOW;
    if (HAL_TIM_PWM_ConfigChannel(&htim_pixclock, &sConfigOC, PIX_CLK_CH) != HAL_OK) {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel		  = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime		  = 0;
    sBreakDeadTimeConfig.BreakState		  = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity	  = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter	  = 0;
    sBreakDeadTimeConfig.Break2State	  = TIM_BREAK2_DISABLE;
    sBreakDeadTimeConfig.Break2Polarity	  = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter	  = 0;
    sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim_pixclock, &sBreakDeadTimeConfig) != HAL_OK) {
        Error_Handler();
    }

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /** TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    **/
    GPIO_InitStruct.Pin		  = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull	  = GPIO_NOPULL;
    GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief clock_timer_msp_init
 */
static void clock_timer_msp_init(void)
{
    __HAL_RCC_TIM1_CLK_ENABLE();
    hdma_tim1_ch1.Instance				   = DMA1_Channel2;
    hdma_tim1_ch1.Init.Direction		   = DMA_MEMORY_TO_PERIPH;
    hdma_tim1_ch1.Init.PeriphInc		   = DMA_PINC_DISABLE;
    hdma_tim1_ch1.Init.MemInc			   = DMA_MINC_ENABLE;
    hdma_tim1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tim1_ch1.Init.MemDataAlignment	   = DMA_MDATAALIGN_BYTE;
    hdma_tim1_ch1.Init.Mode				   = DMA_CIRCULAR;
    hdma_tim1_ch1.Init.Priority			   = DMA_PRIORITY_VERY_HIGH;
    if (HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK) {
        Error_Handler();
    }

    __HAL_LINKDMA(&htim_pixclock, hdma[TIM_DMA_ID_CC1], hdma_tim1_ch1);

    /* TIM1 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

static void gating_timer_msp_init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

static void sync_timer_msp_init(void)
{
    /** TIM3 GPIO Configuration
    PA7     ------> TIM3_CH2
    **/
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin		  = GPIO_PIN_7;
    GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull	  = GPIO_NOPULL;
    GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief gating_timer_setup gating pix clock and vsync
 */
void gating_timer_setup()
{
    TIM_ClockConfigTypeDef	sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig	   = { 0 };
    TIM_OC_InitTypeDef		sConfigOC		   = { 0 };

    htim_gating_vsync.Instance				 = TIM2;
    htim_gating_vsync.Init.Prescaler		 = PCLK_TO_PIX_PERIOD;
    htim_gating_vsync.Init.CounterMode		 = TIM_COUNTERMODE_UP;
    htim_gating_vsync.Init.Period			 = NUM_OF_PIX_CLK;
    htim_gating_vsync.Init.ClockDivision	 = TIM_CLOCKDIVISION_DIV1;
    htim_gating_vsync.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim_gating_vsync) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim_gating_vsync, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim_gating_vsync) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1REF;
    sMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim_gating_vsync, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode	 = TIM_OCMODE_PWM1;
    sConfigOC.Pulse		 = 40;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    /* gating output, no GPIO assigned  */
    if (HAL_TIM_PWM_ConfigChannel(&htim_gating_vsync, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM2;
    sConfigOC.Pulse	 = 45;
    /* Tim2 Ch2 -> PA1 */
    if (HAL_TIM_PWM_ConfigChannel(&htim_gating_vsync, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode	 = TIM_OCMODE_PWM1;
    sConfigOC.Pulse		 = NUM_OF_PIX_CLK + 1U;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    /* Tim2 Ch3 -> PA2, used as frame pulse output */
    if (HAL_TIM_PWM_ConfigChannel(&htim_gating_vsync, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
        Error_Handler();
    }

    /** TIM2 GPIO Configuration
    PA1     ------> TIM2_CH2
    PA2     ------> TIM2_CH3
    **/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Pin				 = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode			 = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull			 = GPIO_NOPULL;
    GPIO_InitStruct.Speed			 = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate		 = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief h_sync_timer_setup
 */
void h_sync_timer_setup_new()
{
    TIM_ClockConfigTypeDef	sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig	   = { 0 };
    TIM_OC_InitTypeDef		sConfigOC		   = { 0 };

    htim_h_sync.Instance			   = TIM3;
    htim_h_sync.Init.Prescaler		   = PCLK_TO_PIX_PERIOD;
    htim_h_sync.Init.CounterMode	   = TIM_COUNTERMODE_UP;
    htim_h_sync.Init.Period			   = NUM_OF_PIX_CLK;
    htim_h_sync.Init.ClockDivision	   = TIM_CLOCKDIVISION_DIV1;
    htim_h_sync.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim_h_sync) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim_h_sync, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim_h_sync) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim_h_sync, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode	 = TIM_OCMODE_PWM1;
    sConfigOC.Pulse		 = 3;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    /* Tim3 Ch0 -> PA6, used as line pulse output */
    if (HAL_TIM_PWM_ConfigChannel(&htim_h_sync, &sConfigOC, FRAME_PULSE_CH) != HAL_OK) {
        Error_Handler();
    }

    /** TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    **/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Pin				 = GPIO_PIN_6;
    GPIO_InitStruct.Mode			 = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull			 = GPIO_PULLUP;
    GPIO_InitStruct.Speed			 = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate		 = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief all_timers_setup
 */
void all_timers_setup()
{
    clock_timer_setup();
    gating_timer_setup();
    h_sync_timer_setup_new();
}

/**
 * @brief HAL_TIM_Base_MspInit
 * @param tim_baseHandle
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM1) {
        clock_timer_msp_init();
    } else if (tim_baseHandle->Instance == TIM2) {
        gating_timer_msp_init();
    } else if (tim_baseHandle->Instance == TIM3) {
        sync_timer_msp_init();
    }
}
