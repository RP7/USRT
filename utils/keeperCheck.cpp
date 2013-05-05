#include <stdio.h>
#include <memory.h>
#include <USRTCapabilityBearer.h>
#include <usrttype.h>
#include <MapMem.h>
#include <md5api.h>
#include <USRTWorkersKeeper.h>

using namespace std;
int main(int argc, char *argv[])
{
  long long int key=md5first("capWorkersKeeper");
  USRTCapabilityBearer *tut = new USRTCapabilityBearer(key);
  if( tut->isValid() ) {
    printf("libs[%llx]=%s\n",key,tut->getName());
    initMem("task0");
    start("task0");
    struct WorkerKeeperCTX *ctx = (struct WorkerKeeperCTX *)allocMem("task0",sizeof(struct WorkerKeeperCTX));
    memset(ctx,0,sizeof(struct WorkerKeeperCTX));
    generalized_memory_t g;
    L2G(&g,(void *)ctx);
    tut->runGP(&g);
  }
  delete tut;
}