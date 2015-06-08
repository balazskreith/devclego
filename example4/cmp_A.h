/*-------------------------------------------------------------------------*/
/**
   @file    cmp_A.h
   @author  B. Kreith
   @brief   "A" component
*/
/*--------------------------------------------------------------------------*/
#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE4_ON

#ifndef CMP_A_H_
#define CMP_A_H_

//--------------------------------------------------
//A component begins (cmp_A)
//----------------------------------------------------
#define CMP_NAME_A "A component"
typedef struct cmp_A_struct_t
{
	void             (*receiver)(packet_t*);
	void             (*sendOdd)(packet_t*);
	void             (*sendEven)(packet_t*);
	void*            (*start)();
	void*            (*stop)();
}cmp_A_t;
//--------------------------------------------------
//A component ends (cmp_A)
//----------------------------------------------------


void cmp_A_ctor();
void cmp_A_dtor();
cmp_A_t* get_cmp_A();

#endif /* CMP_A_H_ */

#endif /* EXAMPLE4_ON */
