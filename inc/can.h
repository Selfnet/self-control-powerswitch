
#ifndef __CAN_H__
#define __CAN_H__

#define CAN_ID 0x80

#include "stm32f10x.h"

// Can functions
void CAN_config(void);
void prozess_can_it(void);

void CAN_Send(CanTxMsg *TxMessage);

CanTxMsg* can_puffer[10];
char can_puffer_cnt;


#define getSender(ExtId)    ( ( (ExtId)     & 0xff ) )
#define getRecipient(ExtId) ( ( (ExtId>>8 ) & 0xff ) )
#define getTyp(ExtId)       ( ( (ExtId>>16) & 0xff ) )

#define setSender(X)        ( ( (X)         & 0xff ) )
#define setRecipient(X)     ( ( (X<<8)      & 0xff00 ) )
#define setType(X)          ( ( (X<<16)     & 0xff0000 ) )

// ID of LED-Controller
#define LED_CAN_ID 0x40

//NODE_CAN_ID = 64 = 0x40 = 0b1000000
#define NODE_CAN_ID 0x80
#define NODE_CAN_BROADCAST 0xFF

#define CAN_PROTO_SYNC  0x0A
#define CAN_PROTO_PING  0x08
#define CAN_PROTO_PONG  0x09
#define CAN_PROTO_LIGHT 0xD0

#define CAN_PROTO_LIGHT_TOGGLE  0x8

// ext id =  0b10 100  0*24
#define CAN_EXT_ID 0x14000000

//counter für led timer
volatile int led_count;

#endif
