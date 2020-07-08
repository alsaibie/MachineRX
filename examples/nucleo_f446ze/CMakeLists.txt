SET(CMAKE_SYSTEM_NAME Generic) #STM32?
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.5)

SET(LOCAL_ARM_TOOLCHAIN $ENV{HOME}/arm_toolchain/bin/)

SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)

SET(CMAKE_C_COMPILER ${LOCAL_ARM_TOOLCHAIN}arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${LOCAL_ARM_TOOLCHAIN}arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${LOCAL_ARM_TOOLCHAIN}arm-none-eabi-gcc)
set(CMAKE_CXX_GDB ${LOCAL_ARM_TOOLCHAIN}gdb)
set(CMAKE_AR ${LOCAL_ARM_TOOLCHAIN}arm-none-eabi-ar)
set(CMAKE_OBJCOPY ${LOCAL_ARM_TOOLCHAIN}arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP ${LOCAL_ARM_TOOLCHAIN}arm-none-eabi-objdump)
set(SIZE ${LOCAL_ARM_TOOLCHAIN}arm-none-eabi-size)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F446ZETx_FLASH.ld)

SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING)

SET(COMMON_FLAGS "-mcpu=cortex-m4 ${FPU_FLAGS} -mthumb -ffunction-sections -fdata-sections \
-g -fno-common -fmessage-length=0 ")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++17")

SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=c11")


# SET(CMAKE_EXE_LINKER_FLAGS_INIT "-MMD -MP -Wl,-gc-sections,--undefined=uxTopUsedPriority -T${LINKER_SCRIPT}")
SET(CMAKE_EXE_LINKER_FLAGS "-MMD -MP -Wl,-gc-sections,--undefined=uxTopUsedPriority -T${LINKER_SCRIPT}")

SET(ASM_OPTIONS "-x assembler-with-cpp")
PROJECT(nucleo_f446ze C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} ${ASM_OPTIONS}")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(PARENT_DIR ${PROJECT_SOURCE_DIR}/../..)

set(TOPICS_DIR ${PARENT_DIR}/examples/topics)
set(MRX_DIR ${PARENT_DIR}/machinerx/include)
set(APPS_DIR ${PARENT_DIR}/examples/apps)

add_definitions(-DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -D_TIMEVAL_DEFINED)

# wildcard includes
file(GLOB_RECURSE PROJECT_SOURCES "Src/*.*" "${APPS_DIR}/*.*")

# specific includes
SET(ASM_SOURCES startup_stm32f446xx.s)

SET(PROJECT_SOURCES ${PROJECT_SOURCES} ${ASM_SOURCES}
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c"
"Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c"
"Middlewares/Third_Party/FreeRTOS/Source/croutine.c"
"Middlewares/Third_Party/FreeRTOS/Source/event_groups.c"
"Middlewares/Third_Party/FreeRTOS/Source/list.c"
"Middlewares/Third_Party/FreeRTOS/Source/queue.c"
"Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c"
"Middlewares/Third_Party/FreeRTOS/Source/tasks.c"
"Middlewares/Third_Party/FreeRTOS/Source/timers.c"
"Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"
"Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c"
"lib/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_utils.c"
"lib/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_clock.c"
"lib/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_sched.c"
"lib/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_unistd.c"
"lib/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread.c"
"lib/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_mutex.c"
# "Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c"
)

# Example Exclude a single or multiple files 
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c")
# list(FILTER SOURCES EXCLUDE REGEX "Drivers/STM32F4xx_HAL_Driver/Src/*rtc*.c$")

include_directories(
    Inc
    Drivers/CMSIS/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    lib/include
    lib/include/private
    lib/include/FreeRTOS_POSIX
    lib/FreeRTOS-Plus-POSIX/include
    lib/FreeRTOS-Plus-POSIX/include/portable
    lib/FreeRTOS-Plus-POSIX/include/portable/empty_portable
    # lib/FreeRTOS-Plus-POSIX/source
    ${APPS_DIR}
    ${TOPICS_DIR}
    ${MRX_DIR}
    )



add_executable(${PROJECT_NAME}.elf ${PROJECT_SOURCES} ${LINKER_SCRIPT})
# TODO: cleanup the flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc -lm -lnosys -specs=nano.specs -specs=nosys.specs -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map -lstdc++")

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}"
"Building ${BIN_FILE}")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${SIZE} --format=berkeley --radix=10 ${PROJECT_NAME}.elf)

set(openocd_target_cfg "target/stm32f4x.cfg")
set(openocd_interface_cfg "interface/stlink-v2-1.cfg")

# Flash 
add_custom_target(flash
COMMAND openocd -f ${openocd_interface_cfg} -f ${openocd_target_cfg} -c init -c 'reset halt' 
-c "flash write_image erase ${PROJECT_NAME}.bin 0x08000000" -c "reset" -c "exit"
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
DEPENDS ${PROJECT_NAME}.elf
)