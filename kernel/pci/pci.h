
#ifndef PCI_H
#define PCI_H
#include <stdint.h>
#include "C:\Users\youness\Documents\os\yos\kernel\video\video.h"

#include "C:\Users\youness\Documents\os\yos\kernel\inout\inout.h"




struct repons;
int pci_r(unsigned short bus,unsigned short device,unsigned short function,unsigned short registre);

void pci_w(unsigned short bus,unsigned short device,unsigned short function,unsigned short registre,uint32_t dat);

void pci_scanusb();
void usb_init();


void scan_usb_port();

struct entryerst;

struct trb;



#endif
