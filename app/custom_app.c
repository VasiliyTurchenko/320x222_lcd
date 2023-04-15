/** @file custom_app.c
 *  @brief
 *
 *  @author Vasiliy Turchenko
 *  @bug
 *  @date 01-Apr-2023
 */

#include "custom_app.h"
#include "gpio_lib.h"
#include "main.h"
#include "pins_defs.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
#include "custom_timers.h"

extern const uint8_t screens[];
extern const size_t	 bigsize;

#define N_COLS (320U)
#define N_ROWS (222U)
#define BUFSIZE (N_COLS / CHAR_BIT * N_ROWS)
static const size_t bufsize = BUFSIZE;

const uint_fast16_t		 n_rows = N_ROWS + 2U;
typedef volatile uint8_t screen_t[BUFSIZE];
static volatile screen_t __aligned(sizeof(uint32_t)) screen0;
static volatile screen_t __aligned(sizeof(uint32_t)) screen1;

volatile uint32_t *ps0 = (uint32_t *)screen0;
volatile uint32_t *ps1 = (uint32_t *)screen1;

uint_fast16_t line_frame_counter = 1U;

static const gpio_init_st gpio_initialisators[] = {

	{ .port		   = DISP_OFF_GPIO_Port,
	  .pin		   = DISP_OFF_PIN,
	  .initial	   = GPIO_PIN_RESET,
	  .init_params = {},
#ifndef GPIO_INI_SILENT
	  .descr = "DISP_OFF_GPIO_Port:DISP_OFF_PIN" },
#endif
	GPIO_INIT_DATA(DISP_OFF_GPIO_Port,
				   DISP_OFF_PIN,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLDOWN,
				   GPIO_SPEED_FREQ_LOW,
				   0U),

	GPIO_INIT_DATA(M_GPIO_Port,
				   M_PIN,
				   GPIO_PIN_SET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLDOWN,
				   GPIO_SPEED_FREQ_LOW,
				   0U),

	GPIO_INIT_DATA(DISP0_GPIO_Port,
				   DISP0_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),

	GPIO_INIT_DATA(DISP1_GPIO_Port,
				   DISP1_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),

	GPIO_INIT_DATA(DISP2_GPIO_Port,
				   DISP2_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),

	GPIO_INIT_DATA(DISP3_GPIO_Port,
				   DISP3_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),

	GPIO_INIT_DATA(DISP4_GPIO_Port,
				   DISP4_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),

	GPIO_INIT_DATA(DISP5_GPIO_Port,
				   DISP5_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),

	GPIO_INIT_DATA(DISP6_GPIO_Port,
				   DISP6_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),

	GPIO_INIT_DATA(DISP7_GPIO_Port,
				   DISP7_Pin,
				   GPIO_PIN_RESET,
				   GPIO_MODE_OUTPUT_PP,
				   GPIO_PULLUP,
				   GPIO_SPEED_FREQ_HIGH,
				   0U),
};

/**
 * @brief gpio_init
 */
static void gpio_init(void)
{
	const size_t n_inits = sizeof(gpio_initialisators) / sizeof(gpio_initialisators[0]);

	for (size_t i = 0U; i < n_inits; i++) {
		gpio_init_new(&gpio_initialisators[i]);
	}
}

/**
 * @brief cls
 * @param scr
 */
static void cls(screen_t scr)
{
	for (size_t i = 0U; i < bufsize; i++) {
		scr[i] = 0x00U;
	}
}

/**
 * @brief sets
 * @param scr
 */
static void sets(screen_t scr)
{
	for (size_t i = 0U; i < bufsize; i++) {
		scr[i] = 0xFFU;
	}
}

/**
 * @brief set_pix
 * @param x
 * @param y
 * @param val
 */
static void set_pix(screen_t screen, uint16_t x, uint16_t y, uint32_t val)
{
	const size_t  lin_ofs	  = y * (N_COLS / CHAR_BIT);
	const size_t  byte_offset = x / CHAR_BIT;
	const size_t  bit_offset  = x % CHAR_BIT + 1U;
	const uint8_t byte		  = (uint8_t)(1U << (CHAR_BIT - bit_offset));

	const size_t ofs = lin_ofs + byte_offset;

	if (ofs >= bufsize) {
		Error_Handler();
	}

	if (val > 0U) {
		screen[ofs] |= byte;
	} else {
		screen[ofs] &= (uint8_t)~byte;
	}
}

/**
 * @brief checker_board
 * @param s
 */
static void checker_board(screen_t screen, uint16_t side, uint32_t init_pix)
{
	uint32_t pix;
	uint32_t row = 0U;
	while (row < N_ROWS) {
		pix			 = init_pix;
		uint32_t col = 0U;
		while (col < N_COLS) {
			if ((col % side) == 0U) {
				pix = ~pix;
			}
			set_pix(screen, col, row, pix);
			col++;
		}
		if (((row % side) == 0U) && (row > 0U)) {
			init_pix = ~init_pix;
		}

		row++;
	}
}

/**
 * @brief grid
 * @param s
 */
