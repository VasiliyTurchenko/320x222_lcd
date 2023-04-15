/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pins_defs.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_tim1_ch1;
extern TIM_HandleTypeDef htim_pixclock;
extern TIM_HandleTypeDef htim_gating_vsync;
/* USER CODE BEGIN EV */
extern TIM_HandleTypeDef htim_h_sync;

extern const uint_fast16_t n_rows;
extern uint_fast16_t	   line_frame_counter;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

    /* USER CODE END DMA1_Channel2_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_tim1_ch1);
    /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

    /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
    /* USER CODE BEGIN TIM1_CC_IRQn 0 */

    /* USER CODE END TIM1_CC_IRQn 0 */
    HAL_TIM_IRQHandler(&htim_pixclock);
    /* USER CODE BEGIN TIM1_CC_IRQn 1 */

    /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
    /* USER CODE BEGIN TIM2_IRQn 0 */

    if ((__HAL_TIM_GET_FLAG(&htim_gating_vsync, TIM_FLAG_CC2) != RESET) &&
        (__HAL_TIM_GET_IT_SOURCE(&htim_gating_vsync, TIM_IT_CC2) != RESET)) {
        __HAL_TIM_CLEAR_IT(&htim_gating_vsync, TIM_IT_CC2);

        __HAL_TIM_DISABLE(&htim_h_sync);
        htim_h_sync.Instance->CNT = 0U;
        __HAL_TIM_ENABLE(&htim_h_sync);

        if (line_frame_counter == 0U) {
            //htim_gating_vsync.Instance->CCER |= 0x00000100U;
#if (1)
            /* pwm */
            htim_gating_vsync.Instance->CCR3 = 100U;
            const uint32_t pwm_mask			 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; // 0110
            htim_gating_vsync.Instance->CCMR2 |= pwm_mask;
#endif
        } else if (line_frame_counter == 1U) {
            htim_gating_vsync.Instance->CCR3 = 0U;

        } else {
#if (1)
            /* force */
            const uint32_t reset_mask0 = (uint32_t) ~(TIM_CCMR2_OC3M_3 | TIM_CCMR2_OC3M_2 |
                                                      TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0);

            htim_gating_vsync.Instance->CCMR2 &= reset_mask0;
            const uint32_t force_mask = TIM_CCMR2_OC3M_2; // 0100
            htim_gating_vsync.Instance->CCMR2 |= force_mask;
#endif
            //htim_gating_vsync.Instance->CCER &= 0xFFFFFEFFU;
        }

        const uint32_t reset_mask =
            (uint32_t) ~(TIM_CCMR1_OC2M_3 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0);

        if (line_frame_counter == 0U) {
            /* enable DMARQ and pix clock */
            htim_pixclock.Instance->DIER = htim_pixclock.Instance->DIER | TIM_DIER_CC1DE_Msk;
            htim_pixclock.Instance->CCMR1 &= reset_mask;
            const uint32_t pwm_mask = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; // 0110
            htim_pixclock.Instance->CCMR1 |= pwm_mask;
        }

        if (line_frame_counter == n_rows - 2U) {
            /* disable DMARQ and pix clock */
            /* Reset the CCxE Bit */
            htim_pixclock.Instance->DIER =
                htim_pixclock.Instance->DIER & (uint32_t)~TIM_DIER_CC1DE_Msk;

            htim_pixclock.Instance->CCMR1 &= reset_mask;

            const uint32_t force_mask = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_0; // 0101
            htim_pixclock.Instance->CCMR1 |= force_mask;
        }

        line_frame_counter++;
        if (line_frame_counter == n_rows) {
            line_frame_counter = 0U;
        }
#if (1)
        if ((line_frame_counter % 2U) == 0U) {
            HAL_GPIO_TogglePin(M_GPIO_Port, M_PIN);
        }
#endif

    } else {
        /* USER CODE END TIM2_IRQn 0 */
        HAL_TIM_IRQHandler(&htim_gating_vsync);
        /* USER CODE BEGIN TIM2_IRQn 1 */
    }
    /* USER CODE END TIM2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
