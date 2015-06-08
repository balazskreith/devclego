#include <stdio.h>
#include "devclego.h"
#include "examples.h"

int main()
{
	//initialize devlego
	devlego_init(NULL, NULL);

	example1_main();

	//deinitialize devlego
	devlego_deinit();
	return 0;
}
