/*-------------------------------------------------------------------------*/
/**
   @file    cmp_B.c
   @author  B. Kreith
   @brief   "A" component
*/
/*--------------------------------------------------------------------------*/

#include "cmp_B.h"
#include "devclego/cmp.h"
#include "dmap.h"


//----------------------------------------------------------------------------------------------------
//---------------------------- Private type declarations ---------------------------------------------
//----------------------------------------------------------------------------------------------------
typedef struct _cmp_Bproc_struct_t
{
	packet_t* (*demand)();
	int32_t*   id;
}_cmp_Bproc_t;
#define BPROC_NUM 3
int32_t _Bproc_ids[BPROC_NUM];
//----------------------------------------------------------------------------------------------------
//---------------------------- Public definitions ----------------------------------------------------
//----------------------------------------------------------------------------------------------------

static void  _cmp_B_init();
static void  _cmp_B_deinit();
CMP_DEF(, 			           //type of definitions
		cmp_B_t,       	       //type of component
		CMP_NAME_B,            //name of the component
		 _cmp_B,               //variable name of the component
		 cmp_B_ctor,           //name of the constructor process implemented automatically
		 cmp_B_dtor,           //name of the destructor process implemented automatically
		 _cmp_B_init,  //name of the external process called after construction
		 __NO_TEST_FUNC_,      //name of the external process called after initialization
		 _cmp_B_deinit //name of the external process called before destruction
		);

CMP_DEF_GET_PROC(get_cmp_B, cmp_B_t, _cmp_B);

//----------------------------------------------------------------------------------------------------
//------------------------- Component definitions  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#define CMP_NAME_BPUFFER "Puffer for B component"
static void _no_dtor(void*);
CMP_DEF_SGPUFFER(static,	       //type of declaration
				packet_t,          //type of data stored in the puffer
				_no_dtor,          //the destructor of the packet
				CMP_NAME_BPUFFER,  //the presented name of the puffer
				_cmp_Bpuffer,      //name of the puffer referenced to it in the program
				32,                //size of the puffer
				_cmp_Bpuffer_ctor, //name of the process constructing the puffer
				_cmp_Bpuffer_dtor  //name of the process destructing the puffer
				);

static void _cmp_Bproc_init();
static void _thr_Bproc_proc_main(thread_t*);
#define CMP_NAME_BPROC "Processor for B component"
CMP_THREADS(static,          //type of declarations
		_cmp_Bproc_t,  //type of components
		Bproc,		 //unique name used for identifying generated variables and processes
		CMP_NAME_BPROC,//name of the component running in the thread
		_Bprocs_ctor,  //name of the constructor process used for creating a thread chain
		_Bprocs_dtor,  //name of the destructor process used for disposing a thread chain
		_itr_Bprocs,   //name of the iterator process
		__THR_NO_DEINIT_PROC__,  //name of the process initialize a thread
		_cmp_Bproc_init,   //name of the process initialize a component
		__THR_NO_DEINIT_PROC__, //name of the process deinitialize a thread
		__CMP_NO_DEINIT_FUNC_,//name of the process deinitialize a component
		_thr_Bproc_proc_main, //name of the thread process called after initialization
		int32_t,               //name of the type used for activating different thread
		id,                 //name of the attribute inside the component used for pointing to the activator type
		_thr_Bproc_start,			//name of the process used creating and activating a component
		_thr_Bproc_stop           //name of the process used for deactivating and disposing a threaded component
		)

static void _cmp_B_start();
static void _cmp_B_stop();
//----------------------------------------------------------------------------------------------------
//------------------------- Initializations  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void  _cmp_B_init()
{
	//constructions
	_Bprocs_ctor();
	_cmp_Bpuffer_ctor();

	//Binding
	CMP_BIND(_cmp_B->start, _cmp_B_start);
	CMP_BIND(_cmp_B->stop, _cmp_B_stop);

	//Connecting
	CMP_CONNECT(_cmp_B->receiver, _cmp_Bpuffer->receiver);

}

void _cmp_B_start()
{
	int32_t index;
	for(index = 0; index < BPROC_NUM; ++index){
		_Bproc_ids[index] = index;
		_thr_Bproc_start(&_Bproc_ids[index]);
	}
}

void _cmp_B_stop()
{
	int32_t index;
	for(index = 0; index < BPROC_NUM; ++index){
		_thr_Bproc_stop(&_Bproc_ids[index]);
	}
}

void  _cmp_B_deinit()
{
	//destructing
	_Bprocs_dtor();
	_cmp_Bpuffer_dtor();
}

void _cmp_Bproc_init(_cmp_Bproc_t* this)
{
	CMP_CONNECT(this->demand, _cmp_Bpuffer->supplier);
}

//----------------------------------------------------------------------------------------------------
//------------------------- Processes  ---------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


void  _thr_Bproc_proc_main(thread_t *thread)
{
	CMP_DEF_THIS(_cmp_Bproc_t, (_cmp_Bproc_t*) thread->arg);
	packet_t            *packet;
	do{
		thread_sleep(30);
		packet = this->demand();
		printf("Processing packet by cmp_B: The id is: %d (by: %d)\n", packet->bytes[2], *(this->id));

	}
	while(thread->state == THREAD_STATE_RUN);
}

//----------------------------------------------------------------------------------------------------
//------------------------- Private tools  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _no_dtor(void *anything)
{
	//do nothing
}
