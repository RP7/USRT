#include <usrttype.h>
#include <task.h>
#include<stdio.h>

extern "C" {
	int readTask( task_t * &t ) {
		task_t *ret;
		int a = getTask(ret);
		t = ret;
		return a;
	}
	int dumpStack() {
		printStack();
		return 0;
	}
}

