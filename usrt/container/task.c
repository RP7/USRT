#include <usrttype.h>
#include <ukey.h>
#include <usrttime.h>
#include <usrtmem.h>

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
void releasSp()
{
	globe.sp = globe.bp;
}
void push( task_t * task )
{
	globe.stack[globe.bp]=task;
	globe.bp++;
	if( globe.bp>=STACKSIZE )
		globe.bp = 0;
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
	printf("\"%s(%d)\"->\"%s(%d)\";\n",nameOfKey(&(task->from))
		,snOfKey(&(task->from))
		,nameOfKey(&(task->to))
		,snOfKey(&(task->to))
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

