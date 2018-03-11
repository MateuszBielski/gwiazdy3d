/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PodzialNaKomorki.cpp
 * Author: BCO
 * 
 * Created on 13 kwietnia 2016, 10:31
 */

#include "PodzialNaKomorki.h"
#include "OdczytParametrow.h"

PodzialNaKomorki::PodzialNaKomorki() {
        ileZakresowWidac=0;
    wspolczynikWidokuPunktow=0.8;
    ileKomorekWzakresach = 0;
    promienPrzedzialu = 0;
    doKtoregoPrzedzialu = 0;
    granice = 0;
    poczatkiKomorek = 0;
    pS = 0;
}

PodzialNaKomorki::PodzialNaKomorki(const PodzialNaKomorki& orig) {
}

PodzialNaKomorki::~PodzialNaKomorki() {
    if (doKtoregoPrzedzialu) delete[]doKtoregoPrzedzialu;
    if (ileKomorekWzakresach) delete[]ileKomorekWzakresach;
    if (pS)delete[] pS;
    if (granice)delete[] granice;
    if (poczatkiKomorek)delete[] poczatkiKomorek;
    if (promienPrzedzialu) delete[] promienPrzedzialu;
}

void PodzialNaKomorki::PobierzParametry(int dlugoscPodst, int dz, int ilePoz, float * tFloat) {
    //dPodst = dlugoscPodst;
    //    dzielnik = dz;
    ileGranic = ilePoz;
    granice = new float[ileGranic];
    for (int i = 0; i < ileGranic; i++)granice[i] = tFloat[i];
}

void PodzialNaKomorki::PobierzParametry(OdczytParametrow * op) {
    std::string p1 = "granicePodzialu";
    std::string p2 = "dzielnik";
    std::string p3 = "dlugoscPierwotna";
    std::string p4 = "ileZakresowWidac";
    std::string p5 = "ileWkomorce";
    std::string p6 = "wspolczynikWidokuPunktow";


    ileGranic = op->OdczytajParametr(p1, granice);
        op->OdczytajParametr(p4, ileZakresowWidac);
    op->OdczytajParametr(p5, ileWkomorce);
    op->OdczytajParametr(p6, wspolczynikWidokuPunktow);
	//sprawdzenie wartości ileZakresów widać
    if(ileZakresowWidac<0 || ileZakresowWidac>ileGranic+2){
        ileZakresowWidac=0;
        printf("\nPodzialNaKomorki::PobierzParametry wyzerowano ileZakrsowWidac");
    }
}

