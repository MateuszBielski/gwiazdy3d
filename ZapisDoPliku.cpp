/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZapisDoPliku.cpp
 * Author: BCO
 * 
 * Created on 5 sierpnia 2016, 08:59
 */

#include "ZapisDoPliku.h"
//#include "Gwiazda.h"

ZapisDoPliku::ZapisDoPliku() {
    
}

ZapisDoPliku::ZapisDoPliku(const char* filename, ios_base::openmode mode) : ofs(filename, mode), adresPliku(filename) {
//zrobić sprawdzenie, czy udało się otworzyć plik
//    if(!ofs.is_open)throw xBrakPliku(filename);
    if(!ofs.is_open())printf("\nnie otwarto pliku");
}

ZapisDoPliku::ZapisDoPliku(const ZapisDoPliku& orig) {
}

ZapisDoPliku::~ZapisDoPliku() {
    if (ofs.is_open()) {
        ofs.close();
    }
}

int ZapisDoPliku::DlugoscPliku() {
    ofs.seekg(0, ofs.end);
    return (int)ofs.tellg();
}

