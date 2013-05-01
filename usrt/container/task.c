#include <usrttype.h>
#include <ukey.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
short newSession()
{
  return (short)(rand()&0xffff);
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

int64 *getTaskKey(task_t *task) {
  return task->key;
}

};