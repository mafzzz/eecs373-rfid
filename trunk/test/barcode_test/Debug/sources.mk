################################################################################
# Automatically-generated file. Do not edit!
################################################################################

O_SRCS := 
C_SRCS := 
S_UPPER_SRCS := 
S_SRCS := 
OBJ_SRCS := 
MEMORYMAP := 
OBJS := 
C_DEPS := 
SRECFILES := 
IHEXFILES := 
LISTINGS := 
EXECUTABLE := 

# Every subdirectory with source files must be described here
SUBDIRS := \
. \

################################################################################
# Microsemi SoftConsole IDE Variables
################################################################################

BUILDCMD = arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -specs=bare.specs -L"C:\Users\mladmon\Documents\EECS 373\rfid project\eecs373-rfid\src\Debug" -T../../../src/CMSIS/startup_gcc/debug-in-actel-smartfusion-esram.ld -Wl,-Map=$(EXECUTABLE).map 
SHELL := cmd.exe
EXECUTABLE := barcode_test

# Target-specific items to be cleaned up in the top directory.
clean::
	-$(RM) $(wildcard ./*.map) 
