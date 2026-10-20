#include <stdint.h>

#include "pci.h"


#include "C:\Users\youness\Documents\os\yos\kernel\video\video.h"

#include "C:\Users\youness\Documents\os\yos\kernel\inout\inout.h"

char usb_buff[64]__attribute__((aligned(64)));

uint8_t portmax;

uint64_t op_base;

uint64_t dcbaap[256]__attribute__((aligned(64)));

 uint8_t msid;

 uint64_t dbbase;
 uint8_t evedx=0;
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


struct usb
{
    
};

struct trb{
    uint32_t lowp;
    uint32_t highp;
    
    uint32_t status;
    uint32_t control;

};


struct entryerst{
    uint32_t rlowp;
    uint32_t rhighp;
    
    uint32_t rings;
    uint32_t reserved;

} __attribute__((aligned(64)));

uint32_t devicectx[256]__attribute__((aligned(64)));

uint32_t inpctx[264]__attribute__((aligned(64)));

struct trb ep0_ring[64] __attribute__((aligned(64)));

            struct trb cmdring[64] __attribute__((aligned(64)));

            volatile struct trb evring[64] __attribute__((aligned(64)));

            volatile struct trb addrdevice[64] __attribute__((aligned(64)));

            struct entryerst erst __attribute__((aligned(64)));




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

void usb_init(){

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
            portmax=(v>>24)&0xff;

            nbtostr(maxslot,b);
            print(1,12,"slotmax usb");
            print(1,13,b);

            nbtostr(portmax,b);
            print(1,14,"portmax usb");
            print(1,15,b);



            uint64_t evraddr=(uint64_t) &evring;

            erst.rlowp=(uint32_t)(evraddr &0xffffffffu);
            erst.rhighp=(uint32_t)(evraddr >>32);
            erst.rings=64;
            erst.reserved=0;

            volatile uint32_t* rtsoffreg=(volatile uint32_t*)(base+0x18);
            uint32_t rtsoff=*rtsoffreg&0xffffffe0u;
            uint64_t rtb=base+rtsoff;

            uint64_t irb=rtb+0x20;
            volatile uint32_t* erstsz=(volatile uint32_t*)(irb+0x08);
            volatile uint32_t* erstba_low=(volatile uint32_t*)(irb+0x10);
            volatile uint32_t* erstba_high=(volatile uint32_t*)(irb+0x14);
            volatile uint32_t* erdp_low=(volatile uint32_t*)(irb+0x18);
            volatile uint32_t* erdp_high=(volatile uint32_t*)(irb+0x1c);

            *erstsz=1;
            uint64_t erstaddr=(uint64_t)&erst;
            *erstba_low=(uint32_t)(erstaddr&0xffffffffu);
            *erstba_high=(uint32_t)(erstaddr>>32);

            *erdp_low=(uint32_t)(evraddr&0xffffffffu);
            *erdp_high=(uint32_t)(evraddr>>32);

             //usb sur on 
            op_base=base+caplenght;


            volatile uint32_t* usbcmd=(volatile uint32_t*)(op_base+0x00);
            *usbcmd |=(1u<<0);

            volatile uint32_t* usbsts=(volatile uint32_t*)(op_base+0x04);
            while ((*usbsts & (1u <<11)!=0)){}
            

            volatile uint32_t* crcr_low=(volatile uint32_t*)(op_base+0x18);
            volatile uint32_t* crcr_high=(volatile uint32_t*)(op_base+0x1c);
            uint64_t cmdaddr=(uint64_t)&cmdring;
            *crcr_low=(uint32_t)(cmdaddr&0xffffffffu);
            *crcr_high=(uint32_t)(cmdaddr>>32);

            cmdring[0].lowp=0;
            cmdring[0].highp=0;
            cmdring[0].status=0;
            cmdring[0].control=(23<<10)|1;

            volatile uint32_t*dboffreg=(volatile uint32_t*)(base+0x14);
            uint32_t dboff =*dboffreg&0xfffffffcu;
            dbbase=base+dboff;

            
            
           
            //on dit a xhci que on est bon 
            volatile uint32_t* doorbell0=(volatile uint32_t*)dbbase;
            *doorbell0=0;

            uint32_t nxtevr = evring+sizeof(struct trb);
            *erdp_low=nxtevr | (*erdp_low &0xf);

            
            while (1)
            {   
                uint8_t rc=evring[0].control & 0x1;
                print(1,17,"pas encore");
                if(rc==1){
                    print(1,17,"usb a rep IL A REP");
                    break;
                }
                uint8_t rc1=evring[1].control & 0x1;
                if(rc1==1){
                    uint8_t rr=(evring[1].control >> 24 )&0xff;

                    print(10,17,"slot acitv");

                    char slo[16];
                    nbtostr(rr,slo);
                    print(10,18,slo);
                    break;
                }
            }
            
            volatile uint64_t* dcreg=(volatile uint64_t*)(op_base+0x30);
            *dcreg=(uint64_t) &dcbaap;

            volatile uint32_t* configr=(volatile uint32_t*)(op_base+0x38);
            *configr=maxslot;

            cmdring[1].lowp=0;
            cmdring[1].highp=0;
            cmdring[1].status=0;
            cmdring[1].control=(9<<10)|1;
            
            doorbell0=(volatile uint32_t*)dbbase;
            *doorbell0=0;

            while((evring[evedx].control & 0x1)==0){

            }
            evedx++;
            msid=(evring[1].control >>24)&0xff;

            char bnb[16];
            nbtostr(msid,bnb);

            print(6,31,bnb);

            dcbaap[msid]=(uint64_t)&devicectx;

           
            
            } 
                    
        }

    

    





}

