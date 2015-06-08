#define DEVCLEGO_RTLIB

#include "devclego.h"
#include <assert.h>

void devlego_init(int (*stdout)(const char*,...), int (*stdlog)(const char*,...))
{
	_devlegoio = (_devlego_t*) malloc(sizeof(_devlego_t));
	memset(_devlegoio, 0, sizeof(_devlego_t));
	_devlegoio->print_stdlog = stdout == NULL ? __noprint : stdout;
	_devlegoio->print_stdout = stdlog == NULL ? __noprint : stdlog;
}

void devlego_deinit()
{
	free(_devlegoio);
}

int __noprint(const char *format, ...)
{
	return 0;
}

// -------------------------------------- threading implementation ---------------
mutex_t* mutex_ctor()
{
	mutex_t* result;
	result = (mutex_t*) malloc(sizeof(mutex_t));
	BZERO(result, sizeof(mutex_t));
	//pthread_mutex_init(result , NULL);
	return result;
}

void mutex_dtor(void *mutex)
{
	mutex_t *target;
	if(mutex == NULL){
		return;
	}
	target = (mutex_t*) mutex;
	pthread_mutex_destroy(target);
}


void mutex_lock(mutex_t* mutex)
{
	debug_lockcall("mutex_lock");
	pthread_mutex_lock(mutex);
}

void mutex_unlock(mutex_t* mutex)
{
	debug_lockcall("mutex_unlock");
	pthread_mutex_unlock(mutex);
}

spin_t* spin_ctor()
{
	spin_t* result;
	result = (spin_t*) malloc(sizeof(spin_t));
	BZERO(result, sizeof(spin_t));
	pthread_spin_init(result, 0);
	return result;
}

void spin_dtor(spin_t *spin)
{
	spin_t *target;
	if(spin == NULL){
		return;
	}

	target = (spin_t*) spin;
	pthread_spin_destroy(target);
}


void spin_lock(spin_t* spin)
{
	debug_lockcall("spin_lock");
	pthread_spin_lock(spin);
}

void spin_unlock(spin_t* spin)
{
	debug_lockcall("spin_unlock");
	pthread_spin_unlock(spin);
}

signal_t* signal_ctor()
{
	signal_t* result;
	result = (signal_t*) malloc(sizeof(signal_t));
	BZERO(result, sizeof(signal_t));
	result->mutex = mutex_ctor();
	result->waiting = BOOL_FALSE;
	result->waiters = 0;
	pthread_cond_init (&(result->cond), NULL);
	return result;
}

void signal_dtor(signal_t *signal)
{
	signal_t *target;
	if(signal == NULL){
		return;
	}

	target = (signal_t*) signal;
	mutex_dtor(target->mutex);
	pthread_cond_destroy(&(target->cond));
	free(signal);
}

void signal_set(signal_t *signal)
{
	debug_lockcall("signal_set");
	pthread_cond_signal(&(signal->cond));
}

void signal_setall(signal_t *signal)
{
	debug_lockcall("signal_set");
	pthread_cond_broadcast(&signal->cond);
}

void signal_wait(signal_t *signal)
{
	debug_lockcall("signal_wait");
	++signal->waiters;
	signal->waiting = BOOL_TRUE;
	pthread_cond_wait(&(signal->cond), signal->mutex);
	signal->waiting = BOOL_FALSE;
	--signal->waiters;
}

void signal_lock(signal_t *signal)
{
	debug_lockcall("signal_lock");
	pthread_mutex_lock(signal->mutex);
}

void signal_unlock(signal_t *signal)
{
	debug_lockcall("signal_unlock");
	pthread_mutex_unlock(signal->mutex);
}

void signal_release(signal_t *signal)
{
	debug_lockcall("signal_release");
	if(pthread_mutex_trylock(signal->mutex) == EDEADLK){
		signal_unlock(signal);
	}
}

#define BARRIER_FLAG (1UL<<31)
barrier_t* barrier_ctor()
{
	barrier_t* result;
	result = (barrier_t*) malloc(sizeof(barrier_t));
	BZERO(result, sizeof(barrier_t));
	result->mutex = mutex_ctor();
	result->gatenum = 0;
	result->current = 0;
	pthread_cond_init (&(result->cond), NULL);
	return result;
}

