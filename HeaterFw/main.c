/*
 * main.c
 *
 *  Created on: Jul 9, 2017
 *      Author: ujagaga
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include "DS18B20.h"
#include "dbg_putchar.h"
#include "hw_cfg.h"

#define MOTOR_TICK_COUNT		(60)
#define MOTOR_PULSE_COUNT		(30)
#define TMR_START()				do{ TCCR0B = (1 << CS01); }while(0) // Prescale by 8
#define TMR_STOP()				do{ TCCR0B = 0; }while(0)
#define MOTOR_OFF()				do{ PORTB &= ~((1 << LED_0_PIN) | (1 << LED_1_PIN) | (1 << LED_2_PIN) | (1 << LED_3_PIN)); }while(0)
#define MOTOR_SET(v)			do{ PORTB |= (v); }while(0)

const uint8_t motor_sequence[] = {
		(1 << LED_0_PIN),
		0,
		(1 << LED_1_PIN),
		0,
		(1 << LED_2_PIN),
		0,
		(1 << LED_3_PIN),
		0
};

void motor_advance( void );


volatile uint8_t motor_counter = 0;
volatile uint16_t tmr_count;


ISR(TIMER0_OVF_vect)
{
	tmr_count++;

	if(tmr_count == MOTOR_PULSE_COUNT){
		MOTOR_OFF();
	}

	if(tmr_count == MOTOR_TICK_COUNT){
		tmr_count = 0;
		motor_advance();
	}
}

void motor_advance( void ){
	uint8_t newValue = motor_sequence[motor_counter];
//	PORTB |= newValue;
	MOTOR_SET(newValue);

	motor_counter--;
//	if(motor_counter == sizeof(motor_sequence)){
//		motor_counter = 0;
//	}
	motor_counter &= 0b111;
}

void HwInit( void )
{
	PORTB = 0;					// is the default, but just to make sure and not rely on the bootloader
	DDRB = (1 << LED_0_PIN) | (1 << LED_1_PIN) | (1 << LED_2_PIN) | (1 << LED_3_PIN);
	dbg_tx_init();

	TIFR |= 0x01;       		// clear interrupt flag
	TCCR0A = 0;      			// Normal mode
	TIMSK = (1 << TOIE0);       // TC0 enable overflow interrupt
	TMR_START();

	sei();			// Enable interrupts
}

int main( void )
{

	HwInit();

	dbg_printStr("\n\rPocetak aplikacije\nPocetak\n\r");
	while(true){
		dbg_putchar('.');
		_delay_ms(500);
	}

	return 0;
}
