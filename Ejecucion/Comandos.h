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

int ContadorPrimaria = 0;
int ContadorExtendida = 0;
int ContadorMount=0;

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

//CREAR ESTRUCTURA NODO
struct nodo
{
    int signature;
    string name;
    string id;
    struct nodo *sig;
};

typedef struct nodo *TLista;

TLista listaMount = NULL;

void insertarFinal(TLista &lista, int signature, string name, string id)
{
    TLista t, q = new (struct nodo);
    q->signature = signature;
    q->name = name;
    q->id = id;
    q->sig = nullptr;

    if (lista == nullptr)
    {
        lista = q;
    }
    else
    {
        t = lista;
        while (t->sig != nullptr)
        {
            t = t->sig;
        }
        t->sig = q;
    }
    cout<<"Particion Montada!"<<endl;
}

void eliminarElemento(TLista &lista, string id)
{
    TLista p, ant;
    p = lista;

    if (lista != nullptr)
    {
        while (p != nullptr)
        {
            if (p->id == id)
            {
                if (p == lista)
                {
                    lista = lista->sig;
                }
                else
                {
                    ant->sig = p->sig;
                }
                delete (p);
                return;
            }
            ant = p;
            p = p->sig;
        }
    }
    else
    {
        cout << "Aún no existen Particiones Montadas!" << endl;
    }
}

TLista ObtenerUltimoNodo(TLista lista)
{
    TLista q = lista;
    TLista aux=q;
    while (q != nullptr)
    {
        aux=q;
        q = q->sig;

    }

    return aux;
}

bool buscarElemento(TLista lista, string id)
{
    bool existencia = false;

    TLista q = lista;
    int i = 1;
    while (q != nullptr)
    {
        if (q->id == id)
        {
            existencia = true;
            break;
        }
        q = q->sig;
        i++;
    }
    return existencia;
}

TLista ParticionesMontadas_Disco(TLista lista, int signature)
{
    int existencia = 0;
    TLista aux;
    TLista q = lista;
    aux=q;
    while (q != nullptr)
    {
        if (q->signature == signature)
        {
            existencia++;
            aux = q;
            ContadorMount = existencia; //Se guarda en signature el número de partición montada que es
        }
        q = q->sig;
    }
    return aux;
}

string GenerarID(int signature, string name)
{
    ContadorMount=0;
    string abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string udt = "1234567890";
    string id = "63";
    TLista aux = ParticionesMontadas_Disco(listaMount, signature);
    //Numero de particiones Montadas en el disco
    
    if (ContadorMount==0)
    { //No se ha montado ninguna partición de este disco
        
        aux = ObtenerUltimoNodo(listaMount);
        if (aux != nullptr) //si la lista no está vacía        
        {
            
            string auxNum = "";
            for (int i = 2; i < aux->id.size() - 1; i++)
            {
                auxNum += aux->id[i];
            }
            //Se asigna el siguiente número 
            int num=atoi(auxNum.c_str())+1;
            auxNum=to_string(num);
            id+=auxNum+"A";
        }
        else //Si la lista está vacía
        {
            id += "1A";
        }
    }
    else
    {
        int contador = ContadorMount;
        string auxNum = "";
        for (int i = 2; i < aux->id.size() - 1; i++)
        {
            auxNum += aux->id[i];
        }
        id += auxNum + abc[contador];
    }

    return id;
}

struct Espacio
{
    int Inicio;
    int Fin;
    int Tama;
    int InicioAnterior;
};

struct NombresParticion
{
    int signature;
    string Nombre_Particion;
    string type_Particion;
};

struct Contadores
{
    int ContadorP;
    int ContadorE;
    int signature;
};

vector<Contadores> lstContadores;
vector<Espacio> lstEspacios;
vector<NombresParticion> ID_ParticionDisco; //Signature Disco - NombreParticion

