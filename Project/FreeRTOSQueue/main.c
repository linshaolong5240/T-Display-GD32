#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreate(void);
void TaskCreater(void *parameters);
void TaskA(void *parameters);
void TaskB(void *parameters);

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;

typedef struct
{
    int value;
} Message;

QueueHandle_t taskAMessageQueue = NULL;

void IRQConfigure(void)
{
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); // 四位优先级组全配置为lvl
    eclic_global_interrupt_enable();                       // 使能全局中断
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    size_t size = 0;
    /* systesm init */
    _init();
    IRQConfigure();
    USARTInit();
    LEDInit();

#if DEBUG
    rtprintf("1\r\n");
#endif
    TaskCreate();
    // xTaskCreate(TaskCreater, "TaskCreater", 256, NULL, 2, &taskCreater);
#if DEBUG
    rtprintf("2\r\n");
#endif
    vTaskStartScheduler();
#if DEBUG
    while (1)
    {
        rtprintf("RTOS Exit\r\n");
    }
#endif
}

/* retarget the C library printf function to the USART */
// int _put_char(int ch)
// {
//     usart_data_transmit(USART0, (uint8_t)ch);
//     while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
//     return ch;
// }

void TaskCreate(void)
{
    size_t size = 0;
    size = xPortGetFreeHeapSize();
#if DEBUG
    rtprintf("tc1 size %d\r\n", size);
#endif
    xTaskCreate(TaskA, "TaskA", 256, NULL, 3, &taskA);
    size = xPortGetFreeHeapSize();
#if DEBUG
    rtprintf("tc2 size %d\r\n", size);
#endif
    xTaskCreate(TaskB, "TaskB", 256, NULL, 3, &taskB);
}

void TaskCreater(void *parameters)
{
    taskENTER_CRITICAL();
    xTaskCreate(TaskA, "TaskA", 256, NULL, 2, &taskA);
    xTaskCreate(TaskB, "TaskB", 256, NULL, 2, &taskB);
    taskEXIT_CRITICAL();
    vTaskDelete(taskCreater);
}

void TaskA(void *parameters)
{
    Message message;
    taskAMessageQueue = xQueueCreate(10, sizeof(Message));
    while (1)
    {

        if (taskAMessageQueue != NULL)
        {
            if (xQueueReceive(taskAMessageQueue, &message, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                rtprintf("TaskA receive message succeed, value = %d\r\n", message.value);
#endif
                LEDToggle(LED_R);
            }
        }
    }
}

void TaskB(void *parameters)
{
    Message message = { 0 };

    while (1)
    {
        if (taskAMessageQueue != NULL)
        {
            message.value++;
            if (xQueueSend(taskAMessageQueue, &message, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                rtprintf("TaskB send message succeed, value = %d\r\n", message.value);
#endif
                LEDToggle(LED_G);
                vTaskDelay(1000);
            }
            else
            {
            }
        }
    }
}

void freertos_risc_v_application_exception_handler(UBaseType_t mcause)
{
    // rtprintfISR("exception: 0x%04x\r\n", mcause);
    // rtprintfISR("In trap handler, the mcause is %d\n", mcause);
    // rtprintfISR("In trap handler, the mepc is 0x%x\n", read_csr(mepc));
    // rtprintfISR("In trap handler, the mtval is 0x%x\n", read_csr(mbadaddr));
    // _exit(mcause);
    return 0;
}

void freertos_risc_v_application_interrupt_handler(UBaseType_t mcause)
{
    rtprintfISR("interrupt: 0x%04x\r\n", mcause);
}

void vApplicationTickHook(void)
{
    // rtprintf("Tick\r\n");
}

void vApplicationIdleHook(void)
{
    // rtprintf("Idle\r\n");
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