#include <stdint.h>
#include "ps2.h"
#include "C:\Users\youness\Documents\os\yos\kernel\video\video.h"

#include "C:\Users\youness\Documents\os\yos\kernel\inout\inout.h"

struct idt32{
    uint16_t addrbass;
    uint16_t selector;
    uint8_t zero;
    uint8_t typeattribute;
    uint16_t addrhaut ;
}__attribute__((packed));

struct idt32 idt[256];

struct pointeridt{
    uint16_t taille;
    uint32_t tabaddr;
}__attribute__((packed));

struct pointeridt idtptr;

void pic_remap(){

    outb(0x20,0x11);
    outb(0xa0,0x11);


    outb(0x21,0x20);
    outb(0xa1,0x28);

    outb(0x21,0x04);
    outb(0xa1,0x02);

    outb(0x21,0x01);
    outb(0xa1,0x01);

    outb(0x21,0xfd);
    outb(0xa1,0xff);


}

int strcmp(char *text,char *cmptext){
    
    int i=0;

    while(text[i] != '\0'&& cmptext[i] != '\0'){

        if(text[i]!=cmptext[i]){
            return 0;
        }
        i++;

    }
    
        
    
    return text[i]=='\0'&&cmptext[i]=='\0';
}


void initidt(){

    idtptr.taille = (sizeof(struct idt32)*256)-1;
    idtptr.tabaddr=(uint32_t)&idt;

    for (int i=0;i<256;i++){
        ramplirtab(i,0);
    }
    
    
    __asm__ volatile("lidt %0"::"m"(idtptr));

}

void ramplirtab(uint8_t linetab,uint32_t data){

    idt[linetab].addrbass= (data & 0xffff);
    idt[linetab].addrhaut= ((data >>16) & 0xffff);
    idt[linetab].selector= 0x08;
    idt[linetab].zero=0;
    idt[linetab].typeattribute=0x8e;
}

static const char clavier_azerty[128] = {
        0, 27,
        '&', 'é', '"', '\'', '(', '-', 'e', '_', 'c', 'a', ')', '=',
        '\b',
        '\t',
        'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$',
        '\n',
        29,
        'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'u', '*',
        0,
        '<', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', '.', '!',
        0x20,
        '*',
        0,
        ' '
    };


const vald=2;
int x=vald;
int y=4;

void base(){
    x=vald;
    print(0,y,":>");
}

char buffer[7680];

int len =0;

__attribute__((interrupt))
void key(void *frame){  
    

    uint8_t scancode=inb(0x60);
    char lettre=clavier_azerty[scancode];
    
    if (scancode <0x80){
        
        
        //entre
        if (scancode==0x1c){
            //debug buffer
            y++;
            print(2,y,buffer);
            
            

            
            if (strcmp(buffer,"perif")){
                y++;
                print(2,y,"ta taper comand perif");
            }
            //retour ligne
            len=0;
            y++;
            base(); 


            
        }

        //supprimer
        else if (scancode==0x0E){
            if (!(x<=2)){
                drawchar(x,y,' ');
                x--;
                
                len--;
                buffer[len]='\0';
            }
            
        }
                        
                        
            

        else{
            
            x++;
            drawchar(x,y,lettre);

            buffer[len]=lettre;

            len++;
        }
    }
    outb(0x20,0x20);


}

