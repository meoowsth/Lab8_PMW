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

enum States{Start, Wait, Play} State;

unsigned char boolTracker = 0x00;
unsigned char tempA;
double octect[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
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
			if (tempA == 0x01){
				if (boolTracker == 0x00){
			  		boolTracker = 0x01;
				}
				else if (boolTracker == 0x01){
			  		boolTracker = 0x00;
		       		}
       			}
			State = (boolTracker)? Play: Wait;
		     	break;
		case Play:
			if (tempA == 0x01){
				if (boolTracker == 0x00){
			  		boolTracker = 0x01;
				}
				else if (boolTracker == 0x01){
			  		boolTracker = 0x00;
		       		}
       			}
			State = (boolTracker)? Play: Wait;
		     	break;
	    	default:
	      		break;
	}
	switch(State) {//Actions
		case Start:
			break;
	   	case Wait:
			if (tempA == 0x02){
				if ( i < 7){
					i++;
				}
			}
			else if (tempA == 0x04){
				if ( i > 0 ){
					i--;
				}
			}
			set_PWM(0);
			break;
		case Play:
			if (tempA == 0x02){
				if ( i < 7){
					i++;
				}
			}
			else if (tempA == 0x04){
				if ( i > 0 ){
					i--;
				}
			}
			set_PWM(octect[i]);
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
