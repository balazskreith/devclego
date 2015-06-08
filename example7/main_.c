#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

#include "cmp_A.h"
#include "dmap.h"
//----------------------------------------------------------------------------------------------------
//------------------------- Program entry point  -----------------------------------------------------------
//----------------------------------------------------------------------------------------------------
int main()
{
	//devclego standard output directions
	devclego_stdlog = &printf;
	devclego_stdout = &printf;

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
