/* 
 * File:   ObliczeniaGwiazd.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 19 lutego 2016, 07:49
 */

#include <vector>
#include <new>
#include "ObliczeniaGwiazd.h"
#include "OdczytajGwiazdy.h"

ObliczeniaGwiazd::ObliczeniaGwiazd() {
    printf("\nObliczenia Gwiazd - poczatek");
    ow = 0;
}

ObliczeniaGwiazd::ObliczeniaGwiazd(OdczytWierzcholkow * pOw) : ow(0), podzial(0) {
    if (pOw != 0) this->ow = pOw;
    indeksHip = new int[wielkoscKataloguHip]; 
    indeksPosortowanychIter = 0;
    ileNajblizszychGwiazd = 2;
	indxItG = new std::list<Gwiazda2>::iterator[wielkoscKataloguHip];
    //zerujemy tablicę zakładając, że nie wszytskie rekordy będą zajęte
    int i = 0;
    try {

        for (i = 0; i < wielkoscKataloguHip; i++) {
            indeksHip[i] = 0;
        }
    } catch (std::out_of_range& oor) {
        printf("konstruktor obliczenia, problem z dostępem do tablicy w rekordzie o numerze %d, błąd: %s", i, oor.what());
        this->~ObliczeniaGwiazd();
    }
}

ObliczeniaGwiazd::ObliczeniaGwiazd(ObliczeniaGwiazd * gwPobierane, int ktoryGwiazdozbior, bool czyWprzestrzeni) : //trzeba wyzerować nieużywane pola składowe
ZbiorGwiazd(), ow(0), ileNazwanychGwiazd(0), kolejneSymboleZnazw(0), //kolejneHipZnazw(0),
ileGwiazdWgwiazdozbiorze(0), pozycjaKolejnegoGwiazdozbioru(0), indxItG(0), indeksGwiazdozbiorow(0), indeksPosortowanychIter(0), indeksHip(0) {//
    gwPobierane->PrzygotujGwiazdyOdejmujac(lGwiazdy, ktoryGwiazdozbior);
}

ObliczeniaGwiazd::~ObliczeniaGwiazd() {
    //tu usunąć obiekty inicjowane w metodzie pobierz gwiazdozbiory i wcześniej
    if (indxItG) delete[] indxItG;
    if (indeksHip) delete[] indeksHip;
    if (indeksPosortowanychIter) delete[]indeksPosortowanychIter;
    printf("\ndestruktor Obliczenia gwiazd");
}

void ObliczeniaGwiazd::SprawdzStanListy(std::list<Gwiazda2>::iterator& it, std::list<Gwiazda2>::iterator& iKoniec, OdczytajGwiazdy * og) {

    //gwiazdy którego poziomu
    short ktoryPoziom = og->Nag()->nDane[4];
    //sprawdzamy, czy dodano wcześniej do listy
    if (iteratoryKolejnychPoziomow.size() == ktoryPoziom)iteratoryKolejnychPoziomow.push_back(DodajGwiazdy(og->pGwiazda(), og->IleGwiazd()));
    //obliczenia trzeba przeprowadzić tylko dla tej częsci listy, której dotyczy wskaźnik og
    it = iteratoryKolejnychPoziomow.at(ktoryPoziom);
    //trzeba ocenić, czy lista zawiera gwiazdy z następnego poziomu
    iKoniec = (iteratoryKolejnychPoziomow.size() > ktoryPoziom + 1 ? iteratoryKolejnychPoziomow.at(ktoryPoziom + 1) : lGwiazdy.end());
    //printf("\nsprawdz stan listy Koniec, czy poczatek rozni sie od konca %d",(it!=iKoniec));
}

int ObliczeniaGwiazd::UsunBezParalaxy() {
    std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
    int licznik = 0;
    while (it != lGwiazdy.end()) {
        if (it->Plx() == 0) {
            it = lGwiazdy.erase(it);
            licznik++;

        } else it++;
    }
    return licznik;
}

void ObliczeniaGwiazd::PobierzParametry(OdczytParametrow * op) {
    std::string p1 = "ileNajblizszychGwiazd";
    op->OdczytajParametr(p1, ileNajblizszychGwiazd);
}

