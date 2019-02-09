Devclego
===

Devclego is a skeleton service written in C programming language in order to
create services in a low-level programming language capable of processing
data and react external events. The skeleton contains libraries and predefined macros
for threading, making connectable components and create finite state machine.

## Table of contents

- [Quick start](#quick-start)
- [Developer manuals](#developer-manuals)
- [Using libraries](#using-libraries)
- [License](#developer-manuals)


## 1. Quick start

To run the service quickly we use docker.

**Step 1.** Build the image compile and run your service
```
docker build -t myservice .
```

**Step 2.** Run your service

```
docker run myservice -d
```

You can see the logfiles at the pid can be retrieved by using ``` docker ps ```

```
docker logs [pid]
```

You need to add your application logic follows the skeleton scheme if you want to build your specific service. So let's read the Developers manual if you wanna use this in the appropriate way.

## 2. Developer manuals

In order to develop using devclego efficiently you need to understand
how the directory structure is intended to be used, how you can
create and connect components, and how you can use events
in a finite state machine to trigger actions and change the service
behaviour.

### 2.1 High level overview

The service written in devclego, processing data and react to events are using the
following techniques:

 - **Datamap**: A managed globally available shared data to store configurations
 - **Components**: Separable functional entities capable of connecting to each other
 - **Finite State Machine**: Describe the application behaviour and defines actions to events.  

### 2.2. Directory structure

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


### 2.3. Datamaps

Datamap stores custom data structures iterable and searchable globally
in a thread safe manner, It serves as a local database every components
can access.


**Usage**

Assume you need to use multiple network paths in your service, and your components sending the packets need to choose one, meanwhile the high-level finite state machine at any time can receive an event triggers add or remove path commands. You defined a datamap, which is globally available in your service. [Here](desc/datamaps.md) you can see how to define a datamap with a custom structure. Below is an example how to use a datamap for paths in your code.

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

### 2.4. Components

Components are the basic elements for data processing in devclego. They cover functional entities operating on data goes through them. We can imagine the whole application as a network of black-boxes, where the black-boxes are the components. This type of application design is called [flow-based programming](http://www.jpaulmorrison.com/fbp/).

**Usage**

Assume your service need to process packets going through your system. You define a component A receiving packets and sending towards component B (->A->B). A and B components are operating on packets.

```C
#include "cmp_a.h"
#include "cmp_b.h"

int main(int argc, char** argv) {
  cmp_A_t* cmp_a = cmp_a_get();
  cmp_B_t* cmp_b = cmp_b_get();

  CMP_CONNECT(cmp_a->send, cmp_b->receive);

  packet_t packet;
  packet.bytes[2] = 0; // CMP A will change the second byte

  cmp_a->receive(&packet);
}
```

So what have just happened in this program? We included two components (A, and B) connected each other and send a packet to A. Note component A does not know about component B, the connection between the components defined outside of them. This make it flexible in a way of adding new processing elements between them. Furthermore if A has one or more output (or send functions)
we can choose, which we send the packet.

There are many type of components we can define and use to build a service. We can choose to have a component running in it's own thread, we can paralelize the component processes signaling between different threads, etc. A full description of components can be found [here](desc/components.md).


### 2.5. Finite State Machines

**Usage**

### 2.6. Libraries 

## License

[Apache 2.0 License](LICENSE.md) - &copy; Balazs Kreith
