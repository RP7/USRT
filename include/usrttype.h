typedef long long int64;
typedef long long time_t;
typedef struct structTask {
	int64 ID;
	int64 key;
	int64 from;
	int64 to;
	time_t noE;
	time_t noL;
	time_t valid;
	void *argv;
	int lock;
} task_t;
	
typedef struct structKey {
	char name[4];
	short sn;
	short session;
} ukey_t;

struct structGlobe {
	short snE;
	short snV;
	int brk;
	int sp;
	int bp;
	void *mem;
	task_t **stack;
};