void PodzialNaKomorki::ObliczWielkosciZakresow(std::list<Gwiazda2>& listaDoRozdzielenia) {
	//granice są pomiędzy przedziałami więc pierwsza jedynka, a druga bo jeszcze zewnetrzny zakres dla wszystkich, ktore się nie mieszczą.
    int ilePrzedzialow = ileGranic + 1 + 1; 
    ileKomorekWzakresach = new int[ilePrzedzialow];
	//zakladamy, że średnia odległość punktu do środka kuli s=r*2^(1/3) - trzeciego stopnia pierwiazstek z 2
    promienPrzedzialu = new float[ilePrzedzialow]; 
	float pierw2st2 = pow(2, 0.5);
    float sumaOdleglosci[ilePrzedzialow];
    float sredniaOdleglosc[ilePrzedzialow];
    float maksymalnaOdleglosc[ilePrzedzialow];
    int ilosciGwiazdWzakresach[ilePrzedzialow];
    int ileZaDlugich[ilePrzedzialow];

    float odl[wielkoscKataloguHip];
    //tablica wszystkie gwiazdy, do ktorego przedziału
    doKtoregoPrzedzialu = new int[wielkoscKataloguHip]; //zwolnienie pamięci będzie w destruktorze
    for (int i = 0; i < wielkoscKataloguHip; i++)doKtoregoPrzedzialu[i] = -1;
    for (int i = 0; i < ilePrzedzialow; i++) {
        sumaOdleglosci[i] = 0;
        ilosciGwiazdWzakresach[i] = 0;
        maksymalnaOdleglosc[i] = 0;
        ileZaDlugich[i] = 0;
        promienPrzedzialu[i] = 0.0;
    }
    iteratorLG it = listaDoRozdzielenia.begin();
    int blad = 0;
    while (it != listaDoRozdzielenia.end()) {
        int przedzial = 1; //bo zerowy przeznaczony dla gwiazd nie mieszczących się w przedziałach
        float M = it->JM();
        int hip = it->Hip();
        if (hip >= wielkoscKataloguHip) {
            blad++;
            printf("\nhip za duzy %d", hip);
            it++;
            continue;
        }
        odl[hip] = it->Odleglosc();
        bool sprawdzaj = true;
        //od najmniejszych liczb, czyli największych gwiazd
        if (M < granice[0]) {
            sprawdzaj = false;
            przedzial = 1;
        }
        int ktoraGranica = 1;
        while (sprawdzaj && (ktoraGranica < ileGranic)) {
            if (M >= granice[ktoraGranica - 1] && M < granice[ktoraGranica]) {
                sprawdzaj = false;
                przedzial = ktoraGranica + 1; //do przedostatniego przedziału
                if (przedzial >= ilePrzedzialow) {
                    printf("\nwhile przedzial %d, ileGranic %d, ktoraGranica %d,nrHip %d", przedzial, ileGranic, ktoraGranica, hip);
                    system("pause");
                }
            }
            ktoraGranica++;
        }
        if (sprawdzaj && M >= granice[ileGranic - 1]) {
            sprawdzaj = false;
            przedzial = ileGranic + 1; //ostatni
        }
        sumaOdleglosci[przedzial] += odl[hip];
        ilosciGwiazdWzakresach[przedzial]++;
        doKtoregoPrzedzialu[hip] = przedzial;
        //ustalenie maksymalnej odległości
        float tempMaxOdl = maksymalnaOdleglosc[przedzial];
        maksymalnaOdleglosc[przedzial] = (odl[hip] > tempMaxOdl) ? odl[hip] : tempMaxOdl;
        it++;
    }
    //obliczenie średniego promienia
    printf("\nObliczWielkosciZakresow przed o/bliczeniem sredniego promienia");
    int ileRozdzielono = 0;
    for (int i = 1; i < ilePrzedzialow; i++) {//od 1 bo ogólna strefa zewnętrzna nie ma odległości
        if (ilosciGwiazdWzakresach[i] != 0)
            sredniaOdleglosc[i] = sumaOdleglosci[i] / (float) ilosciGwiazdWzakresach[i];
        else sredniaOdleglosc[i] = 0.0;
        promienPrzedzialu[i] = sredniaOdleglosc[i] * pierw2st2;
       ileRozdzielono += ilosciGwiazdWzakresach[i];
    }
    printf("\nObliczWielkosciZakresow ile rozdzielono %d, bledy %d", ileRozdzielono, blad);
    it = listaDoRozdzielenia.begin();
    while (it != listaDoRozdzielenia.end()) {
        //jesli rzut odległości na którąkolwiek oś jest większa niż promień średni - oznacza to, że gwiazda znajduje się poza sześcianem danej strefy
        int hip = it->Hip();
        int ktoryPrzedzial = doKtoregoPrzedzialu[hip];
        if (ktoryPrzedzial < 0)printf("\nhip nie przydzielone");
        if (ktoryPrzedzial == 0) {
            printf("\nprzedzial 0 pomimo, ze nie powinien wystapic");
        }
        //najpierw sprawdzamy, czy odległość jest większa niż promień, jeżeli tak to próbujemy zmieścić w sześcianie (chodzi o narożniki, które "wystają" poza sferę), 
        //a jeśli wciąż za duża to w następnym, większym przedziale, aż do ostatniego
        if (odl[hip] > promienPrzedzialu[ktoryPrzedzial]) {
            //printf("\nnie pasuje przedzial %d,odl %2.3f",ktoryPrzedzial,odl[hip]);
            ileZaDlugich[ktoryPrzedzial]++;
            //jeżeli gwiazda jest położona poza danym przedziałem, należy to uwzglednić w ilości gwiazd w przedziałach
            ilosciGwiazdWzakresach[ktoryPrzedzial]--;
            double x, y, z;
            x = (double) (abs) (odl[hip] * it->X());
            y = (double) (abs) (odl[hip] * it->Y());
            z = (double) (abs) (odl[hip] * it->Z());
            bool szukaj = false;
            double promien = 0;
            do {
                promien = (double) promienPrzedzialu[ktoryPrzedzial];
                if (x > promien || y > promien || z > promien) {
                    //sprawdzać, czy zmieści się w większym zakresie
                    szukaj = true;
                    ktoryPrzedzial--;
                } else {
                    szukaj = false;
                }
            } while (szukaj && ktoryPrzedzial > 0);
            //po wyjściu z pętli mamy odszukany właściwy przedział, który należy zauktualizować
            //jeżeli mieści się w tym przedziale, to dodajemy do ilości zakresu:
            ilosciGwiazdWzakresach[ktoryPrzedzial]++;
            //i uaktualniamy nr przedziału, do którego należy
            doKtoregoPrzedzialu[hip] = ktoryPrzedzial;
            if (ktoryPrzedzial < 0) {
                printf("\nprzedzial ujemny pomimo, ze nie powinien wystapic");
            }
        }
        it++;
    }
    //obliczenie ilosci komórek. Zakładamy, że komórek ma być tyle, aby w każdej średnio była zadana ilość gwiazd
    //ustalamy też od którego miejsca w liście lGwiazdyWkomorkach będzie zaczynał się dany przedział
    pS = new int[ilePrzedzialow];
	//tymczasowe, podniesione do 3 potęgi wartości ileKomorekWzakresach
    int ileKom3[ilePrzedzialow];
    pS[0] = 0;
    ileKomorek = 1 + 1; //na strefę zerową i na lista.end()
	//strefa ogólna dla wszystkich gwiazd, które nie mieszczą się w pozostałych sześcianach
    ileKomorekWzakresach[0] = 1; 
    ileKom3[0] = 1;
    for (int i = 1; i < ilePrzedzialow; i++) {
        float ile = pow((float) (ilosciGwiazdWzakresach[i] / ileWkomorce), (1.0 / 3)); //pierwiastek trzeciego stopnia, zaokrąglony
        ileKomorekWzakresach[i] = ile >= 2.0 ? (int) round(ile) : 2; //minimalnie w zakresie 8 komórek
        //trzeba wymusić parzystą ilość
        if((ileKomorekWzakresach[i]%2)==0)ileKomorekWzakresach[i]++;
        ileKom3[i] = (int) round(pow(ileKomorekWzakresach[i], 3.0));
    }

    for (int i = 1; i < ilePrzedzialow; i++) {
        pS[i] = pS[i - 1] + ileKom3[i - 1];
        ileKomorek += ileKom3[i];
    }
    printf("\nIle komórek lacznie %d", ileKomorek);
    //wyniki
    //ile gwiazd, jaki promień, jaka maksymalna odległość, ile procent dluższa od promienia, ile trzeba komórek, żeby było równo
    printf("\nObliczWielkosciZakresow: ileGwiazd, promien sredni, maksymalna odleglosc, ile proc za dlugich, ile komorek w zakresach, pS[]");
    printf("\n %d, pS[] %d", ilosciGwiazdWzakresach[0], pS[0]);
    for (int i = 1; i < ilePrzedzialow; i++) {
        float ileProcentowoZaDlugich = 100.0 * ((float) ileZaDlugich[i] / ilosciGwiazdWzakresach[i]);
        printf("\n %d szt, %2.3f ls, %2.3f ls, %2.1f proc, %d,pS[] %d", ilosciGwiazdWzakresach[i], promienPrzedzialu[i], maksymalnaOdleglosc[i], ileProcentowoZaDlugich, ileKomorekWzakresach[i], pS[i]);
    }
}

