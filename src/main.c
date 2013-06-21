
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "io-helper.h" //dirks button+led func

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

