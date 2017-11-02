/*
 * scheduler.h
 *
 * Created: 25.10.2017 9:11:38
 *  Author: e1400460
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#define _BV(bit) \
	(1 << (bit)) 

#ifdef __cplusplus
extern "C" {
	#endif

	#include <avr/io.h>
	#include <stddef.h>
	#include <avr/interrupt.h>
	#include <stdint.h>
	

	//#define SCHEDULER_PRESCALER 5 
	//#define SCHEDULER_TICKS 15624
	//Instead, those values are set in the scheduler.c file
	#define SCHEDULER_MAX_TASKS 4
	

	void scheduler_init(uint8_t minutes, uint8_t seconds);

	int16_t task_add(uint8_t minutes, uint8_t seconds, void (*f)());

	//void task_order_shuffle();
	

	#ifdef __cplusplus
}
#endif


#endif /* SCHEDULER_H_ */