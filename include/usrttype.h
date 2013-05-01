#ifndef __USRTTYPE_H
#define __USRTTYPE_H
typedef long long int64;
typedef long long utime_t;

typedef struct {
  unsigned int slock;
} raw_spinlock_t;

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

#define STACKSIZE 256


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

static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
  asm volatile("\n1:\t"
         LOCK_PREFIX " ; decb %0\n\t"
         "jns 3f\n"
         "2:\t"
         "rep;nop\n\t"
         "cmpb $0,%0\n\t"
         "jle 2b\n\t"
         "jmp 1b\n"
         "3:\n\t"
         : "+m" (lock->slock) : : "memory");
}

static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
  asm volatile("movb $1,%0" : "+m" (lock->slock) :: "memory");
}

#endif