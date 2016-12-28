#include <stdio.h>
#include <stdlib.h>
#include "inc_inet.h"
#include "inc_predefs.h"
#include "inc_socket.h"
#include "inc_unistd.h"
#include "inc_io.h"
#include "lib_defs.h"
#include "lib_descs.h"
#include "inc_predefs.h"
#include "fsm.h"
#include "etc_argtable2.h"
#include "lib_interrupting.h"

/*An example for using interrupts inside of the program.
 * exit_signal interrupts the program and executes the eit_handler if ctrl+c is pressed
 * */
static interrupt_t exit_signal;

/**/
static void init_sysio();
static void program(struct arg_int  *time);
static void _exit_handler(int sgno);

/**
 * Main function of server applcation that creates the tunnel interface, starts threads and configures the connections
 */
int main(int argc, char **argv)
{
    //initializing system io components
    init_sysio();
    const char* __progname__ = argv[0];

    /*Setting up the interrupt and binding it to a handler*/
    init_interrupt(&exit_signal, SIGINT, BOOL_FALSE);
    set_interrupt_sh(&exit_signal, _exit_handler);

    /*The program uses argtable2 implementation for parsing arguments
     * More information and examples for argtable2:
     *
     * http://argtable.sourceforge.net/example/index.html
     *
     * */
    struct arg_lit  *help = arg_lit0("h","help","show help");
    struct arg_int  *time = arg_int0("t", "time", "<n>", "time interval");
    struct arg_end  *end  = arg_end(20);
    void* argtable[] = {help, time, end};
    int nerrors;
    int exitcode=EXIT_SUCCESS;


    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
    {
      /* NULL entries were detected, some allocations must have failed */
      ERRORPRINT("%s: insufficient memory\n",__progname__);
      exitcode=1;
      goto exit;
    }

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
      INFOPRINT("Usage: %s", __progname__);
      arg_print_syntax(stdout,argtable,"\n");
      INFOPRINT("Network Traffic Rate Tracker (ntrt).\n\n");
      arg_print_glossary(stdout,argtable,"  %-20s %s\n");
      exitcode=0;
      goto exit;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
      /* Display the error details contained in the arg_end struct.*/
      arg_print_errors(stdout,end,__progname__);
      INFOPRINT("Try '%s --help' for more information.\n",__progname__);
      exitcode=1;
      goto exit;
    }


    /*devclego starts here
     * Build the abstract finite state machine by ceating it
     * and then running the program.
     * After the program proc finished, fsm is doomed.*/
    fsm_ctor();
    program(time);
    fsm_dtor();
exit:
	return exitcode;
}

/*In case of catching the terminate signal we have some room for
 * finishing stuff and closing connections by triggering a shutdown for the fsm*/
void _exit_handler(int sgno)
{
  CRITICALPRINT("%s", "The program is terminated");
  get_fsm()->fire(EVENT_STOP, NULL);
  get_fsm()->fire(EVENT_SHUTDOWN, NULL);
  fsm_dtor();
}

void program(struct arg_int  *time)
{
    int sec = 0;

    /*Firing the FSM so lanch the program*/
    get_fsm()->fire(EVENT_SETUP, NULL);
    get_fsm()->fire(EVENT_START, NULL);

    //If time argument is given the program terminates itself automatically
    if(time->count){
      INFOPRINT("The program is going to be terminated %d sec later", *(time->ival));
    }

    while(1){
      thread_sleep(1000);
      if(time->count && *(time->ival) < ++sec){
        break;
      }
    }

    get_fsm()->fire(EVENT_STOP, NULL);
    get_fsm()->fire(EVENT_SHUTDOWN, NULL);

}

void init_sysio()
{
    sysio = (devlegoio_t*) malloc(sizeof(devlegoio_t));
    sysio->print_stdlog = sysio_print_stdlog;
    sysio->print_stdout = sysio_print_stdout;
    sysio->opcall = system;
}
