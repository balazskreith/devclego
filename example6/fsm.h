#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE6_ON

#ifndef INCGUARD_FSM_H_
#define INCGUARD_FSM_H_

typedef enum
{
	FSM_STATE_HALT,
	FSM_STATE_RUN,
}fsm_states_t;

typedef enum
{
	FSM_EVENT_START             = 1,
	FSM_EVENT_STOP              = 2,
}fsm_events_t;

typedef struct fsm_mpt_struct_t
{
	fsm_states_t         current;
	char_t               state_str[255];
	char_t               event_str[255];
	void               (*fire)(int32_t, void*);
	signal_t            *signal;
}fsm_t;

void fsm_ctor();
void fsm_dtor();
fsm_t* get_fsm();

//private tools
void fsm_str_state(int32_t state, char_t *dst);
void fsm_str_event(int32_t event, char_t *dst);

#endif /* INCGUARD_FSM_H_ */
#endif /* EXAMPLE6_ON */
