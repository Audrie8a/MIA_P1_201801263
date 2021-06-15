#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../Estructuras/MBR.h"
#include "../Estructuras/EBR.h"
#include "../Estructuras/SuperBloque.h"
#include "../Estructuras/Usuarios.h"
#include <algorithm>
using namespace std;

#ifndef COMANDO_H
#define COMANDO_H
vector<int> ID_Disco; //Signature

int ContadorPrimaria = 0;
int ContadorExtendida = 0;
int ContadorMount = 0;
int sizeLstMount = 0;

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

//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************
//-------------------------------------------------------DISCOS, PARTICIONES ------------------------------------------
//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************

string Chart_String(string arreglo, int limite)
{
    string Cadena = "";
    for (int i = 0; i < limite; i++)
    {
        Cadena = Cadena + arreglo[i];
    }
    return Cadena;
}

//CREAR ESTRUCTURA NODO
struct nodo //--------------------------------------------------todo usado para el comando Mount
{
    int signature;
    string path;
    string name;
    string id;
    struct nodo *sig;
};

typedef struct nodo *TLista;

TLista listaMount = NULL;

void insertarFinal(TLista &lista, int signature, string name, string id, string path)
{
    TLista t, q = new (struct nodo);
    q->signature = signature;
    q->name = name;
    q->id = id;
    q->path = path;
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
    sizeLstMount++;
    cout << "Particion Montada!" << endl;
}

bool eliminarElemento(TLista &lista, string id)
{
    bool Eliminado = false;
    TLista p, ant;
    p = lista;

    if (lista != nullptr)
    {
        while (p != nullptr)
        {
            string ID = p->id;
            transform(ID.begin(), ID.end(), ID.begin(), ::tolower);
            transform(id.begin(), id.end(), id.begin(), ::tolower);
            if (ID == id)
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
                Eliminado = true;
                cout << "Partición " + id + " desmontada!" << endl;
                break;
            }

            Eliminado = false;
            ant = p;
            p = p->sig;
        }
    }
    else
    {
        cout << "Aún no existen Particiones Montadas!" << endl;
    }

    return Eliminado;
}

TLista ObtenerUltimoNodo(TLista lista)
{
    TLista q = lista;
    TLista aux = q;
    while (q != nullptr)
    {
        aux = q;
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
        string ID = q->id;
        transform(ID.begin(), ID.end(), ID.begin(), ::tolower);
        transform(id.begin(), id.end(), id.begin(), ::tolower);
        if (ID == id)
        {
            existencia = true;
            break;
        }
        q = q->sig;
        i++;
    }
    return existencia;
}

TLista getElemento(TLista lista, string id)
{
    TLista existencia = nullptr;

    TLista q = lista;
    int i = 1;
    while (q != nullptr)
    {
        string ID = q->id;
        transform(ID.begin(), ID.end(), ID.begin(), ::tolower);
        transform(id.begin(), id.end(), id.begin(), ::tolower);
        if (ID == id)
        {
            existencia = q;
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
    aux = q;
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

string ObtenerLetra(string numero)
{
    string abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int cont = 0;
    string id = "63" + numero;
    string letra = "";
    for (int i = 0; i < abc.size(); i++)
    {
        id = "63" + numero + abc[i];
        if (!buscarElemento(listaMount, id))
        {
            letra = abc[i];
            break;
        }
    }

    return letra;
}

string GenerarID(int signature, string name)
{
    ContadorMount = 0;
    string udt = "1234567890";
    string id = "63";
    TLista aux = ParticionesMontadas_Disco(listaMount, signature);
    //Numero de particiones Montadas en el disco

    if (ContadorMount == 0)
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
            int num = atoi(auxNum.c_str()) + 1;
            auxNum = to_string(num);
            id += auxNum + "A";
        }
        else //Si la lista está vacía
        {
            id += "1A";
        }
    }
    else
    {

        string auxNum = "";
        for (int i = 2; i < aux->id.size() - 1; i++)
        {
            auxNum += aux->id[i];
        }
        string Letra = ObtenerLetra(auxNum);
        id += auxNum + Letra;
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

NombresParticion getNombre(string name)
{
    struct NombresParticion verificacion;

    if (ID_ParticionDisco.size() != 0)
    {
        for (struct NombresParticion x : ID_ParticionDisco)
        {
            if (x.Nombre_Particion == name && x.signature == mbr->SIGNATURE)
            {
                //Se repite
                verificacion = x;
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

void ActualizarStatus(string name, string path, string status)
{

    struct partition aux;
    bool existe = false;
    for (int i = 0; i < 4; i++)
    {
        aux = mbr->PARTICION[i];
        string nombre = aux.nombre;
        if (nombre == name)
        {
            string fit = mbr->PARTICION[i].fit;
            string type = mbr->PARTICION[i].type;
            int start = mbr->PARTICION[i].start;
            int size = mbr->PARTICION[i].size;
            string nom = mbr->PARTICION[i].nombre;
            strcpy(mbr->PARTICION[i].status, status.c_str());
            strcpy(mbr->PARTICION[i].fit, fit.c_str());
            strcpy(mbr->PARTICION[i].type, type.c_str());
            strcpy(mbr->PARTICION[i].nombre, nom.c_str());
            mbr->PARTICION[i].start = start;
            mbr->PARTICION[i].size;
            ActualizarMBR(path);
            existe = true;
            break;
        }
    }

    if (!existe)
    { //No está en particiones primarias o extendidas

        bool extendida = false;
        for (int i = 0; i < 4; i++)
        {
            aux = mbr->PARTICION[i];
            if (Chart_String(aux.type, 1) == "e")
            {
                struct partition Ext = aux;
                ebr = ObtenerEBR(path, Ext.start);
                EBR *ebrAux = new struct EBR;

                do
                {
                    string n = ebr->nombre;
                    if (n == name)
                    {
                        string fit = ebr->fit;
                        string nom = ebr->nombre;
                        int start = ebr->start;
                        int next = ebr->next;
                        int size = ebr->size;
                        strcpy(ebr->status, status.c_str());
                        strcpy(ebr->nombre, nom.c_str());
                        strcpy(ebr->fit, fit.c_str());
                        ebr->start = start;
                        ebr->next = next;
                        ebr->size = size;
                        int Inicio = ebr->start - sizeof(EBR);
                        EscribirEBR(path, Inicio);
                        break;
                    }

                    ebrAux = ebr;
                    if (ebrAux->next != -1)
                    {
                        ebr = ObtenerEBR(path, ebr->next);
                    }
                } while (ebrAux->next != -1);
            }
        }
    }
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
                        strcpy(Particion.status, "0");
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
                        strcpy(ebr->status, "0");
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
                            strcpy(ebr->status, "0");
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
                    strcpy(Particion.status, "0");
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
                        strcpy(ebr->status, "0");
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
                        strcpy(ebr->status, "0");
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
                    strcpy(Particion.status, "0");
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
                        strcpy(ebr->status, "0");
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
                        strcpy(ebr->status, "0");
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
                                int inicio = p.start + sizeof(EBR);
                                if (inicio != ebr->start)
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
                                }
                                else
                                {
                                    //ebrAux->next = ebr->next;
                                    //strcpy(ebr->status, "0");
                                    //strcpy(ebr->fit, "w");
                                    //int copiaStart = ebr->start - sizeof(EBR);
                                    //ebr->start = p.start+sizeof(EBR);
                                    //strcpy(ebr->nombre, "xx");
                                    //ebr->size = 0;
                                    //
                                    //EscribirEBR(path, copiaStart);
                                    //ebr = ebrAux;
                                    //int posicion = ebr->start - sizeof(EBR);
                                    //EscribirEBR(path, posicion);
                                    //
                                    //cout << "Particion Eliminada!" << endl;
                                    cout << "Hay errores al eliminar la primera particion logica, falta arreglarlo!" << endl;
                                }
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
                                int inicio = p.start + sizeof(EBR);
                                if (inicio != ebr->start)
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
                                }
                                else
                                {
                                    cout << "Hay errores al eliminar la primera particion logica, falta arreglarlo!" << endl;
                                }
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

//------------------------------------------------------------------GRAFICAS-------------------------------------------------------------------
void Graficar(string ruta, string nombreArchivo, string Contenido)
{
    ofstream file;
    file.open(nombreArchivo);
    file << Contenido;
    file.close();
    string comando = "dot -Tjpg " + nombreArchivo + " -o " + ruta;
    system(comando.c_str());
}

string ReporteMBR(string pathDisco)
{
    string Imprimir = "digraph G {\n";
    Imprimir += "graph [label=\"" + pathDisco + "\", labelloc=t, fontsize=30];\n";

    //GUARDAR PARTE DEL MBR
    Imprimir += "\tnodo[shape=plaintext label=<\n";
    Imprimir += "\t \t    <table>\n";
    Imprimir += "\t \t \t       <tr>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"8\"><b>MBR</b></td>\n";
    Imprimir += "\t \t \t       </tr>\n";
    Imprimir += "\t \t \t        <tr>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"4\"><b>Nombre</b></td>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"4\"><b>Valor</b></td>\n";
    Imprimir += "\t \t \t        </tr>\n";
    Imprimir += "\t \t \t        <tr>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">mbr_tamano</td>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->SIZE) + "</td>\n";
    Imprimir += "\t \t \t        </tr>\n";
    Imprimir += "\t \t \t        <tr>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">mbr_fecha_creacion</td>\n";
    //Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->FECHA) + "</td>";
    char fecha[25];
    time_t current_time = mbr->FECHA;
    ctime(&current_time);
    strcpy(fecha, ctime(&current_time));
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(fecha) + "</td>\n";
    Imprimir += "\t \t \t        </tr>\n";
    Imprimir += "\t \t \t        <tr>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">mbr_disk_signature</td>\n";
    Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->SIGNATURE) + "</td>\n";
    Imprimir += "\t \t \t        </tr>\n";
    Imprimir += "\t \t \t        <tr>\n";
    Imprimir += "\t \t \t \t            <td colspan=\"4\">Disk_fit</td>\n";
    Imprimir += "\t \t \t \t            <td colspan=\"4\">" + string(mbr->FIT) + "</td>\n";
    Imprimir += "\t \t \t        </tr>\n";
    Imprimir += "\t \t \t        \n";
    if (mbr->PARTICION[0].size != 0)
    {
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_1</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[0].status), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_1</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[0].type), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_1</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[0].fit), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_1</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[0].start) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_1</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[0].size) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_1</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[0].nombre) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        \n";
    }
    if (mbr->PARTICION[1].size != 0)
    {
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_2</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[1].status), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_2</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[1].type), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_2</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[1].fit), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_2</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[1].start) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_2</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[1].size) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_2</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[1].nombre) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        \n";
    }
    if (mbr->PARTICION[2].size != 0)
    {
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_3</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[2].status), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_3</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[2].type), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_3</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[2].fit), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_3</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[2].start) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_3</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[2].size) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_3</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[2].nombre) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        \n";
    }
    if (mbr->PARTICION[3].size != 0)
    {
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_status_4</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[3].status), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_type_4</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[3].type), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_fit_4</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + Chart_String(string(mbr->PARTICION[3].fit), 1) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_start_4</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[3].start) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_size_4</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + to_string(mbr->PARTICION[3].size) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t        <tr>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">part_name_4</td>\n";
        Imprimir += "\t \t \t \t           <td colspan=\"4\">" + string(mbr->PARTICION[3].nombre) + "</td>\n";
        Imprimir += "\t \t \t        </tr>\n";
        Imprimir += "\t \t \t    \n";
    }
    Imprimir += "\t \t    </table>\n";
    Imprimir += "\t>];\n";

    //GUARDA LOS DATOS DE LOS EBR EXISTENTES
    int cont = 1;

    for (struct partition p : mbr->PARTICION)
    {
        if (p.type[0] == 101)
        {
            ebr = ObtenerEBR(pathDisco, p.start);
            string nombre = ebr->nombre;
            if (nombre != "xx" && ebr->next != -1)
            {
                EBR *ebrAux = new struct EBR;
                do
                {

                    Imprimir += "nodo" + to_string(cont) + "[shape=plaintext label=<\n";
                    Imprimir += "    <table>\n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"8\"><b>EBR_" + to_string(cont) + "</b></td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"4\"><b>Nombre</b></td>\n";
                    Imprimir += "            <td colspan=\"4\"><b>Valor</b></td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"4\">part_status1</td>\n";
                    Imprimir += "            <td colspan=\"4\">" + Chart_String(string(ebr->status), 1) + "</td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"4\">part_fit_1</td>\n";
                    Imprimir += "            <td colspan=\"4\">" + Chart_String(string(ebr->fit), 1) + "</td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"4\">part_start_1</td>\n";
                    Imprimir += "            <td colspan=\"4\">" + to_string(ebr->start) + "</td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"4\">part_size_1</td>\n";
                    Imprimir += "            <td colspan=\"4\">" + to_string(ebr->size) + "</td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        \n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"4\">part_next_1</td>\n";
                    Imprimir += "            <td colspan=\"4\">" + to_string(ebr->next) + "</td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        <tr>\n";
                    Imprimir += "            <td colspan=\"4\">part_name_1</td>\n";
                    Imprimir += "            <td colspan=\"4\">" + string(ebr->nombre) + "</td>\n";
                    Imprimir += "        </tr>\n";
                    Imprimir += "        \n";
                    Imprimir += "    \n";
                    Imprimir += "    </table>\n";
                    Imprimir += "\n";
                    Imprimir += ">];\n";
                    ebrAux = ebr;
                    if (ebrAux->next != -1)
                    {
                        ebr = ObtenerEBR(pathDisco, ebr->next);
                    }
                    cont++;
                } while (ebrAux->next != -1);
            }
            //else "No existen Lógicas agregadas"
        }
    }
    Imprimir += "}\n";

    return Imprimir;
}

