#ifndef USRT_Workers_Keeper_H
#define USRT_Workers_Keeper_H

namespace std {
class USRTWorkers;
struct WorkerKeeperCTX {
  USRTWorkers *workers;
  raw_spinlock_t keeperLock;
  };
  
struct mainWorkerCTX {
  USRTWorkers *workers;
  void *argv;
  };
}  
#endif //USRT_Workers_Keeper_H