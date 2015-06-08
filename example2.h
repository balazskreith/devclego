/*
 * example2.h
 *
 *  Created on: Nov 13, 2014
 *      Author: balazs
 */

#ifndef EXAMPLE2_H_
#define EXAMPLE2_H_
#include "devclego.h"

typedef struct packet_struct_t
{
	char bytes[1024];
}packet_t;

#define CMP_NAME_A "A component"
typedef struct cmp_A_struct_t
{
	void             (*receiver)(packet_t*);
	void             (*send)(packet_t*);
	void*            (*start)();
	void*            (*stop)();
}cmp_A_t;
//--------------------------------------------------
//A component ends (cmp_A)
//----------------------------------------------------


void cmp_A_ctor();
void cmp_A_dtor();
cmp_A_t* get_cmp_A();

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

#endif /* EXAMPLE2_H_ */