string ReporteDisk(string pathDisco)
{
    string Imprimir = "digraph G {\n";
    Imprimir += "graph [label=\"" + pathDisco + "\", labelloc=t, fontsize=30];\n";
    //Imprimir +="";
    Imprimir += "\t	nodo[shape=plaintext label=<";
    Imprimir += "\t\t	 	    <table>";
    Imprimir += "\t\t\t	 	        <tr>";
    Imprimir += "\t\t\t\t	 	 	 	           <td rowspan =\"4\"><b>MBR</b></td>";

    struct partition p;
    int Inicio = sizeof(MBR);
    //IMPRIMIR PARTICIONES PRIMARIAS Y EXTENDIDAS
    for (int i = 0; i < 4; i++)
    {
        p = mbr->PARTICION[i];
        if (p.size != 0)
        {
            if (Inicio != p.start)
            {
                Imprimir += "\t\t\t\t	 	 	 	           <td rowspan =\"4\" color=\"red\">Libre</td>";
                Inicio = p.start;
                i--;
            }
            else
            {
                string typpe = p.type;
                if (Chart_String(typpe, 1) != "e")
                {
                    Imprimir += "\t\t\t\t	 	 	 	           <td rowspan =\"4\">" + string(p.nombre) + " </td>";
                }
                else
                {
                    Imprimir += "\t\t\t\t	 	 	 	           <td colspan=\"8\">" + string(p.nombre) + "</td>";
                }
                Inicio = Inicio + p.size;
            }
        }
    }
    if (Inicio != mbr->SIZE - 1)
    {
        Imprimir += "\t\t\t\t	 	 	 	           <td rowspan =\"4\" color=\"red\">Libre</td>";
    }

    Imprimir += "\t\t\t	 	 	    </tr>";

    //IMPRIMIR PARTICIONES LOGICAS
    Imprimir += "\t\t\t	 	 	    <tr>";
    Imprimir += "\t\t\t\t	 	 	 	           ";
    for (struct partition p : mbr->PARTICION)
    {
        if (p.type[0] == 101)
        {
            ebr = ObtenerEBR(pathDisco, p.start);
            string nombre = ebr->nombre;
            if (nombre != "xx" && ebr->next != -1)
            {
                EBR *ebrAux = new struct EBR;
                int Start = p.start + sizeof(EBR);
                bool libre = false;
                do
                {
                    if (Start != ebr->start)
                    {
                        Imprimir += "\t\t\t\t	 	 	 	           <td rowspan =\"4\" color=\"red\">Libre</td>";
                        libre = true;
                        Start = ebr->start;
                    }
                    else
                    {
                        Imprimir += "\t\t\t\t	 	 	 	           <td >EBR</td>";
                        Imprimir += "\t\t\t\t	 	 	 	           <td >" + string(ebr->nombre) + "</td>";
                        Start = ebr->start + ebr->size;
                    }
                    ebrAux = ebr;
                    if (ebrAux->next != -1 && libre != true)
                    {
                        ebr = ObtenerEBR(pathDisco, ebr->next);
                    }
                    libre = false;

                } while (ebrAux->next != -1);
            }
            //else "No existen Lógicas agregadas"
        }
    }

    Imprimir += "\t\t\t	 	 	    </tr>";
    Imprimir += "\t\t	 	    </table>";
    Imprimir += "\t	>];";
    Imprimir += "}";
    return Imprimir;
}

//-----------------------------------------------COMANDOS-----------------------------------------------------------------
void repDisk(string path, string id)
{
    if (buscarElemento(listaMount, id))
    { //Verificar que la partición esté montada
        TLista aux = getElemento(listaMount, id);
        mbr = ObtenerMBR(aux->path);
        OrdenarParticiones(aux->path);
        string Contenido = ReporteDisk(aux->path);
        Graficar(path, "disk.dot", Contenido);
        cout << "Grafica Generada!" << endl;
    }
    else
    {
        cout << "Error, no se encontró una partición con este id!" << endl;
    }
}
//REPORTE MBR
void repMBR(string path, string id)
{
    if (buscarElemento(listaMount, id))
    { //Verificar que la partición esté montada
        TLista aux = getElemento(listaMount, id);
        mbr = ObtenerMBR(aux->path);
        OrdenarParticiones(aux->path);
        string Contenido = ReporteMBR(aux->path);
        Graficar(path, "MBR.dot", Contenido);
        cout << "Grafica Generada!" << endl;
    }
    else
    {
        cout << "Error, no se encontró una partición con este id!" << endl;
    }
}

//DESMONTA PARTICIONES
void Unmount(string id)
{
    TLista aux = getElemento(listaMount, id);
    string path = "";
    string name = "";
    if (aux != nullptr)
    {
        path = aux->path;
        name = aux->name;
    }

    if (!eliminarElemento(listaMount, id))
    {
        cout << "Error,No se encontró una partición con este id!" << endl;
    }
    else
    {
        mbr = ObtenerMBR(path);
        ActualizarStatus(name, path, "0");
    }
}

