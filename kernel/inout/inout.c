#include <stdint.h>

#include "inout.h"

void nbtostr(uint64_t nb,char* buf){
    int i=0;

    if (nb==0){
        buf[0]='0';
        buf[1]='\0';
        return;

    }

    while(nb>0){
        buf[i++]='0'+(nb%10);
        nb/=10;
    }

    for (int j=0;j<i/2;j++){
        char tmp=buf[j];
        buf[j]=buf[i-1-j];
        buf[i-1-j]=tmp;

    }

    buf[i]='\0';

}

unsigned char  inb(unsigned short port){
    unsigned char data;

    __asm__ volatile(
        "inb %1 ,%0"
        
        : "=a"(data): "Nd" (port));

    return data;
}

void outb(unsigned short port,unsigned char value){


    __asm__ volatile(
        "outb %0 ,%1"
        
        :: "a"(value),"Nd" (port));


}

void outw(unsigned short port,unsigned short value){


    __asm__ volatile(
        "outw %0 ,%w1":: "a"(value),"Nd" (port));


}

unsigned short inw(unsigned short port){

    unsigned short result;
    __asm__ volatile("inw %w1 ,%0" : "=a"(result) : "Nd" (port));
        return result;

}

void outl(unsigned int port,unsigned int value){


    __asm__ volatile(
        "outl %0 ,%w1":: "a"(value),"Nd" (port));


}

unsigned int inl(unsigned int port){

    unsigned int result;
    __asm__ volatile("inl %w1 ,%0" : "=a"(result) : "Nd" (port));
        return result;

}


