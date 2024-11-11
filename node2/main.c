#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "can.h"
#include "time.h"

#define F_CPU 84000000

void delay(unsigned int d){
    for(volatile unsigned int i=0; i<1000; i++){
        for(volatile unsigned int j=0; j<d; j++){

        }
    }
}

void PWM_Init(void) {
    //PWM PWMH1 PB13 B

    //Enable Clock for PWM in PMC
    PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

    //Connect PB13 to PWM Peripheral and disconnect from CPU
    PIOB->PIO_PDR |= PIO_PDR_P13;
    PIOB->PIO_ABSR |= PIO_ABSR_P13;
    
    //Section 38.6.1 of Datasheet
    //Use ClockA as that allows us to have a perfect clock of 1Mhz from 84Mhz
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);

    //Use CLOCKA Input source and reverse the polarity of clock
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    
    //Set Period to 20ms and Duty Cycle of 1.5ms
    PWM->PWM_CH_NUM[1].PWM_CPRD = 20000; //20.0ms
    PWM->PWM_CH_NUM[1].PWM_CDTY = 1200; //1.2ms
    
    //PWM Enable
    PWM->PWM_ENA = (1 << 1);
}

void PWM_SetDutyCycle(uint32_t dutycycle){
    if (dutycycle >= 900 || dutycycle <= 2100)
        PWM->PWM_CH_NUM[1].PWM_CDTY = dutycycle;
}

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Initialising CAN\r\n");

    PWM_Init();
    PWM_SetDutyCycle(1500);
    
    can_init((CanInit){.brp = 20, .phase1 = 3, .phase2 = 7, .propag = 2, .sjw = 3}, 0);
    
    CanMsg messageTX = {
        .id = 24,
        .length = 1,
        .byte[0] = 99,
    };
    CanMsg messageRX = {0};

    uint16_t i = 900;
    while (1){
        printf("here\n");
        delay(10000);
        printf("Sending CAN Pckt\n");
        can_tx(messageTX);
        if(can_rx(&messageRX) == 1){
            printf("Received message!\n");
            can_printmsg(messageRX);
        };
        PWM_SetDutyCycle(i);
        i = i + 100;
        if(i > 2100)
            i = 900;
    }
    
}