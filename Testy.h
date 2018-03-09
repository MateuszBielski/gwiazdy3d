/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Testy.h
 * Author: BCO
 *
 * Created on 18 lutego 2016, 13:54
 */

#ifndef TESTY_H
#define TESTY_H
#include "OdczytajGwiazdy.h"
#include "OdczytWierzcholkow.h"

class Testy { //: public OdczytajGwiazdy , public OdczytWierzcholkow
public:
    Testy(){};
    ~Testy(){};
    void ZapiszWierzcholkiDoPliku();
    void PorownajWierzcholki(OdczytWierzcholkow &);
    void PrzyjmijWierzcholki();
    void ZapiszOdczytaneGwiazdyDoPliku(OdczytajGwiazdy &);
    //void ObliczGwiazdy2();// generuje tablicę  new Gwiazda2[]
private:
    double wierzcholki[60];

};

#endif /* TESTY_H */

