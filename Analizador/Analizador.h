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

void SplitFinal(string Entrada){
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
                Comando.push_back(palabra);   
            }
            palabra="";    
            
        }else if(Entrada[i]=='#'){
            do
            {
                palabra=palabra+Entrada[i];
                i++;
            } while (i<Entrada.size());
            if(palabra!=""){
                Comando.push_back(palabra);
            }
            palabra="";
        }
        else{
            if(Entrada[i]!=' '){
                palabra=palabra+Entrada[i];
            }else{
                if(palabra!=""){
                    Comando.push_back(palabra);
                }
                palabra="";
            }
        }        
    }
    execute(Comando);
    Comando.clear();
}

//Separa por valores
void SplitPorComandos(string Entrada)
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

    SplitFinal(palabra);
    //SplitPorEspacios(palabra);
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