void barrier_dtor(barrier_t *barrier)
{
	if(barrier == NULL){
		return;
	}

	pthread_mutex_lock(barrier->mutex);
	while (barrier->current > BARRIER_FLAG)
	{
		/* Wait until everyone exits the barrier */
		pthread_cond_wait(&barrier->cond, barrier->mutex);
	}
	pthread_mutex_unlock(barrier->mutex);

	mutex_dtor(barrier->mutex);
	pthread_cond_destroy(&(barrier->cond));
	free(barrier);
}

int barrier_wait(barrier_t *barrier)
{
	pthread_mutex_lock(barrier->mutex);

	while (barrier->current > BARRIER_FLAG){
		/* Wait until everyone exits the barrier */
		pthread_cond_wait(&barrier->cond, barrier->mutex);
	}

	/* Are we the first to enter? */
	if (barrier->current == BARRIER_FLAG){
		barrier->current = 0;
	}

	if (++barrier->current == barrier->gatenum){
		barrier->current += BARRIER_FLAG - 1;
		pthread_cond_broadcast(&barrier->cond);
		pthread_mutex_unlock(barrier->mutex);

		return PTHREAD_BARRIER_SERIAL_THREAD;
	}

	while (barrier->current < BARRIER_FLAG){
		/* Wait until enough threads enter the barrier */
		pthread_cond_wait(&barrier->cond, barrier->mutex);
	}

	barrier->current--;

	/* Get entering threads to wake up */
	if (barrier->current == BARRIER_FLAG){
		pthread_cond_broadcast(&barrier->cond);
	}

	pthread_mutex_unlock(barrier->mutex);

	return 0;
}

rwmutex_t *rwmutex_ctor()
{
	rwmutex_t* result;
	result = (rwmutex_t*) malloc(sizeof(rwmutex_t));
	BZERO(result, sizeof(rwmutex_t));
	pthread_rwlock_init(result, NULL);
	return result;
}

void rwmutex_dtor(void *rwmutex)
{
	rwmutex_t *target;
	if(rwmutex == NULL){
		return;
	}
	target = (rwmutex_t*) rwmutex;
	pthread_rwlock_destroy(target);
}

void rwmutex_read_lock(rwmutex_t *rwmutex)
{
	pthread_rwlock_rdlock(rwmutex);
}

void rwmutex_write_lock(rwmutex_t *rwmutex)
{
	pthread_rwlock_wrlock(rwmutex);
}

void rwmutex_read_unlock(rwmutex_t *rwmutex)
{
	pthread_rwlock_unlock(rwmutex);
}
void rwmutex_write_unlock(rwmutex_t *rwmutex)
{
	pthread_rwlock_unlock(rwmutex);
}



void thread_sleep(uint16_t ms)
{
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif

}


void thread_usleep(uint16_t us)
{
#ifdef _WIN32
	printf("thread_usleep is not implemented.");
#else
	usleep(us);
#endif

}


thread_t* thread_ctor()
{
	thread_t* result;
	result = (thread_t*) malloc(sizeof(thread_t));
	result->arg = NULL;
	result->process = NULL;
	result->state = THREAD_STATE_CONSTRUCTED;
	pthread_attr_init(&(result->handler_attr));
	pthread_attr_setdetachstate(&(result->handler_attr), PTHREAD_CREATE_JOINABLE);
	return result;
}

void setup_thread(thread_t* thread, void* (*process)(void*) , void* arg)
{
	if(thread == NULL || thread->state != THREAD_STATE_CONSTRUCTED){
		WARNINGPRINT("The desired thread you want to start is not exists or not in constructed state");
		return;
	}
	thread->process = process;
	thread->arg = arg;
	thread->state = THREAD_STATE_READY;
}

void start_thread(thread_t* thread)
{
	if(thread == NULL || thread->state != THREAD_STATE_READY){
		WARNINGPRINT("Thread start is called and the desired thread is not exists or not in ready state");
		return;
	}
	thread->state = THREAD_STATE_STARTED;
	thread->thread_id =
			pthread_create(
				&(thread->handler),       /*the thread handler*/
				&(thread->handler_attr),  /*the attribute of the thread handler*/
				thread->process,          /*the main process of the thread*/
				(void*)thread             /*the argument the thread main process will gets.*/
			);
}


