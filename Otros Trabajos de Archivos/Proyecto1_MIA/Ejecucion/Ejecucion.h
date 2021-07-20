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

    return palabra;
}

//Verifica la existencia de un archivo o ruta
bool VerificarExistencia(string path)
{
    bool existencia = false;
    struct stat buffer;
    int exists = stat(path.c_str(), &buffer);
    if (exists == 0)
    {
        existencia = true;
    }
    return existencia;
}

//Crea la ruta en caso que no exista.
void VerificarPath(string path)
{
    path = SplitRuta(path);
    if (!VerificarExistencia(path))
    {

        //cout << path << endl;
        string comando = "mkdir -m 777" + path;
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


void Mkdisk()
{
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
            }
            indice++;
        }

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
            cout << "Disco Creado!" << endl;
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
            remove(path.c_str());
            cout << "Disco Eliminado!" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Error al eliminar el disco" << endl;
        cout << e.what() << endl;
    }
}

void Fdisk()
{
    int size = 0;
    string f = "";
    string u = "";
    string path = "";
    string type = "";
    string deleted = "";
    int add = 0;
    string name = "";
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
            else if (cmd == "-type")
            {
                indice++;
                type = CMD[indice];
            }
            else if (cmd == "-delete")
            {
                indice++;
                if (add == 0)
                {
                    deleted = CMD[indice];
                }
                else
                {
                    cout << "Ya se encontró el parametro Add anteriormente";
                    indice++;
                }
            }
            else if (cmd == "-name")
            {
                indice++;
                name = CMD[indice];
            }
            else if (cmd == "-add")
            {
                indice++;
                if (deleted == "")
                {
                    add = stoi(CMD[indice]);
                }
                else
                {
                    cout << "Ya se encontró el parametro Delete anteriormente";
                    indice++;
                }
            }else if (cmd[0] == '#')
            {
                //Comentario
                //cout << "Comentario" << endl;
                indice=(CMD.size()-1);
            }
            else
            {
                cout << "Error, atributo no identificado para este comando" << endl;
            }
            indice++;
        }

        if (path != "" && name != "")
        {
            if (VerificarExistencia(path))
            {
                if (u == "")
                {
                    u = "k";
                }
                if (type == "")
                {
                    type = "p";
                }
                if (f == "")
                {
                    f = "w";
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
                transform(type.begin(), type.end(), type.begin(), ::tolower);
                transform(u.begin(), u.end(), u.begin(), ::tolower);
                name = ObtenerPaths(name);
                path = ObtenerPaths(path);
                if (add != 0)
                {
                    fdisk(size, u, path, type, f, "add", add, name);
                }
                else if (deleted != "")
                {
                    transform(deleted.begin(), deleted.end(), deleted.begin(), ::tolower);
                    fdisk(size, u, path, type, f, deleted, 0, name);
                }
                else
                {
                    if (size != 0)
                    {
                        fdisk(size, u, path, type, f, "NO", 0, name);
                    }
                    else
                    {
                        cout << "Error, no se ha encontrado el disco en el path dado" << endl;
                    }
                }
            }
            else
            {
                cout << "Error, no se ha encontrado el disco en el path dado" << endl;
            }
        }
        else
        {
            cout << "Error, no cuenta con atributos esenciales para crear Particion" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear la particion" << endl;
    }
}

void Mount()
{
    string path = "";
    string name = "";
    try
    {
        int CMDsize = CMD.size();
        while (indice < CMDsize)
        {

            string cmd = CMD[indice];
            transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
            if (cmd == "-path")
            {
                indice++;
                path = CMD[indice];
            }
            else if (cmd == "-name")
            {
                indice++;
                name = CMD[indice];
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
            }
            indice++;
        }
        if (path != "" && name != "")
        {
            path = ObtenerPaths(path);
            name = ObtenerPaths(name);
            mount(path, name);
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear el disco" << endl;
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

void Unmount()
{
    string id = "";
    string cmd = CMD[indice];
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    if (cmd == "-id")
    {
        indice++;
        id = CMD[indice];
    }

    if (id != "")
    {
        Unmount(id);
    }
    else
    {
        cout << "Falta el atributo id" << endl;
    }
}

void Rep()
{
    string path = "";
    string name = "";
    string id = "";
    string ruta = "";
    try
    {
        int CMDsize = CMD.size();
        while (indice < CMDsize)
        {

            string cmd = CMD[indice];
            transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
            if (cmd == "-name")
            {
                indice++;
                name = CMD[indice];
            }
            else if (cmd == "-path")
            {
                indice++;
                path = CMD[indice];
            }
            else if (cmd == "-id")
            {
                indice++;
                id = CMD[indice];
            }
            else if (cmd == "-ruta")
            {
                indice++;
                ruta = CMD[indice];
            }else if (cmd[0] == '#')
            {
                //Comentario
                //cout << "Comentario" << endl;
                indice=(CMD.size()-1);
            }
            else
            {
                cout << "Error, atributo no identificado para este comando" << endl;
            }
            indice++;
        }
        if (name != "" && path != "" && id != "")
        {
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            path = ObtenerPaths(path);
            name = ObtenerPaths(name);
            ruta = ObtenerPaths(name);
            VerificarPath(SplitRutaSinNombreArchivo(path));
            if (name == "mbr")
            {
                repMBR(path, id);
            }
            else if (name == "disk")
            {
                repDisk(path, id);
            }
            else if (name == "inode")
            {
                repInode(path, id);
            }
            else if (name == "journaling")
            {
                repJournaling(path, id);
            }
            else if (name == "block")
            {
                repBlock(path, id);
            }
            else if (name == "bm_inode")
            {
                repBm_inode(path, id);
            }
            else if (name == "bm_block")
            {
                repBm_block(path, id);
            }
            else if (name == "tree")
            {
                repTree(path, id);
            }
            else if (name == "sb")
            {
                repSb(path, id);
            }
            else if (name == "file")
            {
                repFile(path, id);
            }
            else if (name == "ls")
            {
                repLs(path, id);
            }
            else
            {
                cout << "Error, no se reconoce ningún reporte a este name" << endl;
            }
        }
        else
        {
            cout << "Error, no cuenta con los atributos esenciales para generar el reporte" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para generar reportes" << endl;
    }
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
        Fdisk();
        indice = 0;
    }
    else if (cmd == "rep")
    {
        indice++;
        Rep();
        indice = 0;
    }
    else if (cmd == "mount")
    {
        indice++;
        Mount();
        indice = 0;
    }
    else if (cmd == "unmount")
    {
        indice++;
        Unmount();
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