#include <map>
#include <string>
#include <USRTFifo.h>
#include <USRTTask.h>
#include <task.h>

void USRTTask::_printStack( int bp )
{
  if( bp>=FIFOSIZE )
    printf("Error too big bp\n");
  printf("digraph G {\n");
  int i;
  if( bp<getHead()->sp ) 
    for( i=bp;i<getHead()->sp;i++ )
      dump( (task_t*)getBuf(getHead()->fifo[i],0LL) );
  else {
    for( i=getHead()->sp;i<FIFOSIZE;i++ )
      dump( (task_t*)getBuf(getHead()->fifo[i],0LL) );
    for( i=0;i<bp;i++ )
      dump( (task_t*)getBuf(getHead()->fifo[i],0LL) );
  }
  printf("}\n");
}

void USRTTask::dump( task_t *task ) {
  if( task != NULL ) {
    printf("\"%s(%d)\"->\"%s(%d)\" [label=\"%s(%d)\"];\n"
      ,nameOfKey(&(task->from))
      ,snOfKey(&(task->from))
      ,nameOfKey(&(task->to))
      ,snOfKey(&(task->to))
      ,nameOfKey(&(task->ID))
      ,snOfKey(&(task->ID))
      );
  }
  else
    printf("NULL task\n");
}
char *USRTTask::nameOfKey( int64 *key )
{
	ukey_t *pk=(ukey_t*)key;
	return (char *)(void *)&(pk->name); 
}
int USRTTask::snOfKey( int64 *key )
{
	ukey_t *pk=(ukey_t*)key;
	return (int)(pk->sn); 
}
int64 USRTTask::key2int( ukey_t *key )
{
	return *(int64 *)(key);
}
void USRTTask::printStack( int len )
{
  int bp = (getHead()->sp-len+FIFOSIZE)%FIFOSIZE;
  _printStack(bp);
}

void USRTTask::printStack( )
{
  _printStack(getHead()->rp);
}

USRTTask::USRTTask()
{
}

void USRTTask::newUSRTTask( const char *n )
{
  newUSRTFifo( n
    , (long long)(sizeof(task_t)*FIFOSIZE)
    , (long long)(sizeof(task_t))
    , (long long)sizeof(struct structFifoHead)
    );
}

void USRTTask::newUSRTTask( const char *n, long long dataL, long long cpL, long long resL )
{
  newUSRTFifo( n
    , dataL
    , cpL
    , resL
    );
}

USRTTask::~USRTTask()
{ 
}

void USRTTask::pushTask( task_t* task )
{
  push((void *)task);
}

int USRTTask::getTask( task_t* &ret )
{
  ret = (task_t*)get();
  int r=-1;
  if( ret!=NULL )
    r=0;
  return r;
}
