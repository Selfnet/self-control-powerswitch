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

#include <stdio.h>

/*#include "usb_core.h"
#include "usbd_usr.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc_core.h"
#include "usbd_cdc_vcp.h"*/

#include "led_pwm.h"
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
    /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* NVIC Configuration */
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Compute the prescaler value 1x pro sek*/
    uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 1000) - 1;

    /* Time base configuration */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 10;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* TIM enable counter */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM IT enable */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    /*Update Interupt (URS bit) Enable */
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
    button_init(); // wird für pwm verwendet

    init_timer(); //TODO macht es noch kaputt

    /* LED Init */
    LED_init();
    LED_On(0);


    /* Setup STM32 system (clock, PLL and Flash configuration) */
    SystemInit();

    // initialize Light IO Pins
    Light_init();

    // initialize CAN-Bus and enable CAN Interrupts
    CAN_config();

    struct timer sec_timer;
    timer_set(&sec_timer, TICKS_PER_SECOND); //1x pro sec wird gesynced
    led_count = 0;

    LED_On(0);

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
            LED_Toggle(2);
            LED_Toggle(0);

            //id++;
            //if( id <= 5 )
            //    Light_On(id, 1);
            //else if( id < 10 )
            //    Light_On(id-4, 2);

            //Light_On(2, 2);

//            if(led_count < 4200000000) //soll keinen ueberlauf geben
//                led_count++;

            /*if(led_count % 10 == 0) //send current color and led_count
            {
                CanTxMsg TxMessage;
                TxMessage.IDE = CAN_ID_EXT;                                 //immer extended can frames
                TxMessage.ExtId = CAN_EXT_ID;                               //default ID setzen
                TxMessage.ExtId |= setSender( NODE_CAN_ID );
                TxMessage.ExtId |= setType( CAN_PROTO_LED );
                TxMessage.ExtId |= setRecipient( 0x20 );
                TxMessage.RTR = CAN_RTR_Data;

                TxMessage.DLC = 8; 
                TxMessage.Data[0] = leds[0].color_mode << 4 | 1; //LedID and leds[id].color_mode;
                TxMessage.Data[1] = 0xFE; // GETCOLORMODE 

                TxMessage.Data[2] = leds[0].mode;
                TxMessage.Data[3] = leds[0].r;
                TxMessage.Data[4] = leds[0].g;
                TxMessage.Data[5] = leds[0].b;

                TxMessage.Data[6] = 0xFF;
                TxMessage.Data[7] = led_count;
                CAN_Send(&TxMessage);
            }

            if(led_count == 30*60) //random fade nach 30min
            {
                leds[0].mode = 5;
                leds[0].std_time = 50;
                leds[0].change_r = (float)((rand()% 5+1))/leds[0].std_time;
                leds[0].change_g = (float)((rand()% 5+1))/leds[0].std_time;
                leds[0].change_b = (float)((rand()% 5+1))/leds[0].std_time;

                leds[1].mode = 1;
                leds[1].master = 0;

                leds[2].mode = 1;
                leds[2].master = 0;

                leds[3].mode = 1;
                leds[3].master = 0;
            }
            else if(led_count == 60*60) //fade to black nach 1h
            {
                leds[0].std_time = 5000;
                leds[0].mode = 3;
                leds[0].time = 0;
                leds[0].target_r = 0;
                leds[0].target_g = 0;
                leds[0].change_b = 0;
            }*/
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
