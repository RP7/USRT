#include <usrttype.h>
#include <ukey.h>
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
  if( task_t != NULL ) {
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
