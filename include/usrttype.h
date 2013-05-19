#ifndef __USRTTYPE_H
#define __USRTTYPE_H
typedef long long int64;
typedef long long utime_t;

#undef CheckThreadLock

typedef struct {
  unsigned int slock;
#ifdef CheckThreadLock
  long int tid;
#endif
} raw_spinlock_t;

#ifdef CheckThreadLock
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#endif

static inline void dumpLock( raw_spinlock_t* lock )
{
#ifdef CheckThreadLock
  int slock = lock->slock;
  long int tid = lock->tid;
  fprintf(stderr,"lock = %d, tid = %ld\n",slock,tid);  
#else
  
#endif
} 

typedef struct structGeneralizedMemory {
  int64 memKey;
  int64 offset;
  int64 len;
  int64 check;
} generalized_memory_t;

typedef struct structTask {
  int64 ID;
  int64 key;
  int64 from;
  int64 to;
  generalized_memory_t argv;
  int64 callback;
  utime_t noE;
  utime_t noL;
  utime_t valid;
  raw_spinlock_t lock;
  int state;
  int ref;
  int version;
  generalized_memory_t mem;
  int64 md5[2];
} task_t;
  
typedef struct structKey {
  char name[4];
  short sn;
  short session;
} ukey_t;

#define FIFOSIZE (256*256)
#define METASIZE 256

struct structGlobe {
  short snE;
  short snV;
};

extern struct structGlobe globe;

#ifdef CONFIG_SMP
#define LOCK_PREFIX \
    ".section .smp_locks,\"a\"\n"  \
    _ASM_ALIGN "\n"      \
    _ASM_PTR "661f\n" /* address */  \
    ".previous\n"      \
    "661:\n\tlock; "

#else /* ! CONFIG_SMP */
#define LOCK_PREFIX ""
#endif
static inline void __raw_spin_lock(raw_spinlock_t *lock) {
   asm("\n"
       "1:\t" "lock;" "decl %0\n\t"
       "jne 2f\n\t"
       ".subsection 1\n\t"
       ".align 16\n"
       "2:\trep; nop\n\t"
       "cmpl $0, %0\n\t"
       "jg 1b\n\t"
       "jmp 2b\n\t"
       ".previous"
       : "=m" (lock->slock)
       : "m" (lock->slock)
        );
#ifdef CheckThreadLock
  lock->tid=(long int)syscall(__NR_gettid);
#endif
}        
/*
static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
  asm volatile("\n1:\t"
         LOCK_PREFIX " ; decl %0\n\t"
         "jns 3f\n"
         "2:\t"
         "rep;nop\n\t"
         "cmpl $0,%0\n\t"
         "jle 2b\n\t"
         "jmp 1b\n"
         "3:\n\t"
         : "+m" (lock->slock) : : "memory");
#ifdef CheckThreadLock
  lock->tid=(long int)syscall(__NR_gettid);
#endif
}
*/
static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
#ifdef CheckThreadLock
  lock->tid=-1LL;
#endif
  asm volatile("movl $1,%0" : "+m" (lock->slock) :: "memory");
}


typedef struct { unsigned long t[2]; } timing;
#define timing_now(x) asm volatile(".byte 15;.byte 49" : "=a"((x)->t[0]),"=d"((x)->t[1]))
static inline long long int __getNow()
{
  timing now;
  timing_now(&now);
  return (long long int)now.t[0]+4294967296LL*(long long int)now.t[1];
}

#endif

