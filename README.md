Devclego
===

Devclego is a lightweight skeleton for services written in C programming language. It is intended to be used to create services in a low-level programming language capable of processing
data and react external events. The skeleton contains libraries and predefined macros for threading, making connectable components and create finite state machine.

## Table of contents

- [Quick start](#quick-start)
- [Developer manuals](#developer-manuals)
- [Bugs and feature requests](#bugs-and-feature-requests)
- [Contributing](#contributing)
- [Versioning](#versioning)
- [Creators](#creators)
- [Personal note](#personal-note)
- [License](#developer-manuals)


## 1. Quick start

To run the service quickly we can use docker.

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

Assume you need to use multiple network paths in your service, and your components sending the packets need to choose one, meanwhile the high-level finite state machine at any time can receive an event triggers add or remove path commands. You defined a datamap, which is globally available in your service. [Here](docs/datamaps.md) you can see how to define a datamap with a custom structure. Below is an example how to use a datamap for paths in your code.

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

There are many type of components we can define and use to build a service. We can choose to have a component running in it's own thread, we can paralelize the component processes, or signaling between different threads, etc. A full description of components can be found [here](docs/components.md).


### 2.5. Finite State Machines

Finite State Machines (FSMs)are high-level abstraction to describe a program itself. Without going to the details you can read more about FSM [here](https://brilliant.org/wiki/finite-state-machines/). In short You need to know the following concept: State, Events, Action, and Transition.
At a certain time the program is in a certain State (i.e.: Run), and for a certain external or internal event (i.e.: Interruption), an action must be performed (i.e.: Stop), for which as a result the program is going to be in a new state (i.e.: Halt). We call an event and an action together a transition.

Now, if we implement a service there are external events (i.e.: received commands) cause actions, which changes configurations, and as a consequence behavior modifications in our application. To organise and make all of the actions trackable we should define an FSM in our application, which describe the application behavior and also it can be used to extend the service capabilities later.   

**Usage**

```C
void program(struct arg_int  *time) {
    int sec = 0;

    /*Firing the FSM so lanch the program*/
    get_fsm()->fire(EVENT_SETUP, NULL); /* TODO: if you have a config file use the path instead of null */
    get_fsm()->fire(EVENT_START, NULL);

    //If time argument is given the program terminates itself automatically
    if(time->count){
      INFOPRINT("The program is going to be terminated %d sec later", *(time->ival));
    }

    while(running){
      thread_sleep(1000);
      if(time->count && *(time->ival) < ++sec){
        break;
      }
    }

    get_fsm()->fire(EVENT_STOP, NULL);
    get_fsm()->fire(EVENT_SHUTDOWN, NULL);
}
```

In default the main entry point of the program fires a setup and start events, before
the service starts running. Similarly a stop and teardown events triggers certain action if the service needs to be stopped. More about FSM is [here](docs/fsm.md)

### 2.6. Libraries

Library contains collections, interruptions, signals, buffers, and other basic library we could use to implement the service logic. The description of libraries can be found [here](docs/libraries.md).


## 3. Bugs and feature requests

I appreciate every comments, and bug requests you do, please create an issue for each.


## 4. Contributing

You are welcome.

## 5. Versioning

In the main.c there is a constant called VERSION.


## 6. Creators

- [Balazs Kreith](http://balazs.kreith.hu)

## 7. Personal note

After working several years creating applications and services processing mainly data I realized that a good service must be as small as possible, meanwhile the best way to extend and maintain it is to organize it's behaviors and reactions in a certain way.

This project was created as a reaction of the following experiences:

 - If I needed to create a small low-level application processes data and generate certain things the code is gonna be messy after 2000 lines, no matter how beautifully I named my variables and functions.
 - If I needed to create a larger application in C, I needed to include large third party libraries just to use queues and stacks, which is simply inefficient.
 - For data processing flow-based programming paradigm is good. To implement an API the event-based programming is adequate, to store data globally available during the application lifetime the database is overkill, using gobal variable is messy.

Using FSM, you can describe what actions needs to be done after a certain event effects your application. Using flow-based programming, components clearly describe what operations they are doing and you can clearly see which component is connected to which other. And using a lightweight fast globally available (called it) datamaps in a thread safe manner saves space, time and improve efficiency.

If you use or borrow idea for your own project from this one, it worth the effort to write down all these lines.

## 8. Copyright and license

I haven't think about it, but here it is one:

 - [Apache 2.0 License](LICENSE.md) - &copy; Balazs Kreith
