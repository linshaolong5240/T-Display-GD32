#!/bin/sh
MAKE_OPTION=-j

FLASH=flash

make clean

# TARGET
#
# TARGET=ADC0
# TARGET=ADC0Temperature
# TARGET=HelloWorld
# TARGET=MachineTimer
TARGET=USART

# FreeRTOS TARGET
#
# OS=FreeRTOS
#
# TARGET=FreeRTOSQueue
# TARGET=FreeRTOSSemaphoreBinary
# TARGET=FreeRTOSSemaphoreCounting
# TARGET=FreeRTOSSemaphoreMutex
# TARGET=FreeRTOSTask
# TARGET=FreeRTOSTemplate

make $MAKE_OPTION TARGET=$TARGET OS=$OS $FLASH
