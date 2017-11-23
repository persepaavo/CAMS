/*
 * scheduler.c
 *
 * Created: 25.10.2017 9:11:22
 *  Author: e1400460, e1400461
 */
#include "scheduler.h"

struct _sched_task_t {

	uint8_t seconds;
	void (*task)();
	int *stack;
};



volatile uint8_t _sched_seconds;
volatile uint16_t _sched_mseconds;

volatile struct _sched_task_t _sched_tasks[SCHEDULER_MAX_TASKS];

void scheduler_init(void) {

	_sched_mseconds= 0;
	_sched_seconds = 0;

	OCR1A= 0x0007;	
	TCCR1A = 0;

	TCNT1=0;
	TCCR1B = 0;

	TCCR1B |= (1<<CS12) | (1<< CS10);
	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);
	
}


int16_t task_add(uint8_t seconds, void (*f)(), int *stack) {
	int16_t index;
	volatile struct _sched_task_t *task;
	task = &_sched_tasks[0];
	for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
		task = &_sched_tasks[index];
		if (task->task == NULL) {
			TIMSK &= ~(_BV(OCIE1A));
			task->seconds = seconds;
			task->task = f;
			task->stack = &stack[index];
			task->stack = 0;
			TIMSK |= _BV(OCIE1A);
			return index;
		}
	}
	return -1;
}

ISR(TIMER1_COMPA_vect) {
	int16_t index;
	volatile struct _sched_task_t *task;
	_sched_mseconds++;
	if (_sched_mseconds == 1000) {
		_sched_mseconds = 0;
		
		_sched_seconds++;
		if (_sched_seconds == 60)
		{
			_sched_seconds = 0;

		}


	
	

	
	for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
		task = &_sched_tasks[index];
		
		if ((task->task != NULL)  && ( _sched_seconds % task->seconds == 0))
		//if (b == 0)
		//{
		//		b = 1;
		{
			//TIMSK &= ~(_BV(OCIE1A));

			task->task(task->stack);
			task->stack;
			//TIMSK |= _BV(OCIE1A);
		}
		//	}
		//b = 0;
		
	}

	
	}
			
}
