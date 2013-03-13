#ifndef __USRTTYPE_H
#define __USRTTYPE_H
typedef long long int64;
typedef long long utime_t;
typedef struct structTask {
	int64 ID;
	int64 key;
	int64 from;
	int64 to;
	utime_t noE;
	utime_t noL;
	utime_t valid;
	void *argv;
	int lock;
} task_t;
	
typedef struct structKey {
	char name[4];
	short sn;
	short session;
} ukey_t;

#define VHEAPSIZE 65536
#define STACKSIZE 256

struct structGlobe {
	short snE;
	short snV;
	int brk;
	int sp;
	int bp;
	int rp;
	char mem[VHEAPSIZE];
	task_t *stack[STACKSIZE];
};

extern struct structGlobe globe;

#endif