void scan_usb_port(){
    char b[16];

    uint64_t pb=op_base+0x400;

    for (int i=0; i<portmax ;i++){
        
        volatile uint32_t* ports=(volatile uint32_t*) (pb+(i*0x10));

        int valp=*ports;
        uint8_t con=valp&0x1;

        if (con==1){
            
            print(1,20+i,"port");
            nbtostr(i,b);
            print(6,20+i,b);
            print(9,20+i,"peif detect");
            *ports=valp|(1<<4);
            print(1,22+i,"reveil...");
            while ((*ports&(1<<1))==0){}
            print(1,23+i,"port reveiller et actif");


            inpctx[1]=(1u<<0)|(1u<<1);
            inpctx[8+0]=(1u<<27)|((uint32_t)(i+1)<<16);

            inpctx[16+0]=(4u<<3)|(64u<<16);
            uint64_t trad=(uint64_t) &ep0_ring[0];
            inpctx[16+2]=((uint32_t)trad &0xffffffffu)|1u;
            inpctx[16+3]=(uint32_t)(trad>>32);
            inpctx[16+4]=8;

            cmdring[2].lowp=(uint32_t)((uint64_t)&inpctx&0xffffffffu);
            cmdring[2].highp=(uint32_t)((uint64_t)&inpctx>>32);
            cmdring[2].status=0;
            cmdring[2].control=(11<<10)|((uint32_t)msid<<24)|1;

            volatile uint32_t* doorbell0=(volatile uint32_t*)dbbase;
            *doorbell0=0;


            while((evring[evedx].control&0x1)==0){}
            evedx=(evedx+1)&63;
            print(1,25+i,"status:perisph addr");

            for (int k=0;k<64;k++){
                addrdevice[k].lowp=0; 
                addrdevice[k].highp=0;
                addrdevice[k].status=0;
                addrdevice[k].control=0;

                

                


            
            }

            ep0_ring[0].lowp=0x01000680u; 
            ep0_ring[0].highp=0x00080000u;
            ep0_ring[0].status=8;
            ep0_ring[0].control=(2u<<10)|(1u<<6)|(3u<<16)|(1u<<4)| 1u;

            uint64_t buf_p=(uint64_t)&usb_buff;

            ep0_ring[1].lowp=(uint32_t)(buf_p&0xffffffffu); 
            ep0_ring[1].highp=(uint32_t)(buf_p>>32);
            ep0_ring[1].status=8;
            ep0_ring[1].control=(3u<<10)|(1u<<16)|1u;

                
            ep0_ring[2].lowp=0; 
            ep0_ring[2].highp=0;
            ep0_ring[2].status=0;
            ep0_ring[2].control=(4u<<10)|(1u<<2)|(1u<<5) |1u;

            volatile uint32_t* devdor=(volatile uint32_t*)(dbbase +(msid*4));
            *devdor=1;

                while((evring[evedx].control &0x1)==0){

                }
                evedx=(evedx+1)&63;
                

                if(usb_buff[4]==0x03 && usb_buff[5]==0x01 && usb_buff[6]==0x01){
                    print(1,26+i,"c un clavier usb ");
                }else if(usb_buff[4]==0x00 && usb_buff[5]==0x00 && usb_buff[6]==0x00){
                    print(1,26+i,"classe");
                }else{

                    char a[16];
                    print(1,26+i,"non deso");
                    nbtostr(usb_buff[4],a);
                    print(26,26+i,a);
                    nbtostr(usb_buff[5],a);
                    print(30,26+i,a);
                    nbtostr(usb_buff[6],a);
                    print(40,26+i,a);

                }
                
                //demande de config
                ep0_ring[3].lowp=0x02000680u; 
                ep0_ring[3].highp=0x00090000u;
                ep0_ring[3].status=8;
                ep0_ring[3].control=(2u<<10)|(1u<<6)|(3u<<16)|(1u<<4)| 1u;

                 buf_p=(uint64_t)&usb_buff;

                ep0_ring[4].lowp=(uint32_t)(buf_p&0xffffffffu); 
                ep0_ring[4].highp=(uint32_t)(buf_p>>32);
                ep0_ring[4].status=9;
                ep0_ring[4].control=(3u<<10)|(1u<<16)|1u;

                

                ep0_ring[5].lowp=0; 
                ep0_ring[5].highp=0;
                ep0_ring[5].status=0;
                ep0_ring[5].control=(4u<<10)|(1u<<2)|(1u<<5) |1u;

                *devdor=1;
                
                while((evring[evedx].control &0x1)==0){

                }
                evedx=(evedx+1)&63;
                
                
                
                uint16_t  totall=usb_buff[2] | (usb_buff[3]<<8);

                ep0_ring[6].lowp=0x02000680u; 
                ep0_ring[6].highp=((uint32_t)totall<<16)|0x0000u;
                ep0_ring[6].status=8;
                ep0_ring[6].control=(2u<<10)|(1u<<6)|(3u<<16)|(1u<<4)| 1u;

                ep0_ring[7].lowp=(uint32_t)(buf_p&0xffffffffu); 
                ep0_ring[7].highp=(uint32_t)(buf_p>>32);
                ep0_ring[7].status=totall;
                ep0_ring[7].control=(3u<<10)|(1u<<16)|1u;

                

                ep0_ring[8].lowp=0; 
                ep0_ring[8].highp=0;
                ep0_ring[8].status=0;
                ep0_ring[8].control=(4u<<10)| (1u<<2) |(0u<<16) |1u;

                *devdor=1;
                
                while((evring[evedx].control &0x1)==0){
                    char cc[16];

                    nbtostr(evedx,cc);
                    print(55,2,cc);
                }
                evedx=(evedx+1)&63;
                print(50,0,"test pour tkt ta compris ");
                

                
                
                
                int ptr=0;

                while (ptr<totall){
                    uint8_t len= usb_buff[ptr];
                    uint8_t type= usb_buff[ptr+1];
                    
                    if(len==0)break;
                    
        

                    if (type==4){
                        if (usb_buff[ptr+5]==0x03&&usb_buff[ptr+6]==0x01&&usb_buff[ptr+7]==0x01){
                            print(1,33+i,"clavier usb trouver ");
                        }
                    }
                    ptr+=len;

                }



             
            

        }else{
            
        }

    }
}


    


   