void ObliczeniaGwiazd::ObliczGwiazdy(OdczytajGwiazdy * og) {

    clock_t start, koniec;
    start = clock(); // bieżący czas systemowy w ms
    // działania, których czas zamierzamy zmierzyć 
	std::list<Gwiazda2>::iterator it, ikoniec;
    SprawdzStanListy(it, ikoniec, og);
    float krok = (float) og->Nag()->nDane[6] / og->Nag()->nDane[7];
    float plx, odl, m, M, w, Aw;
    int j = 0;
    int maxBV = 0;
    int hipMaxBV = 0;
    int sumaBV = 0;
    for (it; it != ikoniec; it++) {
        //obliczenia odległości, wielkości pozornej i absolutnej
        plx = it->Plx();
        m = (og->Nag()->nDane[5] + krok * it->Mag()) / 1000;
        (plx != 0) ? M = m + 5 * (1 + log10(plx / 100000)) : M = m; //w niektórych brakuje odległości/paralaxy
        (plx != 0) ? odl = (326212 / plx) : odl = 0; //odległość
        w = 1 + pow((15.0 - m)*0.14, 3); //wielkość do wyświetlenia
        Aw = 1 + pow((15.0 - M)*0.16, 2); //wielkość do wyświetlenia
        it->Ustaw(m, M, odl, w, Aw);
        j++;
        int BV = it->B_v();
        if (BV > maxBV) {
            maxBV = BV;
            hipMaxBV = it->Hip();
        }
        sumaBV += BV;


    }
    float srednioBV = (float) sumaBV / j;
    koniec = clock(); // bieżący czas systemowy w ms
    long delta = (long) (koniec - start); //czas działań w ms
    printf("\nobliczono %d gwiazd, max BV %d dla gwiazdy Hip %d, srednio %2.3f w czasie %d ms", j, maxBV, hipMaxBV, srednioBV, delta);
}

//

