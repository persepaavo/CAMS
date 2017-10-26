/*
 * scheduler.h
 *
 * Created: 25.10.2017 9:11:38
 *  Author: e1400460
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_


#ifdef __cplusplus
extern "C" {
	#endif

	#include <avr/io.h>
	#include <stddef.h>
	#include <avr/interrupt.h>
	#include <stdint.h>
	

	#define SCHEDULER_PRESCALER 5

	#define SCHEDULER_TICKS 15625U

	#define SCHEDULER_MAX_TASKS 5


	void scheduler_init(uint8_t minutes, uint8_t seconds);
	void scheduler_set(uint8_t minutes, uint8_t seconds);

	int16_t task_add(uint8_t minutes, uint8_t seconds, void (*f)());
	void task_del(int16_t index);
	void task_set(int16_t index, uint8_t minutes, uint8_t seconds);

	#ifdef __cplusplus
}
#endif


#endif /* SCHEDULER_H_ */