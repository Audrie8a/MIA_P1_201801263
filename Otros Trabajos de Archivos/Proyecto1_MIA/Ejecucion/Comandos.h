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

string Porcentajes(int size)
{
    float tamano = (float)size;
    float tamanoTotal = (float)mbr->SIZE;
    float porcentaje = (tamano / tamanoTotal) * 100;
    return to_string(porcentaje);
}

string Chart_String(char *arreglo, int limite)
{
    string Cadena = "";
    for (int i = 0; i < limite; i++)
    {
        Cadena = Cadena + arreglo[i];
    }
    return Cadena;
}

string SplitRutaSinNombreArchivo2(string path)
{
    vector<string> vec;
    string palabra = "";
    for (auto x : path)
    {
        if (x == '/')
        {
            //cout << palabra << endl;
            if (palabra != "")
            {
                vec.push_back(palabra);
            }
            palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    palabra = "";
    for (auto x : vec)
    {
        palabra += "/" + x;
    }
    return palabra + "/";
}

vector<string> SplitSlash(string Entrada)
{
    vector<string> vec;
    string palabra = "";
    for (auto x : Entrada)
    {
        if (x == '/')
        {
            //cout << palabra << endl;
            if (palabra != "")
            {
                vec.push_back(palabra);
            }
            palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    if (palabra != "")
    {
        //cout << palabra << endl;
        vec.push_back(palabra);
    }
    return vec;
}

vector<string> SplitPunto(string Entrada)
{
    vector<string> vec;
    string palabra = "";
    for (auto x : Entrada)
    {
        if (x == '.')
        {
            //cout << palabra << endl;
            if (palabra != "")
            {
                vec.push_back(palabra);
            }
            palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    if (palabra != "")
    {
        //cout << palabra << endl;
        vec.push_back(palabra);
    }
    return vec;
}

vector<int> getInicioFin(string Entrada)
{
    string palabra = "";
    vector<int> numero;
    numero.clear();
    for (auto x : Entrada)
    {
        if (x == '-')
        {
            //cout << palabra << endl;
            if (palabra != "")
            {
                numero.push_back(stoi(palabra));
            }
            palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    if (palabra != "")
    {
        //cout << palabra << endl;
        numero.push_back(stoi(palabra));
    }
    return numero;
}

vector<string> getParticionPathDisco(string Entrada)
{
    string palabra = "";
    vector<string> numero;
    numero.clear();
    for (auto x : Entrada)
    {
        if (x == '-')
        {
            //cout << palabra << endl;
            if (palabra != "")
            {
                numero.push_back(palabra);
            }
            palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    if (palabra != "")
    {
        //cout << palabra << endl;
        numero.push_back(palabra);
    }
    return numero;
}

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
    else
    {
        ID_Disco.push_back(numero);
    }
    return numero;
}

void Graficar(string Contenido, string path)
{
    vector<string> aux;
    aux = SplitSlash(path);
    aux = SplitPunto(aux[aux.size() - 1]);
    string extension = aux[1];
    string nombreResRep = aux[0] + "." + extension;
    string nombreReporte = aux[0] + ".dot"; //"disco.dot";
    path = SplitRutaSinNombreArchivo2(path) + nombreReporte;
    ofstream myfile;
    myfile.open(path);
    myfile << Contenido;
    myfile.close();
    //dot -T png Disco7.dot -o Disco7.png
    string comando = "dot -T " + extension + " " + nombreReporte + " -O " + nombreResRep;
    cout << comando << endl;
    //system(comando.c_str());
}

void Graficar2(string Contenido, string path)
{
    vector<string> aux;
    aux = SplitSlash(path);
    aux = SplitPunto(aux[aux.size() - 1]);
    string nombreReporte = aux[0] + ".dot"; //"disco.dot";
    ofstream file;
    path = SplitRutaSinNombreArchivo2(path) + nombreReporte;
    file.open(path.c_str());
    file << "digraph G  {  node1;node2;node3}"; //Contenido;
    file.close();

    char instruccion[50]; //={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,000 000};

    instruccion[0] = '\0';
    string comando1 = "dot -Tpng -O " + nombreReporte;
    strcat(instruccion, comando1.c_str());
    system(instruccion);
    instruccion[0] = '\0';
    string comando2 = "xdg-open " + nombreReporte + ".png";
    strcat(instruccion, comando2.c_str());
    system(instruccion);
}

void EscribirParticionDisco(string path, int size, int start, struct partition *particion)
{

    //Escribiendo en Archivo
    FILE *fichero;
    fichero = fopen(path.c_str(), "r+b");
    fseek(fichero, start, 0);
    fwrite(particion, sizeof(particion), 1, fichero);

    //Actualizar MBR
    fseek(fichero, 0, 0);
    fwrite(mbr, sizeof(MBR) - 1, 1, fichero);
    fclose(fichero);
}

struct MBR *ObtenerMBR(string path)
{
    FILE *arch;
    arch = fopen(path.c_str(), "rb");
    struct MBR *newMBR = new struct MBR;
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, 0, SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(newMBR, sizeof(MBR) - 1, 1, arch);
    //cout<<newMBR->SIGNATURE<<endl;
    //cout<<newMBR->FIT<<endl;
    //cout<<newMBR->SIZE<<endl;
    //cout<<newMBR->FECHA<<endl;
    fclose(arch);

    return newMBR;
}

struct MBR *EscribirMBR(char *fit, int size)
{
    struct partition parte[4];
    parte[0].start = 0;
    parte[0].size = 0;
    struct MBR *newMBR = new struct MBR;
    int numeroID = SignatureDisco();
    ID_Disco.push_back(numeroID);
    newMBR->SIGNATURE = numeroID;
    newMBR->SIZE = size;
    strcpy(newMBR->FIT, fit);
    char fecha[25]; //ctime devuelve 26 caracteres pero tambien se podría usar un puntero de char
    time_t current_time;
    current_time = time(NULL);
    ctime(&current_time);
    strcpy(fecha, ctime(&current_time));
    strcpy(newMBR->FECHA, fecha);
    newMBR->PARTICION[0] = parte[0];
    newMBR->PARTICION[1] = parte[0];
    newMBR->PARTICION[2] = parte[0];
    newMBR->PARTICION[3] = parte[0];
    return newMBR;
}

struct EBR *ObtenerEBR(string path, int Start)
{
    FILE *arch;
    arch = fopen(path.c_str(), "rb");
    struct EBR *newMBR = new struct EBR;
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch, Start, SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(newMBR, sizeof(EBR) - 1, 1, arch);
    //cout<<newMBR->SIGNATURE<<endl;
    //cout<<newMBR->FIT<<endl;
    //cout<<newMBR->SIZE<<endl;
    //cout<<newMBR->FECHA<<endl;
    fclose(arch);

    return newMBR;
}

struct EBR *EscribirEBR(char *fit, int size, char *status, char *nombre, int start, int next)
{
    struct EBR *newMBR = new struct EBR;
    newMBR->size = size;
    strcpy(newMBR->fit, fit);
    strcpy(newMBR->status, status);
    strcpy(newMBR->nombre, nombre);
    newMBR->start = start;
    newMBR->next = next;
    return newMBR;
}

int ObtenerStartEBR(string path, int start)
{
    if (ebr->size != 0)
    {
        ebr = ObtenerEBR(path, ebr->next);
        if (ebr->next != -1)
        {
            start = ObtenerStartEBR(path, ebr->next);
        }
        else
        {
            start = ebr->start + sizeof(EBR) + ebr->size - 1; //Lo que obtengo es el final de la partición lógica
        }
    }
    return start;
};

//Función que devuelve TRUE sí el nombre de la partición no está repetido en el disco, de lo contrario devuelve FALSE
bool ComprobarNombre(string nombre, string path)
{
    bool condicion = true;
    int tama = ID_ParticionDisco.size();
    vector<string> aux;
    string Dato = "";
    if (tama != 0)
    {
        for (auto x : ID_ParticionDisco)
        {
            aux = getParticionPathDisco(x);
            if (aux[0] == nombre && aux[1] == path)
            {
                condicion = false; //Ya hay una partición con este nombre en el disco
                break;
            }
        }

        if (condicion != false)
        {
            Dato = nombre + "-" + path;
            ID_ParticionDisco.push_back(Dato);
        }
    }
    else
    {
        Dato = nombre + "-" + path;
        ID_ParticionDisco.push_back(Dato);
    }

    return condicion;
}

string GenerarIDParticionMontada(string path, string name)
{
    vector<string> aux;
    string ID = "";
    string letra = "";
    int contParticiones = ID_ParticionDisco.size() - 1;
    int contDiscos = ID_Disco.size();
    if (!ComprobarNombre(name, path)) //Comprobar que el nombre existe
    {
        for (auto x : ID_ParticionDisco)
        {
            aux = getParticionPathDisco(x);
            if (aux[1] == path)
            {
                contParticiones--; //Contando cuantas particiones hay en el disco
            }
        }
        mbr = ObtenerMBR(path);
        for (auto x : ID_Disco)
        {
            contDiscos--;
            if (x == mbr->SIGNATURE)
            {
                break;
            }
        }

        if (contParticiones == 1)
        {
            letra = "A";
        }
        else if (contParticiones == 2)
        {
            letra = "B";
        }
        else if (contParticiones == 3)
        {
            letra = "C";
        }
        else if (contParticiones == 4)
        {
            letra = "D";
        }
        //201801263
        ID = "63" + to_string(contDiscos) + letra;
    }
    else
    {
        ID = "NO";
    }
    return ID;
}

void ObtenerEspaciosLibres()
{

    ListaEspaciosLibres.clear();
    struct partition particiones[4];
    particiones[0] = mbr->PARTICION[0];
    particiones[1] = mbr->PARTICION[1];
    particiones[2] = mbr->PARTICION[2];
    particiones[3] = mbr->PARTICION[3];

    vector<string> listaEspaciosOcupados;
    listaEspaciosOcupados.clear();
    string espacioOcupado = "";

    for (struct partition p : particiones)
    {
        if (p.size != 0)
        {
            espacioOcupado = to_string(p.start) + "-" + to_string((p.start + p.size - 1));
            listaEspaciosOcupados.push_back(espacioOcupado);
        }
    }
    int fin = sizeof(MBR);
    int start = 0;
    int tama = listaEspaciosOcupados.size();
    if (tama != 0)
    {
        //Ordenar espacios ocupados
        bool swapped;
        vector<int> aux;
        aux.clear();
        vector<int> auxSig;
        auxSig.clear();
        do
        {
            swapped = false;
            for (int i = 0; i < tama; i++)
            {
                if ((i + 1) < tama)
                {
                    aux = getInicioFin(listaEspaciosOcupados[i]);
                    auxSig = getInicioFin(listaEspaciosOcupados[i + 1]);
                    if (aux[0] > auxSig[0])
                    {
                        swap(listaEspaciosOcupados[i], listaEspaciosOcupados[i + 1]);
                        swapped = true;
                    }
                }
            }

        } while (swapped);

        aux.clear();
        auxSig.clear();
        for (int i = 0; i < tama; i++)
        {
            tama = listaEspaciosOcupados.size();
            if ((i + 1) < tama)
            {
                aux = getInicioFin(listaEspaciosOcupados[i]);
                auxSig = getInicioFin(listaEspaciosOcupados[i + 1]);
                if ((auxSig[0] - aux[1]) > 1)
                {
                    //Hay espacio libre
                    string Espacio = to_string((aux[1] + 1)) + "-" + to_string((auxSig[0] - 1));
                    ListaEspaciosLibres.push_back(Espacio);
                }
            }
            else
            {
                aux = getInicioFin(listaEspaciosOcupados[i]);
                if (aux[1] != mbr->SIZE - 1)
                {
                    string Espacio = to_string((aux[1] + 1)) + "-" + to_string((mbr->SIZE - 1));
                    ListaEspaciosLibres.push_back(Espacio);
                }
            }
        }
    }
    else
    {
        //No se han guardado particiones aún
        start = fin;
        fin = mbr->SIZE - 1;
        string Espacio = to_string(start) + "-" + to_string(fin);
        ListaEspaciosLibres.push_back(Espacio);
    }
}

string GraficarEBR(int contador)
{
    string status = Chart_String(ebr->status, 1);
    string fit = Chart_String(ebr->fit, 1);
    string name = Chart_String(ebr->nombre, 16);

    string contenido = "";
    contenido += "digraph G{\n";
    contenido += "\tnodo[ shape=plaintext label=<";
    contenido += "\t\t <table>\n";

    //Titulo
    contenido += "\t\t\t<tr>\n";
    contenido += "\t\t\t\t<td colspan=\"4\" color=\"red\"><b>";
    contenido += "EBR " + to_string(contador);
    contenido += "</b></td>\n";
    contenido += "\t\t\t\t</tr>\n";

    //Contenido tabla

    contenido += "\t\t\t<tr>\n";
    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += "part_status_1";
    contenido += "</b></td>\n";

    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += status;
    contenido += "</b></td>\n";
    contenido += "\t\t\t\t</tr>\n";

    contenido += "\t\t\t<tr>\n";
    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += "part_fit_1";
    contenido += "</b></td>\n";

    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += fit;
    contenido += "</b></td>\n";
    contenido += "\t\t\t\t</tr>\n";

    contenido += "\t\t\t<tr>\n";
    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += "part_start_1";
    contenido += "</b></td>\n";

    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += to_string(ebr->start);
    contenido += "</b></td>\n";
    contenido += "\t\t\t\t</tr>\n";

    contenido += "\t\t\t<tr>\n";
    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += "part_size_1";
    contenido += "</b></td>\n";

    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += to_string(ebr->size);
    contenido += "</b></td>\n";
    contenido += "\t\t\t\t</tr>\n";

    contenido += "\t\t\t<tr>\n";
    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += "part_next_1";
    contenido += "</b></td>\n";

    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += to_string(ebr->next);
    contenido += "</b></td>\n";
    contenido += "\t\t\t\t</tr>\n";

    contenido += "\t\t\t<tr>\n";
    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += "part_name_1";
    contenido += "</b></td>\n";

    contenido += "\t\t\t\t<td ropswan=\"3\" color=\"blue\"><b>";
    contenido += name;
    contenido += "</b></td>\n";
    contenido += "\t\t\t\t</tr>\n";

    contenido += "\t\t</table>\n";
    contenido += "\t\t>];";
    contenido += "}";
    return contenido;
}

void ReporteMBR(string path, string pathDisco)
{
    string contenido = "digraph G  {";
    contenido += " graph [pad=\"0.5\", nodesep=\"0.5\", ranksep=\"2\"];";
    contenido += "node [shape=record, fontname=\"Arial\"];";

    contenido += " set1 [label = \"{Nombre |mbr_tamaño | mbr_fecha_creacion | mbr_disk_signature |mbr_disk_fit |part_status_1| part_type_1| part_fit_1| part_start_1| part_size_1| part_name_1 | part_status_2| part_type_2| part_fit_2| part_start_2| part_size_2| part_name_2 | part_status_3| part_type_3| part_fit_3| part_start_3| part_size_3| part_name_3 |part_status_4| part_type_4| part_fit_4| part_start_4| part_size_4| part_name_4 }| { Valor ";
    contenido += "|" + to_string(mbr->SIZE);
    string Fecha = mbr->FECHA;
    contenido += "|" + Fecha;
    contenido += "|" + to_string(mbr->SIGNATURE);
    string Fit = mbr->FIT;
    contenido += "|" + Fit;

    //Escribiendo información de particiones
    for (struct partition particion : mbr->PARTICION)
    {
        string status = Chart_String(particion.status, 1);
        contenido += "|" + status;
        string tipo = Chart_String(particion.type, 1);
        contenido += "|" + tipo;
        string fitP = Chart_String(particion.fit, 1);
        contenido += "|" + fitP;
        contenido += "|" + to_string(particion.start);
        contenido += "|" + to_string(particion.size);
        string NameP = particion.nombre;
        contenido += "|" + NameP;
    }
    //TE FALTAN LA PARTE DEL EBR  PARA LAS PARTICIONES EXTENDIDAS

    contenido += "}\"];}";

    Graficar(contenido, path);
    contenido = "";
    int contador = 0;
    vector<string> aux;
    for (auto x : listParticionLogica)
    {
        aux = getParticionPathDisco(x);
        if (aux[1] == pathDisco)
        {
            ebr = ObtenerEBR(pathDisco, stoi(aux[2]));
            contenido += GraficarEBR(contador);
            contador++;
        }
    }
    if (contenido != "")
    {
        vector<string> aux;
        aux = SplitSlash(path);
        aux = SplitPunto(aux[aux.size() - 1]);
        string extension = aux[1];
        string nombreResRep = aux[0] + "." + extension;
        string nombreReporte = aux[0] + "EBR.jpg"; //"disco.dot";
        path = SplitRutaSinNombreArchivo2(path) + nombreReporte;
        Graficar(contenido, path);
    }
    //cout<<"CONTENIDO------------------------------------------------------------"<<endl;
    //cout<<contenido<<endl;
}

string DibujarEBR(string path)
{
    string contenido = "";
    if (ebr->size != 0)
    {
        if (ebr->next != -1)
        {
            ebr = ObtenerEBR(path, ebr->next);
            contenido = DibujarEBR(path);
        }
        else
        {
            contenido += "\n\t\t <tr>";
            contenido += "\t\t\t<td color=\"brown\">";
            string espacio = " ";
            contenido += "Logica" + espacio + Porcentajes(ebr->size) + "%" + "</td>\n";
        }
    }

    return contenido;
}

void ReporteDISK(string path, string pathDisco)
{
    ObtenerEspaciosLibres();
    string contenido = "digraph G  {\n";
    contenido += "\tnodo [shape=plaintext label=<\n";
    contenido += "\t<table>\n\t\t<tr>\n";

    //MBR"
    contenido += "\t\t\t<td rowspan=\"4\" color=\"red\"><b>MBR ";
    contenido += Porcentajes(sizeof(MBR)) + "%";
    //contenido +="\t\t<td rowspan=\" 3 \" color=\" orange \">";
    contenido += "</b></td>\n";
    //PARTICIONES
    struct partition particion[4];
    particion[0] = mbr->PARTICION[0];
    particion[1] = mbr->PARTICION[1];
    particion[2] = mbr->PARTICION[2];
    particion[3] = mbr->PARTICION[3];
    //Ordenar espacios ocupados
    bool swapped;
    int inicioA = 0;
    int inicioB = 0;
    do
    {
        swapped = false;
        for (int i = 0; i < 4; i++)
        {
            if ((i + 1) < 4)
            {
                inicioA = particion[i].start;
                inicioB = particion[i + 1].start;
                if (inicioA > inicioB)
                {
                    swap(particion[i], particion[i + 1]);
                    swapped = true;
                }
            }
        }

    } while (swapped);

    for (int i = 0; i < 4; i++)
    {
        string n = Chart_String(particion[i].type, 1); //string(particion[i].type);
        int start = sizeof(MBR);
        vector<int> datos;
        int tamaArray = ListaEspaciosLibres.size();
        if (tamaArray != 0 && i < tamaArray)
        {

            datos = getInicioFin(ListaEspaciosLibres[i]);
            if (start == datos[0])
            {
                int tama = datos[1] - datos[0];
                contenido += "\t\t\t<td rowspan=\" 3 \" color=\"gold\">";
                contenido += "Libre " + Porcentajes(tama) + "%";
                contenido += "</td>\n";
            }
            else
            {
                if (n == "p")
                {
                    contenido += "\t\t\t<td rowspan=\" 3 \" color=\"orange\">";
                    contenido += string(particion[i].nombre) + " " + Porcentajes(particion[i].size) + "%";
                    contenido += "</td>\n";
                }
                else if (n == "e")
                {
                    contenido += "\t\t\t<td colspan=\"4\" color=\"blue\">";
                    contenido += string(particion[i].nombre) + " " + Porcentajes(particion[i].size) + "%";
                    contenido += "</td>\n";
                }
                start = particion[i].start + particion[i].size;
            }
        }
        else
        {
            if (n == "p")
            {
                contenido += "\t\t\t<td rowspan=\" 3 \" color=\"orange\">";
                contenido += string(particion[i].nombre) + " " + Porcentajes(particion[i].size) + "%";
                contenido += "</td>\n";
            }
            else if (n == "e")
            {
                contenido += "\t\t\t<td colspan=\"4\" color=\"blue\">";
                contenido += string(particion[i].nombre) + " " + Porcentajes(particion[i].size) + "%";
                contenido += "</td>\n";
            }
        }
    }
    contenido += "\t\t</tr>";
    //PARTICIONES LOGICAS
    bool logica = false;
    for (int i = 0; i < 4; i++)
    {
        string n = Chart_String(particion[i].type, 1);
        if (n == "e")
        {
            int inicioParticion = particion[i].start + sizeof(particion[i]) + 1;
            //int finalParticion = particion[i].start + particion[i].size - 1;
            ebr = ObtenerEBR(pathDisco, inicioParticion);
            string contenido2 = DibujarEBR(pathDisco);
            if (contenido2 != "")
            {
                contenido += contenido2;
                logica = true;
            }
            break;
        }
    }
    if (logica)
    {
        contenido += "\t\t</tr>";
    }

    contenido += "\t</table>\n \t>];\n}";
    Graficar(contenido, path);
}

void repLs(string path, string id) {}

void repFile(string path, string id) {}

void repSb(string path, string id) {}

void repTree(string path, string id) {}

void repBm_block(string path, string id) {}

void repBm_inode(string path, string id) {}

void repBlock(string path, string id) {}

void repJournaling(string path, string id) {}

void repInode(string path, string id) {}

void repDisk(string path, string id)
{
    bool condicion = false;
    //if (ParticionMontada.size() != 0)
    //{
        string pathDisco = "";
        vector<string> aux; // id- nombre- path
        for (auto x : ParticionMontada)
        {
            aux = getParticionPathDisco(x);

            //if (aux[0] == id)
            //{
                condicion = true;
                mbr = ObtenerMBR(aux[2]);
                pathDisco = aux[2];
                ReporteDISK(path, pathDisco);
                cout << "Se ha generado el Reporte!" << endl;
                break;
            //}
        }

        if (!condicion)
        {
            cout << "Error al generar el Reporte! Particion no Montada" << endl;
        }
    //}
    //else
    //{
    //    cout << "No se encuentran particiones Montadas" << endl;
    //}
}

void repMBR(string path, string id)
{
    bool condicion = false;

    if (ParticionMontada.size() != 0)
    {
        vector<string> aux; // id- nombre- path
        for (auto x : ParticionMontada)
        {
            aux = getParticionPathDisco(x);

            if (aux[0] == id)
            {
                condicion = true;
                mbr = ObtenerMBR(aux[2]);
                ReporteMBR(path, aux[2]);
                cout << "Se ha generado el Reporte!" << endl;
                break;
            }
        }
        if (!condicion)
        {
            cout << "Error al generar reporte! Particion no montada" << endl;
        }
        //ReporteDISK(path);
    }
    else
    {
        cout << "No se encuentran particiones Montadas" << endl;
    }
}

void mkdisk(int size, string f, string u, string path)
{
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
    }
    else
    {
        string mensaje = "No se pudo crear el archivo en: " + path;

        cout << mensaje << endl;
    }
    fclose(fichero);
}

vector<int> PeorAjuste()
{
    vector<int> start;
    start.clear();
    vector<int> datos;
    datos.clear();
    int tama = 0;
    int max = 0;
    int ini = 0;
    if (ListaEspaciosLibres.size() != 0)
    {
        for (auto dato : ListaEspaciosLibres)
        {
            datos = getInicioFin(dato);
            tama = datos[1] - datos[0];
            if (tama > max)
            {
                max = tama;
                ini = datos[0];
            }
        }
    }
    start.push_back(ini);
    start.push_back(max);
    return start; //Se envía un arreglo con los siguientes datos: Donde empieza (inicio) y tamaño del espacio libre
}

int MejorAjuste(int size)
{
    vector<int> datos;
    datos.clear();
    int tama = 0;
    int min = mbr->SIZE - 1;
    int start = -1;
    for (auto dato : ListaEspaciosLibres)
    {
        datos = getInicioFin(dato);
        tama = datos[1] - datos[0];
        if (tama < min && tama <= size)
        {
            min = tama;
            start = datos[0];
        }
    }
    return start; //Se envía un arreglo con los siguientes datos: Donde empieza (inicio) y tamaño del espacio libre
}

int PrimerAjuste(int size)
{
    vector<int> datos;
    datos.clear();
    int tama = 0;
    int start = -1;
    for (auto dato : ListaEspaciosLibres)
    {
        datos = getInicioFin(dato);
        tama = datos[1] - datos[0];
        if (tama >= (size - 1))
        {
            start = datos[0];
            break;
        }
    }
    return start; //Se envía un arreglo con los siguientes datos: Donde empieza (inicio) y tamaño del espacio libre
}

int UbicacionSegunAjuste(string fit, int size)
{
    vector<int> dato;
    dato.clear();
    int ubicacion = -1;
    if (fit == "b")
    { //Mejor Ajuste
        ubicacion = MejorAjuste(size);
    }
    else if (fit == "f")
    { //Primer Ajuste
        ubicacion = PrimerAjuste(size);
    }
    else if (fit == "w")
    { //Peor Ajuste
        dato = PeorAjuste();
        if (dato[1] >= size)
        {
            ubicacion = dato[0];
        }
    }
    return ubicacion;
}

void PruebaUbicacion(string path)
{
    struct partition particiones[4];
    string status = "u";
    strcpy(particiones[0].status, status.c_str());
    strcpy(particiones[0].type, "p");
    strcpy(particiones[0].fit, "wf");
    strcpy(particiones[0].nombre, "particion1");
    particiones[0].start = 152;
    particiones[0].size = 307200;
    mbr->PARTICION[0] = particiones[0];
    EscribirParticionDisco(path, 307200, 152, &particiones[0]);
    strcpy(particiones[1].status, status.c_str());
    strcpy(particiones[1].type, "e");
    strcpy(particiones[1].fit, "wf");
    strcpy(particiones[1].nombre, "particion1");
    particiones[1].start = 614552;
    particiones[1].size = 3579750;
    mbr->PARTICION[1] = particiones[1];
    EscribirParticionDisco(path, 3579750, 614552, &particiones[1]);
    mbr = ObtenerMBR(path);
    ObtenerEspaciosLibres();
}

void ParticionPrimaria(string fit, int size, string path, string nombre, string tipo)
{
    mbr = ObtenerMBR(path);
    ListaEspaciosLibres.clear();
    ObtenerEspaciosLibres();
    int contador = 0;
    int contadorPrimarias = 0;
    if (ComprobarNombre(nombre, path))
    {
        for (struct partition particion : mbr->PARTICION)
        {
            //string part=string(particion.type);
            //string p="p";
            if (particion.type[0] == 112)
            {
                contadorPrimarias++;
            }
        }
        if (contadorPrimarias != 3)
        {
            for (struct partition particion : mbr->PARTICION)
            {

                if (particion.size == 0)
                {
                    //PruebaUbicacion(path);
                    int start = UbicacionSegunAjuste(fit, size);
                    if (start != -1)
                    {
                        if (contadorPrimarias <= 3)
                        {

                            string status = "u";
                            strcpy(particion.status, status.c_str());
                            strcpy(particion.type, tipo.c_str());
                            strcpy(particion.fit, fit.c_str());
                            strcpy(particion.nombre, nombre.c_str());
                            particion.start = start;
                            particion.size = size;
                            mbr->PARTICION[contador] = particion;
                            EscribirParticionDisco(path, size, start, &particion);
                            cout << "Se ha creado la partición primaria!" << endl;
                            break;
                        }
                        else
                        {
                            cout << "Se ha alcanzado el límite de particiones primarias." << endl;
                        }
                    }
                    else
                    {
                        cout << "No se pudo crear la partición por insuficiencia de espacio" << endl;
                    }
                }
                else
                {
                    if (contador == 3)
                    {
                        cout << "Se ha llegado al límite de particiones, este disco ya cuenta con 4 particiones." << endl;
                    }
                }
                contador++;
            }
        }
        else
        {
            cout << "Error, Se ha llegado al límite de particiones primarias" << endl;
        }
    }
    else
    {
        cout << "Error, Este nombre ya existe en el disco que se desea crear la partición" << endl;
        ID_ParticionDisco.pop_back();
    }
}

void ParticionExtendida(string fit, int size, string path, string nombre, string tipo)
{
    mbr = ObtenerMBR(path);
    ListaEspaciosLibres.clear();
    ObtenerEspaciosLibres();
    int contador = 0;
    int contadorPrimarias = 0;
    if (ComprobarNombre(nombre, path))
    {
        for (struct partition particion : mbr->PARTICION)
        {
            string n = Chart_String(particion.type, 1);
            if (n == "e")
            {
                contadorPrimarias++;
            }
        }
        if (contadorPrimarias != 1)
        {
            for (struct partition particion : mbr->PARTICION)
            {

                if (particion.size == 0)
                {
                    //PruebaUbicacion(path);
                    int start = UbicacionSegunAjuste(fit, size);
                    if (start != -1)
                    {
                        if (contadorPrimarias < 1)
                        {

                            string status = "u";
                            strcpy(particion.status, status.c_str());
                            strcpy(particion.type, tipo.c_str());
                            strcpy(particion.fit, fit.c_str());
                            strcpy(particion.nombre, nombre.c_str());
                            particion.start = start;
                            particion.size = size;
                            mbr->PARTICION[contador] = particion;
                            EscribirParticionDisco(path, size, start, &particion);
                            cout << "Se ha creado la partición extendida!" << endl;
                            break;
                        }
                        else
                        {
                            cout << "Se ha alcanzado el límite de particiones extendidas." << endl;
                        }
                    }
                    else
                    {
                        cout << "No se pudo crear la partición por insuficiencia de espacio" << endl;
                    }
                }
                else
                {
                    if (contador == 3)
                    {
                        cout << "Se ha llegado al límite de particiones, este disco ya cuenta con 4 particiones." << endl;
                    }
                }
                contador++;
            }
        }
        else
        {
            cout << "Error, Se ha llegado al límite de particiones extendidas" << endl;
        }
    }
    else
    {
        cout << "Error, Este nombre ya existe en el disco que se desea crear la partición" << endl;
        ID_ParticionDisco.pop_back();
    }
}

void ParticionLogica(string fit, int size, string path, string nombre, string tipo)
{
    int contador = 0;
    mbr = ObtenerMBR(path);
    int inicioParticion = 0;
    int finalParticion = 0;
    if (ComprobarNombre(nombre, path))
    {
        for (struct partition particion : mbr->PARTICION)
        {
            string n = Chart_String(particion.type, 1);
            if (n == "e")
            {
                inicioParticion = particion.start + sizeof(particion) + 1;
                finalParticion = particion.start + particion.size - 1;
                ebr = ObtenerEBR(path, inicioParticion);
                inicioParticion = ObtenerStartEBR(path, inicioParticion);
                int tamanoOcupar = finalParticion - size;
                if (tamanoOcupar > 0)
                {
                    FILE *fichero;
                    fichero = fopen(path.c_str(), "r+b");
                    if (fichero)
                    {
                        //Se actualiza el next del la última particion encontrada
                        if (ebr->size != 0)
                        {
                            string stat2 = Chart_String(ebr->status, 1);
                            strcpy(ebr->status, stat2.c_str());
                            string fit2 = Chart_String(ebr->fit, 1);
                            strcpy(ebr->fit, fit2.c_str());
                            string name2 = Chart_String(ebr->nombre, 16);
                            strcpy(ebr->nombre, name2.c_str());
                            ebr->next = inicioParticion;
                            fseek(fichero, ebr->start, 0);
                            fwrite(ebr, sizeof(EBR) - 1, 1, fichero);
                        }

                        //Se escribe la partición
                        string state = "u";
                        char f[1];
                        char tipos[1];
                        char nombres[16];
                        char status[1];
                        strcpy(f, fit.c_str());
                        strcpy(tipos, tipo.c_str());
                        strcpy(nombres, nombre.c_str());
                        strcpy(status, state.c_str());
                        int next = -1;
                        struct EBR *getEBR = EscribirEBR(f, size, status, nombres, inicioParticion, next);

                        fseek(fichero, inicioParticion, 0);
                        fwrite(getEBR, sizeof(EBR) - 1, 1, fichero);
                        fclose(fichero);
                        ebr = ObtenerEBR(path, inicioParticion);
                        cout << "Se ha creado particion Lógica" << endl;
                        string Logica = nombre + "-" + path + "-" + to_string(inicioParticion);
                        listParticionLogica.push_back(Logica);
                    }
                    else
                    {
                        string mensaje = "No se pudo crear el archivo en: " + path;

                        cout << mensaje << endl;
                    }
                    break;
                }
                else
                {
                    cout << "No se pudo crear la partción, espacio insuficiente" << endl;
                }
            }
            else
            {
                if (contador == 3)
                {
                    cout << "No se ha encontrado partición extendida en este disco." << endl;
                }
            }
            contador++;
        }
    }
    else
    {
        cout << "Error, Este nombre ya existe en el disco que se desea crear la partición" << endl;
        ID_ParticionDisco.pop_back();
    }
}

void EditarParticon(string edicion, string tipoEliminar, string name, string path, int cantidadExtender)
{
    bool particionLogica = false;
    bool particionOtras = false;
    mbr = ObtenerMBR(path);
    int cont = 0;
    for (struct partition particion : mbr->PARTICION)
    {
        string n = string(particion.nombre);
        if (n == name.c_str())
        {
            if (edicion == "montar")
            {
                string tipoPart = Chart_String(particion.type, 1);
                string fitPart = Chart_String(particion.fit, 1);
                string status = "m";
                strcpy(particion.status, status.c_str());
                strcpy(particion.type, tipoPart.c_str());
                strcpy(particion.fit, fitPart.c_str());
                mbr->PARTICION[cont] = particion;
                EscribirParticionDisco(path, particion.size, particion.start, &particion);
                cout << "Se montó la partición correctamente!" << endl;
                particionOtras = true;
            }
            else if (edicion == "eliminar")
            {
                int sizeP = particion.size;
                int startP = particion.start;
                string fit = "";
                string nombre = "";
                string tipo = "";
                string status = "";
                strcpy(particion.status, status.c_str());
                strcpy(particion.fit, fit.c_str());
                strcpy(particion.nombre, nombre.c_str());
                strcpy(particion.type, tipo.c_str());
                particion.size = 0;
                particion.start = 0;
                if (tipoEliminar == "fast")
                {
                    mbr->PARTICION[cont] = particion;
                    //Escribiendo en Archivo
                    FILE *fichero;
                    fichero = fopen(path.c_str(), "r+b");
                    //Actualizar MBR
                    fseek(fichero, 0, 0);
                    fwrite(mbr, sizeof(MBR) - 1, 1, fichero);
                    fclose(fichero);
                }
                else if (tipoEliminar == "full")
                {
                    mbr->PARTICION[cont] = particion;
                    EscribirParticionDisco(path, sizeP, startP, &particion);
                }
                else
                {
                    cout << "Error, opción no valida" << endl;
                }
            }
            else if (edicion == "agregar")
            {
                ObtenerEspaciosLibres();
                vector<int> datos;
                //Posiciones de inicio y fin de la partición a extender
                int start = particion.start;
                int fin = start + particion.size - 1;
                if (ListaEspaciosLibres.size() != 0)
                {
                    bool condicion = false;
                    for (auto dato : ListaEspaciosLibres)
                    {
                        datos = getInicioFin(dato);
                        if ((fin + 1) == dato[0])
                        {
                            //Significa que sí hay espacio al lado derecho
                            string tipoPart = Chart_String(particion.type, 1);
                            string fitPart = Chart_String(particion.fit, 1);
                            string status = Chart_String(particion.status, 1);
                            strcpy(particion.status, status.c_str());
                            strcpy(particion.type, tipoPart.c_str());
                            strcpy(particion.fit, fitPart.c_str());
                            particion.size = particion.size + cantidadExtender;
                            mbr->PARTICION[cont] = particion;
                            EscribirParticionDisco(path, particion.size, particion.start, &particion);
                            condicion = true;
                        }
                    }
                    if (!condicion)
                    {
                        cout << "No se puede expandir la partición dado que no existe espacio libre después de la partición." << endl;
                    }
                }
            }

            break;
        }
        else
        {
            if (cont == 3)
            {
                particionLogica = true;
                //cout << "Error, no se ha encontrado la partición con este nombre en el disco" << endl;
            }
        }
        cont++;
    }

    if (particionLogica == true)
    {
        vector<string> aux;
        for (auto x : listParticionLogica)
        {
            aux = getParticionPathDisco(x);
            if (aux[0] == name and aux[1] == path)
            {
                if (edicion == "montar")
                {
                    ebr = ObtenerEBR(path, stoi(aux[2]));
                    string nombrePart = Chart_String(ebr->nombre, 16);
                    string fitPart = Chart_String(ebr->fit, 1);
                    string status = "m";
                    strcpy(ebr->status, status.c_str());
                    strcpy(ebr->nombre, nombrePart.c_str());
                    strcpy(ebr->fit, fitPart.c_str());

                    FILE *fichero;
                    fichero = fopen(path.c_str(), "r+b");
                    if (fichero)
                    {
                        fseek(fichero, stoi(aux[2]), 0);
                        fwrite(mbr, sizeof(EBR) - 1, 1, fichero);
                        fclose(fichero);
                        cout << "Se montó la partición Lobiga correctamente!" << endl;
                    }
                    else
                    {
                        cout << "Error, al montar particion lógica" << endl;
                    }
                }

                else if (edicion == "eliminar")
                {
                    ebr = ObtenerEBR(path, stoi(aux[2]));
                    string fit = "";
                    string nombre = "";
                    string status = "";
                    strcpy(ebr->status, status.c_str());
                    strcpy(ebr->fit, fit.c_str());
                    strcpy(ebr->nombre, nombre.c_str());
                    ebr->next = 0;
                    ebr->size = 0;
                    ebr->start = 0;
                    if (tipoEliminar == "fast" || tipoEliminar == "full")
                    {
                        //Escribiendo en Archivo
                        FILE *fichero;
                        fichero = fopen(path.c_str(), "r+b");
                        if (fichero)
                        {
                            //Actualizar MBR
                            fseek(fichero, stoi(aux[2]), 0);
                            fwrite(mbr, sizeof(EBR) - 1, 1, fichero);
                            fclose(fichero);
                            cout << "Se ha eliminado la partición!" << endl;
                        }
                        else
                        {
                            cout << "Error, al eliminar Particion" << endl;
                        }
                    }
                    else
                    {
                        cout << "Error, opción no valida" << endl;
                    }
                }
                else if (edicion == "agregar")
                {
                    cout << "Perdón no puse add para las particiones Lógicas :(" << endl;
                }

                break;
            }
        }
    }
    else
    {
        if (particionOtras == false)
        {
            cout << "Error, no se ha encontrado la partición con este nombre en el disco" << endl;
        }
    }
}

void fdisk(int size, string u, string path, string type, string f, string instruccion, int extiende, string name)
{

    if (u == "m")
    {
        size = size * (1024 * 1024);
        extiende = extiende * (1024 * 1024);
    }
    else if (u == "k")
    {
        size = size * 1024;
        extiende = extiende * 1024;
    }

    //Determinar si se crea, elimina o extiende
    if (instruccion == "NO")
    { //Crear
        if (type == "p")
        { //Primaria
            ParticionPrimaria(f, size, path, name, "p");
        }
        else if (type == "e")
        { //Extendida
            ParticionExtendida(f, size, path, name, "e");
        }
        else if (type == "l")
        {
            ParticionLogica(f, size, path, name, "l");
        }
        else
        {
            cout << "Este valor no corresponde a ningún tipo de partición, se creará una partición primaria en su lugar." << endl;
            ParticionPrimaria(f, size, path, name, "p");
        }
    }
    else if (instruccion == "fast" || instruccion == "full")
    { //Elimina
        EditarParticon("eliminar", instruccion, name, path, 0);
    }
    else if (instruccion == "add")
    { //Extiende
        EditarParticon("agregar", "", name, path, extiende);
    }
    else
    {
        cout << "Error, no se reconoce la opción de eliminación elegida." << endl;
    }
}

void mount(string path, string name)
{
    string ID = GenerarIDParticionMontada(path, name);
    string Montar = "";
    if (ID != "NO")
    {
        mbr = ObtenerMBR(path);
        Montar = ID + "-" + name + "-" + path;
        ParticionMontada.push_back(Montar);
        EditarParticon("montar", "", name, path, 0);
        mbr = ObtenerMBR(path);
        cout << ID << endl;
    }
    else
    {
        cout << "No se puede montar, no existe una particion con este nombre" << endl;
    }
}

void Unmount(string id)
{
    vector<string> aux; // id- nombre- path
    int contador = 0;
    for (auto x : ParticionMontada)
    {
        aux = getParticionPathDisco(x);

        if (aux[0] == id)
        {
            ParticionMontada[contador] = "a-b-c";
        }
        contador++;
    }
}


#endif