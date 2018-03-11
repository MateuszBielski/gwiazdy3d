/* 
 * File:   PrzygotowaniePunktow.h
 * Author: Mateusz Bielski
 *
 * Created on 11 kwietnia 2016, 10:05
 */

#ifndef PRZYGOTOWANIEPUNKTOW_H
#define PRZYGOTOWANIEPUNKTOW_H

#define NA_SFERZE 1
#define W_PRZESTRZENI 2
#define LINIE 4
#define WSZYSTKIE_W_PRZESTRZENI 8
#define WPISANE_OD_NOWA 16
#define ZAZNACZONE_OD_NOWA 32
#define ZAZNACZONE_DORYSUJ 64
#define WPISANE_DORYSUJ 128
#define ETYKIETY 256

#include <GL\gl.h>
#include <GL\glu.h>
#include "Varia.h"
#include <list>
#include <deque>
#include <vector>
#include <cstdio>
#include "ObliczeniaGwiazd.h"

class PrzygotowaniePunktow {
    friend class RysujGwiazdy;
public:
    PrzygotowaniePunktow();
    PrzygotowaniePunktow(const PrzygotowaniePunktow& orig);
    virtual ~PrzygotowaniePunktow();
    void PrzygotujPunktyDoWyswietlenia(int );//maska bitowa co do zmiany
    void PrzygotujNazwyDoWyswietlania(int, std::list<int>& );
    void PrzygPktRysujScene();//pobierany wskaźnik służy do ustalania nazw
    void UstawTabliceKolorow(unsigned char *,int);
    int UsunZlistyPokazywanychGwiazdozbiorow(int);//szuka na liście, jeśli jest - zwraca jego pozycję i kasuje
private:
    void PokazEtykietyObiektow(listaItLG&,bool*,int);//lista zawierająca etykiety,ekran podzielony na komórki,co wyświetlać 
    std::deque<double> punktyWszystkichGwiazd;
    std::deque<double> punktyLinii;
    std::deque<double> punktyGwiazdNaSferze;
//    std::deque<double> punktyGwiazdNajblizszych;
    std::deque<double> punktyGwiazdWprzestrzeni;
    
    listaItLG iteratoryWybranychDoWyswietlenia;//do wyświetlenia nazw największych spośród wybranego gwiazdozbioru
    listaItLG iteratoryWszystkichDoWyswietlenia;//do wyświetlenia nazw największych spośród wszystkich pozostałych wyświetlanych aktualnie
    std::list<int>     hipWyswietlanychGwiazd;
    std::list<int>::iterator tIdoListyHip[2];
    std::vector<ObliczeniaGwiazd *> voGwiazdozbior;//wskaźniki do aktualnie wyświetlanych gwiazdozbiorów
    std::list<int> numeryUzywanychGwiazdozbiorow;
    ObliczeniaGwiazd * oBaza;
    float wK[3];//wektor skierowany w dal od środka - potrzebny przy ustalaniu które z wszystkich gwiazd widać
    float wspKam;
    //ustawienia macierzy
    int viewport[4];
    double macierzModelWidok[16];
    double macierzRzutowania[16];
    //ustawienia punktów
    int komorkiWpionie;//do ustalania gęstości występowania napisów na ekranie
    int komorkiWpoziomie;
    int idWybranegoPunktu;//do zamiany na tablicę, lub listę
    int idWybranegoGwiazdozbioru;//jw
    float cX,cY,cZ;//c- współrzędne do centrowania
    float r[128],g[128],b[128];
    int coPokazac;
    //napisy i teksty
    unsigned int czcionkaBitmapowa;
    //std::deque<const char *> dcTekstParametry;//do wyswietlenia w metodzie PrzygPktRysujScene()
    std::deque<std::string> dsParametryDoWyswietlenia;//parametry gwiazdy zaznaczonejdo wyswietlenia w metodzie PrzygPktRysujScene()
    int iKolorTekstu;
    
    
    /*******parametry do podziału na komórki************/
    
   

};

#endif /* PRZYGOTOWANIEPUNKTOW_H */

