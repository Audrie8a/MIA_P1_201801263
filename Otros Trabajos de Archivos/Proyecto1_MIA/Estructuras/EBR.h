#ifndef EBR_H
#define EBR_H

struct EBR
{
    char status [1];
    char fit [1];   //B, F , W
    int start;
    int size;
    int next;
    char nombre[16];
   
};


#endif