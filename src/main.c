
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "io-helper.h" //dirks button+led func
#include "uart.h"

#include "timer.h"
#include "io_pwm.h"

#include <stdio.h>

#include "can.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define UI32_DELAY_TIME 0x1FFFFF


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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


void eltakoAnalyzeFrame(uint8_t* puiFrame){
    static uint32_t ledState = 0;
    if(ledState == 0){
        ledState = 1;
        //LED_On(2);
    }
    else{
        ledState = 0;
        //LED_Off(2);
    }
}


/**
* @brief  Main program.
* @param  None
* @retval None
*/
int main(void)
{

    uint8_t ui8EltakoOrg = 0;
    uint32_t ui32EltakoId = 0;
    uint8_t pui8EltakoData[9] = {0};
    uint8_t ui8EltakoSwitchPos = 0;
    uint32_t ui32i;
    uint32_t ui32ButtonStates[4] = {0};

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
    //send_led(0);
    // initailize UART1 and enable its interrupts
    uart_init();

    struct timer sec_timer;
    timer_set(&sec_timer, TICKS_PER_SECOND/10); //1x pro sec wird gesynced
    led_count = 0;

    init_timer();

    LED_Off(1);   // green can sync led
    LED_Off(2);   // green general purpose led
    LED_On(3);    // red power led


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
        
        
        if(ui32EltakoFrameReady){
            ui8EltakoOrg = pui8EltakoFrame[1];
            ui32EltakoId = *((uint32_t*)&pui8EltakoFrame[6]);
            for(ui32i=2;ui32i<6;ui32i++)
                pui8EltakoData[ui32i-2] = pui8EltakoFrame[ui32i];
            ui32EltakoFrameReady = 0;
            
            if(ui8EltakoOrg == 0x05){
                ui8EltakoSwitchPos = pui8EltakoData[0]>>4;
                //send_led(0);
                // links unten (U2)
                if(ui8EltakoSwitchPos == 0b00000001){
                    ui32ButtonStates[0]++;
                    if(ui32ButtonStates[0]>3)
                        ui32ButtonStates[0] = 0;
                        //send_led(0);
                        
                    if(!ui32ButtonStates[0]){
                        lights[7].state = 0;
                        lights[6].state = 0;
                        //send_led(0);
                    }
                    else if(ui32ButtonStates[0]==1){
                        lights[7].state = 0;
                        lights[6].state = 3;
                        send_led(0);
                    }
                    else if(ui32ButtonStates[0]==2){
                        lights[7].state = 3;
                        lights[6].state = 3;
                        send_led(0);
                    }
                    else if(ui32ButtonStates[0]==3){
                        lights[7].state = 3;
                        lights[6].state = 0;
                        //send_led(1);
                    }
                    LED_On(2);
                }
                // links oben (U3)
                else if(ui8EltakoSwitchPos == 0b00000011){
                    ui32ButtonStates[1]++;
                    if(ui32ButtonStates[1]>1)
                        ui32ButtonStates[1] = 0;
                        
                    if(!ui32ButtonStates[1]){
                        lights[5].state = 0;
                    }
                    else if(ui32ButtonStates[1]==1){
                        lights[5].state = 3;
                    }
                    LED_On(2);
                }
                // rechts unten (U4)
                else if(ui8EltakoSwitchPos == 0b00000101){
                    ui32ButtonStates[2]++;
                    if(ui32ButtonStates[2]>1)
                        ui32ButtonStates[2] = 0;
                        
                    if(!ui32ButtonStates[2]){
                        lights[2].state = 0;
                    }
                    else if(ui32ButtonStates[2]==1){
                        lights[2].state = 3;
                    }
                    LED_On(2);
                }
                // rechts oben (U5)
                else if(ui8EltakoSwitchPos == 0b00000111){
                    ui32ButtonStates[3]++;
                    if(ui32ButtonStates[3]>3)
                        ui32ButtonStates[3] = 0;
                        
                    if(!ui32ButtonStates[3]){
                        lights[3].state = 0;
                        lights[4].state = 0;
                    }
                    else if(ui32ButtonStates[3]==1){
                        lights[3].state = 0;
                        lights[4].state = 3;
                    }
                    else if(ui32ButtonStates[3]==2){
                        lights[3].state = 3;
                        lights[4].state = 3;
                    }
                    else if(ui32ButtonStates[3]==3){
                        lights[3].state = 3;
                        lights[4].state = 0;
                    }
                    LED_On(2);
                }
                // loslassen
                else if(ui8EltakoSwitchPos<<7 == 0b00000000){
                    LED_Off(2);
                }
            }
        }
            
        
        
        
        if(timer_expired(&sec_timer))
        {
            timer_reset(&sec_timer);
            
/*            send_pong_simple();*/

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

