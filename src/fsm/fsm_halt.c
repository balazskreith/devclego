#include "fsm.h"
#include "fsm_actions.h"
#include "cmp_predefs.h"
#include "etc_iniparser.h"
#include "dmap.h"
#include "fsm_cmpactions.h"
#include "sys_confs.h"

//components:
#include "cmp_predefs.h"
#include "cmp_defs.h"
#include "dmap.h"

static void _sys_setup(char_t* config_file);
static void _sys_start();
static void _sys_shutdown();

//----------------------------------------------------------------------------------------------------
//------------------------- Transitions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

fsm_states_t _fsm_halt_trans(int32_t event, void *arg)
{
	CMP_DEF_THIS(fsm_t, get_fsm());

	switch(event)
	{
	  case EVENT_SETUP:
	    _cmps_ctor();
	    _sys_setup((char_t*) arg);
	    return STATE_HALT;
	  case EVENT_START:
        _sys_start();
        return STATE_RUN;
	  case EVENT_SHUTDOWN:
        _sys_shutdown();
        _cmps_dtor();
        return STATE_HALT;
	  default:
	    WARNINGPRINT("The event (%s) you required doesn't have transition in the current state (%s)", this->event_str, this->state_str);
	    break;
	}

	return this->current;
}



//----------------------------------------------------------------------------------------------------
//------------------------- Actions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _sys_setup(char_t* config_file)
{
  if(!config_file){
    WARNINGPRINT("Config file is not defined");
  }else{
    config_load_from_file(config_file);
  }

}

void _sys_start()
{

  //Start all components
  //example:
  //get_cmp_cmdexecutor()->start();

  //If the components registered inside of a table you can do:
  //dmap_itr_do(dmap_itr_table_pcapls, get_cmp_sniffer()->start);

}

void _sys_shutdown()
{
  dmap_itr_do(dmap_itr_table_thr, dmap_rem_thr);
}



