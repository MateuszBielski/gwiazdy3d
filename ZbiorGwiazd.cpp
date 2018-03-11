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
{

}
ZbiorGwiazd::~ZbiorGwiazd() {
    if (!ileGwiazdWstrefach) delete[] ileGwiazdWstrefach;
    if (!gdziePoczatekStrefy) delete[] gdziePoczatekStrefy;
    if (!gwiazda) delete[] gwiazda;

}
std::list<Gwiazda2>::iterator ZbiorGwiazd::DodajGwiazdy(Gwiazda * pGwiazda,int ile){
    printf("\ndodaj %d gwiazd ",ile);
    //tworzymy obiekt Gwiazda2 i wstawiamy go do listy
    lGwiazdy.push_back(Gwiazda2(pGwiazda));
    //pierwszy element trzeba zachować do zwrócenia
    std::list<Gwiazda2>::iterator iDoZwrotu=--lGwiazdy.end();
    //zaczynamy od drugiego
    for(int i=1;i<ile;i++){
        lGwiazdy.push_back(Gwiazda2(pGwiazda+i));
    }
    return iDoZwrotu;
}
int ZbiorGwiazd::ZestawIteratorowListy(std::vector<std::list<Gwiazda2>::iterator>& tablicaIteratorow,int odKtorego){
    try{
        if(lGwiazdy.empty() ){
            return odKtorego;
        }
        if(tablicaIteratorow.empty()){
            return odKtorego;
        }
        std::list<Gwiazda2>::iterator it=lGwiazdy.begin();
        while(it!=lGwiazdy.end())tablicaIteratorow.at(odKtorego++)=it++;
    }catch(std::out_of_range & oor){
        printf("\nZestawIteratorowListy przekroczenie rozmiaru vektora element nr %d, rozmiar %d, komunikat %s",odKtorego,tablicaIteratorow.size(),oor.what());
    }
    return odKtorego;
}
void ZbiorGwiazd::WyswietlParametryGwiazd(short i, short * par, short jakSortowane) {
    //odczytać należy gwiazdy z odpowiedniego miejsca
    //i - numer strefy
    int pocz, kon;
    pocz = gdziePoczatekStrefy[i];
    kon = pocz + ileGwiazdWstrefach[i];
    //można utworzyć listę gwiazd
    std::list<Gwiazda> lGwiazdyStrefy((gwiazda + pocz), (gwiazda + kon));
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
