#include <usrttype.h>
#include <stdlib.h>


void initMem()
{
	globe.mem = malloc(VHEAPSIZE);	
	globe.stack = (task_t **)malloc(STACKSIZE*sizeof(void *));	
}
void *allocMem( int len )
{
	void *ret;
	if( globe.brk+len > VHEAPSIZE ) {
		globe.brk = len;	
		ret=globe.mem;
	}
	else {
		ret = globe.mem+globe.brk;
		globe.brk+=len;
	}
	return ret;
}
