#include<capability.h>
#include<stdio.h>
#include "fun2.h"

extern "C" {
	CCapability *newFun() {
		exampleFun2 *item = new exampleFun2();
		return (CCapability*)item;
	}
	void run( CCapability* item, void *argv ) {
		(exampleFun2 *)item->run( argv );
	}
	void getKey( CCapability* item,long long int *k){
		(exampleFun2 *)item->getKey( k );
	}
};	


long long int exampleFun2::key[] = {0x1318bb259d65ee15LL,0x7e95373c35421368LL};
int exampleFun2::getKey( long long int *k ) {
	k[0]=key[0];
	k[1]=key[1];
	return 1;
}

int exampleFun2::run( void *argv ) {
	printf("I am 2\n");
	return 1;
}

