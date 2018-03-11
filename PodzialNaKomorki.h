/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PodzialNaKomorki.h
 * Author: BCO
 *
 * Created on 13 kwietnia 2016, 10:31
 */

#ifndef PODZIALNAKOMORKI_H
#define PODZIALNAKOMORKI_H
#include <deque>
#include "Gwiazda.h"
#include "OdczytParametrow.h"


class PodzialNaKomorki {
public:
    PodzialNaKomorki();
    PodzialNaKomorki(const PodzialNaKomorki& orig);
    virtual ~PodzialNaKomorki();
    void PobierzParametry(int,int,int ,float *);//tablice parametrów docelwo odczytanych z pliku, a na początku ze zmiennych w funkcji RysujGwiazdy::Inicjuj
    void PobierzParametry(OdczytParametrow *);//OdczytParametrow udostępnia metodę  OdczytajParametr w argumencie przyjmuje nazwę parametrów do odczytania
    void IleGwiazdWkatalogu(int ile){wielkoscKataloguHip=ile;};
    void UstalKomorki();//tworzy i oblicza d- tablica długości komórek, pS - tablica pozycji początkowych komórek danego poziomu w tablicy początkiKomórek
     int KtorePunktyWidac(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip,listaItLG &);//
     int KtorePunktyWidacNowe(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip);//
    void PrzydzielDoStref(std::list<Gwiazda2>& );
    void ObliczWielkosciZakresow(std::list<Gwiazda2>& );//w zależności od przyjętych granic, będą różne wielkości sześcianów, w taki sposób, by sześcian obejmował 
                                                        //w miarę wszystkie gwiazdy danej wielkości oraz by każda pojedyńcza komórka miała w miarę tyle samo gwiazd
                                                        //zwraca wskaźnik do utworzonej tablicy zawierającej wstępny podział gwiazd - do którego zakresu/przedziału
     int UsunIterator(iteratorLG &);//po przejściu gwiazdy z oBaza do innego obiektu, jej iterator trzeba usunąć z lGwiazdyWkomorkach, zwraca zero, a jeśli się 
                                    //nie da, to zwraca jeden, bo do sumowania błędów
     int RozmiarListy(){return lGwiazdyWkomorkach.size();};
private:
    int wielkoscKataloguHip;
    int ileGranic;
    int ileWkomorce;//do nowego podziału - od tego zależy ile będzie komórek dla danego przedziału
    int ileKomorek;//jest to rozmiar dla tablicy poczatkiKomorek
    int* doKtoregoPrzedzialu;//tablica z elementem dla każdej obliczonej gwiazdy
    int * ileKomorekWzakresach;//ilość komórek w długości jednego boku, liczone od minusowch do plusowych,każdy zakres, przedział może mieć inną ilość komórek, inicjowane w ObliczWielkościZakresów
    int * pS;
    float* granice;//odczytane z pliku
    float * promienPrzedzialu;//obliczone dla każdego przedziału w metodzie ObliczWielkosciZakresow
    float wspolczynikWidokuPunktow;
    iteratorLitLG * poczatkiKomorek;//wskaźnik do tablicy tworzonej w przydziel do stref
    listaItLG lGwiazdyWkomorkach;//zawierać będzie iteratory do wyszystkich gwiazd ułożonych kolejno komórkami
	int ileZakresowWidac;//od najmniejszych,jeżeli zero to widać wszystkie
};

#endif /* PODZIALNAKOMORKI_H */

