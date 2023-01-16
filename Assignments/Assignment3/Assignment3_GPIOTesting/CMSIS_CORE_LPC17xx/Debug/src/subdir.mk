################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc17.c \
../src/main.c \
../src/system_LPC17xx.c \
../src/uart.c 

C_DEPS += \
./src/cr_startup_lpc17.d \
./src/main.d \
./src/system_LPC17xx.d \
./src/uart.d 

OBJS += \
./src/cr_startup_lpc17.o \
./src/main.o \
./src/system_LPC17xx.o \
./src/uart.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSIS_CORE_LPC17xx -I"C:\Users\admin\Documents\MCUXpressoIDE_11.6.0_8187\workspace\CMSIS_CORE_LPC17xx\inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/cr_startup_lpc17.d ./src/cr_startup_lpc17.o ./src/main.d ./src/main.o ./src/system_LPC17xx.d ./src/system_LPC17xx.o ./src/uart.d ./src/uart.o

.PHONY: clean-src

