/** @file custom_timers.h
 *  @brief
 *
 *  @author Vasiliy Turchenko
 *  @bug
 *  @date 03-Apr-2023
 */

#ifndef CUSTOM_TIMERS_H
#define CUSTOM_TIMERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#include "main.h"

extern TIM_HandleTypeDef htim_pixclock;
extern TIM_HandleTypeDef htim_gating_vsync;
extern TIM_HandleTypeDef htim_h_sync;
extern DMA_HandleTypeDef hdma_tim1_ch1;

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle);

void clock_timer_setup();
void gating_timer_setup();
void h_sync_timer_setup_new();

void all_timers_setup();

#ifdef __cplusplus
}
#endif

#endif // CUSTOM_TIMERS_H
