/*! \file devlego.h
\brief Definitions and macros for devlego framework
*/

#ifndef DEVCLEGO_H_
#define DEVCLEGO_H_
#include <stdio.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <string.h>
#include <errno.h>        /* errno */
#include <sys/cdefs.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

/** \typedef bool_t
      \brief Boolean constants
*/
typedef enum
{
	BOOL_FALSE = 0,  //!< Indicates a boolean false value
	BOOL_TRUE = 1    //!< Indicates a boolean true value
}bool_t;


/*! \var typedef char char_t
    \brief A type definition for a character

    Details.
*/
typedef char char_t;  //!< 8 bit length character

/*! \var typedef u_int8_t uint8_t;
    \brief A type definition for an 8 bit length unsigned integer

    Details.
*/
typedef u_int8_t uint8_t;

/*! \var typedef u_int16_t uint16_t;
    \brief A type definition for a 16 bit length unsigned integer

    Details.
*/
typedef u_int16_t uint16_t;  //!< 16 bit length unsigned integer

/*! \var typedef u_int32_t uint32_t;
    \brief A type definition for a 32 bit length unsigned integer

    Details.
*/
typedef u_int32_t uint32_t;

/*! \var typedef u_int64_t uint64_t;
    \brief A type definition for a 64 bit length unsigned integer

    Details.
*/
typedef u_int64_t uint64_t;

/*! \var typedef unsigned char byte_t
    \brief A type definition for a byte

    Details.
*/
typedef unsigned char byte_t;


/*! \var typedef unsigned char byte_t
    \brief A type definition for an io functionality of the devlego framework
*/


/** @struct _devlegoio_t
 *  @brief A type definition for an io functionality of the devlego framework
 *  @var _devlegoio_t::print_stdout
 *  pointing to a function handling the printing outputs from devlego
 *  @var _devlegoio_t::print_stdlog
 *  pointing to a function handling the log outputs from devlego
 */
typedef struct _devlegoio_struct_t
{
	//print functionalities
	int     		 (*print_stdout)(const char*,...);
	int     		 (*print_stdlog)(const char*,...);
	//
}_devlego_t;

/*! \var typedef pthread_spinlock_t    spin_t;
    \brief A type definition for spinlock
*/
typedef pthread_spinlock_t    spin_t;

/*! \var typedef  pthread_mutex_t       mutex_t;
    \brief A type definition for mutex
*/
typedef pthread_mutex_t       mutex_t;

/*! \var typedef  pthread_cond_t        cond_t;
    \brief A type definition for a conditional waiting
*/
typedef pthread_cond_t        cond_t;

/*! \var typedef  pthread_t             threader_t;
    \brief A type definition for a thread handler
*/
typedef pthread_t             threader_t;

/*! \var typedef  pthread_attr_t        threader_attr_t;
    \brief A type definition for a thread attribute handler
*/
typedef pthread_attr_t        threader_attr_t;

/*! \var typedef  pthread_rwlock_t      rwmutex_t;
    \brief A type definition for a read write mutex
*/
typedef pthread_rwlock_t      rwmutex_t;

/*! \var typedef  pthread_rwlockattr_t  rwmutex_attr_t;
    \brief A type definition for a read write mutex
*/
typedef pthread_rwlockattr_t  rwmutex_attr_t;

/** @struct dmap_row_struct_t
 *  @brief A type definition for a row of the datamap table
 *  @var dmap_row_struct_t::item
 *  pointing to the item
 *  @var dmap_row_struct_t::dtor
 *  pointing to the destructor of an item
 */
typedef struct dmap_row_struct_t{
	void *item;
	void (*dtor)(void*);
}dmap_row_t;

/** @struct dmap_table_struct_t
 *  @brief A type definition for a datamap table
 *  @var dmap_row_t::name
 *  name of the table. Used for debug purpuse
 *  @var dmap_table_struct_t::size
 *  Indicates the maximal size of the datamap table
 *  @var dmap_table_struct_t::rwmutex
 *  used for lock the table for reading or writing
 *  @var dmap_table_struct_t::rows
 *  pointing to the rows
 */
typedef struct dmap_table_struct_t{
	char_t      		name[64];
	int32_t     		size;
	rwmutex_t           *rwmutex;
	dmap_row_t 			**rows;
}dmap_table_t;

/*! \var int errno
	\brief Contains the last error code.
	\warning Not thread safe!
*/
extern int errno;

/*! \var _devlegoio_t *_devlegoio;
	\brief Pointing to a _devlegoio variable
	Used for debug purpuse
*/
_devlego_t *_devlegoio;

/*! \fn void devlego_init()
\brief initialize the devlego framework
*/
void devlego_init();
/*! \fn void devlego_deinit()
\brief deinitialize the devlego framework
*/
void devlego_deinit();
/*! \fn void __noprint(const char*,...)
\brief provide the same signature as the printf, but do not print anything
it is generally used by the _devlegoio for directing the stdlog or
stdio to a null printer.
*/
int __noprint(const char*,...);
//----------------------------------------------------

//original macros are available at:http://c.learncodethehardway.org/book/ex20.html

/*! clean the errno variable */
#define _CLEAN_ERRNO() (errno == 0 ? "None" : strerror(errno))

/*! A critical printout */
#define CRITICALPRINT(M, ...) _devlegoio->print_stdout("[CRITIC] "M"\n", __VA_ARGS__)

/*! An error printout */
#define ERRORPRINT(M, ...) _devlegoio->print_stdout("[ERR] (errno: %s) "M"\n", _CLEAN_ERRNO(), ##__VA_ARGS__)

/*! A warning printout */
#define WARNINGPRINT(M, ...)
//_devlegoio->print_stdout("[WARN] (errno: %s) "M"\n",  _CLEAN_ERRNO(), ##__VA_ARGS__)

/*! An info printout */
#define INFOPRINT(M, ...) _devlegoio->print_stdout("[INFO] "M"\n",  ##__VA_ARGS__)

/*! A debug printout */
#define DEBUGPRINT(M, ...) _devlegoio->print_stdout("[DEBUG] "M"\n",  ##__VA_ARGS__)

/*! A reference for testing feature */
#ifdef TEST_ENABLED
/*! assert test. Test A. If it is false then print M with the following arguments and exit.*/
#define ASSERT_TRUE(A, M, ...) if(!(A)) { ERRORPRINT(M, ##__VA_ARGS__); errno=0; EXERROR("Test assertation failed", EXIT_FAILURE); }
/*! Breakpoint test. Print M with the following arguments and exit*/
#define SENTINEL(M, ...)  { ERRORPRINT(M, ##__VA_ARGS__); errno=0; EXERROR("Program reached a sentinel point.", EXIT_FAILURE); }
#else
/*! false assert test. print nothing, test noting, no exit*/
#define ASSERT_TRUE(A, M, ...)
/*! false breakpoint test. print nothing, no exit.*/
#define SENTINEL(M, ...)
#endif

/*! exit and print error informations*/
#define EXERROR(exit_text,exit_num)  {ERRORPRINT("%s:%d", exit_text, exit_num); exit(exit_num);}
/*! fill up the start memory address plus the size with 0*/
#define BZERO(mem_addr, size) memset(mem_addr, 0, size)

//debuglog definitions
/*! print to the log output a debug message*/
#define DEBUGLOG(TYPE, M, ...) _devlegoio->print_stdlog("[DEBUG]"TYPE" %s:%d: "M"\n", __FILE__, __LINE__, ##__VA_ARGS__)
/*! print to the log output a flow debug message*/
#define FLOWDEBUG(M,...) DEBUGLOG("[FLOW]", M, __VA_ARGS__)
/*! print to the log output a memory debug message*/
#define MEMDEBUG(M,...)  DEBUGLOG("[MEM]", M, __VA_ARGS__)
/*! print to the log output a performance debug message*/
#define PERFDEBUG(M,...) DEBUGLOG("[PERF]", M, __VA_ARGS__)
/*! print to the log output a variable debug message*/
#define VARDEBUG(M,...) DEBUGLOG("[VAR]", M, __VA_ARGS__)
/*! print to the log output a lock debug message*/
#define LOCKDEBUG(M,...) DEBUGLOG("[LOCKS]", M, __VA_ARGS__)
/*! print to the log output a type debug message*/
#define TYPEDEBUG(M,...) DEBUGLOG("[TYPE]", M, __VA_ARGS__)
/*! print to the log output a warning debug message*/
#define WARNDEBUG(M,...) DEBUGLOG(" Warning: ", M, __VA_ARGS__)
/*! print to the log output a log debug message*/
#define LOGDEBUG(M,...) DEBUGLOG(" Log: ", M, __VA_ARGS__)

/*! printing a text as an info using a macro named GET_TEXT_##TYPE with the other arguments*/
#define PRINTING(TYPE, ...) INFOPRINT(GET_TEXT_##TYPE(__VA_ARGS__))

/*! replace the symbol to Start [sg]*/
#define GET_TEXT_STARTING_SG(sg, ...) "Start "sg
/*! printing by using get_text_starting*/
#define PRINTING_STARTING_SG(sg, ...) PRINTING(STARTING_SG, sg, __VA_ARGS__)

/*! replace the symbol to Start [sg] (FORSG)*/
#define PRINTING_STARTING_SG_FORSG(sg, FORSG) INFOPRINT("%s (%s)", GET_TEXT_STARTING_SG(sg), FORSG)

/*! replace the symbol to [sg] is started*/
#define GET_TEXT_SG_IS_STARTED(sg) sg" is started."
/*! printing by using get_text_sg_is_started*/
#define PRINTING_SG_IS_STARTED(sg) INFOPRINT("%s", GET_TEXT_SG_IS_STARTED(sg))

/*! replace the symbol to Stop [sg]*/
#define GET_TEXT_STOPPING_SG(sg) "STOP "sg"."
/*! printing by using get_text_stopping_sg*/
#define PRINTING_STOPPING_SG(sg) INFOPRINT("%s", GET_TEXT_STOPPING_SG(sg))

/*! replace the symbol to Stop [sg] (FORSG)*/
#define PRINTING_STOPPING_SG_FORSG(sg, FORSG) INFOPRINT("%s (%s)", GET_TEXT_STOPPING_SG(sg), FORSG)

/*! replace the symbol to [sg] is stopped*/
#define GET_TEXT_SG_IS_STOPPED(sg) sg" is stopped."
/*! printing by using get_text_sg_is_stopped*/
#define PRINTING_SG_IS_STOPPED(sg) INFOPRINT("%s", GET_TEXT_SG_IS_STOPPED(sg))

/*! replace the symbol to constructing [sg]*/
#define GET_TEXT_CONSTRUCTING_SG(sg) "Constructing "sg"."
/*! printing by using get_text_constrcting_sg*/
#define PRINTING_CONSTRUCTING_SG(sg) INFOPRINT("%s", GET_TEXT_CONSTRUCTING_SG(sg))

/*! replace the symbol to [sg] is constructed*/
#define GET_TEXT_SG_IS_CONSTRUCTED(sg) sg" is constructed"
/*! printing by using get_text_sg_is_constructed*/
#define PRINTING_SG_IS_CONSTRUCTED(sg) INFOPRINT("%s", GET_TEXT_SG_IS_CONSTRUCTED(sg))

/*! replace the symbol to Destructing [sg]*/
#define GET_TEXT_DESTRUCTING_SG(sg) "Destructing "sg"."
/*! printing by using get_text_destructing_sg*/
#define PRINTING_DESTRUCTING_SG(sg) INFOPRINT("%s", GET_TEXT_DESTRUCTING_SG(sg))

/*! replace the symbol to [sg] is destructed*/
#define GET_TEXT_SG_IS_DESTRUCTED(sg) sg" is destructed."
/*! printing by using get_text_is_destructed*/
#define PRINTING_SG_IS_DESTRUCTED(sg) INFOPRINT("%s", GET_TEXT_SG_IS_DESTRUCTED(sg))

/*! replace the symbol to Testing [sg]*/
#define GET_TEXT_TESTING_SG(sg) "Testing "sg"."
/*! printing by using get_text_testing_sg*/
#define PRINTING_TESTING_SG(sg) logging("%s", GET_TEXT_TESTING_SG(sg))

/*! replace the symbol to [sg] is not exist*/
#define GET_TEXT_SG_NOTEXISTS(sg) sg" is not exists."
/*! printing by using get_text_sg_noexists*/
#define PRINTING_SG_NOTEXISTS(sg) runtime_warning("%s",GET_TEXT_SG_NOTEXISTS(sg))

/*! replace the symbol to Missing connection between [A] and [B]*/
#define GET_TEXT_CONNECTION_NOTEXISTS(A,B) "Missing connection between "A" and "B"."
/*! replace the symbol to assign is missing at [sg]*/
#define GET_TEXT_ASSIGN_NOTEXISTS(SG) "Assign is missing at "SG"."

/*! replace the symbol to [sg] is already exists*/
#define GET_TEXT_SG_ALREADY_EXISTS(sg) sg" is already exists."
/*! printing by using get_text_sg_already_exists*/
#define PRINTING_SG_ALREADY_EXISTS(sg) runtime_warning("%s", GET_TEXT_SG_ALREADY_EXISTS(sg))

/*! replace the symbol to Initializing [sg]*/
#define GET_TEXT_INIT_SG(sg) "Initializing "sg"."
/*! printing by using get_text_init_sg*/
#define PRINTING_INIT_SG(sg) logging("%s", GET_TEXT_INIT_SG(sg))

/*! replace the symbol to Deinitializing [sg]*/
#define GET_TEXT_DEINIT_SG(sg) "Deinitializing "sg"."
/*! printing by using get_text_deinit_sg*/
#define PRINTING_DEINIT_SG(sg) logging("%s", GET_TEXT_DEINIT_SG(sg))

