
#ifndef __LED_PWM_H__
#define __LED_PWM_H__


#include "stm32f10x.h"

#define PWM_STEPS 2048

struct light_state_t {
    char state;
    uint16_t *pwm;
    uint32_t last_changed;
};

typedef struct light_state_t Light_State;

extern Light_State lights[8];

void init_timer(void);
void Light_pwm_init(void);
void update_PWM(void);

#endif
