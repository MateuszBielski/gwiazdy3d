/* 
 * File:   ZbiorGwiazd.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 19 lutego 2016, 07:31
 */

#include <vector>

#include "ZbiorGwiazd.h"

ZbiorGwiazd::ZbiorGwiazd() : ileStref(0), ileGwiazdWstrefach(0),
        gdziePoczatekStrefy(0), gwiazda(0),ileGwiazd(0),wielkoscKataloguHip(118323)//zmieniono końcówkę z 218, bo były gwiazdy o wyzszym hip
//,indeksHip(0),
{

}

//ZbiorGwiazd::ZbiorGwiazd(const ZbiorGwiazd& orig) {
//}

ZbiorGwiazd::~ZbiorGwiazd() {
    
    //if (!indeksHip) delete[] indeksHip;
    if (!ileGwiazdWstrefach) delete[] ileGwiazdWstrefach;
    if (!gdziePoczatekStrefy) delete[] gdziePoczatekStrefy;
    if (!gwiazda) delete[] gwiazda;

}
//double * ZbiorGwiazd::PozycjaGwiazdy(int, int){
//    
//}
std::list<Gwiazda2>::iterator ZbiorGwiazd::DodajGwiazdy(Gwiazda * pGwiazda,int ile){
    printf("\ndodaj %d gwiazd ",ile);
    //tworzymy obiekt Gwiazda2 i wstawiamy go do listy
    lGwiazdy.push_back(Gwiazda2(pGwiazda));
    //pierwszy element trzeba zachować do zwrócenia
    std::list<Gwiazda2>::iterator iDoZwrotu=--lGwiazdy.end();
    //zaczynamy od drugiego
    for(int i=1;i<ile;i++){
    //Gwiazda2 g2(pGwiazda[i]);//może to będzie potrzebne?
        lGwiazdy.push_back(Gwiazda2(pGwiazda+i));
    }
    return iDoZwrotu;
}
int ZbiorGwiazd::ZestawIteratorowListy(std::vector<std::list<Gwiazda2>::iterator>& tablicaIteratorow,int odKtorego){
    //printf("\npoczatek ZestawIteratorowListy");
    try{
        if(lGwiazdy.empty() ){
            //printf("\nlista pusta, iteratorow brak, odKtorego=%d", odKtorego);
            return odKtorego;
        }
        if(tablicaIteratorow.empty()){
            //printf("\nvektor o zerowej pojemnosci, odKtorego=%d", odKtorego);
            return odKtorego;
        }
        std::list<Gwiazda2>::iterator it=lGwiazdy.begin();
        while(it!=lGwiazdy.end())tablicaIteratorow.at(odKtorego++)=it++;
    }catch(std::out_of_range & oor){
        printf("\nZestawIteratorowListy przekroczenie rozmiaru vektora element nr %d, rozmiar %d, komunikat %s",odKtorego,tablicaIteratorow.size(),oor.what());
    }
    //printf("\nZestawIteratorowListy, numer ostatniego uzupelnionego elementu %d",odKtorego-1);
    return odKtorego;
}
void ZbiorGwiazd::WyswietlParametryGwiazd(short i, short * par, short jakSortowane) {
    //odczytać należy gwiazdy z odpowiedniego miejsca
    //i - numer strefy
    int pocz, kon;
    pocz = gdziePoczatekStrefy[i]; //    pocz = *(gdziePoczatekStrefy + i);
    kon = pocz + ileGwiazdWstrefach[i]; //    kon = pocz + *(ileGwiazdWstrefach + i);
    //printf("gwiazda początkowa %d i końcowa %d",pocz,kon);

    //można utworzyć listę gwiazd
    std::list<Gwiazda> lGwiazdyStrefy((gwiazda + pocz), (gwiazda + kon)); //nie działa wersja konstruktora:lGwiazdyStrefy((gwiazda[pocz],(gwiazda[kon])
    //oraz zrobić opcję sortowania

    if (jakSortowane > 0) {
        bool (*wSort[3])(Gwiazda &, Gwiazda &);
        //wskaźniki do funkcji sortujących
        wSort[0] = wPlx;
        wSort[1] = wHip;
        wSort[2] = wOdlDoCentrum;
        lGwiazdyStrefy.sort(wSort[jakSortowane - 1]); //bo zero definiuje, że nie korzystamy z sortowania 
    }
    int j = 0;
    for (std::list<Gwiazda>::iterator it = lGwiazdyStrefy.begin(); it != lGwiazdyStrefy.end(); it++) {
        printf("\n %d ", j++); //numer gwiazdy
        it->WyswietlParametry(par);
    }

    //    for (int j = pocz; j < kon; j++) {
    //        printf("\n %d ", j - pocz); //numer gwiazdy
    //        gwiazda[j].WyswietlParametry(par);//(gwiazda + j)->WyswietlParametry(par);//wersja wcześniejsza
    //    }
}
void ZbiorGwiazd::WyswietlHipZpoczatkuStref(int doKtorej,int jakSortowane){
    //określenie wskaźników do sortowania
    bool (*wSort[3])(Gwiazda &, Gwiazda &);
    wSort[0] = wPlx;
    wSort[1] = wHip;
    wSort[2] = wOdlDoCentrum;
    
    printf("\nile stref %d",ileStref);
    
    for(int i=0;i<ileStref;i++){
        int pocz, kon;
        pocz = gdziePoczatekStrefy[i]; //    pocz = *(gdziePoczatekStrefy + i);
        kon = pocz + ileGwiazdWstrefach[i]; //    kon = pocz + *(ileGwiazdWstrefach + i);
        //printf("gwiazda początkowa %d i końcowa %d",pocz,kon);
        printf("\n %d strefa",i);
        //utworzenie tymczasowej listy gwiazd dla każdej strefy
        std::list<Gwiazda> lGwiazdyStrefy((gwiazda + pocz), (gwiazda + kon));
        if (jakSortowane > 0) {
            //wskaźniki do funkcji sortujących
            lGwiazdyStrefy.sort(wSort[jakSortowane - 1]); //bo zero definiuje, że nie korzystamy z sortowania 
        }    
        std::list<Gwiazda>::iterator it = lGwiazdyStrefy.begin();
        for ( int j = 0;j<doKtorej;j++) {
            printf("\n %d hip %d",j,it->Hip()); //numer gwiazdy
            it++;
        }
        
        
    }
}
    

    
    

void ZbiorGwiazd::WyswietlHipListyGwiazd(){
    printf("\nWyswietlHipListyGwiazd liczba elementow %d",lGwiazdy.size());
    std::list<Gwiazda2>::iterator it=lGwiazdy.begin();
    int i=0;
    while(it!=lGwiazdy.end())printf("\n%d %d",i++,(it++)->Hip());
}





//int Gwiazda::Mag(){
//    //zwraca int ze znakiem, co trzeba zmienić
//    int iMag;
////    (mag<0)?iMag=
//}

//    printf("odczytano i zamknięto plik o długości %d ", length);
//    for (short i = 0; i < 8; i++) {
//
//        printf("\n %d %d", i, pNag->nDane[i]);
//    } 
//    printf("dane stref: ");
//    for (short i = 0; i < ileStref; i++) {
//
//        printf("\n %d %d", i, ileGwiazdWstrefach[i]);
//    } 
//}

