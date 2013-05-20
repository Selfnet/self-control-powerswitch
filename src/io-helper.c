
#include "io-helper.h"


/**
  * @brief  Configures Button NVIC and EXTI Line.
  * @retval None
  */
void button_interrupt_init(void)
{
    // *** Button1 ***
    //EXTI structure to init EXT
    EXTI_InitTypeDef EXTI_InitStructure;
    //Connect EXTI Line to Button Pin
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //PortA.0 --> Btn1
    //Configure Button EXTI line
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    //select interrupt mode
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //generate interrupt on rising edge
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    //enable EXTI line
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    //send values to registers
    EXTI_Init(&EXTI_InitStructure);

    //configure NVIC
    //NVIC structure to set up NVIC controller
    NVIC_InitTypeDef NVIC_InitStructure;
    //select NVIC channel to configure
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    //set priority to lowest
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    //set subpriority to lowest
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    //enable IRQ channel
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //update NVIC registers
    NVIC_Init(&NVIC_InitStructure);


    // *** Pin für Licht-Relay configurieren ****
//    GPIO_InitTypeDef  GPIO_InitStructure;
    /* Enable the Licht-Relay-Bin Clock */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* Configure the Licht-Relay-Bin */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  Configures Button GPIO
  * @retval None
  */
void button_init(void)
{
    //Button1
    GPIO_InitTypeDef GPIO_InitStructure_Button1;

    /* Enable the BUTTON Clock */
    RCC_APB2PeriphClockCmd(BUTTON_1_CLK | RCC_APB2Periph_AFIO, ENABLE);

    /* Configure Button pin as input floating */
    GPIO_InitStructure_Button1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure_Button1.GPIO_Pin = BUTTON_1_PIN;
    GPIO_Init( BUTTON_1_PORT , &GPIO_InitStructure_Button1);

    //enables interrupts
    button_interrupt_init();
}


/**
  * @brief  Returns the selected Button state.
  * @retval The Button GPIO pin value.
  */
uint32_t Button_GetState(uint32_t btnnr)
{
    if(btnnr == 1)
        return GPIO_ReadInputDataBit(BUTTON_1_PORT, BUTTON_1_PIN); //pullup
    return 0;
}



void Light_init(void)
{
    {
    //** LIGHT_U2_1 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U2_1_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U2_1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U2_1_PORT, &GPIO_InitStructure);
    }
    {
    //** LIGHT_U2_2 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U2_2_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U2_2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U2_2_PORT, &GPIO_InitStructure);
    }



    {
    //** LIGHT_U3_1 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U3_1_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U3_1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U3_1_PORT, &GPIO_InitStructure);
    }
    {
    //** LIGHT_U3_2 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U3_2_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U3_2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U3_2_PORT, &GPIO_InitStructure);
    }




    {
    //** LIGHT_U4_1 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U4_1_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U4_1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U4_1_PORT, &GPIO_InitStructure);
    }
    {
    //** LIGHT_U4_2 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U4_2_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U4_2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U4_2_PORT, &GPIO_InitStructure);
    }



    {
    //** LIGHT_U5_1 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U5_1_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U5_1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U5_1_PORT, &GPIO_InitStructure);
    }
    {
    //** LIGHT_U5_2 **
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the LIGHT Clock */
    RCC_APB2PeriphClockCmd(LIGHT_U5_2_CLK, ENABLE);

    /* Configure the LIGHT pin */
    GPIO_InitStructure.GPIO_Pin = LIGHT_U5_2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LIGHT_U5_2_PORT, &GPIO_InitStructure);
    }
    
    //alles auf 0
    lights[0].state = 0;
    lights[0].last_changed = 0;
    //...
}

void Light_On(char roomnr, char lightnr)
{
    //if(lights[ (roomnr-2)*2+lightnr-1 ].last_changed + 30 < clock_time()/100)
    //    return; //wenn die lampe in den letzten 30 sek schonmal geschaltet wurde einfach nichts tun

    lights[ (roomnr-2)*2+lightnr-1 ].last_changed = clock_time()/100;
    lights[ (roomnr-2)*2+lightnr-1 ].state = 1;

    /*
    TODO
    Data[0] = Raum:
                        0b0001 = U2
                        0b0010 = U3
                        0b0100 = U4
                        0b1000 = U5
    Data[1] = Light:
                        0b01 = Light1
                        0b10 = Light2
    Data[2] = On/Off
                        0b1  = On
                        0b0  = Off
    */

    if(roomnr == 2)
    {
        if( lightnr == 1)
            LIGHT_U2_1_PORT->BSRR = LIGHT_U2_1_PIN;
        else if( lightnr == 2)
            LIGHT_U2_2_PORT->BSRR = LIGHT_U2_2_PIN;
    }
    if(roomnr == 3)
    {
        if( lightnr == 1)
            LIGHT_U3_1_PORT->BSRR = LIGHT_U3_1_PIN;
        else if( lightnr == 2)
            LIGHT_U3_2_PORT->BSRR = LIGHT_U3_2_PIN;
    }
    if(roomnr == 4)
    {
        if( lightnr == 1)
            LIGHT_U4_1_PORT->BSRR = LIGHT_U4_1_PIN;
        else if( lightnr == 2)
            LIGHT_U4_2_PORT->BSRR = LIGHT_U4_2_PIN;
    }
    if(roomnr == 5)
    {
        if( lightnr == 1)
            LIGHT_U5_1_PORT->BSRR = LIGHT_U5_1_PIN;
        else if( lightnr == 2)
            LIGHT_U5_2_PORT->BSRR = LIGHT_U5_2_PIN;
    }
/*

    if(roomnr == 2)
    {
        if( lightnr == 1)
            LIGHT_U2_1_PORT->BRR = LIGHT_U2_1_PIN;
        else if( lightnr == 2)
            LIGHT_U2_2_PORT->BRR = LIGHT_U2_2_PIN;
    }
    if(roomnr == 3)
    {
        if( lightnr == 1)
            LIGHT_U3_1_PORT->BRR = LIGHT_U3_1_PIN;
        else if( lightnr == 2)
            LIGHT_U3_2_PORT->BRR = LIGHT_U3_2_PIN;
    }
    if(roomnr == 4)
    {
        if( lightnr == 1)
            LIGHT_U4_1_PORT->BRR = LIGHT_U4_1_PIN;
        else if( lightnr == 2)
            LIGHT_U4_2_PORT->BRR = LIGHT_U4_2_PIN;
    }
    if(roomnr == 5)
    {
        if( lightnr == 1)
            LIGHT_U5_1_PORT->BRR = LIGHT_U5_1_PIN;
        else if( lightnr == 2)
            LIGHT_U5_2_PORT->BRR = LIGHT_U5_2_PIN;
    }*/
}


