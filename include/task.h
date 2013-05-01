#include<usrttype.h>
extern "C" {
  short newSession();
  void *buildTask( task_t *task, int64 start,int64 ar, utime_t noE, utime_t noL, utime_t valid);
  int64* getTaskKey( task_t *task);
  void setTaskTo( task_t *task, int64 to );
};