/*! replace the symbol to Assign components*/
#define GET_TEXT_ASSIGN_COMPONENTS "Assign components"
/*! printing by using get_text_sg_assign_components*/
#define PRINTING_ASSIGN_COMPONENTS logging("%s", GET_TEXT_ASSIGN_COMPONENTS)

/*! printing by using get_text_sg_to_proc*/
#define PRINTING_BIND_SG_TO_PROC(SG,PROC) logging("Bind %s(%p) to %s(%p)", #SG, SG, #PROC, PROC)

/*! printing by using get_text_connect_sg1_to_sg2*/
#define PRINTING_CONNECT_SG1_TO_SG2(SG1,SG2) logging("Connect %s(%p) to %s(%p)", #SG1, SG1, #SG2, SG2)
/*! printing by using get_text_assigning_sg1_to_sg2*/
#define PRINTING_ASSIGNING_SG1_TO_SG2(SG1,SG2) logging("Assign %s(%p) to %s(%p)", #SG1, SG1, #SG2, SG2)

/*! replace the symbol to Connect components*/
#define GET_TEXT_CONNECT_COMPONENTS "Connect components"
/*! printing by using get_text_sg_components*/
#define PRINTING_CONNECT_COMPONENTS logging("%s", GET_TEXT_CONNECT_COMPONENTS)

/*! replace the symbol to Initialization process is completed*/
#define GET_TEXT_INIT_COMPLETED "Initialization process is completed"
/*! printing by using get_text_init_completed*/
#define PRINTING_INIT_COMPLETED logging("%s", GET_TEXT_INIT_COMPLETED)

/*! replace the symbol to Deinitialization process is completed*/
#define GET_TEXT_DEINIT_COMPLETED "Deinitialization process is completed"
/*! printing by using get_text_deinit_completed*/
#define PRINTING_DEINIT_COMPLETED logging("%s", GET_TEXT_DEINIT_COMPLETED)

/*! replace the symbol to Check [sg]*/
#define GET_TEXT_CHECK_SG(sg) "Check "sg"."
/*! printing by using get_text_printing_check_sg*/
#define PRINTING_CHECK_SG(sg) logging("%s", GET_TEXT_CHECK_SG(sg))

/*! replace the symbol to Check is completed*/
#define GET_TEXT_CHECK_IS_COMPLETED "Check is completed"
/*! printing by using get_text_sg_check_is_completed*/
#define PRINTING_CHECK_IS_COMPLETED logging("%s", GET_TEXT_CHECK_IS_COMPLETED)

/*! replace the symbol to Test is completed*/
#define GET_TEXT_TEST_IS_COMPLETED "Test is completed"
/*! printing by using get_text_sg_is_completed*/
#define PRINTING_TEST_IS_COMPLETED logging("%s", GET_TEXT_CHECK_IS_COMPLETED)


#if defined DEBUG_FLOWS || defined DEBUG_ALL
/*! printing flow debug message*/
#define debug_flow(M, ...) FLOWDEBUG(M, __VA_ARGS__)
#else
/*! printing nothing*/
#define debug_flow(M, ...)
#endif

#if defined DEBUG_LOCKS || defined DEBUG_ALL
/*! printing read lock debug message*/
#define debug_rdlock(lockname) LOCKDEBUG("%s is locked for reading", lockname)
/*! printing read unlock debug message*/
#define debug_rdunlock(lockname) LOCKDEBUG("%s is unlocked from reading", lockname)
/*! printing write lock debug message*/
#define debug_wrlock(lockname) LOCKDEBUG("%s is locked for writing", lockname)
/*! printing write unlock debug message*/
#define debug_wrunlock(lockname) LOCKDEBUG("%s is unlocked from writing", lockname)
/*! printing lock debug message*/
#define debug_lock(lockname) LOCKDEBUG("%s is locked", lockname)
/*! printing lock debug message*/
#define debug_unlock(lockname) LOCKDEBUG("%s is unlocked", lockname)
#else
/*! print nothing*/
#define debug_rdlock(lockname)
/*! print nothing*/
#define debug_rdunlock(lockname)
/*! print nothing*/
#define debug_wrlock(lockname)
/*! print nothing*/
#define debug_wrunlock(lockname)
/*! print nothing*/
#define debug_lock(lockname)
/*! print nothing*/
#define debug_unlock(lockname)
#endif

#if defined DEBUG_LOCKCALLS || defined DEBUG_ALL
/*! printing lock call debug message*/
#define debug_lockcall(function_name) LOCKDEBUG("%s is called", function_name)
#else
#define debug_lockcall(function_name)
#endif

#if defined(RUNTIME_WARNINGS) || defined(DEBUG_ALL)
/*! printing runtime warning message*/
#define runtime_warning(M, ...) WARNINGPRINT(M, __VA_ARGS__)
#else
#define runtime_warning(M, ...)
#endif


#if defined LOGGING_ENABLED || defined DEBUG_ALL
/*! printing lock debug message*/
#define logging(M, ...) LOGDEBUG(M, __VA_ARGS__)
#else
#define logging(M, ...)
#endif


//dmap macros

/*! declare a get accessor process with a return type passed by parameters*/
#define DMAP_DECL_GET_DATA_PROC(																	\
								 PROC_NAME,															\
								 RETURN_TYPE														\
								 ) 																	\
	RETURN_TYPE* PROC_NAME();				  														\

/*! define a get accessor process with return type and
 * the data pointer passed by parameter*/
#define DMAP_DEF_GET_DATA_PROC(						  												\
								PROC_NAME, 			  												\
								RETURN_TYPE,		  												\
								DATA_PTR			  												\
								) 					  												\
	RETURN_TYPE* PROC_NAME()				 		  												\
	{												 												\
		return DATA_PTR;							  												\
	}																								\


/*! declare a mutex lock process with a name passed as a parameter*/
#define DMAP_DECL_M_LOCK_PROC(																		\
								  PROC_NAME															\
								 )		  	 	   													\
	void PROC_NAME();   																			\


/*! define a mutex lock process with a name, return type and
 * the data pointer passed by parameter*/
#define DMAP_DEF_M_LOCK_PROC(			 	   														\
									  PROC_NAME, 													\
									  MUTEX_PTR,													\
									  DATA_NAME														\
								)			 														\
									  	  	  	  													\
	void PROC_NAME() 																				\
	{																								\
		mutex_lock(MUTEX_PTR);																\
	}																								\


/*! declare a mutex unlock process with a name passed as a parameter*/
#define DMAP_DECL_M_UNLOCK_PROC(PROC_NAME)  														\
	void PROC_NAME();   																			\


/*! define a mutex unlock process with a name, return type and
 * the data pointer passed by parameter*/
#define DMAP_DEF_M_UNLOCK_PROC(			 	    													\
									  PROC_NAME, 													\
									  MUTEX_PTR,													\
									  DATA_NAME														\
								)			 														\
									  	  	  	  													\
	void PROC_NAME() 																				\
	{																								\
		mutex_unlock(MUTEX_PTR);															\
	}																								\



/*! declare a read lock process with a name passed as a parameter*/
#define DMAP_DECL_R_LOCK_PROC(																		\
								  PROC_NAME															\
								 )		  	 	   													\
	void PROC_NAME();   																			\


/*! define a read lock process with a name, return type and
 * the data pointer passed by parameter*/
#define DMAP_DEF_R_LOCK_PROC(			 	   														\
									  PROC_NAME, 													\
									  RWMUTEX_PTR,													\
									  DATA_NAME														\
								)			 														\
									  	  	  	  													\
	void PROC_NAME() 																				\
	{																								\
		rwmutex_read_lock(RWMUTEX_PTR);																\
	}																								\


/*! declare a read unlock process with a name passed as a parameter*/
#define DMAP_DECL_R_UNLOCK_PROC(PROC_NAME)  														\
	void PROC_NAME();   																			\


/*! define a read unlock process with a name, return type and
 * the data pointer passed by parameter*/
#define DMAP_DEF_R_UNLOCK_PROC(			 	    													\
									  PROC_NAME, 													\
									  RWMUTEX_PTR,													\
									  DATA_NAME														\
								)			 														\
									  	  	  	  													\
	void PROC_NAME() 																				\
	{																								\
		rwmutex_read_unlock(RWMUTEX_PTR);															\
	}																								\

/*! declare a write lock process with a name passed as a parameter*/
#define DMAP_DECL_W_LOCK_PROC(																		\
								  PROC_NAME															\
								 )		  	 	   													\
	void PROC_NAME();   																			\

/*! define a write lock process with a name, return type and
 * the data pointer passed by parameter*/
#define DMAP_DEF_W_LOCK_PROC(			 	   														\
									  PROC_NAME, 													\
									  RWMUTEX_PTR,													\
									  DATA_NAME														\
								)			 														\
									  	  	  	  													\
	void PROC_NAME() 																				\
	{																								\
		rwmutex_write_lock(RWMUTEX_PTR);															\
	}																								\


/*! declare a write unlock process with a name passed as a parameter*/
#define DMAP_DECL_W_UNLOCK_PROC(PROC_NAME)  														\
	void PROC_NAME();   																			\

/*! define a write unlock process with a name, return type and
 * the data pointer passed by parameter*/
#define DMAP_DEF_W_UNLOCK_PROC(			 	    													\
									  PROC_NAME, 													\
									  RWMUTEX_PTR,													\
									  DATA_NAME														\
								)			 														\
									  	  	  	  													\
	void PROC_NAME() 																				\
	{																								\
		rwmutex_write_unlock(RWMUTEX_PTR);															\
	}																								\




//---------------------------------------------------------------------------------------------------
//----------------------------- TABLE MACROS --------------------------------------------------------
//---------------------------------------------------------------------------------------------------

/*! declare an add process with a name passed as a parameter*/
#define DMAP_DECL_TABLE_ADD_VOIDT_PROC(																\
										PROC_NAME													\
									  )  			    											\
	int32_t PROC_NAME(void *item, void (*item_dtor)(void*item))     								\
				__attribute__((nonnull (1,2)));   													\


/*! define an add process to a table determined by a
 * table pointer */
#define DMAP_DEF_TABLE_ADD_VOIDT_PROC(																\
										PROC_NAME, 													\
										TABLE_PTR													\
									  )					  		    								\
	int32_t PROC_NAME(void *item, void (*dtor)(void*))   											\
	{																							    \
		int32_t result;																				\
		dmap_row_t *row;																			\
		row = _make_dmap_row(item, dtor);															\
		result = _dmap_add_row(TABLE_PTR, row);														\
		return result;																				\
	}												  												\

/*! declare a table init process*/
#define DMAP_DEF_TABLE_INIT_PROC(PROC_NAME, TABLE_PTR, TABLE_LENGTH, TABLE_NAME)					\
	static void PROC_NAME()     																	\
	{																								\
		_dmap_table_init(TABLE_PTR, TABLE_LENGTH, TABLE_NAME);										\
	}																								\


/*! define a deinit process to a table determined by a
 * table pointer */
#define DMAP_DEF_TABLE_DEINIT_PROC(PROC_NAME, TABLE_PTR)											\
	static void PROC_NAME()     																	\
	{																								\
		_dmap_table_deinit(TABLE_PTR);																\
	}																								\


/*! declare a table add process*/
#define DMAP_DECL_TABLE_ADD_SPECT_PROC( 															\
									  PROC_NAME, 													\
									  TABLE_PTR, 													\
									  DATA_TYPE														\
									  ) 			  												\
	int32_t PROC_NAME(DATA_TYPE* item)       														\
				__attribute__((nonnull (1)));   													\

/*! define an add process to a table determined by a
 * table pointer */
#define DMAP_DEF_TABLE_ADD_SPECT_PROC( 																\
									  PROC_NAME, 													\
									  TABLE_PTR, 													\
									  DATA_TYPE, 													\
									  DATA_DTOR														\
									 )			 	  												\
									  	  	  	  	 											    \
	int32_t PROC_NAME(DATA_TYPE* item)																\
	{																								\
		int32_t result;																				\
		dmap_row_t *row;																			\
		row = _make_dmap_row(item, DATA_DTOR);														\
		result = _dmap_add_row(TABLE_PTR, row);														\
		return result;																				\
	}												  												\

/*! declare a table remove by index process*/
#define DMAP_DECL_TABLE_REM_INDEX_PROC(PROC_NAME)  												    \
	void PROC_NAME(int32_t dmap_index);   														    \


/*! define a remove process to a table determined by a
 * table pointer */
#define DMAP_DEF_TABLE_REM_INDEX_PROC( 	   															\
									  PROC_NAME, 													\
									  TABLE_PTR														\
									 ) 				  												\
									  	  	  	  	  											    \
	void PROC_NAME(int32_t dmap_index)																\
	{																								\
		_dmap_rem(TABLE_PTR, dmap_index);															\
	}												  												\

/*! declare a table remove item process*/
#define DMAP_DECL_TABLE_REM_ITEM_PROC(PROC_NAME, ITEM_TYPE)  										\
	void PROC_NAME(ITEM_TYPE* item);   																\

/*! define a remove process to a table determined by a
 * table pointer */
#define DMAP_DEF_TABLE_REM_ITEM_PROC(			 													\
									  PROC_NAME, 													\
									  ITEM_TYPE,													\
									  TABLE_PTR														\
									) 																\
									  	  	  	  													\
	void PROC_NAME(ITEM_TYPE* item) 																\
	{																								\
		_dmap_rem_item(TABLE_PTR, (void *) item);													\
	}																								\


/*! declare a table iterate process*/
#define DMAP_DECL_ITR_TABLE_PROC(				 													\
								 PROC_NAME,															\
								 RESULT_TYPE														\
								)			  														\
	bool_t PROC_NAME(int32_t *dmap_index, RESULT_TYPE **result)										\
								__attribute__((nonnull (1,2)));   									\

/*! define an iterate process to a table determined by a
 * table pointer */
