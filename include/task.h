#include<usrttype.h>
extern "C" {
  short newSession();
  void *buildTask( task_t *task, int64 start,void *ar, double noE,double noL,double valid);
  int64* getTaskKey( task_t *task);
  void setTaskTo( task_t *task, int64 to );
  void dump( task_t *task ); 
};