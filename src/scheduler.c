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

void scheduler_init(uint8_t minutes, uint8_t seconds) {
	int16_t index;
	
	TIMSK1 &= ~(_BV(OCIE1A));
	
	for(index=0; index<SCHEDULER_MAX_TASKS; index++)
	_sched_tasks[index].task = NULL;
	
	_sched_minutes = minutes;
	_sched_seconds = seconds;

	
	OCR1A = SCHEDULER_TICKS;
	TCCR1A = 0;
	TCCR1B = |= (1 << WGM12);
	//Mode 4, CTC (clear timer on compare) mode set to 4
	TCNT1 = 0;
	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt to compare match 
	TCCR1B = |= (1 << CS12) | (1 << CS10);
	//set prescaler to 1024 and start the timer
}


int16_t task_add(uint8_t minutes, uint8_t seconds, void (*f)()) {
	int16_t index;
	volatile struct _sched_task_t *task;
	
	for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
		task = &_sched_tasks[index];
		if (task->task == NULL) {
			TIMSK1 &= ~(_BV(OCIE1A));
			task->minutes = minutes;
			task->seconds = seconds;
			task->task = f;
			TIMSK1 |= _BV(OCIE1A);
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
		_sched_minutes++;
		if (_sched_minutes == 60) {
			_sched_minutes = 0;
			_sched_hours++;
			if (_sched_hours == 1)
			_sched_hours = 0;
		}
		for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
			task = &_sched_tasks[index];
			if ((task->task != NULL) && (sched_minutes % task->minutes  == 0) && ( sched_seconds % task->seconds == 0))
			task->task();
		}
	}
}