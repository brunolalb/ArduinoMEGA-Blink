/*
 * blink.cpp
 *
 *  Created on: 3 de jun de 2016
 *      Author: Bruno Albrecht
 *
 *  Blink a LED on PB7 in an Timer1 interrupt.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	//Setup the clock (timer 1, 62.5kHz (16MHz / 256 = 62.5kHz), CTC mode, interrupt on)
	cli(); //Disable global interrupts
	TCCR1A = 0b00000000; //no physical outputs (COM1Ax, COM1Bx and COM1Cx == 0b00), WGM10 and WGM11 = 0.
	TCCR1B = 0b00001000; //input capture off, WGM13 = 0, WGM12 = 1, prescaler 1/256 (CS1x = 0b100 - off by now, thus 0b000)
	TCCR1C = 0b00000000; //writing a 1 to any of the 3 most significative bits forces a compare match
	TCNT1 = 0x0000; //resets the timer counter
	OCR1A = 62499; //Count 62500 cycles for 1 second interrupt
	OCR1B = 0x0000; //output compare register B is not used
	OCR1C = 0x0000; //output compare register C is not used
	ICR1 = 0x0000; //input capture not used
	TIMSK1 = 0b00000010; //input capture interrupt off, output compare B and C interrupts off, output compare A interrupt on, overflow interrupt off
	TIFR1 = 0x00; //resets the interrupt flags
	TCCR1B |= 0b00000100; //turn on the timer (CS1x = 0b100)
	sei(); //Enable global interrupts

	//Setup the I/O for the LED (digital pin 13, or PB7, according to the Arduino pin mapping)
	DDRB = 0b10000000; //PB7 is digital pin 13 (LED), configured as an output
	PORTB |= 0b10000000; //Set PB7 high to turn on LED

	while(1) { } //Loop forever, interrupts do the rest
}

ISR(TIMER1_COMPA_vect) //Interrupt Service Routine - flag is cleared automatically
{
  PORTB ^= (1<<PB7); //Use xor to toggle the LED
}

