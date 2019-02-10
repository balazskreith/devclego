Library in Devclego
===

In default Devclego libraries are inteded to implement basic collections, and making threads easy to create, use, and communicate to each other. In the following examples are given to use the implemented libraries.

## 1. Threading

```C
#include "lib_threading.h"

thread_t *thread;
void* bar(void *arg);
void foo() {

	int num;
	thread = make_thread(bar,&num);
	num = 5;
	start_thread(thread);
	thread_sleep(10); 	
	stop_thread(thread);
	dispose_thread(thread);
}

void* bar(void *arg)
{
	int* num = (int*) arg;
	thread->state = THREAD_STATE_RUN;
	do{
		printf("Number: %d\n", *num);
	} while(thread->state == THREAD_STATE_RUN);
	thread->state = THREAD_STATE_STOPPED;

}
```
The above example is self-explained. We create a thread, which runs a function we define. This function has one ```void *``` typed arguments.
Threads has states must be explicitly changed inside of it's main function.
These states are the following:
THREAD_STATE_CONSTRUCTED, THREAD_STATE_READY, THREAD_STATE_STARTED,
THREAD_STATE_RUN, THREAD_STATE_STOP, THREAD_STATE_STOPPED

When a thread structure is created the state is THREAD_STATE_READY.
The ```start_thread``` function set the state to THREAD_STATE_STARTED,
and the stop_thread checks whether the thread state is THREAD_STATE_RUN or not, and set the thread state into THREAD_STATE_STOP and wait for 10*50ms  for the state to become THREAD_STATE_STOPPED,
otherwise it cancels the thread. Now, you have to explicitly set the thread state to THREAD_STATE_RUN and to THREAD_STATE_STOPPED in the main function of your thread.


### 1.1. Locks

```C
#include "lib_threading.h"

mutex_t  *mutex;
void foo() {
	mutex = mutex_ctor();
	mutex_lock();

	//... do something ...

	mutex_unlock();
	mutex_dtor(mutex);
}
```

Mutexes can be created and initialized anywhere in the application, but must be shared
among the thread mutually excludes each other's operation from a certain execution flow.

Note, that mutex blocks the thread until a lock is active. It usually takes microseconds
to wake it up.

References:

 - [1] http://en.wikipedia.org/wiki/Mutex


### 1.2. Read Write mutexes

```C
#include "lib_threading.h"

rwmutex_t *rwmutex;
void foo() {
	rwmutex = rwmutex_ctor();
	rwmutex_read_lock(rwmutex);
	//... reading ...
	rwmutex_read_unlock(rwmutex);

	rwmutex_write_lock(rwmutex);
	//... writing ...
	rwmutex_write_unlock(rwmutex);
	rwmutex_dtor(rwmutex);
}
```

A more sophisticated way of protecting a certain area of the execution flow is to distinguish
read and write operations. That is because several thread can read from the same memory
address it does not cause a problem. But all read operations must be finished and are blocked
to start before any writing operation is started. Datamaps are using read-write mutexes.


References:

 - [1] http://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock
 - [2] http://doc.qt.digia.com/qq/qq11-mutex.html


### 1.3 Spin locks

```C
#include "lib_threading.h"

spin_t *spin;
void foo() {
	spin = spin_ctor();

	spin_lock(spin);
	//... do something ...
	spin_unlock(spin);

	spin_dtor(spin);
}
```

(Theoretically) A much faster, but more CPU intensive technique to mutually excludes threads from a
certain execution flow is to use spins.

References:
 - [1] http://en.wikipedia.org/wiki/Spinlock


### 1.4. Conditional Waiting


```C
#include "lib_threading.h"

signal_t* signal; // yes, in Devclego it is called signals
void foo() {
 	signal = signal_ctor();
 	signal_lock(signal);

 	signal_wait(signal);

 	signal_unlock(signal);
 	signal_dtor(signal);
 }

void bar() //parallel with foo, but after signal is constructed {
 	signal_lock(signal);
 	signal_set(signal);
 	signal_unlock(signal);
 }
```


It is called conditional waiting and the this name confuses with the operating system
signals. I am sorry. Signals here intended to send notification between threads.
A process send a signal to another one, which waits for it.
Operating system signals are called interrupts in Devclego.

References:
 - [1] http://en.wikipedia.org/wiki/Monitor_(synchronization)


### 1.5. Barriers


```C
#include "lib_threading.h"

barrier_t* barrier;
void foo() {
	barrier = make_barrier(2);

	barrier_wait(barrier);
}

void bar() //parallel with foo, but after barrier is maked
{
	barrier_wait(barrier);
}
```

## 2. Operating System signals

In Devclego operating system signals are called interruptions and
the lib_interrupting collection of library is created to handle operations
regarding to that.

### 2.1. Mask a signal from the OS

