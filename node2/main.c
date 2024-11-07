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

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Initialising CAN\r\n");

    can_init((CanInit){.brp = 20, .phase1 = 3, .phase2 = 7, .propag = 2, .sjw = 3}, 0);
    
    CanMsg messageTX = {
        .id = 24,
        .length = 1,
        .byte[0] = 99,
    };
    CanMsg messageRX = {0};

    while (1){
        printf("here\n");
        delay(10000);
        printf("Sending CAN Pckt\n");
        can_tx(messageTX);
        if(can_rx(&messageRX) == 1){
            printf("Received message!\n");
            can_printmsg(messageRX);
        };
    }
    
}