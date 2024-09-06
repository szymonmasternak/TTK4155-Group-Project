#include "config.h"
#include "hal_uart.h"

#include <avr/io.h>
#include <util/delay.h>

void HAL_UART0_Init(uint32_t baudrate){
    uint8_t ubrr = ((F_CPU / (16 * baudrate))-1);
    
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    // Enable RX/TX
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    // Setup Format of Uart: 8 data bits, No Parity Bits, 1 stop bit
    UCSR0C = (1<<URSEL0) | (0<<USBS0) | (3<<UCSZ00);
}

void HAL_UART0_SendChar(char data){
    while (!(UCSR0A & (1 << UDRE0))){
    };
    UDR0 = data;
}

char HAL_UART0_ReceiveChar(void){
    while (!(UCSR0A & (1 << RXC0))){
    };
    return UDR0;
}
