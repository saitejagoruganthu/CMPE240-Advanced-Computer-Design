################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Project_ShadingAndDiffuseReflection
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: Project_ShadingAndDiffuseReflection.axf

# Tool invocations
Project_ShadingAndDiffuseReflection.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/saitejagoruganthu/Documents/CMPE240/MCUXWorkspace/CMSIS_CORE_LPC17xx/Debug" -Xlinker -Map="Project_ShadingAndDiffuseReflection.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "Project_ShadingAndDiffuseReflection_Debug.ld" -o "Project_ShadingAndDiffuseReflection.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Project_ShadingAndDiffuseReflection.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Project_ShadingAndDiffuseReflection.axf"; # arm-none-eabi-objcopy -v -O binary "Project_ShadingAndDiffuseReflection.axf" "Project_ShadingAndDiffuseReflection.bin" ; # checksum -p LPC1769 -d "Project_ShadingAndDiffuseReflection.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
