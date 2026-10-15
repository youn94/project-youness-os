
#ifndef PCI_H
#define PCI_H
#include <stdint.h>
#include "C:\Users\youness\Documents\os\yos\kernel\video\video.h"

#include "C:\Users\youness\Documents\os\yos\kernel\inout\inout.h"




struct repons;
int pci(unsigned short bus,unsigned short device,unsigned short function,unsigned short registre);

void pci_scan();

#endif
