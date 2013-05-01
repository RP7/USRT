#include <map>
#include <string>
#include <CPBuffer.h>
#include <USRTTask.h>
#include <task.h>

void USRTTask::_printStack( int bp )
{
  if( bp>=STACKSIZE )
    printf("Error too big bp\n");
  printf("digraph G {\n");
  int i;
  if( bp<head->sp ) 
    for( i=bp;i<head->sp;i++ )
      dump( (task_t*)getBuf(head->stack[i],0LL) );
  else {
    for( i=head->sp;i<STACKSIZE;i++ )
      dump( (task_t*)getBuf(head->stack[i],0LL) );
    for( i=0;i<bp;i++ )
      dump( (task_t*)getBuf(head->stack[i],0LL) );
  }
  printf("}\n");
}

void USRTTask::printStack( int len )
{
  int bp = (head->sp-len+STACKSIZE)%STACKSIZE;
  _printStack(bp);
}

void USRTTask::printStack( )
{
  _printStack(head->rp);
}
void USRTTask::dumpHead()
{
  printf("mem file name: %s\n",head->meta.name);
  printf("mem brk: %lld\n",head->_brk);
  printf("write point: %d\n",head->sp);
  printf("read point: %d\n",head->rp);    
  if( head->lockM.slock==1 )
    printf("Mem lock is unlock\n");
  else
    printf("Mem lock is lock\n");
  if( head->lockS.slock==1 )
    printf("stack lock is unlock\n");
  else
    printf("stack lock is lock\n");
  int bp=head->rp;
  int cnt=0;
  while( bp != head->sp ) {
    printf("%4.4llx ",head->stack[bp]&0xffff);
    bp++;
    if( bp>=STACKSIZE )
      bp=0;
    cnt++;
    if( (cnt&0x7)==0 )
      printf("\n");
  }
  printf("\n");
}

USRTTask::USRTTask()
{
}

void USRTTask::newUSRTTask( const char *n )
{
  newCPBuffer( (long long)(sizeof(task_t)*STACKSIZE)
    , (long long)(sizeof(task_t))
    , (long long)sizeof(struct structTaskMemHead)
    , n
    );
  head = (struct structTaskMemHead*)CPBuffer::attach();
}

void USRTTask::attach( const char *n )
{
  newCPBuffer( n );
  head = (struct structTaskMemHead*)CPBuffer::attach();
}

void USRTTask::newUSRTTask( const char *n, long long dataL, long long cpL, long long resL )
{
  newCPBuffer( dataL
    , cpL
    , resL
    , n
    );
  head = (struct structTaskMemHead*)CPBuffer::attach();
}
void USRTTask::init()
{
  memset( ((unsigned char*)head)+sizeof(struct structCPBMeta)
    , 0
    , sizeof(struct structTaskMemHead)-sizeof(struct structCPBMeta)
    );
}

char* USRTTask::getName()
{
  return head->meta.name;
}

USRTTask::~USRTTask()
{ 
}

int USRTTask::len()
{ 
  return (head->sp-head->rp+STACKSIZE)%STACKSIZE;
}

void USRTTask::start()
{ 
  __raw_spin_unlock(&(head->lockS));
  __raw_spin_unlock(&(head->lockM));
}

void *USRTTask::allocMem( long long len )
{
  __raw_spin_lock(&(head->lockM));
  long long last = head->_brk;
  head->_brk+=len;
  __raw_spin_unlock(&(head->lockM));
  return getBuf(last,len);
}

void USRTTask::pushTask( task_t* task )
{
  __raw_spin_lock(&(head->lockS));
  head->stack[head->sp]=getOff((void *)task);
  head->sp++;
  if( head->sp>=STACKSIZE )
    head->sp = 0;
  __raw_spin_unlock(&(head->lockS));
}

int USRTTask::getTask( task_t* &ret )
{
  ret = NULL;
  int r=-1;
  __raw_spin_lock(&(head->lockS));
  if( head->rp!=head->sp ) {
    ret = (task_t*)getBuf(head->stack[head->rp],(long long)sizeof(task_t));
    head->rp++;
    if( head->rp>=STACKSIZE )
      head->rp = 0;
    r=0;
  }
  __raw_spin_unlock(&(head->lockS));
  return r;
}

int64 USRTTask::getKey()
{
  return head->meta.key[0];
}
