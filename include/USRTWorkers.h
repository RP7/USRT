#ifndef USRT_Workers_H
#define USRT_Workers_H
#include <map>
#include <string>
#include <capability.h>
#include <CPBuffer.h>
#include <usrttype.h>
#include <USRTTaskQueue.h>
#include <USRTCapabilityBearer.h>
#include <USERWorkersKeeper.h>

namespace std {
  class USERWorkers {
    private:
      USRTTaskQueue *tQ;
      map<int64,USRTCapabilityBearer *> caps;
    public:
      USERWorkers( const char* name );
      void defaultKeeper( struct WorkerKeeperCTX *ctx );
      void start( int n );
      ~USERWorkers(){};
      task_t* pop( );
  };
}
#endif //USRT_Workers_H

