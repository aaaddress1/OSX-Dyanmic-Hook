/**
*	OSX Hotpatch PoC By.aaaddress1 
*	DATE: 2015/5/13
**/

#include <dlfcn.h>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include "Hotpatch.h"
void *data, *libHandle, *randAddr;

int fakeRand()
{
	char ask = 0;
	int choise = -1;
Retry:
	puts("Do you want to control the random number? [y/n]");
	scanf("%c" , &ask);

	if (ask == 'n') {
		funcHookOff( randAddr, data);
		return rand();
	}
	else if (ask == 'y') {
		puts("What do you want?");
		scanf("%i", &choise);
	}
	else 
		goto Retry;

	return choise;
}
 
int main(void)
{
    /* Set up hook */
    libHandle = dlopen( "libSystem.B.dylib", RTLD_NOW );
    randAddr = dlsym( libHandle , "rand" );    
    data = malloc(payloadSize);   
    funcHookOn( randAddr, (void*)fakeRand, data );
    
    /* Output the random number */
    puts("!!! The Random Number !!!\nI will give you a number from 0 to 99");
    srand(time(0));
    printf("This is current random number: %i\n", rand() % 100 );

    return 0;
}