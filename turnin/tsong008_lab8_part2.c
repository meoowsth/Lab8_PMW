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

enum States{Start, Wait, C4, D, E, F, G, A, B, C5} State;

unsigned char boolTracker = 0x00;
unsigned char tempA;

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
	switch(State){ // transitions
		case Start:
			State = C4;
	      		break;
		case C4:
		      if (tempA == 0x02){
			State = D;
		      }
		      else{
			State = C4;
		      }
		      break;
	    	case D:
		      if (tempA == 0x02){
			State = E;
		      }
		      else if (tempA == 0x04){
			State = C4;
		      }
		      else{
			State = D;
		      }
		      break;
	    	case E:
		      if (tempA == 0x02){
			State = F;
		      }
		      else if (tempA == 0x04){
			State = D;
		      }
		      else{
			State = E;
		      }
		      break;
	    	case F:
		      if (tempA == 0x02){
			State = G;
		      }
		      else if (tempA == 0x04){
			State = E;
		      }
		      else{
			State = F;
		      }
		      break;
	    	case G:
		      if (tempA == 0x02){
			State = A;
		      }
		      else if (tempA == 0x04){
			State = F;
		      }
		      else{
			State = G;
		      }
		      break;
		case A:
		      if (tempA == 0x02){
			State = B;
		      }
		      else if (tempA == 0x04){
			State = G;
		      }
		      else{
			State = A;
		      }
		      break;
	    	case B:
		      if (tempA == 0x02){
			State = C5;
		      }
		      else if (tempA == 0x04){
			State = A;
		      }
		      else{
			State = B;
		      }
		      break;
	    	case C5:
		      if (tempA == 0x04){
			State = B;
		      }
		      else{
			State = C5;
		      }
		      break;
	    	default:
	      		break;
	}
	switch(State) {
	    case C4:
		      set_PWM(261.63);
		      break;
	    case D:
		      set_PWM(293.66);
		      break;
	    case E:
		      set_PWM(329.63);
		      break;
	    case F:
		      set_PWM(349.23);
		     break;
	    case G:
		      set_PWM(392.00);
		      break;
	    case A:
		      set_PWM(440.00);
		      break;
	    case B:
		      set_PWM(493.88);
		      break;
	    case C5:
		      set_PWM(523.25);
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
    while (1) {
	tempA = ~PINA;
	while(!TimerFlag);
   	TimerFlag = 0;
       	if (tempA == 0x01){
        	if (boolTracker == 0x00){
          		boolTracker = 0x01;
		}
        	else if (boolTracker == 0x01){
          		boolTracker = 0x00;
       		}
       }
	if (boolTracker){
		Tick();
        }
	else{
        	set_PWM(0);
      	}
    }
    return 1;
}
