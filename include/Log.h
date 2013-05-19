extern "C" {
  #define MAXLogLevel 16
  enum LogLevel { EMERG=0, ALERT, CRIT, ERROR, WARNING, NOTICE, INFO, DEBUG };     
  void initLog( int level );
  int attachLog( int level );
  const char* getLog( int level );
  void releaseLog(int level);
  void log(int level,const char * format, ...); 
  void dumpLog( int level ); 
  void clearLog();
  void dumpLog2stdout( int level );
};
