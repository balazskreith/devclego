Components in Devclego
===

## 1. Concept



## 2. Predefined macros

**CMP_DEF_THIS(cmp_foo_t, _bar)**: Declare a
 variable this with a type cmp_foo_t and assign it with _bar

**CMP_CONNECT(foo, bar) or CMP_BIND(foo, bar)**; Assign foo
to bar. (foo = bar). CMP_CONNECT is conventionally used for
assigning a function pointer to another function pointer,
meanwhile CMP_BIND is conventionally used for assign a
function pointer to a function.

**CMP_DEF_GET_PROC(get_foo, cmp_foo_t, _cmp_foo)**; Generate
the following function: cmp_foo_t get_foo(){ return _cmp_foo; }

**\__CMP_NO_INIT_FUNC\__ or \__CMP_NO_DEINIT_FUNC\__**:
gives a function name that do not do anything.

**\__CMP_NO_INIT_MFUNC\__ or \__CMP_NO_DEINIT_MFUNC\__**:
gives a function name with void* parameter do nothing.


## Quick tutorial

Components should be placed in the [cmp](https://github.com/balazskreith/devclego/tree/master/src/cmp) folder, and should be declared in header files, meanwhile  their processes should be defined in the body file (.c). We go through how to
create a pipeline and using different type of components.


### Simple component

```C

typedef struct packet_struct_t {
  char bytes[1024];
}packet_t;

typedef struct _cmp_A_struct_t {
  void (*receiver)(packet_t*);
  void (*send)(packet_t*);
  int32_t id;
}_cmp_A_t;

// Here we define component A
#define CMP_NAME_A "A component"
static void _cmp_A_init(void);
static void void _cmp_A_process(packet_t *packet);
CMP_DEF(
   static,               // Type of definition. static means every function marked as static
   _cmp_A_t,             // Type of component
   CMP_NAME_A,           // Name of the component
   _cmp_A,               // Variable name of the component
   _cmp_A_ctor,          // Name of the constructor process implemented automatically
   _cmp_A_dtor,          // Name of the destructor process implemented automatically
   _cmp_A_init,          // Name of the external process called after construction
   __NO_TEST_FUNC_,      // Name of the external process called after initialization
   __CMP_NO_DEINIT_FUNC_ // Name of the external process called before destruction
);

void _cmp_A_init(void) {
  CMP_BIND(_cmp_A->receiver, _cmp_A_process);
}

void _cmp_A_process(packet_t *packet) {
  CMP_DEF_THIS(_cmp_A_t, _cmp_A);
  packet->bytes[2] = 2;
  this->send(packet);
}

```


**CMP_DEF** generates generic code for a component.
It makes the constructor and the destructor. If you have any further
tasks related to the construction you can give a component initialization
function, which is going to be called right after the component is
constructed. Similarly you can give the deinitialization process.
You can see in definitions there is a name definition and a function
declaration. The name is a string use for providing usable informations
about the component states in logfiles. The function declared here will
be the main process belongs to the component and bound later.

### Threaded component

```C
typedef struct _cmp_foo_struct_t
{
	bar_t* (*demand)();
	void   (*send)(bar_t*);
}_cmp_foo_t;

static void _cmp_foo_init();
static void _thr_foo_proc_main(thread_t*);
#define CMP_NAME_FOO "Foo component"
CMP_THREAD(
        static,                 //type of declaration
       cmp_foo_t,               //type of the threaded component
       foo,                     //unique name used for identify a thread and a component
       CMP_NAME_FOO,            //name of the component
       _cmp_foo_init,           //name of the process initialize the component
       __CMP_NO_DEINIT_FUNC_,   //name of the process deinitialize the component
       _cmp_foo_start,          //name of the process activate the thread
       _cmp_foo_stop,           //name of the process deactivate the thread
       _thr_foo_proc_main       //name of the process called in the thread
       );
#undef CMP_NAME_FOO

void  _thr_foo_proc_main(thread_t *thread)
{
    CMP_DEF_THIS(_cmp_foo_t, (_cmp_foo_t*) thread->arg);
    //Initializations
    bar_t *item;

    //thread loop
    do{
    	item = this->demand();
    	//...do something...  
    	this->send(item);  
    }
    while(thread->state == THREAD_STATE_RUN);
}
```

In the above example you see a component runs in a separated thread. Above
the ```_cmp_foo_start()``` and ```_cmp_foo_stop()``` method are generated for
activating and deactivating the thread for the component. In the underlying implementation the ```_thr_foo_proc_main(thread_t *thread)``` is called and the component variable is passed as the thread argument.


#### Parallel threads

```C
typedef struct _cmp_foo_struct_t
{
	int32_t *id;
	bar_t* (*demand)();
	void   (*send)(bar_t*);
}_cmp_foo_t;

static void _thr_foo_init(thread_t*);
static void _thr_foo_proc_main(thread_t*);
static void _cmp_foo_deinit(_cmp_foo_t*);
CMP_THREADS(static,              //type of declarations
		_cmp_foo_t,              //type of components
		foo,		             //unique name used for identifying generated variables and processes
		CMP_NAME_CONLISTENER,    //name of the component running in the thread
		_foos_ctor,              //name of the constructor process used for creating a thread chain
		_foos_dtor,              //name of the destructor process used for disposing a thread chain
		_itr_foos,               //name of the iterator process
		_thr_foo_init,           //name of the process initialize a thread
		__CMP_NO_INIT_MFUNC__,   //name of the process initialize a component
		__THR_NO_DEINIT_PROC__,  //name of the process deinitialize a thread
		_cmp_foo_deinit,         //name of the process deinitialize a component
		_thr_foo_proc_main,      //name of the thread process called after initialization
		bar_t,                   //name of the type used for activating different thread
		bar,                     //name of the attribute inside the component used for pointing to the activator type
		_foo_start,			     //name of the process used creating and activating a component
		_foo_stop                //name of the process used for deactivating and disposing a threaded component
		)
```

The main difference from the CMP_THREAD, where only one thread is executed
as an instance of the component, here more thread is defined as individual
instances of the same component. We achieve this by using the CMP_THREADS
macro. The obligation of using is to define an attribute inside
the component structure definition which identifying somehow the instance.
Here we used a separated array of integers for this purpuse.
Another difference that the generated init, deinit, start and stop methods
are require parameters pointing to an instance. The start, stop requires
the parameter defined in the attribute in the structure to determine
which component has to be started, the init and the deinit function passes
the component instance itself as a this parameter.

### Signalized puffers (Blocking Queues)

```C
#define CMP_DATA_PUFFER_SIZE 16
#define CMP_NAME_PUFFER "Foo puffer"
CMP_DEF_SGPUFFER(
			static,					      /* Type of declaration*/
			bar_t,				        /* Type of items*/
			cmd_dtor,			        /* Name of the destrctor process*/
			CMP_NAME_PUFFER,      /* Name of the component*/
			_cmp_foopuffer,		    /* Name of the variable used for referencing it*/
			CMP_DATA_PUFFER_SIZE,	/* Maximal number of items in the puffer*/
			_cmp_foopuffer_ctor,	/* Name of the process used for constructing*/
			_cmp_foopuffer_dtor	  /* Name of the process used for destructing*/
		);
#undef CMP_NAME_PUFFER
#undef CMP_DATA_PUFFER_SIZE
```

A signalized puffer is a thread safe puffer uses conditional waiting and
mutexes in case they are full or empty. They are used to send data from one thread to another and collects data in between. The limitation of the buffer is given (here it is 16).


### Recycle puffers (Object pools)

```C
#define CMP_DATA_PUFFER_SIZE 64
#define CMP_NAME_PUFFER "Foo puffer"
static bar_t* bar_ctor();
static void bar_dtor(bar_t*);
static void clean_bar(bar_t*);
CMP_DEF_RECPUFFER(
	static,               /*type of the declarations*/
	packet_t,             /*name of the type of the items*/
	bar_ctor,             /*name of the process used for constructing an item if the puffer is empty.*/
	bar_dtor,             /*name of the process used for deconstruting an item if the puffer is full.*/
	clean_bar,            /*name of the process clear the item when the recycle receives it*/
	CMP_NAME_PUFFER,      /*name of the recycle puffer*/
	_cmp_foo,             /*name of the variable used for referencing to the recycle*/
	CMP_DATA_PUFFER_SIZE, /*length of the recycle puffer*/
	_cmp_foo_ctor,        /*name of the constructor process for this recycle*/
	_cmp_foo_dtor         /*name of the destructor process for this recycle*/
);
#undef CMP_DATA_PUFFER_SIZE
#undef CMP_NAME_PUFFER
```

Recycle puffer is used for reusing allocated bytes if we need to use the same data structure over and over again. it never blocks a thread and it uses spinlocks
instead of mutexes. If the puffer is empty it calls the bar_ctor function,
which creates bar_t object. If the puffer is full the bar_dtor is going
to be called for the received item. If it is not empty the bar_clean is going to
be called for setting the necessary values to the default.


### Thread separator

```C
#define CMP_NAME_FOO "Foo component"
CMP_PUSH_THREADER(bar_t,
	forwarder,
	CMP_NAME_FOO,
	_forwarder_start,
	_forwarder_stop
);
```

CMP_PUSH_THREADER define a component defines a component with a receiver
and sender. The task of this component is to separate the send
function from the receiver function running in different threads,
and thereby the two process can run parallely.
It doesn’t contiains puffer, thereby if this component is already busy passing the previously received data to the successor thread it blocks the caller.
