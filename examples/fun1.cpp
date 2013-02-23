#include<capability.h>
#include<stdio.h>
#include "fun1.h"

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
};	


long long int exampleFun1::key[] = {0x1318bb259d65ee14LL,0x7e95373c35421368LL};
int exampleFun1::getKey( long long int *k ) {
	k[0]=key[0];
	k[1]=key[1];
	return 1;
}

int exampleFun1::run( void *argv ) {
	printf("I am 1\n");
	return 1;
}

