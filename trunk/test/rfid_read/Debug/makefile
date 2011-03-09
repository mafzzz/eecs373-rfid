################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include CMSIS/subdir.mk
-include CMSIS/startup_gcc/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
rfid_read \

MEMORYMAP += \
memory-map.xml \

BINFILES += \
rfid_read.bin \

IHEXFILES += \
rfid_read.hex \

SRECFILES += \
rfid_read.srec \

LISTINGS += \
rfid_read.lst \


# All Target
all: rfid_read secondary-outputs

# Tool invocations
rfid_read: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU C Linker'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -specs=bare.specs  -o"rfid_read" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

memory-map.xml: $(EXECUTABLES)
	@echo 'Invoking: Memory map generator'
	actel-map -o "memory-map.xml" $(EXECUTABLES) $(BUILDCMD)
	@echo 'Finished building: $@'
	@echo ' '

rfid_read.bin: $(EXECUTABLES)
	@echo 'Invoking: GNU Binary Generator'
	arm-none-eabi-objcopy -O binary $(EXECUTABLES) "rfid_read.bin"
	@echo 'Finished building: $@'
	@echo ' '

rfid_read.hex: $(EXECUTABLES)
	@echo 'Invoking: GNU Intel Hex File Generator'
	arm-none-eabi-objcopy -O ihex $(EXECUTABLES) "rfid_read.hex"
	@echo 'Finished building: $@'
	@echo ' '

rfid_read.srec: $(EXECUTABLES)
	@echo 'Invoking: GNU S-Record Generator'
	arm-none-eabi-objcopy -O srec $(EXECUTABLES) "rfid_read.srec"
	@echo 'Finished building: $@'
	@echo ' '

rfid_read.lst: $(EXECUTABLES)
	@echo 'Invoking: GNU Listing Generator'
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "rfid_read.lst"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(MEMORYMAP)$(OBJS)$(C_DEPS)$(SRECFILES)$(IHEXFILES)$(BINFILES)$(LISTINGS)$(EXECUTABLES) rfid_read
	-@echo ' '

secondary-outputs: $(MEMORYMAP) $(BINFILES) $(IHEXFILES) $(SRECFILES) $(LISTINGS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
