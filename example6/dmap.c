#include "../defs.h"
#include "../devclego.h"
#ifdef EXAMPLE6_ON

#include "dmap.h"

#include <string.h>

static void config_dtor(config_t*);
DMAP_DEF_SPECT_TABLE(																	 		 \
	config_t,		 	   /*name of the specific data type*/									 \
	config_dtor,		   /*name of the destructor for the specified data*/					 \
	_dmap_table_config,	   /*name of the variable reference to the table*/						 \
	"Configurations",         /*name of the table*/						 	     				 \
	10,                       /*maximal number of item the table can contain. */                    \
	dmap_init_table_config,   /*name of the process initialize the table*/                          \
	dmap_deinit_table_config, /*name of the process deinitialze the table*/                         \
	dmap_get_table_config,	   /*name of the process gets the table*/								 \
	dmap_get_config,		   /*name of the process gets an item from the table by index*/			 \
	dmap_add_config,		   /*name of the process adds an item and returns with its index*/  	 \
	dmap_rem_config_byindex,  /*name of the process remove an item from the table by index*/   	 \
	dmap_rem_config, 		   /*name of the process remove an item from the table by pointer*/ 	 \
	dmap_rdlock_table_config,  /*name of the process lock the table for reading*/					 \
	dmap_rdunlock_table_config,/*name of the process unlock the table from reading*/				 \
	dmap_wrlock_table_config,  /*name of the process lock the table for writing*/					 \
	dmap_wrunlock_table_config,/*name of the process unlock the table from writing*/				 \
	dmap_itr_table_config	   /*name of the process iterate the table*/							 \
);

void config_dtor(config_t *config)
{
	//destructing config
}

config_t* dmap_get_config_byname(char_t *name)
{
	config_t *result;
	int32_t   index;
	for(index = 0; dmap_itr_table_config(&index, &result) == BOOL_TRUE; ++index){
		if(strcmp(result->name, name) != 0){
			continue;
		}
		break;
	}
	if(index == 10){
		return NULL;
	}
	return result;
}


void dmap_init()
{
	dmap_init_table_config();
}

void dmap_deinit()
{
	dmap_deinit_table_config();
}
#endif /* EXAMPLE6_ON */

