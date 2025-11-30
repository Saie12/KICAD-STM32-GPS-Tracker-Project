################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/gps_parser.c \
../Core/Src/gps_uart.c \
../Core/Src/main.c \
../Core/Src/sim808_driver.c \
../Core/Src/sim808_gprs.c \
../Core/Src/sim808_gps.c \
../Core/Src/sim808_mock.c \
../Core/Src/stm32f0xx_hal_msp.c \
../Core/Src/stm32f0xx_it.c \
../Core/Src/system_stm32f0xx.c 

OBJS += \
./Core/Src/gps_parser.o \
./Core/Src/gps_uart.o \
./Core/Src/main.o \
./Core/Src/sim808_driver.o \
./Core/Src/sim808_gprs.o \
./Core/Src/sim808_gps.o \
./Core/Src/sim808_mock.o \
./Core/Src/stm32f0xx_hal_msp.o \
./Core/Src/stm32f0xx_it.o \
./Core/Src/system_stm32f0xx.o 

C_DEPS += \
./Core/Src/gps_parser.d \
./Core/Src/gps_uart.d \
./Core/Src/main.d \
./Core/Src/sim808_driver.d \
./Core/Src/sim808_gprs.d \
./Core/Src/sim808_gps.d \
./Core/Src/sim808_mock.d \
./Core/Src/stm32f0xx_hal_msp.d \
./Core/Src/stm32f0xx_it.d \
./Core/Src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F030C8Tx -c -I../Core/Inc -I"C:/Users/91755/Documents/PCB Design/STM32 GPS Tracker/Firmware/gps_tracker/Drivers/STM32F0xx_HAL_Driver/Inc" -I"C:/Users/91755/Documents/PCB Design/STM32 GPS Tracker/Firmware/gps_tracker/Drivers/CMSIS/Include" -I"C:/Users/91755/Documents/PCB Design/STM32 GPS Tracker/Firmware/gps_tracker/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/gps_parser.cyclo ./Core/Src/gps_parser.d ./Core/Src/gps_parser.o ./Core/Src/gps_parser.su ./Core/Src/gps_uart.cyclo ./Core/Src/gps_uart.d ./Core/Src/gps_uart.o ./Core/Src/gps_uart.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/sim808_driver.cyclo ./Core/Src/sim808_driver.d ./Core/Src/sim808_driver.o ./Core/Src/sim808_driver.su ./Core/Src/sim808_gprs.cyclo ./Core/Src/sim808_gprs.d ./Core/Src/sim808_gprs.o ./Core/Src/sim808_gprs.su ./Core/Src/sim808_gps.cyclo ./Core/Src/sim808_gps.d ./Core/Src/sim808_gps.o ./Core/Src/sim808_gps.su ./Core/Src/sim808_mock.cyclo ./Core/Src/sim808_mock.d ./Core/Src/sim808_mock.o ./Core/Src/sim808_mock.su ./Core/Src/stm32f0xx_hal_msp.cyclo ./Core/Src/stm32f0xx_hal_msp.d ./Core/Src/stm32f0xx_hal_msp.o ./Core/Src/stm32f0xx_hal_msp.su ./Core/Src/stm32f0xx_it.cyclo ./Core/Src/stm32f0xx_it.d ./Core/Src/stm32f0xx_it.o ./Core/Src/stm32f0xx_it.su ./Core/Src/system_stm32f0xx.cyclo ./Core/Src/system_stm32f0xx.d ./Core/Src/system_stm32f0xx.o ./Core/Src/system_stm32f0xx.su

.PHONY: clean-Core-2f-Src

