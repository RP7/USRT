#ifndef _LOG_H
#define _LOG_H
extern "C" {
  #define MAXLogLevel 16
  enum LogLevel { EMERG=0, ALERT, CRIT, ERROR, WARNING, NOTICE, INFO, DEBUG };     
  void initLog( int level );
  int attachLog( int level );
  void* getLog( int level );
  void releaseLog(int level);
  void log(int level,const char * format, ...); 
  void dumpLog( int level ); 
  void clearLog();
  void dumpLog2stdout( int level );
  void pushOffLog( int level, long long int t );
  void *allocLog( int level, int len );
  long long int L2GLog(int level, void *l);
  long long int keyOfLog( int level );
};
#endif // _LOG_H