//------------------------------------------------------------------GRAFICAS-------------------------------------------------------------------
void ReporteMBR()
{
    string Imprimir = "digraph G {";

    Imprimir += "\tnodo[shape=plaintext label=<";
    Imprimir += "\t \t    <table>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\"><b>Nombre</b></td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\"><b>Valor</b></td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">mbr_tamano</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->SIZE) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">mbr_fecha_creacion</td>";
    //Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->FECHA) + "</td>";
    char fecha[25];
    time_t current_time = mbr->FECHA;
    ctime(&current_time);
    strcpy(fecha, ctime(&current_time));
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(fecha) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">mbr_disk_signature</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->SIGNATURE) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t            <td colspan=\"4\">Disk_fit</td>";
    Imprimir += "\t \t \t \t            <td colspan=\"4\">" + string(mbr->FIT) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        ";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_1</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[0].status) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_1</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[0].type) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_1</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[0].fit) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_1</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[0].start) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_1</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[0].size) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_1</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[0].nombre) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        ";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_2</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[1].status) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_2</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[1].type) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_2</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[1].fit) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_2</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[1].start) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_2</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[1].size) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_2</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[1].nombre) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        ";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_3</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[2].status) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_3</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[2].type) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_3</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[2].fit) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_3</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[2].start) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_3</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[2].size) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_3</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[2].nombre) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        ";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_4</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[3].status) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_4</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[3].type) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_4</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[3].fit) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_4</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[3].start) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_4</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[3].size) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t        <tr>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_4</td>";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[3].nombre) + "</td>";
    Imprimir += "\t \t \t        </tr>";
    Imprimir += "\t \t \t    ";
    Imprimir += "\t \t    </table>";
    Imprimir += "\t>];";
    Imprimir += "}";
}

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

    newMBR->FECHA = current_time;

    //ctime(&current_time);
    //strcpy(fecha, ctime(&current_time));
    //strcpy(newMBR->FECHA, fecha);
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
            if (x.Nombre_Particion == name && x.signature == mbr->SIGNATURE)
            {
                //Se repite
                verificacion = true;
                break;
            }
        }
    }

    return verificacion;
}

//ACTUALIZA MBR EN EL DISCO
void ActualizarMBR(string path)
{
    FILE *fichero;
    fichero = fopen(path.c_str(), "r+b");
    fseek(fichero, 0, 0);
    fwrite(mbr, sizeof(MBR), 1, fichero);
    fclose(fichero);
    mbr = ObtenerMBR(path);
}

void ActualizarContadores(int opcion)
{
    if (opcion == 1) //Actualiza los contadores
    {
        ContadorPrimaria = 0;
        ContadorExtendida = 0;
        for (struct Contadores x : lstContadores)
        {
            if (x.signature == mbr->SIGNATURE)
            {
                ContadorPrimaria = x.ContadorP;
                ContadorExtendida = x.ContadorE;
                break;
            }
        }
    }
    else
    { //Actualiza los contadores en la lista
        int cont = 0;
        for (struct Contadores x : lstContadores)
        {
            if (x.signature == mbr->SIGNATURE)
            {
                lstContadores[cont].ContadorE = ContadorExtendida;
                lstContadores[cont].ContadorP = ContadorPrimaria;
                cont++;
                break;
            }
        }

        //Si no encunetra los contadores pretenecientes al disco, los agrega
        if (cont == 0)
        {
            struct Contadores Contando;
            Contando.ContadorE = ContadorExtendida;
            Contando.ContadorP = ContadorPrimaria;
            Contando.signature = mbr->SIGNATURE;
            lstContadores.push_back(Contando);
        }
    }
}

struct EBR *ObtenerEBR(string path, int posicion)
{
    FILE *arch;
    arch = fopen(path.c_str(), "rb");
    struct EBR *newEBR = new struct EBR;
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, posicion, SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(newEBR, sizeof(EBR), 1, arch);
    //cout<<sizeof(EBR)<<endl;
    //cout<<newEBR->status<<endl;
    //cout<<newEBR->fit<<endl;
    //cout<<newEBR->start<<endl;
    //cout<<newEBR->next<<endl;
    //cout<<newEBR->nombre<<endl;
    fclose(arch);

    return newEBR;
}

void EscribirEBR(string path, int start)
{
    FILE *fichero;
    fichero = fopen(path.c_str(), "r+b");
    fseek(fichero, start, 0);
    fwrite(ebr, sizeof(EBR), 1, fichero);
    fclose(fichero);
    mbr = ObtenerMBR(path);
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
            if (mbr->PARTICION[j].start > mbr->PARTICION[j + 1].start)
            {
                particionTemp = mbr->PARTICION[j];
                mbr->PARTICION[j] = mbr->PARTICION[j + 1];
                mbr->PARTICION[j + 1] = particionTemp;
            }
        }
    }

    //Guardar cambios en el archivo binario (Reescribir MBR)
    ActualizarMBR(path);
}

