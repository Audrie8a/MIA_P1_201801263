#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../Estructuras/MBR.h"
#include "../Estructuras/EBR.h"
#include <algorithm>
using namespace std;

#ifndef COMANDO_H
#define COMANDO_H
vector<string> ID_Disco_Path;       // Signature- path
vector<int> ID_Disco;               //Signature
vector<string> ID_ParticionDisco;   //nombre particion - path
vector<string> ListaEspaciosLibres; //start- fin
vector<string> ParticionMontada;    // id- nombre- path
vector<string> listParticionLogica; //nombre - path
struct MBR *mbr = new struct MBR;
struct EBR *ebr = new struct EBR;

class Comando
{
public:
    Comando()
    {
    }
    ~Comando(){};
};


//OBTIENE UN NÚMERO RANDOOM DEL 1 AL 100, DIFERENTE A OTROS GENERADOS
int SignatureDisco()
{
    int numero = (rand() % 100);
    if (ID_Disco.size() != 0)
    {
        for (auto x : ID_Disco)
        {
            if (x == numero)
            {
                numero = SignatureDisco();
            }
        }
    }    
    return numero;
}

//OBTIENE LOS DATOS DEL MBR, NECESITA RUTA
struct MBR *ObtenerMBR(string path)
{
    FILE *arch;
    arch = fopen(path.c_str(), "rb");
    struct MBR *newMBR = new struct MBR;
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, 0, SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(newMBR, sizeof(MBR), 1, arch);
    //cout<<sizeof(MBR)<<endl;
    //cout<<newMBR->SIZE<<endl;
    //cout<<newMBR->FECHA<<endl;
    //cout<<newMBR->SIGNATURE<<endl;
    //cout<<newMBR->FIT<<endl;
    fclose(arch);

    return newMBR;
}

//ESCRIBIR DATOS DEL MBR EN EL ARCHIVO
struct MBR *EscribirMBR(char *fit, int size)
{
    struct partition parte[4];
    parte[0].start = 0;
    parte[0].size = 0;

    struct MBR *newMBR = new struct MBR;

    //Signature
    int numeroID = SignatureDisco();
    ID_Disco.push_back(numeroID);   //Guarda El signature del disco para no repetir
    newMBR->SIGNATURE = numeroID;

    //Size
    newMBR->SIZE = size;

    //Fit
    strcpy(newMBR->FIT, fit);

    //Fecha
    char fecha[25]; //ctime devuelve 26 caracteres pero tambien se podría usar un puntero de char
    time_t current_time;
    current_time = time(NULL);
    ctime(&current_time);
    strcpy(fecha, ctime(&current_time));
    strcpy(newMBR->FECHA, fecha);
    //Particiones
    newMBR->PARTICION[0] = parte[0];
    newMBR->PARTICION[1] = parte[0];
    newMBR->PARTICION[2] = parte[0];
    newMBR->PARTICION[3] = parte[0];
    return newMBR;
}


//-----------------------------------------------COMANDOS-----------------------------------------------------------------

//CREAR DISCO Y ESCRIBIR MBR
void mkdisk(int size, string f, string u, string path)
{
    string mensaje ="";
    //Obtener tamaño en bytes
    if (u == "m")
    {
        size = size * (1024 * 1024);
    }
    else if (u == "k")
    {
        size = size * 1024;
    }
    FILE *fichero;
    fichero = fopen(path.c_str(), "wb");

    if (fichero)
    {
        //Se crea el archivo binario
        fseek(fichero, 0, 0);
        fwrite("0", 1, 1, fichero);
        fseek(fichero, size, 0);
        fwrite("0", 1, 1, fichero);

        //Escribiendo el MBR
        char fit[1];
        strcpy(fit, f.c_str());
        struct MBR *getMbr = EscribirMBR(fit, size);

        fseek(fichero, 0, 0);
        fwrite(getMbr, sizeof(MBR) - 1, 1, fichero);
        mensaje="Disco Creado!";
    }
    else
    {
        mensaje = "No se pudo crear el archivo en: " + path;

        
    }
    fclose(fichero);
    cout << mensaje << endl; 
    //mbr= ObtenerMBR(path);
}

#endif