```C
#include "lib_interrupting.h"

void sigint_handler(int signum);
void main() {
	interrupt_t interrupt;
	init_interrupt(&interrupt, SIGINT, BOOL_TRUE);
	set_interrupt_sh(&interrupt, sigint_handler);
	interrupt_unblock(&interrupt);
}

void sigint_handler(int signum) {
	printf("handling interrupts");
}
```

In the above example we mask an operating system
signal as we created an interruption. The function
given as a callback is invoked whenever the operating system
sends the specified signal to this process.

In ```init_interrupt(&interrupt, SIGINT, BOOL_TRUE)```, the SIGINT is the signal number
the operating system (Linux) send to the process, the third parameter
indicates weather the signal is initially blocked or not.
Blocked signals will be not handled until its unblocked.
set_interrupt_sh defines a simple handler for interrupt.


### 2.2. Timely based interruptions

```C
#include "lib_interrupting.h"

void flusher(int signum);
datapuffer_t *puffer;
void main()
{
	interrupt_t interrupt;
	init_interrupt(interrupt, SIGRTMIN, BOOL_TRUE);
	set_interrupt_sh(interrupt, flusher);

	// set_timeinterval(interrupt_ptr,  seconds, miliseconds, microseconds, nanoseconds);
	set_timeinterval(&interrupt->period,  0, 16, 0, 0);
	set_timeinterval(&interrupt->initial, 0, 16, 0, 0);

	interrupt_arm(interrupt, CLOCK_REALTIME);
	interrupt_unblock(interrupt);

	puffer = datapuffer_ctor(1 << 20);
	packet_t *packet;
	do{
		packet = generate_random_packet();
		interrupt_block(&interrupt);
		datapuffer_write(puffer, (void*) packet);
		interrupt_unblock(&interrupt);
	}while(1);
}

void flusher(int signum)
{
	int32_t i, c;
	packet_t *packet;
	for(i = 0, c = puffer->count; i < c; ++i){
		packet = (packet_t*) datapuffer_read(puffer);
		//do something
	}
}
```

We can mask an interruption periodically comes from the operating system.
Assume for an instance a puffer is defined and a process fills it up
with packets. You may want to flush this puffer exacly 16ms periods, you have
two ways to do in Devclego:

 - Define a custom timer calls a certain process periodically
 - Use real-time interruptions provided by the OS.

In the above example we used the latter method.

Note that between the ```interrupt_block(&interrupt)``` and ```interrupt_unblock(&interrupt)```
the interrupt is blocked, and if it happens between these blocks the flusher
is called after the unblock.


## 3. Timers

```C
#include "lib_interrupting.h"

//defined in another thread
datapuffer_t *puffer;
mutex_t *mutex;

timerfd_t *timer;
void flusher() {
  int32_t i, c;
  packet_t *packet;

  timer = timer_ctor();

  // set_timeinterval(interrupt_ptr,  seconds, miliseconds, microseconds, nanoseconds);
  set_timeinterval(&timer->initial, 0, 0, 0, 1);
  set_timeinterval(&timer->period, 0, 16, 0, 0);
  timer_arm(timer);

  while(1) {
	  timer_wait(timer);
	  mutex_lock(mutex);
	  for(i = 0, c = puffer->count; i < c; ++i){
		  packet = (packet_t*) datapuffer_read(puffer);
		  // do something
	  }
	  mutex_unlock(mutex);
  }

  timer_disarm(timer);
  timer_dtor(timer);
}
```

We can use timers to do the same task as we accomplished with interruptions
described in 2.2.


## 4. Finite State Machines (FSMs)

Finite State Machine (FSM) in low-level application is a
general concept for organizing behvaioral apects for a possible events.

 - [1] http://en.wikipedia.org/wiki/Finite-state_machine
 - [2] http://en.wikipedia.org/wiki/Virtual_finite-state_machine

In devclego there is no predefined library, however here we provide a
skeleton for an FSM. Look the example below.

__fsm.h__:

```C
typedef enum {
  PROGRAM_STATE_HALT              = 1,
  PROGRAM_STATE_RUN               = 2,
} program_states_t;

typedef enum {
  PROGRAM_EVENT_START             = 1,
  PROGRAM_EVENT_STOP              = 2,
} program_events_t;

typedef struct fsm_struct_t {
  program_states_t     actual;
  char_t               state_str[255];
  char_t               event_str[255];
  void               (*fire)(int32_t, void*);
  signal_t            *signal;
} fsm_t;

void fsm_ctor();
void fsm_dtor();
fsm_t* get_fsm();
```

__fsm.c__:

