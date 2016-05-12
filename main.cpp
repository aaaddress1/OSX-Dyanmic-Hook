/**
*	OSX Hotpatch PoC By.aaaddress1 
*	DATE: 2015/5/12
**/

#include <dlfcn.h>
#include <stdio.h>
#include "Hotpatch.h"

void fakePuts(char* p)
{
    printf("%s aaaddress1.\n", p);
}
 
int main(void)
{
    void *libHandle = dlopen( "libSystem.B.dylib", RTLD_NOW );
    void *putsAddr = dlsym( libHandle , "puts" );
    if (!funcInject( putsAddr, (void*)fakePuts )) {
    	puts("memory patch fail.");
    	return 0;
    }
    puts("Hello");
    return 0;
}