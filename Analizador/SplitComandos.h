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

void SplitFinal2(string Entrada){
    string palabra="";
    for (int i = 0; i < Entrada.size(); i++)
    {
        if(Entrada[i]=='"'){
            do
            {
                palabra=palabra+Entrada[i];
                i++;
            } while (Entrada[i]!='"');   
            if(palabra!=""){
                Comando2.push_back(palabra);   
            }
            palabra="";    
            
        }else if(Entrada[i]=='#'){
            do
            {
                palabra=palabra+Entrada[i];
                i++;
            } while (i<Entrada.size());
            if(palabra!=""){
                Comando2.push_back(palabra);
            }
            palabra="";
        }
        else{
            if(Entrada[i]!=' '){
                palabra=palabra+Entrada[i];
            }else{
                if(palabra!=""){
                    Comando2.push_back(palabra);
                }
                palabra="";
            }
        }        
    }
    Ejecutar(Comando2);
    Comando2.clear();
}

void SplitPorComandos2(string Entrada)
{
    string palabra = "";
    bool comentario = false;
    for (auto x : Entrada)
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

    if (palabra != " " && palabra != "")
    {
        palabra = palabra + " ";
    }
    SplitFinal2(palabra);
    //SplitPorEspacios2(palabra);
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