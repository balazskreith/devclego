#include "fsm.h"
#include "lib_tors.h"
#include "dmap.h"
#include "inc_predefs.h"
#include "sys_confs.h"
#include "lib_makers.h"
#include "lib_dispers.h"

#include "cmp_defs.h"
#include "cmp_predefs.h"

//states:
#include "fsm_halt.h"
#include "fsm_run.h"

static void  _fsm_ntrt_init();
static void  _fsm_ntrt_deinit();
#define FSM_NAME "Main system machine"
CMP_DEF(, 			         /*type of definitions*/ 										   \
		fsm_t,       /*type of component*/ 										  	   \
		FSM_NAME,     /*name of the component*/ 										   \
		 _fsm,        /*variable name of the component*/  							   \
		 fsm_ctor,    /*name of the constructor function implemented automatically*/     \
		 fsm_dtor,    /*name of the destructor function implemented automatically*/      \
		 _fsm_ntrt_init,   /*name of the external function called after construction*/        \
		 __NO_TEST_FUNC_,      /*name of the external function called after initialization*/       \
		 _fsm_ntrt_deinit  /*name of the external function called before destruction*/        \
		);																						   \

CMP_DEF_GET_PROC(get_fsm, fsm_t, _fsm);

//fire function
static void _fsm_fire(int, void*);

//private tools
static void _str_state(int32_t state, char_t *dst);
static void _str_event(int32_t event, char_t *dst);


//----------------------------------------------------------------------------------------------------
//------------------------- Initializations  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _fsm_ntrt_init()
{
	CMP_DEF_THIS(fsm_t, _fsm);

	//Construct
	this->signal = signal_ctor();

	dmap_init();

	//Binding
	CMP_BIND(this->fire, _fsm_fire);

	//start value:
	this->current = STATE_HALT;
}

void _fsm_ntrt_deinit()
{
	CMP_DEF_THIS(fsm_t, _fsm);

        //Destruct
        dmap_deinit();

	signal_dtor(this->signal);
}

//----------------------------------------------------------------------------------------------------
//------------------------- FIRE  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _fsm_fire(int32_t event, void *arg)
{
	CMP_DEF_THIS(fsm_t, _fsm);

	_str_event(event, this->event_str);
	_str_state(this->current, this->state_str);
	INFOPRINT("FSM: %s event is fired, actual state is %s", this->event_str, this->state_str);

	switch(this->current)
	{
	case STATE_RUN:
		this->current = _fsm_run_trans(event, arg);
			break;
	case STATE_HALT:
		this->current = _fsm_halt_trans(event, arg);
			break;
	default:
		ERRORPRINT("Machine is in unknown state");
	}

	_str_state(this->current, this->state_str);
	INFOPRINT("FSM: Transitions performed, the actual state is %s ", this->state_str, this->current);
}



//----------------------------------------------------------------------------------------------------
//------------------------- Actions  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------


void _str_state(int32_t state, char_t *dst)
{
	char_t str[255];
	switch(state)
	{
	case STATE_RUN:
		strcpy(dst, "Run");
		break;
	case STATE_HALT:
		strcpy(dst, "Halt");
		break;
	default:
		sprintf(str, "Unknown (%d)", state);
		strcpy(dst, str);
		break;
	}
}

void _str_event(int32_t event, char_t *dst)
{
	char_t str[255];
	switch(event)
	{
	case EVENT_SETUP:
		strcpy(dst, "Setup");
		break;
	case EVENT_SHUTDOWN:
		strcpy(dst, "Shutdown");
		break;
	case EVENT_START:
		strcpy(dst, "Start");
		break;
	case EVENT_STOP:
		strcpy(dst, "Stop");
		break;
	default:
		sprintf(str, "Unknown (%d)", event);
		strcpy(dst, str);
		break;
	}
}
