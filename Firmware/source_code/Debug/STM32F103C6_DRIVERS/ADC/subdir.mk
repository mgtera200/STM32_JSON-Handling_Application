################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F103C6_DRIVERS/ADC/ADC.c \
../STM32F103C6_DRIVERS/ADC/help_func.c 

OBJS += \
./STM32F103C6_DRIVERS/ADC/ADC.o \
./STM32F103C6_DRIVERS/ADC/help_func.o 

C_DEPS += \
./STM32F103C6_DRIVERS/ADC/ADC.d \
./STM32F103C6_DRIVERS/ADC/help_func.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F103C6_DRIVERS/ADC/ADC.o: ../STM32F103C6_DRIVERS/ADC/ADC.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/inc" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6_DRIVERS/ADC/ADC.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6_DRIVERS/ADC/help_func.o: ../STM32F103C6_DRIVERS/ADC/help_func.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/inc" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6_DRIVERS/ADC/help_func.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

