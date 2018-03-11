/* 
 * File:   OdczytPliku.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 5 lutego 2016, 11:45
 */



//#include <GL\gl.h>
//#include <GL\glu.h>
#include "OdczytPliku.h"

OdczytPliku::OdczytPliku() {
    //    std::ifstream ifs("C:\\Program Files\\Stellarium\\stars\\default\\stars_0_0v0_5.cat", std::ios::binary); // otwieramy plik do odczytu binarnego
    //    PrzeczytajNaglowek(ifs);
    //    ileGwiazd = PrzeczytajStrefy(ifs);
    //    PrzeczytajGwiazdy(ifs);
    //    ifs.close();
    //    ObliczGwiazdy();
    //    //
    //    std::ifstream ifsW("C:\\Users\\BCO\\Documents\\NetBeansProjects\\CppApplication_2\\dist\\Debug\\MinGW-Windows\\Data.bin", std::ios::binary);
    //    OdczytajWierzcholki(ifsW); //odczytuje zapisane w javie liczby double po zamianie kolejności bajtów
    //   

    //    ZapiszWierzcholkiDoPliku();

}

OdczytPliku::OdczytPliku(const char * aP) : ifs(aP, std::ios::binary) {
    printf("\notwieranie pliku:  %s", aP);
    if (!ifs.is_open()) {
        this->~OdczytPliku();
        throw xBrakPliku(aP);

    }
}

OdczytPliku::OdczytPliku(const char * aP, char bParametry) {

    printf("\notwieranie pliku:  %s", aP);
    if (bParametry & TEKSTOWO){
        ifs.open(aP);
        printf("\notwarto plik %s do odczytu tekstowego",aP);
    }
        
    //bezpieczenstwo
    if (!ifs.is_open()) {
        this->~OdczytPliku();
        throw xBrakPliku(aP);
    }

}

OdczytPliku::OdczytPliku(const OdczytPliku& orig) {
    //printf("\ndestruktor odczyt pliku");
}

OdczytPliku& OdczytPliku::operator=(OdczytPliku& rhs) {
    this->adresPliku = rhs.adresPliku;
    this->ifs.open(rhs.adresPliku);
}

OdczytPliku::~OdczytPliku() {

    //printf("\ndestruktor OdczytPliku");
    if (ifs.is_open()) {
        ifs.close();
        //printf("  zamknieto plik");
    }
    //    delete pNag;
    //    delete[] ileGwiazdWstrefach;
    //    delete[] gwiazda;
}
int OdczytPliku::OdczytajKolejneLinie(std::ifstream & plik) {
    int nrLinii = 0;
    //printf("\nOdczytNazw::OdczytajKolejneLinie poczatek, rozmiar linieOdczytane %d",linieOdczytane.size());
    while (!plik.eof()) {
        std::string sWiersz;
        std::getline(plik, sWiersz);
        if (plik.bad()) //podczas próby odczytania danych wystąpił błąd sprzętowy
        {
            plik.close();
            printf("\nplik.bad");
            throw xBladOdczytu(nrLinii);
        }
        linieOdczytane.push_back(sWiersz);
        //printf("\nczytanie linii%d",nrLinii);
        
       // printf("\nczytanie linii bez DODAWANIA DO LISTY%d",nrLinii);
        nrLinii++;
        //printf("\n%s",sWiersz.c_str());

        //std::cout << sWiersz << std::endl;
    }
    printf("\nodczytano %d linii", linieOdczytane.size());
    return nrLinii;
}











