
#include <stdint.h>
#include "kernel/video/video.h"
#include "kernel/inout/inout.h"
#include "kernel/pci/pci.h"


int kernel_main (){
    clear_screen();
    
    drawchar(50,9,'a');
    
    
    print(0,1,"bienvenu yOS v1 ");
    print(0,2,"terminal yterm");

    pci_scan();



    while (1) {
        
    }
    return 0;
}




