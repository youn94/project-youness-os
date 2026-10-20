

#ifndef PS2_H
#define PS2_H
#include <stdint.h>
#include "C:\Users\youness\Documents\os\yos\kernel\video\video.h"

#include "C:\Users\youness\Documents\os\yos\kernel\inout\inout.h"

void pic_remap();

void initidt();

int strcmp(char *text,char *cmptext);

void base();

void ramplirtab(uint8_t linetab,uint32_t data);


__attribute__((interrupt))
void key(void *frame);


#endif