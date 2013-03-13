#include <usrttype.h>
#include <stdlib.h>


void initMem()
{
//	globe.mem = malloc(VHEAPSIZE);	
//	globe.stack = (task_t **)malloc(STACKSIZE*sizeof(void *));
	globe.snE=0;
	globe.snV=0;
	globe.brk=0;
	globe.sp=0;
	globe.bp=0;
	globe.rp=0;	
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
