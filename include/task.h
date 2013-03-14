#include<usrttype.h>
short newSession();
task_t *allocTask(int64 ID);
void holdSp();
void releaseSp();
void pushTask( task_t * task );
void *buildTask( task_t *task, int64 start,void *ar, double noE,double noL,double valid);
int64* getTaskKey( task_t *task);
void setTaskTo( task_t *task, int64 to );
void dump( task_t *task ); 
void printStack();
int getTask( task_t* &ret );
