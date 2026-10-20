
#include <stdint.h>
#include "kernel/video/video.h"
#include "kernel/inout/inout.h"
#include "kernel/pci/pci.h"
#include "kernel/pci/pci.h"
#include "kernel/ps2/ps2.h"

int kernel_main (){
    clear_screen();
    
    
    drawchar(50,9,'a');
    
    
    print(0,1,"bienvenu yOS v1 2");
    print(0,2,"terminal yterm");

    //usb_init();
    
    //scan_usb_port();

    pic_remap();

    initidt();

    ramplirtab(33,(uint32_t) key);
    
    __asm__ volatile("sti");
    
    base();
    while (1) {
        
    }
    return 0;
}




