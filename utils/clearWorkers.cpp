#include <workers.h>
int main(int argc, char *argv[])
{
  std::workers *tut = new std::workers();
  tut->attach(argv[1]);
  tut->dumpHead();
  tut->restoreHeap();
  tut->start();
  
  task_t *p;
  int cnt=0;

  while( (p=tut->pop(tut->wait)) != NULL ) {
    printf( "wait %d: ",cnt );
    tut->dumpTaskTime( p );
    cnt++;
  }
  cnt=0;
  while( (p=tut->pop(tut->ready)) != NULL ) {
    printf( "ready %d: ",cnt );
    tut->dumpTaskTime( p );
    cnt++;
  }

}