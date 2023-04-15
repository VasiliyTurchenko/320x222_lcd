/** @file gpio_lib.c
 *  @brief
 *
 *  @author Vasiliy Turchenko
 *  @bug
 *  @date 15-Apr-2023
 */

#include "gpio_lib.h"

extern void Error_Handler(void);

/**
 * @brief gpio_init_new
 * @param p
 */
void gpio_init_new(const gpio_init_st* p)
{
    const GPIO_TypeDef* pp = p->port;
    if (GPIOA == pp) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (GPIOB == pp) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (GPIOC == pp) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (GPIOD == pp) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (GPIOE == pp) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (GPIOF == pp) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else {
        /* error */
        Error_Handler();
    }
    HAL_GPIO_WritePin(p->port, p->pin, p->initial);
    HAL_GPIO_Init(p->port, &p->init_params);
#if(0)
    printf("%s\n", p->descr);
#endif
}