//OBTIENE LOS ESPACIOS DISPONIBLES PARA PARTICIONES EXTENDIDAS Y PRIMARIAS
void CalcularEspaciosLibres(string path)
{
    OrdenarParticiones(path);
    lstEspacios.clear();
    Espacio aux;
    aux.Inicio = sizeof(MBR);
    int contador = 0;
    for (struct partition p : mbr->PARTICION)
    {
        if (p.size != 0)
        {
            if (p.start == aux.Inicio)
            {
                aux.Inicio = p.start + p.size;
            }
            else
            {
                Espacio libre;
                libre.Inicio = aux.Inicio;
                libre.Fin = p.start - 1;
                libre.Tama = (libre.Fin - libre.Inicio) + 1;
                lstEspacios.push_back(libre);

                aux.Inicio = p.start + p.size;
            }
        }
        else
        {
            contador++;
        }
    }

    //En caso no se hayan ingresado particiones aún
    if (contador == 4)
    {
        Espacio libre;
        libre.Inicio = sizeof(MBR);
        libre.Fin = mbr->SIZE - 1;
        libre.Tama = libre.Fin - libre.Inicio + 1;
        lstEspacios.push_back(libre);
    }
    else if (aux.Inicio != mbr->SIZE - 1)
    { //En caso la última posición esté libre
        Espacio libre;
        libre.Inicio = aux.Inicio;
        libre.Fin = mbr->SIZE - 1;
        libre.Tama = libre.Fin - libre.Inicio + 1;
        lstEspacios.push_back(libre);
    }
}

void CalcularEspaciosLogicas(int numParticion, string path)
{
    lstEspacios.clear();
    struct partition Extendida = mbr->PARTICION[numParticion];

    ebr = ObtenerEBR(path, Extendida.start);
    string nombre = ebr->nombre;
    if (nombre == "xx" && ebr->next == -1)
    { //No existen particiones lógicas aún
        Espacio libre;
        libre.Inicio = ebr->start - sizeof(EBR); //Inicio donde debería iniciar la pratición lógica después del ebr
        libre.Fin = Extendida.start + Extendida.size - 1;
        libre.Tama = libre.Fin - libre.Inicio + 1;
        libre.InicioAnterior = -1; //Significa que es el primero
        lstEspacios.push_back(libre);
    }
    else
    { //Ya existen particiones lógicas

        while (ebr->next != -1)
        {
            int SizeLogica = ebr->start + ebr->size - sizeof(EBR);
            if (SizeLogica != ebr->next)
            { //Significa que queda un espacio de por medio entre el final de la lógica y el inicio de la otra lógica
                Espacio libre;
                libre.Inicio = SizeLogica;
                libre.Fin = ebr->next - 1;
                libre.Tama = libre.Fin - libre.Inicio + 1;
                libre.InicioAnterior = ebr->start - sizeof(EBR);
                lstEspacios.push_back(libre);
            }
            ebr = ObtenerEBR(path, ebr->next);
        }
        int ultimaPosicion = ebr->start + ebr->size - 1 - sizeof(EBR);
        int limiteExtendida = Extendida.start + Extendida.size - 1;
        if (ultimaPosicion != limiteExtendida)
        {
            Espacio libre;
            libre.Inicio = ultimaPosicion + 1;
            libre.Fin = limiteExtendida;
            libre.Tama = libre.Fin - libre.Inicio + 1;
            libre.InicioAnterior = ebr->start - sizeof(EBR);
            lstEspacios.push_back(libre);
        }
    }
}

