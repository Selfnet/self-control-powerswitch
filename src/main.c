/**
******************************************************************************
* @file    Project/Template/main.c
* @author  MCD Application Team
* @version V3.1.0
* @date    06/19/2009
* @brief   Main program body
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "io-helper.h" //dirks button+led func

#include "timer.h"
#include "io_pwm.h"

#include <stdio.h>

/*#include "usb_core.h"
#include "usbd_usr.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc_core.h"
#include "usbd_cdc_vcp.h"*/

#include "can.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define UI32_DELAY_TIME 0x1FFFFF


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t CriticalSecCntr;
USART_InitTypeDef USART_InitStructure;


//__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void SysTickStart(uint32_t Tick)
{
    RCC_ClocksTypeDef Clocks;
    volatile uint32_t dummy;

    RCC_GetClocksFreq(&Clocks);

    dummy = SysTick->CTRL;
    SysTick->LOAD = (Clocks.HCLK_Frequency/8)/Tick;

    SysTick->CTRL = 1;
}

void SysTickStop(void)
{
    SysTick->CTRL = 0;
}


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

    // Compute the prescaler value 
    uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;

    // Time base configuration 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 5;
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
}

/**
* @brief  Main program.
* @param  None
* @retval None
*/
int main(void)
{
    /* Button Init */
    button_init();

    init_timer();

    /* LED Init */
    LED_init();
    LED_On(0);

    /* Setup STM32 system (clock, PLL and Flash configuration) */
    SystemInit();

    // initialize Light IO Pins
    //Light_init();
    Light_pwm_init();

    // initialize CAN-Bus and enable CAN Interrupts
    CAN_config();

    struct timer sec_timer;
    timer_set(&sec_timer, TICKS_PER_SECOND/10); //1x pro sec wird gesynced
    led_count = 0;

    init_timer();

    LED_On(1);
    LED_On(2);
    LED_On(3);


    //Main loop, ohne ethernet gibts hir nix zu tun ;)
    while(1)
    {
        if( can_puffer_cnt > 0 )
        {
            if((CAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0 && can_puffer_cnt > 0)
                CAN_Transmit(CAN1, can_puffer[can_puffer_cnt--]);

            if((CAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1 && can_puffer_cnt > 0)
                CAN_Transmit(CAN1, can_puffer[can_puffer_cnt--]);

            if((CAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2 && can_puffer_cnt > 0)
                CAN_Transmit(CAN1, can_puffer[can_puffer_cnt--]);
        }

        if(timer_expired(&sec_timer))
        {
            timer_reset(&sec_timer);
            LED_Toggle(3);

            int i;
            for(i = 0 ; i < 8 ; i++ )
            {
                // relays sollen nur alle 0.x sekunden geschaltet werden
                if( (clock_time()-lights[i].last_changed)/TICKS_PER_SECOND <= 1 )
                    continue;

                int pwm = 0;
                if(lights[i].state == 3)
                {
                    pwm = 2000;
                    lights[i].state = 2;
                    lights[i].last_changed = clock_time();
                }
                else if(lights[i].state == 2)
                {
                    pwm = 1000;
                    lights[i].state = 1;
                    lights[i].last_changed = clock_time();
                }
                else if(lights[i].state == 1)
                {
                    pwm = 1000;
                    LED_Toggle(2);
                }

                switch(i)
                {
                    case 0:
                        TIM3->CCR4 = pwm;
                    break;
                    case 1:
                        TIM3->CCR2 = pwm;
                    break;
                    case 2:
                        TIM3->CCR3 = pwm;
                    break;
                    case 3:
                        TIM4->CCR4 = pwm;
                    break;
                    case 4:
                        TIM4->CCR2 = pwm;
                    break;
                    case 5:
                        TIM4->CCR3 = pwm;
                    break;
                    case 6:
                        TIM4->CCR1 = pwm;
                    break;
                    case 7:
                        TIM3->CCR1 = pwm;
                    break;
                }
            }

/*#define LIGHT_U2_1_PIN  GPIO_Pin_1 //PB1  EXT1-26   TIM3_CH4
#define LIGHT_U2_2_PIN  GPIO_Pin_7 //PA7  EXT1-22   TIM3_CH2
#define LIGHT_U3_1_PIN  GPIO_Pin_0 //PB0  EXT1-21   TIM3_CH3
#define LIGHT_U4_1_PIN  GPIO_Pin_9 //PB9  EXT1-17   TIM4_CH4
#define LIGHT_U5_1_PIN  GPIO_Pin_7 //PB7  EXT1-15   TIM4_CH2
#define LIGHT_U5_2_PIN  GPIO_Pin_8 //PB8  EXT1-16   TIM4_CH3
#define LIGHT_UX_1_PIN  GPIO_Pin_6 //PB6  EXT1-13   TIM4_CH1
#define LIGHT_UX_2_PIN  GPIO_Pin_6 //PA6  EXT1-14   TIM3_CH1
*/
        }
    }
}


#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
* @}
*/


/******************* (C) COPYRIGHT 2009 STMicroelectronics ***
    **END OF FILE****/
