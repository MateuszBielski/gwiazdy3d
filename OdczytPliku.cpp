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
}

OdczytPliku::OdczytPliku(const char * aP) : ifs(aP, std::ios::binary) {
    printf("\notwieranie pliku:  %s", aP);
	nazwaPliku.assign(aP);
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
		nazwaPliku.assign(aP);
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

    if (ifs.is_open()) {
        ifs.close();
    }
}
int OdczytPliku::OdczytajKolejneLinieIzamknijStrumien(std::ifstream & plik) {
    int nrLinii = 0;
	plik.seekg(0);
	std::string sWiersz;
     while (!plik.eof()) {
        std::getline(plik, sWiersz);
		if(!sWiersz.size())continue;
        if (plik.bad()) //podczas próby odczytania danych wystąpił błąd sprzętowy
        {
            plik.close();
            printf("\nplik.bad");
            throw xBladOdczytu(nrLinii);
        }
        linieOdczytane.push_back(sWiersz);
        nrLinii++;
     }
	 plik.close();
    printf("\nodczytano %d linii", linieOdczytane.size());
    return nrLinii;
}
void OdczytPliku::WypiszLinieOdczytane()
{	
	iterDS linia, koniecLinii;
	linia = linieOdczytane.begin();
	koniecLinii = linieOdczytane.end();
	int ktora = 0;
	printf("\n%s, odczytano:",nazwaPliku);
	while(linia != koniecLinii){
		printf("\n%d. %s",ktora++,(*linia++).c_str());
	}
	
}
