/*
 * scheduler.c
 *
 * Created: 25.10.2017 9:11:22
 *  Author: e1400460, e1400461
 */
#include "scheduler.h"



volatile uint8_t _sched_minutes;
volatile uint8_t _sched_seconds;
volatile uint16_t _sched_mseconds;


volatile struct _sched_task_t _sched_tasks[SCHEDULER_MAX_TASKS];

void scheduler_init(void) {

	_sched_mseconds= 0;
	_sched_seconds = 0;
	_sched_minutes = 0;

	OCR1A= 0x0007;	
	TCCR1A = 0;

	TCNT1=0;
	TCCR1B = 0;

	TCCR1B |= (1<<CS12) | (1<< CS10);
	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);
	
}





int16_t task_add(uint8_t period_seconds, uint8_t period_minutes, uint8_t start_time_seconds, uint8_t start_time_minutes, void (*f)()) {
	int16_t index;
	volatile struct _sched_task_t *task;
	task = &_sched_tasks[0];
	for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
		task = &_sched_tasks[index];
		if (task->task == NULL) {
			TIMSK &= ~(_BV(OCIE1A));
			task->period_sec = period_seconds;
			task->period_min = period_minutes;
			task->start_seconds = start_time_seconds;
			task->start_minutes = start_time_minutes;
			task->task = f;
			TIMSK |= _BV(OCIE1A);
			return index;
		}
	}
	return -1;
}

ISR(TIMER1_COMPA_vect)
{
	int16_t index;
	volatile struct _sched_task_t *task;
	_sched_mseconds++;
	if (_sched_mseconds == 1000) {
		_sched_mseconds = 0;
		
		_sched_seconds++;
		if (_sched_seconds == 60)
		{
			_sched_seconds = 0;
			_sched_minutes++;
			if (_sched_minutes == 60)
				_sched_minutes = 0;
		}

	
		for(index=0; index<SCHEDULER_MAX_TASKS; index++) {
			task = &_sched_tasks[index];
		
			if (task->task != NULL)
			{
				if ( ( (task->start_minutes == _sched_minutes) && (task->start_seconds <= _sched_seconds) ) ||
					(task->start_minutes < _sched_minutes) )
				{
					if ((task->period_min == 61) && (task->period_sec == 61))
					{
						if ( (task->start_minutes == _sched_minutes) && (task->start_seconds == _sched_seconds) )
							task->task();
					}
					else if ( (( _sched_minutes % task->period_min == 0) && (_sched_seconds % task->period_sec == 0)) ||
							( (task->period_min == 0) && (_sched_seconds % task->period_sec == 0) ) ||
							( (task->period_sec == 0) && (_sched_minutes % task->period_min == 0) ))
					{
							task->task();
					}
				}
			}
		

		}

		
	}

	
}