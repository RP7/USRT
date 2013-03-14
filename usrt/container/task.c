#include <usrttype.h>
#include <ukey.h>
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
  __raw_spin_lock(&(globe.lock));
	globe.bp=globe.sp;	
	__raw_spin_unlock(&(globe.lock));	
}
void releaseSp()
{
  __raw_spin_lock(&(globe.lock));
	globe.sp = globe.bp;
	__raw_spin_unlock(&(globe.lock));	
}
void pushTask( task_t * task )
{
  __raw_spin_lock(&(globe.lock));
	globe.stack[globe.bp]=task;
	globe.bp++;
	if( globe.bp>=STACKSIZE )
		globe.bp = 0;
	__raw_spin_unlock(&(globe.lock));	
}

int getTask( task_t* &ret )
{
	ret = NULL;
	int r=-1;
  __raw_spin_lock(&(globe.lock));
	if( globe.rp!=globe.sp )
	{
		ret = globe.stack[globe.rp];
		globe.rp++;
		if( globe.rp>=STACKSIZE )
			globe.rp = 0;
		r=0;
	}
	__raw_spin_unlock(&(globe.lock));	
	return r;
}

int64* getTaskKey( task_t *task )
{
	return task->key;
}

void buildTask( task_t *task, int64 start,void *ar, double noE,double noL,double valid)
{
	task->from = start;
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

