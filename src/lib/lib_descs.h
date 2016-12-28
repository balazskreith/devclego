/**
 * @file lib_descs.h
 * @brief struct descriptions used by the mpt program
 * @author Bal�zs, Kreith; Debrecen, Hungary
 * @copyright Project maintained by Almasi, Bela; Debrecen, Hungary
 * @date 2014.02.11
*/
#ifndef INCGUARD_PROGRAM_LIBRARY_DESCRIPTIONS_H_
#define INCGUARD_PROGRAM_LIBRARY_DESCRIPTIONS_H_

#include <time.h>
#include "lib_defs.h"
#include "lib_threading.h"
#include <signal.h>
#include <pcap.h>

#define PROGRAM_MAX_THREAD_NUM 32
#define PROGRAM_TICK_PER_MS_RATIO 1.024
#define PROGRAM_MS_PER_TICK_RATIO 0.9765625


typedef struct eveter_arg_struct_t
{
	int32_t      event;
	void*        arg;
}eventer_arg_t;

typedef struct sysdat_struct_t
{
  //global available and singletone data
}sysdat_t;

typedef struct array_struct_t
{
	void *items;
	int32_t length;
}array_t;

/*For communicating through the operating system
 * function pointers and handlers should be listed here
 * and should be use by via sysio-> 'object'.*/

typedef struct devlegoio_struct_t
{
	//print functionalities
	void     	 (*print_stdout)(const char*,...);
	void     	 (*print_stdlog)(const char*,...);
	int32_t          (*opcall)(const char*);
//	char_t           bash_dir[255];
	//
}devlegoio_t;

devlegoio_t *sysio;

#endif //INCGUARD_PROGRAM_LIBRARY_DESCRIPTIONS_H_