void PodzialNaKomorki::PrzydzielDoStref(std::list<Gwiazda2>&listaDoRozdzielenia) {
	iteratorLG it, it1, koniec;
    it = listaDoRozdzielenia.begin();
    koniec = listaDoRozdzielenia.end();
    it1 = it;
    bool wtorne = false;
    //utworzenie i zainicjowanie tablicy zawierajacej iteratory wskazujące na początek danych każdej komórki(strefy przestrzennej)
    if (lGwiazdyWkomorkach.empty() && doKtoregoPrzedzialu == 0 && poczatkiKomorek == 0) {
        ObliczWielkosciZakresow(listaDoRozdzielenia);
        //ponieważ czymś trzeba wypełnić powyższą listę, to wypełniamy ją iteratorami kolejnych gwiazd z głównej listy
        for (int i = 0; i < ileKomorek - 1; i++)lGwiazdyWkomorkach.push_back(it1++);
        iteratorLitLG itlg = lGwiazdyWkomorkach.begin();
        //a tablicę iteratorami do powyższej listy
        poczatkiKomorek = new iteratorLitLG[ileKomorek];
        for (int i = 0; i < ileKomorek; i++)poczatkiKomorek[i] = itlg++; //teoretycznie na ostatnim miejscu powinien być lGwiazdyWkomorkach.end()
        printf("\nPrzydzielDoStrefPrzestrzennych wypelniono tablice poczatkiKomorek[%d]", ileKomorek);
    } else {
        wtorne = true;
    }
    //pętla dla wszystkich gwiazd
    int blad = 0;
    int rozmiarPrzedrozdzieleniem = lGwiazdyWkomorkach.size();
    int ileDoRozdzielenia = listaDoRozdzielenia.size();
    int ktory = 0;
    while (it != koniec) {

        int hip = it->Hip();
        int nrKom = 0;
        float odl = it->Odleglosc();
        //który przedział
        int ktoryPrzedzial = doKtoregoPrzedzialu[hip];
        if (ktoryPrzedzial < 0)printf("\nhip nie przydzielone");
        if (ktoryPrzedzial > ileGranic + 1) {
            it++;
            blad++;
            continue;
        }
        if (ktoryPrzedzial == 0)nrKom = 0;
        else {

            double x, y, z, r;
            int ix, iy, iz, iK;
            x = (double) odl * it->X();
            y = (double) odl * it->Y();
            z = (double) odl * it->Z();
            r = promienPrzedzialu[ktoryPrzedzial];
            iK = ileKomorekWzakresach[ktoryPrzedzial];
            ix = (int) round(((iK - 1) / 2.0)*(1.0 + x / r)); //(iK-1) - bo komórki powinny mieć numerację od 0 do iK-1, tak jak tablice
            iy = (int) round(((iK - 1) / 2.0)*(1.0 + y / r));
            iz = (int) round(((iK - 1) / 2.0)*(1.0 + z / r));

            nrKom = (pS[ktoryPrzedzial] + iz * iK * iK + iy * iK + ix); //numer komórki do której należy dodać daną gwiazdę
            if (nrKom >= ileKomorek - 1)printf("\nprzedzial %d, nrKom %d,pS %d,iK %d,ix %d,iy %d,iz %d,x %2.3f,r %2.3f", ktoryPrzedzial, nrKom, pS[ktoryPrzedzial], iK, ix, iy, iz, x, r);
        }

        if (nrKom >= ileKomorek - 1) {
            it++;
            blad++;

            continue;
        }
        //sięgamy do odpowieniej tabeli,ażeby znaleźć miejsce gdzie wstawić 
        iteratorLitLG gdzieWstawic = poczatkiKomorek[nrKom + 1]; //bo wstawiamy przed początkiem następnej komórki
        //w aktualnej Gwieździe umieszczamy iterator listy iteratorow, zwrócony przez funkcję insert, aby można go było usunąć zlisty właśnie z poziomu gwiazdy

        it->UstawItWliscieKomorek(lGwiazdyWkomorkach.insert(gdzieWstawic, it));
        it++;
		ktory++;
	}
}

