################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include STM32F103C6_DRIVERS/USART/subdir.mk
-include STM32F103C6_DRIVERS/SPI/subdir.mk
-include STM32F103C6_DRIVERS/RCC\ (\ DEMO\ )/subdir.mk
-include STM32F103C6_DRIVERS/I2C/subdir.mk
-include STM32F103C6_DRIVERS/GPIO/subdir.mk
-include STM32F103C6_DRIVERS/EXTI/subdir.mk
-include STM32F103C6_DRIVERS/ADC/subdir.mk
-include JSON/subdir.mk
-include FREE_RTOS/portable/MemMang/subdir.mk
-include FREE_RTOS/portable/GCC/ARM_CM3/subdir.mk
-include FREE_RTOS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
smart_egat_task.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
smart_egat_task.list \

OBJCOPY_BIN += \
smart_egat_task.bin \


# All Target
all: smart_egat_task.elf secondary-outputs

# Tool invocations
smart_egat_task.elf: $(OBJS) $(USER_OBJS) F:\Mostafa\smart_egat_task\STM32F103C6TX_FLASH.ld
	arm-none-eabi-gcc -o "smart_egat_task.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"F:\Mostafa\smart_egat_task\STM32F103C6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="smart_egat_task.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

smart_egat_task.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "smart_egat_task.list"
	@echo 'Finished building: $@'
	@echo ' '

smart_egat_task.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "smart_egat_task.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
