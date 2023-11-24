#!/bin/sh

make clean

# make TARGET=HelloWorld
# make TARGET=MachineTimer

# FreeRTOS
# make TARGET=FreeRTOSTemplate OS=FreeRTOS
make TARGET=FreeRTOSTask OS=FreeRTOS flash