//MONTAR PARTICIONES
void mount(string path, string name)
{
    mbr = ObtenerMBR(path);
    if (VerificarNombre(name))
    {
        string id = GenerarID(mbr->SIGNATURE, name);
        cout << id << endl;
        insertarFinal(listaMount, mbr->SIGNATURE, name, id, path);
        ActualizarStatus(name, path, "1");
    }
    else
    {
        cout << "Error, No existe ninguna partición con este nombre!" << endl;
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

//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************
//--------------------------------------------------------SISTEMA DE ARCHIVOS------------------------------------------
//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************

int numeroEstructuras; //---------------------------------------------------------Aquí comienzan comandos del MKFS
string typeFormat;     //fs
string type;           //full o fast
struct partition ParticionActual;
string pathSA;

struct partition ObtenerParticionPE(string name, string path)
{
    struct partition aux;
    bool existe = false;
    for (int i = 0; i < 4; i++)
    {
        aux = mbr->PARTICION[i];
        string nombre = aux.nombre;
        if (nombre == name)
        {
            existe = true;
            break;
        }
    }

    return aux;
}

int ObrenerN(string tipoSis)
{
    int n = 0;
    if (tipoSis == "2fs")
    {
        n = static_cast<int>((ParticionActual.size - sizeof(SuperBloque)) / (4 + sizeof(TablaInodos) + 3 * sizeof(BloqueArchivo)));
    }
    else
    {
        n = static_cast<int>((ParticionActual.size - sizeof(SuperBloque)) / (4 + sizeof(TablaInodos) + 3 * sizeof(BloqueArchivo) + sizeof(Journaling)));
    }
    return n;
}

void EXT2(SuperBloque super, time_t fecha)
{
    int startParticion = ParticionActual.start;
    int startBI = startParticion + sizeof(SuperBloque);            //Inicio Bitmap Inodos
    int startBB = startBI + numeroEstructuras;                     //Inicio Bitmap Bloques
    int startI = startBB + numeroEstructuras * 3;                  //Inicio Inodos
    int startB = startI + numeroEstructuras * sizeof(TablaInodos); //Inicio Bloques

    super.bm_inode_start = startBI;
    super.bm_block_start = startBB;
    super.inode_start = startI;
    super.block_start = startB;

    struct TablaInodos Inodo;
    Inodo.type = '9';

    struct BloqueCarpeta Carpeta;
    Carpeta.content[0].inodo = 666;

    //Comenzamos a escribir
    FILE *file2 = fopen(pathSA.c_str(), "rb+");

    fseek(file2, startParticion, 0);
    //Escribir Superbloque
    fwrite(&super, sizeof(SuperBloque), 1, file2);

    char aux = '0';
    //Escribir bitmap de inodos
    fseek(file2, startBI, 0);
    for (int i = 0; i < numeroEstructuras; i++)
    {
        fwrite(&aux, sizeof(aux), 1, file2);
    }

    //Escribir bitmap de bloques
    fseek(file2, startBB, 0);
    for (int i = 0; i < 3 * numeroEstructuras; i++)
    {
        fwrite(&aux, sizeof(aux), 1, file2);
    }

    //Escribir Inodos
    fseek(file2, startI, 0);
    for (int i = 0; i < numeroEstructuras; i++)
    {
        fwrite(&Inodo, sizeof(TablaInodos), 1, file2);
    }

    //Escribir Bloques
    fseek(file2, startB, 0);
    for (int i = 0; i < 3 * numeroEstructuras; i++)
    {
        fwrite(&Carpeta, sizeof(BloqueCarpeta), 1, file2);
    }

    fclose(file2);
    cout << "Partición Formateada!" << endl;

    //Crear archivo Users.txt

    struct SuperBloque SB;

    FILE *file = fopen(pathSA.c_str(), "rb");
    fseek(file, startParticion, 0);
    fread(&SB, sizeof(SuperBloque), 1, file);
    fclose(file);

    cout << " " << endl;
    cout << "************************************" << endl;
    cout << "*********DATOS DEL SISTEMA**********" << endl;
    cout << "************************************" << endl;

    if (SB.filesystem_type == 2)
    {
        cout << "Sistema: EXT2" << endl;
    }
    else
    {
        cout << "Sistema: EXT3" << endl;
    }
    cout << "Numero de Inodos: " << SB.inodes_count << endl;
    cout << "Numero de Bloques: " << SB.blocks_count << endl;
    char fechas[25];
    time_t current = SB.Mtime;
    ctime(&current);
    stpcpy(fechas, ctime(&current));
    cout << "Fecha Montado: " << string(fechas) << endl;
    cout << "Start bitmap Bloques: " << SB.block_start << endl;

    struct TablaInodos I;
    I.uid = 1;
    I.gid = 1;
    I.size = 0;
    I.atime = fecha;
    I.ctime = fecha;
    I.mtime = fecha;
    I.block[0] = 0;
    I.type = '0';
    I.perm = 664; //Permisos de lectura y escritura para usuarios y grupos, sólo lectura para ajenos

    struct BloqueCarpeta Root;
    strcpy(Root.content[0].name, ".");
    Root.content[0].inodo = 0;
    strcpy(Root.content[1].name, "..");
    Root.content[1].inodo = 0;

    //Archivo User.txt
    strcpy(Root.content[2].name, "user.txt");
    Root.content[2].inodo = 1;

    struct TablaInodos I0_usuario; //Usuarios son únicos no importando el grupo
    I0_usuario.uid = 1;            //1 Activo, 0 eliminado
    I0_usuario.gid = 1;            //1 Activo, 0 eliminado
    I0_usuario.size = 28;          //Verificar si los saltos de línea cuentan sino son 26
    I0_usuario.atime = fecha;
    I0_usuario.ctime = fecha;
    I0_usuario.mtime = fecha;
    I0_usuario.block[0] = 1;
    I0_usuario.type = '1';
    I0_usuario.perm = 664;

    //Guardando contenido de archivo inicial
    struct BloqueArchivo Archivo_user;
    strcpy(Archivo_user.content, "1,g,root\n1,u,root,root,123\n");

    //Guardando en disco
    FILE *archivo = fopen(pathSA.c_str(), "rb+");
    fseek(archivo, startBI, 0);
    char c = '1';
    fwrite(&c, sizeof(c), 1, archivo);
    fwrite(&c, sizeof(c), 1, archivo);
    fseek(archivo, startBB, 0);
    fwrite(&c, sizeof(c), 1, archivo);
    fwrite(&c, sizeof(c), 1, archivo);

    //Actualizando Tabla Inodos y Bloques
    fseek(archivo, startI, 0);
    fwrite(&I, sizeof(TablaInodos), 1, archivo);
    fwrite(&I0_usuario, sizeof(TablaInodos), 1, archivo);

    fseek(archivo, startB, 0);
    fwrite(&Root, sizeof(BloqueCarpeta), 1, archivo);
    fwrite(&Archivo_user, sizeof(BloqueArchivo), 1, archivo);
    fclose(archivo);
}

void EXT3(SuperBloque super, time_t fecha)
{
    int startParticion = ParticionActual.start;
    int startBI = startParticion + sizeof(SuperBloque) + numeroEstructuras * sizeof(Journaling); //Inicio Bitmap Inodos
    int startBB = startBI + numeroEstructuras;                                                   //Inicio Bitmap Bloques
    int startI = startBB + numeroEstructuras * 3;                                                //Inicio Inodos
    int startB = startI + numeroEstructuras * sizeof(TablaInodos);                               //Inicio Bloques

    super.bm_inode_start = startBI;
    super.bm_block_start = startBB;
    super.inode_start = startI;
    super.block_start = startB;

    struct TablaInodos Inodo;
    Inodo.type = '9';

    struct BloqueCarpeta Carpeta;
    Carpeta.content[0].inodo = 666;

    struct Journaling Journal;
    Journal.tipo = '9';

    //Comenzamos a escribir
    FILE *file2 = fopen(pathSA.c_str(), "rb+");

    fseek(file2, startParticion, 0);
    //Escribir Superbloque
    fwrite(&super, sizeof(SuperBloque), 1, file2);

    //Escribir Journals
    for (int i = 0; i < numeroEstructuras; i++)
    {
        fwrite(&Journal, sizeof(Journaling), 1, file2);
    }

    char aux = '0';
    //Escribir bitmap de inodos
    fseek(file2, startBI, 0);
    for (int i = 0; i < numeroEstructuras; i++)
    {
        fwrite(&aux, sizeof(aux), 1, file2);
    }

    //Escribir bitmap de bloques
    fseek(file2, startBB, 0);
    for (int i = 0; i < 3 * numeroEstructuras; i++)
    {
        fwrite(&aux, sizeof(aux), 1, file2);
    }

    //Escribir Inodos
    fseek(file2, startI, 0);
    for (int i = 0; i < numeroEstructuras; i++)
    {
        fwrite(&Inodo, sizeof(TablaInodos), 1, file2);
    }

    //Escribir Bloques
    fseek(file2, startB, 0);
    for (int i = 0; i < 3 * numeroEstructuras; i++)
    {
        fwrite(&Carpeta, sizeof(BloqueCarpeta), 1, file2);
    }

    fclose(file2);
    cout << "Partición Formateada!" << endl;

    //Crear archivo Users.txt

    struct SuperBloque SB;

    FILE *file = fopen(pathSA.c_str(), "rb");
    fseek(file, startParticion, 0);
    fread(&SB, sizeof(SuperBloque), 1, file);
    fclose(file);
    cout << " " << endl;
    cout << "************************************" << endl;
    cout << "*********DATOS DEL SISTEMA**********" << endl;
    cout << "************************************" << endl;

    if (SB.filesystem_type == 2)
    {
        cout << "Sistema: EXT2" << endl;
    }
    else
    {
        cout << "Sistema: EXT3" << endl;
    }
    cout << "Numero de Inodos: " << SB.inodes_count << endl;
    cout << "Numero de Bloques: " << SB.blocks_count << endl;
    char fechas[25];
    time_t current = SB.Mtime;
    ctime(&current);
    stpcpy(fechas, ctime(&current));
    cout << "Fecha Montado: " << string(fechas) << endl;
    cout << "Start bitmap Bloques: " << SB.block_start << endl;

    struct TablaInodos I;
    I.uid = 1;
    I.gid = 1;
    I.size = 0;
    I.atime = fecha;
    I.ctime = fecha;
    I.mtime = fecha;
    I.block[0] = 0;
    I.type = '0';
    I.perm = 664; //Permisos de lectura y escritura para usuarios y grupos, sólo lectura para ajenos

    struct Journaling JRoot;
    stpcpy(JRoot.contenido, "carpetaRoot");
    strcpy(JRoot.path, "/");
    JRoot.tipo = 0;
    strcpy(JRoot.operacion, "mkdir");
    JRoot.log_fecha = fecha;
    JRoot.InodoAfectado = 0;

    struct BloqueCarpeta Root;
    strcpy(Root.content[0].name, ".");
    Root.content[0].inodo = 0;
    strcpy(Root.content[1].name, "..");
    Root.content[1].inodo = 0;

    //Archivo User.txt
    strcpy(Root.content[2].name, "user.txt");
    Root.content[2].inodo = 1;

    struct TablaInodos I0_usuario; //Usuarios son únicos no importando el grupo
    I0_usuario.uid = 1;            //1 Activo, 0 eliminado
    I0_usuario.gid = 1;            //1 Activo, 0 eliminado
    I0_usuario.size = 0;
    I0_usuario.atime = fecha;
    I0_usuario.ctime = fecha;
    I0_usuario.mtime = fecha;
    I0_usuario.block[0] = 1;
    I0_usuario.type = '1';
    I0_usuario.perm = 664;

    //Guardando contenido de archivo inicial
    struct BloqueArchivo Archivo_user;
    strcpy(Archivo_user.content, "1,g,root\n1,u,root,root,123\n");

    //Guardando en Journaling
    struct Journaling J;
    strcpy(J.contenido, "1,g,root\n1,u,root,root,123\n");
    strcpy(J.path, "/user.txt");
    J.size = 28;
    J.tipo = 1;
    strcpy(J.operacion, "make");
    J.log_fecha = fecha;
    J.InodoAfectado = 1;
    J.size = sizeof(BloqueArchivo);

    //Guardando en disco
    FILE *archivo = fopen(pathSA.c_str(), "rb+");
    fseek(archivo, startBI, 0);
    char c = '1';
    fwrite(&c, sizeof(c), 1, archivo);
    fwrite(&c, sizeof(c), 1, archivo);
    fseek(archivo, startBB, 0);
    fwrite(&c, sizeof(c), 1, archivo);
    fwrite(&c, sizeof(c), 1, archivo);

    //Actualizando Tabla Inodos y Bloques
    fseek(archivo, startI, 0);
    fwrite(&I, sizeof(TablaInodos), 1, archivo);
    fwrite(&I0_usuario, sizeof(TablaInodos), 1, archivo);

    fseek(archivo, startB, 0);
    fwrite(&Root, sizeof(BloqueCarpeta), 1, archivo);
    fwrite(&Archivo_user, sizeof(BloqueArchivo), 1, archivo);

    //Escribiendo Journals
    fseek(archivo, startParticion + sizeof(SuperBloque), 0);
    fwrite(&JRoot, sizeof(Journaling), 1, archivo);
    fwrite(&J, sizeof(Journaling), 1, archivo);
    fclose(archivo);
}

void FormatearPE(string tipoFormatear)
{

    time_t current_time;
    current_time = time(NULL);

    struct SuperBloque super;

    super.inodes_count = numeroEstructuras;
    super.blocks_count = 3 * numeroEstructuras;
    super.free_blocks_count = 3 * numeroEstructuras;
    super.free_inodes_count = numeroEstructuras;
    super.Mtime = current_time;
    super.Umtime = current_time;
    super.mnt_count = 1;

    if (tipoFormatear == "2fs")
    {
        super.filesystem_type = 2;
        //Formatear como Ext2
        EXT2(super, current_time);
    }
    else
    {
        super.filesystem_type = 3;
        //Formatear como Ext3
        EXT3(super, current_time);
    }
}

//---------------------------------------------------------------COMANDOS---------------------------------------------------------------------

void mkfs(string id, string type, string fs)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        typeFormat = fs;
        type = type;
        pathSA = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActual = ObtenerParticionPE(aux->name, aux->path);
                    numeroEstructuras = ObrenerN(fs);
                    FormatearPE(typeFormat);
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************
//--------------------------------------------------------GRUPOS Y USUARIOS--------------------------------------------
//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************

struct SuperBloque *superB = new struct SuperBloque;
struct UsuarioLogin *UsuarioActivo = new struct UsuarioLogin;
string pathSB = "";
struct partition ParticionActualSB;
bool InicioSesion = false; //False no hay sesión iniciada, True ya se loggearon

//OBTIENE LOS DATOS DEL MBR, NECESITA RUTA
struct SuperBloque *ObtenerSuperBloque()
{
    FILE *arch;
    arch = fopen(pathSB.c_str(), "rb");
    struct SuperBloque *newSB = new struct SuperBloque;
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, ParticionActualSB.start, SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(newSB, sizeof(SuperBloque), 1, arch);
    fclose(arch);

    return newSB;
}

struct TablaInodos *ObtenerInodo(int posicion)
{
    struct TablaInodos *auxInodo = new struct TablaInodos;
    FILE *arch = fopen(pathSB.c_str(), "rb");
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, superB->inode_start + (sizeof(TablaInodos) * posicion), 0); //Seek_set o "0", es desede el inicio del archivo
    fread(auxInodo, sizeof(TablaInodos), 1, arch);
    fclose(arch);

    return auxInodo;
}

struct BloqueCarpeta *ObtenerBloqueCarpeta(int posicion)
{
    struct BloqueCarpeta *auxCarpeta = new struct BloqueCarpeta;
    FILE *arch;
    arch = fopen(pathSB.c_str(), "rb");
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, superB->block_start + (sizeof(BloqueCarpeta) * posicion), SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(auxCarpeta, sizeof(BloqueCarpeta), 1, arch);
    fclose(arch);

    return auxCarpeta;
}

struct BloqueApuntadores *ObtenerBloqueApuntador(int posicion)
{
    struct BloqueApuntadores *auxBA = new struct BloqueApuntadores;
    FILE *arch;
    arch = fopen(pathSB.c_str(), "rb");
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, superB->block_start + (sizeof(BloqueApuntadores) * posicion), SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(auxBA, sizeof(BloqueApuntadores), 1, arch);
    fclose(arch);

    return auxBA;
}

struct BloqueArchivo *ObtenerBloqueArchivo(int posicion)
{
    struct BloqueArchivo *auxBA = new struct BloqueArchivo;
    FILE *arch;
    arch = fopen(pathSB.c_str(), "rb");
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, superB->block_start + (sizeof(BloqueArchivo) * posicion), SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(auxBA, sizeof(BloqueArchivo), 1, arch);
    fclose(arch);

    return auxBA;
}

void ActualizarBitMapInodos(int posicion, char dato)
{
    FILE *arch = fopen(pathSB.c_str(), "rb+");
    fseek(arch, superB->bm_inode_start + (sizeof(dato) * posicion), 0);
    fwrite(&dato, sizeof(dato), 1, arch);
    fclose(arch);
}

void ActualizarBitMapBloques(int posicion, char dato)
{
    FILE *arch = fopen(pathSB.c_str(), "rb+");
    fseek(arch, superB->bm_block_start + (sizeof(dato) * posicion), 0);
    fwrite(&dato, sizeof(dato), 1, arch);
    fclose(arch);
}

string ReporteSB()
{
    char fecha[25];
    string Imprimir = "digraph g{\n";

    Imprimir += "\t     nodo [ shape=plaintext label=<\n";
    Imprimir += "\t \t        <table border='1' cellborder='1'>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t               <td colspan=\"2\" >Super Bloque</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t               <td>s_inodes_count</td>\n";
    Imprimir += "\t \t \t \t               <td>" + to_string(superB->inodes_count) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t               <td>s_blocks_count</td>\n";
    Imprimir += "\t \t \t \t               <td>" + to_string(superB->blocks_count) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t               <td>s_free_blocks_count</td>\n";
    Imprimir += "\t \t \t \t               <td>" + to_string(superB->free_blocks_count) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_free_inodes_count</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->free_inodes_count) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";

    time_t current_time = superB->Mtime;
    ctime(&current_time);
    strcpy(fecha, ctime(&current_time));
    Imprimir += "\t \t \t \t                <td>s_mtime</td>\n";
    Imprimir += "\t \t \t \t                <td>" + string(fecha) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    current_time = superB->Umtime;
    ctime(&current_time);
    strcpy(fecha, ctime(&current_time));
    Imprimir += "\t \t \t \t                <td>s_umtime</td>\n";
    Imprimir += "\t \t \t \t                <td>" + string(fecha) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_mnt_count</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->mnt_count) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_magic</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->magic) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_inode_size</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->inode_size) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_block_size</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->block_size) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_first_ino</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->fist_ino) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_first_blo</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->first_blo) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_bm_inode_start</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->bm_inode_start) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_bm_block_start</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->block_start) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_inode_start</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->inode_start) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t \t            <tr>\n";
    Imprimir += "\t \t \t \t                <td>s_block_start</td>\n";
    Imprimir += "\t \t \t \t                <td>" + to_string(superB->block_start) + "</td>\n";
    Imprimir += "\t \t \t            </tr>\n";
    Imprimir += "\t \t        </table>\n";
    Imprimir += "\t     >];\n";
    Imprimir += "}\n";
    return Imprimir;
}

