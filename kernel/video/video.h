
#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>


void draw_pixel(int x,int y,uint32_t color);


void drawchar(int x,int y,char c);

void clear_screen();

void print(int x,int y,char*c);

#endif