/*
 * scheduler.c
 *
 * Created: 25.10.2017 9:11:22
 *  Author: e1400460, e1400461, e1400481
 */
#include "scheduler.h"


/*	*
	* There are three global variables for a scheduler timer
	*
*/
volatile uint8_t _sched_minutes;
volatile uint8_t _sched_seconds;
volatile uint16_t _sched_mseconds;

/*	*
	* Scheduler tasks' structures' array. SCHEDULER_MAX_TASKS is defined in scheduler.h file.
	*
*/

volatile struct _sched_task_t _sched_tasks[SCHEDULER_MAX_TASKS];

/*	*
	* Scheduler initialization function
	*
*/

void scheduler_init(void)
{

	_sched_mseconds= 0;
	_sched_seconds = 0;
	_sched_minutes = 0; // setting global variables to 0s

	OCR1A= 0x0007;	// Compare value
	TCCR1A = 0;	// Disable 8-bit timer

	TCNT1=0;	// Timer counter is set to 0
	TCCR1B = 0;

	TCCR1B |= (1<<CS12) | (1<< CS10);	// Prescaler mode is set to 1/1024
	TCCR1B |= (1 << WGM12);			// Timer is set to CTC mode
	TIMSK |= (1 << OCIE1A);			// Timer1 Output/Compare A Match interrupt is enabled
	
}


/*	*
	* Task adding function
	* The function's argument are explained in scheduler.h file
	*
*/

int16_t task_add(uint8_t period_seconds, uint8_t period_minutes, 
		 uint8_t start_time_seconds, uint8_t start_time_minutes, 
		 void (*f)())
{
	int16_t index;
	volatile struct _sched_task_t *task; // pointer to a task
	for(index=0; index<SCHEDULER_MAX_TASKS; index++)
	{
		task = &_sched_tasks[index]; // set a pointer to an actual task reference
		if (task->task == NULL)
		{
			TIMSK &= ~(1<<OCIE1A);	// disabling timer interrupt
			task->period_sec = period_seconds; 
			task->period_min = period_minutes;
			task->start_seconds = start_time_seconds;
			task->start_minutes = start_time_minutes; // setting period seconds, period minutes, start seconds and start minutes
			task->task = f;	// task function pointer
			TIMSK |= (1<<OCIE1A); // enable timer interrupt
			return index;
		}
	}
	return -1;
}

/*	*
	* Interrupt Service Routine function
	* ISR is set to Timer/Counter1 Comparator vector
*/

ISR(TIMER1_COMPA_vect)
{
	int16_t index;
	volatile struct _sched_task_t *task;
	_sched_mseconds++;
	// adding seconds
	if (_sched_mseconds == 1000) {
		_sched_mseconds = 0;
		
		_sched_seconds++;
		if (_sched_seconds == 60) // in case if timer seconds exceeds 60
		{
			_sched_seconds = 0;
			_sched_minutes++;
			if (_sched_minutes == 60) //
				_sched_minutes = 0;
		}

		/*
			This loop goes through the array of tasks structure to find the possibly working task
			if a structure's pointer to a task function is not equal to null, the tasks time is checked 
		*/
		for(index=0; index<SCHEDULER_MAX_TASKS; index++)
		{
			task = &_sched_tasks[index];
		
			if (task->task != NULL)
			{
				// if start time is equal to the clock's time or it is already passed
				if ( ( (task->start_minutes == _sched_minutes) && (task->start_seconds <= _sched_seconds) ) ||
					(task->start_minutes < _sched_minutes) )
				{
					// if a timer mode is a one shot mode with one hour period
					if ((task->period_min == 61) && (task->period_sec == 61))
						if ( (task->start_minutes == _sched_minutes) && (task->start_seconds == _sched_seconds) )
							task->task();
					else if
					// if a task has a certain period less than hour	
						( ((_sched_minutes % task->period_min == 0) && (_sched_seconds % task->period_sec == 0) ) ||
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
