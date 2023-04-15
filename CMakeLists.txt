# обязательная строка, устанавливает требуемую версию CMake
cmake_minimum_required(VERSION 3.15)

# название проекта, версия, описание в свободной форме
project(
  STM32F303_CISCO_LCD
  VERSION 1.0
  DESCRIPTION "STM32F303_CISCO_LCD"
  # языки проекта, если не указать, будет принято, что проект на С и С++
  LANGUAGES C CXX ASM)

# более информативный вывод в процессе сборки проекта
set(CMAKE_VERBOSE_MAKEFILE)

#execute_process(
#  COMMAND git rev-parse HEAD
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#  OUTPUT_VARIABLE GIT_SHA_STRING
#  OUTPUT_STRIP_TRAILING_WHITESPACE)

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/git_sha_string.h.in
#               ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/git_sha_string.h @ONLY)

#if(CMAKE_HOST_WIN32)

#  set(BUILDINFO_COMMAND_STRING "D:/python27/python.exe"
#                               "${CMAKE_CURRENT_SOURCE_DIR}/build-info.py")

#elseif(CMAKE_HOST_UNIX)

#  set(BUILDINFO_COMMAND_STRING "${CMAKE_CURRENT_SOURCE_DIR}/build-info.py")

#else()

  set(BUILDINFO_COMMAND_STRING)

#endif()

# дебаг билд или нет ?
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message("Compiling with debug info")
  set(NDEBUG)
else()
  set(NDEBUG "NDEBUG")
endif()

#if(NOT CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
#  add_custom_target(
#    Build_Info ALL
#    COMMAND ${BUILDINFO_COMMAND_STRING} "${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc"
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#endif()

# строка ниже определяет, нужна ли кросс-компиляция если сборка будет
# происходить для той же системы, что и текущая, 2 переменные равны. Иначе - не
# равны.

if(NOT CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  # PC-Lint function
  if(${INCLUDE_LINT})
    if(EXISTS ${TOOLCHAIN_PREFIX}/pc-lint2.cmake)
      message("linting included")
      include(${TOOLCHAIN_PREFIX}/pc-lint2.cmake)
    endif()
  endif()

  # добавил дефайн, который можно анализировать в тестах, к примеру
  add_definitions(-DMCU_TARGET)

  # просто отладочный вывод
  message(STATUS "CMAKE_TOOLCHAIN_FILE = " ${CMAKE_TOOLCHAIN_FILE})
  message(STATUS "CMAKE_CURRENT_SOURCE_DIR = " ${CMAKE_CURRENT_SOURCE_DIR})

  # assembler flags
  set(CMAKE_ASM_FLAGS
      "${CMAKE_ASM_FLAGS} -Wall -fdata-sections -ffunction-sections")

  set(GCC_COMPILE_FLAGS
      "${GCC_COMPILE_FLAGS} -Wall -fmessage-length=0 -ffunction-sections --specs=nano.specs"
  )

  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${CMAKE_CXX_DEBUG_FLAGS}")
  set(CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} ${GCC_COMPILE_FLAGS} ${CMAKE_CXX_DEBUG_FLAGS}")

  # подключаем cmake файлы, сгенерированные скриптом message(FATAL_ERROR
  # "Comment out this line after keil-converted cmake lists are included!"
  # message(FATAL_ERROR "Закомментируйте эту строку после добавления cmakе
  # файлов, созданных keil2cmake.py!"
  include(${CMAKE_CURRENT_SOURCE_DIR}/303-lcd_sources_list.cmake)

#  include(${CMAKE_CURRENT_SOURCE_DIR}/dumb.cmake)

  # отладочный вывод
  message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
  message(STATUS "CMAKE_C_FLAGS = " ${CMAKE_CXX_FLAGS})

  # добавляем таргет PC-Lint
  if(COMMAND add_pc_lint)
    # establish list of compile defifntiond
    set(comp_defs_list)
    # скорее всего, эти спсики пусты - ничего страшного
    list(APPEND comp_defs_list ${D_MCU})
    list(APPEND comp_defs_list ${D_MCU_EXACT})
    list(APPEND comp_defs_list ${D_HSE_VAL})
        message (STATUS ${TARGET_NAME}_LIST_OF_SOURCES)
    # fantastic loop
    foreach(s IN LISTS ${TARGET_NAME}_LIST_OF_SOURCES)
      message(${s})
      set(t_name)
      string(REPLACE "/" "_" t_name ${s})

      add_pc_lint(
        # compile defines
        "${comp_defs_list}"
        # target
        ${t_name} ${TARGET_NAME} ${s})
    endforeach()
  endif(COMMAND add_pc_lint)

else() # if (NOT CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME )
  message("Non-MCU target detected")

endif() # if (NOT CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME )

# инфа и диагностика
message("*********************")
message("CMAKE_SYSTEM_NAME = " ${CMAKE_SYSTEM_NAME}
        "      CMAKE_HOST_SYSTEM_NAME = " ${CMAKE_HOST_SYSTEM_NAME})
message("CMAKE_SYSTEM = " ${CMAKE_SYSTEM} "      CMAKE_HOST_SYSTEM = "
        ${CMAKE_HOST_SYSTEM})
message("CMAKE_C_COMPILER = " ${CMAKE_C_COMPILER})
message("CMAKE_CXX_COMPILER = " ${CMAKE_C_COMPILER})
message("CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
message("*********************")
