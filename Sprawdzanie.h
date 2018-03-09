/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sprawdzanie.h
 * Author: BCO
 *
 * Created on 11 marca 2016, 07:59
 */

#ifndef SPRAWDZANIE_H
#define SPRAWDZANIE_H
#include "ObliczeniaGwiazd.h"

class Sprawdzanie: public ObliczeniaGwiazd {
public:
    Sprawdzanie();
    Sprawdzanie(ObliczeniaGwiazd *);//ma przyjąć wszystkie parametry pobrane i wyliczone przez pobierany obiekt
    Sprawdzanie(ObliczeniaGwiazd *,int);
    Sprawdzanie(const Sprawdzanie& orig);
    virtual ~Sprawdzanie();
    void WypiszOdczytaneSymbole();
    void WypiszParametryJednegoGwiazdozbioru(int );
    
    void WyswietlParametryG2(short);//numer strefy - nie używany
    
    void MagnitudoGwiazd(OdczytajGwiazdy &,short);//która strefa
private:

};

#endif /* SPRAWDZANIE_H */

