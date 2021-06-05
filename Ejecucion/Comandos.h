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
vector<int> ID_Disco; //Signature
vector<string> ID_Disco_Path; // Signature- path

int ContadorPrimaria=0;
int ContadorExtendida=0;

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

struct Espacio
{
    int Inicio;
    int Fin;
    int Tama;
};

struct NombresParticion
{
    int signature;
    string Nombre_Particion;
    string type_Particion;
};

vector<Espacio> lstEspacios;
vector<NombresParticion> ID_ParticionDisco; //Signature Disco - NombreParticion


//OBTIENE UN NÚMERO RANDOOM DEL 1 AL 100, DIFERENTE A OTROS GENERADOS-------------------Usados en Mkdisk
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
    ID_Disco.push_back(numeroID); //Guarda El signature del disco para no repetir
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



//VERIFICA QUE EL NOMBRE DE LA PARTCION NO SE REPITA 
//-- DEVUELVE VERDADERO EN CASO SE REPITE EL NOMBRE-------------------------------------Usados en Fdisk
bool VerificarNombre(string name)
{
    bool verificacion = false;

    if (ID_ParticionDisco.size() != 0)
    {
        for (struct NombresParticion x : ID_ParticionDisco)
        {
            if (x.Nombre_Particion==name && x.signature==mbr->SIGNATURE){
                //Se repite
                verificacion=true;
                break;
            }
        }
    }   

    return verificacion;
}

//ORDENAR LAS PARTICIONES GUARDADAS EN EL MBR
void OrdenarParticiones(string path)
{
    struct partition particionTemp;

    //Ordenar Particiones
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (mbr->PARTICION[j].start < mbr->PARTICION[j + 1].start)
            {
                particionTemp = mbr->PARTICION[j];
                mbr->PARTICION[j] = mbr->PARTICION[j + 1];
                mbr->PARTICION[j + 1] = particionTemp;
            }
        }
    }

    //Guardar cambios en el archivo binario (Reescribir MBR)
    FILE *fichero;
    fichero = fopen(path.c_str(), "r+b");
    fseek(fichero, 0, 0);
    fwrite(mbr, sizeof(MBR), 1, fichero);
    fclose(fichero);
}

//OBTIENE LOS ESPACIOS DISPONIBLES PARA PARTICIONES EXTENDIDAS Y PRIMARIAS
void CalcularEspaciosLibres()
{
    OrdenarParticiones(path);
    lstEspacios.clear();
    Espacio aux;
    aux.Inicio=sizeof(MBR)+1;
    for(struct partition p: mbr->PARTICION){
        if(p.start==aux.Inicio){
            aux.Inicio=p.start+p.size;
        }else{
            Espacio libre;
            libre.Inicio=aux.Inicio;
            libre.Fin=p.start-1;
            libre.Tama=(libre.Fin-libre.Inicio)+1;
            lstEspacios.push_back(libre);

            aux.Inicio=p.start+p.size;
        }    
    }


}

void CrearParticion(int size, string f, string name, string type, string path)
{
    if (!VerificarNombre(name))
    {       
        if (f == "f")
        { //Primer Ajuste
        }
        else if (f == "b")
        { //Mejor Ajuste
        }
        else if (f == "w")
        { //Peor Ajuste
        }
    }else{
        cout<<"Error, el nombre de la partición ya existe en este disco!"<<endl;
    }
}

//-----------------------------------------------COMANDOS-----------------------------------------------------------------

//CREAR PARTICIONES
void fdisk(int size, string u, string path, string type, string f, string name, int add, string deleted, string operacion)
{
    mbr = ObtenerMBR(path);
    //Tamaño
    if (u == "m")
    {
        size = size * (1024 * 1024);
        add = add * (1024 * 1024);
    }
    else if (u == "k")
    {
        size = size * 1024;
        add = add * 1024;
    }

    //Operacion
    if (operacion == "ninguna")
    {
        //Crear particion
        CrearParticion(size, f, name, type, path);
    }
    else if (operacion == "add")
    {
        //Extender
    }
    else if (operacion == "delete")
    {
        //Eliminar
    }
    //cout << size << endl;
    //string imprimir = u + " " + path + " " + type + "\n " + f + " " + name + " " + operacion;
    //cout << imprimir << endl;
    //cout << add << endl;
    //cout << deleted << endl;
}

//CREAR DISCO Y ESCRIBIR MBR
void mkdisk(int size, string f, string u, string path)
{
    string mensaje = "";
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
        mensaje = "Disco Creado!";
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