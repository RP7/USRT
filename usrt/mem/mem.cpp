#include <map>
#include <string>
#include <CPBuffer.h>
#include "mem.h"
#include <task.h>

void USRTmem::_printStack( int bp )
{
  if( bp>=STACKSIZE )
    printf("Error too big bp\n");
  printf("digraph G {\n");
  int i;
  if( bp<head->sp ) 
    for( i=bp;i<head->sp;i++ )
      dump( (task_t*)buf->getBuf(head->stack[i],0LL) );
  else {
    for( i=head->sp;i<STACKSIZE;i++ )
      dump( (task_t*)buf->getBuf(head->stack[i],0LL) );
    for( i=0;i<bp;i++ )
      dump( (task_t*)buf->getBuf(head->stack[i],0LL) );
  }
  printf("}\n");
}

void USRTmem::printStack( int len )
{
  int bp = (head->sp-len+STACKSIZE)%STACKSIZE;
  _printStack(bp);
}

void USRTmem::printStack( )
{
  _printStack(head->rp);
}
void USRTmem::dumpHead()
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

USRTmem::USRTmem()
{
  buf=NULL;
}

void USRTmem::newUSRTmem( const char *n )
{
  buf = new CPBuffer( (long long)(sizeof(task_t)*STACKSIZE)
    , (long long)(sizeof(task_t))
    , (long long)sizeof(struct structTaskMemHead)
    , n
    );
  head = (struct structTaskMemHead*)buf->attach();
}

void USRTmem::attach( const char *n )
{
  buf = new CPBuffer( n );
  head = (struct structTaskMemHead*)buf->attach();
}

void USRTmem::newUSRTmem( const char *n, long long dataL, long long cpL, long long resL )
{
  buf = new CPBuffer( dataL
    , cpL
    , resL
    , n
    );
  head = (struct structTaskMemHead*)buf->attach();
}
void USRTmem::init()
{
  memset( ((unsigned char*)head)+sizeof(struct structCPBMeta)
    , 0
    , sizeof(struct structTaskMemHead)-sizeof(struct structCPBMeta)
    );
}

char* USRTmem::getName()
{
  return head->meta.name;
}

USRTmem::~USRTmem()
{ 
  if( buf )
    delete buf;
  buf=NULL;
}

int USRTmem::len()
{ 
  return (head->sp-head->rp+STACKSIZE)%STACKSIZE;
}

void USRTmem::start()
{ 
  __raw_spin_unlock(&(head->lockS));
  __raw_spin_unlock(&(head->lockM));
}

void *USRTmem::allocMem( long long len )
{
  __raw_spin_lock(&(head->lockM));
  long long last = head->_brk;
  head->_brk+=len;
  __raw_spin_unlock(&(head->lockM));
  if( buf )
    return buf->getBuf(last,len);
  else
    return NULL;
}

void USRTmem::pushTask( task_t* task )
{
  __raw_spin_lock(&(head->lockS));
  head->stack[head->sp]=buf->getOff((void *)task);
  head->sp++;
  if( head->sp>=STACKSIZE )
    head->sp = 0;
  __raw_spin_unlock(&(head->lockS));
}

int USRTmem::getTask( task_t* &ret )
{
  ret = NULL;
  int r=-1;
  __raw_spin_lock(&(head->lockS));
  if( head->rp!=head->sp ) {
    ret = (task_t*)buf->getBuf(head->stack[head->rp],(long long)sizeof(task_t));
    head->rp++;
    if( head->rp>=STACKSIZE )
      head->rp = 0;
    r=0;
  }
  __raw_spin_unlock(&(head->lockS));
  return r;
}

