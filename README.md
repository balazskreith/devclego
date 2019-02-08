Devclego
===

Devclego is a skeleton service written in C programming language in order to
create services in a low-level programming language capable of processing
data and react external events. The skeleton contains libraries and predefined macros
for threading, making connectable components and create finite state machine.

## 1. Quick start


## 2. Docker support


## 3. Developer manuals

In order to develop using devclego efficiently you need to understand
how the directory structure is intended to be used, how you can
create and connect components, and how you can use events
in a finite state machine to trigger actions and change the service
behaviour.

### 3.1 High level overview

Building a responsive, scalable service requires three

### 3.1. Directory structure
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

### 3.2. Datamaps

Datamap stores iterable and searchable custom typed structures,
and cover these structures by functions, which are thread safe. Take a look at the following declaration generate necessary functions
to cover globally available operations to access a datamap stores the path_t structure.

```C
typedef struct path_struct_t
{
        char_t  name[255];
        char ipv4_address[4];
}path_t;


DMAP_DECL_SPECT_TABLE(
        path_t,                      /* type of the items stored in the table */
        dmap_get_table_path,         /* name of the process gets the table */
        dmap_get_path,               /* name of the process gets an item by index */
        dmap_add_path,               /* name of the process adds an item and returns its index */
        dmap_rem_path_byindex,       /* name of the process remove an item by index */
        dmap_rem_path,               /* name of the process remove an item by pointer */
        dmap_rdlock_table_path,      /* name of the process lock the table for reading */
        dmap_rdunlock_table_path,    /* name of the process unlock the table from reading */
        dmap_wrlock_table_path,      /* name of the process lock the table for writing */
        dmap_wrunlock_table_path,    /* name of the process unlock the table from writing */
        dmap_itr_table_path          /* name of the process iterate the table */
);
```



## License

[Apache 2.0 License](LICENSE.md) - &copy; Balazs Kreith
