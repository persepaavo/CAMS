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


	#define SCHEDULER_MAX_TASKS 5

	struct _sched_task_t {

		uint8_t start_seconds;
		uint8_t start_minutes;
		uint8_t period_sec;
		uint8_t period_min;
		void (*task)();
		};

	void scheduler_init(void);


	int16_t task_add(uint8_t period_seconds, uint8_t period_minutes, 
					uint8_t start_time_seconds, uint8_t start_time_minutes, 
					void (*f)());

	#ifdef __cplusplus
}
#endif


#endif /* SCHEDULER_H_ */
