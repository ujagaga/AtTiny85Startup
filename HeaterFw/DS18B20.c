/*
 * DS18B20.c
 *
 *  Created on: Oct 23, 2016
 *      Author: ujagaga
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "DS18B20.h"

//#define COMM_PIN		(PB0)
#define COMM_PIN_MASK	(1 << DS_COMM_PIN)

#define pinOut()		do{ DDRB |= COMM_PIN_MASK;}while(0)
#define pinIn()			do{ DDRB &= ~COMM_PIN_MASK;}while(0)
#define pinSet()		do{ PORTB |= COMM_PIN_MASK;}while(0)
#define pinClr()		do{ PORTB &= ~COMM_PIN_MASK;}while(0)
#define pinRead()		((PINB & COMM_PIN_MASK) == COMM_PIN_MASK)

#define SKIP_ROM_MSG 		(0xCC)
#define CONVERT_T_MSG		(0x44)
#define READ_SCRATCHPAD_MSG (0xBE)



bool checkPresence( void )
{
	bool isPresent;

	pinClr();
	pinOut();
	_delay_us(480);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		pinIn();
		_delay_us(64);

		isPresent = pinRead();
	}

	_delay_us(480);

	return !isPresent;
}

void writeBit( bool value )
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		pinClr();
		pinOut();

		_delay_us(2);

		if(value){
			pinIn();
			_delay_us(60);
		}else{
			_delay_us(60);
			pinIn();
		}
	}

}

void sendByte(uint8_t data)
{
	uint8_t i;

	for(i = 0; i < 8; i++){
		writeBit((((data >> i) & 1) == 1));
	}

}

bool readBit( void )
{
	bool result;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		pinClr();
		pinOut();

		_delay_us(2);
		pinIn();
		_delay_us(10);
		result = pinRead();
	}

	_delay_us(60-10);
	return result;
}

uint8_t readByte( void )
{
	uint8_t i, result = 0;

	for(i = 0; i < 8; i++)
	{
		if(readBit())
		{
			result |= 1 << i;
		}
	}

	return result;
}

uint8_t DS_readTemp( void )
{

	_delay_ms(100);
	if(!checkPresence())
	{
		return DS_ERR;
	}

	sendByte(SKIP_ROM_MSG);
	sendByte(CONVERT_T_MSG);

	/* Extra power for temp conversion */
	pinSet();
	pinOut();

	_delay_ms(1500);

	pinIn();

	if(!checkPresence())
	{
		return DS_ERR;
	}

	sendByte(SKIP_ROM_MSG);
	sendByte(READ_SCRATCHPAD_MSG);

	uint16_t result = readByte();

	result |= (readByte() << 8);
	result = (result & 0x7FF);
	result /= 16;

	if((result < DS_MIN) || (result > DS_MAX) ){
		return DS_ERR;
	}

	return (int8_t)result;
}