void stop_thread(thread_t* thread)
{
	int32_t       wait = 0;
	const int32_t wait_limit = 10;
	if(thread == NULL || thread->state != THREAD_STATE_RUN ){
		WARNINGPRINT("Thread stop is called and the desired thread is not in run state");
		return;
	}
	thread->state = THREAD_STATE_STOP;
	while(thread->state != THREAD_STATE_STOPPED && wait++ < wait_limit){
		thread_sleep(50);
	}
	if(wait >= wait_limit){
		WARNINGPRINT("thread must be cancelled");
		pthread_cancel(thread->handler);
	}
	thread->state = THREAD_STATE_READY;
}

void thread_dtor(void *thread)
{
	thread_t* target;
	if(thread == NULL){
		return;
	}
	target = (thread_t*) thread;
	if(target->state == THREAD_STATE_RUN){
		stop_thread(target);
	}
	pthread_attr_destroy(&(target->handler_attr));
	target->process = NULL;
	target->thread_id = -1;
	free(target);
}

// -------------------------------------- end od threading implementation ---------------



datapuffer_t* datapuffer_ctor(int32_t size)
{
	datapuffer_t* result;
	int32_t index;
	result = (datapuffer_t*) malloc(sizeof(datapuffer_t));
	result->items = (void**) malloc(sizeof(void*) * size);
	result->length = size;
	result->start = 0;
	result->end = 0;
	result->count = 0;
	for(index = 0; index < result->length; index++)
	{
		result->items[index] = NULL;
	}
	return result;
}//# datapuffer_ctor end


void datapuffer_dtor(datapuffer_t* puffer)
{
	int32_t index;
	void* item;
	index = 0;
	if(puffer == NULL){
		return;
	}
	for(index = 0; index <  puffer->length; index++)
	{
		item = puffer->items[index];
		if(item == NULL)
		{
			continue;
		}
		free(item);
	}
	free(puffer->items);
	free(puffer);
}//# datapuffer_dtor end

void datapuffer_write(datapuffer_t* puffer, void* item)
{
	puffer->items[puffer->end++] = item;
	++puffer->count;
	if(puffer->length <= puffer->end){
		puffer->end = 0;
	}
}//# datapuffer_write end

void* datapuffer_read(datapuffer_t* puffer)
{
	puffer->read = puffer->items[puffer->start];
	puffer->items[puffer->start] = NULL;
	if(puffer->length <= ++puffer->start){
		puffer->start = 0;
	}
	--puffer->count;
	return puffer->read;
}//# datapuffer_read end

int32_t datapuffer_readcapacity(datapuffer_t *datapuffer)
{
	return datapuffer->count;
}

int32_t datapuffer_writecapacity(datapuffer_t *datapuffer)
{
	return datapuffer->length - datapuffer->count;
}

bool_t datapuffer_isfull(datapuffer_t *datapuffer)
{
	return datapuffer->count == datapuffer->length ? BOOL_TRUE : BOOL_FALSE;
}

bool_t datapuffer_isempty(datapuffer_t *datapuffer)
{
	return datapuffer->count == 0 ? BOOL_TRUE : BOOL_FALSE;
}

void datapuffer_clear(datapuffer_t *puffer, void (*dtor)(void*))
{
	int32_t i,c;
	void *item;
	for(i = 0, c = datapuffer_readcapacity(puffer); i < c; ++i){
		item = datapuffer_read(puffer);
		if(dtor == NULL){
			continue;
		}
		dtor(item);
	}
}

//----------------- end of puffer implementations ----------------------


// ------------------ make functions -----------------------------------

thread_t* make_thread(void *(*process)(void*), void *arg)
{
	thread_t* result;
	result = thread_ctor();
	setup_thread(result, process, arg);
	return result;
}

barrier_t *make_barrier(int32_t gatenum)
{
	barrier_t* result;
	result = barrier_ctor();
	result->gatenum = gatenum;
	return result;
}

//---------------- end of make functions ---------------------------------

//---------------------- vector implementiations ---------------------


