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

string SinComillas(string texto)
{
    texto.erase(0, 1);
    texto.erase(texto.length() - 1, texto.length());
    return texto;
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
    if (aux != "")
    {
        path = aux;
    }

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
        //string comando = "mkdir -m 777 " + path;
        string comando = "mkdir -p " + path;
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
                if (size >= 0)
                {
                    mkdisk(size, f, u, path);
                }
                else
                {
                    cout << "Error, size negativo!" << endl;
                }
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
            arch = fopen(path.c_str(), "r");
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

void Fdisk()
{
    bool error = false;
    int size = 0;
    string f = "";
    string u = "";
    string path = "";
    string type = "";
    string operacion = "ninguna";
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
                    operacion = "delete";
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
                    operacion = "add";
                }
                else
                {
                    cout << "Ya se encontró el parametro Delete anteriormente";
                    indice++;
                }
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
            if (path != "" && name != "")
            {
                if (size >= 0)
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
                        if (operacion == "add")
                        {
                            fdisk(size, u, path, type, f, name, add, "", operacion);
                        }
                        else if (operacion == "delete")
                        {
                            transform(deleted.begin(), deleted.end(), deleted.begin(), ::tolower);
                            fdisk(size, u, path, type, f, name, 0, deleted, operacion);
                        }
                        else
                        {
                            if (size != 0)
                            {
                                fdisk(size, u, path, type, f, name, 0, "", operacion);
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
                    cout << "Error, size es negativo!" << endl;
                }
            }
            else
            {
                cout << "Error, no cuenta con atributos esenciales para crear Particion" << endl;
            }
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
            //Comprobar existencia del archivo
            FILE *arch;
            arch = fopen(path.c_str(), "rb");
            if (arch)
            {
                mount(path, name);
                fclose(arch);
            }
            else
            {
                cout << "Error, archivo no encontrado!" << endl;
            }
        }
        else
        {
            cout << "Error, faltan atributos necesarios!" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear el disco" << endl;
    }
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

void Mkfs()
{
    string id = "";
    string type = "";
    string fs = "";
    try
    {
        int CMDsize = CMD.size();
        while (indice < CMDsize)
        {

            string cmd = CMD[indice];
            transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
            if (cmd == "-id")
            {
                indice++;
                id = CMD[indice];
            }
            else if (cmd == "-type")
            {
                indice++;
                type = CMD[indice];
            }
            else if (cmd == "-fs")
            {
                indice++;
                fs = CMD[indice];
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
        if (id != "")
        {
            transform(id.begin(), id.end(), id.begin(), ::tolower);
            transform(type.begin(), type.end(), type.begin(), ::tolower);
            transform(fs.begin(), fs.end(), fs.begin(), ::tolower);
            if (type == "")
            {
                type = "full";
            }
            if (fs == "")
            {
                fs = "2fs";
            }
            mkfs(id, type, fs);
        }
        else
        {
            cout << "Error, falta id" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear el disco" << endl;
    }
}

void Login()
{
    string usuario = "";
    string password = "";
    string id = "";
    try
    {
        int CMDsize = CMD.size();
        while (indice < CMDsize)
        {

            string cmd = CMD[indice];
            transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
            if (cmd == "-usuario")
            {
                indice++;
                usuario = CMD[indice];
            }
            else if (cmd == "-password")
            {
                indice++;
                password = CMD[indice];
            }
            else if (cmd == "-id")
            {
                indice++;
                id = CMD[indice];
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
        if (usuario != "" && password != "" && id != "")
        {
            transform(id.begin(), id.end(), id.begin(), ::tolower);

            login(usuario, password, id);
        }
        else
        {
            cout << "Error, falta id" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear el disco" << endl;
    }
}

void Loss()
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
        loss(id);
    }
    else
    {
        cout << "Falta el atributo id" << endl;
    }
}

void Recovery()
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
        recovery(id);
    }
    else
    {
        cout << "Falta el atributo id" << endl;
    }
}

void Mkusr()
{
    string usr = "";
    string pwd = "";
    string grp = "";
    try
    {
        int CMDsize = CMD.size();
        while (indice < CMDsize)
        {

            string cmd = CMD[indice];
            transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
            if (cmd == "-usr")
            {
                indice++;
                usr = CMD[indice];
            }
            else if (cmd == "-pwd")
            {
                indice++;
                pwd = CMD[indice];
            }
            else if (cmd == "-grp")
            {
                indice++;
                grp = CMD[indice];
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
        if (usr != "" && pwd != "" && grp != "")
        {
            mkusr(usr, pwd, grp);
        }
        else
        {
            cout << "Error, falta id" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear el disco" << endl;
    }
}

void Mkgrp()
{
    string name = "";
    string cmd = CMD[indice];
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    if (cmd == "-name")
    {
        indice++;
        name = CMD[indice];
    }

    if (name != "")
    {
        //mkgrp(id);
    }
    else
    {
        cout << "Falta el atributo id" << endl;
    }
}

void Mkfile()
{
    string path = "";
    bool r = false;
    int size = 0;
    string cont = "";
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
            else if (cmd == "-cont")
            {
                indice++;
                cont = CMD[indice];
            }
            else if (cmd == "-r")
            {
                indice++;
                r = true;
            }
            else if (cmd == "-size")
            {
                indice++;
                size = stoi(CMD[indice]);
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
        if (path != "")
        {
            if (path[0] == '"')
            {
                path = SinComillas(path);
            }
            if (cont[0] == '"')
            {
                cont = SinComillas(cont);
            }
            if (size >= 0)
            {
                mkfile(path,r,size,cont);
            }
            else
            {
                cout << "Error, size negativo!" << endl;
            }
        }
        else
        {
            cout << "Error, falta id" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Ha sucedido un error al obtener los datos para crear el disco" << endl;
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
        if (name != "" && path != "" && id != "")
        {
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            path = ObtenerPaths(path);
            name = ObtenerPaths(name);
            ruta = ObtenerPaths(name);
            VerificarPath(path);
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
                //repJournaling(path, id);
            }
            else if (name == "block")
            {
                repBlock(path, id);
            }
            else if (name == "bm_inode")
            {
                //repBm_inode(path, id);
            }
            else if (name == "bm_block")
            {
                //repBm_block(path, id);
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
                //repFile(path, id);
            }
            else if (name == "ls")
            {
                //repLs(path, id);
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
        Mkfs();
        indice = 0;
    }
    else if (cmd == "login")
    {
        indice++;
        Login();
        indice = 0;
    }
    else if (cmd == "logout")
    {
        indice++;
        logout();
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
    else if (cmd == "loss")
    {
        indice++;
        Loss();
        indice = 0;
    }
    else if (cmd == "recovery")
    {
        indice++;
        Recovery();
        indice = 0;
    }else if (cmd == "mkfile")
    {
        indice++;
        Mkfile();
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