//AJUSTES PARA GUARDAR LAS PARTICIONES
void PrimerAjuste(int size, string name, string type, string path)
{
    if (type == "p" || type == "e")
    { //Primarias y Extendidas

        if ((type == "e" && ContadorExtendida == 0) || (type == "p" && ContadorPrimaria != 3))
        {

            CalcularEspaciosLibres(path);
            if (lstEspacios.size() != 0)
            {
                //CODIGO AJUSTE

                struct partition Particion;
                int contador = 0;

                for (struct Espacio x : lstEspacios)
                {
                    if (x.Tama >= size)
                    {
                        strcpy(Particion.status, "u");
                        strcpy(Particion.type, type.c_str());
                        strcpy(Particion.fit, "f");
                        Particion.start = x.Inicio;
                        Particion.size = size;
                        strcpy(Particion.nombre, name.c_str());
                        contador++;
                        break;
                    }
                }

                if (contador != 0)
                {
                    //Guarda la partición en el mbr y se escribe en el archivo
                    mbr->PARTICION[0] = Particion;
                    ActualizarMBR(path);

                    //En caso de ser una partición extendida escribir EBR
                    if (type == "e")
                    {
                        //ESCRIBIR EBR
                        strcpy(ebr->status, "u");
                        strcpy(ebr->fit, "w");
                        ebr->start = mbr->PARTICION[0].start + sizeof(EBR);
                        ebr->next = -1;
                        strcpy(ebr->nombre, "xx");

                        EscribirEBR(path, mbr->PARTICION[0].start);
                        ContadorExtendida++;
                    }
                    else
                    {
                        ContadorPrimaria++;
                    }
                    NombresParticion nombre;
                    nombre.Nombre_Particion = name;
                    nombre.signature = mbr->SIGNATURE;
                    nombre.type_Particion = type;
                    ID_ParticionDisco.push_back(nombre);
                    cout << "Particion Creada!" << endl;
                }
                else
                {
                    cout << "Error, los espacios disponibles no cumplen con el tamaño que requiere la particion!" << endl;
                }
            }
            else
            {
                cout << "Error, No se ha encontrado el espacio suficiente para crear esta particion!" << endl;
            }
        }
        else
        {
            cout << "Error, no cumple con la cantidad permitida de particiones extendidas o primarias!" << endl;
        }
    }
    else if (type == "l")
    { //Logicas
        bool extendida = false;
        int cont = 0;
        for (struct partition p : mbr->PARTICION)
        {
            if (p.type[0] == 101)
            {
                extendida = true;
                CalcularEspaciosLogicas(cont, path);
                if (lstEspacios.size() != 0)
                {
                    int contador = 0;

                    for (struct Espacio x : lstEspacios)
                    {
                        if (x.Tama >= size)
                        {
                            strcpy(ebr->status, "u");
                            strcpy(ebr->fit, "f");
                            ebr->start = x.Inicio + sizeof(EBR);
                            strcpy(ebr->nombre, name.c_str());
                            ebr->size = size;
                            if (x.InicioAnterior != -1)
                            {
                                struct EBR *auxEbr = new struct EBR;
                                auxEbr = ObtenerEBR(path, x.InicioAnterior);
                                ebr->next = auxEbr->next;

                                auxEbr->next = ebr->start - sizeof(EBR);

                                //Actualizo la nueva lógica insertada
                                EscribirEBR(path, x.Inicio);
                                //Actualizo el puntero de la lógica anterior

                                ebr = auxEbr;
                                EscribirEBR(path, x.InicioAnterior);
                            }
                            else
                            {
                                ebr->next = -1;
                                EscribirEBR(path, x.Inicio);
                            }

                            contador++;

                            NombresParticion nombre;
                            nombre.Nombre_Particion = name;
                            nombre.signature = mbr->SIGNATURE;
                            nombre.type_Particion = type;
                            ID_ParticionDisco.push_back(nombre);
                            cout << "Particion Creada!" << endl;
                            break;
                        }
                    }
                    if (contador == 0)
                    {
                        cout << "Error, los espacios disponibles no cumplen con el tamaño que requiere la particion!" << endl;
                    }
                }
                else
                {
                    cout << "Error, No se ha encontrado el espacio suficiente para crear esta particion!" << endl;
                }
                break;
            }
            cont++;
        }
        if (!extendida)
        {
            cout << "Error, no se puede crear ninguna partición lógica sin que exista una partición extendida!" << endl;
        }
    }
    else
    {
        cout << "Error, no se ha reconocido el tipo de particion!" << endl;
    }
}

