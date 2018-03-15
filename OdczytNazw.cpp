/* 
 * File:   OdczytNazw.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 1 marca 2016, 09:09
 */



#include <exception>

#include "OdczytNazw.h"

OdczytNazw::OdczytNazw(const char * aP) : OdczytPliku(aP, TEKSTOWO) {
}

OdczytNazw::OdczytNazw(const char* aP, char sposobOdczytu) {
    if(sposobOdczytu & BINARNIE) OdczytPliku(aP);
    if(sposobOdczytu & TEKSTOWO) OdczytPliku(aP, TEKSTOWO);
}


OdczytNazw::OdczytNazw(const OdczytNazw& orig) {
}

OdczytNazw::~OdczytNazw() {
}

std::string OdczytNazw::PodzialNazw(std::string& linia, int & iHip, std::string& sSymb) {
    int p0, p1, p2; //punkty podziału
    p0 = 0;
    p1 = linia.find('|');
    p2 = linia.find('_');

    iHip = atoi(linia.substr(0, p1).c_str());
   sSymb = linia.substr(p1 + 1, p2 - p1 - 1);
   
    return linia.substr(p2 + 1);
}

void OdczytNazw::PodzialNazwGwiazd(std::string& linia, iteratorLG * indeksIteratorow,std::map<std::string,std::string>& nazwyTlumaczenie) {
    int p1, p2, p3;
    p1 = linia.find('|');
    p2 = linia.find('(') + 2;
    p3 = linia.find(')')-1;
	if((p1 < 0) or (p2 < 0) or (p3 < 0))return;
	
    int nrGwiazdy = 0;
    std::string nazwaOryg;
    std::string nazwaTlum;
    try{
		nrGwiazdy = atoi(linia.substr(0, p1).c_str());
		//ustawienie nazwy we właściwym obiekcie
		nazwaOryg = linia.substr(p2, p3-p2);
		nazwaTlum = nazwyTlumaczenie[nazwaOryg];
    }catch(std::exception e){
     //gdyby mapa nie miała tłumaczeń   
        indeksIteratorow[nrGwiazdy]->UstawNazwe(nazwaOryg);
		printf("\nBlad OdczytNazw::PodzialNazwGwiazd linia: %X, p1: %d, p2: %d, p3: %d, blad %s",linia.c_str(),p1,p2,p3,e.what());
		//WypiszLinieOdczytane();
        return;
    }
        indeksIteratorow[nrGwiazdy]->UstawNazwe(nazwaTlum);
}

Gwiazdozbior OdczytNazw::PodzialNazwGwiazdozbiorow(std::string& linia,std::map<std::string, std::string>& tlumaczenie) {
    std::string nazwaSkrocona,nazwaOryg,nazwaTlumaczona;
    
    int p1, p2, p3;
    p1 = linia.find((char)9);//znak o numerze 9
    p2 = linia.find('(') + 2;
    p3 = linia.find(')')-1;
	if((p1 < 0) or (p2 < 0) or (p3 < 0)){
		return Gwiazdozbior("brak Skrotu","brak Tlumaczenia");
	}
    try{
		nazwaSkrocona = linia.substr(0,p1);
		nazwaOryg=linia.substr(p2,p3-p2);
        nazwaTlumaczona=tlumaczenie[nazwaOryg];
    }catch(std::exception &e){
        //std::string blad(e.what());
		printf("\nBlad OdczytNazw::PodzialNazwGwiazdozbiorow linia: %X, p1: %d, p2: %d, p3: %d, blad %s",linia.c_str(),p1,p2,p3,e.what());
		//WypiszLinieOdczytane();
        nazwaTlumaczona="brak nazwy";
    }
    
    return Gwiazdozbior(nazwaSkrocona,nazwaTlumaczona);
}

int OdczytNazw::IleOdczytanychLinii() {
    int r = 0;
    try {

        r = OdczytajKolejneLinieIzamknijStrumien(ifs);
    } catch (std::exception& e) {
        //coś zrobić, gdyby nie odczytało pliku
    }
    return r;
}

