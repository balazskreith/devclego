/*-------------------------------------------------------------------------*/
/**
   @file    defs.h
   @author  B. Kreith
   @brief   common definitions
*/
/*--------------------------------------------------------------------------*/
#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE6_ON

#ifndef DMAP_H_
#define DMAP_H_

typedef struct config_struct_t
{
	char_t  name[255];
	int32_t divider;
}config_t;

config_t* dmap_get_config_byname(char_t*);
DMAP_DECL_SPECT_TABLE(
	config_t,		           /*type of the items stored in the table*/
	dmap_get_table_config,	   /*name of the process gets the table*/
	dmap_get_config,		   /*name of the process gets an item from the table by index*/
	dmap_add_config,		   /*name of the process adds an item and returns with its index*/
	dmap_rem_config_byindex,  /*name of the process remove an item from the table by index*/
	dmap_rem_config, 		   /*name of the process remove an item from the table by pointer*/
	dmap_rdlock_table_config,  /*name of the process lock the table for reading*/
	dmap_rdunlock_table_config,/*name of the process unlock the table from reading*/
	dmap_wrlock_table_config,  /*name of the process lock the table for writing*/
	dmap_wrunlock_table_config,/*name of the process unlock the table from writing*/
	dmap_itr_table_config	   /*name of the process iterate the table*/
);
#endif /* DMAP_H_ */
#endif /* EXAMPLE6_ON */

