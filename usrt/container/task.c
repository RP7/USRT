#include <usrttype.h>
#include <ukey.h>
#include <usrttime.h>
#include <usrtmem.h>
#include <stdio.h>
#include <stdlib.h>

short newSession()
{
	return (short)(rand()&0xffff);
}
task_t *allocTask(int64 ID)
{
	task_t *task = (task_t*)allocMem( sizeof(task_t) );
	task->ID = ID;
	task->lock = 0;
	return task;	
}
void holdSp()
{
	globe.bp=globe.sp;	
}
void releaseSp()
{
	globe.sp = globe.bp;
}
void pushTask( task_t * task )
{
	globe.stack[globe.bp]=task;
	globe.bp++;
	if( globe.bp>=STACKSIZE )
		globe.bp = 0;
}

int getTask( task_t* &ret )
{
	ret = NULL;
	if( globe.rp!=globe.sp )
	{
		ret = globe.stack[globe.rp];
		printf("%d, %p\n",globe.rp,ret);
		globe.rp++;
		if( globe.rp>=STACKSIZE )
			globe.rp = 0;
		return 0;
	}
	else
		return -1;
}

void *buildTask( task_t *task, int64 start,int64 key,void *ar, double noE,double noL,double valid)
{
	task->from = start;
	task->key	= key;
	task->argv = ar;
	task->noE = noE;
	task->noL = noL;
	task->valid = valid;
}
void setTaskTo( task_t *task, int64 to )
{
	task->to = to;	
}
void dump( task_t *task ) {
	printf("\"%s(%d)\"->\"%s(%d)\" [label=\"%s(%d)\"];\n"
		,nameOfKey(&(task->from))
		,snOfKey(&(task->from))
		,nameOfKey(&(task->to))
		,snOfKey(&(task->to))
		,nameOfKey(&(task->ID))
		,snOfKey(&(task->ID))
		);
}
void printStack()
{
	printf("digraph G {\n");
	int i;
	if( globe.bp>globe.sp ) 
		for( i=globe.sp;i<globe.bp;i++ )
			dump( globe.stack[i] );
	else {
		for( i=globe.sp;i<STACKSIZE;i++ )
			dump( globe.stack[i] );
		for( i=0;i<globe.bp;i++ )
			dump( globe.stack[i] );
	}
	printf("}\n");
}

