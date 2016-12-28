#include "fsm_cmpactions.h"

#include "fsm.h"
#include "lib_tors.h"

//components:
#include "cmp_predefs.h"
#include "cmp_defs.h"
#include "dmap.h"

void _cmps_ctor()
{
	fsm_t* machine = get_fsm();

	//CONSTRUCTING
	//example:
	//cmp_cmdexecutor_ctor();

	//CONNECTING
	PRINTING_CONNECT_COMPONENTS;

	//example:
	//CMP_CONNECT(get_cmp_recorder()->send, get_cmp_groupcounter()->receier);

}

void _cmps_dtor()
{
  //DESTRUCT
  //example:
  //cmp_cmdexecutor_dtor();

}


//----------------------------------------------------------------------------------------------------
//------------------------- Private tools  ----------------------------------------------------
//----------------------------------------------------------------------------------------------------

