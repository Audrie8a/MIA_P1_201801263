#include <iostream>
#include <cstring>
#include "Analizador/Analizador.h"
using namespace std;

char comando[10000];
void Consola(){
    bool condicion= true;
    
    do
    {
        cout << "Esperando Instruccion"<<endl;
        cin.getline(comando,10000);
        
        string s(comando);
        if (s=="salir"){
            condicion=false;
        }else{
            //string Prueba = "exec -path=/media/audrie8a/Archivos/C/Proyecto1_MIA/Archiva.txt";//\"/media/audrie8a/Archivos/C/Proyecto1_MIA/Archivo Entrada.txt\"";
            string Prueba="mount -name=Part1 #Comentario \n #Otro";
            
            //Inicio(comando);
            Inicio(Prueba);
            
        }       
        
    } while (condicion);
    
    cout<<"Saliendo..."<<endl;    
}


int main(){
    cout << "Bienvenido"<< endl;
    Consola();
    
}
