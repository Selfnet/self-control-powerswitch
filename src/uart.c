#define USART1_GPIO              GPIOA
#define USART1_CLK               RCC_APB2Periph_USART1
#define USART1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART1_RxPin             GPIO_Pin_10
#define USART1_TxPin             GPIO_Pin_9
#define USART1_IRQn              USART1_IRQn
#define USART1_IRQHandler        USART1_IRQHandler

#include "stm32f10x.h"
#include "includes.h"

#include "uart.h"
#include "io-helper.h"
#include "can.h"

uint32_t ui32UartRxBufPos = 0;
uint32_t ui32EltakoSyncPending = 0;
uint32_t ui32EltakoSynced = 0;
uint32_t ui32EltakoCount = 0;
uint32_t ui32EltakoFrameReady = 0;
uint32_t ui32i = 0;
uint8_t ui8EltakoByte = 0;
uint8_t pui8EltakoBuf[16] = {0};
uint8_t pui8EltakoFrame[16] = {0};    



void uart_init(void){

  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_InitTypeDef USART_InitStructure;

/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USART1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USART1 Clock */
  RCC_APB2PeriphClockCmd(USART1_CLK, ENABLE); 



  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USART1_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USART1_GPIO, &GPIO_InitStructure);


  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);



/* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx;

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);
  
  /* Enable USART1 Receive interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
  
  
  
}
  

  /******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    uint8_t incomingByte = 0;
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
    }
    /* Read one byte from the receive data register */
    ui8EltakoByte = USART_ReceiveData(USART1);
    send_enocean(pui8EltakoBuf);
    pui8EltakoBuf[ui32EltakoCount] = ui8EltakoByte;
    ui32EltakoCount++;
    if(ui32EltakoCount>6){
        ui32EltakoCount = 0;
        LED_On(2);
        //send_enocean(pui8EltakoBuf);
    }
    
    //LED_Toggle(2);
    
    if(ui8EltakoByte == 0xA5){
        ui32EltakoSyncPending = 1;
        //LED_On(2);
    }
    else if(ui8EltakoByte == 0x5A && ui32EltakoSyncPending == 1){
        ui32EltakoSynced = 1;
        ui32EltakoCount = 0;
        ui32EltakoSyncPending = 0;
        //LED_On(2);
    }
    else{
        ui32EltakoSyncPending = 0;
    }

    if(ui32EltakoSynced){
        pui8EltakoBuf[ui32EltakoCount] = ui8EltakoByte;
        ui32EltakoCount++;
        if(ui32EltakoCount>10){
            ui32EltakoSynced = 0;
            ui32EltakoCount = 0;
            if(ui32EltakoFrameReady == 0){
                for(ui32i=1; ui32i<=10; ui32i++)
                    pui8EltakoFrame[ui32i-1] = pui8EltakoBuf[ui32i];
                ui32EltakoFrameReady = 1;
                
            }
        }
    } 
}
  
  
  
