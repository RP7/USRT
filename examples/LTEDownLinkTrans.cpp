#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <usrttype.h>
#include <assert.h>
#include <usrttime.h>
#include <usrtmem.h>
#include <task.h>
#include <ukey.h>


#include"LTEDownLinkTransMock.h"

#define UENum 5


int main( int argc, char *argv[] )
{
  int session = newSession();
  initMem();
  int64 start = newV( "sta",session );
  int subframe = 0;
  int frame = 1;
	sscanf(argv[1],"%d",&frame);
	sscanf(argv[2],"%d",&subframe);
	holdSp();
	utime_t valid = getFrameTiming(frame,subframe);
	utime_t noL = valid-getHardware();
	utime_t noE = valid-getSubFrameDuration();
	int64 beforeFFT = newV( "bFF",session );
	void *ar;
	int64 key;
	task_t *task;
	if( subframe==0 ) {
		ar = buildPss();
		key = keyPss();
		task = allocTask( newE("PsM",session) ); 
		buildTask(task,start,key,ar,noE,noL,valid);
		int64 afterPssMod = newV("APM",session);
		setTaskTo( task, afterPssMod );
		pushTask( task );
		ar = buildPssAntMap();
		key = keyPssAntMap();
		task = allocTask( newE("PAM",session) ); 
		buildTask(task,afterPssMod,key,ar,noE,noL,valid);
		setTaskTo( task, beforeFFT );
		pushTask( task );
	}
	for( int i=0;i<UENum;i++ ) {
		ar = buildUeDownCoding(i);
		key = keyUeDownCoding();
		task = allocTask( newE("UeC",session) ); 
		buildTask(task,start,key,ar,noE,noL,valid);
		int64 afterUeCoding = newV("AUC",session);
		setTaskTo( task, afterUeCoding );
		pushTask( task );
		ar = buildUeDownMod(i);
		key = keyUeDownMod();
		task = allocTask( newE("UeM",session) ); 
		buildTask(task,afterUeCoding,key,ar,noE,noL,valid);
		int64 afterUeMod = newV("AUM",session);
		setTaskTo( task, afterUeMod );
		pushTask( task );
		ar = buildUeDownAntMap(i);
		key = keyUeDownAntMap();
		task = allocTask( newE("UeA",session) ); 
		buildTask(task,afterUeMod,key,ar,noE,noL,valid);
		setTaskTo( task, beforeFFT );
		pushTask( task );
	}
	printStack();
}
