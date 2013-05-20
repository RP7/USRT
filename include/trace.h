#ifndef _TRACE_H
#define _TRACE_H
#include <usrttype.h>
#include <Log.h>
#include <memory.h>
#include <stdio.h>
#include <MapMem.h>

#define TRACELOG 15
typedef struct __trace {
  int64 off;
  utime_t delay;
  utime_t lunch;
  utime_t pop;
  utime_t wait;
  utime_t ready;
  utime_t end;
  long int tid;
  long int keeper;
  long int update;
} _trace_t;

static _trace_t *newTrace()
{
  _trace_t *aLog =(_trace_t *)allocLog(TRACELOG,sizeof(_trace_t));
  memset(aLog,0,sizeof(_trace_t));
  aLog->off=L2GLog(TRACELOG,(void*)aLog);
  return aLog; 
}

static void *allocWithTrace(long long int len)
{
  _trace_t *aLog = (_trace_t *)allocLog(TRACELOG,len);
  memset(aLog,0,len);
  aLog->off=L2GLog(TRACELOG,(void*)aLog);
  return (void *)aLog; 

}

static void dumpTrace( FILE *fp, _trace_t *t )
{
  fprintf(fp,"%24lld | ",t->end);
  fprintf(fp,"d(%ld):%+10lld ",t->tid,t->end-t->lunch-t->delay);
  fprintf(fp,"k(%ld):%+10lld ",t->keeper,t->pop-t->lunch);
  fprintf(fp,"u(%ld):(w:%+10lld r:%+10lld) "
    , t->update
    , t->wait-t->lunch-t->delay
    , t->ready-t->lunch-t->delay
  );
  fprintf(fp,"\n");
}

static _trace_t *formGpTask(generalized_memory_t *task)
{
  task_t *t = (task_t*)G2L(task);
  return (_trace_t *)G2L(&(t->argv));
}


static long long int keyOfTrace()
{
  return keyOfLog(TRACELOG);
}
#endif //_TRACE_H
