#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include "../Analizador/SplitComandos.h"
//#include "../Ejecucion/Ejecucion.h"
#include <fstream>

using namespace std;

#ifndef ANALIZADOR_H
#define ANALIZADOR_H

vector<string> Comando;
int indic = 0;

class Analizador
{
public:
    Analizador()
    {
    }
    ~Analizador(){};
};

string ObtenerPath(string path)
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

void Exec(string path)
{
    ifstream archivo;
    string texto;
    string contenido = "";
    archivo.open(path, ios::in);
    if (archivo.fail())
    {
        cout << "No se pudo abrir el archivo";
        exit(1);
    }

    while (!archivo.eof())
    {
        getline(archivo, texto);
        //cout<<texto<<endl;
        if (contenido == "")
        {
            contenido = texto;
        }
        else
        {
            contenido = contenido + "\n" + texto;
        }
    }
    archivo.close();
    Start(contenido);
}

void exec(vector<string> vector)
{
    try
    {
        string path = "";

        string atributo = vector[indic];
        transform(atributo.begin(), atributo.end(), atributo.begin(), ::tolower);
        if (atributo == "-path")
        {
            indic++;
            path = vector[indic];
        }
        else
        {
            cout << "Se esperaba el atributo path" << endl;
        }
        if (path != "")
        {
            path = ObtenerPath(path);
            Exec(path);
        }
    }
    catch (const std::exception &e)
    {
        if (indic < 3)
        {
            cout << "Error, no cumple con el formato de atributos. Falta un atributo o el valor de un atributo" << endl;
        }
        else
        {
            cout << "Error: " << &e << endl;
        }
    }
}

void execute(vector<string> vector)
{
    string cmd = vector[0];
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "exec")
    {
        indic++;
        exec(vector);
        indic = 0;
    }
    else
    {
        ExecEjecutar(vector);
        //cout << "ERROR, el comando: " << cmd << " no se ha encontrado." << endl;
    }
}

void Comprobar(vector<string> Vectro)
{
    cout << "Lista de comandos a enviar----------------------------" << endl;
    for (auto x : Vectro)
    {
        cout << x << endl;
    }
}

void SplitPorEspacios(string Entrada)
{
    string palabra = "";
    bool cadena = false;
    bool comentario = false;
    for (auto x : Entrada)
    {
        //if (x != '%')
        //{
            if (x == '"' && cadena != true) //Entro si x  es "" y si cadena no es verdadera
            {
                cadena = true;
                palabra = palabra + x;
            }
            else
            {
                if (cadena == false)
                {
                    if (x == ' ')
                    {
                        if (palabra != "")
                        {
                            Comando.push_back(palabra);
                        }
                        palabra = "";
                    }
                    else
                    {
                        palabra = palabra + x;
                    }
                }
                else
                {
                    palabra = palabra + x;
                    if (x == '"')
                    {
                        cadena = false;
                        Comando.push_back(palabra);
                        palabra = "";
                    }
                }
            }
        //}
        //else
        //{
        //    comentario = true;
        //}
    }
    //if (comentario != true)
    //{
        if (palabra != " " && palabra != "")
        {
            Comando.push_back(palabra);
        }

        execute(Comando);
        //Comprobar(Comando);
        Comando.clear();
    //}
}

//Separa por valores
void SplitPorComandos(string Entrada)
{
    string palabra = "";
    bool comentario = false;
    for (auto x : Entrada)
    {
        //if (x != '#')
        //{
            if (x == '=')
            {
                if (palabra != "")
                {
                    palabra = palabra + " ";
                }
            }
            else
            {
                palabra = palabra + x;
            }
        //}
        //else
        //{
        //    comentario = true;
        //    //palabra= palabra+x;
        //}
    }
    //if (comentario != true)
    //{
        if (palabra != " " && palabra != "")
        {
            palabra = palabra + " ";
        }

        SplitPorEspacios(palabra);
    //}
}

//Separa por cada salto de línea
void SplitPorLineas(string Entrada)
{
    string palabra = "";
    for (auto x : Entrada)
    {
        if (x == '\n')
        {
            cout << palabra << endl;
            if (palabra != "")
            {
                SplitPorComandos(palabra);
            }
            palabra = "";
        }
        else
        {
            palabra = palabra + x;
        }
    }
    if (palabra != "") // En caso no vengan más lineas
    {
        cout << palabra << endl;
        SplitPorComandos(palabra);
    }
}

void Inicio(string Entrada)
{
    Comando.clear();
    SplitPorLineas(Entrada);
}

#endif