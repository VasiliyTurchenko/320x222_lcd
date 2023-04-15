/** @file pins_defs.h
 *  @brief
 *
 *  @author Vasiliy Turchenko
 *  @bug
 *  @date 15-Apr-2023
 */

#ifndef PINS_DEFS_H
#define PINS_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif


/* only definitions of the pins used */

/* parallel data bus */
#define DISP0_Pin GPIO_PIN_0
#define DISP0_GPIO_Port GPIOB
#define DISP1_Pin GPIO_PIN_1
#define DISP1_GPIO_Port GPIOB
#define DISP2_Pin GPIO_PIN_2
#define DISP2_GPIO_Port GPIOB
#define DISP3_Pin GPIO_PIN_3
#define DISP3_GPIO_Port GPIOB
#define DISP4_Pin GPIO_PIN_4
#define DISP4_GPIO_Port GPIOB
#define DISP5_Pin GPIO_PIN_5
#define DISP5_GPIO_Port GPIOB
#define DISP6_Pin GPIO_PIN_6
#define DISP6_GPIO_Port GPIOB
#define DISP7_Pin GPIO_PIN_7
#define DISP7_GPIO_Port GPIOB

/* pixel clock */
#define SHIFT_CLK_Pin GPIO_PIN_8
#define SHIFT_CLK_GPIO_Port GPIOB

/* display off */
#define DISP_OFF_PIN GPIO_PIN_5
#define DISP_OFF_GPIO_Port GPIOA

/* lcd driver */
#define M_PIN GPIO_PIN_4
#define M_GPIO_Port GPIOA


#ifdef __cplusplus
}
#endif

#endif // PINS_DEFS_H
