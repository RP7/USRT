#define FUNCLASS capWorkersKeeper
#include <capabilityAPI.h>
#include <stdio.h>
#include <usrttype.h>
#include <USRTWorkerKeeper.h>

int FUNCLASS::run( void *argv ) {

  task_t *t = (task_t*)argv;
    
	printf("you are into Worker Keeper\n");
	printf("task key : %llx\n",t->ID);
	return 1;
}