#define DMAP_DEF_ITR_TABLE_PROC(			 	    												\
									  PROC_NAME, 													\
									  RESULT_TYPE,													\
									  TABLE_PTR														\
								   )			 													\
									  	  	  	  													\
	bool_t PROC_NAME(																				\
				   int32_t *dmap_index, 															\
				   RESULT_TYPE **result																\
				  )																					\
	{																								\
		bool_t exist;																				\
		dmap_row_t *row = NULL;																		\
		*result = NULL;																				\
		exist = _dmap_itr_table(TABLE_PTR, dmap_index, &row);										\
		if(exist == BOOL_FALSE){																	\
			return BOOL_FALSE;																		\
		}																							\
		*result = (RESULT_TYPE*) row->item;															\
		return exist;																				\
	}																								\

/*! declare a get table process*/
#define DMAP_DECL_GET_TABLE_ITEM_PROC(				 												\
								 PROC_NAME,															\
								 RESULT_TYPE														\
								)			  														\
	RESULT_TYPE* PROC_NAME(int32_t dmap_index);   													\

/*! define a get process to a table determined by a
 * table pointer */
#define DMAP_DEF_GET_TABLE_ITEM_BYINDX_PROC(			  											\
									  PROC_NAME, 													\
									  RESULT_TYPE,													\
									  TABLE_PTR														\
								   )			 													\
									  	  	  	  													\
	RESULT_TYPE* PROC_NAME(																			\
				   int32_t dmap_index	 															\
				  )																					\
	{																								\
		return (RESULT_TYPE*) _dmap_get_item(TABLE_PTR, dmap_index);								\
	}																								\

/*! declare a arbitrary table*/
#define DMAP_DECL_VOIDT_TABLE(																		\
						GET_TABLE_PROC_NAME,														\
						GET_ITEM_PROC_NAME,															\
						ADD_ITEM_PROC_NAME,															\
						REM_INDEX_PROC_NAME,														\
						REM_ITEM_PROC_NAME,															\
						R_LOCK_TABLE_PROC_NAME,														\
						R_UNLOCK_TABLE_PROC_NAME,													\
						W_LOCK_TABLE_PROC_NAME,														\
						W_UNLOCK_TABLE_PROC_NAME,													\
						ITR_TABLE_PROC_NAME															\
						)																			\
		DMAP_DECL_GET_DATA_PROC(GET_TABLE_PROC_NAME, dmap_table_t);									\
		DMAP_DECL_GET_TABLE_ITEM_PROC(GET_ITEM_PROC_NAME, void);									\
		DMAP_DECL_TABLE_ADD_VOIDT_PROC(ADD_ITEM_PROC_NAME);											\
		DMAP_DECL_TABLE_REM_INDEX_PROC(REM_INDEX_PROC_NAME);										\
		DMAP_DECL_TABLE_REM_ITEM_PROC(REM_ITEM_PROC_NAME, void);									\
		DMAP_DECL_R_LOCK_PROC(R_LOCK_TABLE_PROC_NAME);												\
		DMAP_DECL_R_UNLOCK_PROC(R_UNLOCK_TABLE_PROC_NAME);											\
		DMAP_DECL_W_LOCK_PROC(W_LOCK_TABLE_PROC_NAME);												\
		DMAP_DECL_W_UNLOCK_PROC(W_UNLOCK_TABLE_PROC_NAME);											\
		DMAP_DECL_ITR_TABLE_PROC(ITR_TABLE_PROC_NAME, void);										\


/*! define an arbitrary typed table
 * table pointer */
#define DMAP_DEF_VOIDT_TABLE(																		\
						TABLE_VARIABLE,																\
						TABLE_NAME,																	\
						TABLE_LENGTH,																\
						INIT_TABLE_PROC_NAME,														\
						DEINIT_TABLE_PROC_NAME,														\
						GET_TABLE_PROC_NAME,														\
						GET_ITEM_PROC_NAME,															\
						ADD_ITEM_PROC_NAME,															\
						REM_INDEX_PROC_NAME,														\
						REM_ITEM_PROC_NAME,															\
						R_LOCK_TABLE_PROC_NAME,														\
						R_UNLOCK_TABLE_PROC_NAME,													\
						W_LOCK_TABLE_PROC_NAME,														\
						W_UNLOCK_TABLE_PROC_NAME,													\
						ITR_TABLE_PROC_NAME															\
						)																			\
		static dmap_table_t TABLE_VARIABLE;															\
		DMAP_DEF_TABLE_INIT_PROC(INIT_TABLE_PROC_NAME, &TABLE_VARIABLE, TABLE_LENGTH, TABLE_NAME);  \
		DMAP_DEF_TABLE_DEINIT_PROC(DEINIT_TABLE_PROC_NAME, &TABLE_VARIABLE);						\
		DMAP_DEF_GET_DATA_PROC(GET_TABLE_PROC_NAME, dmap_table_t, &TABLE_VARIABLE);					\
		DMAP_DEF_GET_TABLE_ITEM_BYINDX_PROC(GET_ITEM_PROC_NAME, void, &TABLE_VARIABLE);					\
		DMAP_DEF_TABLE_ADD_VOIDT_PROC(ADD_ITEM_PROC_NAME, &TABLE_VARIABLE);							\
		DMAP_DEF_TABLE_REM_INDEX_PROC(REM_INDEX_PROC_NAME, &TABLE_VARIABLE);						\
		DMAP_DEF_TABLE_REM_ITEM_PROC(REM_ITEM_PROC_NAME, void, &TABLE_VARIABLE); 					\
		DMAP_DEF_R_LOCK_PROC(R_LOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);			\
		DMAP_DEF_R_UNLOCK_PROC(R_UNLOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);		\
		DMAP_DEF_W_LOCK_PROC(W_LOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);			\
		DMAP_DEF_W_UNLOCK_PROC(W_UNLOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);		\
		DMAP_DEF_ITR_TABLE_PROC(ITR_TABLE_PROC_NAME, void, &TABLE_VARIABLE);						\


/*! declare a specialized typed table*/
#define DMAP_DECL_SPECT_TABLE(																		\
						DATA_TYPE,																	\
						GET_TABLE_PROC_NAME,														\
						GET_ITEM_PROC_NAME,															\
						ADD_ITEM_PROC_NAME,															\
						REM_INDEX_PROC_NAME,														\
						REM_ITEM_PROC_NAME,															\
						R_LOCK_TABLE_PROC_NAME,														\
						R_UNLOCK_TABLE_PROC_NAME,													\
						W_LOCK_TABLE_PROC_NAME,														\
						W_UNLOCK_TABLE_PROC_NAME,													\
						ITR_TABLE_PROC_NAME															\
						)																			\
		DMAP_DECL_GET_DATA_PROC(GET_TABLE_PROC_NAME, dmap_table_t);									\
		DMAP_DECL_GET_TABLE_ITEM_PROC(GET_ITEM_PROC_NAME, DATA_TYPE);								\
		DMAP_DECL_TABLE_ADD_SPECT_PROC(ADD_ITEM_PROC_NAME, TABLE_PTR, DATA_TYPE);					\
		DMAP_DECL_TABLE_REM_INDEX_PROC(REM_INDEX_PROC_NAME);										\
		DMAP_DECL_TABLE_REM_ITEM_PROC(REM_ITEM_PROC_NAME, DATA_TYPE);								\
		DMAP_DECL_R_LOCK_PROC(R_LOCK_TABLE_PROC_NAME);												\
		DMAP_DECL_R_UNLOCK_PROC(R_UNLOCK_TABLE_PROC_NAME);											\
		DMAP_DECL_W_LOCK_PROC(W_LOCK_TABLE_PROC_NAME);												\
		DMAP_DECL_W_UNLOCK_PROC(W_UNLOCK_TABLE_PROC_NAME);											\
		DMAP_DECL_ITR_TABLE_PROC(ITR_TABLE_PROC_NAME, DATA_TYPE);									\

/*! define a specialized table */
#define DMAP_DEF_SPECT_TABLE(																		\
						DATA_TYPE,																	\
						DATA_DTOR,																	\
						TABLE_VARIABLE,																\
						TABLE_NAME,																	\
						TABLE_LENGTH,																\
						INIT_TABLE_PROC_NAME,														\
						DEINIT_TABLE_PROC_NAME,														\
						GET_TABLE_PROC_NAME,														\
						GET_ITEM_PROC_NAME,															\
						ADD_ITEM_PROC_NAME,															\
						REM_INDEX_PROC_NAME,														\
						REM_ITEM_PROC_NAME,															\
						R_LOCK_TABLE_PROC_NAME,														\
						R_UNLOCK_TABLE_PROC_NAME,													\
						W_LOCK_TABLE_PROC_NAME,														\
						W_UNLOCK_TABLE_PROC_NAME,													\
						ITR_TABLE_PROC_NAME															\
						)																			\
		static dmap_table_t TABLE_VARIABLE;															\
		DMAP_DEF_TABLE_INIT_PROC(INIT_TABLE_PROC_NAME, &TABLE_VARIABLE, TABLE_LENGTH, TABLE_NAME);  \
		DMAP_DEF_TABLE_DEINIT_PROC(DEINIT_TABLE_PROC_NAME, &TABLE_VARIABLE);						\
		DMAP_DEF_GET_DATA_PROC(GET_TABLE_PROC_NAME, dmap_table_t, &TABLE_VARIABLE);					\
		DMAP_DEF_GET_TABLE_ITEM_BYINDX_PROC(GET_ITEM_PROC_NAME, DATA_TYPE, &TABLE_VARIABLE); 				\
		DMAP_DEF_TABLE_ADD_SPECT_PROC(ADD_ITEM_PROC_NAME, &TABLE_VARIABLE, DATA_TYPE, DATA_DTOR);	\
		DMAP_DEF_TABLE_REM_INDEX_PROC(REM_INDEX_PROC_NAME, &TABLE_VARIABLE);						\
		DMAP_DEF_TABLE_REM_ITEM_PROC(REM_ITEM_PROC_NAME, DATA_TYPE, &TABLE_VARIABLE); 				\
		DMAP_DEF_R_LOCK_PROC(R_LOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);			\
		DMAP_DEF_R_UNLOCK_PROC(R_UNLOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);		\
		DMAP_DEF_W_LOCK_PROC(W_LOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);			\
		DMAP_DEF_W_UNLOCK_PROC(W_UNLOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);		\
		DMAP_DEF_ITR_TABLE_PROC(ITR_TABLE_PROC_NAME, DATA_TYPE, &TABLE_VARIABLE);					\

/*! declare a specialized typed local static table*/
#define DMAP_DEF_SPECT_LOCAL_TABLE(																	\
									DATA_TYPE,														\
									DATA_DTOR,														\
									TABLE_VARIABLE,													\
									TABLE_NAME,														\
									TABLE_LENGTH,													\
									INIT_TABLE_PROC_NAME,											\
									DEINIT_TABLE_PROC_NAME,											\
									GET_TABLE_PROC_NAME,											\
									GET_ITEM_PROC_NAME,												\
									ADD_ITEM_PROC_NAME,												\
									REM_INDEX_PROC_NAME,											\
									REM_ITEM_PROC_NAME,												\
									R_LOCK_TABLE_PROC_NAME,											\
									R_UNLOCK_TABLE_PROC_NAME,										\
									W_LOCK_TABLE_PROC_NAME,											\
									W_UNLOCK_TABLE_PROC_NAME,										\
									ITR_TABLE_PROC_NAME												\
								   )																\
		static dmap_table_t TABLE_VARIABLE;															\
		static DMAP_DECL_GET_DATA_PROC(GET_TABLE_PROC_NAME, dmap_table_t);							\
		static DMAP_DECL_GET_TABLE_ITEM_PROC(GET_ITEM_PROC_NAME, DATA_TYPE);						\
		static DMAP_DECL_TABLE_ADD_SPECT_PROC(ADD_ITEM_PROC_NAME, TABLE_PTR, DATA_TYPE);			\
		static DMAP_DECL_TABLE_REM_INDEX_PROC(REM_INDEX_PROC_NAME);									\
		static DMAP_DECL_TABLE_REM_ITEM_PROC(REM_ITEM_PROC_NAME, DATA_TYPE);						\
		static DMAP_DECL_R_LOCK_PROC(R_LOCK_TABLE_PROC_NAME);										\
		static DMAP_DECL_R_UNLOCK_PROC(R_UNLOCK_TABLE_PROC_NAME);									\
		static DMAP_DECL_W_LOCK_PROC(W_LOCK_TABLE_PROC_NAME);										\
		static DMAP_DECL_W_UNLOCK_PROC(W_UNLOCK_TABLE_PROC_NAME);									\
		static DMAP_DECL_ITR_TABLE_PROC(ITR_TABLE_PROC_NAME, DATA_TYPE);							\
																									\
		DMAP_DEF_TABLE_INIT_PROC(INIT_TABLE_PROC_NAME, &TABLE_VARIABLE, TABLE_LENGTH, TABLE_NAME);  \
		DMAP_DEF_TABLE_DEINIT_PROC(DEINIT_TABLE_PROC_NAME, &TABLE_VARIABLE);						\
		DMAP_DEF_GET_DATA_PROC(GET_TABLE_PROC_NAME, dmap_table_t, &TABLE_VARIABLE);					\
		DMAP_DEF_GET_TABLE_ITEM_BYINDX_PROC(GET_ITEM_PROC_NAME, DATA_TYPE, &TABLE_VARIABLE);				\
		DMAP_DEF_TABLE_ADD_SPECT_PROC(ADD_ITEM_PROC_NAME, &TABLE_VARIABLE, DATA_TYPE, DATA_DTOR);	\
		DMAP_DEF_TABLE_REM_INDEX_PROC(REM_INDEX_PROC_NAME, &TABLE_VARIABLE);						\
		DMAP_DEF_TABLE_REM_ITEM_PROC(REM_ITEM_PROC_NAME, DATA_TYPE, &TABLE_VARIABLE); 				\
		DMAP_DEF_R_LOCK_PROC(R_LOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);			\
		DMAP_DEF_R_UNLOCK_PROC(R_UNLOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);		\
		DMAP_DEF_W_LOCK_PROC(W_LOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);			\
		DMAP_DEF_W_UNLOCK_PROC(W_UNLOCK_TABLE_PROC_NAME, TABLE_VARIABLE.rwmutex, TABLE_NAME);		\
		DMAP_DEF_ITR_TABLE_PROC(ITR_TABLE_PROC_NAME, DATA_TYPE, &TABLE_VARIABLE);					\