void vector_init(vector_t *v, size_t elem_size, size_t init_size, void (*free_func)(void *))
{
	v->item_size = elem_size;
	v->items_alloc_num = (int) init_size > 0 ? init_size : VECTOR_INIT_SIZE;
	v->items_num = 0;
	v->items = malloc(elem_size * v->items_alloc_num);
	assert(v->items != NULL);
	v->disposer = free_func != NULL ? free_func : NULL;
}

void vector_dispose(vector_t *v)
{
	size_t i;

	if (v->disposer != NULL) {
		for (i = 0; i < v->items_num; i++) {
			v->disposer(VECTOR_INDEX(i));
		}
	}

	free(v->items);
}


void vector_copy(vector_t *v1, vector_t *v2)
{
	v2->items_num = v1->items_num;
	v2->items_alloc_num = v1->items_alloc_num;
	v2->item_size = v1->item_size;

	v2->items = realloc(v2->items, v2->items_alloc_num * v2->item_size);
	assert(v2->items != NULL);

	memcpy(v2->items, v1->items, v2->items_num * v2->item_size);
}

void vector_insert(vector_t *v, void *elem, size_t index)
{
	void *target;

	if ((int) index > -1) {
		if (!VECTOR_INBOUNDS(index))
			return;
		target = VECTOR_INDEX(index);
	} else {
		if (!VECTOR_HASSPACE(v))
			vector_grow(v, 0);
		target = VECTOR_INDEX(v->items_num);
		v->items_num++; /* Only grow when adding a new item not when inserting in a spec indx */
	}

	memcpy(target, elem, v->item_size);
}

void vector_insert_at(vector_t *v, void *elem, size_t index)
{
	if ((int) index < 0)
		return;

	if (!VECTOR_HASSPACE(v))
		vector_grow(v, 0);

	if (index < v->items_num)
		memmove(VECTOR_INDEX(index + 1), VECTOR_INDEX(index), (v->items_num - index) * v->item_size);

	/* 1: we are passing index so insert won't increment this 2: insert checks INBONDS... */
	v->items_num++;

	vector_insert(v, elem, index);
}

void vector_push(vector_t *v, void *elem)
{
	vector_insert(v, elem, -1);
}

void vector_pop(vector_t *v, void *elem)
{
	memcpy(elem, VECTOR_INDEX(v->items_num - 1), v->item_size);
	v->items_num--;
}

void vector_shift(vector_t *v, void *elem)
{
	memcpy(elem, v->items, v->item_size);
	memmove(VECTOR_INDEX(0), VECTOR_INDEX(1), v->items_num * v->item_size);

	v->items_num--;
}

void vector_unshift(vector_t *v, void *elem)
{
	if (!VECTOR_HASSPACE(v))
		vector_grow(v, v->items_num + 1);

	memmove(VECTOR_INDEX(1), v->items, v->items_num * v->item_size);
	memcpy(v->items, elem, v->item_size);

	v->items_num++;
}

void vector_transpose(vector_t *v, size_t index1, size_t index2)
{
	vector_swap(VECTOR_INDEX(index1), VECTOR_INDEX(index2), v->item_size);
}

void vector_grow(vector_t *v, size_t size)
{
	if (size > v->items_alloc_num)
		v->items_alloc_num = size;
	else
		v->items_alloc_num *= 2;

	v->items = realloc(v->items, v->item_size * v->items_alloc_num);
	assert(v->items != NULL);
}

void vector_get(vector_t *v, size_t index, void *elem)
{
	assert((int) index >= 0);

	if (!VECTOR_INBOUNDS(index)) {
		elem = NULL;
		return;
	}

	memcpy(elem, VECTOR_INDEX(index), v->item_size);
}

void vector_get_all(vector_t *v, void *elems)
{
	memcpy(elems, v->items, v->items_num * v->item_size);
}

void vector_remove(vector_t *v, size_t index)
{
	assert((int) index > 0);

	if (!VECTOR_INBOUNDS(index))
		return;

	memmove(VECTOR_INDEX(index), VECTOR_INDEX(index + 1), v->item_size);
	v->items_num--;
}

void vector_remove_all(vector_t *v)
{
	v->items_num = 0;
	v->items = realloc(v->items, v->items_alloc_num);
	assert(v->items != NULL);
}

size_t vector_length(vector_t *v)
{
	return v->items_num;
}

