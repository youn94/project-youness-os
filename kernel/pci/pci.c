#include <stdint.h>

#include "pci.h"


#include "C:\Users\youness\Documents\os\yos\kernel\video\video.h"

#include "C:\Users\youness\Documents\os\yos\kernel\inout\inout.h"


struct repons{
        uint8_t bus;
        uint8_t devices;
        uint8_t function;

        uint16_t vendor_id;
        uint16_t device_id;

        uint8_t class_code;
        uint8_t subclass;
        uint8_t progif;
        uint8_t revison;



    };


struct trb{

    uint32_t highp;
    uint32_t lowp;
    uint32_t status;
    uint32_t control;

};


struct entryerst{

    uint32_t rhighp;
    uint32_t rlowp;
    uint32_t rings;
    uint32_t reserved;

}; __attribute__((aligned(64)));







int pci_r(unsigned short bus,unsigned short device,unsigned short function,unsigned short registre){


    unsigned short addr=0xcf8;
    int data;

 

  

    data =0x80000000u |((uint32_t) bus << 16)|((uint32_t)device << 11)|((uint32_t)function<<8)|(registre &0xfc);

    outl(addr,data);


    int rep= inl(0x0CFC);
    
    return rep;

}

void pci_w(unsigned short bus,unsigned short device,unsigned short function,unsigned short registre,uint32_t dat){


    unsigned short addr=0xcf8;
    int data;

 

  

    data =0x80000000u |((uint32_t) bus << 16)|((uint32_t)device << 11)|((uint32_t)function<<8)|(registre &0xfc);

    outl(addr,data);


    outl(0x0CFC,dat);
    

}

void pci_scanusb(){
        int rep=pci_r(0,5,0,0); 

    struct repons p;


    p.vendor_id=rep & 0xffff;
    p.device_id=(rep >>16) & 0xffff;

    
    
    int rep2=pci_r(0,5,0,0x08); 

    p.revison=rep2 &0xff;
    p.progif=(rep2 >>8)&0xff;
    p.subclass=(rep2 >>16)&0xff;
    p.class_code=(rep2 >>24)&0xff;

    if (p.class_code==0x0c && p.subclass==0x03){
        print(20,20,"controleur usb trouver");
    }else{
        print(20,20,"controleur usb non trouver");

    }

    if (p.progif==0x00){
        print(0,2,"usb uhci 1.0");
    }else if (p.progif==0x10){
        print(0,3,"usb ohci 1.1");
    }else if (p.progif==0x20){
        print(0,4,"usb ehci 2.0");
    }else if (p.progif==0x30){
        print(0,5,"usb xhci 3.0 ou 4");
    }
}

void pci_scan(){

    int pcicmd= pci_r(0,5,0,0x04);
    pcicmd|=(1<<1)|(1<<2);

    pci_w(0,5,0,0x04,pcicmd);

    pci_scanusb();

    int bar0=pci_r(0,5,0,0x10);

    int bar1=pci_r(0,5,0,0x14);

    char bf[16];

    nbtostr(bar0,bf);

    print(25,25,bf);

    if ((bar0 &1u)==1){
        print(24,24,"bar e/s");
    }else{
        print(24,24,"c mmio");
        int type=(bar0>>1)&3;
        if (type==0){
            print(1,32,"32");
        }else if (type==2){
            print(1,31,"64");
            
            uint64_t base=((uint64_t)bar1<<32)|(bar0 &0xfffffff0u);

            volatile uint32_t* d=(volatile uint32_t *) base;

            uint32_t val=*d;

            nbtostr(val,bf);
            print(1,7,bf);

            uint8_t caplenght=val& 0xff;
            uint16_t hciversion=(val>>16)& 0xffff;

            char b[16];

            nbtostr(caplenght,b);
            print(1,8,"cap");
            print(1,9,b);

            nbtostr(hciversion,b);
            print(1,10,"hci");
            print(1,11,b);

            
            uint64_t b1=((uint64_t)bar1<<32)|(bar0 &0xfffffff0u);

            volatile uint32_t* d1=(volatile uint32_t *) (b1+0x04);

            uint32_t v=*d1;

            uint8_t maxslot=v&0xff;
            uint8_t portmax=(v>>24)&0xff;

            nbtostr(maxslot,b);
            print(1,12,"slotmax usb");
            print(1,13,b);

            nbtostr(portmax,b);
            print(1,14,"portmax usb");
            print(1,15,b);
            struct trb cmdring[64] __attribute__((aligned(64)));

            struct entryerst erst __attribute__((aligned(64)));
            uint64_t evraddr=(uint64_t) &cmdring;

            erst.rhighp=(uint32_t)(evraddr &0xffffffffu);
            erst.rlowp=(uint32_t)(evraddr >>32);
            erst.rings=64;
            erst.reserved=0;
            




            }
                    
        }

    

    





}


    


   