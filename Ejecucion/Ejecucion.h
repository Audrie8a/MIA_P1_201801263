#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include "../Ejecucion/Comandos.h"
#include <sys/stat.h>
#include <stdio.h>
using namespace std;

#ifndef EJECUCION_H
#define EJECUCION_H

int indice = 0;
vector<string> CMD;
vector<string> RutaAux;
class Ejecucion
{
public:
    Ejecucion()
    {
    }
    ~Ejecucion(){};
};

string SplitRutaSinNombreArchivo(string path)
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

//OBTIENE LA RUTA SIN EL NOMBRE DEL ARCHIVO
string SplitRuta(string path)
{
    string palabra = "";
    RutaAux.clear();
    for (auto x : path)
    {
        if (x == '/')
        {
            if (palabra != "")
            {
                RutaAux.push_back(palabra);
            }
            palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    int contador = 1;
    int tamaRuta = RutaAux.size();
    palabra = "";
    for (auto z : RutaAux)
    {

        if (contador != tamaRuta)
        {
            palabra += "/" + z;
        }
    }
    palabra = '"' + palabra + '"';
    return palabra;
}

//VERIFICA SI EXISTE LA RUTA
bool VerificarExistencia(string path)
{
    string aux = "";
    if (path[0] == '"')
    {
        for (auto x : path)
        {
            if (x != '"')
            {
                aux = aux + x;
            }
        }
    }
    path = aux;
    bool existencia = false;
    struct stat buffer;
    int exists = stat(path.c_str(), &buffer);
    if (exists == 0)
    {
        existencia = true;
    }
    return existencia;
}

//CREA LA RUTA EN CASO NO EXISTA
void VerificarPath(string path)
{
    path = SplitRuta(path);
    if (!VerificarExistencia(path))
    {
        string comando = "mkdir -m 777 " + path;
        system(comando.c_str());
    }
}

//Obtiene una cadena sin comillas dobles
string ObtenerPaths(string path)
{
    string palabra = "";
    for (auto x : path)
    {
        if (x == '"')
        {
            //palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    return palabra;
}

//Crear Discos
void Mkdisk()
{
    bool error = false;
    int size = 0;
    string f = "";
    string u = "";
    string path = "";

    try
    {
        int CMDsize = CMD.size();
        while (indice < CMDsize)
        {

            string cmd = CMD[indice];
            transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

            if (cmd == "-size")
            {
                indice++;
                size = stoi(CMD[indice]);
            }
            else if (cmd == "-f")
            {
                indice++;
                f = CMD[indice];
            }
            else if (cmd == "-u")
            {
                indice++;
                u = CMD[indice];
            }
            else if (cmd == "-path")
            {
                indice++;
                path = ObtenerPaths(CMD[indice]);
            }
            else if (cmd[0] == '#')
            {
                //Comentario
                //cout << "Comentario" << endl;
                indice = (CMD.size() - 1);
            }
            else
            {
                cout << "Error, atributo no identificado para este comando" << endl;
                error = true;
                break;
            }
            indice++;
        }

        if (!error)
        {
            if (size == 0 && path == "")
            {
                cout << "Error, no cuenta con atributos esenciales para crear Disco" << endl;
            }
            else
            {
                if (f == "")
                {
                    f = "f";
                }
                transform(f.begin(), f.end(), f.begin(), ::tolower);
                if (f == "bf")
                {
                    f = "b";
                }
                else if (f == "ff")
                {
                    f = "f";
                }
                else if (f == "wf")
                {
                    f = "w";
                }
                if (u == "")
                {
                    u = "m";
                }
                VerificarPath(path);
                transform(u.begin(), u.end(), u.begin(), ::tolower);

                mkdisk(size, f, u, path);
            }
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear el disco" << endl;
    }
}

void Rmdisk()
{
    string path = "";

    string cmd = CMD[indice];
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    try
    {
        if (cmd == "-path")
        {
            indice++;
            path = CMD[indice];
        }

        if (path != "")
        {
            path = ObtenerPaths(path);
            //Comprobar existencia del archivo
            FILE *arch;
            arch = fopen(path.c_str(), "rb");
            if (arch)
            {
                //Existe el archivo
                remove(path.c_str());
                cout << "Disco Eliminado!" << endl; 
                fclose(arch);               
            }
            else
            {
                cout << "Error, archivo no encontrado!" << endl;                
            }
            
        }
        else
        {
            cout << "Error, atributo no identificado para este comando" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Error al eliminar el disco" << endl;
        cout << e.what() << endl;
    }
}

void pause()
{
    do
    {
        cout << '\n'
             << "Press the Enter key to continue.";
    } while (cin.get() != '\n');
}

//Aquí va la el listado de comandos que se pueden ejecutar
void Ejecutar(vector<string> vector)
{
    CMD = vector;
    string cmd = vector[0];
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "mkdisk")
    {
        indice++;
        Mkdisk();
        indice = 0;
    }
    else if (cmd == "rmdisk")
    {
        indice++;
        Rmdisk();
        indice = 0;
    }
    else if (cmd == "fdisk")
    {
        indice++;
        //Fdisk();
        indice = 0;
    }
    else if (cmd == "rep")
    {
        indice++;
        //Rep();
        indice = 0;
    }
    else if (cmd == "mount")
    {
        indice++;
        //Mount();
        indice = 0;
    }
    else if (cmd == "unmount")
    {
        indice++;
        //Unmount();
        indice = 0;
    }
    else if (cmd == "mkfs")
    {
        indice++;
        indice = 0;
    }
    else if (cmd == "login")
    {
        indice++;
        indice = 0;
    }
    else if (cmd == "logout")
    {
        indice++;
        indice = 0;
    }
    else if (cmd == "mkgrp")
    {
        indice++;
        indice = 0;
    }
    else if (cmd == "rmgrp")
    {
        indice++;
        indice = 0;
    }
    else if (cmd == "mkusr")
    {
        indice++;
        indice = 0;
    }
    else if (cmd == "rmusr")
    {
        indice++;
        indice = 0;
    }
    else if (cmd == "pause")
    {
        pause();
        indice = 0;
    }
    else if (cmd[0] == '#')
    {
        //Comentario
        //cout << "Comentario" << endl;
        indice = 0;
    }
    else
    {
        cout << "Error, comando no encontrado" << endl;
    }
}
#endif