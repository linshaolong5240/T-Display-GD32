#include "LED.h"
#include "Foundation.h"
#include "gd32vf103_rcu.h"

#define LED_R_GPIO          GPIOC
#define LED_R_PIN           GPIO_PIN_13
#define LED_R_ACTIVE        RESET
#define LED_R_DEACTIVE      SET

#define LED_G_GPIO          GPIOA
#define LED_G_PIN           GPIO_PIN_1
#define LED_G_ACTIVE        RESET
#define LED_G_DEACTIVE      SET

#define LED_B_GPIO          GPIOA
#define LED_B_PIN           GPIO_PIN_2
#define LED_B_ACTIVE        RESET
#define LED_B_DEACTIVE      SET

void LEDInit(void)
{
    /* LED R:PC13 G:PA1 B:PA2 低电平有效 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(LED_R_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_R_PIN);
    gpio_init(LED_G_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_G_PIN);
    gpio_init(LED_B_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_B_PIN);

    gpio_bit_write(LED_R_GPIO, LED_R_PIN, LED_R_DEACTIVE);
    gpio_bit_write(LED_G_GPIO, LED_G_PIN, LED_G_DEACTIVE);
    gpio_bit_write(LED_B_GPIO, LED_B_PIN, LED_B_DEACTIVE);
}

void LEDControl(LED led, bool enable)
{
    switch (led)
    {
    case LED_R:
        gpio_bit_write(LED_R_GPIO, LED_R_PIN, enable ? LED_R_ACTIVE : LED_R_DEACTIVE);
        break;
    case LED_G:
        gpio_bit_write(LED_G_GPIO, LED_G_PIN, enable ? LED_G_ACTIVE : LED_G_DEACTIVE);
        break;
    case LED_B:
        gpio_bit_write(LED_B_GPIO, LED_B_PIN, enable ? LED_B_ACTIVE : LED_B_DEACTIVE);
        break;
    }
}

void LEDToggle(LED led)
{
    switch (led)
    {
    case LED_R:
        gpio_bit_write(LED_R_GPIO, LED_R_PIN, gpio_input_bit_get(LED_R_GPIO, LED_R_PIN) == LED_R_ACTIVE ? LED_R_DEACTIVE : LED_R_ACTIVE);
        break;
    case LED_G:
        gpio_bit_write(LED_G_GPIO, LED_G_PIN, gpio_input_bit_get(LED_G_GPIO, LED_G_PIN) == LED_G_ACTIVE ? LED_G_DEACTIVE : LED_G_ACTIVE);
        break;
    case LED_B:
        gpio_bit_write(LED_B_GPIO, LED_B_PIN, gpio_input_bit_get(LED_B_GPIO, LED_B_PIN) == LED_B_ACTIVE ? LED_B_DEACTIVE : LED_B_ACTIVE);
        break;
    }
}