#include "buzzer.h"

static TIM_HandleTypeDef buzzerTimer;
static uint32_t timerClock;

void Buzzer_Init(void) {
    GPIO_InitTypeDef gpio = {0};
    TIM_OC_InitTypeDef channel = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* Same wiring as SDK_Buzzer: PA5 = TIM2_CH1. */
    gpio.Pin = GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &gpio);

    timerClock = HAL_RCC_GetPCLK1Freq();
    /* APB timers run at twice PCLK only when the APB divider is not 1. */
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0U) timerClock *= 2U;
    buzzerTimer.Instance = TIM2;
    buzzerTimer.Init.Prescaler = 0;
    buzzerTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
    buzzerTimer.Init.Period = timerClock / 440U - 1U;
    buzzerTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    buzzerTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&buzzerTimer) != HAL_OK) Error_Handler();
    channel.OCMode = TIM_OCMODE_PWM1;
    channel.Pulse = 0;
    channel.OCPolarity = TIM_OCPOLARITY_HIGH;
    channel.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&buzzerTimer, &channel, TIM_CHANNEL_1) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_PWM_Start(&buzzerTimer, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
}

void Buzzer_SetTone(uint16_t frequency) {
    __HAL_TIM_SET_COMPARE(&buzzerTimer, TIM_CHANNEL_1, 0);
    if (frequency == 0U) return;

    uint32_t period = timerClock / frequency;
    __HAL_TIM_DISABLE(&buzzerTimer);
    __HAL_TIM_SET_AUTORELOAD(&buzzerTimer, period - 1U);
    __HAL_TIM_SET_COUNTER(&buzzerTimer, 0);
    __HAL_TIM_SET_COMPARE(&buzzerTimer, TIM_CHANNEL_1, period / 2U);
    /* Load the channel preload immediately, including when changing pitch. */
    buzzerTimer.Instance->EGR = TIM_EGR_UG;
    __HAL_TIM_ENABLE(&buzzerTimer);
}
