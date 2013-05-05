#ifndef USRT_Workers_Keeper_H
#define USRT_Workers_Keeper_H
#include <USRTWorkers.h>

struct WorkerKeeperCTX {
  std::USRTWorkers *workers;
  raw_spinlock_t keeperLock;
  };
  
#endif //USRT_Workers_Keeper_H