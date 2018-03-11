/* 
 * File:   OdczytWierzcholkow.h
 * Author: Mateusz Bielski
 *
 * Created on 19 lutego 2016, 07:56
 */

#ifndef ODCZYTWIERZCHOLKOW_H
#define ODCZYTWIERZCHOLKOW_H

#include "OdczytPliku.h"
#include <deque>

class OdczytWierzcholkow : public OdczytPliku {
public:
    OdczytWierzcholkow();
    OdczytWierzcholkow(const char *);
    OdczytWierzcholkow(const OdczytWierzcholkow& orig);
    virtual ~OdczytWierzcholkow();
    void OdczytajWierzcholki(std::ifstream &);
    double WierzcholkiZpliku(int i){return wierzcholkiZpliku[i];};
    double SrodkiScian(int i){return dSrodkiScian.at(i);}
    bool WyznaczSrodkiKolejnegoPoziomu();//muszą być wyznaczane kolejno poziomy - bez pierwszego nie da się wyznaczyć drugiego
    short Poziom(){return poziom;};
    int PoczatekAktualnychSrodkowScian(){return poczatekAktualnychSrodkowScian;};
    void WyswietlWartosciWierzcholkow();
    void WyswietlWartosciSrodkowScian();
    void WyswietlWartosciWierzcholkowPoczatkowych();
    void ListaWspolrzednych( std::deque<double> & );
    //do zrobienia funkcja, która dzieli na wektory
protected:
    void PodzielStrefe(int,std::vector<double>&);
    short poziom;//na którym aktualnie poziomie jestesmy;
    int poczatekAktualnychSrodkowScian;
    double k120st=2*3.14159/3;//120 stopni
     double wierzcholkiZpliku[36];//po jednej komórce na każdą współrzędną a jest 12 wierzchołków
     int numeryWierzcholkow[20];
     
     std::vector<double> vWierzcholkiPoczatkowe;//do rozpoczynania podziału, z każdym podziałem wielkość i zawartość ustalana od nowa
     std::deque<double> dSrodkiScian;// deque, bo dodając kolejne poziomy trzeba będzie zwiększać rozmiar

};

#endif /* ODCZYTWIERZCHOLKOW_H */

