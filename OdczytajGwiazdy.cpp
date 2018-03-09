/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OdczytajGwiazdy.cpp
 * Author: BCO
 * 
 * Created on 19 lutego 2016, 07:44
 */

#include "OdczytajGwiazdy.h"

OdczytajGwiazdy::OdczytajGwiazdy()  {
}
OdczytajGwiazdy::OdczytajGwiazdy(const char * aP):OdczytPliku(aP),pNag(0)  {
PrzeczytajNaglowek(ifs);
    ileGwiazd = PrzeczytajStrefy(ifs);
    PrzeczytajGwiazdy(ifs);
    ifs.close();
    printf("\nzamknieto plik");
}

OdczytajGwiazdy::OdczytajGwiazdy(const OdczytajGwiazdy& orig) {
    
}

OdczytajGwiazdy::~OdczytajGwiazdy() {
    if(pNag) delete pNag;//sprawdzamy, czy było coś przypisane
     
}
void OdczytajGwiazdy::PrzeczytajNaglowek(std::ifstream & ifs) {
    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    pNag = new Naglowek2();
    ifs.read((char*) pNag, sizeof (Naglowek2));
}

int OdczytajGwiazdy::PrzeczytajStrefy(std::ifstream & ifs) {
    ileStref = 20 * pow(4, pNag->nDane[4]);
    printf("\n plik zawiera %d stref",ileStref);
    ileGwiazdWstrefach = new int[ileStref];
    gdziePoczatekStrefy = new int[ileStref];
    int licznik = 0;
    for (short i = 0; i < ileStref; i++) {
        ifs.read((char*) (ileGwiazdWstrefach + i), 4);
        *(gdziePoczatekStrefy + i) = licznik;
        licznik = *(ileGwiazdWstrefach + i) + licznik;
    }
    return licznik;
}

void OdczytajGwiazdy::PrzeczytajGwiazdy(std::ifstream & ifs) {
    gwiazda = new Gwiazda[ileGwiazd];
    printf("\nczytanie %d gwiazd", ileGwiazd);
    //printf(" ; rozmiary Gwiazda %d Gwiazda2 %d", sizeof (Gwiazda), sizeof (Gwiazda2));

    for (int i = 0; i < ileGwiazd; i++) {
        ifs.read((char*) (gwiazda + i), sizeof (Gwiazda));
    }
    //trzeba dodać tablicę wskaźników, żeby odwoływać się przez indeksy
}
void OdczytajGwiazdy::WypiszNaglowekIilosciGwiazd() {
    printf("\nNagłówek i ilości gwiazd: ");
    for (short i = 0; i < 8; i++) {
        printf("\n %d %d", i, pNag->nDane[i]);
    }
    printf("\ndane stref: ");
    for (short i = 0; i < ileStref; i++) {
        printf("\n %d %d, %d", i, *(ileGwiazdWstrefach + i), *(gdziePoczatekStrefy + i));
    }
    printf("\nłącznie gwiazd %d ", ileGwiazd);
}
int OdczytajGwiazdy::ListaWspolrzednych(int ktoraStrefa, std::deque<double> & lista){
    printf("\nLista Współrzędnych z klasy potomnej");
    int pocz, kon;
    pocz = gdziePoczatekStrefy[ktoraStrefa]; //    pocz = *(gdziePoczatekStrefy + i);
    kon = pocz + ileGwiazdWstrefach[ktoraStrefa];
    printf("\nindeksy: poczatek: %d, koniec %d",pocz,kon);
     for (int i = pocz; i < kon; i++) {

        lista.push_back((double) (gwiazda+ i)->X0() / 1000000000);
        lista.push_back((double) (gwiazda + i)->X1() / 1000000000);
        lista.push_back(0.4);

    }
    //
return lista.size();    
}
void OdczytajGwiazdy::GwiazdyWyzerujDoXnaprzemianY(){
    int i=0;
    while(i<ileGwiazd){
        gwiazda[i++].WyzerujX0();
        if(i<ileGwiazd)gwiazda[i++].WyzerujX1();
    }
}
