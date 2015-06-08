/*-------------------------------------------------------------------------*/
/**
   @file    cmp_A.c
   @author  B. Kreith
   @brief   "A" component
*/
/*--------------------------------------------------------------------------*/
#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE4_ON

#include "cmp_A.h"


//----------------------------------------------------------------------------------------------------
//---------------------------- Private type declarations ---------------------------------------------
//----------------------------------------------------------------------------------------------------
typedef struct _cmp_Aproc_struct_t
{
	packet_t* (*demand)();
	void      (*sendOdd)(packet_t*);
	void      (*sendEven)(packet_t*);
}_cmp_Aproc_t;
//----------------------------------------------------------------------------------------------------
//---------------------------- Public definitions ----------------------------------------------------
//----------------------------------------------------------------------------------------------------

static void  _cmp_A_init();
static void  _cmp_A_deinit();
CMP_DEF(, 			           //type of definitions
		cmp_A_t,       	       //type of component
		CMP_NAME_A,            //name of the component
		 _cmp_A,               //variable name of the component
		 cmp_A_ctor,           //name of the constructor process implemented automatically
		 cmp_A_dtor,           //name of the destructor process implemented automatically
		 _cmp_A_init,          //name of the external process called after construction
		 __NO_TEST_FUNC_,      //name of the external process called after initialization
		 _cmp_A_deinit         //name of the external process called before destruction
		);

CMP_DEF_GET_PROC(get_cmp_A, cmp_A_t, _cmp_A);

//----------------------------------------------------------------------------------------------------
//------------------------- Component definitions  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#define CMP_NAME_APUFFER "Puffer for A component"
static void _no_dtor(void*);
CMP_DEF_SGPUFFER(static,	       //type of declaration
				packet_t,          //type of data stored in the puffer
				_no_dtor,          //the destructor of the packet
				CMP_NAME_APUFFER,  //the presented name of the puffer
				_cmp_Apuffer,      //name of the puffer referenced to it in the program
				32,                //size of the puffer
				_cmp_Apuffer_ctor, //name of the process constructing the puffer
				_cmp_Apuffer_dtor  //name of the process destructing the puffer
				);



static void _cmp_Aproc_init();
static void _thr_Aproc_proc_main(thread_t*);
#define CMP_NAME_SENDER "Packet Aproc component"
CMP_THREAD(
		static,				    //type of declaration
		_cmp_Aproc_t,	        //type of the threaded component
	   Aproc,			        //unique name used for thread and component
	   CMP_NAME_SENDER,	        //name of the component
	   _cmp_Aproc_init,		    //name of the process initialize the component
	   __CMP_NO_DEINIT_FUNC_,	//name of the process deinitialize the component
	   _cmp_Aproc_start,	    //name of the process activate the thread
	   _cmp_Aproc_stop,		    //name of the process deactivate the thread
	   _thr_Aproc_proc_main	    //name of the process called in the thread
	   );
//----------------------------------------------------------------------------------------------------
//------------------------- Initializations  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void  _cmp_A_init()
{
	//constructions
	_cmp_Apuffer_ctor();

	//Binding
	CMP_BIND(_cmp_A->start, _cmp_Aproc_start);
	CMP_BIND(_cmp_A->stop, _cmp_Aproc_stop);

	//Connecting
	CMP_CONNECT(_cmp_A->receiver, _cmp_Apuffer->receiver);

}

void  _cmp_A_deinit()
{
	//destructing
	_cmp_Apuffer_dtor();
}

void _cmp_Aproc_init()
{
	CMP_DEF_THIS(_cmp_Aproc_t, _cmp_Aproc);
	CMP_CONNECT(this->demand, _cmp_Apuffer->supplier);
	CMP_CONNECT(this->sendOdd,   _cmp_A->sendOdd);
	CMP_CONNECT(this->sendEven,   _cmp_A->sendEven);
}

//----------------------------------------------------------------------------------------------------
//------------------------- Processes  ---------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void  _thr_Aproc_proc_main(thread_t *thread)
{
	CMP_DEF_THIS(_cmp_Aproc_t, (_cmp_Aproc_t*) thread->arg);
	packet_t            *packet;
	byte_t              next = 0;
	do{
		packet = this->demand();
		packet->bytes[2] = ++next;
		printf("Processing packet: Set the id: %d\n", packet->bytes[2]);
		if(packet->bytes[2] % 2 == 0){
			this->sendEven(packet);
		}else{
			this->sendOdd(packet);
		}
		thread_sleep(1);
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

#endif /* EXAMPLE4_ON */

