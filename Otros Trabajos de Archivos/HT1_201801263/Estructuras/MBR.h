#include "../Estructuras/Particion.h"

#ifndef MBR_H
#define MBR_H

struct MBR
{
    int SIGNATURE;
    char FIT[1];
    int SIZE;
    char FECHA [25];
    struct partition PARTICION[4];
};



#endif