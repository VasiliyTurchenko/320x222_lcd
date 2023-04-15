/** @file gpio_lib.h
 *  @brief
 *
 *  @author Vasiliy Turchenko
 *  @bug
 *  @date 15-Apr-2023
 */

#ifndef GPIO_LIB_H
#define GPIO_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#include "stm32f3xx_hal.h"

typedef struct {
    GPIO_TypeDef	 *port;
    uint16_t		 pin;
    GPIO_PinState	 initial;
    GPIO_InitTypeDef init_params;
#ifndef GPIO_INI_SILENT
    const char *descr;
#endif
} gpio_init_st;

#if !defined(GPIO_INI_SILENT)
#define GPIO_INIT_DATA(PORT, PIN, IS, MODE, PUPD, SPEED, ALT)                                      \
    {                                                                                              \
        .port = (PORT), .pin = (PIN), .initial = (IS), .init_params.Pin = (uint32_t)(PIN),         \
        .init_params.Mode = (uint32_t)(MODE), .init_params.Pull = (uint32_t)(PUPD),                \
        .init_params.Speed = (uint32_t)(SPEED), .init_params.Alternate = (uint32_t)(ALT),          \
        .descr = #PORT ":" #PIN,                                                                   \
    }
#else
#define GPIO_INIT_DATA(PORT, PIN, IS, MODE, PUPD, SPEED, ALT)                                      \
    {                                                                                              \
        .port = (PORT), .pin = (PIN), .initial = (IS), .init_params.Pin = (uint32_t)(PIN),         \
        .init_params.Mode = (uint32_t)(MODE), .init_params.Pull = (uint32_t)(PUPD),                \
        .init_params.Speed = (uint32_t)(SPEED), .init_params.Alternate = (uint32_t)(ALT),          \
    }
#endif

void gpio_init_new(const gpio_init_st* p);


#ifdef __cplusplus
}
#endif

#endif // GPIO_LIB_H
