/* 
 * File:   OdczytParametrow.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 20 kwietnia 2016, 08:24
 */

#include "OdczytParametrow.h"

OdczytParametrow::OdczytParametrow() {
}
OdczytParametrow::OdczytParametrow(const char * aP) : OdczytPliku(aP, TEKSTOWO) {
    int ile=OdczytajKolejneLinie(ifs);
    printf("\nOdczytano %d linii parametrow",ile);
}
OdczytParametrow::OdczytParametrow(const OdczytParametrow& orig) {
}

OdczytParametrow::~OdczytParametrow() {
}
int OdczytParametrow::OdczytajParametr(std::string& nazwa,float *& tParametry){
    //wyszukanie pierwszego i ostatniego indeksu, przedziału w którym są zapisane żądane parametry
    int i,pocz,kon;
    i=0;pocz=0;kon=0;
    std::string nazwaEnd=nazwa+"End";
    while(i<linieOdczytane.size() && kon==0 ){//jeżeli koniec będzie ustawiony, to dalej nie sprawdza
        if(linieOdczytane.at(i)==nazwa)pocz=i+1;//pierwszy element listy
        if(linieOdczytane.at(i)==nazwaEnd)kon=i;//następny za ostatnim elementem listy
        i++;
    }
    int ileParametrow=kon-pocz;
    if(ileParametrow<=0)return 0;
    tParametry=new float[ileParametrow];
    //printf("\nodczytano %d parametrow",ileParametrow);
    for(int i=0;i<ileParametrow;i++){
        tParametry[i]=atof(linieOdczytane.at(i+pocz).c_str());
        
    }
    
    return ileParametrow;
    //printf("\n indeksy szukanych parametrow  float pocz: %d, kon %d",pocz,kon);
}
int OdczytParametrow::OdczytajParametr(std::string& nazwa,int& par){
     int i,pocz,kon;
    i=0;pocz=0;kon=0;
    std::string nazwaEnd=nazwa+"End";
    while(i<linieOdczytane.size() && kon==0 ){//jeżeli koniec będzie ustawiony, to dalej nie sprawdza
        if(linieOdczytane.at(i)==nazwa)pocz=i+1;//pierwszy element listy
        if(linieOdczytane.at(i)==nazwaEnd)kon=i;//następny za ostatnim elementem listy
        i++;
    }
    //printf("pocz %d kon %d",pocz,kon);
    if((kon-pocz)!=1)return 0;
    //printf("odczytano %s",linieOdczytane.at(pocz).c_str());
    par=atoi(linieOdczytane.at(pocz).c_str());
    
//    par=(int)atof(linieOdczytane.at(pocz).c_str());
    //printf("\n indeksy szukanych parametrow int pocz: %d, kon %d",pocz,kon);
}
int OdczytParametrow::OdczytajParametr(std::string& nazwa,float& par){
     int i,pocz,kon;
    i=0;pocz=0;kon=0;
    std::string nazwaEnd=nazwa+"End";
    while(i<linieOdczytane.size() && kon==0 ){//jeżeli koniec będzie ustawiony, to dalej nie sprawdza
        if(linieOdczytane.at(i)==nazwa)pocz=i+1;//pierwszy element listy
        if(linieOdczytane.at(i)==nazwaEnd)kon=i;//następny za ostatnim elementem listy
        i++;
    }
    if((kon-pocz)!=1)return 0;
    par=atof(linieOdczytane.at(pocz).c_str());
    //printf("\n indeksy szukanych parametrow int pocz: %d, kon %d",pocz,kon);
}
void OdczytParametrow::WypiszParametry(){
    for(int i=0;i<linieOdczytane.size();i++){
        printf("\n %s",linieOdczytane.at(i).c_str());
    }
    printf("\nkoniec odczytanych parametrow");
}

