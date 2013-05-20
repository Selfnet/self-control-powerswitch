
#ifndef __IO_HELPER_H__
#define __IO_HELPER_H__

#include "stm32f10x.h"


#define UI32_DELAY_TIME 0x1FFFFF

#define BUTTON_1_PIN    GPIO_Pin_0
#define BUTTON_1_PORT   GPIOA
#define BUTTON_1_CLK    RCC_APB2Periph_GPIOA

#define KEY_PRESSED     0x00
#define KEY_NOT_PRESSED 0x01

//onboard
#define LED0_PIN        GPIO_Pin_12
#define LED0_GPIO_PORT  GPIOC
#define LED0_GPIO_CLK   RCC_APB2Periph_GPIOC

//gruen
#define LED1_PIN        GPIO_Pin_5
#define LED1_GPIO_PORT  GPIOB
#define LED1_GPIO_CLK   RCC_APB2Periph_GPIOB

#define LED2_PIN        GPIO_Pin_6
#define LED2_GPIO_PORT  GPIOA
#define LED2_GPIO_CLK   RCC_APB2Periph_GPIOA

//rot
#define LED3_PIN        GPIO_Pin_8
#define LED3_GPIO_PORT  GPIOB
#define LED3_GPIO_CLK   RCC_APB2Periph_GPIOB



//light
#define LIGHT_U2_1_PIN  GPIO_Pin_0
#define LIGHT_U2_1_PORT GPIOB
#define LIGHT_U2_1_CLK  RCC_APB2Periph_GPIOB

#define LIGHT_U2_2_PIN  GPIO_Pin_0
#define LIGHT_U2_2_PORT GPIOC
#define LIGHT_U2_2_CLK  RCC_APB2Periph_GPIOC

#define LIGHT_U3_1_PIN  GPIO_Pin_9
#define LIGHT_U3_1_PORT GPIOB
#define LIGHT_U3_1_CLK  RCC_APB2Periph_GPIOB

#define LIGHT_U4_1_PIN  GPIO_Pin_7
#define LIGHT_U4_1_PORT GPIOB
#define LIGHT_U4_1_CLK  RCC_APB2Periph_GPIOB

#define LIGHT_U5_1_PIN  GPIO_Pin_6
#define LIGHT_U5_1_PORT GPIOB
#define LIGHT_U5_1_CLK  RCC_APB2Periph_GPIOB

#define LIGHT_U5_2_PIN  GPIO_Pin_2
#define LIGHT_U5_2_PORT GPIOD
#define LIGHT_U5_2_CLK  RCC_APB2Periph_GPIOD

#define LIGHT_UX_1_PIN  GPIO_Pin_11
#define LIGHT_UX_1_PORT GPIOC
#define LIGHT_UX_1_CLK  RCC_APB2Periph_GPIOC

#define LIGHT_UX_2_PIN  GPIO_Pin_10
#define LIGHT_UX_2_PORT GPIOA
#define LIGHT_UX_2_CLK  RCC_APB2Periph_GPIOA


/*
//Falschrum
#define LIGHT_U2_1_PIN  GPIO_Pin_10
#define LIGHT_U2_1_PORT GPIOA
#define LIGHT_U2_1_CLK  RCC_APB2Periph_GPIOA

#define LIGHT_U2_2_PIN  GPIO_Pin_11
#define LIGHT_U2_2_PORT GPIOC
#define LIGHT_U2_2_CLK  RCC_APB2Periph_GPIOC

#define LIGHT_U3_1_PIN  GPIO_Pin_2
#define LIGHT_U3_1_PORT GPIOD
#define LIGHT_U3_1_CLK  RCC_APB2Periph_GPIOD

#define LIGHT_U3_2_PIN  GPIO_Pin_6
#define LIGHT_U3_2_PORT GPIOB
#define LIGHT_U3_2_CLK  RCC_APB2Periph_GPIOB

#define LIGHT_U4_1_PIN  GPIO_Pin_7
#define LIGHT_U4_1_PORT GPIOB
#define LIGHT_U4_1_CLK  RCC_APB2Periph_GPIOB

#define LIGHT_U4_2_PIN  GPIO_Pin_9
#define LIGHT_U4_2_PORT GPIOB
#define LIGHT_U4_2_CLK  RCC_APB2Periph_GPIOB

#define LIGHT_U5_1_PIN  GPIO_Pin_0
#define LIGHT_U5_1_PORT GPIOC
#define LIGHT_U5_1_CLK  RCC_APB2Periph_GPIOC

#define LIGHT_U5_2_PIN  GPIO_Pin_0
#define LIGHT_U5_2_PORT GPIOB
#define LIGHT_U5_2_CLK  RCC_APB2Periph_GPIOB
*/

struct light_state_t {
    char state;
    uint32_t last_changed;
};

typedef struct light_state_t Light_State;

volatile  Light_State lights[8];

// Light
void Light_init(void);
void Light_On(char roomnr, char lightnr);
void Light_Off(char roomnr, char lightnr);


//Buttons
void button_init(void);
uint32_t Button_GetState(uint32_t);

// Can functions
void CAN_config(void);

// Led
void LED_init(void);
void LED_On(uint32_t lednr);
void LED_Off(uint32_t lednr);
void LED_Toggle(uint32_t lednr);

#endif
