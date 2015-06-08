#include "fsm_run.h"

#include "cmp_A.h"
#include "cmp_B.h"
#include "cmp_C.h"

static void _stop();
//----------------------------------------------------------------------------------------------------
//------------------------- State transitions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------
fsm_states_t _fsm_run_trans(int32_t event, void *arg)
{
	CMP_DEF_THIS(fsm_t, get_fsm());

	switch(event)
	{
	case FSM_EVENT_STOP:
		_stop();
		return FSM_STATE_HALT;
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

void _stop()
{
	//stop components
	get_cmp_A()->stop();
	get_cmp_B()->stop();
	get_cmp_C()->stop();
}
