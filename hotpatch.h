/**
*   Title:  OSX Function Inject (Dynamic Hotpatch)
*   Author: aaaddress1@gmail.com
*   DATE:   2016/5/12
*   special thanks to aaronluo17@gmail.com, inndy.tw@gmail.com
**/
#include <dlfcn.h>
#include <stdio.h>
#include <mach/vm_map.h>        // vm_*
#include <mach/mach_traps.h>    // task_for_pid()
#include <mach/mach.h>
typedef unsigned char byte;
 
int memArrWrite(byte* ptr, byte* data, size_t count)
{
    kern_return_t err;
    mach_port_t port = mach_task_self();

    /* fix memory protect for writing */
    err = vm_protect(port, (vm_address_t) ptr, count, FALSE, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
    if (err != KERN_SUCCESS) {
        printf("prot error: %s \n", mach_error_string(err));
        return 0;
    }
    
    /* write array */
    vm_write(port, (vm_address_t) ptr, (vm_address_t) data, sizeof(data));

    /* recovery the protect */
    err = vm_protect(port, (vm_address_t) ptr, count , FALSE, VM_PROT_READ | VM_PROT_EXECUTE);
    if (err != KERN_SUCCESS) {
        printf("prot error: %s \n", mach_error_string(err));
        return 0;
    }
    return 1;
}
 
bool funcInject(void* orgFunc,void * newFunc)
{
    /** Create payload and inline hook on orgFunc **
    *   48 b8 - movabs rax,
    *   ???? ???? - newFunc address <== for OSX x86_x64
    *   ff e0 - jmp rax
    **/ 
    char c = 0;
    c += memArrWrite((byte*)orgFunc+0x00, (byte*)"\x48\xb8", sizeof(byte)*2 );
    c += memArrWrite((byte*)orgFunc+0x02, (byte*)&newFunc, sizeof(void*) );
    c += memArrWrite((byte*)orgFunc+0x0a, (byte*)"\xff\xe0", sizeof(byte)*2 );
    return c == 3;
}
