/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sprawdzanie.cpp
 * Author: BCO
 * 
 * Created on 11 marca 2016, 07:59
 */

#include "Sprawdzanie.h"

Sprawdzanie::Sprawdzanie() {
}
Sprawdzanie::Sprawdzanie(ObliczeniaGwiazd * og,int i):ObliczeniaGwiazd(og,i,0){
    
}
Sprawdzanie::Sprawdzanie(ObliczeniaGwiazd * og):ObliczeniaGwiazd(*og){
    printf("\nSprawdzanie konstruktor po skopiowaniu parametrow ");
  //kopiowanie wszystkich posiadanych parametrów  
}

Sprawdzanie::Sprawdzanie(const Sprawdzanie& orig) {
}

Sprawdzanie::~Sprawdzanie() {
}
void Sprawdzanie::WyswietlParametryG2(short strefa) {
    //    int pocz, kon;
    //    pocz = gdziePoczatekStrefy[strefa]; //    pocz = *(gdziePoczatekStrefy + i);
    //    kon = pocz + ileGwiazdWstrefach[strefa]; //    kon = pocz + *(ileGwiazdWstrefach + i);
    /*bez podziału na strefy ale za to z sortowaniem*/
    std::list<Gwiazda2> lGwiazdyPosortowane(lGwiazdy); //żeby na liście pozostał pierwotny porządek
    lGwiazdyPosortowane.sort(wmag);
    int licznik = 0;
    std::list<Gwiazda2>::iterator it = lGwiazdyPosortowane.begin();
    while (licznik++ < 100 || it!=lGwiazdyPosortowane.end()) {
        printf("\n  %d. hip %d %2.2fm %2.2fM %4.2f lat sw. wielkosc do wysw %4.2f", licznik++, it->Hip(), it->Jm(), it->JM(), it->Odleglosc(),it->WielkoscDoWyswietlenia()); //numer gwiazdy
        it++;
    }
}
void Sprawdzanie::MagnitudoGwiazd(OdczytajGwiazdy & og, short i) {
    //obliczyć długość kroku do magnitudo
    float krok = (float) og.Nag()->nDane[6] / og.Nag()->nDane[7];
    //odnalezienie odpowiednich gwiazd
    int pocz, kon;
    pocz = gdziePoczatekStrefy[i];
    kon = pocz + ileGwiazdWstrefach[i];
    //tworzenie listy:
    std::list<Gwiazda> lGwiazdyStrefy((og.pGwiazda() + pocz), (og.pGwiazda() + kon));
    int j = 0;
    for (std::list<Gwiazda>::iterator it = lGwiazdyStrefy.begin(); it != lGwiazdyStrefy.end(); it++) {
        float m = (og.Nag()->nDane[5] + krok * it->Mag()) / 1000;
        float M = m + 5 * (1 + log10((float) it->Plx() / 100000));

        printf("\n  %d. hip %d %2.2fm %2.2fM", j++, it->Hip(), m, M); //numer gwiazdy
    }
}
void Sprawdzanie::WypiszOdczytaneSymbole(){
    for(int i=0;i<kolejneSymboleZnazw.size();i++){
        printf("\n%s %d",kolejneSymboleZnazw[i].c_str(),i);
    }
}
void Sprawdzanie::WypiszParametryJednegoGwiazdozbioru(int ktory){
    //potrzebujemy hip magnitudo , Magnitudo, wielkosc do wyswietlenia: obserwowana i absolutna
    //porównać hip ze zbioru nazw oraz z parametrów wybranych wg iteratora
    //hip z listy nazw
    printf("\nhip z listy nazw, symbol z listy,z iteratora: hip ,mag256, mag, Mag, wielkosc, Abs wielkosc, bajt w pliku");
    int pocz = pozycjaKolejnegoGwiazdozbioru[ktory];
    int kon = pocz + ileGwiazdWgwiazdozbiorze[ktory];
    int licznik=0;
    int hip=0;
    Gwiazda2 gTemp;
    for (int i = pocz; i < kon; i++) {
        hip=kolejneHipZnazw[i];
        gTemp=*indxItG[hip];
        
        printf("\n %d %d, %s, %d, %d, %2.3f, %2.3f, %2.3f, %2.3f, %d",licznik++,kolejneHipZnazw[i],kolejneSymboleZnazw[i].c_str(),
                gTemp.Hip(),gTemp.Mag(),gTemp.Jm(),gTemp.JM(),gTemp.WielkoscDoWyswietlenia(),gTemp.AbsolutnaWlkDoWyswietlenia(),112+28*indeksHip[hip]+13);
    }
}

