#include<usrttype.h>
short newSession();
task_t *allocTask(int64 ID);
void holdSp();
void releasSp();
void push( task_t * task );
void *buildTask( task_t *task, int64 start,int64 key,void *ar, double noE,double noL,double valid);
void setTaskTo( task_t *task, int64 to );
void dump( task_t *task ); 
void printStack();
