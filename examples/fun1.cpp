#include<capability.h>
#include<stdio.h>
#include<string.h>
#include "fun1.h"
#include <stdlib.h>
#include <sys/time.h>
#include <usrttype.h>
#include <assert.h>
#include <usrtmem.h>
#include <task.h>
#include <ukey.h>


#include"LTEDownLinkTransMock.h"

#define UENum 5

extern "C" {
	CCapability *newFun() {
		exampleFun1 *item = new exampleFun1();
		return (CCapability*)item;
	}
	void run( CCapability* item, void *argv ) {
		(exampleFun1 *)item->run( argv );
	}
	void getKey( CCapability* item,long long int *k){
		(exampleFun1 *)item->getKey( k );
	}
	void destroy( CCapability* item ) {
		(exampleFun1 *)item->destroy();
	}
	long long int md2_5() {
		return 0x1318bb259d65ee19LL;
	}
};	


long long int exampleFun1::key[] = {md2_5(),0x7e95373c35421368LL};
int exampleFun1::getKey( long long int *k ) {
	k[0]=key[0];
	k[1]=key[1];
	return 1;
}

int exampleFun1::run( void *argv ) {
  int session = newSession();
  int64 start = newV( "sta",session );
  int subframe = 0;
  int frame = 1;
  int *iargv = (int *)argv;
	frame=iargv[1];
	subframe=iargv[2];
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
	releaseSp();
	return 1;
}

int exampleFun1::destroy( ) {
	delete this;
	return 1;
}


