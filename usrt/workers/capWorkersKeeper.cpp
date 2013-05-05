#define FUNCLASS capWorkersKeeper
#include <capabilityAPI.h>
#include <stdio.h>
#include <usrttype.h>
#include <USRTWorkersKeeper.h>
#include <USRTWorkers.h>
#include <MapMem.h>
#include <sys/types.h>
#include <linux/unistd.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct WorkerKeeperCTX *ctx = (struct WorkerKeeperCTX *)argv;
  if( !ctx->workers ) {    
	  printf("you are into Worker Keeper\n");
	  printf("thread %ld can not get worker\n",(long int)syscall(224));
	  return;
	}
  if( ctx->workers->tQueue() == NULL ) {    
	  printf("you are into Worker Keeper\n");
	  printf("thread %ld can not get Queue\n",(long int)syscall(224));
	  return;
	}
  if( ctx->keeperLock.slock ) {
    __raw_spin_lock(&(ctx->keeperLock));
    while( ctx->workers->tQueue()->len()>0 ) {
      generalized_memory_t *gpTask = (generalized_memory_t *)ctx->workers->tQueue()->get();
      if( gpTask != NULL ) {
        int put = ctx->workers->tQueue()->insert( gpTask );
        if( put == -1 ) {
          fprintf(stderr,"Wait Heap is full\n");
          ctx->workers->tQueue()->push((void *)gpTask);
        }
      }
    }
    ctx->workers->tQueue()->update();
    __raw_spin_unlock(&(ctx->keeperLock));    
  }
  else {    
	  printf("you are into Worker Keeper\n");
	  printf("thread %ld can not get keeper lock\n",(long int)syscall(224));
	}
	return 1;
}



