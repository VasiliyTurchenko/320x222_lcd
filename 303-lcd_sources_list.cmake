
## ------- TARGET STARTS HERE -------
set(TARGET_NAME 303-lcd)
set(EXEC_NAME 303-lcd_STM32F303VCTx)
set(TARGET_NAME ${EXEC_NAME} )
add_executable(${EXEC_NAME})

set(MCU STM32F303VCTx)
target_compile_definitions(${EXEC_NAME} PRIVATE USE_HAL_DRIVER STM32F303xC)

target_include_directories(${EXEC_NAME} PRIVATE app)
target_include_directories(${EXEC_NAME} PRIVATE Core/Inc)
target_include_directories(${EXEC_NAME} PRIVATE Drivers/STM32F3xx_HAL_Driver/Inc/Legacy)
target_include_directories(${EXEC_NAME} PRIVATE Drivers/STM32F3xx_HAL_Driver/Inc)
target_include_directories(${EXEC_NAME} PRIVATE Drivers/CMSIS/Device/ST/STM32F3xx/Include)
target_include_directories(${EXEC_NAME} PRIVATE Drivers/CMSIS/Include)
target_include_directories(${EXEC_NAME} PRIVATE Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/)
target_include_directories(${EXEC_NAME} PRIVATE Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core_A/Include/)

target_include_directories(${EXEC_NAME} PRIVATE .)

set(COMPILE_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16  -mno-thumb-interwork -v)

# The path to the asm startup file(s)
set(STARTUP_CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# A debug output
message("STARTUP_CODE_DIR = " ${STARTUP_CODE_DIR})

# The path to the linker script(s)
set(LINKER_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message("LINKER_SCRIPTS_DIR = " ${LINKER_SCRIPTS_DIR})

set(STARTUP_CODE_SOURCE ${STARTUP_CODE_DIR}/startup_stm32f303xc.s)
set(LDSCRIPT ${STARTUP_CODE_DIR}/STM32F303VCTx_FLASH.ld)
set	(GROUP_SRC_APPLICATION_MDK_ARM
#		startup_stm32f303xc.s
    )

set	(GROUP_SRC_APPLICATION_USER_CORE
        Core/Src/main.c
        Core/Src/main2.c
        Core/Src/gpio.c
        Core/Src/dma.c
#        Core/Src/tim.c
        Core/Src/stm32f3xx_it.c
        Core/Src/stm32f3xx_hal_msp.c
    )

set	(GROUP_SRC_DRIVERS_STM32F3XX_HAL_DRIVER
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
        Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
    )

set	(GROUP_SRC_DRIVERS_CMSIS
        Core/Src/system_stm32f3xx.c
    )

set	(GROUP_APP
        app/custom_app.c
        app/custom_timers.c
        app/screens.c
        app/gpio_lib.c
    )

set	(${EXEC_NAME}_LIST_OF_SOURCES
        ${GROUP_SRC_APPLICATION_MDK_ARM}
        ${GROUP_SRC_APPLICATION_USER_CORE}
        ${GROUP_SRC_DRIVERS_STM32F3XX_HAL_DRIVER}
        ${GROUP_SRC_DRIVERS_CMSIS}
        ${GROUP_APP}
    )

target_sources(${EXEC_NAME} PRIVATE ${${EXEC_NAME}_LIST_OF_SOURCES} ${STARTUP_CODE_SOURCE})

target_compile_options(${EXEC_NAME} PRIVATE ${COMPILE_FLAGS} )

target_link_options(
                ${EXEC_NAME} BEFORE PRIVATE
                "-Wl,-Map=${EXEC_NAME}.map"
                "-Wl,-T${LDSCRIPT}"
                "-Wl,--gc-sections"
                "-Wl,--verbose"
                "-Wl,-V"
                "-Wl,--cref"
                ${COMPILE_FLAGS})


target_link_libraries(
                ${EXEC_NAME}
                c		# c runtime
                m		# math
                nosys	# for non-os
                )

STM32_ADD_HEX_BIN_TARGETS(${EXEC_NAME})
STM32_PRINT_SIZE_OF_TARGETS(${EXEC_NAME})