void MejorAjuste(int size, string name, string type, string path)
{
    if (type == "p" || type == "e")
    { //Primarias y Extendidas

        if ((type == "e" && ContadorExtendida == 0) || (type == "p" && ContadorPrimaria != 3))
        {

            CalcularEspaciosLibres(path);
            if (lstEspacios.size() != 0)
            {
                //CODIGO AJUSTE
                struct Espacio Mejor;
                struct partition Particion;
                Mejor.Tama = mbr->SIZE - 1;

                for (struct Espacio x : lstEspacios)
                {
                    if (x.Tama <= Mejor.Tama && x.Tama >= size)
                    {
                        Mejor = x;
                    }
                }
                if (Mejor.Tama != mbr->SIZE - 1)
                {
                    strcpy(Particion.status, "u");
                    strcpy(Particion.type, type.c_str());
                    strcpy(Particion.fit, "b");
                    Particion.start = Mejor.Inicio;
                    Particion.size = size;
                    strcpy(Particion.nombre, name.c_str());

                    //Actualizar MBR
                    mbr->PARTICION[0] = Particion;
                    ActualizarMBR(path);

                    if (type == "e")
                    {
                        //ESCRIBIR EBR
                        strcpy(ebr->status, "u");
                        strcpy(ebr->fit, "w");
                        ebr->start = mbr->PARTICION[0].start + sizeof(EBR);
                        ebr->next = -1;
                        strcpy(ebr->nombre, "xx");

                        EscribirEBR(path, mbr->PARTICION[0].start);
                        ContadorExtendida++;
                    }
                    else
                    {
                        ContadorPrimaria++;
                    }
                    NombresParticion nombre;
                    nombre.Nombre_Particion = name;
                    nombre.signature = mbr->SIGNATURE;
                    nombre.type_Particion = type;
                    ID_ParticionDisco.push_back(nombre);
                    cout << "Particion Creada!" << endl;
                }
                else
                {
                    cout << "Error, los espacios disponibles no cumplen con el tamaño que requiere la particion!" << endl;
                }
            }
            else
            {
                cout << "Error, No se ha encontrado el espacio suficiente para crear esta particion!" << endl;
            }
        }
        else
        {
            cout << "Error, no cumple con la cantidad permitida de particiones extendidas o primarias!" << endl;
        }
    }
    else if (type == "l")
    { //Logicas
        bool extendida = false;
        int cont = 0;
        for (struct partition p : mbr->PARTICION)
        {
            if (p.type[0] == 101) //"e"=101
            {
                extendida = true;
                CalcularEspaciosLogicas(cont, path);
                if (lstEspacios.size() != 0)
                {
                    struct Espacio Mejor;
                    Mejor.Tama = mbr->SIZE - 1;
                    for (struct Espacio x : lstEspacios)
                    {
                        if (x.Tama <= Mejor.Tama && x.Tama >= size)
                        {
                            Mejor = x;
                        }
                    }

                    if (Mejor.Tama != mbr->SIZE - 1)
                    {
                        strcpy(ebr->status, "u");
                        strcpy(ebr->fit, "b");
                        ebr->start = Mejor.Inicio + sizeof(EBR);
                        strcpy(ebr->nombre, name.c_str());
                        ebr->size = size;

                        if (Mejor.InicioAnterior != -1) //Si no es el primer EBR
                        {
                            struct EBR *auxEbr = new struct EBR;
                            auxEbr = ObtenerEBR(path, Mejor.InicioAnterior);
                            ebr->next = auxEbr->next;

                            auxEbr->next = ebr->start - sizeof(EBR);

                            //Actualizo la nueva lógica insertada
                            EscribirEBR(path, Mejor.Inicio);
                            //Actualizo el puntero de la lógica anterior

                            ebr = auxEbr;
                            EscribirEBR(path, Mejor.InicioAnterior);
                        }
                        else
                        {
                            ebr->next = -1;
                            EscribirEBR(path, Mejor.Inicio);
                        }
                        NombresParticion nombre;
                        nombre.Nombre_Particion = name;
                        nombre.signature = mbr->SIGNATURE;
                        nombre.type_Particion = type;
                        ID_ParticionDisco.push_back(nombre);
                        cout << "Particion Creada!" << endl;
                    }
                    else
                    {
                        cout << "Error, los espacios disponibles no cumplen con el tamaño que requiere la particion!" << endl;
                    }
                }
                else
                {
                    cout << "Error, No se ha encontrado el espacio suficiente para crear esta particion!" << endl;
                }

                break;
            }
            cont++;
        }
        if (!extendida)
        {
            cout << "Error, no se puede crear ninguna partición lógica sin que exista una partición extendida!" << endl;
        }
    }
    else
    {
        cout << "Error, no se ha reconocido el tipo de particion!" << endl;
    }
}

