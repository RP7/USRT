#ifndef USRT_Workers_H
#define USRT_Workers_H
#include <map>
#include <string>
#include <capability.h>
#include <CPBuffer.h>
#include <usrttype.h>
#include <USRTTaskQueue.h>
#include <USRTCapabilityBearer.h>
#include <USRTWorkersKeeper.h>

namespace std {
  class USRTWorkers {
    private:
      USRTTaskQueue *tQ;
      map<int64,USRTCapabilityBearer *> caps;
    public:
      USRTWorkers( const char* name );
      USRTTaskQueue *tQueue() { return tQ; };
      void defaultKeeper( struct WorkerKeeperCTX *ctx );
      void start( int n );
      ~USRTWorkers(){};
      task_t* pop();
  };
}
#endif //USRT_Workers_H

