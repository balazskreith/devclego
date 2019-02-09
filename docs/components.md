
#### 2.4.1. Quick Tutorial

The best way how it works is to give examples. So here they are.
In the following we declare components in the header files (.h) and define their processes in the body file (.c). Components should be placed in the [cmp](https://github.com/balazskreith/devclego/tree/master/src/cmp) folder.

Let's define a **mycmps.h** as follows:

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

And the corresponding **mycmps.c** as follows

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

void _cmp_A_process(packet_t *packet) {
  CMP_DEF_THIS(_cmp_A_t, _cmp_A);
  packet->bytes[2] = 2;
  this->send(packet);
}

void _cmp_B_process(packet_t *packet) {
  CMP_DEF_THIS(_cmp_B_t, _cmp_B);
  printf("The second byte in the packet is the following: %d", packet->bytes[2]);
}
```