void ObliczeniaGwiazd::UstawIndeksy() {
    //można na początku zrobić sortowanie całej listy
    //        lGwiazdy.sort(wPlx);
    //po sortowaniu prawdopodobnie można ustawiac ilośc wyświetlanych gwiazd
    clock_t start, start1, koniec1, koniec;
    start = clock();
    int j = 0;
    int ileZerowaPlx = 0;
    int hip;

    //listaItLG nrHipWzdluzX,nrHipWzdluzY,nrHipWzdluzZ;
    std::list<Gwiazda2>::iterator ite, it;
    ite = lGwiazdy.end();
    it = lGwiazdy.begin();
    std::list<std::list<Gwiazda2>::iterator> lItGzerowyHip; //lista iteratorów do gwiazd, w których hip-y były użyte wcześniej, najczęściej zerowe

    //tu trzeba uzupełnić tablicę iteratorów, żeby nie wskazywały pustych miejsc
    //czyli te nie wypełnione wskazują na gwiazdę Fomalhaut
    for (int i = 0; i < wielkoscKataloguHip; i++) {
        indxItG[i] = ite;
        indeksHip[i] = -1;
    }
    try {
        while (it != lGwiazdy.end()) {

            hip = it->Hip();
            //żeby ustawiał te, które jeszcze nie są ustawione
            if (indeksHip[hip] == -1 && it->Plx() > 0) {
                if (hip > wielkoscKataloguHip)printf("\nUstawIndeksy nr hip %d poza zakresem", hip);
                else {
                    indeksHip[hip] = j; //potrzebne do sprawdzania poprawności odczytu rekordów
                    indxItG[hip] = it;
                }
                j++;
                it++;
            } else {
                it = lGwiazdy.erase(it);
                ileZerowaPlx++;
             }
        }
        //w tym miejscu sprawdzamy, które są wolne hip, tzn gdzie jest -1
        std::list<int> wolneNrHip;
        for (int i = 0; i < wielkoscKataloguHip; i++) {
            if (indeksHip[i] == -1)wolneNrHip.push_back(i);
        }
        //przydzielenie wolnych numerów hip

        rozmiarLGwiazdy = lGwiazdy.size();
        printf("\nustawiono indeksy dla %d gwiazd, rozmiar lGwiazdy %d usunieto %d gwiazd o powtarzającym się numerze,"
                " liczba nr hip do wykorzystania %d", j, rozmiarLGwiazdy, ileZerowaPlx, wolneNrHip.size());
    } catch (std::out_of_range& oor) {
        printf("Metoda UstawIndeksy, problem z dostępem do tablicy iteratorów, obiekt listy nr %d, błąd: %s", j, oor.what());
    }
    start1 = clock();

    /*********************przydzielenie gwiazd do sześcianów *****************/
    PrzydzielDoStrefPrzestrzennych(lGwiazdy);

    //dla gwiazd, które nie przynależą do żadnego gwiazdozbioru będzie zwracało -1
    vIndeksGwiazdozbiorow.assign(wielkoscKataloguHip, -1);
    //dla każdej oddzielonej nazwy trzeba zrobić pętlę po wszystkich jej gwiazdach
    int ktoraGwiazda = 0;
    int ileBledow = 0;
    iteratorLG gwiazdaDoUstawienia,koniecListy;
    koniecListy=lGwiazdy.end();
    //zewnętrzna pętla jest wykonana 193 -tyle jest wydzielonych nazw
    for (int i = 0; i < ileGwiazdWgwiazdozbiorze.size(); i++) {
        for (int j = 0; j < ileGwiazdWgwiazdozbiorze.at(i); j++) {
            try {
                if (ktoraGwiazda < ileNazwanychGwiazd) {

                    //zdarza się,że gwiazda należy do kilku gwiazdozbiorów, niech będzie zapisane pierwsze jej wystąpienie
                    int hip1 = kolejneHipZnazw.at(ktoraGwiazda);
                    if (vIndeksGwiazdozbiorow.at(hip1) == -1)vIndeksGwiazdozbiorow.at(hip1) = i;
                    gwiazdaDoUstawienia=indxItG[hip1];
					if(gwiazdaDoUstawienia!=koniecListy)gwiazdaDoUstawienia->UstawSymbol(kolejneSymboleZnazw.at(ktoraGwiazda));
                    ktoraGwiazda++;
                }
            } catch (std::out_of_range & oor) {
                ileBledow++;

            }
        }

    }
    //ustawienie symboli gwiazd:
	//sprawdzenie zawartosci vektora
    int licznik = 0;
    std::vector<int>::iterator itv = vIndeksGwiazdozbiorow.begin();
    while (itv != vIndeksGwiazdozbiorow.end()) {
        if (*itv >= 0)licznik++;
        itv++;
    }
    koniec = clock(); // bieżący czas systemowy w ms
    long delta = (long) (koniec - start); //czas działań w ms
    printf("\nkoniec metody Ustaw indeksy, przyporzadkowano gwiazdozbiory do %d gwiazd, wystapily bledy %d, czas wykonania %d ms", licznik, ileBledow, delta);
}
void ObliczeniaGwiazd::PobierzTlumaczenie(const char* adresPliku,int odKtoregoMiejsca ) {
    //nie jest dobrze znana budowa pliku .qm, więc czytamy od określonego miejsca, tam gdzie zaczynają się nazwy
    //powinno utworzyć pary (czyli mapę, lub dwa vectory)nazw oryginalnych i przetłumaczonych
    std::ifstream strumienOtwarty(adresPliku, std::ios::binary);
    if (!strumienOtwarty.is_open()) {
        throw xBrakPliku(adresPliku);
    }
    //ilość bajtów do odczytania
    strumienOtwarty.seekg(0,std::ios::end);
    int rozmiarPliku=strumienOtwarty.tellg();
    //ustawienie miejsca od ktorego czytamy:
    strumienOtwarty.seekg(odKtoregoMiejsca,std::ios::beg);
    unsigned char * cdlugoscNazwy=new unsigned char;
    unsigned dlugoscNazwy=0;
    char * nazwa=new char[128];
    //w celu konwersji znaków:
    wchar_t * wNazwa=new wchar_t[64];
    
    
    while(strumienOtwarty.tellg()<rozmiarPliku){
        std::string sNazwa1;
        //uzyskanie długości nazwwy
        strumienOtwarty.read((char*)cdlugoscNazwy,1);
        dlugoscNazwy=(int)*cdlugoscNazwy;
        strumienOtwarty.read((char*)nazwa,dlugoscNazwy);
        //kopiowanie w celu sprawdzenia polskich znaków:
        memcpy(wNazwa,nazwa,dlugoscNazwy);
        //zamiana znaków:
        for(int i=0;i<dlugoscNazwy/2;i++){
            //w zapisie szestnastkowym należy bajty zapisywać od końca
            if(wNazwa[i]==0x4201)wNazwa[i]=0x82c5;//"ł"
            if(wNazwa[i]==0xf300)wNazwa[i]=0xb3c3;//"ó"
            if(wNazwa[i]==0x4401)wNazwa[i]=0x84c5;//"ń"
            if(wNazwa[i]==0x7a01)wNazwa[i]=0xbac5;//"ź"
            if(wNazwa[i]==0x0501)wNazwa[i]=0x85c4;//"ą"
            if(wNazwa[i]==0x0701)wNazwa[i]=0x87c4;//"ć"
            if(wNazwa[i]==0x1901)wNazwa[i]=0x99c4;//"ę"
            if(wNazwa[i]==0x7c01)wNazwa[i]=0xbcc5;//"ż"
            if(wNazwa[i]==0x5b01)wNazwa[i]=0x9bc5;//"ś"
            if(wNazwa[i]==0x4101)wNazwa[i]=0x81c5;//"Ł"
            if(wNazwa[i]==0x7b01)wNazwa[i]=0xbbc5;//"Ż"
            if(wNazwa[i]==0x7901)wNazwa[i]=0xb9c5;//"Ź"
        }
        memcpy(nazwa,wNazwa,dlugoscNazwy);
        //kopiowanie do stringa
        for(int i=0;i<dlugoscNazwy;i++){
            if(0!=nazwa[i])sNazwa1+=nazwa[i];
        }
        strumienOtwarty.seekg(9,std::ios::cur);
        strumienOtwarty.read((char*)cdlugoscNazwy,1);
        dlugoscNazwy=(int)*cdlugoscNazwy;
        strumienOtwarty.read((char*)nazwa,dlugoscNazwy);
        std::string sNazwa2(nazwa,dlugoscNazwy);
		strumienOtwarty.seekg(10,std::ios::cur);
        nazwyTlumaczenie[sNazwa2]=sNazwa1;
    }
   strumienOtwarty.close();
   delete[] nazwa;
   delete[] wNazwa;
}
void ObliczeniaGwiazd::UstalPolozenie(OdczytajGwiazdy * og) {
    /* żeby rozpoznawało poziom i związaną z tym ilość stref a metodę Gwiazda2::UstawWspolrzedne nadpisać, żeby wiedziała w jak dużym trójkącie się znajduje*/
    std::list<Gwiazda2>::iterator it, ikoniec;
    SprawdzStanListy(it, ikoniec, og);
    short ktoryPoziom = og->Nag()->nDane[4];
    //sprawdzamy, jaki jest aktualny poziom w obiekcie odczyt Wierzcholkow
    try {

        if (ktoryPoziom > ow->Poziom())ow->WyznaczSrodkiKolejnegoPoziomu();
    } catch (std::out_of_range& oor) {
        printf("\nUstalPolozenie problem z ow->WyznaczSrodkiKolejnegoPoziomu %s", oor.what());
    }
    //skoro wiemy jaki mamy poziom, to musimy obliczyć, z którego miejsca listy srodkow ścian mamy czytać
    int offWierzcholkow = ow->PoczatekAktualnychSrodkowScian();
    double zmniejszenie = pow(2, ktoryPoziom); //wraz z kolejnym podziałem współrzędne x0 i x1 w gwiazdach muszą być odpowiednio zmniejszane
    double wsp[3]; //współrzędne wektora środka strefy
    printf("\nustalenie polozenia dla gwiazd poziomu %d", ktoryPoziom);
    int licznik = 0;
    for (int i = 0; i < og->IleStref(); i++) {//og.IleStref()
        //inny sposób jest taki, żeby obliczyć kąt i oś obrotu od wektora do punktu 1,0,0
        double wspBieg[2];
        //if (i < 20) {//zabezpieczenie przed czytaniem za końcem tablicy
        //ustalenie współrzędnych
        wsp[0] = ow->SrodkiScian(offWierzcholkow + 3 * i + 0);
        wsp[1] = ow->SrodkiScian(offWierzcholkow + 3 * i + 1);
        wsp[2] = ow->SrodkiScian(offWierzcholkow + 3 * i + 2);
        //obliczenie kąta i osi obrotu do wektora 1,0,0
        //KatIWektorObrotuMiedzyWektoramid(katIosObrotu, wekt100, wsp);

        xyz_to_LF(wspBieg, wsp);
        for (int j = 0; j < og->IleGwiazdWstrefach()[i]; j++) {//for (int j = 0; j < og.IleGwiazdWstrefach()[i]; j++) {
            licznik++;
            it->UstawWspolrzedne(wspBieg, zmniejszenie); //
            //it->PokazWspolrzedneXYZ();
            it++;
        }
    }

    printf("\nustalono polozenie %d gwiazd", licznik);

}

