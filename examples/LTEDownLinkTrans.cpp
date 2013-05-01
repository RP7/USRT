#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <usrttype.h>
#include <assert.h>
#include <MapMem.h>
#include <task.h>
#include <ukey.h>
#include <md5api.h>


#include"LTEDownLinkTransMock.h"

#define UENum 5


int main( int argc, char *argv[] )
{
  int session = newSession();
  initMem("LTEDownLinkTrans");
  start("LTEDownLinkTrans");
  int64 start = newV( "sta",session );
  int subframe = 0;
  int frame = 1;
	sscanf(argv[1],"%d",&frame);
	sscanf(argv[2],"%d",&subframe);
	utime_t valid = getFrameTiming(frame,subframe);
	utime_t noL = valid-getHardware();
	utime_t noE = valid-getSubFrameDuration();
	int64 beforeFFT = newV( "bFF",session );
	void *ar;
	int64 key;
	task_t *task;
	if( subframe==0 ) {
		ar = buildPss();
		task = allocTask( "LTEDownLinkTrans",newE("PsM",session) ); 
		task->key=md5first("capPssMod");
		buildTask(task,start,ar,noE,noL,valid);
		int64 afterPssMod = newV("APM",session);
		setTaskTo( task, afterPssMod );
		pushTask( "LTEDownLinkTrans",task );
		ar = buildPssAntMap();
		task = allocTask( "LTEDownLinkTrans",newE("PAM",session) ); 
		task->key=md5first("capPssMap");
		buildTask(task,afterPssMod,ar,noE,noL,valid);
		setTaskTo( task, beforeFFT );
		pushTask( "LTEDownLinkTrans", task );
	}
	for( int i=0;i<UENum;i++ ) {
		ar = buildUeDownCoding(i);
		task = allocTask( "LTEDownLinkTrans",newE("UeC",session) ); 
		task->key=md5first("capDCHCoding");
		buildTask(task,start,ar,noE,noL,valid);
		int64 afterUeCoding = newV("AUC",session);
		setTaskTo( task, afterUeCoding );
		pushTask( "LTEDownLinkTrans",task );
		ar = buildUeDownMod(i);
		task = allocTask( "LTEDownLinkTrans",newE("UeM",session) ); 
		task->key=md5first("capDCHMod");
		buildTask(task,afterUeCoding,ar,noE,noL,valid);
		int64 afterUeMod = newV("AUM",session);
		setTaskTo( task, afterUeMod );
		pushTask( "LTEDownLinkTrans",task );
		ar = buildUeDownAntMap(i);
		task = allocTask( "LTEDownLinkTrans",newE("UeA",session) ); 
		task->key=md5first("capDCHMap");
		buildTask(task,afterUeMod,ar,noE,noL,valid);
		setTaskTo( task, beforeFFT );
		pushTask( "LTEDownLinkTrans",task );
	}
	printStack("LTEDownLinkTrans");
}
