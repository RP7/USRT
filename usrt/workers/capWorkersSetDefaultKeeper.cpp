#define FUNCLASS capWorkersSetDefaultKeeper
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
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  int64 key= md5first("capWorkersKeeper");
  ctx->workers->setDefaultKeeper( key );
  return 0;
}



