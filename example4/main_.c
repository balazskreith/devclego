#include "../defs.h"
#ifdef EXAMPLE4_ON

#include "../devclego.h"
#include "cmp_A.h"
#include "cmp_B.h"
#include <stdio.h>
#include <stdlib.h>


//----------------------------------------------------------------------------------------------------
//------------------------- Type definitions  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------

typedef struct _cmp_A_struct_t
{
	void (*receiver)(packet_t*);
	void (*send)(packet_t*);
	int32_t id;
}_cmp_A_t;

typedef struct _cmp_B_struct_t
{
	void (*receiver)(packet_t*);
}_cmp_B_t;

//----------------------------------------------------------------------------------------------------
//------------------------- Component definitions  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------

static void _cmp_A_process(packet_t*);
#define CMP_NAME_A "A component"
CMP_DEF(static, 			   //type of definitions
		_cmp_A_t,       	   //type of component
		CMP_NAME_A,            //name of the component
		 _cmp_A,               //variable name of the component
		 _cmp_A_ctor,          //name of the constructor process implemented automatically
		 _cmp_A_dtor,          //name of the destructor process implemented automatically
		 __CMP_NO_INIT_FUNC_,  //name of the external process called after construction
		 __NO_TEST_FUNC_,      //name of the external process called after initialization
		 __CMP_NO_DEINIT_FUNC_ //name of the external process called before destruction
		);


static void _cmp_B_process(packet_t*);
#define CMP_NAME_B "B component"
CMP_DEF(static, 			   //type of definitions
		_cmp_B_t,       	   //type of component
		CMP_NAME_B,            //name of the component
		 _cmp_B,               //variable name of the component
		 _cmp_B_ctor,          //name of the constructor process implemented automatically
		 _cmp_B_dtor,          //name of the destructor process implemented automatically
		 __CMP_NO_INIT_FUNC_,  //name of the external process called after construction
		 __NO_TEST_FUNC_,      //name of the external process called after initialization
		 __CMP_NO_DEINIT_FUNC_ //name of the external process called before destruction
		);

//----------------------------------------------------------------------------------------------------
//------------------------- Program entry point  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------
static int _nprint(const char *format, ...);
int example4_main()
{
	//devclego standard output directions

	//Component constructions
	_cmp_A_ctor();
	_cmp_B_ctor();

	//Bind component processes
	CMP_BIND(_cmp_A->receiver, _cmp_A_process);
	CMP_BIND(_cmp_B->receiver, _cmp_B_process);

	//connect components
	CMP_CONNECT(_cmp_A->send, _cmp_B->receiver);

	//Try component processes
	packet_t packet;
	_cmp_A->receiver(&packet);

	//destructing components
	_cmp_B_dtor();
	_cmp_A_dtor();
	return 0;
}

//----------------------------------------------------------------------------------------------------
//------------------------- Component processes  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _cmp_A_process(packet_t *packet)
{
	CMP_DEF_THIS(_cmp_A_t, _cmp_A);
	packet->bytes[2] = 2;
	this->send(packet);
}

void _cmp_B_process(packet_t *packet)
{
	CMP_DEF_THIS(_cmp_B_t, _cmp_B);
	printf("The second byte in the packet is the following: %d\n", packet->bytes[2]);
}

//----------------------------------------------------------------------------------------------------
//------------------------- Private tools  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------
int _nprint(const char *format, ...)
{
	return 0;
}

#else

int example4_main()
{
	return 0;
}

#endif /* EXAMPLE4_ON */
