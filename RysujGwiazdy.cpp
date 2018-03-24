/* 
 * File:   RysujGwiazdy.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 10 kwietnia 2016, 13:15
 */

#include "RysujGwiazdy.h"

#include "PodzialNaKomorki.h" 
#include "OdczytParametrow.h"


RysujGwiazdy * RysujGwiazdy::wskaznikRysujGwiazdy = 0;

RysujGwiazdy::RysujGwiazdy() : punkty(0), oBaza(0), podzial(0), ok(0) {
}

RysujGwiazdy::RysujGwiazdy(const RysujGwiazdy& orig) {

}

RysujGwiazdy::~RysujGwiazdy() {
    if (oBaza)delete oBaza;
    if (punkty)delete punkty;
    if (podzial)delete podzial;
    if (ok)delete ok;
    printf("\ndestruktor RysujGwiazdy, zwolniono pamiec po wskaznikach");
    wskaznikRysujGwiazdy = 0;

}

void RysujGwiazdy::Inicjuj() {
    std::string adres[10];
    adres[0] = "stars_0_0v0_5.cat";
    adres[1] = "stars_1_0v0_5.cat";
    adres[2] = "stars_2_0v0_5.cat";
    adres[3] = "name.fab";
    adres[4] = "srodkiScianWierzcholkiNumeryWierzcholkow.bin";
    adres[5] = "parametry.txt";
    adres[6] = "barwyGwiazd.bmp";
    adres[7] = "star_names.fab";
    adres[8] = "constellation_names.eng.fab";
    adres[9] = "pl.qm";



    OdczytajGwiazdy * og[3]; //moze ilość do ustalenia?
    OdczytWierzcholkow * ow;
    OdczytNazw * on;
    OdczytNazw * nazwyGwiazd=0;
    OdczytNazw * nazwyGwiazdozbiorow=0;
    OdczytParametrow * op;
    std::string komunikat;


    try {
        for (int i = 0; i < 3; i++)og[i] = new OdczytajGwiazdy(adres[i].c_str());
    } catch (std::exception& e) {
        printf("\nproblem z odczytem gwiazd %s", e.what());
    }
    try {
        ow = new OdczytWierzcholkow(adres[4].c_str());
    } catch (std::exception& e) {
        printf("\nproblem z odczytem wierzchołków %s", e.what());
        
        ow = 0;
    }
    try {
        op = new OdczytParametrow(adres[5].c_str());
    } catch (std::exception& e) {
        printf("\nnie odczytano parametrow %s", e.what());
    }
    podzial = new PodzialNaKomorki();
    podzial->PobierzParametry(op);
    try {
        oBaza = new ObliczeniaGwiazd(ow);
    } catch (std::exception & e) {
        printf("\nproblem z pobraniem wierzcholkow do obliczen %s", e.what());
        oBaza = 0;
    }
    try {
            oBaza->PobierzParametry(op);
        for (int i = 0; i < 3; i++) {
            oBaza->ObliczGwiazdy(og[i]);
            oBaza->UstalPolozenie(og[i]);
        }
    } catch (std::exception& e) {
        printf("\nProblem z Obliczaniem Gwiazd %s", e.what());
    }
    try {
        on = new OdczytNazw(adres[3].c_str());
        nazwyGwiazd = new OdczytNazw(adres[7].c_str());
        nazwyGwiazdozbiorow = new OdczytNazw(adres[8].c_str());
        oBaza->PobierzTlumaczenie(adres[9].c_str(),11326);
        oBaza->PobierzGwiazdozbiory(on,nazwyGwiazdozbiorow);//wypełnia również nazwami i liczbami porządkowymi kolejkę gwiazdozbiory do przycisku,
        
    } catch (std::exception& e) {
        printf("\nnie można pobrać gwiazdozbiorów %s", e.what());
    }
    
    try {
        komunikat="ustaw podzial";
        oBaza->UstawPodzial(podzial);
        komunikat="ustaw indeksy";
        oBaza->UstawIndeksy();
        komunikat="pobierz nazwy gwiazd";
        oBaza->PobierzNazwyGwiazd(nazwyGwiazd);
        komunikat="dodaj kolejne poziomy jako gwiazdozbiory";
        //oBaza->DodajKolejnePoziomyJakoGwiazdozbiory();
    } catch (std::exception& e) {
        printf("\nRysuj Gwiazdy Inicjuj problem z %s %s",komunikat.c_str(), e.what());
    }

    wskaznikRysujGwiazdy = this;
    punkty = new PrzygotowaniePunktow;
    try {
        ok = new OdczytKolorowBMP(adres[6].c_str());
    } catch (std::exception & e) {
        printf("\nnie odczytano kolorow %s", e.what());
    }
    ok->PrzeczytajNaglowki();
    punkty->UstawTabliceKolorow(ok->TablicaKolorowChar(), ok->DlugoscTablicy());
    //klasa jest zaprzyjaźniona, więc nie ma potrzeby tworzyć i uzywać akcesora
    punkty->oBaza = this->oBaza;
    try {
        for (int i = 0; i < 3; i++)delete og[i];
        delete ow;
        delete on;
        delete nazwyGwiazd;
        delete nazwyGwiazdozbiorow;
        delete op;
        delete ok;
        ok=0;
    } catch (std::exception& e) {
        printf("\nProblem z usunięciem obiektów %s", e.what());
    }
    printf("\nzwolniono pamiec po wskaznikach og,ow,on,op,ok");
}