size_t vector_size(vector_t *v)
{
	return v->items_num * v->item_size;
}

void vector_cmp_all(vector_t *v, void *elem, int (*cmp_func)(const void *, const void *))
{
	size_t i;
	void *best_match = VECTOR_INDEX(0);

	for (i = 1; i < v->items_num; i++)
		if (cmp_func(VECTOR_INDEX(i), best_match) > 0)
			best_match = VECTOR_INDEX(i);

	memcpy(elem, best_match, v->item_size);
}

void vector_qsort(vector_t *v, int (*cmp_func)(const void *, const void *))
{
	qsort(v->items, v->items_num, v->item_size, cmp_func);
}

void vector_swap(void *elemp1, void *elemp2, size_t elem_size)
{
	void *tmp = malloc(elem_size);

	memcpy(tmp, elemp1, elem_size);
	memcpy(elemp1, elemp2, elem_size);
	memcpy(elemp2, tmp, elem_size);

             free(tmp); /* Thanks to gromit */
}

//---------------------- end of vector implementations

//------------------------ other functions -----------------------------
void ptrmov(void **dst, void **src)
{
	*dst = *src;
	*src = NULL;
}

void func_do_nothing()
{
	/*nothing*/
}

void func_with_param_do_nothing(void *param)
{
	/*nothing*/
}

bool_t func_return_true()
{
	return BOOL_TRUE;
}

bool_t func_with_param_return_true(void *param)
{
	return BOOL_TRUE;
}

//------------------------ end of other functions ----------------------


//disposers
void dispose_thread(thread_t *thread)
{
	stop_thread(thread);
}

//real time library
#ifdef DEVLEGO_REALTIME_LIBRARY_NEEDED


void init_interrupt(interrupt_t *interrupt, int signum, bool_t initial_block)
{
	interrupt->signum = signum;
	interrupt->initial_block = initial_block;
}

void set_interrupt_sh(interrupt_t *interrupt, void (*handler)(int))
{
	struct sigaction *action = &interrupt->action;
	int signum = interrupt->signum;
	bool_t initial_block = interrupt->initial_block;
	interrupt->signum = signum;

	if(initial_block == BOOL_TRUE){
		interrupt_block(interrupt);
	}else{
		interrupt_unblock(interrupt);
	}
	/* Install the signal handler. */
	action->sa_handler = handler;
	action->sa_flags = 0;
	sigemptyset(&action->sa_mask);
	sigaction(signum, action, NULL);

	return;
}



void set_timeinterval(timeinterval_t *timeinterval, int32_t s, int32_t ms, int32_t us, int32_t ns)
{
	timeinterval->s = s;
	timeinterval->ms = ms;
	timeinterval->us = us;
	timeinterval->ns = ns;
}

void set_interrupt_ph(interrupt_t *interrupt, void (*handler)(int))
{

}

__always_inline
void interrupt_block(interrupt_t *interrupt)
{
	int signum = interrupt->signum;
	sigset_t *mask = &interrupt->mask;

	/* Block the signal */
   sigemptyset(mask);
   sigaddset(mask, signum);
   //sigsetmask(SIG_BLOCK, mask);
   pthread_sigmask(SIG_BLOCK, mask, NULL);
   //sigprocmask(SIG_BLOCK, mask, NULL);
}

__always_inline
void interrupt_unblock(interrupt_t *interrupt)
{
	int signum = interrupt->signum;
	sigset_t *mask = &interrupt->mask;

	/* Unblock the signal. */
	sigemptyset(mask);
	sigaddset(mask, signum);
	//sigsetmask(SIG_UNBLOCK, mask, 0);
	pthread_sigmask(SIG_UNBLOCK, mask, NULL);
	//sigprocmask(SIG_UNBLOCK, mask, NULL);

}

