#include "../Estructuras/Particion.h"

#ifndef MBR_H
#define MBR_H

struct MBR
{    
    int SIZE;
    //char FECHA [25];
    time_t FECHA;
    int SIGNATURE;  //numero randum ID disco
    char FIT[1];
    struct partition PARTICION[4];
};



#endif