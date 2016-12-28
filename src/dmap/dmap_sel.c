#include "dmap_sel.h"
#include "dmap.h"
#include "lib_defs.h"
#include "lib_descs.h"
#include "inc_texts.h"
#include "lib_debuglog.h"

/*
void dmap_rem_pths_byconid(int32_t con_dmap_id)
{
        int32_t    index;
        path_t    *path = NULL;
        for(index = 0; dmap_itr_table_pth(&index, &path) == BOOL_TRUE; ++index){
                if(path->con_dmap_id != con_dmap_id){
                        continue;
                }
                dmap_rem_pth_byindex(index);
        }
}
*/


//Example for searching dmap for field
//operation_t* dmap_get_operation_by_name(char_t* name)
//{
//  int32_t    index;
//  operation_t* result = NULL;
//  for(index = 0; dmap_itr_table_operations(&index, &result) == BOOL_TRUE; ++index){
//      if(strcmp(name, result->name) == 0){
//        return result;
//      }
//  }
//  return NULL;
//}


