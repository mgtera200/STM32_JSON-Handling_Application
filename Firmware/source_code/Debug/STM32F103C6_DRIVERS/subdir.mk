################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F103C6_DRIVERS/EXTI_DRIVER.c \
../STM32F103C6_DRIVERS/STM32F103C6_USART_DRIVERS.c \
../STM32F103C6_DRIVERS/STM32_F103C6_GPIO_DRIVERS.c \
../STM32F103C6_DRIVERS/STM32_F103C6_RCC_DRIVERS.c \
../STM32F103C6_DRIVERS/Timer.c 

OBJS += \
./STM32F103C6_DRIVERS/EXTI_DRIVER.o \
./STM32F103C6_DRIVERS/STM32F103C6_USART_DRIVERS.o \
./STM32F103C6_DRIVERS/STM32_F103C6_GPIO_DRIVERS.o \
./STM32F103C6_DRIVERS/STM32_F103C6_RCC_DRIVERS.o \
./STM32F103C6_DRIVERS/Timer.o 

C_DEPS += \
./STM32F103C6_DRIVERS/EXTI_DRIVER.d \
./STM32F103C6_DRIVERS/STM32F103C6_USART_DRIVERS.d \
./STM32F103C6_DRIVERS/STM32_F103C6_GPIO_DRIVERS.d \
./STM32F103C6_DRIVERS/STM32_F103C6_RCC_DRIVERS.d \
./STM32F103C6_DRIVERS/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F103C6_DRIVERS/EXTI_DRIVER.o: ../STM32F103C6_DRIVERS/EXTI_DRIVER.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/INC" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6_DRIVERS/EXTI_DRIVER.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6_DRIVERS/STM32F103C6_USART_DRIVERS.o: ../STM32F103C6_DRIVERS/STM32F103C6_USART_DRIVERS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/INC" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6_DRIVERS/STM32F103C6_USART_DRIVERS.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6_DRIVERS/STM32_F103C6_GPIO_DRIVERS.o: ../STM32F103C6_DRIVERS/STM32_F103C6_GPIO_DRIVERS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/INC" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6_DRIVERS/STM32_F103C6_GPIO_DRIVERS.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6_DRIVERS/STM32_F103C6_RCC_DRIVERS.o: ../STM32F103C6_DRIVERS/STM32_F103C6_RCC_DRIVERS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/INC" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6_DRIVERS/STM32_F103C6_RCC_DRIVERS.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6_DRIVERS/Timer.o: ../STM32F103C6_DRIVERS/Timer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"F:/Mostafa/smart_egat_task/STM32F103C6_DRIVERS/INC" -I../Inc -I"F:/Mostafa/smart_egat_task/JSON/includes" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/include" -I"F:/Mostafa/smart_egat_task/FREE_RTOS/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6_DRIVERS/Timer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

