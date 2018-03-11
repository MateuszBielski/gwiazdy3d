/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZbiorGwiazd.h
 * Author: BCO
 *
 * Created on 19 lutego 2016, 07:31
 */

#ifndef ZBIORGWIAZD_H
#define ZBIORGWIAZD_H
#include "Gwiazda.h"
#include <deque>
#include <vector>

class Naglowek2 {
public:

    Naglowek2() {
    };

    ~Naglowek2() {
    };
    int nDane[8];
};


class ZbiorGwiazd {
public:
    ZbiorGwiazd();
    virtual ~ZbiorGwiazd();
    void WyswietlParametryGwiazd(short, short *,short);//która strefa, jakie parametry, jak sortowane
    void WyswietlHipZpoczatkuStref(int,int);//z każdej strefy wyświetli zadaną ilość pierwszych gwiazd, jak sortowane
    void WyswietlHipListyGwiazd();
    int IleGwiazd(){return ileGwiazd;};
    int * IleGwiazdWstrefach(){return ileGwiazdWstrefach;};
    Gwiazda * pGwiazda(){return gwiazda;};
    std::list<Gwiazda2>& LGwiazdy(){return lGwiazdy;};
    int ZestawIteratorowListy(std::vector<std::list<Gwiazda2>::iterator>&,int);/*w argumencie przekazywany jest wskaźnik do tablicy, którą należy wypełnić od miejsca
                                                                    *wskazanego w drugim argumencie, funkcja zwraca ilość uzupełnionych elementów*/
    
   virtual int ListaWspolrzednych(int,std::deque<double> &){};
    
protected:
    std::list<Gwiazda2>::iterator DodajGwiazdy(Gwiazda *,int);/*używana w metodach klasy potomnej, 
                                                               *ma kopiować między obiektami z tablicy Gwiazda * gwiazda do list<Gwiazda2> lGwiazdy
                                                               * w argumencie wskaźnik do pierwszego obiektu z kopiowanej listy, ilość elementów do skopiowania*/
    short ileStref;
    
    
    int * ileGwiazdWstrefach;
    int * gdziePoczatekStrefy;
    Gwiazda * gwiazda;//po odczytaniu gwiazdy trafiają do nowo utworzonej tablicy gwiazda = new Gwiazda[ileGwiazd];
    std::list<Gwiazda2> lGwiazdy;
    std::deque<std::list<Gwiazda2>::iterator> iteratoryKolejnychPoziomow;//uzupełniany pierwszym iteratorem listy, gdy dodajemy gwiazdy nowego poziomu
    int ileGwiazd;
    int wielkoscKataloguHip;
};

#endif /* ZBIORGWIAZD_H */

