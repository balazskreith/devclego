Devclego
===

Devclego is a skeleton service written in C programming language in order to
create services in a low-level programming language capable of processing
data and react external events. The skeleton contains libraries and predefined macros
for threading, making connectable components and create finite state machine.

## Table of contents

- [Quick start](#quick-start)
- [Docker support](#docker-support)
- [Developer manuals](#developer-manuals)
- [Using libraries](#using-libraries)
- [License](#developer-manuals)


## 1. Quick start


## 2. Docker support


## 3. Developer manuals

In order to develop using devclego efficiently you need to understand
how the directory structure is intended to be used, how you can
create and connect components, and how you can use events
in a finite state machine to trigger actions and change the service
behaviour.

### 3.1 High level overview

The service written in devclego, processing data and react to events are using the
following techniques:

 - **Datamap**: A managed globally available shared data to store configurations
 - **Components**: Separable functional entities capable of connecting to each other
 - **Finite State Machine**: Describe the application behaviour and defines actions to events.  

### 3.2. Directory structure

Within the download you'll find the following directories and files, logically grouping common assets. You'll see something like this:


```
├─ bin                        # contains the compiled binary file  
├─ desc                       # contains descriptions about this skeleton
└─ src                        # contains all source files  
    ├── cmp                   # contains the components  
    ├── conf                  # contains configuration ini files
    ├── dmap                  # contains datamap implementations
    ├── etc                   # contains included third party sources
    ├── fsm                   # implements a finite state machine
    ├── inc                   # contain files needs to be included anywhere
    ├── lib                   # contains libraries we define and use
    ├── main.c                # entry poit of the application
    ├── Makefile.am           # contains the list of files needs to be compiled
    ├── sys                   # contains system processes

```

A library for threading, signaling, interruption and basic collections are provided. By developing an application you should extend the basic functionalities and place your piece of codes in the structure it belongs.


### 3.3. Datamaps

Datamap stores iterable and searchable custom typed structures,
and cover these structures by functions, which are thread safe. Take a look at the following declaration generate necessary functions
to cover globally available operations to access a datamap stores the ``` path_t``` structure.

```C
typedef struct path_struct_t
{
        char_t  name[255];
        char ipv4_address[4];
}path_t;


DMAP_DECL_SPECT_TABLE(
        path_t,                      /* Type of the items stored in the table */
        dmap_get_table_path,         /* Declare the process gets the table */
        dmap_get_path,               /* Declare the process gets an item by index */
        dmap_add_path,               /* Declare the process adds an item and returns its index */
        dmap_rem_path_byindex,       /* Declare the process remove an item by index */
        dmap_rem_path,               /* Declare the process remove an item by pointer */
        dmap_rdlock_table_path,      /* Declare the process lock the table for reading */
        dmap_rdunlock_table_path,    /* Declare the process unlock the table from reading */
        dmap_wrlock_table_path,      /* Declare the process lock the table for writing */
        dmap_wrunlock_table_path,    /* Declare the process unlock the table from writing */
        dmap_itr_table_path          /* Declare the process iterate the table */
);
```

This macro is used to declare functions, which are going to be globally available in the application lifetime. You need to place it in the header file. Because dmaps are not dynamic (sorry for that) it's basically a protected array of pointers. After every add an index is returned, which can be use to retrieve data from the table directly. We can call that index as the id of the item of the data.
Below there is a naming convention recommended to use datamap, as well as the functionalities are described.

 - **dmap_get_table_[NAME]**: Define access the table properties we use. We can retrieve the table size, name and the stored actual data.
 - **dmap_get_[NAME]** Access to a certain item inside of the array of data.  For example dmap_get_path(0) returns a path_t structure if it exists at the 0th index or NULL otherwise.
 - **dmap_add_[NAME]** Adds an item to the array and returns the index of it.
 - **dmap_rem_[NAME]_by_index**: Removes an element by it's index.
 - **dmap_rem_[NAME]**: Removes an item by using the memory address of the item as a parameter.
 - **dmap_rdlock_table_[NAME]** Sets a lock for reading.
 -  **dmap_rdunlock_table_[NAME]**: Release a lock for reading.
 - **dmap_wrlock_table_[NAME]** Sets a lock for writing.
 - **dmap_wrunlock_table_[NAME]** Release a lock for writing
 - **dmap_itr_table_[NAME]**: Iterate through the items.

In the body file (.c) the definition of the above declared functions can be simplified by using the following macro:

```C
#define PATH_TABLE_LENGTH 10

DMAP_DEF_SPECT_TABLE(
        path_t,                   /* Specifies the type of the data stored here */
        path_dtor,                /* Specified the destructor in case of removing an element */
        dmap_table_path,          /* Static variable use to store the datamap */
        "paths",                  /* Define a custom name for the datamap */
        PATH_TABLE_LENGTH,        /* Define the maximal number of item the datamap can contain. */
        dmap_init_table_path,     /* Define the process initialize the table*/
        dmap_deinit_table_path,   /* Define the process deinitialze the table*/
        dmap_get_table_path,      /* Define the process gets the table*/
        dmap_get_path,            /* Define the process gets an item from the table by it's index */
        dmap_add_path,            /* Define the process adds an item and returns with its index*/
        dmap_rem_path_byindex,    /* Define the process remove an item by index */
        dmap_rem_path,            /* Define the process remove an item by pointer */
        dmap_rdlock_table_path,   /* Define the process lock the table for reading */
        dmap_rdunlock_table_path, /* Define the process unlock the table from reading*/
        dmap_wrlock_table_path,   /* Define the process lock the table for writing*/
        dmap_wrunlock_table_path, /* Define the process unlock the table from writing*/
        dmap_itr_table_path       /* Define the process iterate the table */
);

```

Finally you need to initialize the datamap before you use. Add your initialization function
to [dmap_init](https://github.com/balazskreith/devclego/blob/d4f9172a6e1e1ed5bdbc78c539ce2f73654d95e7/src/dmap/dmap.c#L70) and your deinitialization to the corresponding dmap_deinit, they are called automatically at startup and tear down.

The skeleton contains a built-in datamap for keeping thrack of the threads and make it possible
to query them. See [dmap](https://github.com/balazskreith/devclego/tree/master/src/dmap) folder for details in the source.

**Usage**

```C
#include "defs.h"

// Reading from the datamap
int32_t   index;
path_t *path;

dmap_rdlock_table_path();
printf("dmap name is %s and size is %d", dmap_get_table_path()->name, dmap_get_table_path()->size);

for(index = 0; dmap_itr_table_path(&index, &path) == BOOL_TRUE; ++index){
        printf("The name of the path is: %s\n", path->name);
}
dmap_rdunlock_table_path();

dmap_wrlock_table_path();
path = dmap_get_path(0);
strcpy(path->name, "new value");
dmap_wrunlock_table_path();
```

### 3.4. Components

Components are the basic elements for data processing in devclego. They cover functional entities operating on data goes through them. We can imagine the whole application as a network of black-boxes, where the black-boxes are the components. This type of application design is called [flow-based programming](http://www.jpaulmorrison.com/fbp/).

In the following we declare components in the header files (.h) and define their processes in the body file (.c). Components should be placed in the [cmp](https://github.com/balazskreith/devclego/tree/master/src/cmp) folder.

Let's define a mycmps.h as follows:

```C
// mycmp.h
#include <stdio.h>
#include <stdlib.h>
#include "cmp.h"

typedef struct packet_struct_t {
  char bytes[1024];
}packet_t;

typedef struct _cmp_A_struct_t {
  void (*receiver)(packet_t*);
  void (*send)(packet_t*);
  int32_t id;
}_cmp_A_t;


typedef struct _cmp_B_struct_t {
  void (*receiver)(packet_t*);
}_cmp_B_t;
```

And the corresponding mycmps.c as follows

```C
// mycmps.c
#include "mycmps.h"

// Here we define component A
#define CMP_NAME_A "A component"
CMP_DEF(
   static,               // Type of definition. static means every function marked as static
   _cmp_A_t,             // Type of component
   CMP_NAME_A,           // Name of the component
   _cmp_A,               // Variable name of the component
   _cmp_A_ctor,          // Name of the constructor process implemented automatically
   _cmp_A_dtor,          // Name of the destructor process implemented automatically
   __CMP_NO_INIT_FUNC_,  // Name of the external process called after construction
   __NO_TEST_FUNC_,      // Name of the external process called after initialization
   __CMP_NO_DEINIT_FUNC_ // Name of the external process called before destruction
);


// Here we define component B
#define CMP_NAME_B "B component"
CMP_DEF(
   static,               // Type of definition. static means every function marked as static
   _cmp_B_t,             // Type of component
   CMP_NAME_B,           // Name of the component
   _cmp_B,               // Variable name of the component
   _cmp_B_ctor,          // Name of the constructor process implemented automatically
   _cmp_B_dtor,          // Name of the destructor process implemented automatically
   __CMP_NO_INIT_FUNC_,  // Name of the external process called after construction
   __NO_TEST_FUNC_,      // Name of the external process called after initialization
   __CMP_NO_DEINIT_FUNC_ // Name of the external process called before destruction
);

static void _cmp_A_process(packet_t*);
static void _cmp_B_process(packet_t*);

```

## License

[Apache 2.0 License](LICENSE.md) - &copy; Balazs Kreith
