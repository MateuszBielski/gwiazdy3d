/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ObliczeniaGwiazd.h
 * Author: BCO
 *
 * Created on 19 lutego 2016, 07:49
 */

#ifndef OBLICZENIAGWIAZD_H
#define OBLICZENIAGWIAZD_H

#include "ZbiorGwiazd.h"
#include "OdczytajGwiazdy.h"
#include "OdczytWierzcholkow.h"
#include "OdczytNazw.h"
#include "PodzialNaKomorki.h"
#include "OdczytParametrow.h"
#include <vector>
#include <time.h>
#include <process.h>
#include <map>


class ObliczeniaGwiazd : public ZbiorGwiazd{
public:
    ObliczeniaGwiazd();
    ObliczeniaGwiazd(OdczytWierzcholkow *);
    ObliczeniaGwiazd(ObliczeniaGwiazd *,int,bool);/*założenie: wypełnia tylko lGwiazdy według danych z pobieranego obiektu,
                                                   * resztę składowych należałoby wyzerować*/
    //ObliczeniaGwiazd(const ObliczeniaGwiazd& orig);
    virtual ~ObliczeniaGwiazd();
    void UstawPodzial(PodzialNaKomorki * pNk){pNk->IleGwiazdWkatalogu(wielkoscKataloguHip);podzial=pNk;};
    void ObliczGwiazdy(OdczytajGwiazdy *);//na początku sprawdza, czy trzeba dodać do listy lGwiazdy
    void UstalPolozenie(OdczytajGwiazdy *);//j/w
    void PobierzParametry(OdczytParametrow * );
    int DodajKolejnePoziomyJakoGwiazdozbiory();//po wybraniu nr 193 ukazują się wszystkie gwiazdy pierwszego pliku (na sferze, linie i w przestrzeni), 194 i 195 kolejnych
    void UstawIndeksy();
    void PobierzTlumaczenie(const char *,int);
    void PrzydzielDoStrefPrzestrzennych(std::list<Gwiazda2>&);//funkcja występuje w początkowej fazie w funkcji ustaw indeksy, powinna być też używana każdorazowo, po oddaniu gwiazd z gwiazdozbioru do bazy
   float ObliczDystans(float,float,float,int);//odległość od punktu do gwiazdy o numerze hip
   bool NajblizszeGwiazdy(int);//według numeru hip
    int PobierzGwiazdozbiory(OdczytNazw * ,OdczytNazw * );//uzupełnia składowe: KkolejneHip, kolejneSymbole, itd.//
    void PobierzPelneNazwyGwiazdozbiorow(OdczytNazw *);
    int PobierzNazwyGwiazd(OdczytNazw *);
    int KtorePunktyWidac(float *,std::deque<double>&, std::list<int>& ,listaItLG &);
    int ListaWspolrzednych(int,std::deque<double> &);
    int ListaWspolrzednychZOdleglosciami(std::deque<double> & lista,listaItLG &);
    int ListaWspolrzednychParPunktow(std::deque<double> & );
    std::list<int>::iterator ListaHip(std::list<int>&,std::list<int>::iterator );
    void PrzygotujGwiazdyOdejmujac(std::list<Gwiazda2>&,int);//j/w lecz z użyciem iteratorów list i funkcją splice. Odejmuje z głównej listy
    void PrzekazGwiazdyDoBazy(ObliczeniaGwiazd *);
    int ileOdczytanychNazwGwiazdozbiorow(){return ileGwiazdWgwiazdozbiorze.size();};
    std::string DoJakiegoGwiazdozbioruNalezy(int);//ma rozpoznać po numerze hip
    int DoKtoregoGwiazdozbioruNalezy(int);//j/w
    std::list<Gwiazda2>::iterator IteratorWgHip(int);
    std::deque<Gwiazdozbior> dGwiazdozbiorDoPrzycisku;
      
protected:
    void SprawdzStanListy(std::list<Gwiazda2>::iterator&,std::list<Gwiazda2>::iterator&,OdczytajGwiazdy *);//czy trzeba dodać gwiazdy, oraz ustawia iteratory początku i końca związane z obiektem OdczytajGwiazdy
    int UsunBezParalaxy();//zwraca ile usunięto
    OdczytWierzcholkow * ow;
    int ileNajblizszychGwiazd;//do wyswietlenia wokół wybranej gwiazdy
    int rozmiarLGwiazdy;//ustalany w ustaw indeksy i nie zmieniany
    int ileNazwanychGwiazd;
    std::vector<int> kolejneHipZnazw;
    iteratorLG * indxItG;//indeks iteratorów listy lGwiazdy
    iteratorLG * indeksPosortowanychIter;//dla posortowanych wzdłuż trzech osi
    
    std::vector<std::string> kolejneSymboleZnazw ;
    std::vector<int> vIndeksGwiazdozbiorow ;
    std::vector<std::string> vNazwy;
    std::deque<int> ileGwiazdWgwiazdozbiorze;
    std::deque<int> pozycjaKolejnegoGwiazdozbioru;
    std::map<std::string,std::string> nazwyTlumaczenie;//wypełniane z pliku w Rysuj Gwiazdy->Inicjuj: oBaza->PobierzTlumaczenie, zawiera nazwe łacińską i tłumaczoną
    int * indeksHip;
    int * indeksGwiazdozbiorow;//hip jest numerem rekordu, zawartosc numerem gwiazdozbioru - inicjujemy w konstuktorze, wypełniamy przy odczytywaniu gwiazdozbiorow
    
    PodzialNaKomorki * podzial;
    
    /***przenieść do PodzialNaKomorki*****/
};
//funkcja do osobnego wątku
typedef bool (ObliczeniaGwiazd::*WFbi)(int);//wskaźnik funkcyjny bool int
class ParametryWatkuObliczenGwiazd{
public:
    ObliczeniaGwiazd * oBaza;
    int hip;
    //wskaźnik na funkcję
    WFbi wskaznik;
};
void __cdecl WatekObliczenGwiazd(void * );

#endif /* OBLICZENIAGWIAZD_H */