```C
#include "fsm.h"
#include "cmp.h"
#include lib_defs.h"

static void  _fsm_mpt_init();
static void  _fsm_mpt_deinit();

#define FSM_NAME_PROGRAM "Program machine"

//Define a component
CMP_DEF(, 			         /* Type of definitions*/
		fsm_t,               /* Type of component*/
		FSM_NAME_PROGRAM,    /* Name of the component*/
		 _fsm_mpt,           /* Variable name of the component*/
		 fsm_ctor,           /* Name of the constructor function implemented automatically*/
		 fsm_dtor,           /* Name of the destructor function implemented automatically*/
		 _fsm_mpt_init,      /* Name of the external function called after construction*/
		 __NO_TEST_FUNC_,    /* Name of the external function called after initialization*/
		 _fsm_mpt_deinit     /* Name of the external function called before destruction*/
		);

#undef FSM_NAME_PROGRAM

CMP_DEF_GET_PROC(get_fsm, fsm_t, _fsm_mpt);

// fire function
static void _fsm_fire(int, void*);

// states:
static program_states_t _halt_transitions(int32_t event, void *arg);
static program_states_t _run_transitions(int32_t event, void *arg);

// actions
static void _start();
static void _stop();

//-----------------------------------------------------
//----------------- Initialization   ------------------
//-----------------------------------------------------

void _fsm_mpt_init() {
	CMP_DEF_THIS(fsm_t, _fsm_mpt);

	CMP_BIND(this->fire, _fsm_fire);

	//Construct necessary stuff
	this->signal = signal_ctor();

	//Init necessary stuff
	dmap_init();

}

void _fsm_mpt_deinit() {
	CMP_DEF_THIS(fsm_t, _fsm_mpt);

	//Deinit necessary stuff
	dmap_deinit();

	//Destruct necessary stuff
	signal_dtor(this->signal);
}

//-----------------------------------------------------
//----------------- Fire     --------------------------
//-----------------------------------------------------

void _fsm_fire(int32_t event, void *arg) {
	CMP_DEF_THIS(fsm_t, _fsm_mpt);

	switch(this->actual)
	{
	case PROGRAM_STATE_RUN:
		this->actual = _run_transitions(event, arg);
			break;
	case PROGRAM_STATE_HALT:
		this->actual = _halt_transitions(event, arg);
			break;
	default:
		break;
	}
}

//-----------------------------------------------------
//----------------- Transitions   ---------------------
//-----------------------------------------------------

program_states_t _halt_transitions(int32_t event, void *arg) {
	CMP_DEF_THIS(fsm_t, get_fsm());

	switch(event)
	{
	case PROGRAM_EVENT_START:
		_start();
		return PROGRAM_STATE_RUN;
		break;
	default:
		break;
	}

	return this->actual;
}

program_states_t _run_transitions(int32_t event, void *arg) {
	CMP_DEF_THIS(fsm_t, get_fsm());

	switch(event)
	{
	case PROGRAM_EVENT_STOP:
		_stop();
		return PROGRAM_STATE_HALT;
		break;
	default:
		break;
	}

	return this->actual;
}

//-----------------------------------------------------
//----------------- Actions  --------------------------
//-----------------------------------------------------

void _start() {
	printf("Action start is performed\n");
}

void _stop() {
	printf("Action stop is performed\n");
}
```

Notes:

  - If you have events and transmissions it is
	better to define transitions in separated file.
  - _start(), _stop() actions are good for an instance
  starting and stopping threads.

## 5. Collections

### 5.1. Queue

### 5.2. Datapuffers

```C
#include "lib_puffers.h"

void foo() {
	datapuffer_t *puffer = datapuffer_ctor(10);
	int32_t       numbers[10], i, *item;
	for(i = 0; i < 10; ++i){
		numbers[i] = i;
		datapuffer_write(puffer, &numbers[i]);
		printf("Item written into the puffer: %d\n", numbers[i]);
	}

	for(i = 0; i < 10; ++i){
		item = (int32_t *) datapuffer_read(puffer);
		printf("Item read from the puffer: %d\n", *item);
	}

	datapuffer_dtor(puffer);
}
```

Datapuffers can be interpreted as queues with certain limitations. Datapuffers
are the fastest way to handle FIFO operations, as they after creation do not
allocate any additional memory.


## 6. Callbacks

```C
#include "lib_makers.h"

void foo_sa();
void foo_pa(void*);

void foo() {
	callback_t *callback;
	int32_t number = 5;

	callback = make_callback_sa(foo_sa);
	execute_callback(callback);
	callback_dtor(callback);

	callback = make_callback_pa(foo_pa, (void*) &number);
	execute_callback(callback);
	callback_dtor(callback);
}

void foo_sa() {
	printf("simple foo action is called");
}

void foo_pa(void *arg) {
	int32_t *number = (int32_t*) arg;
	printf("parameterized foo action is called with argument value of %d", *number);
}
```

Callbacks cover functions can be invoked with certain parameters and values anytime
in the program. There are two type of callbacks are defined:

 - simple action: in this case no parameter is given to the callback when it is invoked.
 - parameterized action: in which case a ```void*``` typed parameter is given, when
 the action is invoked.
