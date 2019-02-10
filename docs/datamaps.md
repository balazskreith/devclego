Datamaps in devclego
===



Take a look at the following declaration generate necessary functions
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