void RysujGwiazdy::KopiujUstawienia(RysujGwiazdy* rg) {

    //czcionka ustawiona wcześniej
    this->punkty->czcionkaBitmapowa=rg->punkty->czcionkaBitmapowa;
    //czy widać linie, gwiazdy wszystkie itd..
    this->punkty->coPokazac=rg->punkty->coPokazac;
    //który zaznaczony
    this->punkty->idWybranegoPunktu=rg->punkty->idWybranegoPunktu;
    printf("\nskopiowano nr zaznaczonej gwiazdy %d",punkty->idWybranegoPunktu);
    //listę widzianych wcześniej gwiazdozbiorów, trzeba by jakoś na piechotę skopiować
    //oto lista z numerami:
    std::list<int>::iterator it,kon;
    it=rg->punkty->numeryUzywanychGwiazdozbiorow.begin();
    kon=rg->punkty->numeryUzywanychGwiazdozbiorow.end();
    while(it!=kon){
        try{
            //powinno wyłapać próbę sięgnięcia po zbiór nr 196, czyli zawierający najbliższe gwiazdy, a go nie ma
        PokazKolejnyGwiazdozbior(*it);
        }catch(std::exception &e){
            if(*it==196){
                if(this->oBaza->NajblizszeGwiazdy(punkty->idWybranegoPunktu))PokazKolejnyGwiazdozbior(196);
            }else
            printf("\nproblem ze skopiowaniem numeru %d ze zbiorow\n%s",*it,e.what());
           
        }
        it++;
    }
    this->punkty->PrzygotujPunktyDoWyswietlenia(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
}


void RysujGwiazdy::WyswietlGwiazdozbiory(int jakWyswietlac) {
    int idWybranegoGwiazdozbioru = punkty->idWybranegoGwiazdozbioru;
    int wprowadzoneDane[3];
    for(int i=0;i<3;i++)wprowadzoneDane[i]=0;
    if(jakWyswietlac & (WPISANE_OD_NOWA|WPISANE_DORYSUJ)){
        //moduł pobierania danych z klawiatury przeniesiony tu z klasy COknoGL
        wprowadzoneDane[0] = 2;
        PobierzNrIloscZklawiatury(wprowadzoneDane,"nr gwiazdozbioru i ilosc");
    }
    switch (jakWyswietlac) {
        case WPISANE_OD_NOWA:
            WydzielWybraneGwiazdozbiory(wprowadzoneDane[1], wprowadzoneDane[2]);
            break;
        case ZAZNACZONE_OD_NOWA:
            if (idWybranegoGwiazdozbioru >= 0)
                WydzielWybraneGwiazdozbiory(idWybranegoGwiazdozbioru, 1);
            break;
        case ZAZNACZONE_DORYSUJ:
            if (idWybranegoGwiazdozbioru >= 0)
                PokazKolejnyGwiazdozbior(idWybranegoGwiazdozbioru);
            break;
        case WPISANE_DORYSUJ:
            PokazKolejnyGwiazdozbior(wprowadzoneDane[1]);
            break;
    }
    punkty->PrzygotujPunktyDoWyswietlenia(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
}

void RysujGwiazdy::WyswietlGwiazdozbiorZprzyciskow(int nrPobrany) {
    PokazKolejnyGwiazdozbior(nrPobrany);
    punkty->PrzygotujPunktyDoWyswietlenia(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
}

bool RysujGwiazdy::PokazUkryjGwiazdozbior(int nrGwiazdozbioru,int ilePozycji) {
    //jeżeli nie ma na liście
    int ktoryNaLiscie=-1;
    bool wynik=false;
    for(int i=0;i<ilePozycji;i++){
        ktoryNaLiscie=punkty->UsunZlistyPokazywanychGwiazdozbiorow(nrGwiazdozbioru+i);
        if(ktoryNaLiscie==-1){
           PokazKolejnyGwiazdozbior(nrGwiazdozbioru+i);
           wynik=true;
        }
        else{
            //jeśli jest, to usuwamy
            punkty->voGwiazdozbior.at(ktoryNaLiscie)->PrzekazGwiazdyDoBazy(oBaza);
            delete punkty->voGwiazdozbior.at(ktoryNaLiscie);
            punkty->voGwiazdozbior.at(ktoryNaLiscie) = 0;
            punkty->voGwiazdozbior.erase(punkty->voGwiazdozbior.begin()+ktoryNaLiscie);
        }
        
    }
    punkty->PrzygotujPunktyDoWyswietlenia(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
    return wynik;
}

void RysujGwiazdy::PokazUkryj(int doZmiany) {
    switch (doZmiany) {
        case LINIE:
            punkty->coPokazac ^= LINIE;
            break;
        case NA_SFERZE:
            punkty->coPokazac ^= NA_SFERZE;
            break;
        case W_PRZESTRZENI:
            punkty->coPokazac ^= W_PRZESTRZENI;
            break;
        case WSZYSTKIE_W_PRZESTRZENI:
            punkty->coPokazac ^= WSZYSTKIE_W_PRZESTRZENI;
            break;
        case ETYKIETY:
            punkty->coPokazac ^=ETYKIETY;
            break;
    }
    punkty->PrzygotujPunktyDoWyswietlenia(doZmiany);
}

void RysujGwiazdy::UstawKameraXYZf(float& kamX, float& kamY, float& kamZ) {
    kamX = punkty->cX;
    kamY = punkty->cY;
    kamZ = punkty->cZ;
}

void RysujGwiazdy::PobierzPrzeksztalcenia(float kameraX, float kameraY, float kameraZ, Matrix4fT & Transform) {
    punkty->wK[2] = (-kameraX - Transform.s.XZ); //znów nie wiadomo czemu trzeba zamienić x i z
    punkty->wK[1] = (-kameraY - Transform.s.YZ);
    punkty->wK[0] = (-kameraZ - Transform.s.ZZ);

}
void RysujGwiazdy::PobierzUstawieniaMacierzy(){
    glGetIntegerv(GL_VIEWPORT,punkty->viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,punkty->macierzModelWidok);
    glGetDoublev(GL_PROJECTION_MATRIX,punkty->macierzRzutowania);
}

ObliczeniaGwiazd * RysujGwiazdy::OBaza() {
    return oBaza;

}

void RysujGwiazdy::ZaznaczGwiazde(float kamX, float kamY, float kamZ, int idPunktu) {
    int idPoprzednioWybranego = punkty->idWybranegoPunktu; //??


    punkty->idWybranegoPunktu = idPunktu;
    //jeśli chcemy odznaczyć zaznaczony punkt
    if (idPoprzednioWybranego == punkty->idWybranegoPunktu){
    punkty->idWybranegoPunktu = 0; //zakładam, że wtedy nie zaznaczy żadnego
    punkty->dsParametryDoWyswietlenia.clear();
    punkty->iKolorTekstu = 127;
    return;
    }
        

    printf("\nwybrano punkt o numerze %d", punkty->idWybranegoPunktu);
    int nrHip;
    float dystans = 0.0;
    char p = (HIP | MAG | W_ABS | ODL | B_V); //|WLK_DO_WYSWIETLENIA|ABS_WIELKOSC_DO_WYSWIETLENIA
    int linia=0;
    try {
        iteratorLG itWybrany=CoWybrano();
        linia++;
        nrHip = itWybrany->Parametry(punkty->dsParametryDoWyswietlenia, p);
        linia++;
        //ustawienie centrowania
        punkty->cZ=-itWybrany->OX()/100;
        punkty->cY=-itWybrany->OY()/100;
        punkty->cX=-itWybrany->OZ()/100;
        linia++;
        //poniżej zamiana X i Z nie wiadomo dlaczego
        dystans = itWybrany->DystansDo(-kamZ*100, -kamY*100, -kamX*100);
        linia++;
        //powinno przypisać nr gwiazdozbioru, a jeśli nic nie pasuje to wstawia -1, czyli tak, jak na początku
        punkty->idWybranegoGwiazdozbioru = oBaza->DoKtoregoGwiazdozbioruNalezy(nrHip);
        linia++;
        punkty->iKolorTekstu=itWybrany->B_v();
    } catch (xNullPointerException & e) {
        punkty->dsParametryDoWyswietlenia.clear();
        printf("\nnie odnaleziono numeru hip, blad po linii %d %s",linia,e.what());
        return;
    }
    punkty->dsParametryDoWyswietlenia.push_back(oBaza->DoJakiegoGwiazdozbioruNalezy(nrHip));
    std::ostringstream os; 
    os<<"dystans "<<dystans<<" ly"; 
    punkty->dsParametryDoWyswietlenia.push_back(os.str());
    try{
       
        ParametryWatkuObliczenGwiazd * pw=new ParametryWatkuObliczenGwiazd;//pamięć jest zwalniana w wywołanym wątku
        pw->hip=nrHip;
        pw->oBaza=oBaza;
        pw->wskaznik=&ObliczeniaGwiazd::NajblizszeGwiazdy;
       
        HANDLE hThread =( HANDLE ) _beginthread(WatekObliczenGwiazd,0, pw);
       
    }catch(std::exception& e){
        printf("\nproblem z funkcja NajblizszeGwiazdy %s",e.what());
    }
}


void RysujGwiazdy::WydzielWybraneGwiazdozbiory(int nrGwiazdozbioru, int ilePozycji) {
    int voGwiazdozbiorSize = punkty->voGwiazdozbior.size();
	for (int i = 0; i < voGwiazdozbiorSize; i++) {
        punkty->voGwiazdozbior.at(i)->PrzekazGwiazdyDoBazy(oBaza);
        if (punkty->voGwiazdozbior.at(i) != 0) delete punkty->voGwiazdozbior.at(i);
        punkty->voGwiazdozbior.at(i) = 0;
    }
    //trzeba skasować poprzednią zawartość vectora
    punkty->voGwiazdozbior.clear();
    //a także listę numerów wyświetlanych gwiazdozbiorów
    punkty->numeryUzywanychGwiazdozbiorow.clear();

    ObliczeniaGwiazd * tOg[ilePozycji]; //wykorzystany do zainicjowania
    for (int i = 0; i < ilePozycji; i++) {
        tOg[i] = new ObliczeniaGwiazd(oBaza, nrGwiazdozbioru + i, NA_SFERZE);
        punkty->numeryUzywanychGwiazdozbiorow.push_back(nrGwiazdozbioru + i);
    }
    punkty->voGwiazdozbior.assign(tOg, tOg + ilePozycji);
}

void RysujGwiazdy::PokazKolejnyGwiazdozbior(int nrGwiazdozbioru) {
    ObliczeniaGwiazd * pog = new ObliczeniaGwiazd(oBaza, nrGwiazdozbioru, NA_SFERZE);
    punkty->voGwiazdozbior.push_back(pog);
    punkty->numeryUzywanychGwiazdozbiorow.push_back(nrGwiazdozbioru);
}
void RysujGwiazdy::OdszukajNr(float kamX, float kamY, float kamZ,int nrDoznalezienia){
    if(oBaza->IteratorWgHip(nrDoznalezienia)!=oBaza->LGwiazdy().end())
        this->ZaznaczGwiazde(kamX,kamY,kamZ,nrDoznalezienia);
}
void RysujGwiazdy::PokazNajblizsze(){
    //zamiast wybranych gwiazdozbiorów ma pokazać tylko gwiazdy sąsiadujące z wybraną
    //w argumencie należy przekazać listę lub wskaźnik na listę tych gwiazd
    //wyświetlane do tej pory wrzucić na stos, lub gdzie indziej
    //wprowadzić zmienną bool (flagę) - pokazujemy, czy ukrywamy
    //należy skasować i oddać gwiazdy do listy, jeżeli już raz były pokazywane najbliższe
    printf("\nPokazNajblizsze");
    this->WydzielWybraneGwiazdozbiory(196,1);
    punkty->PrzygotujPunktyDoWyswietlenia(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
}

bool RysujGwiazdy::PobierzNrIloscZklawiatury(int * wprowadzoneLiczby,char * komunikat) {
    //ile liczb pobrać
    int ileLiczb = wprowadzoneLiczby[0];
    if (ileLiczb < 2)return false;
    printf("\nprosze wprowadzic %s",komunikat);
    for (int i = 1; i <= ileLiczb; i++) {
        printf("\n%d wartosc: ", i);
        std::cin >> wprowadzoneLiczby[i];
    }
    return (wprowadzoneLiczby[1] < oBaza->ileOdczytanychNazwGwiazdozbiorow() + 2 || wprowadzoneLiczby[1] >= 0 ||
            wprowadzoneLiczby[1] + wprowadzoneLiczby[3] < oBaza->ileOdczytanychNazwGwiazdozbiorow() + 2); //dodajemy 2, żeby w zakresie było mozliwe
}

iteratorLG RysujGwiazdy::CoWybrano() {
    //trzeba siegnąć do obiektu oBaza, gdyż ma on listę, lub wektor iteratorów wszystkich gwiazd, z niej trzeba skorzystać
    return oBaza->IteratorWgHip(punkty->idWybranegoPunktu);
}


