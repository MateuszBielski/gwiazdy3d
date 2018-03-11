/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OdczytPliku.cpp
 * Author: BCO
 * 
 * Created on 5 lutego 2016, 11:45
 */



//#include <GL\gl.h>
//#include <GL\glu.h>
#include "OdczytPliku.h"

OdczytPliku::OdczytPliku() {
}

OdczytPliku::OdczytPliku(const char * aP) : ifs(aP, std::ios::binary) {
    printf("\notwieranie pliku:  %s", aP);
    if (!ifs.is_open()) {
        this->~OdczytPliku();
        throw xBrakPliku(aP);

    }
}

OdczytPliku::OdczytPliku(const char * aP, char bParametry) {

    printf("\notwieranie pliku:  %s", aP);
    if (bParametry & TEKSTOWO){
        ifs.open(aP);
        printf("\notwarto plik %s do odczytu tekstowego",aP);
    }
        
    //bezpieczenstwo
    if (!ifs.is_open()) {
        this->~OdczytPliku();
        throw xBrakPliku(aP);
    }

}

OdczytPliku::OdczytPliku(const OdczytPliku& orig) {
    //printf("\ndestruktor odczyt pliku");
}

OdczytPliku& OdczytPliku::operator=(OdczytPliku& rhs) {
    this->adresPliku = rhs.adresPliku;
    this->ifs.open(rhs.adresPliku);
}

OdczytPliku::~OdczytPliku() {

    if (ifs.is_open()) {
        ifs.close();
    }
}
int OdczytPliku::OdczytajKolejneLinie(std::ifstream & plik) {
    int nrLinii = 0;
     while (!plik.eof()) {
        std::string sWiersz;
        std::getline(plik, sWiersz);
        if (plik.bad()) //podczas próby odczytania danych wystąpił błąd sprzętowy
        {
            plik.close();
            printf("\nplik.bad");
            throw xBladOdczytu(nrLinii);
        }
        linieOdczytane.push_back(sWiersz);
        nrLinii++;
     }
    printf("\nodczytano %d linii", linieOdczytane.size());
    return nrLinii;
}
