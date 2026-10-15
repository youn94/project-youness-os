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


int pci(unsigned short bus,unsigned short device,unsigned short function,unsigned short registre){


    unsigned short addr=0xcf8;
    int data;

 

  

    data =0x80000000u |((uint32_t) bus << 16)|((uint32_t)device << 11)|((uint32_t)function<<8)|(registre &0xfc);

    outl(addr,data);


    int rep= inl(0x0CFC);
    
    return rep;

}



void pci_scan(){

    int rep=pci(0,5,0,0); 

    struct repons p;


    p.vendor_id=rep & 0xffff;
    p.device_id=(rep >>16) & 0xffff;

    
    
    int rep2=pci(0,5,0,0x08); 

    p.revison=rep2 &0xff;
    p.class_code=(rep2 >>24)&0xff;

    if (p.class_code==0x0c){
        print(20,20,"controleur usb trouver");
    }else{
        print(20,20,"controleur usb non trouver");

    }

    int bar0=pci(0,5,0,0x10);

    int bar1=pci(0,5,0,0x14);

    char bf[16];

    nbtostr(bar0,bf);

    print(25,25,bf);

    if ((bar0 &1u)==0){
        print(24,24,"c mmio");
    }else{
        print(24,24,"c pas mmio");
    }

    int type=(bar0>>1)&3;

    if (type==0){
        print(32,32,"32");
    }else if (type==2){
        print(31,31,"64");
    }

    uint64_t base=((uint64_t)bar1<<32)|(bar0 &0xfffffff0u);




}
    


   