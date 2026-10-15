
#ifndef INOUT_H
#define INOUT_H

void nbtostr(uint64_t nb,char* buf);

unsigned char  inb(unsigned short port);

void outb(unsigned short port,unsigned char value);

void outw(unsigned short port,unsigned short value);

unsigned short inw(unsigned short port);

void outl(unsigned int port,unsigned int value);

unsigned int inl(unsigned int port);

#endif