void ObliczeniaGwiazd::PrzydzielDoStrefPrzestrzennych(std::list<Gwiazda2>& listaDoRozdzielenia) {

    podzial->PrzydzielDoStref(listaDoRozdzielenia);

}

int ObliczeniaGwiazd::DodajKolejnePoziomyJakoGwiazdozbiory() {

    int i = 0;
    int miejsceDoWstawienia = 0;
    std::list<Gwiazda2>::iterator it, kon;
    printf("\nDodajKolejnePoziomyJakoGwiazdozbiory iteratoryKolejnychPoziomow.size() %d",iteratoryKolejnychPoziomow.size());
    int ilePoziomow = iteratoryKolejnychPoziomow.size();
    for (i; i < iteratoryKolejnychPoziomow.size(); i++) {
        int j = 0;
        int bledy = 0;
        printf("\npoziom %d", i);
        it = iteratoryKolejnychPoziomow.at(i);
		Gwiazda2 gwiazdaTemp = *it;
        kon = (i + 1 < iteratoryKolejnychPoziomow.size()) ? iteratoryKolejnychPoziomow.at(i + 1) : lGwiazdy.end();
        //kopiowanie numerow do listy, należy znaleźć miejsce od którego można uzupełniać tablicę kolejneHipZnazw
        //pola ileGwiazdWgwiazdozbiorze oraz pozycjaKolejnegoGwiazdozbioru dadzą informację do którego miejsca tablica jest zajęta
        int pKG = pozycjaKolejnegoGwiazdozbioru.back();
        int iGG = ileGwiazdWgwiazdozbiorze.back();
        miejsceDoWstawienia = pKG + iGG;
        pozycjaKolejnegoGwiazdozbioru.push_back(miejsceDoWstawienia);
        try {
            while (it != kon) {

                int hip = it->Hip();

                if (hip > 0 && hip < 200000)kolejneHipZnazw.at(miejsceDoWstawienia + j++) = hip;
                it++;
            }
        } catch (std::exception& e) {
            printf("\nDodajKolejnePoziomyJakoGwiazdozbiory blad hip = it->Hip() %s,miejsce %d", e.what(), miejsceDoWstawienia + j);
            bledy++;
        }



        ileGwiazdWgwiazdozbiorze.push_back(j);
        printf(" dodano %d gwiazd, bledy %d", j, bledy);
    }
    return i;
}

