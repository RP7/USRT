extern "C" {
    
  void initMem( const char *n );
  int attach( const char *n );
  void start( const char *n );
  void pushTask( const char *n, task_t* t );
  void *allocMem( const char *n, long long len );
  task_t *allocTask(const char *n,int64 ID);
  int len( const char *n );
  int getTask( const char *n, task_t* &ret );
  void release(const char *n);
  void printStack(const char *n);
  void dumpHead(const char *n);
  void *G2L( generalized_memory_t *gp );
  void L2G( generalized_memory_t* g, void* lp );
  void dumpGM( generalized_memory_t& g );
};