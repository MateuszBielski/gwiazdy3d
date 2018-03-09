/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ObliczeniaGwiazd.cpp
 * Author: BCO
 * 
 * Created on 19 lutego 2016, 07:49
 */

#include <vector>
#include <new>
//#include <winbase.h>

#include "ObliczeniaGwiazd.h"
#include "OdczytajGwiazdy.h"

ObliczeniaGwiazd::ObliczeniaGwiazd() {
    printf("\nObliczenia Gwiazd - poczatek");
    ow = 0;
}

ObliczeniaGwiazd::ObliczeniaGwiazd(OdczytWierzcholkow * pOw) : ow(0), podzial(0) {
    //printf("\nObliczenia Gwiazd z wierzcholkami - początek");
    if (pOw != 0)
        this->ow = pOw;
    indeksHip = new int[wielkoscKataloguHip]; //tyle zawiera katalog Hipparcosa
    indeksPosortowanychIter = 0;
    ileNajblizszychGwiazd = 2;

    //vIndeksGwiazdozbiorow.assign(wielkoscKataloguHip,0);//*********************zainicjowanie w tym miejscu powoduje bład, dlaczego?
    indxItG = new std::list<Gwiazda2>::iterator[wielkoscKataloguHip];
    //zerujemy tablicę zakładając, że nie wszytskie rekordy będą zajęte
    int i = 0;
    try {

        for (i = 0; i < wielkoscKataloguHip; i++) {
            indeksHip[i] = 0;
            //indeksGwiazdozbiorow[i] = 0;
            //indxItG[i]=0;
        }
    } catch (std::out_of_range& oor) {
        printf("konstruktor obliczenia, problem z dostępem do tablicy w rekordzie o numerze %d, błąd: %s", i, oor.what());
        this->~ObliczeniaGwiazd();
    }
    //    else {
    //        // this->ow = 0;
    //        char doCzego[] = {"obiektu pobieranego - odczyt wierzcholkow, Konstruktor obliczenia gwiazd"};
    //        throw xNullPointerException(doCzego);
    //    }
    //printf("\nObliczeniaGwiazd koniec konstruktora");
}

ObliczeniaGwiazd::ObliczeniaGwiazd(ObliczeniaGwiazd * gwPobierane, int ktoryGwiazdozbior, bool czyWprzestrzeni) : //trzeba wyzerować nieużywane pola składowe
ZbiorGwiazd(), ow(0), ileNazwanychGwiazd(0), kolejneSymboleZnazw(0), //kolejneHipZnazw(0),
ileGwiazdWgwiazdozbiorze(0), pozycjaKolejnegoGwiazdozbioru(0), indxItG(0), indeksGwiazdozbiorow(0), indeksPosortowanychIter(0), indeksHip(0) {//
    //std::list<Gwiazda2> lGwiazdy;//do wypełnienia

    //żeby lepiej się szukało trzeba listę wszystkich gwiazd z gwPobierane wrzucona jest do wektora w funkcji Ustalpołożenie

    //odszukujemy które gwiazdy należą do tego gwiazdozbioru:
    gwPobierane->PrzygotujGwiazdyOdejmujac(lGwiazdy, ktoryGwiazdozbior);
    //wypisanie, co zawiera wypełniona lista
    //printf("\ngw %d",ktoryGwiazdozbior);
    //WyswietlHipListyGwiazd();
    //printf("\nkoniec konstruktora ObliczeniaGwiazd rozmiar lGwiazdy %d",lGwiazdy.size());
}

//ObliczeniaGwiazd::ObliczeniaGwiazd(const ObliczeniaGwiazd& orig) {
//    
//}

ObliczeniaGwiazd::~ObliczeniaGwiazd() {
    //tu usunąć obiekty inicjowane w metodzie pobierz gwiazdozbiory i wcześniej
    //if (kolejneHipZnazw) delete[] kolejneHipZnazw;
    if (indxItG) delete[] indxItG;
    //printf("\nzwolniono indxItG");
    if (indeksHip) delete[] indeksHip;
    //printf("\nzwolniono indeksHip");
    if (indeksPosortowanychIter) delete[]indeksPosortowanychIter;
    printf("\ndestruktor Obliczenia gwiazd");
    //if(!indeksGwiazdozbiorow) delete[] indeksGwiazdozbiorow;

}