static void grid(screen_t screen, uint16_t side, uint32_t init_pix)
{
	if (init_pix == 0U) {
		sets(screen);
	} else {
		cls(screen);
	}
	for (size_t y = 0U; y < N_ROWS; y = y + side) {
		for (size_t x = 0U; x < N_COLS; x++) {
			set_pix(screen, x, y, init_pix);
		}
	}
	for (size_t x = 0U; x < N_COLS; x = x + side) {
		for (size_t y = 0U; y < N_ROWS; y++) {
			set_pix(screen, x, y, init_pix);
		}
	}
}

void inc_screen(screen_t screen)
{
	for (size_t i = 0U; i < bufsize; i++) {
		screen[i] = (uint8_t)i;
	}
}

/**
 * @brief tim1_pre_run last-moment preparations
 */
static inline void tim1_pre_run(void)
{
	static const uint32_t PCLK_TO_PIX_PERIOD = 71U;
	htim_pixclock.Instance->CNT				 = PCLK_TO_PIX_PERIOD;
	// Ch1 is DMA RQ
	HAL_DMA_Start(&hdma_tim1_ch1, (uint32_t)screen0, (uint32_t)&GPIOB->ODR, BUFSIZE);
	//    HAL_DMA_Start(&hdma_tim1_ch1, (uint32_t)screens, (uint32_t)&GPIOB->ODR, bigsize);
	htim_pixclock.Instance->DIER = htim_pixclock.Instance->DIER | TIM_DIER_CC1DE_Msk;

	const uint32_t tmp	= 0x0011U;		  // bit 0 and bit 4 are set
	const uint32_t tmpi = (uint32_t)~tmp; // bit 0 and bit 4 are reset

	/* Reset the CCxE Bit */
	htim_pixclock.Instance->CCER &= tmpi;

	/*enable 2 channels */
	htim_pixclock.Instance->CCER |= tmp;
	__HAL_TIM_MOE_ENABLE(&htim_pixclock);
	TIM_CHANNEL_STATE_SET(&htim_pixclock, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
	TIM_CHANNEL_STATE_SET(&htim_pixclock, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
}

/**
 * @brief tim2_pre_run last-moment preparations
 */
static inline void tim2_pre_run(void)
{
	htim_gating_vsync.Instance->CNT = 0; //99U;
	const uint32_t tmp2				= 0x0111U;
	const uint32_t tmpi2			= (uint32_t)~tmp2;
	htim_gating_vsync.Instance->CCER &= tmpi2;
	htim_gating_vsync.Instance->CCER |= tmp2;
	__HAL_TIM_MOE_ENABLE(&htim_gating_vsync);
	TIM_CHANNEL_STATE_SET(&htim_gating_vsync, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
	TIM_CHANNEL_STATE_SET(&htim_gating_vsync, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
	TIM_CHANNEL_STATE_SET(&htim_gating_vsync, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_BUSY);
}

/**
 * @brief tim2_pre_run last-moment preparations
 */
static inline void tim3_pre_run(void)
{
	static const uint32_t PCLK_TO_PIX_PERIOD = 71U;
	htim_h_sync.Instance->CNT				 = PCLK_TO_PIX_PERIOD;
	const uint32_t tmp3						 = 0x0001U;			// bit 0 is set
	const uint32_t tmpi3					 = (uint32_t)~tmp3; // bit 0 is reset
	htim_h_sync.Instance->CCER &= tmpi3;
	htim_h_sync.Instance->CCER |= tmp3;
	__HAL_TIM_MOE_ENABLE(&htim_h_sync);
	TIM_CHANNEL_STATE_SET(&htim_h_sync, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
}

/**
 * @brief run_app
 */
void run_app(void)
{
	static size_t checker = 10U;
	gpio_init();
	HAL_GPIO_WritePin(DISP_OFF_GPIO_Port, DISP_OFF_PIN, GPIO_PIN_SET);
	HAL_Delay(100U);
	all_timers_setup();
	cls(screen0);
	cls(screen1);
	checker_board(screen0, checker, 0U);
	grid(screen0, checker, 1U);

	GPIOB->ODR = 0x55U;
	GPIOB->ODR = 0xAAU;

	tim1_pre_run();
	tim2_pre_run();
	tim3_pre_run();
	__HAL_TIM_ENABLE_IT(&htim_gating_vsync, TIM_IT_CC2);
	__HAL_TIM_ENABLE(&htim_pixclock);
	__HAL_TIM_ENABLE(&htim_gating_vsync);

	uint32_t init_pixel = (uint32_t)~0U;
	uint32_t inv		= 0U;
	while (1 == 1) {
		if (inv == 1U) {
			init_pixel = (uint32_t)~init_pixel;
			inv		   = 0U;
		}
		size_t ch0 = checker++;
		checker	   = (size_t)((float)(checker)*1.05f);
		checker	   = (ch0 > checker) ? ch0 : checker;
		if (checker > N_COLS) {
			inv = 1U;
		}
		checker = checker % N_COLS;
		checker_board(screen0, checker, init_pixel);
		//        grid(screen0, checker, init_pixel);
		HAL_GPIO_TogglePin(GPIOE,
						   (LD4_Pin | LD3_Pin | LD5_Pin | LD7_Pin | LD9_Pin | LD10_Pin | LD8_Pin |
							LD6_Pin));
	}
}
