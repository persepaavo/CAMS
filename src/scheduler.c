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
void dispatch(volatile struct _sched_task_t *task)
{
	// the call to this function should push the return address into the stack.
	// we will now construct saving context. The entire context needs to be
	// saved because it is very possible that this could be called from within
	// an isr that doesn't use the call-used registers and therefore doesn't
	// save them.
	asm volatile (
	"push r31 \n\t"
	"push r30 \n\t"
	"push r29 \n\t"
	"push r28 \n\t"
	"push r27 \n\t"
	"push r26 \n\t"
	"push r25 \n\t"
	"push r24 \n\t"
	"push r23 \n\t"
	"push r22 \n\t"
	"push r21 \n\t"
	"push r20 \n\t"
	"push r19 \n\t"
	"push r18 \n\t"
	"push r17 \n\t"
	"push r16 \n\t"
	"push r15 \n\t"
	"push r14 \n\t"
	"push r13 \n\t"
	"push r12 \n\t"
	"push r11 \n\t"
	"push r10 \n\t"
	"push r9 \n\t"
	"push r8 \n\t"
	"push r7 \n\t"
	"push r6 \n\t"
	"push r5 \n\t"
	"push r4 \n\t"
	"push r3 \n\t"
	"push r2 \n\t"
	"push r1 \n\t"
	"push r0 \n\t"
	"in   r0, %[_SREG_] \n\t" //push sreg
	"push r0 \n\t"
	"lds  r26, kos_current_task \n\t"
	"lds  r27, kos_current_task+1 \n\t"
	"sbiw r26, 0 \n\t"
	"breq 1f \n\t" //null check, skip next section
	"in   r0, %[_SPL_] \n\t"
	"st   X+, r0 \n\t"
	"in   r0, %[_SPH_] \n\t"
	"st   X+, r0 \n\t"
	"1:" //begin dispatching
	"mov  r26, %A[_next_task_] \n\t"
	"mov  r27, %B[_next_task_] \n\t"
	"sts  kos_current_task, r26 \n\t" //set current task
	"sts  kos_current_task+1, r27 \n\t"
	"ld   r0, X+ \n\t" //load stack pointer
	"out  %[_SPL_], r0 \n\t"
	"ld   r0, X+ \n\t"
	"out  %[_SPH_], r0 \n\t"
	"pop  r31 \n\t" //status into r31: andi requires register above 15
	"bst  r31, %[_I_] \n\t" //we don't want to enable interrupts just yet, so store the interrupt status in T
	"bld  r31, %[_T_] \n\t" //T flag is on the call clobber list and tasks are only blocked as a result of a function call
	"andi r31, %[_nI_MASK_] \n\t" //I is now stored in T, so clear I
	"out  %[_SREG_], r31 \n\t"
	"pop  r0 \n\t"
	"pop  r1 \n\t"
	"pop  r2 \n\t"
	"pop  r3 \n\t"
	"pop  r4 \n\t"
	"pop  r5 \n\t"
	"pop  r6 \n\t"
	"pop  r7 \n\t"
	"pop  r8 \n\t"
	"pop  r9 \n\t"
	"pop  r10 \n\t"
	"pop  r11 \n\t"
	"pop  r12 \n\t"
	"pop  r13 \n\t"
	"pop  r14 \n\t"
	"pop  r15 \n\t"
	"pop  r16 \n\t"
	"pop  r17 \n\t"
	"pop  r18 \n\t"
	"pop  r19 \n\t"
	"pop  r20 \n\t"
	"pop  r21 \n\t"
	"pop  r22 \n\t"
	"pop  r23 \n\t"
	"pop  r24 \n\t"
	"pop  r25 \n\t"
	"pop  r26 \n\t"
	"pop  r27 \n\t"
	"pop  r28 \n\t"
	"pop  r29 \n\t"
	"pop  r30 \n\t"
	"pop  r31 \n\t"
	"brtc 2f \n\t" //if the T flag is clear, do the non-interrupt enable return
	"reti \n\t"
	"2: \n\t"
	"ret \n\t"
	"" ::
	[_SREG_] "i" _SFR_IO_ADDR(SREG),
	[_I_] "i" SREG_I,
	[_T_] "i" SREG_T,
	[_nI_MASK_] "i" (~(1 << SREG_I)),
	[_SPL_] "i" _SFR_IO_ADDR(SPL),
	[_SPH_] "i" _SFR_IO_ADDR(SPH),
	[_next_task_] "r" (task));
}
