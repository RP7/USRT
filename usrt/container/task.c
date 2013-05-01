#include <usrttype.h>
#include <ukey.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
short newSession()
{
  return (short)(rand()&0xffff);
}

void buildTask( task_t *task, int64 start, int64 ar, utime_t noE, utime_t noL, utime_t valid)
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

int64 getTaskKey(task_t *task) {
  return task->key;
}

};