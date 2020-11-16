/*	Author: Tinghui Song
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "Timer.h"

enum States{Start, Wait, A0_P, A0_R} State;

//unsigned char boolTracker = 0x00;
unsigned char tempA;
double melody[7] = {261.63, 261.63, 392.00, 392.00, 440.00, 440.00, 392.00};
unsigned char i = 0;

void set_PWM(double frequency){
	static double current_frequency;
	if (frequency != current_frequency){
		if ( !frequency){ TCCR3B &= 0x08;}
		else { TCCR3B |= 0x03;}
		if ( frequency < 0.954) { OCR3A = 0xFFFF;}
		else if ( frequency > 31250) { OCR3A = 0x000;}
		else{ OCR3A = (short)(8000000 / (128 * frequency)) -1;}
	TCNT3 = 0;
	current_frequency = frequency;
	}
}

void PWM_on(){
	TCCR3A = ( 1 << COM3A0);
	TCCR3B = ( 1 << WGM32) | ( 1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void Tick(){
	switch(State){ //Transitions
		case Start:
			State = Wait;
	      		break;
		case Wait:
			State = (tempA == 0x01)? A0_P: Wait;
		     	break;
		case A0_P:
			State = (tempA == 0x01)? A0_P: A0_R;
		     	break;
		case A0_R:
			State = Wait;
			break;
	    	default:
	      		break;
	}
	switch(State) {//Actions
		case Start:
			break;
	   	case Wait:
			set_PWM(0);
			break;
		case A0_P:
			for ( i = 0; i < 7; ++i){
				set_PWM(octect[i]);
			}
			break;
		case A0_R:
			break;
	    default:
	      	break;
  	}	
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	
	TimerSet(200);
 	TimerOn();
	PWM_on();
	State = Start;
	boolTracker = 0x00;
    while (1) {
	tempA = ~PINA;
	while(!TimerFlag);
   	TimerFlag = 0;
	Tick();
    }
    return 1;
}
