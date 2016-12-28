#include "fsm.h"
#include "dmap.h"
#include "etc_utils.h"
#include "fsm_actions.h"
#include "inc_opcall.h"

//components:
#include "cmp_predefs.h"
#include "cmp_defs.h"

static void _sys_stop();
//----------------------------------------------------------------------------------------------------
//------------------------- Transitions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

fsm_states_t _fsm_run_trans(int32_t event, void *arg)
{
	CMP_DEF_THIS(fsm_t, get_fsm());

	switch(event)
	{
	case EVENT_STOP:
		_sys_stop();
		return STATE_HALT;
		break;
	default:
		WARNINGPRINT("The event (%s) you required doesn't have transition in the current state (%s)", this->event_str, this->state_str);
		break;
	}

	return this->current;
}



//----------------------------------------------------------------------------------------------------
//------------------------- Actions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _sys_stop()
{
  //Stop all the components
  //cmp_get_cmdexetor()->stop()

  //if components are in table
  //dmap_itr_do(dmap_itr_table_pcapls, get_cmp_sniffer()->stop);
}

