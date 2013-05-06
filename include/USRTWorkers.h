#ifndef USRT_Workers_H
#define USRT_Workers_H
#include <map>
#include <string>
#include <pthread.h>
#include <capability.h>
#include <CPBuffer.h>
#include <usrttype.h>
#include <md5api.h>
#include <USRTTaskQueue.h>
#include <USRTTask.h>
#include <USRTCapabilityBearer.h>
#include <USRTWorkersKeeper.h>
#include <MapMem.h>

namespace std {
  class USRTWorkers : USRTTask {
    private:
      static int64 keeperKey;
      USRTTaskQueue *tQ;
      map<int64,USRTCapabilityBearer *> caps;
      enum threadState { CREATING, RUNNING, WAITING, KEEPER, EXITING };
      struct structThread {
        pthread_t tid;
        volatile int control;
        volatile enum threadState state;
        int id;
        USRTWorkers *workers;
      } **tids;
      raw_spinlock_t tidsLock;
      int threadNum;
      int control;

      void releaseThread();
      int holdThread();
      int stopThread( int k );
      int runThread( int k );
    public:
      struct WorkerKeeperCTX ctx;
      USRTWorkers( const char* name );
      USRTTaskQueue *tQueue() { return tQ; };
      void defaultKeeper( struct WorkerKeeperCTX *ctx );
      USRTCapabilityBearer *getBearerByKey( int64 key );
      ~USRTWorkers();
      task_t* pop();
      void setCap(const char*);
      void setCap(int64);
      void setQueue( const char *taskQueueName );
      void start( int n );
      static void worker( void *argv );
      static void mainWorker( void *argv );
      void setDefaultKeeper(int64 key);
      void workerExit(){ control=-1; };
      void listCaps();
      void dumpQueue() { if( tQ ) tQ->dumpHeap(); };
      void listThread() {}
  };
}
#endif //USRT_Workers_H

