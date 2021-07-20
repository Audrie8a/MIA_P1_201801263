#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Estructuras/MBR.h"
#include <fstream>

using namespace std;

struct MBR *mbr=new struct MBR;
void Graficar(string Contenido){
    Contenido="digraph G {";
  Contenido+="{"; 
   Contenido+=" node [margin=0 fontcolor=blue fontsize=32 width=0.5 shape=circle style=filled]";
    Contenido+="b [fillcolor=yellow fixedsize=true label=\"a very long label\"]";
    Contenido+="d [fixedsize=shape label=\"an even longer label\"]";
  Contenido+="}";
  Contenido+="a -> {c d}";
  Contenido+="b -> {c d}";
Contenido+="}";
    ofstream file;
    file.open("/home/audrie8a/Reportes/disco.dot");
    file<<Contenido;    
    file.close();

    char instruccion[50];
    instruccion[0]='\0';
    strcat(instruccion,"dot -Tpng -O disco.dot");
    system(instruccion);
    instruccion[0]='\0';
    strcat(instruccion,"xdg-open disco.dot.png");
    system(instruccion);	
}

string Chart_String(char* arreglo, int limite){
    string Cadena="";
    for(int i=0; i<limite; i++){
        Cadena=Cadena+arreglo[i];
    }
    return Cadena;
}



void ReporteDisco(){
    

}

void ReporteMBR(){
    string contenido="digraph G  {";
	contenido += " graph [pad=\"0.5\", nodesep=\"0.5\", ranksep=\"2\"];";
	contenido += "node [shape=record, fontname=\"Arial\"];";

    contenido += " set1 [label = \"{Nombre |mbr_tamaño | mbr_fecha_creacion | mbr_disk_signature |mbr_disk_fit |part_status_1| part_type_1| part_fit_1| part_start_1| part_size_1| part_name_1 | part_status_2| part_type_2| part_fit_2| part_start_2| part_size_2| part_name_2 | part_status_3| part_type_3| part_fit_3| part_start_3| part_size_3| part_name_3 |part_status_4| part_type_4| part_fit_4| part_start_4| part_size_4| part_name_4 }| { Valor ";
	contenido += "|" + to_string(mbr->SIZE);
    string Fecha=mbr->FECHA;
	contenido += "|" + Fecha;
	contenido += "|" + to_string(mbr->SIGNATURE);
    string Fit = mbr->FIT;
	contenido += "|" + Fit;

    //Escribiendo información de particiones
    for(struct partition particion: mbr->PARTICION){
        string status=Chart_String(particion.status,1);
        contenido += "|" + status;
        string tipo=Chart_String(particion.type,1);
		contenido += "|" + tipo;
        string fitP=Chart_String(particion.fit,2);
		contenido += "|" + fitP;
		contenido += "|" + to_string(particion.start);
		contenido += "|" + to_string(particion.size);
        string NameP=particion.nombre;
		contenido += "|" + NameP;
    }

    contenido += "}\"];}";

    Graficar(contenido);

}

struct MBR *ObtenerMBR(){
    FILE *arch;
    arch=fopen("/media/audrie8a/Archivos/C/HT1_201801263/disco.dk","rb");
    struct MBR *newMBR= new struct MBR;
    //Fichero, posición, En que dirección empieza a leer
    fseek(arch,0,SEEK_SET); //Seek_set o "0", es desede el inicio del archivo
    fread(newMBR, sizeof(MBR), 1, arch);
    cout<<newMBR->SIGNATURE<<endl;
    cout<<newMBR->FIT<<endl;
    cout<<newMBR->SIZE<<endl;
    cout<<newMBR->FECHA<<endl;    
    fclose(arch);

    return newMBR;
}

struct MBR *EscribirMBR(int signature, char *fit, int size ){
    struct MBR *newMBR= new struct MBR;
    newMBR->SIGNATURE=signature;
    newMBR->SIZE=size;
    strcpy(newMBR->FIT, fit);
    time_t hora = time(0);
    char fecha[25];//ctime devuelve 26 caracteres pero tambien se podría usar un puntero de char
    time_t current_time;
    current_time=time(NULL);
    ctime(&current_time);
    strcpy(fecha, ctime(&current_time));    
    strcpy(newMBR->FECHA,fecha);

    return newMBR;

}


void CrearParticionPrimaria(){
    mbr=ObtenerMBR();
    int contador=0;
    int tama=0;
    for(struct partition particion: mbr->PARTICION){
        if (contador==0){
            strcpy(particion.status,"M");
            strcpy(particion.type,"P");
            strcpy(particion.fit,"BF");
            tama= sizeof(MBR);
            particion.size=sizeof(partition);
            particion.start=tama;
            strcpy(particion.nombre,"Particion1");
            mbr->PARTICION[0]= particion;
            

        }else if(contador==1){
            tama=tama+sizeof(particion)+20;
            strcpy(particion.status,"U");
            strcpy(particion.type,"P");
            strcpy(particion.fit,"WF");
            particion.size=sizeof(partition)+20;
            particion.start=tama;
            strcpy(particion.nombre,"Particion2");
            mbr->PARTICION[1]= particion;

        }else if (contador==2){
            tama=tama+sizeof(particion)+30;
            strcpy(particion.status,"U");
            strcpy(particion.type,"P");
            strcpy(particion.fit,"WF");
            particion.size=sizeof(partition)+30;
            particion.start=tama;
            strcpy(particion.nombre,"Particion3");
            mbr->PARTICION[2]= particion;

        }else if (contador==3){
            tama=tama+sizeof(particion)+30;
            strcpy(particion.status,"U");
            strcpy(particion.type,"P");
            strcpy(particion.fit,"WF");
            particion.size=sizeof(partition)+30;
            particion.start=tama;
            strcpy(particion.nombre,"Particion4");
            mbr->PARTICION[3]= particion;

        }
        //Escribiendo en Archivo
        FILE *fichero;
        fichero = fopen("/media/audrie8a/Archivos/C/HT1_201801263/disco.dk","r+b");
        fseek(fichero,tama,0);
        fwrite(&particion,sizeof(partition),1,fichero);
        if(contador==3){
            mbr->SIGNATURE=12;
            fseek(fichero,0,0);
            fwrite(mbr,sizeof(MBR),1,fichero);
            fclose(fichero);
        }
        contador++;
    }
}

void CrearArchivoBinario(){
    //Tamaño 2MB = 2*(1024*1024)
    int tama=2*1024*1024;    

    //Creando Archivo Binario de 2MB
    FILE *fichero;
    fichero = fopen("/media/audrie8a/Archivos/C/HT1_201801263/disco.dk","wb");

    fseek(fichero,0,0);
    fwrite("0",1,1,fichero);
    fseek(fichero,tama,0);
    fwrite("0",1,1,fichero);

    //Escribiendo el MBR
    char fit[]={"F"};
    struct MBR *getMbr= EscribirMBR(1,fit,tama);

    fseek(fichero,0,0);
    fwrite(getMbr,sizeof(MBR),1,fichero);
    //cout<<mbr->FECHA<<endl;
    //cout<<mbr->FIT<<endl;
    //cout<<mbr->SIGNATURE<<endl;
    //cout<<mbr->SIZE<<endl;

    fclose(fichero);    
}



void Menu(){    
}

int main(){
    CrearArchivoBinario();
    CrearParticionPrimaria();
    //ReporteMBR();
    Graficar("");
    return 0;
}