void PeorAjuste(int size, string name, string type, string path)
{
    if (type == "p" || type == "e")
    { //Primarias y Extendidas

        if ((type == "e" && ContadorExtendida == 0) || (type == "p" && ContadorPrimaria != 3))
        {

            CalcularEspaciosLibres(path);
            if (lstEspacios.size() != 0)
            {
                //CODIGO AJUSTE
                struct Espacio Peor;
                struct partition Particion;
                Peor.Tama = 0;

                for (struct Espacio x : lstEspacios)
                {
                    if (x.Tama >= Peor.Tama && x.Tama >= size)
                    {
                        Peor = x;
                    }
                }

                if (Peor.Tama != 0)
                {
                    strcpy(Particion.status, "u");
                    strcpy(Particion.type, type.c_str());
                    strcpy(Particion.fit, "b");
                    Particion.start = Peor.Inicio;
                    Particion.size = size;
                    strcpy(Particion.nombre, name.c_str());

                    //Actualizar MBR
                    mbr->PARTICION[0] = Particion;
                    ActualizarMBR(path);

                    if (type == "e")
                    {
                        //ESCRIBIR EBR
                        strcpy(ebr->status, "u");
                        strcpy(ebr->fit, "w");
                        ebr->start = mbr->PARTICION[0].start + sizeof(EBR); //Inicio particionExtendida+ tamaño EBR= Inicio Particion logica
                        ebr->next = -1;
                        strcpy(ebr->nombre, "xx");

                        EscribirEBR(path, mbr->PARTICION[0].start);
                        ContadorExtendida++;
                    }
                    else
                    {
                        ContadorPrimaria++;
                    }
                    NombresParticion nombre;
                    nombre.Nombre_Particion = name;
                    nombre.signature = mbr->SIGNATURE;
                    nombre.type_Particion = type;
                    ID_ParticionDisco.push_back(nombre);
                    cout << "Particion Creada!" << endl;
                }
                else
                {
                    cout << "Error, los espacios disponibles no cumplen con el tamaño que requiere la particion!" << endl;
                }
            }
            else
            {
                cout << "Error, No se ha encontrado el espacio suficiente para crear esta particion!" << endl;
            }
        }
        else
        {
            cout << "Error, no cumple con la cantidad permitida de particiones extendidas o primarias!" << endl;
        }
    }
    else if (type == "l")
    { //Logicas
        bool extendida = false;
        int cont = 0;
        for (struct partition p : mbr->PARTICION)
        {
            if (p.type[0] == 101)
            {
                extendida = true;
                CalcularEspaciosLogicas(cont, path);
                if (lstEspacios.size() != 0)
                {
                    struct Espacio Peor;
                    Peor.Tama = 0;

                    for (struct Espacio x : lstEspacios)
                    {
                        if (x.Tama >= Peor.Tama && x.Tama >= size)
                        {
                            Peor = x;
                        }
                    }

                    if (Peor.Tama != 0)
                    {
                        strcpy(ebr->status, "u");
                        strcpy(ebr->fit, "w");
                        ebr->start = Peor.Inicio + sizeof(EBR);
                        strcpy(ebr->nombre, name.c_str());
                        ebr->size = size;
                        if (Peor.InicioAnterior != -1)
                        {
                            struct EBR *auxEbr = new struct EBR;
                            auxEbr = ObtenerEBR(path, Peor.InicioAnterior);
                            ebr->next = auxEbr->next;

                            auxEbr->next = ebr->start - sizeof(EBR);

                            //Actualizo la nueva lógica insertada
                            EscribirEBR(path, Peor.Inicio);
                            //Actualizo el puntero de la lógica anterior

                            ebr = auxEbr;
                            EscribirEBR(path, Peor.InicioAnterior);
                        }
                        else
                        {
                            ebr->next = -1;
                            EscribirEBR(path, Peor.Inicio);
                        }
                        NombresParticion nombre;
                        nombre.Nombre_Particion = name;
                        nombre.signature = mbr->SIGNATURE;
                        nombre.type_Particion = type;
                        ID_ParticionDisco.push_back(nombre);
                        cout << "Particion Creada!" << endl;
                    }
                    else
                    {
                        cout << "Error, los espacios disponibles no cumplen con el tamaño que requiere la particion!" << endl;
                    }
                }
                else
                {
                    cout << "Error, No se ha encontrado el espacio suficiente para crear esta particion!" << endl;
                }
                break;
            }
            cont++;
        }
        if (!extendida)
        {
            cout << "Error, no se puede crear ninguna partición lógica sin que exista una partición extendida!" << endl;
        }
    }
    else
    {
        cout << "Error, no se ha reconocido el tipo de particion!" << endl;
    }
}

