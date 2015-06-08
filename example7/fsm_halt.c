#include "fsm_halt.h"

#include "cmp_A.h"
#include "cmp_B.h"
#include "cmp_C.h"

static void _start();
//----------------------------------------------------------------------------------------------------
//------------------------- State transitions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------
fsm_states_t _fsm_halt_trans(int32_t event, void *arg)
{
	CMP_DEF_THIS(fsm_t, get_fsm());

	switch(event)
	{
	case FSM_EVENT_START:
		_start();
		return FSM_STATE_RUN;
		break;
	default:
		WARNINGPRINT("The event (%s) you required doesn't have transition in the current state (%s)", this->event_str, this->state_str);
		break;
	}

	return this->actual;
}


//----------------------------------------------------------------------------------------------------
//------------------------- Actions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _start()
{
	//start components
	get_cmp_A()->start();
	get_cmp_B()->start();
	get_cmp_C()->start();
}
