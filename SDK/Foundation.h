#ifndef FOUNDATION_H
#define FOUNDATION_H
#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

#include "gd32vf103_libopt.h"
#include "rtprint.h"

#define ON      1
#define OFF     0

void delay(uint count);

#ifdef __cplusplus
}
#endif
#endif /* FOUNDATION_H */