#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include "../Ejecucion/Ejecucion.h"
#include <fstream>
using namespace std;

#ifndef SPLITCOMANDOS_H
#define SPLITCOMANDOS_H

vector<string> Comando2;

class SplitComandos
{
public:
    SplitComandos() {}

    ~SplitComandos() {}
};

void ExecEjecutar(vector<string> vector)
{
    Ejecutar(vector);
}

void Comprobar2(vector<string> Vectro)
{
    cout << "Lista de comandos a enviar----------------------------" << endl;
    for (auto x : Vectro)
    {
        cout << x << endl;
    }
}

void SplitPorEspacios2(string Entrada)
{
    string palabra = "";
    bool cadena = false;
    bool comentario = false;
    for (auto x : Entrada)
    {
        if (x != '#')
        {
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
                            Comando2.push_back(palabra);
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
                        Comando2.push_back(palabra);
                        palabra = "";
                    }
                }
            }
        }
        else
        {
            comentario = true;
        }
    }
    if (comentario != true)
    {
        if (palabra != " " && palabra != "")
        {
            Comando2.push_back(palabra);
        }
        //Comprobar2(Comando2);
        Ejecutar(Comando2);
        Comando2.clear();
    }
}

void SplitPorComandos2(string Entrada)
{
    string palabra = "";
    bool comentario = false;
    for (auto x : Entrada)
    {
        if (x != '#')
        {
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
        }
        else
        {
            comentario = true;
        }
    }
    if (comentario != true)
    {
        if (palabra != " " && palabra != "")
        {
            palabra = palabra + " ";
        }

        SplitPorEspacios2(palabra);
    }
}
void SplitPorLineas2(string Entrada)
{
    string palabra = "";
    for (auto x : Entrada)
    {
        if (x == '\n')
        {
            cout << palabra << endl;
            if (palabra != "")
            {
                SplitPorComandos2(palabra);
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
        cout << palabra << endl;
        SplitPorComandos2(palabra);
    }
}

void Start(string Entrada)
{
    Comando2.clear();
    SplitPorLineas2(Entrada);
}
#endif