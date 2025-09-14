################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/src/stm32f4-hal/subdir.mk
-include system/src/newlib/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cmsis/subdir.mk
-include src/Robo_int/subdir.mk
-include src/RTOS/scheduler/subdir.mk
-include src/MCAL/USART/subdir.mk
-include src/MCAL/SYSTICK/subdir.mk
-include src/MCAL/SYSCFG/subdir.mk
-include src/MCAL/SPI/subdir.mk
-include src/MCAL/RTC/subdir.mk
-include src/MCAL/RCC/subdir.mk
-include src/MCAL/PWM/subdir.mk
-include src/MCAL/NVIC/subdir.mk
-include src/MCAL/GPIO/subdir.mk
-include src/MCAL/EXTI/subdir.mk
-include src/MCAL/DMA/subdir.mk
-include src/MCAL/ADC/subdir.mk
-include src/LIB/subdir.mk
-include src/HAL/TFT/subdir.mk
-include src/HAL/Seven_Segment/subdir.mk
-include src/HAL/SERMUX/subdir.mk
-include src/HAL/S2P/subdir.mk
-include src/HAL/RGB_LED/subdir.mk
-include src/HAL/LEDMATRIX/subdir.mk
-include src/HAL/IR/subdir.mk
-include src/HAL/Fonts/subdir.mk
-include src/HAL/ESP_WIFI/subdir.mk
-include src/HAL/DAC_8bit/subdir.mk
-include src/HAL/Button/subdir.mk
-include src/Examples/subdir.mk
-include src/APP/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ROBO32.hex \

SECONDARY_SIZE += \
ROBO32.siz \


# All Target
all: ROBO32.elf secondary-outputs

# Tool invocations
ROBO32.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"ROBO32.map" --specs=nano.specs -o "ROBO32.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ROBO32.hex: ROBO32.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ROBO32.elf"  "ROBO32.hex"
	@echo 'Finished building: $@'
	@echo ' '

ROBO32.siz: ROBO32.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "ROBO32.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) ROBO32.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