void ObliczeniaGwiazd::SprawdzStanListy(std::list<Gwiazda2>::iterator& it, std::list<Gwiazda2>::iterator& iKoniec, OdczytajGwiazdy * og) {

    //gwiazdy którego poziomu
    short ktoryPoziom = og->Nag()->nDane[4];
    //printf("\nSprawdzStan listy nowe ktory poziom %d, rozmiar iteratoryKolejnychPoziomow %d",ktoryPoziom,iteratoryKolejnychPoziomow.size());
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
    //printf("\nObliczeniaGwiazd::PobierzParametry ileNajblizszychGwiazd %d",ileNajblizszychGwiazd);
    op->OdczytajParametr(p1, ileNajblizszychGwiazd);
    //    printf("\nObliczeniaGwiazd::PobierzParametry - koniec metody");
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
        //usunięcie gwiazdy bez odległości
        //        if(plx==0){
        //            lGwiazdy.erase(it);
        //            //j++;
        //            continue;
        //        }
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
                // printf("\ngwiazda %d juz wczesniej ustawiona",hip);
                //może należy taką gwiazdę skasować?
                //if(it->Plx()<=0){
                it = lGwiazdy.erase(it);
                ileZerowaPlx++;
                //}

                //lItGzerowyHip.push_back(it);
                //zamiast usuwać - gwiazdę, jeżeli ma niezerową paralaxę można przenieść do tymczasowej listy a potem przydzielić jej inny - wolny numer hip
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
    //printf("\n UstawIndeksy stwierdzono i usunięto %d powtarzajacych sie numerow hip",ilePowtarzajacych);
    //można usunąć gwiazdy o zerowej paralaxie
    //printf("\nusunieto %d gwiazd o zerowej paralaxie, lista zawiera %d gwiazd", UsunBezParalaxy(), lGwiazdy.size());
    /******************sortowanie listy wg położenia wzdłuż osi x,y,z i przypisanie do tablicy
     *Koncepcja wyszukiwania najbliższych gwiazd
     lista lGwiazdy jest sortowana wg położenia wzdłuż osi X, w tablicy wzdluzX[] zapisywane iteratory wynikłe z sortowania, a w każdej gwieździe 
     * numer pozycji z tej tablicy, tak samo z następnymi osiami */


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
//                    printf(" h%d %s",hip1,kolejneSymboleZnazw.at(ktoraGwiazda).c_str());
                    if(gwiazdaDoUstawienia!=koniecListy)gwiazdaDoUstawienia->UstawSymbol(kolejneSymboleZnazw.at(ktoraGwiazda));
                    ktoraGwiazda++;
                }
            } catch (std::out_of_range & oor) {
                //printf("\ni=%d, problem z uzupelnieniem rekordu nr ileNazwanychGwiazd %d", i, kolejneHipZnazw[ktoraGwiazda - 1]);
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
        //this->~OdczytPliku();
        throw xBrakPliku(adresPliku);
    }
    //ilość bajtów do odczytania
    strumienOtwarty.seekg(0,std::ios::end);
    int rozmiarPliku=strumienOtwarty.tellg();
    //printf("\nrozmiar pliku %d\n",rozmiarPliku);
    //ustawienie miejsca od ktorego czytamy:
    strumienOtwarty.seekg(odKtoregoMiejsca,std::ios::beg);
    unsigned char * cdlugoscNazwy=new unsigned char;
    unsigned dlugoscNazwy=0;
    char * nazwa=new char[128];
    //w celu konwersji znaków:
    wchar_t * wNazwa=new wchar_t[64];
    
    
    while(strumienOtwarty.tellg()<rozmiarPliku){
        std::string sNazwa1;
//        printf("czytanie...");
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
        //nazwyTlumaczone.push_back(sNazwa1);
        strumienOtwarty.seekg(9,std::ios::cur);
        strumienOtwarty.read((char*)cdlugoscNazwy,1);
        dlugoscNazwy=(int)*cdlugoscNazwy;
        strumienOtwarty.read((char*)nazwa,dlugoscNazwy);
        std::string sNazwa2(nazwa,dlugoscNazwy);
       
        strumienOtwarty.seekg(10,std::ios::cur);
        //printf("\ntellg= %d",strumienOtwarty.tellg());
        
        nazwyTlumaczenie[sNazwa2]=sNazwa1;
    }
   strumienOtwarty.close();
   delete[] nazwa;
   delete[] wNazwa;
}
void ObliczeniaGwiazd::UstalPolozenie(OdczytajGwiazdy * og) {
    //    char cOw[] = {"do danej skladowej odczyt wierzcholkow, metoda Ustal Polozenie"};
    //    char cOg[] = {"do pobieranego obiektu odczytaj gwiazdy, metoda Ustal Polozenie"};
    //    if (ow == 0)throw xNullPointerException(cOw); //
    //    if (og == 0)throw xNullPointerException(cOg);
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
    //std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
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
//void ObliczeniaGwiazd::PrzydzielDoStrefPrzestrzennychZapas(std::list<Gwiazda2>& listaDoRozdzielenia) {
//    //wstępne założenia:
//    /*jest pięć wielkości stref : jedna największa zawiera gwiazdy większe niż -7 Mag, bez względu na odległość, kolejne mniejsze:
//     o wielkości 4000 tyś lat, wielkości: -7 -3;  2000 lś od -3 do 0; 1000 lś od 0 do 1,5; 500 lś mniejsze niż 1,5 */
//    //tablica dla każdej gwiazdy przyporządkowuje numer strefy
//    //int iSp[wielkoscKataloguHip];//indeks Stref przestrzennych - nie wiadomo, czy potrzebne
//    int nrKom = 0;
//    int licznik[6];
//    int dPodst = 2000; //długość boku dla komórki podziału nr 1
//    for (int i = 0; i < 6; i++)licznik[i] = 0;
//    //for(int i=0;i<wielkoscKataloguHip;i++)iSp[i]=-1;
//
//    //potrzebna jest tablica, która zawiera iteratory na wskazujące na początek danych każdej komórki(strefy przestrzennej)
//    //uzupełnienie tej tablicy
//    printf("\nPrzydzielDoStrefPrzestrzennych poczatek");
//    iteratorLG it, it1;
//    it = listaDoRozdzielenia.begin();
//    it1 = it;
//    //tylko na początku
//    if (lGwiazdyWkomorkach.empty()) {
//        //ponieważ czymś trzeba wypełnić powyższą listę, to wypełniamy ją iteratorami kolejnych gwiazd z głównej listy
//        for (int i = 0; i < 4681; i++)lGwiazdyWkomorkach.push_back(it1++);
//        iteratorLitLG itlg = lGwiazdyWkomorkach.begin();
//        //a tablicę iteratorami do powyższej listy
//        for (int i = 0; i < 4682; i++)poczatkiKomorek[i] = itlg++; //teoretycznie na ostatnim miejscu powinien być lGwiazdyWkomorkach.end()
//        printf("\nPrzydzielDoStrefPrzestrzennych wypelniono tablice poczatkiKomorek[4682]");
//    }else
//        printf("\nPrzydzielDoStrefPrzestrzennych podczas zwrotu do bazy");
//
//    //pętla oczywiście po całej liście lGwiazdy
//    while (it != listaDoRozdzielenia.end()) {
//        float M = it->JM();
//        float odl = it->Odleglosc();
//        int hip = it->Hip();
//
//
//        if (M<-9.0 || odl > dPodst) {
//            //do największej strefy
//            //iSp[hip]=0;
//            nrKom = 0;
//            licznik[0]++;
//        } else {
//            double x, y, z;
//            int ix, iy, iz, d, iK, pS; //d - długość boku komórki, ile komórek w boku o długości podstawowej lś, pS - pozycja Strefy
//            x = (double) odl * it->X();
//            y = (double) odl * it->Y();
//            z = (double) odl * it->Z();
//            if (M >= -9.0 && M<-2.0) {
//                d = dPodst;
//                pS = 1;
//                licznik[1]++;
//            }
//
//            if (M >= -2.0 && M < 0.5) {
//                d = dPodst / 2;
//                pS = 1 + 8;
//                licznik[2]++;
//            }
//            if (M >= 0.5 && M < 3) {
//                d = dPodst / 4;
//                pS = 1 + 8 + 64;
//                licznik[3]++;
//            }
//            if (M >= 3) {
//                d = dPodst / 8;
//                pS = 1 + 8 + 64 + 512;
//                licznik[4]++;
//            }
//            iK = dPodst / d;
//            ix = iK + round(x / d);
//            iy = iK + round(y / d);
//            iz = iK + round(z / d);
//
//            nrKom = (pS + iz * iK * 2 * iK * 2 + iy * iK * 2 + ix); //numer komórki do której należy dodać daną gwiazdę
//
//
//        }
//        //sięgamy do odpowieniej tabeli,ażeby znaleźć miejsce gdzie wstawić 
//        iteratorLitLG gdzieWstawic = poczatkiKomorek[nrKom + 1]; //bo wstawiamy przed początkiem następnej komórki
//        //w aktualnej Gwieździe umieszczamy iterator listy iteratorow, zwrócony przez funkcję insert, aby można go było usunąć zlisty właśnie z poziomu gwiazdy
//        it->UstawItWliscieKomorek(lGwiazdyWkomorkach.insert(gdzieWstawic, it));
//        it++;
//        //           licznik[6]++;
//        //           if(licznik[6]==200000){
//        //               printf("\nsprawdzono 200 000 pozycji i przerwano");
//        //               it=lGwiazdy.end();
//        //           }
//    }
//    //    printf("\nPrzydzielDoStrefPrzestrzennych wypelniono lGwiazdyWkomorkach srednio w komorkach jest: ");
//    //    for(int i=0;i<5;i++)printf("%d %4.2f; ",i,(float)licznik[i]/pow(8,i));
//    //sprawdzenie
////    for (int i = 0; i < 4681; i++) {
////        iteratorLitLG itL, kon;
////        itL = poczatkiKomorek[i];
////        itL++;
////        kon = poczatkiKomorek[i + 1];
////        int licznik = 0;
////        while (itL != kon) {
////            itL++;
////            licznik++;
////        }
////        //if(licznik>20)printf("\n %d %d",i,licznik);
////    }
//}

//

int ObliczeniaGwiazd::DodajKolejnePoziomyJakoGwiazdozbiory() {

    int i = 0;
    int miejsceDoWstawienia = 0;
    std::list<Gwiazda2>::iterator it, kon;
    printf("\nDodajKolejnePoziomyJakoGwiazdozbiory iteratoryKolejnychPoziomow.size() %d",iteratoryKolejnychPoziomow.size());
    
    for (i; i < iteratoryKolejnychPoziomow.size(); i++) {
        int j = 0;
        int bledy = 0;
        printf("\npoziom %d", i);
        it = iteratoryKolejnychPoziomow.at(i);
        kon = (i + 1 < iteratoryKolejnychPoziomow.size()) ? iteratoryKolejnychPoziomow.at(i + 1) : lGwiazdy.end();
        //printf("\ngwiazda która rozpoczyna poziom %d, hip %d", i, it->Hip());
        //kopiowanie numerow do listy, należy znaleźć miejsce od którego można uzupełniać tablicę kolejneHipZnazw
        //pola ileGwiazdWgwiazdozbiorze oraz pozycjaKolejnegoGwiazdozbioru dadzą informację do którego miejsca tablica jest zajęta
        int pKG = pozycjaKolejnegoGwiazdozbioru.back();
        int iGG = ileGwiazdWgwiazdozbiorze.back();
        miejsceDoWstawienia = pKG + iGG;
        //        printf("\nDodajKolejnePoziomyJakoGwiazdozbiory pKG %d, iGG %d, miejsceDo wstawienia %d",pKG,iGG,miejsceDoWstawienia);
        pozycjaKolejnegoGwiazdozbioru.push_back(miejsceDoWstawienia);
        try {
            while (it != kon) {

                int hip = it->Hip();

                if (hip > 0 && hip < 200000)kolejneHipZnazw.at(miejsceDoWstawienia + j++) = hip;
                it++;
            }
        } catch (std::exception& e) {
            printf("\nDodajKolejnePoziomyJakoGwiazdozbiory blad hip = it->Hip() %s,miejsce %d", e.what(), miejsceDoWstawienia + j);
            //                system("pause");
            bledy++;
        }



        ileGwiazdWgwiazdozbiorze.push_back(j);
        printf(" dodano %d gwiazd, bledy %d", j, bledy);
    }
    //    printf("\nPo dodaniu wszystkich wszystkich gwiazd jako gwiazdozbiorów pozycjaKolejnegoGwiazdozbioru.size %d, ileGwiazdWgwiazdozbiorze.size %d"
    //            ,pozycjaKolejnegoGwiazdozbioru.size(),ileGwiazdWgwiazdozbiorze.size());
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
                //double wielkosc = 9 - ((double) it->Mag() / 32);
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
        //i - ile najjaśniejszych gwiazd z każdej strefy
    }

    return lista.size();
}

