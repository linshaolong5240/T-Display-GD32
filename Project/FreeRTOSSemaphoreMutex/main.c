#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef struct
{
    int value;
} Message;

TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;

SemaphoreHandle_t semaphoreMutex = NULL;

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreate(void);
void TaskA(void *parameters);
void TaskB(void *parameters);

void IRQConfigure(void)
{
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); // 4 bits for level, 0 bits for priority
    eclic_global_interrupt_enable();                       // enable global interrupt
}

int main(void)
{
    size_t size = 0;
    /* systesm init */
    _init();
    IRQConfigure();
    USARTInit();
    LEDInit();

    semaphoreMutex = xSemaphoreCreateMutex();
    if (semaphoreMutex != NULL)
    {
#if DEBUG
        rtprintf("Semaphore create Succeed\r\n");
#endif
    }
    else
    {
#if DEBUG
        rtprintf("Semaphore create Failed\r\n");
#endif
    }

    TaskCreate();
    vTaskStartScheduler();

    while (1)
    {
        rtprintf("RTOS Exit\r\n");
    }
    return 0;
}

void TaskCreate(void)
{
    xTaskCreate(TaskA, "TaskA", 256, NULL, 3, &taskA);
    xTaskCreate(TaskB, "TaskB", 256, NULL, 3, &taskB);
}

void TaskA(void *parameters)
{
    while (1)
    {
        if (semaphoreMutex != NULL)
        {
            if (xQueueSemaphoreTake(semaphoreMutex, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                rtprintf("TaskA semaphore take succeed\r\n");
#endif
                LEDToggle(LED_RED);
                if (xSemaphoreGive(semaphoreMutex) == pdPASS)
                {
#if DEBUG
                    rtprintf("TaskA semaphore give succeed\r\n");
#endif
                    vTaskDelay(100);
                }
                else
                {
#if DEBUG
                    rtprintf("TaskA semaphore give failed");
#endif
                }
            }
            else
            {
#if DEBUG
                rtprintf("TaskA semaphore take failed\r\n");
#endif
            }
        }
    }
}

void TaskB(void *parameters)
{
    while (1)
    {
        if (semaphoreMutex != NULL)
        {
            if (xQueueSemaphoreTake(semaphoreMutex, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                rtprintf("TaskB semaphore take succeed\r\n");
#endif
                LEDToggle(LED_GREEN);
                if (xSemaphoreGive(semaphoreMutex) == pdPASS)
                {
#if DEBUG
                    rtprintf("TaskB semaphore give succeed\r\n");
#endif
                vTaskDelay(1000);
                }
                else
                {
#if DEBUG
                    rtprintf("TaskB semaphore give failed");
#endif
                }
            }
            else
            {
#if DEBUG
                rtprintf("TaskB semaphore take failed\r\n");
#endif
            }
        }
    }
}

void freertos_risc_v_application_exception_handler(UBaseType_t mcause)
{
#if DEBUG_EXCEPTION
    rtprintfISR("exception: 0x%04x\r\n", mcause);
    rtprintfISR("In trap handler, the mcause is %d\n", mcause);
    rtprintfISR("In trap handler, the mepc is 0x%x\n", read_csr(mepc));
    rtprintfISR("In trap handler, the mtval is 0x%x\n", read_csr(mbadaddr));
    _exit(mcause);
#endif
}

void freertos_risc_v_application_interrupt_handler(UBaseType_t mcause)
{
#if DEBUG_INTERRUPT
    rtprintfISR("interrupt: 0x%04x\r\n", mcause);
#endif
}

void vApplicationTickHook(void)
{
#if DEBUG_HOOK
// rtprintf("Tick\r\n");
#endif
}

void vApplicationIdleHook(void)
{
#if DEBUG_HOOK
// rtprintf("Idle\r\n");
#endif
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
#if DEBUG_HOOK
    rtprintf("task：%s Overflow\r\n", pcTaskName);
#endif
}

void vApplicationMallocFailedHook(void)
{
#if DEBUG_HOOK
    rtprintf("MallocFailed\r\n");
#endif
}

void vApplicationDaemonTaskStartupHook(void)
{
#if DEBUG_HOOK
    rtprintf("DaemonTask\r\n");
#endif
}