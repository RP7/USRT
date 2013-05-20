#include <usrttype.h>
#include <USRTFifo.h>
#include <stdarg.h>
#include <Log.h>

namespace std {
USRTFifo *_globeLog[MAXLogLevel];  
extern "C" {
  void clearLog()
  {
    memset(_globeLog,0,MAXLogLevel*sizeof(USRTFifo *));
  }
  void initLog( int level )
  {
    char fileName[256];
    sprintf(fileName,"log%d",level);
    _globeLog[level]=new USRTFifo();
    _globeLog[level]->newUSRTFifo( fileName
      , (long long)(256*FIFOSIZE)
      , (long long)(256)
      , (long long)sizeof(struct USRTFifo::structFifoHead)
      );
    _globeLog[level]->init();
    _globeLog[level]->start();
  }

  int attachLog( int level )
  {
    char fileName[256];
    sprintf(fileName,"log%d",level);
    _globeLog[level]=new USRTFifo();
    _globeLog[level]->attach( fileName );
    _globeLog[level]->start();
    return 0;
  }  
  
  void* getLog( int level )
  {
    return (void *)_globeLog[level]->get();
  }
  
  void releaseLog(int level)
  {
    delete _globeLog[level];
    _globeLog[level]=NULL;
  }
  void log(int level,const char * format, ...) 
  {
    if( _globeLog[level]==NULL )
      attachLog(level);
    char buffer[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer,1023,format, args);
    va_end (args);
    buffer[1023]='\0';
    int lenLog = strlen(buffer)+1;
    void *mem = _globeLog[level]->allocMem((long long int)lenLog);
    memcpy(mem,buffer,lenLog);
    _globeLog[level]->push(mem);
  }
  void pushOffLog( int level, long long int t )
  {
    _globeLog[level]->pushOff(t);
  }
  void *allocLog( int level, int len )
  {
    if( _globeLog[level]==NULL ) return NULL;
    return _globeLog[level]->allocMem((long long)len);
  }
  long long int L2GLog(int level, void *l)
  {
    if( _globeLog[level]==NULL ) return -1LL;
    return _globeLog[level]->getOff(l);
  } 
  void dumpLog( int level ) 
  {
    if( _globeLog[level]==NULL )
      attachLog(level);
    const char *l;
    while( (l=_globeLog[level]->get())!=NULL )
      fprintf(stderr,"%s",l);  
  }
  void dumpLog2stdout( int level ) 
  {
    if( _globeLog[level]==NULL )
      attachLog(level);
    const char *l;
    while( (l=_globeLog[level]->get())!=NULL )
      fprintf(stdout,"%s",l);  
  }
  long long int keyOfLog( int level )
  {
    if( _globeLog[level]==NULL ) return 0LL;    
    return _globeLog[level]->getKey();
  }
};// extern "C"

};//namespace std

