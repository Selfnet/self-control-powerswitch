

#include <string.h>

#include "io_pwm.h"

Light_State lights[8];

//RGB_Led_State leds[4];
/*
void init_timer(void)
{
    // TIM1 clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // NVIC Configuration
    NVIC_InitTypeDef NVIC_InitStructure;

    // Enable the TIM1 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Compute the prescaler value 1x pro sek
    uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 1000) - 1;

    // Time base configuration
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 10;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // TIM enable counter
    TIM_Cmd(TIM1, ENABLE);

    // TIM IT enable
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    //Update Interupt (URS bit) Enable
    //TIM_UpdateRequestConfig(TIM1, TIM_UpdateSource_Regular); 
}*/


void Light_pwm_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    //Timer clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // Output clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


    // ** Output (PWM) Ports definieren **
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    // ** PWM Timer **
    // Time base layout settings: TIM3 works because f = 72KHz 72M divided by 72000000, we subtract 1 and prescaler (0)
    // 72000000 on share for a period of 1 (999 +1) and get some 72000
    TIM_TimeBaseStructure.TIM_Period = PWM_STEPS-1;
    TIM_TimeBaseStructure.TIM_Prescaler = 2;  //fclk = 72M/72M - 1 = 0
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //0 = do not share the clock
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // count up mode
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


    TIM_OCInitTypeDef TIM_OCInitStructure;

    // ** PWM Timer Channel**
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //signal from the timer will be used to control the interrupt controller, so it must be Enable
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   // low state

    //TIM2 CN1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //PA0  EXT2-4

    //TIM2 CN2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //PA1  EXT2-8

    //TIM2 CN3
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //PA2  EXT2-7

    //TIM2 CN4
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //PA3  EXT2-10

    //TIM3 CN1
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //PA6  EXT1-14

    //TIM3 CN2
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //PA7  EXT1-22

    //TIM3 CN3
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //PB0  EXT1-21

    //TIM3 CN4
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //PB1  EXT1-26

    //TIM4 CN1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //PB6  EXT1-13

    //TIM4 CN2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //PB7  EXT1-15

    //TIM4 CN3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //PB8  EXT1-16

    //TIM4 CN4
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //PB9  EXT1-17


    // ** Reload TIM ARR Config **
    TIM_ARRPreloadConfig(TIM2,ENABLE);
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    TIM_ARRPreloadConfig(TIM4,ENABLE);

    //  Enable the timer
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

    // TIM1 Interrupt Config - fürs Farben setzen
    init_timer();
    
    //lights[0].pwm = &TIM3->CCR4;
    
}


// ***************************************************************************************************

// set values between 0 and 2047
void update_PWM(void)
{
    /* Set the Capture Compare Register value */
    /*TIM2->CCR1 = (int)leds[2].cur_r;
    TIM2->CCR4 = (int)leds[2].cur_g;
    TIM2->CCR2 = (int)leds[2].cur_b;

    TIM3->CCR4 = (int)leds[3].cur_r;
    TIM2->CCR3 = (int)leds[3].cur_g;
    TIM3->CCR3 = (int)leds[3].cur_b;

    TIM4->CCR1 = (int)leds[1].cur_r;
    TIM4->CCR4 = (int)leds[1].cur_g;
    TIM4->CCR2 = (int)leds[1].cur_b;

    TIM3->CCR2 = (int)leds[0].cur_r;
    TIM4->CCR3 = (int)leds[0].cur_b;
    TIM3->CCR1 = (int)leds[0].cur_g;*/
}