int ObliczeniaGwiazd::ListaWspolrzednychZOdleglosciami(std::deque<double> & lista, listaItLG & listaIteratorow) {
    if (!lGwiazdy.empty()) {
        std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
        int licznik = 0;
        while (it != lGwiazdy.end()) {// && licznik<100
            double odl = (double) it->Odleglosc() / 100;
            double k, w, x, y, z;
            //k = (double)it->B_v()/128.0;
            //printf("\n k: %d",it->B_v());
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
            //licznik++;
            //printf("\nwielkosc %2.4f, x %2.4f, y %2.4f,z %2.4f",w,x,y,z);
        }
    }
    return lista.size();
}

int ObliczeniaGwiazd::ListaWspolrzednychParPunktow(std::deque<double> & lista) {

    if (!lGwiazdy.empty()) {
        std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
        while (it != lGwiazdy.end()) {// && licznik<100
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
            //licznik++;
            //printf("\nwielkosc %2.4f, x %2.4f, y %2.4f,z %2.4f",w,x,y,z);
        }
    }
    return lista.size();
}

std::list<int>::iterator ObliczeniaGwiazd::ListaHip(std::list<int>& listaDoWypelnienia, std::list<int>::iterator przedKtorymWstawic) {
    //odczytuje hipy z całej swojej listy i wstawia je do listyDoWypelnienia zwracając iterator na poczatek wstawionych danych
    //std::list tempListaHip;
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
    //printf("\nLista Hip iterator ktory ma wskazywac na poczatek wstawionej listy itr=%d",*itr);
    it++;

    //pętlę zaczynamy od drugiego elementu
    while (it != lGwiazdy.end()) {
        listaDoWypelnienia.insert(przedKtorymWstawic, it->Hip());
        it++;
        //ile++;
    }
    //printf("\nListaHip ile %d lGwiazdy.size %d",ile,lGwiazdy.size());
    //ponieważ funkcja insert zwraca iterator do wstawionego elementu, spróbujemy przekazać go dalej, by 
    //listaHip zwracała iterator do poczatku swoich danych
    return itr;
}

