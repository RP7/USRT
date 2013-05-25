#ifndef _CALLBACK_H
#define _CALLBACK_H
#include <usrttype.h>


static setCallBackRepeat( task_t * pTask, int delay, int cnt, enum CBMode mode )
{
  pTask->callback = md5first("capCallBackLunchTask");
  memset(&(pTask->callbackargv),0,sizeof(_callback_argv_t));
  pTask->callbackargv.mode = mode;
  pTask->callbackargv.delay = delay;
  pTask->callbackargv.cnt =cnt;
  memcpy( &(pTask->callbackargv.gp), &(pTask->mem), sizeof(generalized_memory_t) ); 
}


#endif //_CALLBACK_H
