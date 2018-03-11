/* 
 * File:   OdczytajGwiazdy.h
 * Author: Mateusz Bielski
 *
 * Created on 19 lutego 2016, 07:44
 */

#ifndef ODCZYTAJGWIAZDY_H
#define ODCZYTAJGWIAZDY_H

#include "OdczytPliku.h"
#include "ZbiorGwiazd.h"

class OdczytajGwiazdy :public OdczytPliku, public ZbiorGwiazd {
public:
    OdczytajGwiazdy();
    OdczytajGwiazdy(const char * );//adres pliku
    OdczytajGwiazdy(const OdczytajGwiazdy& orig);
    virtual ~OdczytajGwiazdy();
    void PrzeczytajNaglowek(std::ifstream &);
    int PrzeczytajStrefy(std::ifstream &); // ma zwracać ilość wszystkich gwiazd
    void PrzeczytajGwiazdy(std::ifstream &);
    void WypiszNaglowekIilosciGwiazd();
    Naglowek2 * Nag(){return  pNag ;};
    int * IleGwiazdWstrefach(){return ileGwiazdWstrefach;};
    short  IleStref(){return ileStref;};
    int ListaWspolrzednych(int, std::deque<double> &);
    void GwiazdyWyzerujDoXnaprzemianY();
private:
    Naglowek2 * pNag;

};

#endif /* ODCZYTAJGWIAZDY_H */