void CrearParticion(int size, string f, string name, string type, string path) //----fkdisk crear parcion------
{
    if (!VerificarNombre(name))
    {
        if (f == "f")
        { //Primer Ajuste
            PrimerAjuste(size, name, type, path);
        }
        else if (f == "b")
        { //Mejor Ajuste
            MejorAjuste(size, name, type, path);
        }
        else if (f == "w")
        { //Peor Ajuste
            PeorAjuste(size, name, type, path);
        }
    }
    else
    {
        cout << "Error, el nombre de la partición ya existe en este disco!" << endl;
    }
}

void Fast(string name, string path)
{
    int match = 0;
    for (struct NombresParticion x : ID_ParticionDisco)
    {
        if (x.Nombre_Particion == name && x.signature == mbr->SIGNATURE)
        {
            if (x.type_Particion == "p" || x.type_Particion == "e")
            {
                int cont = 0;

                for (struct partition p : mbr->PARTICION)
                {
                    string nombre = p.nombre;
                    if (nombre == name)
                    {

                        strcpy(mbr->PARTICION[cont].fit, "");
                        strcpy(mbr->PARTICION[cont].nombre, "");
                        strcpy(mbr->PARTICION[cont].type, "");
                        mbr->PARTICION[cont].size = 0;
                        mbr->PARTICION[cont].start = 0;
                        strcpy(mbr->PARTICION[cont].status, "");

                        ActualizarMBR(path);

                        string fit = p.fit;
                        if (fit == "e")
                        {
                            ContadorExtendida--;
                        }
                        else if (fit == "p")
                        {
                            ContadorPrimaria--;
                        }
                        cout << "Particion Eliminada!" << endl;
                        break;
                    }
                    cont++;
                }
            }
            else
            {
                bool extendida = false;
                int cont = 0;
                bool eliminado = false;
                for (struct partition p : mbr->PARTICION)
                {
                    if (p.type[0] == 101)
                    {
                        extendida = true;
                        struct partition Extendida = mbr->PARTICION[cont++];
                        ebr = ObtenerEBR(path, Extendida.start);
                        EBR *ebrAux = new struct EBR;
                        do
                        {
                            string nm = ebr->nombre;
                            if (nm == name)
                            {
                                ebrAux->next = ebr->next;
                                strcpy(ebr->status, "");
                                strcpy(ebr->fit, "");
                                int copiaStart = ebr->start - sizeof(EBR);
                                ebr->start = 0;
                                strcpy(ebr->nombre, "");
                                ebr->size = 0;
                                ebr->next = 0;

                                EscribirEBR(path, copiaStart);
                                ebr = ebrAux;
                                int posicion = ebr->start - sizeof(EBR);
                                EscribirEBR(path, posicion);

                                cout << "Particion Eliminada!" << endl;
                                eliminado = true;
                                break;
                            }
                            ebrAux = ebr;
                            if (ebrAux->next != -1)
                            {
                                ebr = ObtenerEBR(path, ebr->next);
                            }
                        } while (ebrAux->next != -1);
                    }

                    cont++;
                    if (eliminado)
                    {
                        break;
                    }
                }
            }

            ID_ParticionDisco.erase(ID_ParticionDisco.begin() + match);
            break;
        }

        match++;
    }
}

