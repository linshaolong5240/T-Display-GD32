#!/bin/sh

make clean

# make TARGET=HelloWorld
# make TARGET=MachineTimer
make TARGET=ADC-Temperature flash

# FreeRTOS
# make TARGET=FreeRTOSTemplate OS=FreeRTOS
# make TARGET=FreeRTOSTask OS=FreeRTOS flash
# make TARGET=FreeRTOSQueue OS=FreeRTOS flash
# make TARGET=FreeRTOSQueue OS=FreeRTOS