int ObliczeniaGwiazd::ListaWspolrzednych(int ile, std::deque<double> & lista) {
    //jeżeli nie dodano gwiazd, to lista powinna być pusta
	if (!lGwiazdy.empty() && ile == 0) {
        std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
        while (it != lGwiazdy.end()) {
			lista.push_back(it->WielkoscDoWyswietlenia());
            lista.push_back(it->X());
            lista.push_back(it->Y());
            lista.push_back(it->Z());
            it++;
        }
    }
    if (!lGwiazdy.empty() && ile > 0) {
        std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
        for (int i = 0; i < ileStref; i++) {
            int licznik = 0;
            while (licznik < ile) {
                lista.push_back(it->WielkoscDoWyswietlenia());
                lista.push_back(it->X());
                lista.push_back(it->Y());
                lista.push_back(it->Z());
                it++;
                licznik++;
            }
            int skok = ileGwiazdWstrefach[i] - ile + 1;
            std::advance(it, skok);
        }
    }

    return lista.size();
}

int ObliczeniaGwiazd::ListaWspolrzednychZOdleglosciami(std::deque<double> & lista, listaItLG & listaIteratorow) {
    if (!lGwiazdy.empty()) {
        std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
        int licznik = 0;
        while (it != lGwiazdy.end()) {
            double odl = (double) it->Odleglosc() / 100;
            double k, w, x, y, z;
            k = (double) it->B_v();
            if (k > 127.0) k = 127.0; //żeby nie czytało za końcem tablicy r[128],g[128],b[128]
            w = it->AbsolutnaWlkDoWyswietlenia();
            x = odl * it->X();
            y = odl * it->Y();
            z = odl * it->Z();
            lista.push_back(k);
            lista.push_back(w);
            lista.push_back(x);
            lista.push_back(y);
            lista.push_back(z);
            //z przodu są z nazwami, a dalej bez nazw
            if (!it->Nazwa().empty())listaIteratorow.push_front(it);
            else listaIteratorow.push_back(it);

            it++;
        }
    }
    return lista.size();
}

int ObliczeniaGwiazd::ListaWspolrzednychParPunktow(std::deque<double> & lista) {

    if (!lGwiazdy.empty()) {
        std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
        while (it != lGwiazdy.end()) {
            double odl = (double) it->Odleglosc() / 100;
            double w, x, y, z;
            w = it->AbsolutnaWlkDoWyswietlenia();
            x = odl * it->X();
            y = odl * it->Y();
            z = odl * it->Z();
            //Najpierw punkt leżący na sferze,
            lista.push_back(it->WielkoscDoWyswietlenia());
            lista.push_back(it->X());
            lista.push_back(it->Y());
            lista.push_back(it->Z());
            //a potem w przestrzeni
            lista.push_back(w);
            lista.push_back(x);
            lista.push_back(y);
            lista.push_back(z);
            it++;
        }
    }
    return lista.size();
}