void Full(string name, string path)
{
    int match = 0;
    for (struct NombresParticion x : ID_ParticionDisco)
    {
        if (x.Nombre_Particion == name && x.signature == mbr->SIGNATURE)
        {
            if (x.type_Particion == "p" || x.type_Particion == "e")
            {
                int cont = 0;

                for (struct partition p : mbr->PARTICION)
                {
                    string nombre = p.nombre;
                    if (nombre == name)
                    {

                        FILE *file = fopen(path.c_str(), "rb+");
                        rewind(file);
                        fwrite(&mbr, sizeof(MBR), 1, file);
                        int tama = static_cast<int>(mbr->PARTICION[cont].size / 2);
                        fseek(file, mbr->PARTICION[cont].start, 0);
                        fwrite("\0", sizeof("\0"), tama, file);
                        fclose(file);

                        strcpy(mbr->PARTICION[cont].fit, "");
                        strcpy(mbr->PARTICION[cont].nombre, "");
                        strcpy(mbr->PARTICION[cont].type, "");
                        mbr->PARTICION[cont].size = 0;
                        mbr->PARTICION[cont].start = 0;
                        strcpy(mbr->PARTICION[cont].status, "");

                        ActualizarMBR(path);
                        string fit = p.fit;
                        if (fit == "e")
                        {
                            ContadorExtendida--;
                        }
                        else if (fit == "p")
                        {
                            ContadorPrimaria--;
                        }

                        cout << "Particion Eliminada!" << endl;
                        break;
                    }
                    cont++;
                }
            }
            else
            {
                bool extendida = false;
                int cont = 0;
                bool eliminado = false;
                for (struct partition p : mbr->PARTICION)
                {
                    if (p.type[0] == 101)
                    {
                        extendida = true;
                        struct partition Extendida = mbr->PARTICION[cont++];
                        ebr = ObtenerEBR(path, Extendida.start);
                        EBR *ebrAux = new struct EBR;
                        do
                        {
                            string nm = ebr->nombre;
                            if (nm == name)
                            {
                                ebrAux->next = ebr->next;
                                strcpy(ebr->status, "");
                                strcpy(ebr->fit, "");
                                int copiaStart = ebr->start - sizeof(EBR);
                                ebr->start = 0;
                                strcpy(ebr->nombre, "");
                                ebr->size = 0;
                                ebr->next = 0;

                                EscribirEBR(path, copiaStart);
                                ebr = ebrAux;
                                int posicion = ebr->start - sizeof(EBR);
                                FILE *file = fopen(path.c_str(), "rb+");
                                fseek(file, copiaStart, 0);
                                fwrite(&ebr, sizeof(EBR), 1, file);
                                fclose(file);
                                EscribirEBR(path, posicion);

                                cout << "Particion Eliminada!" << endl;
                                eliminado = true;
                                break;
                            }
                            ebrAux = ebr;
                            if (ebrAux->next != -1)
                            {
                                ebr = ObtenerEBR(path, ebr->next);
                            }
                        } while (ebrAux->next != -1);
                    }
                    cont++;
                    if (eliminado)
                    {
                        break;
                    }
                }
            }

            ID_ParticionDisco.erase(ID_ParticionDisco.begin() + match);
            break;
        }

        match++;
    }
}

void EliminarParticion(string path, string name, string tipoEliminacion)
{
    if (VerificarNombre(name))
    {
        if (tipoEliminacion == "fast")
        {
            Fast(name, path);
        }
        else if (tipoEliminacion == "full")
        {
            Full(name, path);
        }
        else
        {
            cout << "Error, no se ha encontrado este tipo de eliminación!" << endl;
        }
    }
    else
    {
        cout << "Error, no existe una partición con este nombre en este disco!" << endl;
    }
}

void Prueba_LlenarParticiones(string path)
{
    PrimerAjuste(200, "Prueba", "p", path);
    mbr->PARTICION[0].size = 300;
    mbr->PARTICION[0].start = 152;
    strcpy(mbr->PARTICION[0].type, "p");

    //PrimerAjuste(200,"Prueba","p", path);
    //mbr->PARTICION[1].size = 100;
    //mbr->PARTICION[1].start = 353;
    //strcpy(mbr->PARTICION[1].type, "p");

    PrimerAjuste(200, "Prueba", "p", path);
    mbr->PARTICION[0].size = 100;
    mbr->PARTICION[0].start = 453;
    strcpy(mbr->PARTICION[2].type, "p");

    PrimerAjuste(200, "Prueba", "e", path);
    mbr->PARTICION[0].size = 2518;
    mbr->PARTICION[0].start = 553;
    strcpy(mbr->PARTICION[3].type, "e");

    PrimerAjuste(200, "Prueba", "p", path);

    //ContadorPrimaria=2;
    //ContadorExtendida=1;
}
//-----------------------------------------------COMANDOS-----------------------------------------------------------------

//MONTAR PARTICIONES
void mount(string path, string name)
{
    mbr = ObtenerMBR(path);
    if (VerificarNombre(name))
    {
        string id=GenerarID(mbr->SIGNATURE,name);
        cout<<id<<endl;
        insertarFinal(listaMount,mbr->SIGNATURE,name,id);
    }else{
        cout<<"Error, No existe ninguna partición con este nombre!"<<endl;
    }
}

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
        //Prueba_LlenarParticiones(path);
        ActualizarContadores(1);
        CrearParticion(size, f, name, type, path);
        ActualizarContadores(2);
    }
    else if (operacion == "add")
    {
        //Extender
        cout << "TE FALTA EXTENDER PARTICIONEEEESSS! NO SE TE OLVIDE!" << endl;
    }
    else if (operacion == "delete")
    {
        //Eliminar
        ActualizarContadores(1);
        EliminarParticion(path, name, deleted);
        ActualizarContadores(2);
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