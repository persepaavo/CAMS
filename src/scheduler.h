/*
 * scheduler.h
 *
 * Created: 25.10.2017 9:11:38
 *  Author: e1400460, e1400461, e1400481
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

	/**************************
	This macro defines a maximum amount of tasks performed by AVR microcontroller
	**************************/

	#define SCHEDULER_MAX_TASKS 1

	/*************************
	Task structure:
	start_seconds, start_minutes are used to set the time of the particular task start;
	period_sec, period_min are the variables of a period of a task;
	the range of minutes and seconds variables are 0-59; 
	period_sec and period_min could be set to 61 in order to
	make a task work in a one shot mode each hour;
	
	(*task)() is a pointer to a task function;
	*************************/
	struct _sched_task_t {

		uint8_t start_seconds;
		uint8_t start_minutes;
		uint8_t period_sec;
		uint8_t period_min;
		void (*task)();
		};
	
	/************************
	This function initializes the scheduler
	************************/
	
	void scheduler_init(void);


	/************************
	This function adds the tasks to the _sched_task_t array;
	All the variables are related to the variables of a _sched_task_t structure
	************************/
	int16_t task_add(uint8_t period_seconds, uint8_t period_minutes, 
					uint8_t start_time_seconds, uint8_t start_time_minutes, 
					void (*f)());

	#ifdef __cplusplus
}
#endif


#endif /* SCHEDULER_H_ */