std::list<int>::iterator ObliczeniaGwiazd::ListaHip(std::list<int>& listaDoWypelnienia, std::list<int>::iterator przedKtorymWstawic) {
    //odczytuje hipy z całej swojej listy i wstawia je do listyDoWypelnienia zwracając iterator na poczatek wstawionych danych
    if (lGwiazdy.empty()) {
        //ten przypadek występuje, jeśli gwiazda jest wskazana w dwóch gwiazdozbiorach najpierw jest w jednej liście, a potem druga ją przechwytje
        //tzn jej iteretor i wtedy ta jest pusta
        printf("\nLista hip, nie ma nic do wstawienia");
        return przedKtorymWstawic;
    }

    std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
    std::list<int>::iterator itr; //itr iterator do zwrotu na końcu funkcji

    //przygoowanie iteratora do zwrotu
    itr = listaDoWypelnienia.insert(przedKtorymWstawic, it->Hip());
	it++;

    //pętlę zaczynamy od drugiego elementu
    while (it != lGwiazdy.end()) {
        listaDoWypelnienia.insert(przedKtorymWstawic, it->Hip());
        it++;
    }
    return itr;
}

int ObliczeniaGwiazd::PobierzGwiazdozbiory(OdczytNazw * on,OdczytNazw * nazwyGwiazdozbiorow) {
    char t[] = {"do obiektu odczyt nazw"};
    if (!on)throw xNullPointerException(t);
    ileNazwanychGwiazd = on->IleOdczytanychLinii();
    //trzeba zrobić miejsce dodatkowe na wszystkie gwiazdy, tak aby każdy poziom mógł być wyswietlony jako osobny gwiazdozbiór
    kolejneHipZnazw.assign(ileNazwanychGwiazd + lGwiazdy.size() + ileNajblizszychGwiazd, -1);
    //zakładam, że każdy string będzie składał się z trzech zer
    kolejneSymboleZnazw.assign(ileNazwanychGwiazd, std::string(3, '0'));
    //printf("\nprzed uzupelnieniem gwiazdozbiorow");
    printf("\nuzupelniono dane %d gwiazdozbiorów",
		on->UzupelnijIndeksyHipSymboliNazw(kolejneHipZnazw, kolejneSymboleZnazw, ileGwiazdWgwiazdozbiorze, pozycjaKolejnegoGwiazdozbioru));
	printf("\nuzupelniono nazwy %d gwiazdozbiorow", on->UzupelnijNazwyGwiazdozbiorow(vNazwy));//nazwy skrócone
    nazwyGwiazdozbiorow->UzupelnijPelneNazwyGwiazdozbiorow(dGwiazdozbiorDoPrzycisku,nazwyTlumaczenie);
    //w tym miejscu należy ustalić dla każdej pozycji dGwiazdozbiorDoPrzycisku od którego numeru się zaczyna, i ile zawiera numerów np. Orion 129 9numerów
    std::deque<Gwiazdozbior>::iterator itDaneGwiazdozbiorow;
    std::vector<std::string>::iterator itNazwySkrocone=vNazwy.begin();
    std::string poprzedniaNazwaSkrocona(*itNazwySkrocone,0,3);
    int liczbaWystapien=0;
    int liczbaPorzadkowa=0;
    while(itNazwySkrocone!=vNazwy.end()){
        //ustalić nazwy, które się powtarzają
        std::string aktualnaNazwaSkrocona(*itNazwySkrocone,0,3);
        //jeśli się powtarza
        if(aktualnaNazwaSkrocona.compare(poprzedniaNazwaSkrocona)==0){
           liczbaWystapien++; 
        }else{
            //zapisać liczbę porzadkową i liczbę wystąpień
            //należy znaleźć odpowiedni gwaizdozbior, bo jest w innym porządku
            itDaneGwiazdozbiorow=dGwiazdozbiorDoPrzycisku.begin();
            while(itDaneGwiazdozbiorow!=dGwiazdozbiorDoPrzycisku.end()){
                if(itDaneGwiazdozbiorow->NazwaSkrocona().compare(poprzedniaNazwaSkrocona)==0)break;
                itDaneGwiazdozbiorow++;
            }
            if(itDaneGwiazdozbiorow!=dGwiazdozbiorDoPrzycisku.end()){
                itDaneGwiazdozbiorow->UstawNumery(liczbaPorzadkowa,liczbaWystapien);
            }else
                printf("\nnie znaleziono %s",poprzedniaNazwaSkrocona.c_str());
            //następnie je zwiększyć lub odpowienio ustawić
            itDaneGwiazdozbiorow++;
            liczbaPorzadkowa+=liczbaWystapien;
            liczbaWystapien=1;
        }
        poprzedniaNazwaSkrocona=aktualnaNazwaSkrocona;
        itNazwySkrocone++;
    }
    //sortowanie listy gwiazdozbiorow
    std::list<Gwiazdozbior> lGwiazdozbiory(dGwiazdozbiorDoPrzycisku.begin(),dGwiazdozbiorDoPrzycisku.end());
    lGwiazdozbiory.sort(WgNazwy);
    dGwiazdozbiorDoPrzycisku.clear();
    dGwiazdozbiorDoPrzycisku.assign(lGwiazdozbiory.begin(),lGwiazdozbiory.end());
}

