/* Define to prevent recursive inclusion */
#ifndef __UART_H
#define __UART_H

#define UartRxBufferSize 30

extern uint32_t ui32EltakoFrameReady;
extern uint8_t pui8EltakoFrame[]; 

void uart_init(void);
void USART1_IRQHandler(void);
void send_led(uint32_t onoff);

#endif /* __UART_H */