//---------------------------------------------------------------------------------------------------
//------------------------- DATA ITEM MACROS --------------------------------------------------------
//---------------------------------------------------------------------------------------------------

/*! declare a datamap item*/
#define DMAP_DECL_ITEM(																				\
						ITEM_TYPE,																	\
						GET_ITEM_PROC_NAME,															\
						LOCK_ITEM_PROC_NAME,														\
						UNLOCK_ITEM_PROC_NAME														\
						)																			\
		DMAP_DECL_GET_DATA_PROC(GET_ITEM_PROC_NAME, ITEM_TYPE)	 									\
		DMAP_DECL_R_LOCK_PROC(LOCK_ITEM_PROC_NAME)													\
		DMAP_DECL_R_UNLOCK_PROC(UNLOCK_ITEM_PROC_NAME)												\

/*! define a datamap item*/
#define DMAP_DEF_ITEM(																				\
						ITEM_TYPE,																	\
						ITEM_VARIABLE,																\
						MUTEX_VARIABLE,																\
						ITEM_NAME,																	\
						GET_ITEM_PROC_NAME,															\
						LOCK_ITEM_PROC_NAME,														\
						UNLOCK_ITEM_PROC_NAME														\
						)																			\
		static ITEM_TYPE ITEM_VARIABLE;																\
		static mutex_t *MUTEX_VARIABLE;																\
		DMAP_DEF_GET_DATA_PROC(GET_ITEM_PROC_NAME, ITEM_TYPE, &ITEM_VARIABLE) 						\
		DMAP_DEF_M_LOCK_PROC(LOCK_ITEM_PROC_NAME, MUTEX_VARIABLE, ITEM_NAME)						\
		DMAP_DEF_M_UNLOCK_PROC(UNLOCK_ITEM_PROC_NAME, MUTEX_VARIABLE, ITEM_NAME)					\



#define __THR_NO_INIT_PROC__ func_with_param_do_nothing
#define __THR_NO_DEINIT_PROC__ func_with_param_do_nothing

#define __THR_NO_ACT_INIT_PROC__ func_with_param_do_nothing
#define __THR_NO_DEACT_DEINIT_PROC__ func_with_param_do_nothing

#define LIB_DEF_THREAD( 							\
					   THR_PROC_NAME,  				\
					   THR_INIT_PROC_NAME, 			\
					   THR_MAIN_PROC_NAME, 			\
					   THR_DEINIT_PROC_NAME 		\
					  ) 							\
	static void* THR_PROC_NAME(void *arg) 			\
	{												\
		thread_t *thread = (thread_t*) arg;         \
		THR_INIT_PROC_NAME(thread);					\
		thread->state = THREAD_STATE_RUN;			\
		THR_MAIN_PROC_NAME(thread);					\
		thread->state = THREAD_STATE_STOPPED;		\
		THR_DEINIT_PROC_NAME(thread);				\
		return NULL;								\
	}												\


#define LIB_DEF_CTOR(TYPE, PROC_NAME)					\
		TYPE* PROC_NAME()								\
		{												\
			TYPE *result;								\
			result = (TYPE*) malloc(sizeof(TYPE));		\
			memset(result, 0, sizeof(TYPE));			\
			return result;								\
		}												\


#define LIB_DEF_DTOR(TYPE, PROC_NAME)					\
		PROC_NAME(void* param)							\
		{												\
			TYPE* target;								\
			LOGGING_SG_IS_CALLED(#TYPE"_dtor");			\
			if(param == NULL){							\
				return;									\
			}											\
			target = (TYPE*) param;						\
			free(target);								\
			target = NULL;								\
		}												\


/*! pointing to a function that do nothing */
#define __CMP_NO_INIT_FUNC_ func_do_nothing
/*! pointing to a function with an argument that do nothing */
#define __CMP_NO_INIT_MFUNC__ func_with_param_do_nothing
/*! pointing to a function that do nothing */
#define __CMP_NO_DEINIT_FUNC_ func_do_nothing
/*! pointing to a function with an argument that do nothing */
#define __CMP_NO_DEINIT_MFUNC__ func_with_param_do_nothing
/*! pointing to a function with an argument that do nothing */
#define __NO_TEST_FUNC_ func_with_param_return_true

#ifdef NDEBUG
/*! do nothing */
#define CMP_TEST(CMP_TEST_FUNC, CMP_VAR, CMP_NAME)

#else
/*! test function to a component */
#define CMP_TEST(CMP_TEST_FUNC, CMP_VAR, CMP_NAME)     							   \
	CMP_CHECK_EXISTANCE(CMP_VAR,  CMP_NAME);									   \
	ASSERT_TRUE(CMP_TEST_FUNC(CMP_VAR) == BOOL_TRUE, "Test failed at component: %s", CMP_NAME)

#endif /* NDEBUG */

/*! realize a demand functionality */
#define CMP_DEMAND(name, lvalue, supplier, ...)                		 \
	debug_flow("%s demand an item from %p", name, supplier);         \
	lvalue = supplier(__VA_ARGS__)						         	 \

/*! debug a receive functionality */
#define CMP_RECEIVE(name, item)				                     	 \
	debug_flow("%s received an item %p", name, item);        		 \

/*! debug a supply functionality */
#define CMP_SUPPLY(name, item)				                     	 \
	debug_flow("%s supplied an item %p", name, item);        		 \

/*! realize a send functionality */
#define CMP_SEND(name, receiver, item)								 \
	debug_flow("%s send an item %p to %p ", name, item, receiver);   \
	receiver(item);									 				 \

/*! realize a send functionality with parameters*/
#define CMP_SEND_WITH_ARGS(name, receiver, item, ...)				 \
	debug_flow("%s send an item %p to %p ", name, item, receiver);   \
	receiver(__VA_ARGS__, item);					 				 \

/*! define a this variable */
#define CMP_DEF_THIS(TYPE, VALUE) TYPE* this = VALUE;

/*! define a variable pointing to a type passed as an argument */
#define CMP_DEF_VAR(TYPE, NAME) TYPE* NAME = NULL;
/*! define a variable pointing to a type passed as an argument */
#define CMP_STAT_DEF_VAR(TYPE, NAME) static TYPE* NAME = NULL;

/*! define a puffer receiver method.
 * If the puffer is full it will block the sender thread until
 * the puffer has not available price*/
#define CMP_DEF_PUFFER_RECV_WAIT(										 \
								cmp_name, 								 \
								cmp_type,								 \
								cmp_var, 								 \
								item_type, 								 \
								PROC_NAME							  	 \
								)										 \
	static void PROC_NAME(item_type* item)								 \
	{																	 \
		CMP_DEF_THIS(cmp_type, cmp_var);								 \
		datapuffer_t    *puffer = this->puffer;							 \
		signal_t        *signal = this->signal;							 \
		bool_t  		 was_empty;										 \
																		 \
		/*CMP_PUFFER_RECV_PROC_WAIT(cmp_name, item_type, puffer, signal,*/ \
				/*item, puffer->is_empty, was_empty, puffer->is_full);	 */\
		CMP_PUFFER_RECV_PROC_WAIT(cmp_name, item_type, puffer, signal,   \
				item, datapuffer_isempty(puffer), was_empty, datapuffer_isfull(puffer));\
																		 \
	}/*#PROC_NAME end*/										 		     \


/*! define a puffer supply method. It suuplies items to the caller thread.
 * It will block the caller thread if the puffer is empty until
 * it has not data */
#define CMP_DEF_PUFFER_SUPPLY_WAIT(										 \
								cmp_name, 								 \
								cmp_type,								 \
								cmp_var,								 \
								item_type, 								 \
								PROC_NAME							  	 \
								)										 \
	static item_type* PROC_NAME()									 	 \
	{																	 \
		CMP_DEF_THIS(cmp_type, cmp_var);								 \
		item_type       *result = NULL;									 \
		datapuffer_t    *puffer = this->puffer;							 \
		signal_t        *signal = this->signal;							 \
		bool_t  		 was_full;										 \
																		 \
		/*CMP_PUFFER_SUPPLY_PROC_WAIT(cmp_name, item_type, puffer, signal,*/ \
				/*result, puffer->is_empty, puffer->is_full, was_full);*/	 \
		CMP_PUFFER_SUPPLY_PROC_WAIT(cmp_name, item_type, puffer, signal, \
				result, datapuffer_isempty(puffer), datapuffer_isfull(puffer), was_full);	 \
		return result;													 \
	}/*#PROC_NAME end*/										 		     \


/*! define a puffer supply commands */
#define CMP_PUFFER_SUPPLY_PROC_WAIT(									 	 \
									name,									 \
									type, 								 	 \
									puffer, 								 \
									signal, 								 \
									item,									 \
									is_empty, 								 \
									is_full, 								 \
									was_full								 \
									)										 \
	signal_lock(signal);											    	 \
	if(is_empty == BOOL_TRUE){											     \
		/*logging("%s is empty", name);*/			 		 	 			 \
		signal_wait(signal);										     	 \
	}																	     \
																		     \
    was_full = is_full;														 \
	item = (type*) datapuffer_read(puffer);						  			 \
																		     \
	if(was_full == BOOL_TRUE){											     \
		signal_set(signal);										    		 \
	}																	     \
	/*CMP_SUPPLY(name, item);*/												 \
	signal_unlock(signal);     												 \

/*! define a puffer receiver commands */
#define CMP_PUFFER_RECV_PROC_WAIT(									         \
								   name,									 \
								   type, 								     \
								   puffer, 								     \
								   signal, 								     \
								   item,									 \
								   is_empty, 						     	 \
								   was_empty, 							     \
								   is_full									 \
								)			 							     \
	signal_lock(signal);													 \
	/*CMP_RECEIVE(name, item);*/									 		 \
	if(is_full == BOOL_TRUE){												 \
		runtime_warning("%s is full", name);				 		 	 	 \
		signal_wait(signal);												 \
	}																		 \
		 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	     \
    was_empty = is_empty;													 \
	datapuffer_write(puffer, (void*) item);								 	 \
																		     \
	if(was_empty == BOOL_TRUE){											 	 \
		signal_set(signal);													 \
	}																		 \
																			 \
	signal_unlock(signal);										     		 \


/*! define commands to a constructor for a singletone component */
#define CMP_CTOR_PROC(type, var, name)                      			\
		PRINTING_CONSTRUCTING_SG(name);									\
		if(var != NULL){												\
			runtime_warning("%s is already constructed", name);			\
			return;														\
		}																\
		var = (type*) malloc(sizeof(type));								\
		BZERO(var, sizeof(type));										\
		PRINTING_SG_IS_CONSTRUCTED(name);


 /*! define commands to a constructor for a component */
#define CMP_MCTOR_PROC(TYPE, VALUE, CMP_NAME)                      		\
		PRINTING_CONSTRUCTING_SG(CMP_NAME);								\
		VALUE = (TYPE*) malloc(sizeof(TYPE));							\
		BZERO(VALUE, sizeof(TYPE));										\
		PRINTING_SG_IS_CONSTRUCTED(CMP_NAME);							\

 /*! define a constructor to a singletone component */
#define CMP_DEF_CTOR(PROC_NAME,											\
					  TYPE,												\
					  VALUE, 											\
					  CMP_NAME, 										\
					  INIT_PROC_NAME, 									\
					  TESTING_PROC_NAME)            					\
	void PROC_NAME()													\
	{																	\
			CMP_CTOR_PROC(TYPE, VALUE, CMP_NAME)						\
			PRINTING_INIT_SG(CMP_NAME);									\
			INIT_PROC_NAME();											\
			PRINTING_INIT_COMPLETED;									\
			PRINTING_TESTING_SG(CMP_NAME);								\
			CMP_TEST(TESTING_PROC_NAME, VALUE, CMP_NAME)                \
			PRINTING_TEST_IS_COMPLETED;									\
	}																	\


 /*! define a constructor to a component */
#define CMP_DEF_MCTOR(PROC_NAME,										\
					  TYPE, 	 										\
					  CMP_NAME, 										\
					  INIT_PROC_NAME, 									\
					  TESTING_PROC_NAME)            					\
	TYPE* PROC_NAME()													\
	{																	\
			TYPE* result = NULL;										\
			CMP_MCTOR_PROC(TYPE, result, CMP_NAME)						\
			PRINTING_INIT_SG(CMP_NAME);									\
			INIT_PROC_NAME(result);										\
			PRINTING_INIT_COMPLETED;									\
			PRINTING_TESTING_SG(CMP_NAME);								\
			CMP_TEST(TESTING_PROC_NAME, result, CMP_NAME)               \
			PRINTING_TEST_IS_COMPLETED;									\
			return result;												\
	}																	\


/*! define a list of commands for a destructor to a singletone component */
#define CMP_DTOR_PROC(TYPE, VALUE, CMP_NAME)	                        \
		PRINTING_DESTRUCTING_SG(CMP_NAME);								\
		if(VALUE == NULL){												\
			logging("%s", CMP_NAME" is already destructed or never exists");  \
			return;														\
		}																\
		free(VALUE);													\
		PRINTING_SG_IS_DESTRUCTED(CMP_NAME);							\


