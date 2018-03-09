/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RysujGwiazdy.h
 * Author: BCO
 *
 * Created on 10 kwietnia 2016, 13:15
 */

#ifndef RYSUJGWIAZDY_H
#define RYSUJGWIAZDY_H
#include <iostream>
#include "C:\Users\BCO\Documents\NetBeansProjects\CppApplication_2\OdczytajGwiazdy.h"
#include "C:\Users\BCO\Documents\NetBeansProjects\CppApplication_2\ObliczeniaGwiazd.h"
#include "C:\Users\BCO\Documents\NetBeansProjects\CppApplication_2\OdczytWierzcholkow.h"
#include "PrzygotowaniePunktow.h"
#include "C:\Users\BCO\Documents\NetBeansProjects\ArcBallGwiazdy\ArcBall.h"
#include "OdczytKolorowBMP.h"

class RysujGwiazdy {
   
public:
    RysujGwiazdy();
    void Inicjuj();
    void KopiujUstawienia(RysujGwiazdy *);//czyli np: utworzoną wczesniej czcionke, zbiory wybrane przez użytkownika
    ObliczeniaGwiazd * OBaza();//możliwe, że do usunięcia po zmianach
    std::deque<Gwiazdozbior>& ListaGwiazdozbiorow(){return oBaza->dGwiazdozbiorDoPrzycisku;};
    RysujGwiazdy(const RysujGwiazdy& orig);
    static RysujGwiazdy * WskaznikRysujGwiazdy(){return wskaznikRysujGwiazdy;};
    virtual ~RysujGwiazdy();
    void CzcionkaBitmapowa(int cz){punkty->czcionkaBitmapowa=cz;};//w wm create (COkonoGL)tworzy listy wyświetlania dla czcionek przekazuje i ustawia uchwyt
    void WyswietlGwiazdozbiory(int);//wskaźnik do tablicy numerów,rozmiar tablicy,maska bitowa, czy np ukryć poprzednie,czy z zaznaczonej gwiazdy
    void WyswietlGwiazdozbiorZprzyciskow(int);
    bool PokazUkryjGwiazdozbior(int,int);/*funkcja działa po naciśnięciu konrolki z numerem żądanego gwiazdozbioru; 
                                          * jeżeli jakikolwiek zbiór jest tworzony (dorysowany) funkcja zwraca true
                                          *w przeciwnym razie false*/
    void PokazUkryj(int);//maska bitowa, pokaż lub ukryj: linie, gwiazdy na strefie, w przestrzeni, wszystkie w przestrzeni
    void UstawKameraXYZf(float&,float&,float&);//ustawia pobrane argumenty, np przy centrowaniu widoku
    void PobierzPrzeksztalcenia(float,float,float,Matrix4fT&);//kameraXYZ, Transform (ArcBall), ma ustalić zaaktualizować wektor kamery wK (punkty)
    void PobierzUstawieniaMacierzy();
    void PrzygotujPunkty(int coZmieniono){punkty->PrzygotujPunktyDoWyswietlenia(coZmieniono);};
    bool PobierzNrIloscZklawiatury(int *,char *);/*pobiera i sprawdza poprawność przy wybieraniu numeru gwiazdozbioru z klawiatury
                                           * w pobieranej tabeli pierwszy element to ilość przekazywanych argumentów do wypełnienia
                                           * można będzie wprowadzić, drugi argument mówiący jaki rodzaj sprawdzania przeprowadzić*/
    void WydzielWybraneGwiazdozbiory(int,int);
    void UstawWspolczynnikKamery(float kameraR){punkty->wspKam = pow(2 * kameraR, 0.25);};
    void PokazKolejnyGwiazdozbior(int);
    void OdszukajNr(float ,float ,float ,int );//jeśli istnieje gwiazda o takim numerze, to przypisuje pobrany argument do punkty->idWybranegoPunktu
    void ZaznaczGwiazde(float ,float ,float ,int);
    void PokazNajblizsze();
    iteratorLG CoWybrano();
    
    void rgRysujScene(){return punkty->PrzygPktRysujScene();};
    
private:
    static RysujGwiazdy * wskaznikRysujGwiazdy;
    ObliczeniaGwiazd * oBaza;
    PrzygotowaniePunktow * punkty;
    PodzialNaKomorki * podzial;
    OdczytKolorowBMP * ok;
    
    
    //konieczny jest obiekt, lub w innej formie przechowywane dane odnośnie wszytskich parametrów uzywanych w programie, a przede wszystkim podział na 
    //komórki, współczynniki a także adresy plików - rzeczy te powinny być odczytywane z pliku 
};

#endif /* RYSUJGWIAZDY_H */