int ObliczeniaGwiazd::PobierzNazwyGwiazd(OdczytNazw * nazwyGwiazd) {
    char t[] = {"do obiektu odczyt nazw gwiazd"};
    if (!nazwyGwiazd)throw xNullPointerException(t);
    nazwyGwiazd->UzupelnijNazwyGwiazd(indxItG,nazwyTlumaczenie);
    return 1;
}

int ObliczeniaGwiazd::KtorePunktyWidac(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip, listaItLG & listaIteratorow) {
    podzial->KtorePunktyWidac(p, punktyWszystkichGwiazd, listaHip, listaIteratorow);
}

void ObliczeniaGwiazd::PrzygotujGwiazdyOdejmujac(std::list<Gwiazda2>& listaDoWypelnienia, int ktoryGwiazdozbior) {
    //trzeba zrobić zabezpieczenie przed próbą dostępu, jeśli nie ma gwiazd
    if (lGwiazdy.empty() && !listaDoWypelnienia.empty()) {
        printf("\nprzerwanie metody PrzygotujGwiazdyOdejmujac z powodu zerowego rozmiaru listy zrodlowej");
        return;
    }
    //które gwiazdy
    int pocz = 0;
    int kon = 0;
    pocz = pozycjaKolejnegoGwiazdozbioru.at(ktoryGwiazdozbior);
    kon = pocz + ileGwiazdWgwiazdozbiorze.at(ktoryGwiazdozbior);

    //printf("\nPrzygotujGwiazdyOdejmujac indeksy przekazywanych gwiazd poczatek %d, koniec %d, gwiazdozbior %d", pocz, kon, ktoryGwiazdozbior);
    int hip = 0;
    iteratorLG itG;
    int bladUsuniecia = 0;
    int rozmiarWczesniejszy = podzial->RozmiarListy();
    for (int i = pocz; i < kon; i++) {
        try {
            //jeżeli dana gwiazda już jest przeniesiona do innej listy, to metoda nie wie nic o tym, wycina z tamtej innej listy
            //brak pomysłu na zabezpieczenie przed tym
            hip = kolejneHipZnazw.at(i);
            itG = indxItG[hip];
            if (itG == lGwiazdy.end())continue;
            listaDoWypelnienia.splice(listaDoWypelnienia.end(), lGwiazdy, itG);
            //gwiazda po przejściu do nowej listy, powinna się "wyrejestrować" z listy lGwiazdyWkomorkach
            //nie mam pewności, czy lGwiazdyWkomorkach należy do właściwego obiektu i, czy w ogóle, coś zawiera:
           bladUsuniecia += podzial->UsunIterator(itG);
        } catch (std::exception& e) {
            printf("\n blad iteratora %s", e.what());
        }
    }
    if (bladUsuniecia > 0)printf("\nPrzygotujGwiazdyOdejmujac lGwiazdyWkomorkach jest pusta i nie mozna z niej usuwac, lub proba ponownego usuniecia ile razy %d", bladUsuniecia);
}

void ObliczeniaGwiazd::PrzekazGwiazdyDoBazy(ObliczeniaGwiazd * oBaza) {
    //zakładamy, że przekazujemy całość, po tym własna lista powinna być pusta
    int rozmiarPierwotny = lGwiazdy.size();
    //należy wszystkie gwiazdy "wyrejestrowane" zlisty lGwiazdyWkomorkach powtórnie do niej dodać
    oBaza->PrzydzielDoStrefPrzestrzennych(lGwiazdy);
    oBaza->LGwiazdy().splice(oBaza->LGwiazdy().begin(), lGwiazdy);
}

std::string ObliczeniaGwiazd::DoJakiegoGwiazdozbioruNalezy(int hip) {
    if (vIndeksGwiazdozbiorow.at(hip) < 0)return std::string("znaleziona gwiazda poza gwiazdozbiorem");
    try {
        std::string nazwaSkrocona,nazwa;
        nazwaSkrocona=vNazwy.at(vIndeksGwiazdozbiorow.at(hip));
        std::deque<Gwiazdozbior>::iterator it,kon;
        it=dGwiazdozbiorDoPrzycisku.begin();
        kon=dGwiazdozbiorDoPrzycisku.end();
        while(it!=kon){
            if(it->NazwaSkrocona()==nazwaSkrocona){
                nazwa=nazwaSkrocona+" "+it->Nazwa();
                break;
            }
            it++;        
        }
        return nazwa;
    } catch (std::out_of_range & oor) {
        printf("\n%s", oor.what());
        return std::string("brak nazwy");
    }

}