/*! define a destructor to a singletone component */
#define CMP_DEF_DTOR(PROC_NAME, 										\
					  TYPE, 											\
					  VALUE, 											\
					  CMP_NAME, 										\
					  DEINIT_PROC_NAME)   								\
	void PROC_NAME()													\
	{																	\
			PRINTING_DEINIT_SG(CMP_NAME);								\
			DEINIT_PROC_NAME();											\
			CMP_DTOR_PROC(TYPE, VALUE, CMP_NAME);						\
	}																	\

/*! define a list of commands for a constructor to a singletone component */
#define CMP_DEF_MDTOR(PROC_NAME, 										\
					  TYPE, 											\
					  CMP_NAME, 										\
					  DEINIT_PROC_NAME)   								\
	void PROC_NAME(void *cmp)											\
	{																	\
			PRINTING_DEINIT_SG(CMP_NAME);								\
			DEINIT_PROC_NAME(cmp);										\
			CMP_DTOR_PROC(TYPE, cmp, CMP_NAME);							\
	}																	\



 /*! check weather a component already exists or not */
#define CMP_CHECK_EXISTANCE(CMP_VAR, CMP_NAME)							\
	ASSERT_TRUE(CMP_VAR != NULL, GET_TEXT_SG_NOTEXISTS(CMP_NAME));



 /*! define a singletone component */
#define CMP_DEF(DECL_TYPE,													\
				 CMP_TYPE, 													\
				 CMP_NAME,													\
				 CMP_VAR,													\
				 CTOR_PROC_NAME,											\
				 DTOR_PROC_NAME,											\
				 INIT_PROC_NAME,											\
				 TESTING_PROC_NAME,											\
				 DEINIT_PROC_NAME)											\
						 	 	 	 	 	 	 	 	 	 	 	 	 	\
	CMP_STAT_DEF_VAR(CMP_TYPE, CMP_VAR);									\
																			\
	DECL_TYPE CMP_DEF_CTOR(CTOR_PROC_NAME, 									\
		CMP_TYPE, 															\
		CMP_VAR, 															\
		CMP_NAME,															\
		INIT_PROC_NAME,														\
		TESTING_PROC_NAME);													\
																			\
	DECL_TYPE CMP_DEF_DTOR(DTOR_PROC_NAME,									\
		CMP_TYPE, 															\
		CMP_VAR, 															\
		CMP_NAME,															\
		DEINIT_PROC_NAME);													\



/*! define a component */
#define CMP_MDEF(DECL_TYPE,													\
				 CMP_TYPE, 													\
				 CMP_NAME,													\
				 CTOR_PROC_NAME,											\
				 DTOR_PROC_NAME,											\
				 INIT_PROC_NAME,											\
				 TESTING_PROC_NAME,											\
				 DEINIT_PROC_NAME)											\
						 	 	 	 	 	 	 	 	 	 	 	 	 	\
																			\
	DECL_TYPE CMP_DEF_MCTOR(CTOR_PROC_NAME, 								\
		CMP_TYPE, 															\
		CMP_NAME,															\
		INIT_PROC_NAME,														\
		TESTING_PROC_NAME);													\
																			\
	DECL_TYPE CMP_DEF_MDTOR(DTOR_PROC_NAME,									\
		CMP_TYPE, 															\
		CMP_NAME,															\
		DEINIT_PROC_NAME);													\




 /*! define a get accessor method to a component */
#define CMP_DEF_GET_PROC(PROC_NAME, CMP_TYPE, CMP_VAR)						\
	CMP_TYPE* PROC_NAME()													\
	{																		\
		return CMP_VAR;														\
	}																		\

/*! bind an rvalue process pointer to the lvalue */
#define CMP_BIND(lvalue, rvalue)				    						\
		PRINTING_BIND_SG_TO_PROC(lvalue, rvalue);							\
		lvalue = rvalue;													\

/*! connect an rvalue process pointer to the lvalue */
#define CMP_CONNECT(lvalue, rvalue)		            						\
		PRINTING_CONNECT_SG1_TO_SG2(lvalue, rvalue);						\
		lvalue = rvalue;													\

 /*! assign an rvalue to the lvalue */
#define CMP_ASSIGN_SG1_TO_SG2(lvalue, rvalue)		            			\
		PRINTING_ASSIGNING_SG1_TO_SG2(lvalue, rvalue);					    \
		lvalue = rvalue;													\

/*! declare a signalized puffer for a specific type */
#define CMP_DECL_SGPUFFER(TYPE_NAME, ITEM_TYPE) 							\
	typedef struct TYPE_NAME##_struct_t 									\
	{ 																		\
		datapuffer_t* puffer; 												\
		void         (*receiver)(ITEM_TYPE*); 								\
		ITEM_TYPE*   (*supplier)(); 										\
		signal_t      *signal; 												\
	}TYPE_NAME##_t;

 /*! declare a recycle puffer for a specific type */
#define CMP_DECL_RECPUFFER(TYPE_NAME, ITEM_TYPE) 							\
	typedef struct TYPE_NAME##_struct_t 									\
	{ 																		\
		datapuffer_t* puffer; 												\
		void         (*receiver)(ITEM_TYPE*); 								\
		ITEM_TYPE*   (*supplier)(); 										\
		spin_t      *spin; 													\
	}TYPE_NAME##_t;

