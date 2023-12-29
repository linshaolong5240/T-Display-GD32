#!/bin/sh
MAKE_OPTION=-j

FLASH=flash
OS=FreeRTOS

make clean

# TARGET
#
# TARGET=HelloWorld
# TARGET=MachineTimer
# TARGET=ADC0
# TARGET=ADC0-Temperature

# FreeRTOS TARGET
#
# TARGET=FreeRTOSTemplate
# TARGET=FreeRTOSTask
# TARGET=FreeRTOSQueue
# TARGET=FreeRTOSSemaphoreBinary
TARGET=FreeRTOSSemaphoreCounting
# TARGET=FreeRTOSSemaphoreMutex

make $MAKE_OPTION TARGET=$TARGET OS=$OS $FLASH