int ObliczeniaGwiazd::PobierzGwiazdozbiory(OdczytNazw * on,OdczytNazw * nazwyGwiazdozbiorow) {
    char t[] = {"do obiektu odczyt nazw"};
    if (!on)throw xNullPointerException(t);
    //printf("\npobierz gwiazdozbiory poczatek");
    ileNazwanychGwiazd = on->IleOdczytanychLinii();
    //trzeba zrobić miejsce dodatkowe na wszystkie gwiazdy, tak aby każdy poziom mógł być wyswietlony jako osobny gwiazdozbiór
    //    kolejneHipZnazw = new int[ileNazwanychGwiazd + lGwiazdy.size() + ileNajblizszychGwiazd];
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
        //printf("\nnazwa skrocona %s",aktualnaNazwaSkrocona.c_str());
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
                //printf("\n%s lp %d  lw %d %s",poprzedniaNazwaSkrocona.c_str(),liczbaPorzadkowa,liczbaWystapien,itDaneGwiazdozbiorow->NazwaSkrocona().c_str());//,itDaneGwiazdozbiorow->NazwaSkrocona().c_str()
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
//int ObliczeniaGwiazd::KtorePunktyWidacZapas(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip) {
//
//    //należy odszukać, w której komórce każdego poziomu znajduje się punkt p
//    int nrKom[5]; //numery komórek z kolejnych poziomów dokładności
//    int d; //długość boków komórki kolejnych poziomów dokładności w jednostkach lata świetlne
//    int pS = 0; //numer od którego zaczynają się komórki danego poziomu
//    int ix, iy, iz; //która komórka w osi x(y,z) licząc od najdalej wysuniętej w stronę ujemnych do dodatnich
//    int iK; // ile jest komórek danego poziomu w boku o długości podstawowej dPodst
//    int dPodst = 2000; //długość boku dla komórki podziału nr 1
//    nrKom[0] = 0;
//    for (int i = 0; i < 4; i++) {
//        d = dPodst / pow(2, i); //16000 podzielić przez, kolejno 1,2,4,8, czyli 16000 8000 4000 2000
//        iK = dPodst / d; //kolejno 1 2 4 8 
//        pS += pow(8, i); //czyli kolejno 1+8+64+512
//        ix = iK + round(100 * p[0] / d);
//        //printf("\n(100*p[0]/d) %2.5f, round(100*p[0]/d) %2.5f",(100*p[0]/d),round(100*p[0]/d));
//        iy = iK + round(100 * p[1] / d);
//
//        iz = iK + round(100 * p[2] / d);
//
//        nrKom[i + 1] = (pS + iz * iK * iK * 4 + iy * iK * 2 + ix);
//    }
//    //przed uzupełnieniem listy, trzeba ją wyzerować
//    //a może nie trzeba? bo wcześniej jest zerowana
//    //punktyWszystkichGwiazd.clear();
//    //znając numery komórek możemy dowiedzieć się, które gwiazdy do nich należą
//    //lGwiazdyWkomorkach;//zawierać będzie iteratory do wyszystkich gwiazd ułożonych kolejno komórkami
//    for (int i = 1; i < 5; i++) {//jeśli od 1 to pomijamy strefę największych gwiazd
//        //należy ustalić zakres z którego odczytujemy elementy, czyli początek i koniec
//        //początkiem jest nrKom[i], zwiekszony o jeden, a końcem jest iterator z wskazujący na początek następnej komórki
//        iteratorLitLG itL, kon;
//        itL = poczatkiKomorek[nrKom[i]]; //tutaj przechodzimy do adresu następnej komórki
//        itL++;
//        kon = poczatkiKomorek[nrKom[i] + 1];
//
//        //printf("\npoziom %d, nrKom[i] %d",i,nrKom[i]);
//        iteratorLG itG;
//        int licznik = 0;
//        while (itL != kon) {
//            itG = *itL;
//            double odl = (double) itG->Odleglosc() / 100;
//            double w, x, y, z;
//            w = itG->AbsolutnaWlkDoWyswietlenia();
//            x = odl * itG->X();
//            y = odl * itG->Y();
//            z = odl * itG->Z();
//            punktyWszystkichGwiazd.push_back(w);
//            punktyWszystkichGwiazd.push_back(x);
//            punktyWszystkichGwiazd.push_back(y);
//            punktyWszystkichGwiazd.push_back(z);
//            listaHip.push_back(itG->Hip());
//            licznik++;
//            //printf(" %d,",itG->Hip());
//            itL++;
//        }
//        //printf("ile w komorce: %d,",licznik);
//
//    }
//}

void ObliczeniaGwiazd::PrzygotujGwiazdyOdejmujac(std::list<Gwiazda2>& listaDoWypelnienia, int ktoryGwiazdozbior) {
    //trzeba zrobić zabezpieczenie przed próbą dostępu, jeśli nie ma gwiazd
    if (lGwiazdy.empty() && !listaDoWypelnienia.empty()) {
        printf("\nprzerwanie metody PrzygotujGwiazdyOdejmujac z powodu zerowego rozmiaru listy zrodlowej");
        return;
    }
    //    printf("\nprzygotuj gwiazdy odejmujac, rozmiar listy bazowej %d,rozmiar listy uzupelnianej %d",lGwiazdy.size(),listaDoWypelnienia.size());
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
    //    printf("\nPrzygotujGwiazdyOdejmujac przed odjeciem podzial->RozmiarListy() %d",);
    for (int i = pocz; i < kon; i++) {
        try {
            //jeżeli dana gwiazda już jest przeniesiona do innej listy, to metoda nie wie nic o tym, wycina z tamtej innej listy
            //brak pomysłu na zabezpieczenie przed tym
            hip = kolejneHipZnazw.at(i);
            itG = indxItG[hip];
            //printf("\ni:%d,hip: %d",i,hip);
            if (itG == lGwiazdy.end())continue;
            listaDoWypelnienia.splice(listaDoWypelnienia.end(), lGwiazdy, itG);
            //gwiazda po przejściu do nowej listy, powinna się "wyrejestrować" z listy lGwiazdyWkomorkach
            //nie mam pewności, czy lGwiazdyWkomorkach należy do właściwego obiektu i, czy w ogóle, coś zawiera:
            /***wersja przed wprowadzeniem obiektu PodzialNaKomorki*********/
            //            if (lGwiazdyWkomorkach.empty() && itG->ItWliscieKomorek() == lGwiazdyWkomorkach.end())bladUsuniecia++;
            //            else {
            //                lGwiazdyWkomorkach.erase(itG->ItWliscieKomorek());
            //                itG->UstawItWliscieKomorek(lGwiazdyWkomorkach.end()); //wskazuje na koniec dla bezpieczeństwa, żeby już nie usuwać, jeśli przypadkiem trafi się już raz usunięta
            //            }
            bladUsuniecia += podzial->UsunIterator(itG);
            //printf("\nPrzygotujGwiazdyOdejmujac przekazano gwiazde o numerze %d",indxItG[kolejneHipZnazw[i]]->Hip());
        } catch (std::exception& e) {
            printf("\n blad iteratora %s", e.what());
        }
        //printf("\n %d Mag %2.3f",i,Mag);
    }
    //printf("\nPrzygotujGwiazdyOdejmujac podzial->RozmiarListy() przed odjeciem %d po odjeciu  %d", rozmiarWczesniejszy, podzial->RozmiarListy());
    if (bladUsuniecia > 0)printf("\nPrzygotujGwiazdyOdejmujac lGwiazdyWkomorkach jest pusta i nie mozna z niej usuwac, lub proba ponownego usuniecia ile razy %d", bladUsuniecia);
    //printf("\nuzupełniono listę, obecnie rozmiar listy bazowej %d,rozmiar listy uzupelnianej %d", lGwiazdy.size(), listaDoWypelnienia.size());
}

void ObliczeniaGwiazd::PrzekazGwiazdyDoBazy(ObliczeniaGwiazd * oBaza) {
    //zakładamy, że przekazujemy całość, po tym własna lista powinna być pusta
    //printf("poczatek metody ObliczeniaGwiazd::PrzekazGwiazdyDoBazy");
    int rozmiarPierwotny = lGwiazdy.size();
    //należy wszystkie gwiazdy "wyrejestrowane" zlisty lGwiazdyWkomorkach powtórnie do niej dodać
    oBaza->PrzydzielDoStrefPrzestrzennych(lGwiazdy);
    //printf("\nObliczeniaGwiazd::PrzekazGwiazdyDoBazy przed splice");
    oBaza->LGwiazdy().splice(oBaza->LGwiazdy().begin(), lGwiazdy);
//    printf("\n przekazano %d gwiazd do bazy, pozostala lista ma rozmiar %d", rozmiarPierwotny, lGwiazdy.size());
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
    //    return 0;
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
    //    it=lGwiazdy.begin();
    //    kon=lGwiazdy.end();
    itCentralnej = indxItG[nrHip];
    float cx, cy, cz;
    cx = itCentralnej->OX();
    cy = itCentralnej->OY();
    cz = itCentralnej->OZ();

    //rozmiarLGwiazdy=lGwiazdy.size();
    //potrzebujemy mieć dostęp do wszystkich gwiazd, również tych, które przeniesiono z lGwiazdy do list gwiazdozbiorów
    float odleglosci[wielkoscKataloguHip];
    int indeksyPosortowane[wielkoscKataloguHip];

    //iteratorLG iteratoryListyGwiazd[rozmiarLGwiazdy];
    int i = 0;
    while (i < rozmiarLGwiazdy) {
        //if(indxItG[i]==lGwiazdy.end())
        odleglosci[i] = indxItG[i]->KwadratDystansuDo(cx, cy, cz);

        indeksyPosortowane[i] = i;
        //iteratoryListyGwiazd[i]=it;
        i++;
    }
    int brzeg = 0;
    //    printf("\nprzed sortowaniem, i= %d,rozmiarLGwiazdy %d,",i,rozmiarLGwiazdy);
    while (brzeg < ileNajblizszychGwiazd)SortujIndeksy(odleglosci, indeksyPosortowane, brzeg++, rozmiarLGwiazdy);

    //listę tych gwiazd należy dodać jako kolejny gwiazozbiór
    //w tym celu należy odszukać miejsce na liście, gdzie można umieszczać numery najbliższych gwiazd
    //sprawdzenie, co jest w kontenerach:pozycjaKolejnegoGwiazdozbioru, ileGwiazdWgwiazdozbiorze
    
    int miejsceDoWstawienia = 0;
    int pKG = pozycjaKolejnegoGwiazdozbioru.back();
    int iGG = ileGwiazdWgwiazdozbiorze.back();
//    static bool czyDodawac = true; //
    //zmienna statyczna powoduje problemy przy wielowątkowości
        bool czyDodawac=(ileGwiazdWgwiazdozbiorze.size()<197);//czy już poprzednio ustawiane były najbliższe gwiazdy
    //printf("\nczy dodawac %d",czyDodawac);
    miejsceDoWstawienia = pKG + iGG;
    if (czyDodawac) pozycjaKolejnegoGwiazdozbioru.push_back(miejsceDoWstawienia);
    else pozycjaKolejnegoGwiazdozbioru.at(196) = miejsceDoWstawienia;
    i = 0;
    for (i; i < ileNajblizszychGwiazd; i++) {
        try {
            kolejneHipZnazw.at(miejsceDoWstawienia + i) = indeksyPosortowane[i];
            //printf("\nmiejsce wstawienia %d, rozmiar %d",miejsceDoWstawienia+i,kolejneHipZnazw.size());
            //printf("\nodl %2.3f, hip %d",odleglosci[indeksyPosortowane[i]],indeksyPosortowane[i]);
        } catch (std::exception & e) {
            printf("\nNajblizszeGwiazdy problem z dostepem do kolejneHipZnawz i:%d blad %s", i, e.what());
        }

        //printf("\n%d nr indeksu %d, odleglosc %2.3f ",i,indeks,odleglosci[indeks]);
        //printf("hip: %d",indxItG[indeks]->Hip());
    }

    if (czyDodawac)ileGwiazdWgwiazdozbiorze.push_back(i);
    else ileGwiazdWgwiazdozbiorze.at(196) = i;
    czyDodawac = false;
    koniec = clock();
    long delta = (long) (koniec - start);
    printf("\nwyznaczenie najblizszych %d gwiazd zajelo %d ms", ileNajblizszychGwiazd, delta);
    return true;
    //printf("\nliczba dostepnych gwiazdozbiorow: %d",ileGwiazdWgwiazdozbiorze.size());
}
void __cdecl WatekObliczenGwiazd(void * p){
    ParametryWatkuObliczenGwiazd * param;
    ObliczeniaGwiazd * paramBaza;
    WFbi paramFunkcja;
//  
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
    //printf("\nkoniec watku");
    _endthread();
}
//void ObliczeniaGwiazd::NajblizszeGwiazdyZapas(int nrHip){
//    printf("\nObliczeniaGwiazd::najblizsze gwiazdy - poczatek");
//    iteratorLG itCentralnej;
//    itCentralnej=indxItG[nrHip];
//    //należy odszukać kolejne szóstki gwiazd najbliżej leżących na osiach na lewo i prawo
//    int xL,xP,yL,yP,zL,zP;
//    xL=itCentralnej->KtoryNrPorzadkowyX();
//    xP=xL;
//    yL=itCentralnej->KtoryNrPorzadkowyY();
//    yP=yL;
//    zL=itCentralnej->KtoryNrPorzadkowyZ();
//    zP=zL;
//    printf("\nktory porzadkowy x: %d, y: %d, z: %d",xP,yP,zP);
//    //pętla dla kolejnych szóstek
//        std::vector<Gwiazda2> vNajblizszeGwiazdy;
//        vNajblizszeGwiazdy.reserve(6*ileNajblizszychGwiazd);
//        std::vector<Gwiazda2>::iterator it,kon;
//        it=vNajblizszeGwiazdy.begin();
//    for (int i=0;i<ileNajblizszychGwiazd;i++){
//        //trzeba zapewnić ażeby dla skarajnie położonych nie było próby odczytu za końcem tablicy
//        
//        if( ++xP<rozmiarLGwiazdy)vNajblizszeGwiazdy.push_back(*indeksPosortowanychIter[xP*3]);
//        if( ++yP<rozmiarLGwiazdy)vNajblizszeGwiazdy.push_back(*indeksPosortowanychIter[1+yP*3]);
//        if( ++zP<rozmiarLGwiazdy)vNajblizszeGwiazdy.push_back(*indeksPosortowanychIter[2+zP*3]);
//        if( --xL>0)vNajblizszeGwiazdy.push_back(*indeksPosortowanychIter[xL*3]);
//        if( --yL>0)vNajblizszeGwiazdy.push_back(*indeksPosortowanychIter[1+yL*3]);
//        if( --zL>0)vNajblizszeGwiazdy.push_back(*indeksPosortowanychIter[2+zL*3]);
//    }
//    //należy teraz określić dla każdej gwiazdy z listy odległości do centralnej gwiazdy i posortować listę względem tej odległości
//        
//        it=vNajblizszeGwiazdy.begin();
//        kon=vNajblizszeGwiazdy.end();
//        
//        int ileWyszukano=vNajblizszeGwiazdy.size();
//        float odleglosci[ileWyszukano];
//        int indeksyPosortowane[ileWyszukano];//na początku są ponumerowane od 0 do ileWyszukano-1
//        printf("\nvNajblizszeGwiazdy zawiera %d elementow",ileWyszukano);
//        int i=0;
//        try{
//            
//            while(it!=kon){
//                odleglosci[i]=it->DystansDo(itCentralnej->OX(),itCentralnej->OY(),itCentralnej->OZ());
//                indeksyPosortowane[i]=i;
//                //printf("\n %d odleglosc: %2.3f, hip: %d",i,odleglosci[i],it->Hip());
//                i++;it++;
//            }
//        }catch(std::exception& e ){
//            printf("\n ObliczeniaGwiazd::NajblizszeGwiazdy problem przy obliczniu odleglosci[] %s",e.what());
//        }
//        //sortowanie przez wybieranie
//        //it=vNajblizszeGwiazdy.begin();
//        
//        int brzeg=0;
//        while(brzeg<ileWyszukano)SortujIndeksy(odleglosci,indeksyPosortowane,brzeg++,ileWyszukano);
//        //printf("\npodczas sortowania wykonano %d zamian",zamianyPrzySortowaniu);
////        while(i<ileWyszukano){
////            
////            indeksyPosortowane[i]=Minimum(odleglosci,indeksyPosortowane,i,ileWyszukano);
////            
////            i++;
////        }
//        try{
//            for(i=0;i<10;i++){
//                int indeks=indeksyPosortowane[i];
//                float odl=odleglosci[indeks];
//                printf("\n%d nr indeksu %d, odleglosc %2.3f",i,indeks,odl);
//                printf("hip: %d",vNajblizszeGwiazdy.at(indeks).Hip());
//            }
//        }catch(std::exception& e){
//            printf("\nObliczeniaGwiazd::NajblizszeGwiazdy problem przy sortowaniu %s",e.what());
//        }
//        
//        
//        
//}
//void ObliczeniaGwiazd::UstalPolozenieZapas(OdczytajGwiazdy * og) {
//    char cOw[] = {"do danej skladowej odczyt wierzcholkow, metoda Ustal Polozenie"};
//    char cOg[] = {"do pobieranego obiektu odczytaj gwiazdy, metoda Ustal Polozenie"};
//    if (ow == 0)throw xNullPointerException(cOw); //
//    if (og == 0)throw xNullPointerException(cOg);
///* żeby rozpoznawało poziom i związaną z tym ilość stref a metodę Gwiazda2::UstawWspolrzedne nadpisać, żeby wiedziała w jak dużym trójkącie się znajduje*/
//
//    double wsp[3]; //współrzędne wektora środka strefy
//     std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
//    for (int i = 0; i < og->IleStref(); i++) {//og.IleStref()
//        //inny sposób jest taki, żeby obliczyć kąt i oś obrotu od wektora do punktu 1,0,0
//        int licznik = 0;
//        double wspBieg[2];
//        if (i < 20) {//zabezpieczenie przed czytaniem za końcem tablicy
//            //ustalenie współrzędnych
//            wsp[0] = ow->SrodkiScian(3 * i + 0);
//            wsp[1] = ow->SrodkiScian(3 * i + 1);
//            wsp[2] = ow->SrodkiScian(3 * i + 2);
//            //obliczenie kąta i osi obrotu do wektora 1,0,0
//            //KatIWektorObrotuMiedzyWektoramid(katIosObrotu, wekt100, wsp);
//
//            xyz_to_LF(wspBieg, wsp);
//            for (int j = 0; j < og->IleGwiazdWstrefach()[i]; j++) {//for (int j = 0; j < og.IleGwiazdWstrefach()[i]; j++) {
//                licznik++;
//                it->UstawWspolrzedne(wspBieg,1.0);//
//                //it->PokazWspolrzedneXYZ();
//                it++;
//            }
//        } else {
//            printf("\nnie ma dalszych wierzchołków");
//        }
//        //printf(" do obliczenia %d gwiazd, współrzędne środka strefy %0.3f, %0.3f", licznik, Rad_To_Deg(wspBieg[0]), Rad_To_Deg(wspBieg[1]));
//    }
//    printf("\nustalono polozenie gwiazd");
//   
//}
//void ObliczeniaGwiazd::ObliczGwiazdyZapas(OdczytajGwiazdy * og) {
//    char cOg[] = {"do pobieranego obiektu odczytaj gwiazdy, Metoda oblicz gwiazdy"};
//
//    if (og == 0)throw xNullPointerException(cOg);
//    //obliczyć długość kroku do magnitudo
//    float krok = (float) og->Nag()->nDane[6] / og->Nag()->nDane[7];
//    float plx, odl, m, M;
//    Gwiazda2 g0;
//    printf("\nobliczenia krok: %2.2f, ile gwiazd: %d", krok, og->IleGwiazd());
//    lGwiazdy.assign(og->IleGwiazd(), g0);
//    //tablitablicaIteratorow= new std::list<Gwiazda2>::iterator[og->IleGwiazd()];
//    ileStref = og->IleStref();
//    ileGwiazdWstrefach = new int[og->IleStref()];
//    for (int i = 0; i < og->IleStref(); i++) {
//        ileGwiazdWstrefach[i] = og->IleGwiazdWstrefach()[i];
//    }
//    int j = 0;
//    int hip=0;
//    std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
//    //tu trzeba uzupełnić tablicę iteratorów, żeby nie wskazywały pustych miejsc
//    //czyli te nie wypełnione wskazują na gwiazdę Fomalhaut
//    for(int i=0;i<wielkoscKataloguHip;i++) indxItG[i]=it;
//    for ( it; it != lGwiazdy.end(); it++) {
//        //obliczenia odległości, wielkości pozornej i absolutnej
//        plx = (og->pGwiazda() + j)->Plx();
//        m = (og->Nag()->nDane[5] + krok * (og->pGwiazda() + j)->Mag()) / 1000;
//        (plx != 0) ? M = m + 5 * (1 + log10(plx / 100000)) : M = m;//w niektórych brakuje odległości/paralaxy
//        (plx != 0) ? odl = (326212 / plx) : odl = 0; //odległość
//        try {
//            it->Ustaw((og->pGwiazda() + j), m, M, odl);// w tym miejscu ładują się wszystkie parametry
//        } catch (...) {
//            printf("\nnie ustawiono obliczonych parametrow dla gwiazdy");
//            break;
//        }
//        //umieszczenie w indeksieiteratorów
//        j++;
//    }
//    
//    printf("\nobliczono gwiazdy umieszczjąc iteratory w tablicy");
//}
//void ObliczeniaGwiazd::UstalPolozenieZapas(OdczytajGwiazdy & og) {
//    double wsp[3]; //współrzędne wektora środka strefy
//    double wekt100[] = {1.0, 0.0, 0.0};
//    double katIosObrotu[4];
//    std::list<Gwiazda2>::iterator it = lGwiazdy.begin();
//    for (int i = 0; i < 4; i++) {//og.IleStref()
//        //inny sposób jest taki, żeby obliczyć kąt i oś obrotu od wektora do punktu 1,0,0
//        int licznik = 0;
//        //double wspBieg[2];
//        if (i < 20) {//zabezpieczenie przed czytaniem za końcem tablicy
//            //ustalenie współrzędnych
//            wsp[0] = ow->WierzcholkiZpliku(3 * i + 0);
//            wsp[1] = ow->WierzcholkiZpliku(3 * i + 1);
//            wsp[2] = ow->WierzcholkiZpliku(3 * i + 2);
//            //obliczenie kąta i osi obrotu do wektora 1,0,0
//            KatIWektorObrotuMiedzyWektoramid(katIosObrotu, wekt100, wsp);
//
//            // xyz_to_LF(wspBieg, wsp);
//            for (int j = 0; j < og.IleGwiazdWstrefach()[i]; j++) {//for (int j = 0; j < og.IleGwiazdWstrefach()[i]; j++) {
//                licznik++;
//                /*             *************          do poprawy algorytm ustawiania współrzędnych      */
//                it->UstawWspolrzedne(katIosObrotu); //
//                //it->PokazWspolrzedneXYZ();
//                it++;
//            }
//        } else {
//            printf("\nnie ma dalszych wierzchołków");
//        }
//        //printf(" do obliczenia %d gwiazd, współrzędne środka strefy %0.3f, %0.3f", licznik, Rad_To_Deg(wspBieg[0]), Rad_To_Deg(wspBieg[1]));
//    }
//    //ow->WyswietlWartosci();
//
//}
//std::vector<Gwiazda2>& ObliczeniaGwiazd::VGwiazda() {
//    std::vector<Gwiazda2> & rV = vGwiazdy;
//    return rV;
//}
//void ObliczeniaGwiazd::PrzygotujGwiazdy(std::list<Gwiazda2>& listaDoWypelnienia, int ktoryGwiazdozbior, bool czyWprzestrzeni) {
//    //które gwiazdy
//    printf("\nprzygotuj gwiazdy");
//    int pocz = pozycjaKolejnegoGwiazdozbioru[ktoryGwiazdozbior];
//    int kon = pocz + ileGwiazdWgwiazdozbiorze[ktoryGwiazdozbior];
//    int k = 0;
////    for (int i = pocz; i < kon; i++) {
////        //printf("\n %d hip %d", k++, kolejneHipZnazw[i]);
////        //dostęp wg indeksu
////        listaDoWypelnienia.push_back(vGwiazdy[indeksHip[kolejneHipZnazw[i]]]);
////    }
//    printf("\nuzupełniono listę, obecnie rozmiar wynosi %d",listaDoWypelnienia.size());
//}
//void ObliczeniaGwiazd::ObliczGwiazdyBezIteratorow(OdczytajGwiazdy * og) {
//    char cOg[] = {"do pobieranego obiektu odczytaj gwiazdy, Metoda oblicz gwiazdy"};
//
//    if (og == 0)throw xNullPointerException(cOg);
//    //obliczyć długość kroku do magnitudo
//    float krok = (float) og->Nag()->nDane[6] / og->Nag()->nDane[7];
//    float plx, odl, m, M;
//    Gwiazda2 g0;
//    printf("\nobliczenia krok: %2.2f, ile gwiazd: %d", krok, og->IleGwiazd());
//    lGwiazdy.assign(og->IleGwiazd(), g0);
//
//    ileStref = og->IleStref();
//    ileGwiazdWstrefach = new int[og->IleStref()];
//    for (int i = 0; i < og->IleStref(); i++) {
//        ileGwiazdWstrefach[i] = og->IleGwiazdWstrefach()[i];
//    }
//    int j = 0;
//    /*nie kopiujemy w tym miejscu wszystkich parametrów z og(odczytane gwiazdy)
//     *może trzeba będzie w przyszłości uzupełnić
//     */
//    for (std::list<Gwiazda2>::iterator it = lGwiazdy.begin(); it != lGwiazdy.end(); it++) {
//        //obliczenia odległości, wielkości pozornej i absolutnej
//        plx = (og->pGwiazda() + j)->Plx();
//        m = (og->Nag()->nDane[5] + krok * (og->pGwiazda() + j)->Mag()) / 1000;
//        (plx != 0) ? M = m + 5 * (1 + log10(plx / 100000)) : M = m;
//        (plx != 0) ? odl = (326212 / plx) : odl = 0; //odległość
//        try {
//
//            it->Ustaw((og->pGwiazda() + j), m, M, odl);
//        } catch (...) {
//            printf("\nnie ustawiono obliczonych parametrow dla gwiazdy");
//            break;
//        }
//        //umieszczenie w indeksie
////        try {
////            indeksHip[(og->pGwiazda() + j)->Hip()] = j++;
////        } catch (std::out_of_range& oor) {
////            printf("Metoda oblicz gwiazdy, problem z dostępem do tablicy indeksow, obiekt listy nr %d, błąd: %s", j, oor.what());
////        }
//    }
//}
//    for (std::list<Gwiazda2>::iterator it = lGwiazdy.begin(); it != lGwiazdy.end(); it++) {
//        
//        printf("\n  %d. hip %d %2.2fm %2.2fM", j++, it->Hip(), it->Jm(), it->JM()); //numer gwiazdy
//    }