void interrupt_arm(interrupt_t *interrupt, clockid_t clkid)
{
	interrupt_timer_t *interrupt_timer = &interrupt->timer;
	struct sigevent *sigevent = &interrupt_timer->sigevent;
	struct itimerspec *timeout = &interrupt_timer->timeout;
	timer_t *timerid = &interrupt_timer->timerid;
	int signum = interrupt->signum;
	timeinterval_t *period = &interrupt->period;
	timeinterval_t *initial = &interrupt->initial;

	/* Create a timer. */
	sigevent->sigev_notify = SIGEV_SIGNAL;
	sigevent->sigev_signo = signum;
	sigevent->sigev_value.sival_ptr = timerid;
	timer_create(clkid, sigevent, timerid);

	/* Start the timer. */
	timeout->it_interval.tv_sec = period->s;
	timeout->it_interval.tv_nsec = period->ms * 1000 * 1000 + period->us * 1000 + period->ns;
	timeout->it_value.tv_sec = initial->s;
	timeout->it_value.tv_nsec = initial->ms * 1000 * 1000 + initial->us * 1000 + initial->ns;
	timer_settime(*timerid, 0, timeout, NULL);
}


void interrupt_disarm(interrupt_t *interrupt)
{
	interrupt_timer_t *interrupt_timer = &interrupt->timer;
	timer_delete(interrupt_timer->timerid);
}

__always_inline
void interrupt_wait(interrupt_t *interrupt)
{
	sigwait(&interrupt->mask, &interrupt->signum);
}

__always_inline
void interrupt_waitandcallsh(interrupt_t *interrupt)
{
	sigwait(&interrupt->mask, &interrupt->signum);
	interrupt->action.sa_handler(interrupt->signum);
}

__always_inline
void interrupt_waitandunblock(interrupt_t *interrupt)
{
	sigwait(&interrupt->mask, &interrupt->signum);
	interrupt_unblock(interrupt);
}


timerfd_t* timer_ctor()
{
	timerfd_t* result;
	result = (timerfd_t*) malloc(sizeof(timerfd_t));
	memset(result, 0, sizeof(timerfd_t));
	return result;
}

void timer_dtor(timerfd_t *timer)
{
	free(timer);
}

int32_t timer_wait(timerfd_t *timer)
{
	return read(timer->fd, &timer->expired, sizeof(uint64_t));
}

void timer_arm(timerfd_t *timer)
{
	//struct itimerspec new_value;
	struct timespec now;

	clock_gettime(CLOCK_REALTIME, &now);

   /* Create a CLOCK_REALTIME absolute timer with initial
	  expiration and interval as specified in command line */

   timer->new_value.it_value.tv_sec = now.tv_sec + timer->initial.s;
   timer->new_value.it_value.tv_nsec = now.tv_nsec + (timer->initial.ms * 1000 * 1000 + timer->initial.us * 1000 + timer->initial.ns);
   timer->new_value.it_interval.tv_sec = timer->period.s;
   timer->new_value.it_interval.tv_nsec = timer->period.ms * 1000 * 1000 + timer->period.us * 1000 + timer->period.ns;

   timer->fd = timerfd_create(CLOCK_REALTIME, 0);
   timerfd_settime(timer->fd, TFD_TIMER_ABSTIME, &timer->new_value, NULL);
}

void timer_disarm(timerfd_t *timer)
{
	struct itimerspec new_value;
	new_value.it_value.tv_sec = 0;
	new_value.it_value.tv_nsec = 0;
	new_value.it_interval.tv_sec = 0;
	new_value.it_interval.tv_nsec = 0;

	timerfd_settime(timer->fd, TFD_TIMER_ABSTIME, &new_value, NULL);
}

#endif //DEVLEGO_REALTIME_LIBRARY_NEEDED

//dmap function implementations

//------------------------------------------------------------------------------------------------
//------------------------------------ Custom functions -------------------------------------------------
//------------------------------------------------------------------------------------------------

void dmap_itr_do(bool_t (*dmap_iterator)(int32_t*, void*), void* (*action)(void*))
{
	int32_t    index;
	void      *item = NULL;
	for(index = 0; dmap_iterator(&index, &item) == BOOL_TRUE; ++index){
		action(item);
	}
}

//------------------------------------------------------------------------------------------------
//------------------------------------ Functions ------------------------------------------
//------------------------------------------------------------------------------------------------

dmap_row_t* _dmap_row_ctor()
{
	dmap_row_t *result;
	result = (dmap_row_t*) malloc(sizeof(dmap_row_t));
	BZERO(result, sizeof(dmap_row_t));
	return result;
}