int PodzialNaKomorki::KtorePunktyWidac(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip,listaItLG& listaIteratorow) {
    int nrKom[ileGranic + 2]; //tyle jest przedziałów łącznie z zewnętrznym
	//która komórka w osi x(y,z) licząc od najdalej wysuniętej w stronę ujemnych do dodatnich
    int ix, iy, iz, iK; 
    double r;
    nrKom[0] = 0;
    for (int i = 0; i <= ileGranic + 1; i++) {
        //d = dPodst / pow(2, i); //16000 podzielić przez, kolejno 1,2,4,8, czyli 16000 8000 4000 2000
        iK = ileKomorekWzakresach[i];
        r = promienPrzedzialu[i];
        
		//można spróbwać przesunąć widziane komórki w kierunku dali, w związku z tym można punkt p przemnożyć przez różnej długości wektory,
		//np związane z wielkością przedziału (r), ponieważ byłby on w liczniku, a w mianowniku też, to się skracają
		ix = (int) round(((iK -1) / 2.0)*(1.0 + wspolczynikWidokuPunktow * p[0]/r ));
        iy = (int) round(((iK -1) / 2.0)*(1.0 + wspolczynikWidokuPunktow * p[1]/r ));
        iz = (int) round(((iK -1) / 2.0)*(1.0 + wspolczynikWidokuPunktow* p[2]/r ));
        nrKom[i] = (pS[i] + iz * iK * iK + iy * iK + ix);
    }

    //przed uzupełnieniem listy, trzeba ją wyzerować
    //a może nie trzeba? bo wcześniej jest zerowana
    //punktyWszystkichGwiazd.clear();
    //znając numery komórek możemy dowiedzieć się, które gwiazdy do nich należą
    //lGwiazdyWkomorkach;//zawierać będzie iteratory do wyszystkich gwiazd ułożonych kolejno komórkami
    int ileWyswietlono=0;
    for (int i = ileGranic + 1; i >=0 ; i--) {
        //należy ustalić zakres z którego odczytujemy elementy, czyli początek i koniec
        //początkiem jest nrKom[i], zwiekszony o jeden, a końcem jest iterator z wskazujący na początek następnej komórki
        if ((nrKom[i] > ileKomorek - 1) || (100 * abs(p[0]) > promienPrzedzialu[i]) || (100 * abs(p[1]) > promienPrzedzialu[i]) || (100 * abs(p[2]) > promienPrzedzialu[i])) {
            continue;
        }
        iteratorLitLG itL, kon;
        itL = poczatkiKomorek[nrKom[i]]; //tutaj przechodzimy do adresu następnej komórki
        itL++;
        kon = poczatkiKomorek[nrKom[i] + 1];
		iteratorLG itG;
        int licznik = 0;
        while (itL != kon) {
            itG = *itL;
            double k, w, x, y, z;
            k = (double)itG->B_v();
            if (k>127.0) k=127.0;//żeby nie czytało za końcem tablicy r[128],g[128],b[128]
            w = itG->AbsolutnaWlkDoWyswietlenia();
            x = itG->OX()/100;
            y = itG->OY()/100;
            z = itG->OZ()/100;
            punktyWszystkichGwiazd.push_back(k);
            punktyWszystkichGwiazd.push_back(w);
            punktyWszystkichGwiazd.push_back(x);
            punktyWszystkichGwiazd.push_back(y);
            punktyWszystkichGwiazd.push_back(z);
            listaHip.push_back(itG->Hip());
            //z przodu są z nazwami, a dalej bez nazw
            if(!itG->Nazwa().empty())listaIteratorow.push_front(itG);
            else listaIteratorow.push_back(itG);
            licznik++;
            itL++;
        }
        //jeżeli punkt widzenia jest poza sześcianem o długości boku dPodst przerywamy pętlę i nie dodajemy gwiazd dla mniejszych komórek
        //czy wyświetlać dalsze zakresy?
        ileWyswietlono++;
        if(ileZakresowWidac > 0 && ileWyswietlono>=ileZakresowWidac )break;
    }
    
}

int PodzialNaKomorki::UsunIterator(iteratorLG & itG) {
    int wynik = 0;
	if (lGwiazdyWkomorkach.empty() || itG->ItWliscieKomorek() == lGwiazdyWkomorkach.end()) {
        wynik = 1;
	} else {
        lGwiazdyWkomorkach.erase(itG->ItWliscieKomorek());
        itG->UstawItWliscieKomorek(lGwiazdyWkomorkach.end()); //wskazuje na ostatni elementdla bezpieczeństwa, żeby już nie usuwać, 
        //jeśli przypadkiem trafi się już raz usunięta, ustawianie na list.end() powodowało błędy
    }
    return wynik;
}

