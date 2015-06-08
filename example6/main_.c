#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE6_ON

#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

#include "cmp_A.h"
#include "dmap.h"
#include "fsm.h"
//----------------------------------------------------------------------------------------------------
//------------------------- Program entry point  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------
int example6_main()
{
	//devclego standard output directions

	fsm_ctor();
	get_fsm()->fire(FSM_EVENT_START, NULL);

	//Try component processes
	packet_t packet[30];
	int32_t index;
	for(index = 0; index < 30; ++index){
		get_cmp_A()->receiver(&packet[index]);
	}
	thread_sleep(1000);

	get_fsm()->fire(FSM_EVENT_STOP, NULL);
	fsm_dtor();

	return 0;
}

#else

int example6_main()
{
	return 0;
}

#endif /* EXAMPLE6_ON */