//declare and define a puffer component using signals
#define CMP_DEF_SGPUFFER(DECL_TYPE,											\
				 ITEM_TYPE,													\
				 ITEM_DTOR,													\
				 CMP_NAME,													\
				 CMP_VAR,													\
				 PUFFER_LENGTH,												\
				 CTOR_PROC_NAME,											\
				 DTOR_PROC_NAME												\
				 )															\
				 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	\
		CMP_DECL_SGPUFFER(CMP_VAR, ITEM_TYPE);								\
		static void CMP_VAR##_init();										\
		static void CMP_VAR##_deinit();										\
																			\
		CMP_DEF(DECL_TYPE, 		 										    \
		CMP_VAR##_t,    										 	    	\
		 CMP_NAME,    		 								         	    \
		 CMP_VAR,        	  							 	  				\
		 CTOR_PROC_NAME,    	  											\
		 DTOR_PROC_NAME,     	   											\
		 CMP_VAR##_init,           											\
		 __NO_TEST_FUNC_,             										\
		 CMP_VAR##_deinit            										\
		);																	\
																			\
    CMP_DEF_PUFFER_RECV_WAIT(CMP_NAME, CMP_VAR##_t, CMP_VAR, ITEM_TYPE,     \
				CMP_VAR##_process_receiver)									\
																			\
	CMP_DEF_PUFFER_SUPPLY_WAIT(CMP_NAME, CMP_VAR##_t, CMP_VAR, ITEM_TYPE,	\
				CMP_VAR##_process_supplier)								    \
																			\
	void CMP_VAR##_init()													\
	{																		\
		CMP_VAR->puffer = datapuffer_ctor(PUFFER_LENGTH);					\
		CMP_VAR->signal = signal_ctor();									\
		CMP_BIND(CMP_VAR->receiver, CMP_VAR##_process_receiver)			    \
		CMP_BIND(CMP_VAR->supplier, CMP_VAR##_process_supplier)				\
	}																		\
																			\
	void CMP_VAR##_deinit()													\
	{																		\
		datapuffer_t*  puffer = CMP_VAR->puffer;							\
		GEN_PUFF_CLEAR_PROC(puffer, ITEM_TYPE, ITEM_DTOR);					\
		datapuffer_dtor(CMP_VAR->puffer);									\
		signal_dtor(CMP_VAR->signal);										\
	}																		\




//declare and define a puffer component used for recycling types
#define CMP_DEF_RECPUFFER(DECL_TYPE,										\
				 ITEM_TYPE,													\
				 ITEM_CTOR,													\
				 ITEM_DTOR,													\
				 ITEM_CLEAN,												\
				 CMP_NAME,													\
				 CMP_VAR,													\
				 PUFFER_LENGTH,												\
				 CTOR_PROC_NAME,											\
				 DTOR_PROC_NAME												\
				 )															\
				 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	\
		CMP_DECL_RECPUFFER(CMP_VAR, ITEM_TYPE);								\
		static void CMP_VAR##_init();										\
		static void CMP_VAR##_deinit();										\
																			\
		CMP_DEF(DECL_TYPE, 		 										    \
		CMP_VAR##_t,    										 	    	\
		 CMP_NAME,    		 								         	    \
		 CMP_VAR,        	  							 	  				\
		 CTOR_PROC_NAME,    	  											\
		 DTOR_PROC_NAME,     	   											\
		 CMP_VAR##_init,           											\
		 __NO_TEST_FUNC_,             										\
		 CMP_VAR##_deinit            										\
		);																	\
																			\
	static void CMP_VAR##_process_receiver(ITEM_TYPE* item)					\
	{																		\
		CMP_DEF_THIS(CMP_VAR##_t, CMP_VAR);									\
		datapuffer_t    *puffer = this->puffer;								\
		spin_t        	*spin = this->spin;									\
		spin_lock(spin);													\
																			\
		/*CMP_RECEIVE(CMP_NAME, item);	*/									\
		/*if(puffer->is_full == BOOL_TRUE){								  */\
		if(datapuffer_isfull(puffer) == BOOL_TRUE){							\
			runtime_warning("%s is full", CMP_NAME);				 		\
			spin_unlock(spin);										     	\
			ITEM_DTOR(item);												\
			return;															\
		}																	\
																			\
		ITEM_CLEAN(item);													\
		datapuffer_write(puffer, (void*) item);								\
																			\
		spin_unlock(spin);										     		\
	}/*#PROC_NAME end*/										 		   		\
																			\
	static ITEM_TYPE* CMP_VAR##_process_supplier()					 	 	\
	{																		\
		CMP_DEF_THIS(CMP_VAR##_t, CMP_VAR);								 	\
		ITEM_TYPE       *result = NULL;									 	\
		datapuffer_t    *puffer = this->puffer;							 	\
		spin_t        	*spin = this->spin;							 		\
																		 	\
		spin_lock(spin);											    	\
		/*if(puffer->is_empty == BOOL_TRUE){							  */\
	    if(datapuffer_isempty(puffer) == BOOL_TRUE){						\
			logging("%s is empty", CMP_NAME);				 		 	 	\
			spin_unlock(spin);												\
			result = ITEM_CTOR();											\
			return result;													\
		}																	\
																			\
		result = (ITEM_TYPE*) datapuffer_read(puffer);						\
																			\
		/*CMP_SUPPLY(CMP_NAME, result);*/									\
		spin_unlock(spin);     												\
		return result;														\
	}/*#PROC_NAME end*/										 		     	\
																			\
	void CMP_VAR##_init()													\
	{																		\
		CMP_VAR->puffer = datapuffer_ctor(PUFFER_LENGTH);					\
		CMP_VAR->spin = spin_ctor();										\
		CMP_BIND(CMP_VAR->receiver, CMP_VAR##_process_receiver)			    \
		CMP_BIND(CMP_VAR->supplier, CMP_VAR##_process_supplier)				\
	}																		\
																			\
	void CMP_VAR##_deinit()													\
	{																		\
		datapuffer_t*  puffer = CMP_VAR->puffer;							\
		GEN_PUFF_CLEAR_PROC(puffer, ITEM_TYPE, ITEM_DTOR);					\
		datapuffer_dtor(CMP_VAR->puffer);									\
		spin_dtor(CMP_VAR->spin);											\
	}																		\

/*! define a threaded singletone component */
#define CMP_THREAD(													\
				DECL_TYPE,											\
				CMP_TYPE,											\
				CMP_UNIQUE,											\
				CMP_NAME,											\
				CMP_INIT_PROC_NAME, 								\
				CMP_DEINIT_PROC_NAME, 								\
				THR_START_THREAD_PROC_NAME, 						\
				THR_STOP_THREAD_PROC_NAME, 							\
				THR_MAIN_PROC_NAME									\
				)													\
	DECL_TYPE thread_t* _thr_##CMP_UNIQUE = NULL;					\
	LIB_DEF_THREAD(_thr_##CMP_UNIQUE##_entrypoint, 					\
		   __THR_NO_INIT_PROC__,      								\
		   THR_MAIN_PROC_NAME,  						 			\
		   __THR_NO_DEINIT_PROC__         							\
		  );														\
	CMP_DEF(DECL_TYPE,												\
				CMP_TYPE,       									\
				CMP_NAME,   										\
				_cmp_##CMP_UNIQUE,									\
				_cmp_##CMP_UNIQUE##_ctor,           				\
				_cmp_##CMP_UNIQUE##_dtor,           				\
				 CMP_INIT_PROC_NAME,              					\
				 __NO_TEST_FUNC_,             						\
				 CMP_DEINIT_PROC_NAME               				\
				);													\
DECL_TYPE void* THR_START_THREAD_PROC_NAME() 						\
{																	\
	PRINTING_STARTING_SG(CMP_NAME);									\
																	\
	_cmp_##CMP_UNIQUE##_ctor();										\
	_thr_##CMP_UNIQUE = make_thread(_thr_##CMP_UNIQUE##_entrypoint, _cmp_##CMP_UNIQUE); \
																	\
	start_thread(_thr_##CMP_UNIQUE);								\
																	\
	PRINTING_SG_IS_STARTED(CMP_NAME);								\
																	\
	return NULL;													\
}																	\
																	\
DECL_TYPE void* THR_STOP_THREAD_PROC_NAME() 						\
{																	\
	CMP_TYPE *cmp = _cmp_##CMP_UNIQUE;								\
	thread_t *thread = _thr_##CMP_UNIQUE;							\
	PRINTING_STOPPING_SG(CMP_NAME);									\
																	\
	stop_thread(thread);											\
	_cmp_##CMP_UNIQUE##_dtor(cmp);									\
	_thr_##CMP_UNIQUE = NULL;										\
	_cmp_##CMP_UNIQUE = NULL;										\
	PRINTING_SG_IS_STOPPED(CMP_NAME);								\
																	\
	return NULL;													\
}																	\

/*! define a a single thread separator using push dataflow between them */
#define CMP_PUSH_THREADER(											\
				ITEM_TYPE,											\
				CMP_UNIQUE,											\
				CMP_NAME,											\
				THR_START_THREAD_PROC_NAME, 						\
				THR_STOP_THREAD_PROC_NAME 							\
				)													\
																	\
typedef struct _cmp_##CMP_UNIQUE##_struct_t							\
{																	\
	signal_t *signal;												\
	void     (*receiver)(ITEM_TYPE*);								\
	ITEM_TYPE* item;												\
	void     (*send)(ITEM_TYPE*);									\
}_cmp_##CMP_UNIQUE##_t;												\
static void _cmp_##CMP_UNIQUE##_receiver(ITEM_TYPE*);				\
static void _cmp_##CMP_UNIQUE##_init();								\
static void _cmp_##CMP_UNIQUE##_deinit();							\
static void _cmp_##CMP_UNIQUE##_receiver(ITEM_TYPE *item);			\
static void _thr_##CMP_UNIQUE##_main_proc(thread_t *thread);		\
CMP_THREAD(															\
				static,											    \
				_cmp_##CMP_UNIQUE##_t,								\
				CMP_UNIQUE,											\
				CMP_NAME,											\
				_cmp_##CMP_UNIQUE##_init, 							\
				_cmp_##CMP_UNIQUE##_deinit, 						\
				THR_START_THREAD_PROC_NAME, 						\
				THR_STOP_THREAD_PROC_NAME, 							\
				_thr_##CMP_UNIQUE##_main_proc						\
				)													\
void _cmp_##CMP_UNIQUE##_init()										\
{																	\
	CMP_DEF_THIS(_cmp_##CMP_UNIQUE##_t, _cmp_##CMP_UNIQUE);			\
	this->signal = signal_ctor();									\
	this->item = NULL;												\
	CMP_BIND(_cmp_##CMP_UNIQUE->receiver, _cmp_##CMP_UNIQUE##_receiver); \
}																	\
void _cmp_##CMP_UNIQUE##_deinit()									\
{																	\
	CMP_DEF_THIS(_cmp_##CMP_UNIQUE##_t, _cmp_##CMP_UNIQUE);			\
	signal_dtor(this->signal);										\
}																	\
void _cmp_##CMP_UNIQUE##_receiver(ITEM_TYPE *item)					\
{																	\
	CMP_DEF_THIS(_cmp_##CMP_UNIQUE##_t, _cmp_##CMP_UNIQUE);			\
	signal_lock(this->signal);										\
	this->item = item;												\
	signal_set(this->signal);										\
	signal_unlock(this->signal);									\
}																	\
void _thr_##CMP_UNIQUE##_main_proc(thread_t *thread)				\
{																	\
	CMP_DEF_THIS(_cmp_##CMP_UNIQUE##_t, (_cmp_##CMP_UNIQUE##_t*) thread->arg);		\
	do																\
	{																\
		signal_lock(this->signal);									\
		signal_wait(this->signal);									\
		/*CMP_SEND(CMP_NAME, this->send, this->item);*/				\
		this->send(this->item);										\
		signal_unlock(this->signal);								\
	}while(thread->state == THREAD_STATE_RUN);						\
}																	\


/*! define a threaded multitone components */
#define CMP_THREADS(												\
				DECL_TYPE,											\
				CMP_TYPE,											\
				CMP_UNIQUE,											\
				CMP_NAME,											\
				CHAIN_CTOR_PROC_NAME,								\
				CHAIN_DTOR_PROC_NAME,								\
				CHAIN_ITR_PROC_NAME,								\
				THR_INIT_PROC_NAME, 								\
				CMP_INIT_PROC_NAME,									\
				THR_DEINIT_PROC_NAME,								\
				CMP_DEINIT_PROC_NAME, 								\
				THR_MAIN_PROC_NAME,									\
				THR_ACTIVATOR_TYPE, 								\
				CMP_ACTIVATOR_ATTR, 								\
				THR_START_THREAD_PROC_NAME, 						\
				THR_STOP_THREAD_PROC_NAME 							\
				)													\
	CMP_MDEF(DECL_TYPE,												\
			CMP_TYPE,       									  	\
			CMP_NAME,   										    \
			_cmp_##CMP_UNIQUE##_ctor,           					\
			_cmp_##CMP_UNIQUE##_dtor,           					\
			 CMP_INIT_PROC_NAME,              						\
			 __NO_TEST_FUNC_,             							\
			 CMP_DEINIT_PROC_NAME               					\
			);														\
typedef struct _thrs_##CMP_UNIQUE##_struct_t						\
{																	\
	thread_t*      threads[MPT_MAX_THREAD_NUM];						\
	rwmutex_t*     rwmutex;											\
}_thrs_##CMP_UNIQUE##_t;											\
																	\
static _thrs_##CMP_UNIQUE##_t _thrs_##CMP_UNIQUE;					\
DECL_TYPE void CHAIN_CTOR_PROC_NAME()								\
{																	\
	int32_t  index;													\
	_thrs_##CMP_UNIQUE.rwmutex = rwmutex_ctor();					\
	for(index = 0; index < MPT_MAX_THREAD_NUM; ++index){			\
		_thrs_##CMP_UNIQUE.threads[index] = NULL;					\
	}																\
}																	\
																	\
DECL_TYPE void CHAIN_DTOR_PROC_NAME()								\
{																	\
	rwmutex_dtor(_thrs_##CMP_UNIQUE.rwmutex);						\
}																	\
																	\
DECL_TYPE void _rdlock_##CMP_UNIQUE()								\
{																	\
	debug_rdlock(CMP_NAME);											\
	rwmutex_read_lock(_thrs_##CMP_UNIQUE.rwmutex);					\
}																	\
																	\
DECL_TYPE void _rdunlock_##CMP_UNIQUE()								\
{																	\
	debug_rdunlock(CMP_NAME);										\
	rwmutex_read_unlock(_thrs_##CMP_UNIQUE.rwmutex);				\
}																	\
																	\
DECL_TYPE void _wrlock_##CMP_UNIQUE()								\
{																	\
	debug_wrlock(CMP_NAME);											\
	rwmutex_write_lock(_thrs_##CMP_UNIQUE.rwmutex);					\
}																	\
																	\
DECL_TYPE void _wrunlock_##CMP_UNIQUE()								\
{																	\
	debug_wrunlock(CMP_NAME);										\
	rwmutex_write_unlock(_thrs_##CMP_UNIQUE.rwmutex);				\
}																	\
																	\
DECL_TYPE bool_t CHAIN_ITR_PROC_NAME(int32_t* index, thread_t **thread) \
{																	\
	*thread = NULL;													\
	for(; *index < MPT_MAX_THREAD_NUM								\
		&& _thrs_##CMP_UNIQUE.threads[*index] == NULL; ++(*index)); \
	if(*index == MPT_MAX_THREAD_NUM){								\
		return BOOL_FALSE;											\
	}																\
	*thread = _thrs_##CMP_UNIQUE.threads[*index];					\
	return BOOL_TRUE;												\
}																	\
		   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	\
LIB_DEF_THREAD(_thr_##CMP_UNIQUE##_entrypoint, 						\
		   THR_INIT_PROC_NAME,      								\
		   THR_MAIN_PROC_NAME,  						 			\
		   THR_DEINIT_PROC_NAME         							\
		  );														\
DECL_TYPE void* THR_START_THREAD_PROC_NAME(THR_ACTIVATOR_TYPE *activator) 	\
{																	\
	int32_t   index;												\
	CMP_TYPE *cmp;													\
	thread_t *thread;												\
	PRINTING_STARTING_SG(CMP_NAME);									\
																	\
	cmp = _cmp_##CMP_UNIQUE##_ctor();								\
	thread = make_thread(_thr_##CMP_UNIQUE##_entrypoint, cmp);		\
	cmp->CMP_ACTIVATOR_ATTR = activator;							\
	_rdlock_##CMP_UNIQUE();											\
	for(index = 0; _thrs_##CMP_UNIQUE.threads[index] != NULL; ++index); \
	_rdunlock_##CMP_UNIQUE();										\
	_wrlock_##CMP_UNIQUE();											\
	_thrs_##CMP_UNIQUE.threads[index] = thread;						\
	_wrunlock_##CMP_UNIQUE();										\
	start_thread(thread);											\
																	\
	PRINTING_SG_IS_STARTED(CMP_NAME);								\
																	\
	return NULL;													\
}																	\
																	\
DECL_TYPE void* THR_STOP_THREAD_PROC_NAME(THR_ACTIVATOR_TYPE *activator) 		\
{																	\
	CMP_TYPE *cmp = NULL;											\
	thread_t *thread = NULL;										\
	int32_t   index;												\
	PRINTING_STOPPING_SG(CMP_NAME);									\
																	\
	_wrlock_##CMP_UNIQUE();											\
	for(index = 0; 													\
		CHAIN_ITR_PROC_NAME(&index, &thread) == BOOL_TRUE; 			\
		++index) {													\
		cmp = (CMP_TYPE*) thread->arg;								\
		if(cmp->CMP_ACTIVATOR_ATTR == activator){break;}			\
	}																\
	if(thread == NULL){												\
		runtime_warning("Didn't find component for activator %p",	\
						activator);  								\
		return NULL; 												\
	}																\
	_thrs_##CMP_UNIQUE.threads[index] = NULL;						\
	_wrunlock_##CMP_UNIQUE();										\
	stop_thread(thread);											\
	thread_dtor(thread);											\
	_cmp_##CMP_UNIQUE##_dtor(cmp);									\
	PRINTING_SG_IS_STOPPED(CMP_NAME);								\
																	\
	return NULL;													\
}																	\





//------------------- threading -------------------------------

/** @struct signal_struct_t
 *  @brief A struct necessary for signal handling between two or more thread
 *  @var _devlegoio_t::mutex
 *  pointing to a mutex object used for block a thread
 *  @var _devlegoio_t::cond
 *  pointing to a conditional waiting used in two or more thread
 *  @var _devlegoio_t::waiting
 *  indicates weather the signal is in waiting mode or not
 *  @var _devlegoio_t::waiters
 *  indicates how many thread or process waiting for a signal
 */
typedef struct signal_struct_t
{
	mutex_t           *mutex;
	cond_t             cond;
	volatile bool_t    waiting;
	volatile int32_t   waiters;
}signal_t;

/** @struct barrier_struct_t
 *  @brief A struct necessary for a barrier
 *  @var _devlegoio_t::gatenum
 *	Indicates how many arrives necessary for release the barrier
 *  @var _devlegoio_t::current
 *  Indicates how many thread or process waiting at the moment
 *  @var _devlegoio_t::mutex
 *  pointing to a mutex object used for block a thread
 *  @var _devlegoio_t::cond
 *  pointing to a conditional waiting used in two or more thread
 */
typedef struct barrier_struct_t
{
	uint32_t         gatenum;
	uint32_t         current;
	pthread_mutex_t *mutex;
	pthread_cond_t   cond;
}barrier_t;

/** \typedef thread_state_t
      \brief States of the thread
*/
typedef enum{
	THREAD_STATE_CONSTRUCTED = 1,  //!< Indicates a constructing state
	THREAD_STATE_READY   = 2,      //!< Indicates a ready state
	THREAD_STATE_STARTED = 3,      //!< Indicates a started state
	THREAD_STATE_RUN = 4,          //!< Indicates a run state
	THREAD_STATE_STOP = 5,         //!< Indicates a stop state
	THREAD_STATE_STOPPED = 6,      //!< Indicates a stopped state
}thread_state_t;

/** @struct thread_struct_t
 *  @brief A struct necessary for signal handling between two or more thread
 *  @var thread_struct_t::thread_id
 *	Indicates the unique number of the thread
 *  @var thread_struct_t::handler
 *  Contains a handler to the thread
 *  @var thread_struct_t::handler_attr
 *  Contains the attributes of the handler
 *  @var thread_struct_t::state
 *  Indicates of the current state of the thread
 *  @var thread_struct_t::process
 *  Pointing to an entry process of the thread
 *  @var thread_struct_t::arg
 *  Pointing to an argument passed to the thread entrypoint
 */
typedef struct thread_struct_t
{
	int32_t          thread_id;
	threader_t       handler;
	threader_attr_t  handler_attr;
	volatile thread_state_t state;
	void *(*process)(void*);
	void *arg;
} thread_t;


/*! \fn mutex_t* mutex_ctor()
	\brief constructing a mutex object
*/
mutex_t* mutex_ctor();

/*! \fn void mutex_dtor(void *mutex)
	\brief destructing a mutex object
	\param mutex pointing to a mutex object
*/
void mutex_dtor(void *mutex);

/*! \fn void mutex_lock(mutex_t* mutex)
	\brief lock a mutex passed as an argument
	\param mutex pointing to a mutex object
*/
void mutex_lock(mutex_t* mutex);

/*! \fn void mutex_unlock(mutex_t* mutex)
	\brief unlock a mutex passed as an argument
	\param mutex pointing to a mutex object
*/
void mutex_unlock(mutex_t* mutex);

/*! \fn spin_t* spin_ctor()
	\brief constructing a spin object
*/
spin_t* spin_ctor();

/*! \fn void spin_dtor(spin_t *spin)
	\brief destructing a spin object
	\param spin pointing to a spin object
*/
void spin_dtor(spin_t *spin);

/*! \fn void spin_lock(spin_t* spin)
	\brief lock a spin passed as an argument
	\param spin pointing to a spin object
*/
void spin_lock(spin_t* spin);

/*! \fn void spin_unlock(spin_t* spin)
	\brief unlock a spin passed as an argument
	\param spin pointing to a spin object
*/
void spin_unlock(spin_t* spin);

/*! \fn signal_t* signal_ctor()
	\brief constructing a signal object
*/
signal_t* signal_ctor();

/*! \fn void signal_dtor(signal_t *signal)
	\brief destructing a signal object
	\param signal pointing to a signal object
*/
void signal_dtor(signal_t *signal);

/*! \fn void signal_set(signal_t *signal)
	\brief set a signal. It releases the signal and let one process
	entering to the critical section
	\param signal pointing to a signal object
*/
void signal_set(signal_t *signal);

/*! \fn void signal_setall(signal_t *signal)
	\brief set a signal. It releases the signal and let all process
	entering to the critical section
	\param signal pointing to a signal object
*/
void signal_setall(signal_t *signal);

/*! \fn void signal_wait(signal_t *signal)
	\brief wait for a signal. It blocks the caller thread until
	another thread set the signal and reelase the process
	\param signal pointing to a signal object
*/
void signal_wait(signal_t *signal);

/*! \fn void signal_lock(signal_t *signal)
	\brief lock a signal. It locks the signal's mutex
	\param signal pointing to a signal object
*/
void signal_lock(signal_t *signal);

/*! \fn void signal_unlock(signal_t *signal)
	\brief unlock a signal. It locks the signal's mutex
	\param signal pointing to a signal object
*/
void signal_unlock(signal_t *signal);

/*! \fn void signal_release(signal_t *signal)
	\brief release a signal's mutex if it grabbed.
	\param signal pointing to a signal object
*/
void signal_release(signal_t *signal);

/*! \fn barrier_t* barrier_ctor()
	\brief constructing a signal object
*/
barrier_t* barrier_ctor();

/*! \fn void barrier_dtor(barrier_t *barrier)
	\brief destruct a barrier
	\param signal pointing to a barrier object
*/
void barrier_dtor(barrier_t *barrier);

/*! \fn int barrier_wait(barrier_t *barrier)
	\brief indicate to the barrier that a process is arrived
	to the critical point
	\param signal pointing to a barrier object
*/
int barrier_wait(barrier_t *barrier);

/*! \fn rwmutex_t* rwmutex_ctor()
	\brief constructing a read/write mutex object
*/
rwmutex_t *rwmutex_ctor();

/*! \fn rwmutex_t* rwmutex_dtor(void *rwmutex)
	\brief destructing a read/write mutex object
*/
void rwmutex_dtor(void *rwmutex);

/*! \fn rwmutex_t* rwmutex_ctor()
	\brief read lock the mutex
	\param a read/write mutex object
*/
void rwmutex_read_lock(rwmutex_t *rwmutex);

/*! \fn rwmutex_t* rwmutex_ctor()
	\brief write lock the mutex
	\param a read/write mutex object
*/
void rwmutex_write_lock(rwmutex_t *rwmutex);

/*! \fn rwmutex_t* rwmutex_ctor()
	\brief read unlock the mutex
	\param a read/write mutex object
*/
void rwmutex_read_unlock(rwmutex_t *rwmutex);

/*! \fn rwmutex_t* rwmutex_ctor()
	\brief write unlock the mutex
	\param a read/write mutex object
*/
void rwmutex_write_unlock(rwmutex_t *rwmutex);

/*! \fn void thread_sleep(uint16_t ms)
	\brief send a thread in sleep mode for a specified amount of milliseconds
*/
void thread_sleep(uint16_t ms);

/*! \fn void thread_sleep(uint16_t ms)
	\brief send a thread in sleep mode for a specified amount of microseconds
*/
void thread_usleep(uint16_t us);

/*! \fn thread_t* thread_ctor()
	\brief constructing a thread object
*/
thread_t* thread_ctor();

/*! \fn void thread_dtor(thread_t*)
	\brief destructing a thread object
	\param thread pointing to a thread object
*/
void thread_dtor(void *thread);

/*! \fn void start_thread(thread_t*)
	\brief start a new thread
	\param thread pointing to a thread object
*/
void start_thread(thread_t*);

/*! \fn void stop_thread(thread_t*)
	\brief stop a new thread
	\param thread pointing to a thread object
*/
void stop_thread(thread_t*);

/*! \fn void setup_thread(thread_t*, void* (*)(void*), void*)
	\brief setup a thread
*/
void setup_thread(thread_t*, void* (*)(void*), void*);

//------------------- end of threading -------------------------------

//----------------- puffers implementations


/** \typedef datapuffer_t
      \brief Describe a puffer used for stores unspecified data
  */
/** @struct datapuffer_struct_t
 *  @brief Describe a puffer used for stores unspecified data
 *  @var datapuffer_struct_t::items
 *	Pointing to a set of items the puffer contains
 *  @var datapuffer_struct_t::length
 *  Indicates how many numbers if items allowed in the puffer
 *  @var datapuffer_struct_t::start
 *  pointing to the next index of the items going to be read
 *  @var datapuffer_struct_t::end
 *  pointing to the last index of the items put into items
 *  @var datapuffer_struct_t::count
 *  indicates how many items currently in the puffer
 *  @var datapuffer_struct_t::read
 *  pointing to the last item, which was read
 */
typedef struct datapuffer_struct_t
{
	void                    **items;		///< A pointer array of data the puffer will uses for storing
	int32_t                   length;		///< The maximal amount of data the puffer can store
	int32_t                   start;	///< index for read operations. It points to the next element going to be read
	int32_t                   end;	    ///< index for write operations. It points to the last element, which was written by the puffer
	int32_t                   count;
	void                     *read;
} datapuffer_t;

/*! \fn datapuffer_t* datapuffer_ctor(int32_t items_num)
	\brief constructing a datapuffer
	\param items_num indicates the maximal number of items the puffer can contain
	at the same time
*/
datapuffer_t* datapuffer_ctor(int32_t items_num);

/*! \fn void datapuffer_dtor(datapuffer_t *datapuffer)
	\brief destruct a datapuffer
	\param datapuffer pointing to a datapuffer object
*/
void datapuffer_dtor(datapuffer_t *datapuffer);

/*! \fn void datapuffer_read(datapuffer_t *datapuffer)
	\brief read one elements from the begining of the puffer.
	\param datapuffer pointing to a datapuffer object
*/
void* datapuffer_read(datapuffer_t *datapuffer);

/*! \fn void datapuffer_write(datapuffer_t *datapuffer)
	\brief write one elements to the and of the puffer
	\param datapuffer pointing to a datapuffer object
*/
void datapuffer_write(datapuffer_t *datapuffer, void *item);

/*! \fn void datapuffer_readcapacity(datapuffer_t *datapuffer)
	\brief indicates how many items stay in the puffer at the moment
	\param datapuffer pointing to a datapuffer object
*/
int32_t datapuffer_readcapacity(datapuffer_t *datapuffer);

/*! \fn void datapuffer_writecapacity(datapuffer_t *datapuffer)
	\brief indicates how many place left in the puffer
	\param datapuffer pointing to a datapuffer object
*/
int32_t datapuffer_writecapacity(datapuffer_t *datapuffer);

/*! \fn void datapuffer_isfull(datapuffer_t *datapuffer)
	\brief indicates weather the puffer is full or not
	\param datapuffer pointing to a datapuffer object
*/
bool_t datapuffer_isfull(datapuffer_t *datapuffer);

/*! \fn void datapuffer_isempty(datapuffer_t *datapuffer)
	\brief indicates weather the puffer is empty or not
	\param datapuffer pointing to a datapuffer object
*/
bool_t datapuffer_isempty(datapuffer_t *datapuffer);

/*! \fn void datapuffer_clear(datapuffer_t *datapuffer)
	\brief clear the puffer and call a specified function to every item
	the puffer contain
	\param datapuffer pointing to a datapuffer object
*/
void datapuffer_clear(datapuffer_t *datapuffer, void (*dtor)(void*));

/*! define a puffer receiver method that sleep if the puffer is full
 * until it is available for writing*/
#define GEN_PUFF_RECV_PROC_ROMA(PUFFER_PTR, DATA_PTR, SLEEP_IN_CASE_OF_FULL)  \
	/*if(PUFFER_PTR->is_full == BOOL_TRUE){*/								  \
	if(datapuffer_isfull(PUFFER_PTR) == BOOL_TRUE)					          \
		WARNINGPRINT("Puffer is full.\n");							 		  \
		do{ thread_sleep(SLEEP_IN_CASE_OF_FULL); }							  \
		while(datapuffer_isfull(PUFFER_PTR) == BOOL_TRUE);	  				  \
	}																		  \
	DEBUGPRINT("Writing to the puffer");									  \
	datapuffer_write(PUFFER_PTR, (void*) DATA_PTR);							  \
	/*Romantic policy implementation end*/

/*! define a puffer receiver method that uses wine policy.
 * It drops the latest inserted item if it is full*/
#define GEN_PUFF_RECV_PROC_WINE(PUFFER_PTR, DATA_PTR, DATA_DTOR)	 		 \
	/*Wine policy: the older the better, the fresher will wither*/			 \
	/*if(PUFFER_PTR->is_full)											   */\
	if(datapuffer_isfull(PUFFER_PTR) == BOOL_TRUE)					         \
	{																		 \
		DATA_DTOR(DATA_PTR);												 \
		WARNINGPRINT("Packet puffer is full, new packet is dropped.\n");	 \
		return;																 \
	}																		 \
	//Wine policy implementation end*/

/*! define a puffer receiver method that uses milk policy.
 * It drops the earliest inserted item if it is full*/
#define GEN_PUFF_RECV_PROC_MILK(PUFFER_PTR, DATA_TYPE, DATA_PTR, DATA_DTOR)  			\
	/*Milk policy: the fresher the better, the older will wither*/						\
	if(datapuffer_isfull(PUFFER_PTR) == BOOL_TRUE)										\
	{																					\
		DATA_DTOR((DATA_TYPE*) datapuffer_read(PUFFER_PTR));							\
		WARNINGPRINT("Packet puffer is full, the oldest packet is dropped\n");			\
	}																					\
	datapuffer_write(PUFFER_PTR, (void*) DATA_PTR);							  			\
	//Milk policy implementation end*/

/*! define a general puffer supply method
 * It supplies an item from a puffer and an empty result if it is empty*/
#define GEN_PUFF_SUPL_PROC(PUFFER_PTR, DATA_TYPE, DATA_PTR, EMPTY_RESULT)				\
	/*Data supply process*/																\
	if(datapuffer_isempty(PUFFER_PTR) == BOOL_TRUE)										\
	{																					\
		return EMPTY_RESULT;															\
	}																					\
	DEBUGPRINT("Reading from the puffer");												\
	DATA_PTR = (DATA_TYPE*) datapuffer_read(PUFFER_PTR);
	/*Data supply process end*/

/*! define a puffer clear method*/
#define GEN_PUFF_CLEAR_PROC(PUFFER_PTR, ITEM_TYPE, ITEM_DTOR)							\
		ITEM_TYPE* item;																\
		while(datapuffer_isempty(PUFFER_PTR) == BOOL_FALSE){							\
			item = (ITEM_TYPE*) datapuffer_read(PUFFER_PTR);							\
			ITEM_DTOR(item);															\
		}																				\
//--------------------- end of puffer implementations

// --------------------- make functions -----------------------
/*! \fn thread_t *make_thread(void *(*method)(void*), void *arg)
	\brief make a simple thread object setup with arguments
	\param method indicates the thread entry point
	\param arg indicates the argument passed to the entry point
*/
thread_t *make_thread(void *(*method)(void*), void *arg);

/*! \fn barrier_t *make_barrier(int32_t gatenum)
	\brief make a simple barrier object with a gatenum specified as an argument
	\param gatenum indicates the number of process have to be wait before a barrier is discarded
*/
barrier_t *make_barrier(int32_t gatenum);

//------------------ end of make functions -----------------

//------------------ vector --------------------------------

//Vector implementation in C copyright by  Hamid Alipour
//at: http://codingrecipes.com/implementation-of-a-vector-data-structure-in-c

/*! define the size of the vector*/
#define VECTOR_INIT_SIZE    4

/*! indicate weather the vector has space or not*/
#define VECTOR_HASSPACE(v)  (((v)->items_num + 1) <= (v)->items_alloc_num)

/*! indicates weather the vector i'th index inside of the bounds*/
#define VECTOR_INBOUNDS(i)	(((int) i) >= 0 && (i) < (v)->items_num)

/*! get the vector i's index*/
#define VECTOR_INDEX(i)		((char *) (v)->items + ((v)->item_size * (i)))

/** @struct _vector
 *  @brief Describe a puffer used for stores unspecified data
 *  @var _vector::items
 *	Pointing to a set of items the vector contains
 *  @var _vector::item_size
 *  Indicates how many numbers if items allowed in the vector
 *  @var _vector::items_num
 *  pointing to the current number of items
 *  @var _vector::items_alloc_num
 *  pointing to the number of allocated bytes necessary for the items
 *  @var _vector::disposer
 */
typedef struct _vector {
	void *items;
	size_t item_size;
	size_t items_num;
	size_t items_alloc_num;
    void (*disposer)(void *);
} vector_t;

/*! \fn void vector_init(vector_t *, size_t, size_t, void (*disposer)(void *))
	\brief initialize a vector passed as an argument
*/
void vector_init(vector_t *, size_t, size_t, void (*disposer)(void *));

/*! \fn void vector_dispose(vector_t *)
	\brief dispose a vector
*/
void vector_dispose(vector_t *);

/*! \fn void vector_copy(vector_t *, vector_t*)
	\brief copy the items of a vector to another vector
*/
void vector_copy(vector_t *, vector_t *);

/*! \fn void vector_insert(vector_t *, void *, size_t)
	\brief insert an item to a vector at an index
*/
void vector_insert(vector_t *, void *, size_t index);

/*! \fn void vector_insert_at(vector_t *, void *, size_t)
	\brief insert an item to a vector at an index
*/
void vector_insert_at(vector_t *, void *, size_t index);

/*! \fn void vector_push(vector_t *, void *)
	\brief push an item at the end of the vector
*/
void vector_push(vector_t *, void *);

/*! \fn void vector_pop(vector_t *, void *)
	\brief pop an item from the begining of the vector
*/
void vector_pop(vector_t *, void *);

/*! \fn void vector_shift(vector_t *, void *)
	\brief push an item to the begining of the vector
*/
void vector_shift(vector_t *, void *);

/*! \fn void vector_unshift(vector_t *, void *)
	\brief pop an item to the begining of the vector
*/
void vector_unshift(vector_t *, void *);

/*! \fn void vector_get(vector_t *, size_t, void *)
	\brief get an item from the vector at the index
*/
void vector_get(vector_t *, size_t, void *);

/*! \fn void vector_remove(vector_t *, size_t)
	\brief remove an item from the vector
*/
void vector_remove(vector_t *, size_t);

/*! \fn void vector_transpose(vector_t *, size_t, size_t)
	\brief transpose a vector from size1 to size2
*/
void vector_transpose(vector_t *, size_t, size_t);

/*! \fn void vector_length(vector_t *)
	\brief get the length of the vector
*/
size_t vector_length(vector_t *);

/*! \fn void vector_length(vector_t *)
	\brief get the size of the vector
*/
size_t vector_size(vector_t *);

/*! \fn void vector_get_all(vector_t *)
	\brief get all elements from a vector to a specified memory address
*/
void vector_get_all(vector_t *, void *);

/*! \fn void vector_cmp_all(vector_t *, void *, int (*cmp_func)(const void *, const void *))
	\brief get an element fit best for the results
*/
void vector_cmp_all(vector_t *, void *, int (*cmp_func)(const void *, const void *));

/*! \fn void vector_qsort(vector_t *, void *, int (*cmp_func)(const void *, const void *))
	\brief sort the vector using the comparator function
*/
void vector_qsort(vector_t *, int (*cmp_func)(const void *, const void *));

/*! \fn void vector_grow(vector_t *, size_t)
	\brief expand the vector to a specified amount of size
*/
void vector_grow(vector_t *, size_t);

/*! \fn void vector_swap(void *, void *, size_t)
	\brief swap two items in the vector
*/
void vector_swap(void *, void *, size_t);

//----------------------- end of vector -----------------------------------------


// ---------------- other functions ---------------------------------
/*! \fn void ptrmov(void **, void **)
	\brief move a value of a pointer from another pointer
	and zeroing the source pointer
*/
void ptrmov(void **dst, void **src);

void func_do_nothing();
void func_with_param_do_nothing(void *param);
void func_with_2param_do_nothing(void *param);
bool_t func_return_true();
bool_t func_with_param_return_true(void *param);
//----------------- end of other functons ----------------------------

/*! \fn void dispose_thread(thread_t *)
	\brief dispose a thread
*/
void dispose_thread(thread_t*);

#ifdef DEVLEGO_REALTIME_LIBRARY_NEEDED
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/timex.h>
#include <sys/types.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <linux/ptp_clock.h>
#include <sys/timerfd.h>
#include <pthread.h>
#include <signal.h>

#ifndef ADJ_SETOFFSET
#define ADJ_SETOFFSET 0x0100
#endif

#ifndef CLOCK_INVALID
#define CLOCK_INVALID -1
#endif

/** @struct interrupt_timer_struct_t
 *  @brief Describes an interrupt timer
 *  @var interrupt_timer_struct_t::sigevent
 *	contains an event describer specifies the signal from the operation system
 *	trigger this interrupt
 *  @var interrupt_timer_struct_t::timeout
 *  Describes the timeout
 *  @var interrupt_timer_struct_t::timerid
 *  describes the timer
 */
typedef struct interrupt_timer_struct_t
{
	struct sigevent   sigevent;
	struct itimerspec timeout;
	timer_t           timerid;
	//clockid_t         clkid;
}interrupt_timer_t;

/** @struct timeperiod_struct_t
 *  @brief Describes a time interval
 *  @var timeperiod_struct_t::s
 *	indicate the seconds of the timer
 *  @var timeperiod_struct_t::ms
 *	indicate the milliseconds of the timer
 *  @var timeperiod_struct_t::us
 *	indicate the microseconds of the timer
 *  @var timeperiod_struct_t::ns
 *	indicate the nanoseconds of the timer
 */
typedef struct timeperiod_struct_t{
	uint64_t   s  : 34;
	uint64_t   ms : 10;
	uint64_t   us : 10;
	uint64_t   ns : 10;
}timeinterval_t;

/** @struct interrupt_struct_t
 *  @brief Describes an interrupt
 *  @var interrupt_struct_t::signum
 *  indicate the signal number
 *  @var interrupt_struct_t::action
 *  indicate the signal action
 *  @var interrupt_struct_t::initial_block
 *  indicate weather the signal needs to be blocked after it is initialized or not.
 *  @var interrupt_struct_t::mask
 *  describes the mask of to the signal
 *  @var interrupt_struct_t::timer
 *  describes the timer
 *  @var interrupt_struct_t::period
 *  describes the period of the signal
 *  @var interrupt_struct_t::initial
 *  describes the initial time of the signal
 */
typedef struct interrupt_struct_t
{
	int               signum;
	struct sigaction  action;
	bool_t            initial_block;
	sigset_t          mask;
	interrupt_timer_t timer;
	timeinterval_t    period;
	timeinterval_t    initial;
}interrupt_t;

/** @struct timer_struct_t
 *  @brief Describes a timer
 *  @var timer_struct_t::fd
 *  indicate the file descriptor
 *  @var timer_struct_t::expired
 *  indicate weather the timer is expired or not
 *  @var timer_struct_t::period
 *  Indicate the period of the timer
 *  @var timer_struct_t::initial
 *  Indicate the initial time of the timer
 *  @var timer_struct_t::new_value
 *  Indicate an itimerspec
 */
typedef struct timer_struct_t
{
	int               fd;
	uint64_t          expired;
	timeinterval_t    period;
	timeinterval_t    initial;
	struct itimerspec new_value;
}timerfd_t;

/*! \fn void init_interrupt(interrupt_t *interrupt, int signum, bool_t initial_block)
	\brief initialize an interrupt with a signal
	\param interrupt the interrupt that has to be initialized
	\param signum the signal number
	\param initial_block indicate weather the signal has to be blocked at initial
*/
void init_interrupt(interrupt_t *interrupt, int signum, bool_t initial_block);

/*! \fn void set_interrupt_sh(interrupt_t *interrupt, void (*handler)(int))
	\brief setup the handler of the interrupt
	\param interrupt pointing to an interrupt
	\param handler pointing to the handler
*/
void set_interrupt_sh(interrupt_t *interrupt, void (*handler)(int));

/*! \fn void set_interrupt_ph(interrupt_t *interrupt, void (*handler)(int))
	\brief setup the handler of the interrupt
	\param interrupt pointing to an interrupt
	\param handler pointing to the handler
*/
void set_interrupt_ph(interrupt_t *interrupt, void (*handler)(int));

/*! \fn void interrupt_block(interrupt_t *interrupt)
	\brief mask the interrupt signal
	\param interrupt pointing to an interrupt
*/
void interrupt_block(interrupt_t *interrupt);

/*! \fn void interrupt_unblock(interrupt_t *interrupt)
	\brief unmask the interrupt signal
	\param interrupt pointing to an interrupt
*/
void interrupt_unblock(interrupt_t *interrupt);

/*! \fn void interrupt_wait(interrupt_t *interrupt)
	\brief wait for the interrupt signal
	\param interrupt pointing to an interrupt
*/
void interrupt_wait(interrupt_t *interrupt);

/*! \fn void interrupt_waitandunblock(interrupt_t *interrupt)
	\brief wait for the interrupt signal and block it immediatelly
	\param interrupt pointing to an interrupt
*/
void interrupt_waitandunblock(interrupt_t *interrupt);

/*! \fn void interrupt_waitandcallsh(interrupt_t *interrupt)
	\brief wait for the interrupt signal and call the handler function
	\param interrupt pointing to an interrupt
*/
void interrupt_waitandcallsh(interrupt_t *interrupt);

/*! \fn void interrupt_arm(interrupt_t *interrupt, clockid_t clkid)
	\brief arm the interrupt
	\param interrupt pointing to an interrupt
*/
void interrupt_arm(interrupt_t *interrupt, clockid_t clkid);

/*! \fn void interrupt_disarm(interrupt_t *interrupt)
	\brief disarm the interrupt
	\param interrupt pointing to an interrupt
*/
void interrupt_disarm(interrupt_t *interrupt);

/*! \fn void set_timeinterval(timeinterval_t *timeinterval, int32_t s, int32_t ms, int32_t us, int32_t ns);
	\brief setup a timeinterval
	\param interval pointing to an interval
	\param s indicates the seconds
	\param ms indicates the milliseconds
	\param us indicates the microseconds
	\param ns indicates the nanoseconds
*/
void set_timeinterval(timeinterval_t *timeinterval, int32_t s, int32_t ms, int32_t us, int32_t ns);

/*! \fn timerfd_t* timer_ctor(interrupt_t *interrupt)
	\brief construct a timer
*/
timerfd_t* timer_ctor();

/*! \fn void timer_dtor(timerfd_t *timer)
	\brief destruct a timer
	\param timer pointing to a timer
*/
void timer_dtor(timerfd_t *timer);

/*! \fn void timer_wait(timerfd_t *timer)
	\brief wait to a timer
	\param timer pointing to a timer
*/
int32_t timer_wait(timerfd_t *timer);

/*! \fn void timer_wait(timerfd_t *timer)
	\brief arm a timer
	\param timer pointing to a timer
*/
void timer_arm(timerfd_t *timer);

/*! \fn void timer_wait(timerfd_t *timer)
	\brief disarm a timer
	\param timer pointing to a timer
*/
void timer_disarm(timerfd_t *timer);

#endif /* DEVLEGO_REALTIME_LIBRARY_NEEDED*/

/*! Indicate weather the datamap table is full or not*/
#define DEVCLEGO_ERROR_TABLE_IS_FULL -1

/*! Indicate weather the item in the table is found or not*/
#define DEVCLEGO_DMAP_ERROR_ITEM_NOT_FOUND -1
/*! Indicate weather an error occured durint the add process or not*/
#define DEVCLEGO_DMAP_ERROR_ADD -2

//dmap creation
/*! \fn void dmap_itr_do()
	\brief iterate a datamap table and call a function to its item consequently
	\param dmap_iterator pointing to an iterator function
	\param action pointing to an action function
*/
void dmap_itr_do(bool_t (*dmap_iterator)(int32_t*, void*), void* (*action)(void*));

/*! \fn dmap_row_t* _dmap_row_ctor_()
	\brief construct a datamap row
*/
dmap_row_t* _dmap_row_ctor_();

/*! \fn dmap_row_t* _dmap_row_ctor_()
	\brief destruct a datamap row
*/
void        _dmap_row_dtor(dmap_row_t *row);

/*! \fn dmap_row_t* _make_dmap_row(void *item, void (*dtor)(void*))
	\brief make a datamap row
	\param item pointing to an item
	\param dtor pointing to an item destructor
*/
dmap_row_t* _make_dmap_row(void *item, void (*dtor)(void*));

/*! \fn dmap_row_t* _dmap_table_init((dmap_table_t *table, int32_t size, char_t *name)
	\brief initialize a datamap table
	\param table pointing to a table
	\param size determine the table size
	\param name determines the name of the table
*/
void        _dmap_table_init(dmap_table_t *table, int32_t size, char_t *name)      __attribute__((nonnull (1)));

/*! \fn void _dmap_table_deinit((dmap_table_t *table, int32_t size, char_t *name)
	\brief initialize a datamap table
	\param table pointing to a table
	\param size determine the table size
	\param name determines the name of the table
*/
void        _dmap_table_deinit(dmap_table_t *table);

/*! \fn _dmap_itr_table _dmap_itr_table((dmap_table_t *table, int32_t index, dmap_row_t **row)
	\brief table iterator
	\param table pointing to a table
	\param index indicates the index exemines by the iterator
	\param row pointing to a pointer pointing to a row
*/
bool_t      _dmap_itr_table(dmap_table_t *table, int32_t *index, dmap_row_t **row) __attribute__((nonnull (1,2,3)));

/*! \fn void _dmap_get_item((dmap_table_t *table, int32_t dmap_index)
	\brief get  a datamap item
	\param table pointing to a table
	\param dmap_index indicate a datamap index
*/
void*       _dmap_get_item(dmap_table_t *table, int32_t dmap_index)                __attribute__((nonnull (1)));

/*! \fn void _dmap_rem_item((dmap_table_t *table, void* item)
	\brief remove a datamap table
	\param table pointing to a table
	\param item pointing to an item
*/
void        _dmap_rem_item(dmap_table_t *table, void *item)                        __attribute__((nonnull (1)));

/*! \fn void _dmap_rem_item((dmap_table_t *table, int32_t dmap_index)
	\brief remove a datamap table
	\param table pointing to a table
	\param dmap_index indicates the inde inside of the table
*/
void        _dmap_rem(dmap_table_t *table, int32_t dmap_index)                     __attribute__((nonnull (1)));

/*! \fn void _dmap_add_row((dmap_table_t *table, dmap_row_t *row)
	\brief remove a datamap table
	\param table pointing to a table
	\param dmap_row_t pointing to a row
*/
int32_t     _dmap_add_row(dmap_table_t *table, dmap_row_t *row)                    __attribute__((nonnull (1)));


#endif /* DEVCLEGO_H_ */
