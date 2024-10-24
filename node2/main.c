#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "can.h"
#include "time.h"

#define F_CPU 84000000

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Initialising CAN\r\n");
    can_init((CanInit){.brp = F_CPU/2000000-1, .phase1 = 5, .phase2 = 1, .propag = 6}, 0);
    
    CanMsg message = {
        .id = 24,
        .length = 1,
        .byte[0] = 99,
    };

    while (1){
        can_tx(message);
    }
    
}