void Light_Off(char roomnr, char lightnr)
{
    //if(lights[ (roomnr-2)*2+lightnr-1 ].last_changed + 30 < clock_time()/100)
    //    return; //wenn die lampe in den letzten 30 sek schonmal geschaltet wurde einfach nichts tun

    lights[ (roomnr-2)*2+lightnr-1 ].last_changed = clock_time()/100;
    lights[ (roomnr-2)*2+lightnr-1 ].state = 0;


    if(roomnr == 2)
    {
        if( lightnr == 1)
            LIGHT_U2_1_PORT->BRR = LIGHT_U2_1_PIN;
        else if( lightnr == 2)
            LIGHT_U2_2_PORT->BRR = LIGHT_U2_2_PIN;
    }
    if(roomnr == 3)
    {
        if( lightnr == 1)
            LIGHT_U3_1_PORT->BRR = LIGHT_U3_1_PIN;
        else if( lightnr == 2)
            LIGHT_U3_2_PORT->BRR = LIGHT_U3_2_PIN;
    }
    if(roomnr == 4)
    {
        if( lightnr == 1)
            LIGHT_U4_1_PORT->BRR = LIGHT_U4_1_PIN;
        else if( lightnr == 2)
            LIGHT_U4_2_PORT->BRR = LIGHT_U4_2_PIN;
    }
    if(roomnr == 5)
    {
        if( lightnr == 1)
            LIGHT_U5_1_PORT->BRR = LIGHT_U5_1_PIN;
        else if( lightnr == 2)
            LIGHT_U5_2_PORT->BRR = LIGHT_U5_2_PIN;
    }
}


/**
  * @brief  Configures LED GPIO.
  * @retval None
  */
void LED_init(void)
{
    {
        //** LED0 **
        GPIO_InitTypeDef  GPIO_InitStructure;

        /* Enable the GPIO_LED Clock */
        RCC_APB2PeriphClockCmd(LED0_GPIO_CLK, ENABLE);

        /* Configure the GPIO_LED pin */
        GPIO_InitStructure.GPIO_Pin = LED0_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);
    }

    {
        //** LED1 **
        GPIO_InitTypeDef  GPIO_InitStructure;

        /* Enable the GPIO_LED Clock */
        RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE);

        /* Configure the GPIO_LED pin */
        GPIO_InitStructure.GPIO_Pin = LED1_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
    }
    
    {
        //** LED2 **
        GPIO_InitTypeDef  GPIO_InitStructure;

        /* Enable the GPIO_LED Clock */
        RCC_APB2PeriphClockCmd(LED2_GPIO_CLK, ENABLE);

        /* Configure the GPIO_LED pin */
        GPIO_InitStructure.GPIO_Pin = LED2_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
    }
    
    {
        //** LED3 **
        GPIO_InitTypeDef  GPIO_InitStructure;

        /* Enable the GPIO_LED Clock */
        RCC_APB2PeriphClockCmd(LED3_GPIO_CLK, ENABLE);

        /* Configure the GPIO_LED pin */
        GPIO_InitStructure.GPIO_Pin = LED3_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
    }
}

/**
  * @brief  Turns LEDx on.
  * @retval None
  */
void LED_On(uint32_t lednr)
{
    if(lednr == 0)
        LED0_GPIO_PORT->BRR = LED0_PIN;

    if(lednr == 1)
        LED1_GPIO_PORT->BSRR = LED1_PIN;
    if(lednr == 2)
        LED2_GPIO_PORT->BSRR = LED2_PIN;
    if(lednr == 3)
        LED3_GPIO_PORT->BSRR = LED3_PIN;
}


/**
  * @brief  Turns LEDx on.
  * @retval None
  */
void LED_Off(uint32_t lednr)
{
    if(lednr == 0)
        LED0_GPIO_PORT->BSRR = LED0_PIN;

    if(lednr == 1)
        LED1_GPIO_PORT->BRR = LED1_PIN;
    if(lednr == 2)
        LED2_GPIO_PORT->BRR = LED2_PIN;
    if(lednr == 3)
        LED3_GPIO_PORT->BRR = LED3_PIN;
}


/**
  * @brief  Turns LEDx on.
  * @retval None
  */
void LED_Toggle(uint32_t lednr)
{
    if(lednr == 0)
        LED0_GPIO_PORT->ODR ^= LED0_PIN;

    if(lednr == 1)
        LED1_GPIO_PORT->ODR ^= LED1_PIN;
    if(lednr == 2)
        LED2_GPIO_PORT->ODR ^= LED2_PIN;
    if(lednr == 3)
        LED3_GPIO_PORT->ODR ^= LED3_PIN;
}

