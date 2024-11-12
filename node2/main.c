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
    if (dutycycle >= 900 && dutycycle <= 2100)
        PWM->PWM_CH_NUM[1].PWM_CDTY = dutycycle;
}

void ADC_Init(){
    //Enable Clock for ADC in PMC
    PMC->PMC_PCER1 |= 1 << (ID_ADC - 32);

    //ADCClock = MCK / ( (PRESCAL+1) * 2 )
    ADC->ADC_MR = ADC_MR_PRESCAL((84/2)-1) | ADC_MR_LOWRES_BITS_12 | ADC_MR_SLEEP_NORMAL | ADC_MR_FWUP_OFF | ADC_MR_FREERUN_ON;
    //PA16 is ADC Channel 7 - Section 9.3.1 (PA16 SPCK1 TD AD7)
    ADC->ADC_CHER = ADC_CHER_CH7;
    ADC->ADC_CR = ADC_CR_START;
}

uint16_t ADC_Read(){
    return ADC->ADC_CDR[7] & 0b111111111111;
}

uint16_t SERVO_ConvertJoystickValToPWM(int8_t x) {
    #define JOYSTICK_MIN -128
    #define JOYSTICK_MAX 127
    #define SERVO_MIN 900
    #define SERVO_MAX 2100

    float scale = (float)(SERVO_MAX - SERVO_MIN)/(float)(JOYSTICK_MAX - JOYSTICK_MIN);
    float offset = SERVO_MIN - (scale * JOYSTICK_MIN);

    return (uint16_t)(scale * x + offset);
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
    ADC_Init();
    
    can_init((CanInit){.brp = 20, .phase1 = 3, .phase2 = 7, .propag = 2, .sjw = 3}, 0);
    
    CanMsg messageTX = {
        .id = 24,
        .length = 1,
        .byte[0] = 99,
    };
    CanMsg messageRX = {0};
    int score = 0;

    while (1){
        // printf("here\n");
        // delay(10000);
        // printf("Sending CAN Pckt\n");
        // can_tx(messageTX);
        if(can_rx(&messageRX) == 1){
            int8_t data = (int8_t) messageRX.byte[0];
            uint16_t j = SERVO_ConvertJoystickValToPWM(data);
            printf("Byte[0]: %d, Byte[1]: %d Servo: %d\n", (int8_t) messageRX.byte[0], (int8_t) messageRX.byte[1], j);
            PWM_SetDutyCycle(j);
        };
        if(ADC_Read() < 800){
            score++;
            printf("Score Increased to: %d!!\n", score);
            delay(5000);
        }

    }
    
}