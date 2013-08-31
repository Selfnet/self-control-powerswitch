
#include "stm32f10x.h"

#include "can.h"
#include "io-helper.h"
#include "io_pwm.h"

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN_config(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    CAN_InitTypeDef         CAN_InitStructure;
    CAN_FilterInitTypeDef   CAN_FilterInitStructure;

    // GPIO clock enable 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Configure CAN pin: RX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure CAN pin: TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // CANx Periph clock enable 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // CAN register init 
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);

    // CAN cell init 
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

    /* CAN Baudrate = 1MBps*/
    //  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    //  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    //  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    //  CAN_InitStructure.CAN_Prescaler = 4;
    //  CAN_Init(CAN1, &CAN_InitStructure);

    // Baudrate = 125kbps
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;
    CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler=48;
    if(CAN_Init(CAN1, &CAN_InitStructure) == CAN_InitStatus_Success){
/*        LED_On(1);*/
/*        LED_On(0);*/
    }
    else{
/*        LED_Off(1);*/
/*        LED_Off(0);*/
    }
    
/*    send_pong_simple();*/
/*    send_pong_simple();*/
/*    send_pong_simple();*/
        

    // CAN filter init 
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);


    // Enable the CAN RX Interrupt
    NVIC_InitTypeDef NVIC_InitStructure_CAN;
    NVIC_InitStructure_CAN.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure_CAN.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure_CAN.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure_CAN.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure_CAN);

    //  Enable CAN Interrupt
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


void CAN_Send(CanTxMsg *TxMessage)
{
    ;
    if(CAN_Transmit(CAN1, TxMessage) == CAN_TxStatus_NoMailBox && can_puffer_cnt < 10)
        can_puffer[++can_puffer_cnt] = TxMessage;
}

void send_led(uint32_t onoff){
    static CanTxMsg TxMessage;
    TxMessage.IDE = CAN_ID_EXT;                                 //immer extended can frames
    TxMessage.ExtId = CAN_EXT_ID;                               //default ID setzen
    TxMessage.ExtId |= setSender( NODE_CAN_ID );
    TxMessage.ExtId |= setType( CAN_PROTO_PONG );
    TxMessage.ExtId |= setRecipient( LED_CAN_ID );
    TxMessage.RTR = CAN_RTR_Data;                               // daten senden
    
    if(onoff){
        TxMessage.DLC = 4;
        TxMessage.Data[0] = 0b00001111; // All Leds RGB mode
        TxMessage.Data[1] = 0x05; //Sollte auto fade mode sein
        TxMessage.Data[2] = 0x00;
        TxMessage.Data[3] = 50;
    }
    else{
        TxMessage.DLC = 7;
        TxMessage.Data[0] = 0b00001111; // All Leds RGB mode
        TxMessage.Data[1] = 0x02; // ausschalten, alle farben auf 0
        TxMessage.Data[2] = 0x00;
        TxMessage.Data[3] = 0x00;
        TxMessage.Data[4] = 0x00;
        TxMessage.Data[5] = 0x00;
        TxMessage.Data[6] = 0x00;
    }
    CAN_Send(&TxMessage);
    //CAN_Transmit(CAN1, &TxMessage);
}

void send_pong(CanRxMsg RxMessage)
{
    //ping request
    if( getTyp(RxMessage.ExtId) == CAN_PROTO_PING )
    {
        CanTxMsg TxMessage;
        TxMessage.IDE = CAN_ID_EXT;                                 //immer extended can frames
        TxMessage.ExtId = CAN_EXT_ID;                               //default ID setzen
        TxMessage.ExtId |= setSender( NODE_CAN_ID );
        TxMessage.ExtId |= setType( CAN_PROTO_PONG );
        TxMessage.ExtId |= setRecipient( getSender(RxMessage.ExtId) );
        TxMessage.RTR = CAN_RTR_Data;                               // daten senden
        
        // alle empfangen daten zurueckschicken
        TxMessage.DLC = RxMessage.DLC;
        int i;
        for(i = 0 ; i < RxMessage.DLC ; i++)
        {
            TxMessage.Data[i] = RxMessage.Data[i];
        }
        CAN_Send(&TxMessage);
    }
}

void send_state(CanRxMsg RxMessage)
{
    CanTxMsg TxMessage;
    TxMessage.IDE = CAN_ID_EXT;                                 //immer extended can frames
    TxMessage.ExtId = CAN_EXT_ID;                               //default ID setzen
    TxMessage.ExtId |= setSender( NODE_CAN_ID );
    TxMessage.ExtId |= setType( CAN_PROTO_LIGHT );
    TxMessage.ExtId |= setRecipient( getSender(RxMessage.ExtId) );
    TxMessage.RTR = CAN_RTR_Data;

    TxMessage.DLC = 8;
    //state of all light
    for(int i = 0 ; i < TxMessage.DLC ; i++)
        TxMessage.Data[i] = lights[i].state;
    CAN_Send(&TxMessage);
}


void prozess_can_it(void)
{
    CanRxMsg RxMessage;
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

    if(RxMessage.IDE == CAN_Id_Standard)
    {
        //einfach ignorieren, vllt spaeter auch noch was machen...
    }
    else //das was wir wollen , Extended CanRxMsg
    {
        //msg ist an mich ( board ID )
        if( getRecipient(RxMessage.ExtId) == NODE_CAN_ID || getRecipient(RxMessage.ExtId) == NODE_CAN_BROADCAST )
        {
            //PING
            if( getTyp(RxMessage.ExtId) == CAN_PROTO_PING ){
                LED_Toggle(2);
                send_pong(RxMessage);
                }
            //SYNC
            else if( getTyp(RxMessage.ExtId) == CAN_PROTO_SYNC ){
/*                send_led(0);*/
                if(RxMessage.Data[0] == 0)
                {
                    LED_Off(1);
                    LED_Off(0);
                    send_led(0);
                }
                else if(RxMessage.Data[0] == 1)
                {
                    LED_On(1);
                    LED_On(0);
                    send_led(1);
                }
                else
                {
                    LED_Toggle(1);
                    LED_Toggle(0);
                }
            }
            //Light
            else if( getTyp(RxMessage.ExtId) == CAN_PROTO_LIGHT )
            {
                if(RxMessage.Data[1] == 0xFF) //get the current state of the lights
                {
                    send_state(RxMessage);
                }
                else //set light
                {
                    //Data[0] = Relay  ( bit position bestimmt relay)
                    //Data[1] = Status ( an/aus )
                    if( 0 <= RxMessage.Data[0] && RxMessage.Data[0] <= 255 )
                    {
                        for(int i = 0 ; i <= 7 ; i++)
                            if( ((RxMessage.Data[0] >> i) & 1) == 1 )
                                // schaltet lampe an oder aus, oder toggelt
                                lights[ i ].state = RxMessage.Data[1] == CAN_PROTO_LIGHT_TOGGLE ? ( lights[ i ].state == 0 ? 3 : 0 ) : RxMessage.Data[1]*3;
                    }
                }
            }
        } //end an mich | broadcast
    } //end else extended
}