int ObliczeniaGwiazd::DoKtoregoGwiazdozbioruNalezy(int hip) {
    return vIndeksGwiazdozbiorow.at(hip);
}

std::list<Gwiazda2>::iterator ObliczeniaGwiazd::IteratorWgHip(int hip) {
    if (hip >= 0 && hip < wielkoscKataloguHip)return indxItG[hip];
    else throw xNullPointerException("nie znaleziono odpowiedniej gwiazdy");
}

float ObliczeniaGwiazd::ObliczDystans(float x, float y, float z, int nrHip) {
    return indxItG[nrHip]->DystansDo(x, y, z);
}

bool  ObliczeniaGwiazd::NajblizszeGwiazdy(int nrHip) {
    if(nrHip<0 || nrHip >wielkoscKataloguHip)return false;
    clock_t start, koniec;
    start = clock();
    iteratorLG itCentralnej;
    itCentralnej = indxItG[nrHip];
    float cx, cy, cz;
    cx = itCentralnej->OX();
    cy = itCentralnej->OY();
    cz = itCentralnej->OZ();
	
	//potrzebujemy mieć dostęp do wszystkich gwiazd, również tych, które przeniesiono z lGwiazdy do list gwiazdozbiorów
    float odleglosci[wielkoscKataloguHip];
    int indeksyPosortowane[wielkoscKataloguHip];

    int i = 0;
    while (i < rozmiarLGwiazdy) {
        odleglosci[i] = indxItG[i]->KwadratDystansuDo(cx, cy, cz);
		indeksyPosortowane[i] = i;
		i++;
    }
    int brzeg = 0;
     while (brzeg < ileNajblizszychGwiazd)SortujIndeksy(odleglosci, indeksyPosortowane, brzeg++, rozmiarLGwiazdy);

    //listę tych gwiazd należy dodać jako kolejny gwiazozbiór
    //w tym celu należy odszukać miejsce na liście, gdzie można umieszczać numery najbliższych gwiazd
    //sprawdzenie, co jest w kontenerach:pozycjaKolejnegoGwiazdozbioru, ileGwiazdWgwiazdozbiorze
    
    int miejsceDoWstawienia = 0;
    int pKG = pozycjaKolejnegoGwiazdozbioru.back();
    int iGG = ileGwiazdWgwiazdozbiorze.back();
    //zmienna statyczna powoduje problemy przy wielowątkowości
        bool czyDodawac=(ileGwiazdWgwiazdozbiorze.size()<197);//czy już poprzednio ustawiane były najbliższe gwiazdy
	miejsceDoWstawienia = pKG + iGG;
    if (czyDodawac) pozycjaKolejnegoGwiazdozbioru.push_back(miejsceDoWstawienia);
    else pozycjaKolejnegoGwiazdozbioru.at(196) = miejsceDoWstawienia;
    i = 0;
    for (i; i < ileNajblizszychGwiazd; i++) {
        try {
            kolejneHipZnazw.at(miejsceDoWstawienia + i) = indeksyPosortowane[i];
        } catch (std::exception & e) {
            printf("\nNajblizszeGwiazdy problem z dostepem do kolejneHipZnawz i:%d blad %s", i, e.what());
        }
	}
	if (czyDodawac)ileGwiazdWgwiazdozbiorze.push_back(i);
    else ileGwiazdWgwiazdozbiorze.at(196) = i;
    czyDodawac = false;
    koniec = clock();
    long delta = (long) (koniec - start);
    printf("\nwyznaczenie najblizszych %d gwiazd zajelo %d ms", ileNajblizszychGwiazd, delta);
    return true;
}
void __cdecl WatekObliczenGwiazd(void * p){
    ParametryWatkuObliczenGwiazd * param;
    ObliczeniaGwiazd * paramBaza;
    WFbi paramFunkcja;
    try{
        //rzutowanie wskaźnika (adresu) bez określonego typu na typ obiektu ParametryWątku, który "transportuje" więcej danych
        param = reinterpret_cast<ParametryWatkuObliczenGwiazd *>(p);
        int Hip=param->hip;
        paramBaza=param->oBaza;//wskaźnik na obiekt
        paramFunkcja=param->wskaznik;//wskaźnik na funkcję klasy
         //wywolanie funkcji danego obiektu
        (paramBaza->*paramFunkcja)(Hip);
         
    }catch(std::exception &e){
        printf("problem z watekObliczenGwiazd %s",e.what());
        
    }
    delete param;
    _endthread();
}
