/*-------------------------------------------------------------------------*/
/**
   @file    cmp_B.h
   @author  B. Kreith
   @brief   "A" component
*/
/*--------------------------------------------------------------------------*/
#ifndef CMP_B_H_
#define CMP_B_H_
#include "dmap.h"

//--------------------------------------------------
//A component begins (cmp_B)
//----------------------------------------------------
#define CMP_NAME_B "B component"
typedef struct cmp_B_struct_t
{
	void             (*receiver)(packet_t*);
	void             (*send)(packet_t*);
	void*            (*start)();
	void*            (*stop)();
}cmp_B_t;
//--------------------------------------------------
//A component ends (cmp_B)
//----------------------------------------------------


void cmp_B_ctor();
void cmp_B_dtor();
cmp_B_t* get_cmp_B();

#endif /* CMP_B_H_ */