int OdczytNazw::UzupelnijIndeksyHipSymboliNazw(std::vector<int>& iHip, std::vector<std::string>& vSymb, std::deque<int>& dIleGwiazd, std::deque<int>& dPozycjaKolejnegoZbioru) {
    int i = 0, licznik = 0;
    std::string poprzedniaKonst("0"), biezacaKonst("0");

    for (std::deque<std::string>::iterator it = linieOdczytane.begin(); it != linieOdczytane.end(); it++) {
        biezacaKonst = PodzialNazw(*it, iHip.at(i), vSymb[i]);
        if (biezacaKonst.compare(poprzedniaKonst) != 0) {
            //tu dodać nowy element dIleGwiazd wraz z ilością zliczonych elementów
            dIleGwiazd.push_back(licznik);
            dPozycjaKolejnegoZbioru.push_back(i);
            nazwyOddzielone.push_back(biezacaKonst);
            licznik = 0; //bo liczymy następną konst od początku

        }
        i++;
        licznik++;
        poprzedniaKonst = biezacaKonst;
    }
    //może na koniec uciąć pierwszy element bo jest zerowy i ostatni z dPozycja
    if (dIleGwiazd.size() > 0) {

        dIleGwiazd.pop_front();
        dPozycjaKolejnegoZbioru.pop_back();
    }
    return dIleGwiazd.size();
}
int OdczytNazw::UzupelnijNazwyGwiazdozbiorow(std::vector<std::string>& nazwy){
	nazwy.assign(nazwyOddzielone.begin(), --nazwyOddzielone.end()); //czy drugi argument jest poprawny?
    return nazwy.size();
}

void OdczytNazw::UzupelnijPelneNazwyGwiazdozbiorow(std::deque<Gwiazdozbior>& gwiazdozbiory,std::map<std::string,std::string>& nazwyTlumaczenie) {
    OdczytajKolejneLinieIzamknijStrumien(ifs);
    int nrLinii = 0;
    try {
        for (std::deque<std::string>::iterator it = linieOdczytane.begin(); it != linieOdczytane.end(); it++) {
            nrLinii++;
            gwiazdozbiory.push_back(PodzialNazwGwiazdozbiorow(*it,nazwyTlumaczenie));
        }
    }catch (std::exception & e) {
    printf("\nOdczytNazw::UzupelnijPelneNazwyGwiazdozbiorow problem z linia nr %d %s", nrLinii, e.what());
    }
    
}


int OdczytNazw::UzupelnijNazwyGwiazd(iteratorLG * indeksIteratorow,std::map<std::string,std::string>& nazwyTlumaczenie) {
    clock_t start,koniec;
    start = clock();
    OdczytajKolejneLinieIzamknijStrumien(ifs);
    int nrLinii = 0;
    try {
        for (std::deque<std::string>::iterator it = linieOdczytane.begin(); it != linieOdczytane.end(); it++) {
            nrLinii++;
            PodzialNazwGwiazd(*it, indeksIteratorow,nazwyTlumaczenie);
        }
    }catch (std::exception & e) {
    printf("\nOdczytNazw::PodzialNazwGwiazd problem z linia nr %d %s", nrLinii, e.what());
}
    koniec = clock(); // bieżący czas systemowy w ms
    long delta = (long) (koniec - start); //czas działań w ms
    printf("\nprzetlumaczono i uzupelniono nazwy gwiazd w czasie %d",delta);
} 

void OdczytNazw::WypiszOddzieloneNazwy() {
    for (int i = 0; i < nazwyOddzielone.size(); i++) {
        printf("\n%s %d", nazwyOddzielone[i].c_str(), i);
    }

}

std::string OdczytNazw::NazwaWgNumeru(int i) {
    try {
        return nazwyOddzielone.at(i);
    } catch (std::out_of_range & oor) {
        return oor.what();
    }
}