/*-------------------------------------------------------------------------*/
/**
   @file    cmp_C.h
   @author  B. Kreith
   @brief   "A" component
*/
/*--------------------------------------------------------------------------*/
#ifndef CMP_C_H_
#define CMP_C_H_
#include "dmap.h"

//--------------------------------------------------
//A component begins (cmp_C)
//----------------------------------------------------
#define CMP_NAME_C "C component"
typedef struct cmp_C_struct_t
{
	void             (*receiver)(packet_t*);
	void             (*send)(packet_t*);
	void*            (*start)();
	void*            (*stop)();
}cmp_C_t;
//--------------------------------------------------
//A component ends (cmp_C)
//----------------------------------------------------


void cmp_C_ctor();
void cmp_C_dtor();
cmp_C_t* get_cmp_C();

#endif /* CMP_C_H_ */
