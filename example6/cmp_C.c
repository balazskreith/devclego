/*-------------------------------------------------------------------------*/
/**
   @file    cmp_C.c
   @author  B. Kreith
   @brief   "A" component
*/
/*--------------------------------------------------------------------------*/
#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE6_ON

#include "cmp_C.h"
#include "dmap.h"


//----------------------------------------------------------------------------------------------------
//---------------------------- Private type declarations ---------------------------------------------
//----------------------------------------------------------------------------------------------------
#define CMP_NAME_CPROC "C process component"
typedef struct _cmp_Cproc_struct_t
{
	void      (*receiver)(packet_t*);
}_cmp_Cproc_t;
//----------------------------------------------------------------------------------------------------
//---------------------------- Public definitions ----------------------------------------------------
//----------------------------------------------------------------------------------------------------

static void  _cmp_C_init();
static void  _cmp_C_deinit();
CMP_DEF(, 			           //type of definitions
		cmp_C_t,       	       //type of component
		CMP_NAME_C,            //name of the component
		 _cmp_C,               //variable name of the component
		 cmp_C_ctor,           //name of the constructor process implemented automatically
		 cmp_C_dtor,           //name of the destructor process implemented automatically
		 _cmp_C_init,          //name of the external process called after construction
		 __NO_TEST_FUNC_,      //name of the external process called after initialization
		 _cmp_C_deinit         //name of the external process called before destruction
		);

CMP_DEF_GET_PROC(get_cmp_C, cmp_C_t, _cmp_C);

//----------------------------------------------------------------------------------------------------
//------------------------- Component definitions  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------
CMP_PUSH_THREADER(packet_t,
				  forwarder,
				  "Packet Forwarder Component",
				  _forwarder_start,
				  _forwarder_stop
				  );

static void _cmp_Cproc_process(packet_t*);
CMP_DEF(static, 			           //type of definitions
		_cmp_Cproc_t,       	       //type of component
		CMP_NAME_CPROC,            //name of the component
		 _cmp_Cproc,               //variable name of the component
		 _cmp_Cproc_ctor,           //name of the constructor process implemented automatically
		 _cmp_Cproc_dtor,           //name of the destructor process implemented automatically
		 __CMP_NO_INIT_FUNC_,          //name of the external process called after construction
		 __NO_TEST_FUNC_,      //name of the external process called after initialization
		 __CMP_NO_DEINIT_FUNC_         //name of the external process called before destruction
		);

//----------------------------------------------------------------------------------------------------
//------------------------- Initializations  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void  _cmp_C_init()
{
	//constructions
	_cmp_forwarder_ctor();
	_cmp_Cproc_ctor();

	//Binding
	CMP_BIND(_cmp_C->start, _forwarder_start);
	CMP_BIND(_cmp_C->stop, _forwarder_stop);
	CMP_BIND(_cmp_Cproc->receiver, _cmp_Cproc_process);

	//Connecting
	CMP_CONNECT(_cmp_C->receiver,     _cmp_forwarder->receiver);
	CMP_CONNECT(_cmp_forwarder->send, _cmp_Cproc->receiver);

}

void  _cmp_C_deinit()
{
	//destructing
	_cmp_Cproc_dtor();
}

//----------------------------------------------------------------------------------------------------
//------------------------- Processes  ---------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void _cmp_Cproc_process(packet_t *packet)
{
	printf("Processing packet, printing by cmp_C: The id is: %d\n", packet->bytes[2]);
}

#endif /* EXAMPLE6_ON */