string ReporteArbol()
{
    char fecha[25];
    string Imprimir = "digraph g{\n";
    Imprimir += "rankdir = LR;\n";
    char bit = 'x';
    FILE *file = fopen(pathSB.c_str(), "rb");
    fseek(file, superB->bm_inode_start, 0);

    for (int i = 0; i < superB->inodes_count; i++)
    {
        fread(&bit, sizeof(bit), 1, file);
        if (bit == '1')
        {
            struct TablaInodos *inodo = new struct TablaInodos;
            inodo = ObtenerInodo(i);
            Imprimir += "inodo" + to_string(i) + " [ shape=plaintext label=<";
            Imprimir += "<table border='1' cellborder='1'>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td colspan=\"2\" bgcolor=\"blue\">Inodo " + to_string(i) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td colspan=\"2\" bgcolor=\"blue\">Inodo " + to_string(i) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>UID</td>\n";
            Imprimir += "<td>" + to_string(inodo->uid) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>GID</td>\n";
            Imprimir += "<td>" + to_string(inodo->gid) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Size</td>\n";
            Imprimir += "<td>" + to_string(inodo->size) + "</td>\n";
            Imprimir += "</tr>\n";
            time_t current_time = inodo->atime;
            ctime(&current_time);
            strcpy(fecha, ctime(&current_time));

            Imprimir += "<tr>\n";
            Imprimir += "<td>ATime</td>\n";
            Imprimir += "<td>" + string(fecha) + "</td>\n";
            Imprimir += "</tr>\n";

            current_time = inodo->ctime;
            ctime(&current_time);
            strcpy(fecha, ctime(&current_time));
            Imprimir += "<tr>\n";
            Imprimir += "<td>CTime</td>\n";
            Imprimir += "<td>" + string(fecha) + "</td>\n";
            Imprimir += "</tr>\n";
            current_time = inodo->mtime;
            ctime(&current_time);
            strcpy(fecha, ctime(&current_time));
            Imprimir += "<tr>\n";
            Imprimir += "<td>Mtime</td>\n";
            Imprimir += "<td>" + string(fecha) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block0</td>\n";
            Imprimir += "<td port='b0'>" + to_string(inodo->block[0]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block1</td>\n";
            Imprimir += "<td port='b1'>" + to_string(inodo->block[1]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block2</td>\n";
            Imprimir += "<td port='b2'>" + to_string(inodo->block[2]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block3</td>\n";
            Imprimir += "<td port='b3'>" + to_string(inodo->block[3]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block4</td>\n";
            Imprimir += "<td port='b4'>" + to_string(inodo->block[4]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block5</td>\n";
            Imprimir += "<td port='b5'>" + to_string(inodo->block[5]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block6</td>\n";
            Imprimir += "<td port='b6'>" + to_string(inodo->block[6]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block7</td><td port='b7'>" + to_string(inodo->block[7]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block8</td><td port='b8'>" + to_string(inodo->block[8]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block9</td>\n";
            Imprimir += "<td port='b9'>" + to_string(inodo->block[9]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block10</td>\n";
            Imprimir += "<td port='b10'>" + to_string(inodo->block[10]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block11</td>\n";
            Imprimir += "<td port='b11'>" + to_string(inodo->block[11]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block12</td>\n";
            Imprimir += "<td port='b12'>" + to_string(inodo->block[12]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block13</td>\n";
            Imprimir += "<td port='b13'>" + to_string(inodo->block[13]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block14</td>\n";
            Imprimir += "<td port='b14'>" + to_string(inodo->block[14]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Type</td>\n";
            Imprimir += "<td>" + string(1, inodo->type) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Permisos</td>\n";
            Imprimir += "<td>" + to_string(inodo->perm) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "</table>\n";
            Imprimir += ">];\n";

            if (string(1, inodo->type) == "0")
            { //Si es una carpeta
                for (int j = 0; j < 15; j++)
                {
                    if (inodo->block[j] != -1)
                    {
                        Imprimir += "inodo" + to_string(i) + ":b" + to_string(j) + "->bloque" + to_string(inodo->block[j]) + "\n";
                        struct BloqueCarpeta *block = new struct BloqueCarpeta;
                        for (int f = 0; f < 4; f++)
                        {
                            if (block->content[f].inodo == -1)
                            {
                                strcpy(block->content[f].name, "--");
                            }
                        }
                        Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                        Imprimir += "<table border='1' cellborder='1'>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td colspan=\"2\" bgcolor=\"green\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(block->content[0].name) + "</td>\n";
                        Imprimir += "<td port='b0'>" + to_string(block->content[0].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(block->content[1].name) + "</td>\n";
                        Imprimir += "<td port='b1'>" + to_string(block->content[1].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(block->content[2].name) + "</td>\n";
                        Imprimir += "<td port='b2'>" + to_string(block->content[2].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(block->content[3].name) + "</td>\n";
                        Imprimir += "<td port='b3'>" + to_string(block->content[3].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "</table>\n";
                        Imprimir += ">];\n";

                        for (int k = 0; k < 4; k++)
                        {
                            if (block->content[k].inodo != -1)
                            {
                                string nombre = (block->content[k].name);
                                if (!((nombre == ".") || (nombre == "..")))
                                {
                                    Imprimir += "bloque" + to_string(inodo->block[j]) + ":b" + to_string(k) + " -> inodo" + to_string(block->content[k].inodo) + ";\n";
                                }
                            }
                        }

                        if (j > 11)
                        { //Bloques Indirectos
                            if (j == 12)
                            {
                                Imprimir += "inodo" + to_string(i) + ":b" + to_string(j) + " -> bloque" + to_string(inodo->block[j]) + "\n";

                                struct BloqueApuntadores *apuntador = ObtenerBloqueApuntador(inodo->block[j]);
                                Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                                Imprimir += "<table border='1' cellborder='1'>\n";
                                Imprimir += "<tr>\n";
                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                Imprimir += "</tr>\n";
                                for (int l = 0; l < 16; l++)
                                {
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<td port='b" + to_string(l) + "'>" + to_string(apuntador->b_pointers[l]) + "</td>\n";
                                    Imprimir += "</tr>\n";
                                }
                                Imprimir += "</table>\n";
                                Imprimir += " >];\n";

                                for (int l = 0; l < 16; l++)
                                {
                                    if (apuntador->b_pointers[l] != -1)
                                    {
                                        Imprimir += "bloque" + to_string(inodo->block[j]) + ":b" + to_string(l) + " -> bloque" + to_string(apuntador->b_pointers[l]) + "\n";
                                        BloqueCarpeta *block2 = new struct BloqueCarpeta;
                                        for (int f = 0; f < 4; f++)
                                        {
                                            if (block2->content[f].inodo == -1)
                                            {
                                                strcpy(block2->content[f].name, "--");
                                            }
                                        }
                                        block2 = ObtenerBloqueCarpeta(apuntador->b_pointers[l]);
                                        Imprimir += "bloque" + to_string(apuntador->b_pointers[l]) + " [ shape=plaintext label=<\n";
                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td colspan=\"2\" bgcolor=\"green\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td>" + string(block2->content[0].name) + "</td>\n";
                                        Imprimir += "<td port='b0'>" + to_string(block2->content[0].inodo) + "</td>\n";
                                        Imprimir += "</tr>\n";

                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td>" + string(block2->content[1].name) + "</td>\n";
                                        Imprimir += "<td port='b1'>" + to_string(block2->content[1].inodo) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td>" + string(block2->content[2].name) + "</td>\n";
                                        Imprimir += "<td port='b2'>" + to_string(block2->content[2].inodo) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td>" + string(block2->content[3].name) + "</td>\n";
                                        Imprimir += "<td port='b3'>" + to_string(block2->content[3].inodo) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "</table>\n";
                                        Imprimir += ">];\n";
                                        for (int m = 0; m < 4; m++)
                                        {
                                            if (block2->content[m].inodo != -1)
                                            {
                                                string nombre2 = block2->content[m].name;
                                                if (!((nombre2 == ".") || (nombre2 == "..")))
                                                {
                                                    Imprimir += "bloque" + to_string(apuntador->b_pointers[l]) + ":b" + to_string(m) + " -> inodo" + to_string(block2->content[m].inodo) + ";\n";
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            { //Si es un archivo
                for (int j = 0; j < 15; j++)
                {
                    if (inodo->block[j] != -1)
                    {
                        if (j < 12)
                        {
                            Imprimir += "inodo" + to_string(i) + ":b" + to_string(j) + " -> bloque" + to_string(inodo->block[j]) + "\n";
                            //Metodo para graficar el bloque de archivo
                            struct BloqueArchivo *archivo = new struct BloqueArchivo;
                            archivo = ObtenerBloqueArchivo(inodo->block[j]);
                            Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                            Imprimir += "<table border='1' cellborder='1'>\n";
                            Imprimir += "<tr>\n";
                            Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                            Imprimir += "</tr>\n";
                            Imprimir += "<tr>\n";
                            Imprimir += "<td>" + string(archivo->content) + "</td>\n";
                            Imprimir += "</tr>\n";
                            Imprimir += "</table>\n";
                            Imprimir += " >];\n";
                        }
                        if (j > 11)
                        {
                            if (j == 12)
                            { //Bloque indirecto simple
                                Imprimir += "inodo" + to_string(i) + ":b" + to_string(j) + " -> bloque" + to_string(inodo->block[j]) + "\n";

                                struct BloqueApuntadores *apuntador = new struct BloqueApuntadores;
                                apuntador = ObtenerBloqueApuntador(inodo->block[j]);
                                Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                                Imprimir += "<table border='1' cellborder='1'>\n";
                                Imprimir += "<tr>\n";
                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                Imprimir += "</tr>\n";
                                for (int h = 0; h < 16; h++)
                                {
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<td port='b" + to_string(h) + "'>" + to_string(apuntador->b_pointers[h]) + "</td>\n";
                                    Imprimir += "</tr>\n";
                                }
                                Imprimir += "</table>\n";
                                Imprimir += " >];\n";
                                for (int h = 0; h < 16; h++)
                                {
                                    if (apuntador->b_pointers[h] != -1)
                                    {
                                        Imprimir += "bloque" + to_string(inodo->block[j]) + ":b" + to_string(h) + " -> bloque" + to_string(apuntador->b_pointers[h]) + "\n";
                                        struct BloqueArchivo *arch = new struct BloqueArchivo;
                                        arch = ObtenerBloqueArchivo(apuntador->b_pointers[h]);
                                        Imprimir += "bloque" + to_string(apuntador->b_pointers[h]) + " [ shape=plaintext label=<\n";
                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(apuntador->b_pointers[h]) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td>" + string(arch->content) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "</table>\n";
                                        Imprimir += " >];\n";
                                    }
                                }
                            }
                            else if (j == 13)
                            { //Bloque indirecto doble
                                Imprimir += "inodo" + to_string(i) + ":b" + to_string(j) + " -> bloque" + to_string(inodo->block[j]) + "\n";
                                struct BloqueApuntadores *apuntador = new struct BloqueApuntadores;
                                apuntador = ObtenerBloqueApuntador(inodo->block[j]);
                                Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                                Imprimir += "<table border='1' cellborder='1'>\n";
                                Imprimir += "<tr>\n";
                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                Imprimir += "</tr>\n";
                                for (int h = 0; h < 16; h++)
                                {
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<td port='b" + to_string(h) + "'>" + to_string(apuntador->b_pointers[h]) + "</td>\n";
                                    Imprimir += "</tr>\n";
                                }
                                Imprimir += "</table>\n";
                                Imprimir += " >];\n";
                                for (int h = 0; h < 16; h++)
                                {
                                    if (apuntador->b_pointers[h] != -1)
                                    {
                                        Imprimir += "bloque" + to_string(inodo->block[j]) + ":b" + to_string(h) + " -> bloque" + to_string(apuntador->b_pointers[h]) + "\n";
                                        struct BloqueApuntadores *apt = new struct BloqueApuntadores;
                                        apt = ObtenerBloqueApuntador(inodo->block[j]);
                                        Imprimir += "bloque" + to_string(apuntador->b_pointers[h]) + " [ shape=plaintext label=<\n";
                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(apuntador->b_pointers[h]) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        for (int k = 0; k < 16; k++)
                                        {
                                            Imprimir += "<tr>\n";
                                            Imprimir += "<td port='b" + to_string(k) + "'>" + to_string(apt->b_pointers[k]) + "</td>\n";
                                            Imprimir += "</tr>\n";
                                        }
                                        Imprimir += "</table>\n";
                                        Imprimir += " >];\n";
                                        for (int k = 0; k < 16; k++)
                                        {
                                            if (apt->b_pointers[k] != -1)
                                            {
                                                //Escribiremos los bloques de archivos
                                                Imprimir += "bloque" + to_string(apuntador->b_pointers[h]) + ":b" + to_string(k) + " -> bloque" + to_string(apt->b_pointers[k]) + "\n";
                                                struct BloqueArchivo *arch = new struct BloqueArchivo;
                                                arch = ObtenerBloqueArchivo(apt->b_pointers[k]);
                                                Imprimir += "bloque" + to_string(apt->b_pointers[k]) + " [ shape=plaintext label=<\n";
                                                Imprimir += "<table border='1' cellborder='1'>\n";
                                                Imprimir += "<tr>\n";
                                                Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(apt->b_pointers[k]) + "</td>\n";
                                                Imprimir += "</tr>\n";

                                                Imprimir += "<tr>\n";
                                                Imprimir += "<td>" + string(arch->content) + "</td>\n";
                                                Imprimir += "</tr>\n";
                                                Imprimir += "</table>\n";
                                                Imprimir += " >];\n";
                                            }
                                        }
                                    }
                                }
                            }
                            else if (j == 14)
                            { //Bloque indirecto triple
                                Imprimir += "inodo" + to_string(i) + ":b" + to_string(j) + " -> bloque" + to_string(inodo->block[j]) + "\n";
                                struct BloqueApuntadores *apuntador = new struct BloqueApuntadores;
                                apuntador = ObtenerBloqueApuntador(inodo->block[j]);
                                Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                                Imprimir += "<table border='1' cellborder='1'>\n";
                                Imprimir += "<tr>\n";
                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                Imprimir += "</tr>\n";
                                for (int h = 0; h < 16; h++)
                                {
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<td port='b" + to_string(h) + "'>" + to_string(apuntador->b_pointers[h]) + "</td>\n";
                                    Imprimir += "</tr>\n";
                                }
                                Imprimir += "</table>\n";
                                Imprimir += " >];\n";
                                for (int h = 0; h < 16; h++)
                                {
                                    if (apuntador->b_pointers[h] != -1)
                                    {
                                        Imprimir += "bloque" + to_string(inodo->block[j]) + ":b" + to_string(h) + " -> bloque" + to_string(apuntador->b_pointers[h]) + "\n";
                                        struct BloqueApuntadores *apt = new struct BloqueApuntadores;
                                        apt = ObtenerBloqueApuntador(inodo->block[j]);
                                        Imprimir += "bloque" + to_string(apuntador->b_pointers[h]) + " [ shape=plaintext label=<\n";
                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(apuntador->b_pointers[h]) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        for (int k = 0; k < 16; k++)
                                        {
                                            Imprimir += "<tr>\n";
                                            Imprimir += "<td port='b" + to_string(k) + "'>" + to_string(apt->b_pointers[k]) + "</td>\n";
                                            Imprimir += "</tr>\n";
                                        }
                                        Imprimir += "</table>\n";
                                        Imprimir += " >];\n";
                                        for (int k = 0; k < 16; k++)
                                        {
                                            if (apt->b_pointers[k] != -1)
                                            {
                                                //Escribiremos los bloques de archivos
                                                Imprimir += "bloque" + to_string(apuntador->b_pointers[h]) + ":b" + to_string(k) + " -> bloque" + to_string(apt->b_pointers[k]) + "\n";
                                                struct BloqueApuntadores *apt3 = new struct BloqueApuntadores;
                                                apt = ObtenerBloqueApuntador(apt->b_pointers[k]);
                                                Imprimir += "bloque" + to_string(apt->b_pointers[k]) + " [ shape=plaintext label=<\n";
                                                Imprimir += "<table border='1' cellborder='1'>\n";
                                                Imprimir += "<tr>\n";
                                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(apt->b_pointers[k]) + "</td>\n";
                                                Imprimir += "</tr>\n";
                                                for (int l = 0; l < 16; l++)
                                                {
                                                    Imprimir += "<tr>\n";
                                                    Imprimir += "<td port='b" + to_string(l) + "'>" + to_string(apt3->b_pointers[l]) + "</td>\n";
                                                    Imprimir += "</tr>\n";
                                                }
                                                Imprimir += "</table>\n";
                                                Imprimir += " >];\n";
                                                for (int l = 0; l < 16; l++)
                                                {
                                                    if (apt3->b_pointers[l] != -1)
                                                    {
                                                        Imprimir += "bloque" + to_string(apt->b_pointers[k]) + ":b" + to_string(l) + " -> bloque" + to_string(apt3->b_pointers[l]) + "\n";
                                                        struct BloqueArchivo *arch = new struct BloqueArchivo;
                                                        arch = ObtenerBloqueArchivo(apt3->b_pointers[l]);
                                                        Imprimir += "bloque" + to_string(apt3->b_pointers[l]) + " [ shape=plaintext label=<\n";
                                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                                        Imprimir += "<tr>\n";
                                                        Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(apt3->b_pointers[l]) + "</td>\n";
                                                        Imprimir += "</tr>\n";

                                                        Imprimir += "<tr>\n";
                                                        Imprimir += "<td>" + string(arch->content) + "</td>\n";
                                                        Imprimir += "</tr>\n";
                                                        Imprimir += "</table>\n";
                                                        Imprimir += " >];\n";
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Imprimir += "}";
    return Imprimir;
}

string ReporteInode()
{
    string Imprimir = "digraph g{\n";
    Imprimir += "rankdir = LR;\n";
    char fecha[25];
    char aux = 'a';

    FILE *file = fopen(pathSB.c_str(), "rb");
    fseek(file, superB->bm_inode_start, 0);
    int ant = -1;

    for (int i = 0; i < superB->inodes_count; i++)
    {
        fread(&aux, sizeof(aux), 1, file);
        if (aux == '1')
        {
            struct TablaInodos *inodo = new struct TablaInodos;
            inodo = ObtenerInodo(i);
            Imprimir += "inodo" + to_string(i) + " [ shape=plaintext label=<\n";
            Imprimir += "<table border='1' cellborder ='1'>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td colspan=\"2\" bgcolor=\"skyblue\">Inodo" + to_string(i) + " </td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>UID</td>\n";
            Imprimir += "<td>" + to_string(inodo->uid) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>GID</td>\n";
            Imprimir += "<td>" + to_string(inodo->gid) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Size</td>\n";
            Imprimir += "<td>" + to_string(inodo->size) + "</td>\n";
            Imprimir += "</tr>\n";
            time_t current = inodo->atime;
            ctime(&current);
            strcpy(fecha, ctime(&current));
            Imprimir += "<tr>\n";
            Imprimir += "<td>ATime</td>\n";
            Imprimir += "<td>" + string(fecha) + "</td>\n";
            Imprimir += "</tr>\n";
            current = inodo->ctime;
            ctime(&current);
            strcpy(fecha, ctime(&current));
            Imprimir += "<tr>\n";
            Imprimir += "<td>CTime</td>\n";
            Imprimir += "<td>" + string(fecha) + "</td>\n";
            Imprimir += "</tr>\n";
            current = inodo->mtime;
            ctime(&current);
            strcpy(fecha, ctime(&current));
            Imprimir += "<tr>\n";
            Imprimir += "<td>MTime</td>\n";
            Imprimir += "<td>" + string(fecha) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block0</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[0]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block1</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[1]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block2</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[2]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block3</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[3]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block4</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[4]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block5</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[5]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block6</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[6]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block7</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[7]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block8</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[8]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block9</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[9]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block10</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[10]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block11</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[11]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block12</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[12]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block13</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[13]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Block14</td>\n";
            Imprimir += "<td>" + to_string(inodo->block[14]) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Type</td>\n";
            Imprimir += "<td>" + string(1, inodo->type) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "<tr>\n";
            Imprimir += "<td>Permisos</td>\n";
            Imprimir += "<td>" + to_string(inodo->perm) + "</td>\n";
            Imprimir += "</tr>\n";
            Imprimir += "</table>\n";
            Imprimir += ">];\n";

            if (ant != -1)
            {
                Imprimir += "inodo" + to_string(ant) + " -> inodo" + to_string(i) + "\n";
            }
            ant = i;
        }
    }
    Imprimir += "}";
    fclose(file);
    return Imprimir;
}

string ReporteBlock()
{
    string Imprimir = "digraph g{\n";
    Imprimir += "rankdir = LR;\n";
    char fecha[25];
    char aux = 'a';

    FILE *file = fopen(pathSB.c_str(), "rb");
    fseek(file, superB->bm_inode_start, 0);
    int ant = -1;

    for (int i = 0; i < superB->inodes_count; i++)
    {
        fread(&aux, sizeof(aux), 1, file);
        if (aux == '1')
        {
            struct TablaInodos *inodo = ObtenerInodo(i);

            if (inodo->type == '0')
            {
                for (int j = 0; j < 15; j++)
                {
                    if (inodo->block[j] != -1)
                    {
                        struct BloqueCarpeta *BC = ObtenerBloqueCarpeta(inodo->block[j]);
                        for (int f = 0; f < 4; f++)
                        {
                            if (BC->content[f].inodo == -1)
                            {
                                strcpy(BC->content[f].name, "--");
                            }
                        }
                        Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                        Imprimir += "<table border='1' cellborder='1'>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td colspan=\"2\" bgcolor=\"green\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(BC->content[0].name) + "</td>\n";
                        Imprimir += "<td port='b0'>" + to_string(BC->content[0].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(BC->content[1].name) + "</td>\n";
                        Imprimir += "<td port='b1'>" + to_string(BC->content[1].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(BC->content[2].name) + "</td>\n";
                        Imprimir += "<td port='b2'>" + to_string(BC->content[2].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "<tr>\n";
                        Imprimir += "<td>" + string(BC->content[3].name) + "</td>\n";
                        Imprimir += "<td port='b3'>" + to_string(BC->content[3].inodo) + "</td>\n";
                        Imprimir += "</tr>\n";
                        Imprimir += "</table>\n";
                        Imprimir += ">];\n";

                        if (ant != -1)
                        {
                            Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(inodo->block[j]) + "\n";
                        }
                        ant = inodo->block[j];
                    }
                }
            }
            else
            {
                for (int j = 0; j < 15; j++)
                {
                    if (inodo->block[j] != -1)
                    {
                        if (j < 12)
                        {
                            BloqueArchivo *archivo = ObtenerBloqueArchivo(inodo->block[j]);
                            Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                            Imprimir += "<table border='1' cellborder='1'>\n";
                            Imprimir += "<tr>\n";
                            Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                            Imprimir += "</tr>\n";
                            Imprimir += "<tr>\n";
                            Imprimir += "<td>" + string(archivo->content) + "</td>\n";
                            Imprimir += "</tr>\n";
                            Imprimir += "</table>\n";
                            Imprimir += ">];\n";

                            if (ant != -1)
                            {
                                Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(inodo->block[j]) + "\n";
                            }
                            ant = inodo->block[j];
                        }
                        if (j > 11)
                        {
                            if (j == 12)
                            {
                                struct BloqueApuntadores *apuntadores = ObtenerBloqueApuntador(inodo->block[j]);
                                Imprimir += "bloque" + to_string(inodo->block[j]) + "[ shape=plaintext label=<\n";
                                Imprimir += "<table border='1' cellborder='1'>\n";
                                Imprimir += "<tr>\n";
                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                Imprimir += "</tr>\n";

                                for (int h = 0; h < 16; h++)
                                {
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<tr><td port='b" + to_string(h) + "'>" + to_string(apuntadores->b_pointers[h]) + "</td></tr>\n";
                                    Imprimir += "</tr>\n";
                                }
                                Imprimir += "</table>\n";
                                Imprimir += ">];\n";

                                if (ant != -1)
                                {
                                    Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(inodo->block[j]) + "\n";
                                }

                                ant = inodo->block[j];
                                for (int h = 0; h < 16; h++)
                                {
                                    if (apuntadores->b_pointers[h] != -1)
                                    {
                                        struct BloqueArchivo *Archs = ObtenerBloqueArchivo(apuntadores->b_pointers[h]);
                                        Imprimir += "bloque" + to_string(apuntadores->b_pointers[h]) + " [ shape=plaintext label=<\n";
                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(apuntadores->b_pointers[h]) + "</td>";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td>" + string(Archs->content) + "</td></tr>\n";
                                        Imprimir += "</tr>\n";
                                        Imprimir += "</table>\n";
                                        Imprimir += ">];\n";

                                        if (ant != -1)
                                        {
                                            Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(apuntadores->b_pointers[h]) + "\n";
                                        }
                                        ant = apuntadores->b_pointers[h];
                                    }
                                }
                            }
                            else if (j == 13)
                            {
                                struct BloqueApuntadores *apuntadores = ObtenerBloqueApuntador(inodo->block[j]);
                                Imprimir += "bloque" + to_string(inodo->block[j]) + "[ shape=plaintext label=<\n";
                                Imprimir += "<table border='1' cellborder='1'>\n";
                                Imprimir += "<tr>\n";
                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                Imprimir += "</tr>\n";

                                for (int h = 0; h < 16; h++)
                                {
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<td port='b" + to_string(h) + "'>" + to_string(apuntadores->b_pointers[h]) + "</td>\n";
                                    Imprimir += "</tr>\n";
                                    Imprimir += "</tr>\n";
                                }
                                Imprimir += "</table>\n";
                                Imprimir += ">];\n";

                                if (ant != -1)
                                {
                                    Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(inodo->block[j]) + "\n";
                                }

                                ant = inodo->block[j];
                                for (int h = 0; h < 16; h++)
                                {
                                    if (apuntadores->b_pointers[h] != -1)
                                    {
                                        struct BloqueApuntadores *apt2 = ObtenerBloqueApuntador(apuntadores->b_pointers[h]);
                                        Imprimir += "bloque" + to_string(apuntadores->b_pointers[h]) + " [ shape=plaintext label=<\n";
                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(apuntadores->b_pointers[h]) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        for (int k = 0; k < 16; k++)
                                        {
                                            Imprimir += "<tr>\n";
                                            Imprimir += "<td port='b" + to_string(k) + "'>" + to_string(apt2->b_pointers[k]) + "</td>\n";
                                            Imprimir += "</tr>\n";
                                        }
                                        Imprimir += "</table>\n";
                                        Imprimir += " >];\n";
                                        if (ant != -1)
                                        {
                                            Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(apuntadores->b_pointers[h]) + "\n";
                                        }
                                        ant = apuntadores->b_pointers[h];
                                        for (int k = 0; k < 16; k++)
                                        {
                                            if (apt2->b_pointers[k] != -1)
                                            {
                                                //Escribiremos los bloques de archivos
                                                struct BloqueArchivo *AR = ObtenerBloqueArchivo(apt2->b_pointers[k]);
                                                Imprimir += "bloque" + to_string(apt2->b_pointers[k]) + " [ shape=plaintext label=<\n";
                                                Imprimir += "<table border='1' cellborder='1'>\n";
                                                Imprimir += "<tr>\n";
                                                Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(apt2->b_pointers[k]) + "</td>\n";
                                                Imprimir += "</tr>\n";
                                                Imprimir += "<tr>\n";
                                                Imprimir += "<td>" + string(AR->content) + "</td>\n";
                                                Imprimir += "</tr>\n";
                                                Imprimir += "</table>\n";
                                                Imprimir += " >];\n";
                                                if (ant != -1)
                                                {
                                                    Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(apt2->b_pointers[k]) + "\n";
                                                }
                                                ant = apt2->b_pointers[k];
                                            }
                                        }
                                    }
                                }
                            }
                            else if (j == 14)
                            {
                                struct BloqueApuntadores *apuntadores = ObtenerBloqueApuntador(inodo->block[j]);
                                Imprimir += "bloque" + to_string(inodo->block[j]) + " [ shape=plaintext label=<\n";
                                Imprimir += "<table border='1' cellborder='1'>\n";
                                Imprimir += "<tr>\n";
                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(inodo->block[j]) + "</td>\n";
                                Imprimir += "</tr>\n";
                                for (int h = 0; h < 16; h++)
                                {
                                    Imprimir += "<tr>\n";
                                    Imprimir += "<td port='b" + to_string(h) + "'>" + to_string(apuntadores->b_pointers[h]) + "</td>\n";
                                    Imprimir += "</tr>\n";
                                }
                                Imprimir += "</table>\n";
                                Imprimir += " >];\n";
                                if (ant != -1)
                                {
                                    Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(inodo->block[j]) + "\n";
                                }
                                ant = inodo->block[j];
                                for (int h = 0; h < 16; h++)
                                {
                                    if (apuntadores->b_pointers[h] != -1)
                                    {
                                        struct BloqueApuntadores *apt2 = ObtenerBloqueApuntador(apuntadores->b_pointers[h]);
                                        Imprimir += "bloque" + to_string(apuntadores->b_pointers[h]) + " [ shape=plaintext label=<\n";
                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                        Imprimir += "<tr>\n";
                                        Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(apuntadores->b_pointers[h]) + "</td>\n";
                                        Imprimir += "</tr>\n";
                                        for (int k = 0; k < 16; k++)
                                        {
                                            Imprimir += "<tr>\n";
                                            Imprimir += "<td port='b" + to_string(k) + "'>" + to_string(apt2->b_pointers[k]) + "</td>\n";
                                            Imprimir += "</tr>\n";
                                        }
                                        Imprimir += "</table>\n";
                                        Imprimir += " >];\n";
                                        if (ant != -1)
                                        {
                                            Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(apuntadores->b_pointers[h]) + "\n";
                                        }
                                        ant = apuntadores->b_pointers[h];
                                        for (int k = 0; k < 16; k++)
                                        {
                                            if (apt2->b_pointers[k] != -1)
                                            {
                                                //Escribiremos los bloques de archivos

                                                struct BloqueApuntadores *apt3 = ObtenerBloqueApuntador(apt2->b_pointers[k]);
                                                Imprimir += "bloque" + to_string(apt2->b_pointers[k]) + " [ shape=plaintext label=<\n";
                                                Imprimir += "<table border='1' cellborder='1'>\n";
                                                Imprimir += "<tr>\n";
                                                Imprimir += "<td bgcolor=\"orange\">Bloque " + to_string(apt2->b_pointers[k]) + "</td>\n";
                                                Imprimir += "</tr>\n";
                                                for (int l = 0; l < 16; l++)
                                                {
                                                    Imprimir += "<tr>\n";
                                                    Imprimir += "<td port='b" + to_string(l) + "'>" + to_string(apt3->b_pointers[l]) + "</td>\n";
                                                    Imprimir += "</tr>\n";
                                                }
                                                Imprimir += "</table>\n";
                                                Imprimir += " >];\n";
                                                if (ant != -1)
                                                {
                                                    Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(apt3->b_pointers[k]) + "\n";
                                                }
                                                ant = apt2->b_pointers[k];
                                                for (int l = 0; l < 16; l++)
                                                {
                                                    if (apt3->b_pointers[l] != -1)
                                                    {
                                                        struct BloqueArchivo *arch2 = ObtenerBloqueArchivo(apt3->b_pointers[l]);
                                                        Imprimir += "bloque" + to_string(apt3->b_pointers[l]) + " [ shape=plaintext label=<\n";
                                                        Imprimir += "<table border='1' cellborder='1'>\n";
                                                        Imprimir += "<tr>\n";
                                                        Imprimir += "<td bgcolor=\"yellow\">Bloque " + to_string(apt3->b_pointers[l]) + "</td>\n";
                                                        Imprimir += "</tr>\n";
                                                        Imprimir += "<tr>\n";
                                                        Imprimir += "<td>" + string(arch2->content) + "</td>\n";
                                                        Imprimir += "</tr>\n";
                                                        Imprimir += "</table>\n";
                                                        Imprimir += " >];\n";
                                                        if (ant != -1)
                                                        {
                                                            Imprimir += "bloque" + to_string(ant) + "-> bloque" + to_string(apt3->b_pointers[l]) + "\n";
                                                        }
                                                        ant = apt3->b_pointers[l];
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Imprimir += "}";
    fclose(file);
    return Imprimir;
}

void repTree(string path, string id)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        pathSB = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActualSB = ObtenerParticionPE(aux->name, aux->path);
                    superB = ObtenerSuperBloque();
                    string Contenido = ReporteArbol();
                    //cout<<Contenido<<endl;
                    Graficar(path, "tree.dot", Contenido);
                    cout << "Grafica Generada!" << endl;
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

void repSb(string path, string id)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        pathSB = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActualSB = ObtenerParticionPE(aux->name, aux->path);
                    superB = ObtenerSuperBloque();
                    string Contenido = ReporteSB();
                    Graficar(path, "sb.dot", Contenido);
                    cout << "Grafica Generada!" << endl;
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

void repInode(string path, string id)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        pathSB = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActualSB = ObtenerParticionPE(aux->name, aux->path);
                    superB = ObtenerSuperBloque();
                    string Contenido = ReporteInode();
                    Graficar(path, "inode.dot", Contenido);
                    cout << "Grafica Generada!" << endl;
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

void repBlock(string path, string id)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        pathSB = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActualSB = ObtenerParticionPE(aux->name, aux->path);
                    superB = ObtenerSuperBloque();
                    string Contenido = ReporteBlock();
                    Graficar(path, "bloque.dot", Contenido);
                    cout << "Grafica Generada!" << endl;
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

int getGID(string arreglo[64], string nameGroup)
{
    int gid = 0;
    string UsuarioD[5];
    for (int i = 0; i < 5; i++)
    {
        UsuarioD[i] = "nulo";
    }

    string coma = ",";
    for (int i = 0; i < 50; i++)
    {
        if (arreglo[i] != "nulo")
        {
            string auxD = arreglo[i];
            size_t pos = 0;
            std::string token;
            int cont = 0;
            while ((pos = auxD.find(coma)) != std::string::npos)
            {
                token = auxD.substr(0, pos);
                arreglo[cont] = token;
                auxD.erase(0, pos + coma.length());
                cont++;
            }
            arreglo[cont] = auxD;

            if ((arreglo[2] == nameGroup) && (arreglo[1] == "g"))
            {
                return atoi(arreglo[0].c_str());
            }
        }
    }
    return gid;
}

bool ComprobarUusario(string usuario, string password)
{
    bool condicion = false;
    struct TablaInodos *inodo0 = new struct TablaInodos;
    inodo0 = ObtenerInodo(1);
    struct BloqueArchivo *BA = new struct BloqueArchivo;

    FILE *file = fopen(pathSB.c_str(), "rb");

    //Recuperar Contenido Users.txt
    string Contenido = "";
    for (int i = 0; i < 12; i++)
    {
        if (inodo0->block[i] != -1)
        {
            fseek(file, superB->block_start + sizeof(BloqueArchivo) * inodo0->block[i], 0);
            fread(BA, sizeof(BloqueArchivo), 1, file);
            Contenido += string(BA->content);
        }
    }

    string auxAnalizador[64];
    string auxC = Contenido;
    for (int i = 0; i < 64; i++)
    {
        auxAnalizador[i] = "nulo";
    }
    string salto = "\n";
    size_t pos = 0;
    std::string token;
    int cont = 0;
    while ((pos = auxC.find(salto)) != std::string::npos)
    {
        token = auxC.substr(0, pos);
        auxAnalizador[cont] = token;
        auxC.erase(0, pos + salto.length());
        cont++;
    }
    auxAnalizador[cont] = auxC;

    string usuarioD[5];
    for (int i = 0; i < 5; i++)
    {
        usuarioD[i] = "nulo";
    }
    string coma = ",";
    for (int i = 0; i < 64; i++)
    {
        if (auxAnalizador[i] != "nulo")
        {
            auxC = auxAnalizador[i];
            pos = 0;
            cont = 0;
            while ((pos = auxC.find(coma)) != std::string::npos)
            {
                token = auxC.substr(0, pos);
                usuarioD[cont] = token;
                auxC.erase(0, pos + coma.length());
                cont++;
            }
            usuarioD[cont] = auxC;

            if ((usuarioD[3] == usuario) && (usuarioD[4] == password))
            {
                UsuarioActivo->uid = atoi(usuarioD[0].c_str());
                UsuarioActivo->gid = getGID(auxAnalizador, usuarioD[2]);
                condicion = true;
                break;
            }
        }
    }
    fclose(file);
    return condicion;
}

void SimulacionPerdida()
{
    if (superB->filesystem_type == 3)
    {
        FILE *file = fopen(pathSB.c_str(), "rb+");
        fseek(file, superB->bm_inode_start, 0);
        int cont = 0;
        char aux = '0';

        for (int i = 0; i < superB->inodes_count; i++)
        {
            fwrite(&aux, sizeof(aux), 1, file);
        }

        fclose(file);
        cout << "Simulacion perdida terminada!" << endl;
    }
    else
    {
        cout << "Error, solo es permitido para EXT3" << endl;
    }
}

void SimulacionRecuperacion()
{
    if (superB->filesystem_type == 3)
    {
        FILE *file = fopen(pathSB.c_str(), "rb+");
        fseek(file, superB->bm_inode_start - (sizeof(Journaling) * superB->inodes_count), 0);

        for (int i = 0; i < superB->inodes_count; i++)
        {
            struct Journaling Journal;
            fread(&Journal, sizeof(Journaling), 1, file);
            if (Journal.InodoAfectado != -1)
            {
                string operacion = Journal.operacion;
                if (operacion != "rem")
                {
                    ActualizarBitMapInodos(Journal.InodoAfectado, '1');
                }
                else
                {
                    ActualizarBitMapInodos(Journal.InodoAfectado, '0');
                }
            }
        }

        fclose(file);
        cout << "Simulacion recuperacion terminada!" << endl;
    }
    else
    {
        cout << "Error, solo es permitido para EXT3" << endl;
    }
}

//------------------------------------------------COMANDOS--------------------------------------------------

//INICIAR SESION
void login(string usuario, string password, string id)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        pathSB = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActualSB = ObtenerParticionPE(aux->name, aux->path);
                    superB = ObtenerSuperBloque();
                    if (!InicioSesion)
                    {
                        if (ComprobarUusario(usuario, password))
                        {
                            cout << "Usuario y contraseña correctas!" << endl;
                            cout << "GID: " << UsuarioActivo->gid << endl;
                            cout << "UID: " << UsuarioActivo->uid << endl;
                            UsuarioActivo->id = id;
                            InicioSesion = true;
                        }
                        else
                        {
                            cout << "Usuario y contraseña incorrectas!" << endl;
                        }
                    }
                    else
                    {
                        cout << "No se puede iniciar sesion, existe una sesion activa!" << endl;
                    }
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

//CERRAR SESION
void logout()
{
    InicioSesion = false;
    cout << "Cerrando Sesion!" << endl;
}

//SIMULAR PERDIDA SISTEMA
void loss(string id)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        pathSB = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActualSB = ObtenerParticionPE(aux->name, aux->path);
                    superB = ObtenerSuperBloque();
                    SimulacionPerdida();
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

//SIMULAR RECUPERACION SISTEMA
void recovery(string id)
{
    TLista aux = getElemento(listaMount, id);
    if (aux != nullptr)
    {

        pathSB = aux->path;
        mbr = ObtenerMBR(pathSA);
        for (int i = 0; i < 4; i++)
        {
            string nombre = mbr->PARTICION[i].nombre;
            if (nombre == aux->name)
            {
                string tipo = Chart_String(mbr->PARTICION[i].type, 1);
                if (tipo == "p")
                {
                    ParticionActualSB = ObtenerParticionPE(aux->name, aux->path);
                    superB = ObtenerSuperBloque();
                    SimulacionRecuperacion();
                    break;
                }
                else
                {
                    if (tipo == "e")
                    {
                        cout << "Se debe elegir una particion lógica!" << endl;
                    }
                    else
                    {
                        cout << "FALTA TRABAJAR EL SISTEMA DE ARCHIVOS CON LAS LOGICAS!" << endl;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error, esta particion no ha sido montada!" << endl;
    }
}

//CREACION DE USUARIOS
void mkusr(string usr, string pwd, string grp)
{
}

//CREACION DE GRUPOS
void mkgrp(string id)
{
}
//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************
//-------------------------------------------------ARCHIVOS Y CARPETAS-------------------------------------------------
//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************

string *SplitRuta3(string entrada, string split)
{
    static string arreglo[64];
    for (int i = 0; i < 64; i++)
    {
        arreglo[i] = "nulo";
    }

    if (entrada == "")
    {
        return arreglo;
    }

    string slash = split;
    string aux = entrada;
    size_t pos = 0;
    std::string token;
    int cont = 0;
    while ((pos = aux.find(slash)) != std::string::npos)
    {
        token = aux.substr(0, pos);
        arreglo[cont] = token;
        aux.erase(0, pos + slash.length());
        cont++;
    }

    arreglo[cont] = aux;

    return arreglo;
}

int EspacioInodo()
{
    char aux = 'x';
    FILE *file = fopen(pathSB.c_str(), "rb");
    fseek(file, superB->bm_block_start, 0);

    for (int i = 0; i < superB->inodes_count; i++)
    {
        fread(&aux, sizeof(aux), 1, file);
        if (aux == '0')
        {
            fclose(file);
            return i;
        }
    }
    fclose(file);
    return -1;
}

int EspacioBloque()
{
    char aux = 'x';
    FILE *file = fopen(pathSB.c_str(), "rb");
    fseek(file, superB->bm_block_start, 0);

    for (int i = 0; i < superB->blocks_count; i++)
    {
        fread(&aux, sizeof(aux), 1, file);
        if (aux == '0')
        {
            fclose(file);
            return i;
        }
    }
    fclose(file);
    return -1;
}

int PosicionInodo(string path)
{
    path.erase(0, 1);
    int contBloques;
    bool existencia = false;
    string *aux;
    string auxDatos[64];

    TablaInodos *auxInodo;
    aux = SplitRuta3(path, "/");
    for (int i = 0; i < 64; i++)
    {
        auxDatos[i] = aux[i];
    }

    int CPadre = 0;
    auxInodo = ObtenerInodo(0);

    for (int i = 0; i < 64; i++)
    {
        if (auxDatos[i] == "nulo")
        {
            i = 66;
            break;
        }

        for (int j = 0; j < 15; j++)
        {
            if (j < 12)
            {
                if (auxInodo->block[j] != -1)
                { //Si existe un bloque
                    int posBlock = auxInodo->block[j];

                    struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(posBlock);
                    for (int h = 0; h < 4; h++)
                    {
                        if (Carpeta->content[h].name == auxDatos[i])
                        {
                            existencia = true;
                            CPadre = Carpeta->content[h].inodo;
                            auxInodo = ObtenerInodo(Carpeta->content[h].inodo);
                            j = 18;
                        }
                    }
                }
                else
                {
                    j = 18;
                }
            }
            else
            {
                if (j == 12)
                { //Bloque indirecto
                    if (auxInodo->block[j] != -1)
                    {
                        struct BloqueApuntadores *apt = ObtenerBloqueApuntador(auxInodo->block[j]);

                        for (int f = 0; f < 16; f++)
                        {
                            if (apt->b_pointers[f] != -1)
                            {
                                int posBlock = apt->b_pointers[f];
                                struct BloqueCarpeta *Capreta = ObtenerBloqueCarpeta(posBlock);
                                for (int h = 0; h < 4; h++)
                                {
                                    if (Capreta->content[h].name == auxDatos[i])
                                    {
                                        existencia = true;
                                        CPadre = Capreta->content[h].inodo;
                                        auxInodo = ObtenerInodo(Capreta->content[h].inodo);
                                        j = 18;
                                        f = 18;
                                    }
                                }
                            }
                            else
                            {
                                existencia = false;
                                j = 18;
                                f = 18;
                            }
                        }
                    }
                    else
                    {
                        existencia = false;
                        j = 18;
                    }
                }
                else if (j == 13)
                { //Bloque doble Indirecto
                    if (auxInodo->block[j] != -1)
                    {
                        struct BloqueApuntadores *apt = ObtenerBloqueApuntador(auxInodo->block[j]);
                        for (int f = 0; f < 16; f++)
                        {
                            if (apt->b_pointers[f] != -1)
                            {
                                struct BloqueApuntadores *apt2 = ObtenerBloqueApuntador(apt->b_pointers[f]);

                                for (int g = 0; g < 16; g++)
                                {
                                    if (apt2->b_pointers[g] != -1)
                                    {
                                        int posBloque = apt2->b_pointers[g];
                                        struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(posBloque);
                                        for (int h = 0; h < 4; h++)
                                        {
                                            if (string(Carpeta->content[h].name) == string(auxDatos[i]))
                                            {
                                                existencia = true;
                                                CPadre = Carpeta->content[h].inodo;
                                                auxInodo = ObtenerInodo(Carpeta->content[h].inodo);
                                                f = 18;
                                                g = 18;
                                                j = 18;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        existencia = false;
                                        j = 18;
                                        f = 18;
                                        g = 18;
                                    }
                                }
                            }
                            else
                            {
                                existencia = false;
                                j = 18;
                                f = 18;
                            }
                        }
                    }
                    else
                    {
                        existencia = false;
                        j = 18;
                    }
                }
                else if (j == 14)
                { //Bloque triple Indirecto
                    if (auxInodo->block[j] != -1)
                    {
                        struct BloqueApuntadores *apt = ObtenerBloqueApuntador(auxInodo->block[j]);

                        for (int f = 0; f < 16; f++)
                        {
                            if (apt->b_pointers[f] != -1)
                            {

                                struct BloqueApuntadores *apt2 = ObtenerBloqueApuntador(apt->b_pointers[f]);

                                for (int g = 0; g < 16; g++)
                                {
                                    if (apt2->b_pointers[g] != -1)
                                    {
                                        BloqueApuntadores *apt3 = ObtenerBloqueApuntador(apt2->b_pointers[g]);

                                        for (int e = 0; e < 16; e++)
                                        {
                                            if (apt3->b_pointers[e] != -1)
                                            {
                                                int posBlock = apt3->b_pointers[e];

                                                struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(posBlock);

                                                for (int h = 0; h < 4; h++)
                                                {
                                                    if (Carpeta->content[h].name == auxDatos[i])
                                                    {
                                                        existencia = true;
                                                        CPadre = Carpeta->content[h].inodo;
                                                        auxInodo = ObtenerInodo(Carpeta->content[h].inodo);
                                                        j = 18;
                                                        f = 18;
                                                        g = 18;
                                                        e = 18;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                existencia = false;
                                                j = 18;
                                                f = 18;
                                                g = 18;
                                                e = 18;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        j = 18;
                                        f = 18;
                                        g = 18;
                                        existencia = false;
                                    }
                                }
                            }
                            else
                            {
                                j = 18;
                                f = 18;
                                existencia = false;
                            }
                        }
                    }
                    else
                    {
                        existencia = false;
                        j = 18;
                    }
                }
            }
        }

        if (!existencia)
        {
            return -1;
        }
    }
    return CPadre;
}

void EscribirCarpeta(int posicion, struct BloqueCarpeta *Carpeta)
{
    FILE *file = fopen(pathSB.c_str(), "rb+");
    fseek(file, superB->block_start + (sizeof(BloqueCarpeta) * posicion), 0);
    fwrite(Carpeta, sizeof(BloqueCarpeta), 1, file);
    fclose(file);
}

void EscribirApuntadores(int posicion, BloqueApuntadores *apt)
{
    FILE *file = fopen(pathSB.c_str(), "rb+");
    fseek(file, superB->block_start + (sizeof(BloqueApuntadores) * posicion), 0);
    fwrite(apt, sizeof(BloqueApuntadores), 1, file);
    fclose(file);
}

void EscribirInodo(int posicion, struct TablaInodos *inodo){
    FILE *file=fopen(pathSB.c_str(),"rb+");
    fwrite(inodo, sizeof(TablaInodos), 1, file);
    fclose(file);
}

void EscribirJournaling(int inodoA, int tama, string contenido, string rut, char tipo, string op){
    struct Journaling *A;
    struct Journaling *B;
    strcpy(B->operacion, op.c_str());
    B->tipo=tipo;
    strcpy(B->path, rut.c_str());
    strcpy(B->contenido,contenido.c_str());
    B->size=tama;
    B->InodoAfectado=inodoA;

    FILE *file = fopen(pathSB.c_str(),"rb+");
    fseek(file, superB->bm_inode_start-(sizeof(Journaling)*superB->inodes_count),0);
    int pos =0;
    bool condicion =true;
    int i=0;
    while(condicion){
        fread(A, sizeof(Journaling),1, file);
        if(A->InodoAfectado==-1){
            pos=i;
            condicion=false;
        }
        i++;
    }

    fseek(file, (superB->bm_inode_start-(sizeof(Journaling)*superB->inodes_count))+(sizeof(Journaling)*pos),0);
    fwrite(B,sizeof(Journaling),1,file);
    fclose(file);
}

void ActualizarSuperBloque(int espacioBloque,int espacioInodo,int numBlocks, int numInodos){
    superB->free_inodes_count-=numInodos;
    superB->free_blocks_count-=numBlocks;
    superB->fist_ino=espacioInodo;

    FILE *file= fopen(pathSB.c_str(),"rb+");
    fseek(file, ParticionActualSB.start,0);
    fwrite(superB,sizeof(SuperBloque),1,file);
    fclose(file);
}
void MakeFiles(string path, bool r, int size, string cont)
{
    int numCarpeta = PosicionInodo(path);

    if (numCarpeta != -1)
    {
        cout << "Error, ya existe un archivo con el mismo nombre en la ruta dada!" << endl;
        return;
    }

    path.erase(0, 1);
    int numBlocks;
    string *aux;
    string auxDatos[64];
    bool existencia;

    struct TablaInodos *inodoAux;
    aux = SplitRuta3(path, "/");
    for (int i = 0; i < 64; i++)
    {
        auxDatos[i] = aux[i];
    }

    string nombreNewFile;
    for (int i = 0; i < 64; i++)
    {
        if (auxDatos[i] == "nulo")
        {
            nombreNewFile = auxDatos[i - 1];
            auxDatos[i - 1] = "nulo";
            i = 70;
        }
    }

    int CPadre = 0;
    inodoAux = ObtenerInodo(0);

    for (int i = 0; i < 64; i++)
    {
        if (auxDatos[i] == "nullo")
        {
            i = 70;
            break;
        }
        existencia = false;
        for (int j = 0; j < 15; j++)
        {
            if (j < 12)
            {
                if (inodoAux->block[j] != -1)
                {
                    int posBlock = inodoAux->block[j];

                    struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(posBlock);

                    for (int h = 0; h < 4; h++)
                    {
                        if (string(Carpeta->content[h].name) == string(auxDatos[i]))
                        {
                            existencia = true;
                            CPadre = Carpeta->content[h].inodo;
                            inodoAux = ObtenerInodo(Carpeta->content[h].inodo);
                            j = 18;
                        }
                    }
                }
                else
                {
                    existencia = false;
                    j = 18;
                }
            }
            else
            {
                if (j == 12)
                {
                    if (inodoAux->block[j] != -1)
                    {
                        struct BloqueApuntadores *apt = ObtenerBloqueApuntador(inodoAux->block[j]);

                        for (int e = 0; e < 16; e++)
                        {

                            if (apt->b_pointers[e] != -1)
                            {
                                int posBlock = apt->b_pointers[e];

                                struct BloqueCarpeta *Carpteta = ObtenerBloqueCarpeta(posBlock);

                                for (int h = 0; h < 4; h++)
                                {
                                    if (string(Carpteta->content[h].name) == string(auxDatos[i]))
                                    {
                                        existencia = true;
                                        CPadre = Carpteta->content[h].inodo;
                                        inodoAux = ObtenerInodo(Carpteta->content[h].inodo);
                                        j = 18;
                                        e = 18;
                                    }
                                }
                            }
                            else
                            {
                                existencia = false;
                                e = 18;
                                j = 18;
                            }
                        }
                    }
                    else
                    {
                        existencia = false;
                        j = 18;
                    }
                }
                else if (j == 13)
                {
                    if (inodoAux->block[j] != -1)
                    {
                        struct BloqueApuntadores *apt = ObtenerBloqueApuntador(inodoAux->block[j]);

                        for (int e = 0; e < 16; e++)
                        {

                            if (apt->b_pointers[e] != -1)
                            {
                                struct BloqueApuntadores *apt2 = ObtenerBloqueApuntador(apt->b_pointers[e]);

                                for (int f = 0; f < 16; f++)
                                {

                                    if (apt2->b_pointers[f] != -1)
                                    {

                                        int posBlock = apt2->b_pointers[f];

                                        struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(posBlock);

                                        for (int h = 0; h < 4; h++)
                                        {
                                            if (string(Carpeta->content[h].name) == string(auxDatos[i]))
                                            {
                                                existencia = true;
                                                CPadre = Carpeta->content[h].inodo;
                                                inodoAux = ObtenerInodo(Carpeta->content[h].inodo);
                                                j = 20;
                                                e = 20;
                                                f = 20;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        existencia = false;
                                        f = 18;
                                        e = 18;
                                        j = 18;
                                    }
                                }
                            }
                            else
                            {
                                existencia = false;
                                j = 18;
                                e = 18;
                            }
                        }
                    }
                    else
                    {
                        existencia = false;
                        j = 18;
                    }
                }
                else if (j == 14)
                {
                    if (inodoAux->block[j] != -1)
                    {
                        struct BloqueApuntadores *apt = ObtenerBloqueApuntador(inodoAux->block[j]);

                        for (int e = 0; e < 16; e++)
                        {

                            if (apt->b_pointers[e] != -1)
                            {

                                struct BloqueApuntadores *apt2 = ObtenerBloqueApuntador(apt->b_pointers[e]);

                                for (int f = 0; f < 16; f++)
                                {
                                    if (apt2->b_pointers[f] != -1)
                                    {
                                        struct BloqueApuntadores *apt3 = ObtenerBloqueApuntador(apt2->b_pointers[f]);
                                        for (int g = 0; g < 16; g++)
                                        {
                                            if (apt3->b_pointers[g] != -1)
                                            {
                                                int posBlock = apt3->b_pointers[g];

                                                struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(posBlock);

                                                for (int h = 0; h < 4; h++)
                                                {
                                                    if (string(Carpeta->content[h].name) == string(auxDatos[i]))
                                                    {
                                                        existencia = true;
                                                        CPadre = Carpeta->content[h].inodo;
                                                        inodoAux = ObtenerInodo(Carpeta->content[h].inodo);
                                                        g = 18;
                                                        f = 18;
                                                        e = 18;
                                                        j = 18;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                existencia = false;
                                                g = 18;
                                                f = 18;
                                                e = 18;
                                                j = 18;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        existencia = false;
                                        f = 18;
                                        e = 18;
                                        j = 18;
                                    }
                                }
                            }
                            else
                            {
                                existencia = false;
                                e = 18;
                                j = 18;
                            }
                        }
                    }
                    else
                    {
                        existencia = false;
                        j = 18;
                    }
                }
            }
        }

        if (!existencia)
        { //Creando Carpeta
            if (r)
            {
                string Recursivo = "/";
                for (int j = 0; j <= i; j++)
                {
                    if (j == i)
                    {
                        Recursivo += string(auxDatos[j]);
                    }
                    else
                    {
                        Recursivo += string(auxDatos[j]) + "/";
                    }
                }
                //CREAR DIRECTORIO(Recursivo, false);
                i = -1;
                inodoAux = ObtenerInodo(0);
            }
            else
            {
                cout << "Error, no se encontro directorio" << endl;
                return;
            }
        }
    }

    //Escribiendo Inodo
    int espacioInodo = EspacioInodo();
    int espacioBloque;

    for (int i = 0; i < 15; i++)
    {
        if (i < 12)
        {

            if (inodoAux->block[i] != -1)
            {
                struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(inodoAux->block[i]);
                for (int j = 0; j < 4; j++)
                {
                    if (Carpeta->content[j].inodo == -1)
                    {
                        Carpeta->content[j].inodo = espacioInodo;
                        strcpy(Carpeta->content[j].name, nombreNewFile.c_str());
                        EscribirCarpeta(inodoAux->block[i], Carpeta);
                        j = 5;
                        i = 20;
                    }
                }
            }
            else
            {
                espacioBloque = EspacioBloque();
                struct BloqueCarpeta *nuevaCarp = new struct BloqueCarpeta;
                inodoAux->block[i] = espacioBloque;
                nuevaCarp->content[0].inodo = espacioInodo;
                strcpy(nuevaCarp->content->name, nombreNewFile.c_str());
                EscribirCarpeta(espacioBloque, nuevaCarp);
                ActualizarBitMapBloques(espacioBloque, '1');
                numBlocks++;
                i = 18;
            }
        }
        else
        {
            if (i == 12)
            {
                if (inodoAux->block[i] != -1)
                {
                    struct BloqueApuntadores *apt = ObtenerBloqueApuntador(inodoAux->block[i]);

                    for (int e = 0; e < 16; e++)
                    {
                        if (apt->b_pointers[e] != -1)
                        {

                            int posBlock = apt->b_pointers[e];
                            struct BloqueCarpeta *Carpeta = ObtenerBloqueCarpeta(posBlock);

                            for (int h = 0; h < 4; h++)
                            {
                                if (Carpeta->content[h].inodo == -1)
                                {
                                    Carpeta->content[h].inodo = espacioInodo;
                                    strcpy(Carpeta->content[h].name, nombreNewFile.c_str());
                                    EscribirCarpeta(posBlock, Carpeta);
                                    e = 18;
                                    i = 18;
                                }
                            }
                        }
                        else
                        {
                            espacioBloque = EspacioBloque();

                            struct BloqueCarpeta *nuevaCarp;
                            apt->b_pointers[e] = espacioBloque;
                            nuevaCarp->content[0].inodo = espacioInodo;
                            strcpy(nuevaCarp->content[0].name, nombreNewFile.c_str());
                            EscribirCarpeta(espacioBloque, nuevaCarp);
                            EscribirApuntadores(inodoAux->block[i], apt);
                            ActualizarBitMapBloques(espacioBloque, '1');
                            numBlocks++;
                            i = 18;
                            e = 18;
                        }
                    }
                }
                else
                {
                    espacioBloque=EspacioBloque();
                    int espacioBloque2=espacioBloque+1;

                    struct BloqueApuntadores *apts;
                    struct BloqueCarpeta *carp;
                    inodoAux->block[i]=espacioBloque;
                    apts->b_pointers[0]=espacioBloque2;
                    carp->content[0].inodo=espacioInodo;
                    stpcpy(carp->content[0].name,nombreNewFile.c_str());
                    EscribirApuntadores(espacioBloque,apts);
                    EscribirCarpeta(espacioBloque2,carp);
                    ActualizarBitMapBloques(espacioBloque,'1');
                    ActualizarBitMapBloques(espacioBloque2,'1');
                    numBlocks+=2;
                    i=18;
                }
            }else if(i==13){

            }else if(i==14){

            }
        }
    }

    espacioBloque=EspacioBloque();
    struct TablaInodos *inodoD;

    inodoD->uid=UsuarioActivo->uid;
    inodoD->gid=UsuarioActivo->gid;
    inodoD->size=0;

    time_t tiempo;
    tiempo=time(NULL);
    inodoD->atime=tiempo;
    inodoD->ctime=tiempo;
    inodoD->mtime=tiempo;
    inodoD->type='0';
    inodoD->perm=664;
    inodoD->block[0]=espacioBloque;

    struct BloqueCarpeta *carpD;
    carpD->content[0].inodo=espacioInodo;
    char auxD[]=".";
    strcpy(carpD->content[0].name,auxD);

    carpD->content[1].inodo=CPadre;
    char auxD2[]="..";
    stpcpy(carpD->content[1].name,auxD2);
    EscribirInodo(espacioInodo,inodoD);
    EscribirCarpeta(espacioBloque, carpD);
    numBlocks++;
    ActualizarBitMapInodos(espacioInodo,'1');
    ActualizarBitMapBloques(espacioBloque,'1');
    EscribirInodo(CPadre, inodoAux);
    if(superB->filesystem_type==3){
        //Se actualiza Journaling
        EscribirJournaling(espacioInodo,0,"Carpeta Creada",path,'0',"mkdir");
    }
    cout<<"Carpeta creada!"<<endl;
    //Acatualizar super Bloque
    ActualizarSuperBloque(espacioBloque+1,espacioInodo+1,numBlocks,1);

    

}
//---------------------------------------COMANDOS----------------------------
void mkfile(string path, bool r, int size, string cont)
{
    if (InicioSesion)
    {
        MakeFiles(path, r, size, cont);
    }
    else
    {
        cout << "Error, No se puede crear carpetas si no han iniciado sesion!" << endl;
    }
}
#endif