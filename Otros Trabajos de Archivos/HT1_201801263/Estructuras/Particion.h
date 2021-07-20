

#ifndef PARTITION_H
#define PARTITION_H

struct partition
{
    char status [1];
    char type [1];
    char fit [2];
    int start;
    int size;
    char nombre[20];
   
};


#endif