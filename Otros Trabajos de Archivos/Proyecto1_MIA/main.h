#pragma once
#include <iostream>
#include "Analizador/Analizador.h"
using namespace std;

class Main{
    public:
        Analizador* analizador=new Analizador();
        
        void Consola();

};