void _dmap_row_dtor(dmap_row_t *row)
{
	if(row->item == NULL){
		free(row);
		return;
	}
	if(row->dtor == NULL){
		runtime_warning("_dmap_row_dtor: Possible memory leak at %p.",row);
		free(row);
		return;
	}
	row->dtor(row->item);
	free(row);
}

dmap_row_t* _make_dmap_row(void *item, void (*dtor)(void*))
{
	dmap_row_t *result;
	result = _dmap_row_ctor();
	result->item = item;
	result->dtor = dtor;
	return result;
}

void _dmap_table_init(dmap_table_t *table, int32_t size, char_t *name)
{
	int32_t index;
	logging("%s: %s", GET_TEXT_INIT_SG("dmap_table"), name);
	BZERO(table, sizeof(dmap_table_t));
	table->rwmutex = rwmutex_ctor();
	table->size = size;
	table->rows = (dmap_row_t**) malloc(sizeof(dmap_row_t*) * table->size);
	for(index = 0; index < table->size; index++)
	{
		table->rows[index] = NULL;
	}
	strcpy(table->name, name);
	PRINTING_INIT_COMPLETED;
}

void _dmap_table_deinit(dmap_table_t *table)
{
	int32_t index;
	dmap_row_t *row;
	logging("%s: %s", GET_TEXT_DEINIT_SG("dmap_table"), table->name);
	for(index = 0; _dmap_itr_table(table, &index, &row) == BOOL_TRUE; ++index)
	{
		_dmap_rem(table, index);
	}
	if(table->rwmutex != NULL){
		rwmutex_dtor(table->rwmutex);
		table->rwmutex = NULL;
	}
	PRINTING_DEINIT_COMPLETED;
}

bool_t _dmap_itr_table(dmap_table_t *table, int32_t *dmap_index, dmap_row_t **row)
{
	*row = NULL;
	for(;*dmap_index < table->size && table->rows[*dmap_index] == NULL; ++(*dmap_index));
	if(table->size <= *dmap_index){
		return BOOL_FALSE;
	}
	*row = table->rows[*dmap_index];
	return BOOL_TRUE;
}

void* _dmap_get_item(dmap_table_t *table, int32_t dmap_index)
{
	void       *result = NULL;
	dmap_row_t *row;
	if(dmap_index < 0 || table->size <= dmap_index){
		runtime_warning("_dmap_get_item: the requested index (%d) is out of the borders in %s", dmap_index, table->name);
		return NULL;
	}
	row = table->rows[dmap_index];
	if(row == NULL){
		runtime_warning("_dmap_get_item: row is not found at %d in %s ", dmap_index, table->name);
		return NULL;
	}
	result = row->item;
	return result;
}

void _dmap_rem_item(dmap_table_t *table, void *item)
{
	int32_t     index;
	dmap_row_t *row = NULL;
	for(index = 0;
		_dmap_itr_table(table, &index, &row) == BOOL_TRUE
		&& item == row->item;
		++index);
	if(index == table->size){
		runtime_warning("_dmap_rem: row is not found to %p in %s ", item, table->name);
		return;
	}
	_dmap_row_dtor(row);
}

void _dmap_rem(dmap_table_t *table, int32_t dmap_index)
{
	dmap_row_t *row;
	if(dmap_index < 0 || table->size <= dmap_index){
		runtime_warning("_dmap_rem: the requested index (%d) is out of the borders in %s", dmap_index, table->name);
		return;
	}
	row = table->rows[dmap_index];
	if(row == NULL){
		runtime_warning("_dmap_rem: row is not exists at %d in %s ", dmap_index, table->name);
		return;
	}
	_dmap_row_dtor(row);
	table->rows[dmap_index] = NULL;
}

int32_t _dmap_add_row(dmap_table_t *table, dmap_row_t *row)
{
	int32_t result;
	if(row == NULL){
		runtime_warning("_dmap_add_row: tried to add row with NULL into %s", table->name);
		return DEVCLEGO_DMAP_ERROR_ADD;
	}
	for(result = 0; table->rows[result] != NULL && result < table->size; result++);
	if(result == table->size){
		runtime_warning("_dmap_add_row: %s is full", table->name);
		return DEVCLEGO_ERROR_TABLE_IS_FULL;
	}
	table->rows[result] = row;
	return result;
}

