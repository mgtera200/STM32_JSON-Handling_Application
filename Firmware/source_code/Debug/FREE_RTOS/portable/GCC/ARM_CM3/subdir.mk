################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FREE_RTOS/portable/GCC/ARM_CM3/port.c 

OBJS += \
./FREE_RTOS/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./FREE_RTOS/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
FREE_RTOS/portable/GCC/ARM_CM3/port.o: ../FREE_RTOS/portable/GCC/ARM_CM3/port.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/inc" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"FREE_RTOS/portable/GCC/ARM_CM3/port.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

