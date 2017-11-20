/*
 * scheduler.c
 *
 * Created: 25.10.2017 9:11:22
 *  Author: e1400460, e1400461
 */
#include "scheduler.h"

struct _sched_task_t {
	uint8_t minutes;
	uint8_t seconds;
	void (*task)();
	//short unsigned int state; // !!!
};
//int task_order = {0,1,2};

volatile uint8_t _sched_minutes;
volatile uint8_t _sched_seconds;
volatile uint8_t _sched_hours;

volatile struct _sched_task_t _sched_tasks[SCHEDULER_MAX_TASKS];

void scheduler_init(void) {
	//int16_t index;
	

	//TIMSK &= (1 << OCIE1A);

	//for(index=0; index<SCHEDULER_MAX_TASKS; index++)
	//_sched_tasks[index].task = NULL;
	
	_sched_minutes = 0;
	_sched_seconds = 0;

	
	
	TCCR1A = 0;
	TCNT1=0;
	OCR1A = 24999;
	TCCR1B |= (1 << WGM12)|(1 << CS12);
	TIMSK |= (1 << OCIE1A);
	
	//TCCR1B |= ;

}


int16_t task_add(uint8_t minutes, uint8_t seconds, void (*f)()) {
	int16_t index;
	volatile struct _sched_task_t *task;
	task = &_sched_tasks[0];
	for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
		task = &_sched_tasks[index];
		if (task->task == NULL) {
			TIMSK &= ~(_BV(OCIE1A));
			task->minutes = minutes;
			task->seconds = seconds;
			task->task = f;
			TIMSK |= _BV(OCIE1A);
			return index;
		}
	}
	return -1;
}

ISR(TIMER1_COMPA_vect) {
	int16_t index;
	volatile struct _sched_task_t *task;
	
	_sched_seconds += 1;
	if (_sched_seconds == 60) {
		_sched_seconds = 0;
		
		//task = &_sched_tasks[0];
		//if ( _sched_seconds  == 4)
		//task->task();
		
		/*for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
			task = &_sched_tasks[index];
			if ((task->task != NULL)  && ( _sched_seconds % task->seconds == 0))
			task->task();
		}*/
		}
		task = &_sched_tasks[0];
		for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
			task = &_sched_tasks[index];
			if ((task->task != NULL)  && ( _sched_seconds % task->seconds == 0))
			task->task();
		}
	
	
}
