#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#include "TimerCTC.h"

void TimerCTC_Init(void)
{
	TCCR1B |= (1<<CS12)|(1<<CS10); //clk/1024 (From prescaler) => 16 000 000 / 1024 = 15 625 Hz
	TCCR1B |= (1<<WGM12); // CTC mode
	OCR1A = 15625; // Interrupt from comparing the contents of the Timer's hardware counter with the OCR1A register

	//Enable TIMER1 interrupt and general interrupts
	TIMSK1 |= (1<<OCIE0A);	//Enable TIMER1_COMPA
	sei (); // Enable interrupts
}