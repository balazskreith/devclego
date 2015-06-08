#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE6_ON

#include "fsm.h"
#include "fsm_halt.h"
#include "fsm_run.h"

#include "cmp_A.h"
#include "cmp_B.h"
#include "cmp_C.h"

static void  _fsm_init();
static void  _fsm_deinit();
#define FSM_NAME_MPT "System machine"
CMP_DEF(, 			         /*type of definitions*/
		fsm_t,       /*type of component*/
		FSM_NAME_MPT,   /*name of the component*/
		 _fsm,        /*variable name of the component*/
		 fsm_ctor,    /*name of the constructor function implemented automatically*/
		 fsm_dtor,    /*name of the destructor function implemented automatically*/
		 _fsm_init,   /*name of the external function called after construction*/
		 __NO_TEST_FUNC_,      /*name of the external function called after initialization*/
		 _fsm_deinit  /*name of the external function called before destruction*/
		);

CMP_DEF_GET_PROC(get_fsm, fsm_t, _fsm);

static config_t _config;

//fire function
static void _fsm_fire(int, void*);

//private tools
static void _str_state(int32_t state, char_t *dst);
static void _str_event(int32_t event, char_t *dst);
//----------------------------------------------------------------------------------------------------
//------------------------- Initializations  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _fsm_init()
{
	CMP_DEF_THIS(fsm_t, _fsm);
	this->signal = signal_ctor();
	signal_lock(this->signal);

	dmap_init();
	dmap_wrlock_table_config();
	strcpy(_config.name, "conf");
	_config.divider = 3;
	dmap_add_config(&_config);
	dmap_wrunlock_table_config();

	//Component constructions
	cmp_A_ctor();
	cmp_B_ctor();
	cmp_C_ctor();

	CMP_BIND(_fsm->fire, _fsm_fire);

	//connect components
	CMP_CONNECT(get_cmp_A()->sendEven, get_cmp_B()->receiver);
	CMP_CONNECT(get_cmp_A()->sendOdd,  get_cmp_C()->receiver);


	signal_unlock(this->signal);
	//start value:
	this->current = FSM_STATE_HALT;
}

void _fsm_deinit()
{
	CMP_DEF_THIS(fsm_t, _fsm);
	signal_lock(this->signal);

	//destructing components
	cmp_C_dtor();
	cmp_B_dtor();
	cmp_A_dtor();

	dmap_deinit();

	signal_unlock(this->signal);
	signal_dtor(this->signal);
}

//----------------------------------------------------------------------------------------------------
//------------------------- FIRE  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _fsm_fire(int32_t event, void *arg)
{
	CMP_DEF_THIS(fsm_t, _fsm);
	signal_lock(this->signal);

	_str_event(event, this->event_str);
	_str_state(this->current, this->state_str);
	INFOPRINT("FSM: %s event is fired, actual state is: %s", this->event_str, this->state_str);

	switch(this->current)
	{
	case FSM_STATE_RUN:
		this->current = _fsm_run_trans(event, arg);
			break;
	case FSM_STATE_HALT:
		this->current = _fsm_halt_trans(event, arg);
			break;
	default:
		ERRORPRINT("Machine is in unknown state");
	}

	_str_state(this->current, this->state_str);
	INFOPRINT("FSM: Transitions performed, the actual state is: %s (%d)", this->state_str, this->current);
	signal_unlock(this->signal);
}


//----------------------------------------------------------------------------------------------------
//------------------------- Private tools  ---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void _str_state(int32_t state, char_t *dst)
{
	char_t str[255];
	switch(state)
	{
	case FSM_STATE_RUN:
		strcpy(dst, "Run");
		break;
	case FSM_STATE_HALT:
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
	case FSM_EVENT_START:
		strcpy(dst, "Start");
		break;
	case FSM_EVENT_STOP:
		strcpy(dst, "Stop");
		break;
	default:
		sprintf(str, "Unknown (%d)", event);
		strcpy(dst, str);
		break;
	}
}
#endif /* EXAMPLE6_ON */
