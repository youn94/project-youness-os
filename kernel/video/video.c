
#include <stdint.h>
#include "video.h"


void draw_pixel(int x,int y,uint32_t color){
    uint32_t flfb=*(volatile uint32_t*)0x500;
    uint32_t* lfb=(uint32_t*)flfb;

    uint32_t pitch=*(volatile uint32_t*)0x504;

    uint32_t off=y*(4096/4)+x;
    lfb[off]=color;
    //lfb[2]=0x00ffffff;
}

void drawchar(int x,int y,char c){

   

    uint8_t* afont=*(volatile uint8_t**)0x508;

    uint8_t* A=afont +(c*16);

    x*=8;
    y*=16;


    for (int i=0;i<16;i++){
        
        //draw_pixel(100+i,100+y);
        uint8_t dataa =A[i];
        for (int ii=0;ii<8;ii++){
            
            if (dataa & (1<<(7-ii))){
                draw_pixel(x+ii,y+i,0x00ffffff);
                
            }else{
                draw_pixel(x+ii,y+i,0x00121746);
            }
           
        }

        
        
    }
}

void clear_screen(){
    
    for (int i=0;i<1024;i++){
        for (int ii=0;ii<748;ii++){
            draw_pixel(i,ii,0x00121746);
        }
    }
    

}



void print(int x,int y,char*c){
    
    for (int i=0;c[i]!='\0';i++){
        drawchar(x+i,y,c[i]);
    }
    
}

