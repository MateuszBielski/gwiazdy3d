/* 
 * File:   PodzialNaKomorki.cpp
 * Author: Mateusz Bielski
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
    //    zasiegWidzenia = 0;
    //    d = 0;
    pS = 0;
    //printf("\npodzial na komorki konstruktor");
}

PodzialNaKomorki::PodzialNaKomorki(const PodzialNaKomorki& orig) {
}

PodzialNaKomorki::~PodzialNaKomorki() {
    if (doKtoregoPrzedzialu) delete[]doKtoregoPrzedzialu;
    if (ileKomorekWzakresach) delete[]ileKomorekWzakresach;
    if (pS)delete[] pS;
    //if (d)delete[] d;
    //   if (zasiegWidzenia)delete[] zasiegWidzenia;
    if (granice)delete[] granice;
    if (poczatkiKomorek)delete[] poczatkiKomorek;
    if (promienPrzedzialu) delete[] promienPrzedzialu;
}

//void PodzialNaKomorki::UstalKomorkiZapas() {
//    //dPodst=dlugoscPierwotna*pow(dzielnik,ileGranic-1);
//    d = new int[ileGranic];
//    d[0] = dPodst;
//    pS = new int[ileGranic];
//    pS[0] = 1;
//    //dla dzielnika 2 i dPodst 2000:
//    for (int i = 1; i < ileGranic; i++) {//1,2,3
//        d[i] = (dPodst / pow(dzielnik, i)); //1000,500,250
//        pS[i] = pS[i - 1] + pow(2 * pow(dzielnik, i - 1), 3); //1+8+64+512
//    }
//    ileKomorek = 1 + pS[ileGranic - 1] + pow(2 * pow(dzielnik, ileGranic - 1), 3);
//    poczatkiKomorek = new iteratorLitLG[ileKomorek];
//    printf("\nileGranic %d, dPodst %d, pS[ ]",ileGranic,dPodst);
//    for(int i =0;i<ileGranic;i++)printf(" %d,",pS[i]);
//    for(int i =0;i<ileGranic;i++)printf(" %2.3f,",granice[i]);
//}

void PodzialNaKomorki::PobierzParametry(int dlugoscPodst, int dz, int ilePoz, float * tFloat) {
    //dPodst = dlugoscPodst;
    //    dzielnik = dz;
    ileGranic = ilePoz;
    granice = new float[ileGranic];
    for (int i = 0; i < ileGranic; i++)granice[i] = tFloat[i];
    //    //tFloat może mieć różną ilość
    //UstalKomorki();
    //    printf("\nPobierzParametry - porbrano, dMaks %d, ileKomorek %d",dMaks,ileKomorek);
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
    //op->OdczytajParametr(p3, dPodst);
    //    ileZakresowWidac = op->OdczytajParametr(p4, zasiegWidzenia);
    op->OdczytajParametr(p5, ileWkomorce);
    op->OdczytajParametr(p6, wspolczynikWidokuPunktow);
//sprawdzenie wartości ileZakresów widać
    if(ileZakresowWidac<0 || ileZakresowWidac>ileGranic+2){
        ileZakresowWidac=0;
        printf("\nPodzialNaKomorki::PobierzParametry wyzerowano ileZakrsowWidac");
    }//UstalKomorki();
//        printf("\nPodzialNaKomorki::PobierzParametry ileGranic %d",ileGranic);
    //    for(int i=0;i<ileGranic;i++){
    //        printf(" %2.2f",granice[i]);
    //    }
}

void PodzialNaKomorki::ObliczWielkosciZakresow(std::list<Gwiazda2>& listaDoRozdzielenia) {
    //printf("\nPodzialNaKomorki::ObliczWielkosciZakresow poczatek, ileGranic %d, ileWkomorce %d", ileGranic, ileWkomorce);
    int ilePrzedzialow = ileGranic + 1 + 1; //granice są pomiędzy przedziałami więc pierwsza jedynka, a druga bo jeszcze zewnetrzny zakres dla wszystkich, ktore się nie mieszczą.
    ileKomorekWzakresach = new int[ilePrzedzialow];
    promienPrzedzialu = new float[ilePrzedzialow]; //zakladamy, że średnia odległość punktu do środka kuli s=r*2^(1/3) - trzeciego stopnia pierwiazstek z 2

    //suma odległości
//    float pierw3st2 = pow(2, (1 / 3));
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
//    printf("\nilePrzedzialow %d, ilegwiazd (size) %d", ilePrzedzialow, listaDoRozdzielenia.size());
//    printf("\nObliczWielkosciZakresow poczatek przed rozdzieleniem");
    iteratorLG it = listaDoRozdzielenia.begin();
    //int nrGwiazdy=0;
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
            //printf("\n ktoraGranica %d nrGwiazdy %d",ktoraGranica,nrGwiazdy);
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
            //if(przedzial>=ilePrzedzialow)printf("\nostatni przedzial %d",przedzial);
        }
        //        if(przedzial>=ilePrzedzialow){
        //            blad++;
        //            it++;
        //            nrGwiazdy++;
        //            continue;
        //        }
        sumaOdleglosci[przedzial] += odl[hip];
        ilosciGwiazdWzakresach[przedzial]++;
        doKtoregoPrzedzialu[hip] = przedzial;
        //ustalenie maksymalnej odległości
        float tempMaxOdl = maksymalnaOdleglosc[przedzial];
        maksymalnaOdleglosc[przedzial] = (odl[hip] > tempMaxOdl) ? odl[hip] : tempMaxOdl;
        //printf("\n%d %2.2f",nrGwiazdy,maksymalnaOdleglosc[przedzial]);
        it++;
        //nrGwiazdy++;
        //printf("\n%d %d",nrGwiazdy,przedzial);
    }
    //obliczenie średniego promienia
//    printf("\nilosciGwiazdWzakresach[0]%d ", ilosciGwiazdWzakresach[0]);
    printf("\nObliczWielkosciZakresow przed o/bliczeniem sredniego promienia");
    int ileRozdzielono = 0;
    for (int i = 1; i < ilePrzedzialow; i++) {//od 1 bo ogólna strefa zewnętrzna nie ma odległości
        if (ilosciGwiazdWzakresach[i] != 0)
            sredniaOdleglosc[i] = sumaOdleglosci[i] / (float) ilosciGwiazdWzakresach[i];
        else sredniaOdleglosc[i] = 0.0;
        promienPrzedzialu[i] = sredniaOdleglosc[i] * pierw2st2;
        //printf("\nile gwiazd %d, srednia odleglosc %2.3f, promien sredni %2.3f",ilosciGwiazdWzakresach[i],sredniaOdleglosc[i],promienPrzedzialu[i]);
        ileRozdzielono += ilosciGwiazdWzakresach[i];
    }
    printf("\nObliczWielkosciZakresow ile rozdzielono %d, bledy %d", ileRozdzielono, blad);
    //obliczenie ile dluższych niż promień
//    printf("\nObliczWielkosciZakresow przed ustaleniem za dlugich");
    //petla dla wszystkich gwiazd zawartych w liście
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
                    //printf("\nx>promien || y>promien || z>promien x: %2.3f,y: %2.3f,z: %2.3f,promien: %2.3f, przedzial",x,y,z,promien,ktoryPrzedzial);
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
            //            if(ktoryPrzedzial==0){
            //                printf("\nprzedzial 0 %d",ileKomorekWzakresach[0]);
            //                
            //            }
            //            printf("\npasuje x: %2.3f,y: %2.3f,z: %2.3f,promien: %2.3f, przedzial %d",x,y,z,promien,ktoryPrzedzial);
            //            system("pause");
        }
        it++;
        //            printf("\n%d",ileZaDlugich[ktoryPrzedzial]++);
    }
    //obliczenie ilosci komórek. Zakładamy, że komórek ma być tyle, aby w każdej średnio była zadana ilość gwiazd
    //ustalamy też od którego miejsca w liście lGwiazdyWkomorkach będzie zaczynał się dany przedział
    pS = new int[ilePrzedzialow];
    int ileKom3[ilePrzedzialow]; //tymczasowe, podniesione do 3 potęgi wartości ileKomorekWzakresach
    pS[0] = 0;
    ileKomorek = 1 + 1; //na strefę zerową i na lista.end()
    ileKomorekWzakresach[0] = 1; //strefa ogólna dla wszystkich gwiazd, które nie mieszczą się w pozostałych sześcianach
    ileKom3[0] = 1;
    for (int i = 1; i < ilePrzedzialow; i++) {
        float ile = pow((float) (ilosciGwiazdWzakresach[i] / ileWkomorce), (1.0 / 3)); //pierwiastek trzeciego stopnia, zaokrąglony
        ileKomorekWzakresach[i] = ile >= 2.0 ? (int) round(ile) : 2; //minimalnie w zakresie 8 komórek
        //trzeba wymusić parzystą ilość
        if((ileKomorekWzakresach[i]%2)==0)ileKomorekWzakresach[i]++;
        ileKom3[i] = (int) round(pow(ileKomorekWzakresach[i], 3.0));
        //        printf("\n%d ileKom3[i] %d",i,ileKom3[i]);
        //        pS[i]=pS[i-1]+(int)pow(ileKomorekWzakresach[i-1],3.0);
        //        ileKomorek+=pS[i];
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
    //do zwrotu ilość wszystkich komórek
    //    printf("\nObliczWielkosciZakresow koniec");

}

void PodzialNaKomorki::PrzydzielDoStref(std::list<Gwiazda2>&listaDoRozdzielenia) {

    //printf("\nPrzydzielDoStrefPrzestrzennych poczatek");
    iteratorLG it, it1, koniec;
    it = listaDoRozdzielenia.begin();
    koniec = listaDoRozdzielenia.end();
    it1 = it;
    bool wtorne = false;
    //utworzenie i zainicjowanie tablicy zawierajacej iteratory wskazujące na początek danych każdej komórki(strefy przestrzennej)
    if (lGwiazdyWkomorkach.empty() && doKtoregoPrzedzialu == 0 && poczatkiKomorek == 0) {
        ObliczWielkosciZakresow(listaDoRozdzielenia);
        //printf("\npo funkcji ObliczWielkosciZakresow");
        //ponieważ czymś trzeba wypełnić powyższą listę, to wypełniamy ją iteratorami kolejnych gwiazd z głównej listy
        for (int i = 0; i < ileKomorek - 1; i++)lGwiazdyWkomorkach.push_back(it1++);
        iteratorLitLG itlg = lGwiazdyWkomorkach.begin();
        //a tablicę iteratorami do powyższej listy
        // printf("\nprzed petla itlg++");
        poczatkiKomorek = new iteratorLitLG[ileKomorek];
        for (int i = 0; i < ileKomorek; i++)poczatkiKomorek[i] = itlg++; //teoretycznie na ostatnim miejscu powinien być lGwiazdyWkomorkach.end()
        printf("\nPrzydzielDoStrefPrzestrzennych wypelniono tablice poczatkiKomorek[%d]", ileKomorek);
    } else {
        wtorne = true;
        //printf("\nPrzydzielDoStrefPrzestrzennych podczas zwrotu do bazy");
    }
    //pętla dla wszystkich gwiazd
    int blad = 0;
    int rozmiarPrzedrozdzieleniem = lGwiazdyWkomorkach.size();
    int ileDoRozdzielenia = listaDoRozdzielenia.size();
    int ktory = 0;
//    if (wtorne) {
//        printf("ile do rozdzielenia %d ", ileDoRozdzielenia);
//        //        koniec--;//ostatniej nie przydziela dla próby
//    }
    while (it != koniec) {

        int hip = it->Hip();
        int nrKom = 0;
        float odl = it->Odleglosc();

        //który przedział
        int ktoryPrzedzial = doKtoregoPrzedzialu[hip];
        if (ktoryPrzedzial < 0)printf("\nhip nie przydzielone");
        //printf("\nhip %d, przedzial %d, odl %2.3f",hip,ktoryPrzedzial,odl);
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
            //printf("\nr %2.3f, iK %d",r,iK);


            //iK = dPodst / d[ktoryPrzedzial];
            ix = (int) round(((iK - 1) / 2.0)*(1.0 + x / r)); //(iK-1) - bo komórki powinny mieć numerację od 0 do iK-1, tak jak tablice
            iy = (int) round(((iK - 1) / 2.0)*(1.0 + y / r));
            iz = (int) round(((iK - 1) / 2.0)*(1.0 + z / r));

            nrKom = (pS[ktoryPrzedzial] + iz * iK * iK + iy * iK + ix); //numer komórki do której należy dodać daną gwiazdę
            //            if(licznik[5]<100){
            //                printf("\nM %2.2f, d %d, pS %d",M,d,pS);
            //            }
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
        //if (wtorne)printf(" %d %d %d;", ktoryPrzedzial, hip, nrKom);
        it++;
//        if (it == koniec && wtorne) {
//            int  roznica = lGwiazdyWkomorkach.size() - rozmiarPrzedrozdzieleniem;
//            printf("\nosiagnieto koniec listy do rozdzielenia, wstawiono %d elementow",roznica);
//            //            printf("\nosiagnieto koniec listy do rozdzielenia");
//        }
        ktory++;

        //               printf("\nsprawdzono 200 000 pozycji i przerwano");
        //               it=lGwiazdy.end();
        //           }
    }
    //sprawdzenie ile zawiera każda komórka
    //    int k=0;
    //    for (int i = 0; i < ileKomorek-1; i++){
    //        iteratorLitLG pocz,kon;
    //        pocz=poczatkiKomorek[i];
    //        kon=poczatkiKomorek[i+1];
    //        int licznik=0;
    //        while(pocz++!=kon)licznik++;
    //        printf("\n%d %d",i,licznik);
    //        if(k==100){system("pause");k=0;}
    //        k++;
    //    }
    //printf("\nPodzialNaKomorki::PrzydzielDoStref lGwiazdyWkomorkach uzupelnione rozmiar %d, bledy %d", lGwiazdyWkomorkach.size(), blad);
}

int PodzialNaKomorki::KtorePunktyWidac(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip,listaItLG& listaIteratorow) {
    int nrKom[ileGranic + 2]; //tyle jest przedziałów łącznie z zewnętrznym
    int ix, iy, iz, iK; //która komórka w osi x(y,z) licząc od najdalej wysuniętej w stronę ujemnych do dodatnich
    double r;
    nrKom[0] = 0;
        //printf("\nPodzialNaKomorki::KtorePunktyWidac");
    for (int i = 0; i <= ileGranic + 1; i++) {
        //d = dPodst / pow(2, i); //16000 podzielić przez, kolejno 1,2,4,8, czyli 16000 8000 4000 2000
        iK = ileKomorekWzakresach[i];
        r = promienPrzedzialu[i];
        
    //można spróbwać przesunąć widziane komórki w kierunku dali, w związku z tym można punkt p przemnożyć przez różnej długości wektory,
    //np związane z wielkością przedziału (r), ponieważ byłby on w liczniku, a w mianowniku też, to się skracają
//        ix = (int) round(((iK - 1) / 2.0)*(1.0 + 100.0*r * p[0] / r));
        
        ix = (int) round(((iK -1) / 2.0)*(1.0 + wspolczynikWidokuPunktow * p[0]/r ));
        //printf("\n(100*p[0]/d) %2.5f, round(100*p[0]/d) %2.5f",(100*p[0]/d),round(100*p[0]/d));
        iy = (int) round(((iK -1) / 2.0)*(1.0 + wspolczynikWidokuPunktow * p[1]/r ));

        iz = (int) round(((iK -1) / 2.0)*(1.0 + wspolczynikWidokuPunktow* p[2]/r ));

        nrKom[i] = (pS[i] + iz * iK * iK + iy * iK + ix);
//        if (nrKom[i] > ileKomorek - 1)
//            printf("\nprzedzial %d, nrKom %d,pS %d,iK %d,ix %d,iy %d,iz %d,p[0] %2.3f,p[1] %2.3f,p[2] %2.3f,r %2.3f", i, nrKom[i], pS[i], iK, ix, iy, iz, p[0], p[1], p[2], r);
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
            //printf("\nprzerwanie");
            continue;
        }
        iteratorLitLG itL, kon;
        itL = poczatkiKomorek[nrKom[i]]; //tutaj przechodzimy do adresu następnej komórki
        itL++;
        kon = poczatkiKomorek[nrKom[i] + 1];

        //printf("\npoziom %d, nrKom[i] %d",i,nrKom[i]);
        iteratorLG itG;
        int licznik = 0;
        while (itL != kon) {
            itG = *itL;
            //double odl = (double) itG->Odleglosc() / 100;
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
            //printf(" %d,",itG->Hip());
            itL++;
        }
        //printf("ile w komorce: %d,",licznik);
        //jeżeli punkt widzenia jest poza sześcianem o długości boku dPodst przerywamy pętlę i nie dodajemy gwiazd dla mniejszych komórek
        //czy wyświetlać dalsze zakresy?
        ileWyswietlono++;
        if(ileZakresowWidac > 0 && ileWyswietlono>=ileZakresowWidac )break;
    }
    
}

int PodzialNaKomorki::UsunIterator(iteratorLG & itG) {
    int wynik = 0;

    //printf("\n PodzialNaKomorki::UsunIterator rozmiar lGwiazdyWkomorkach %d, hip %d ",lGwiazdyWkomorkach.size(),itG->Hip());
    if (lGwiazdyWkomorkach.empty() || itG->ItWliscieKomorek() == lGwiazdyWkomorkach.end()) {
        wynik = 1;

        //printf("\n PodzialNaKomorki::UsunIterator blad");
    } else {
        lGwiazdyWkomorkach.erase(itG->ItWliscieKomorek());
        itG->UstawItWliscieKomorek(lGwiazdyWkomorkach.end()); //wskazuje na ostatni elementdla bezpieczeństwa, żeby już nie usuwać, 
        //jeśli przypadkiem trafi się już raz usunięta, ustawianie na list.end() powodowało błędy
       // printf("\nusun iterator");
    }
    return wynik;
}
//void PodzialNaKomorki::PrzydzielDoStrefZapas(std::list<Gwiazda2>& listaDoRozdzielenia) {
//
//    /******statystyka*************/
//    //ObliczWielkosciZakresow(listaDoRozdzielenia);
//    int nrKom = 0;
//    int licznik[6];
//     //potrzebna jest tablica, która zawiera iteratory na wskazujące na początek danych każdej komórki(strefy przestrzennej)
//    //uzupełnienie tej tablicy
//    printf("\nPrzydzielDoStrefPrzestrzennych poczatek");
//    iteratorLG it, it1;
//    it = listaDoRozdzielenia.begin();
//    it1 = it;
//    //tylko na początku
//    if (lGwiazdyWkomorkach.empty()) {
//        //ponieważ czymś trzeba wypełnić powyższą listę, to wypełniamy ją iteratorami kolejnych gwiazd z głównej listy
//        for (int i = 0; i < ileKomorek - 1; i++)lGwiazdyWkomorkach.push_back(it1++);
//        iteratorLitLG itlg = lGwiazdyWkomorkach.begin();
//        //a tablicę iteratorami do powyższej listy
//        for (int i = 0; i < ileKomorek; i++)poczatkiKomorek[i] = itlg++; //teoretycznie na ostatnim miejscu powinien być lGwiazdyWkomorkach.end()
//        printf("\nPrzydzielDoStrefPrzestrzennych wypelniono tablice poczatkiKomorek[%d]",ileKomorek);
//    } else
//        printf("\nPrzydzielDoStrefPrzestrzennych podczas zwrotu do bazy");
//
//    //pętla oczywiście po całej liście lGwiazdy
//    while (it != listaDoRozdzielenia.end()) {
//        float M = it->JM();
//        float odl = it->Odleglosc();
//        int hip = it->Hip();
//
//        //printf("\nhip %d, M %2.2f, odl %2.2f",hip,M,odl);
//        if (M < granice[0] || odl > dPodst) {
//            //do największej strefy
//            //iSp[hip]=0;
//            nrKom = 0;
//            licznik[0]++;
//        } else {
//            double x, y, z;
//            int ix, iy, iz, iK; //d - długość boku komórki, ile komórek w boku o długości podstawowej lś, pS - pozycja Strefy
//            x = (double) odl * it->X();
//            y = (double) odl * it->Y();
//            z = (double) odl * it->Z();
//
//            bool sprawdzaj = true;
//            int ktoraGranica = 1;
//            int ktoryPrzedzial = 0;
//            //int wykladnik=0;
////            while (sprawdzaj || ktoraGranica < ileGranic) {
//            while (sprawdzaj && ktoraGranica < ileGranic) {
//
//                //d=(dPodst/pow(dzielnik,wykladnik));//2000/1, 2, 4,
//                if (M >= granice[ktoraGranica - 1] && M < granice[ktoraGranica]) {
//                    sprawdzaj = false;
//                    licznik[ktoraGranica];
//                    ktoryPrzedzial = ktoraGranica - 1; //do przedostaniego elementu
//                }
//                //pS+=pow(2*pow(dzielnik,wykladnik),3);//1+8+64,co wstawić zamiast 8?
//                //                printf(" granica %d, wykladnik %d",ktoraGranica,wykladnik);
//                ktoraGranica++;
//                //wykladnik++;
//            }
//
//            if (M >= granice[ileGranic - 1]) {//ostatnia granica
//                ktoryPrzedzial = ileGranic - 1;
//                //d = dPodst / pow(dzielnik,ileGranic-1);//wykladnikiem potęgi jest numer ostatniej komórki tablicy
//                //pS = 1 + 8 + 64 + 512;
//
//                licznik[4]++;
//            }
//            iK = dPodst / d[ktoryPrzedzial];
//            ix = iK + round(x / d[ktoryPrzedzial]);
//            iy = iK + round(y / d[ktoryPrzedzial]);
//            iz = iK + round(z / d[ktoryPrzedzial]);
//
//            nrKom = (pS[ktoryPrzedzial] + iz * iK * 2 * iK * 2 + iy * iK * 2 + ix); //numer komórki do której należy dodać daną gwiazdę
//            //            if(licznik[5]<100){
//            //                printf("\nM %2.2f, d %d, pS %d",M,d,pS);
//            //            }
//
//            //printf("nrKom %d",nrKom);
//        }
//        //sięgamy do odpowieniej tabeli,ażeby znaleźć miejsce gdzie wstawić 
//        iteratorLitLG gdzieWstawic = poczatkiKomorek[nrKom + 1]; //bo wstawiamy przed początkiem następnej komórki
//        //w aktualnej Gwieździe umieszczamy iterator listy iteratorow, zwrócony przez funkcję insert, aby można go było usunąć zlisty właśnie z poziomu gwiazdy
//        it->UstawItWliscieKomorek(lGwiazdyWkomorkach.insert(gdzieWstawic, it));
//        it++;
//        licznik[5]++;
//        //               printf("\nsprawdzono 200 000 pozycji i przerwano");
//        //               it=lGwiazdy.end();
//        //           }
//    }
//}
//int PodzialNaKomorki::KtorePunktyWidacNowe(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip) {
//    int nrKom[ileZakresowWidac]; //numery komórek z kolejnych poziomów dokładności
//    //int d; //długość boków komórki kolejnych poziomów dokładności w jednostkach lata świetlne
//    //int pS = 0; //numer od którego zaczynają się komórki danego poziomu
//    int ix, iy, iz; //która komórka w osi x(y,z) licząc od najdalej wysuniętej w stronę ujemnych do dodatnich
//    int iK; // ile jest komórek danego poziomu w boku o długości podstawowej dPodst
//    
//     //jeżeli punkt widzenia jest poza sześcianem o długości boku dPodst przerywamy pętlę i nie dodajemy gwiazd dla mniejszych komórek
//    bool pozaZakresem=((100 * p[0] > dPodst) || (100 * p[1] > dPodst) || (100 * p[2] > dPodst));
//    for (int i = 0; i < ileZakresowWidac; i++) {
//        int ktoryZakres=(int)zasiegWidzenia[i];
//        if(ktoryZakres==0){
//            nrKom[i]=0;
//        }else{
//            //d = dPodst / pow(2, i); //16000 podzielić przez, kolejno 1,2,4,8, czyli 16000 8000 4000 2000
//            iK = dPodst / d[ktoryZakres]; //kolejno 1 2 4 8 
//            //pS += pow(8, i); //czyli kolejno 1+8+64+512
//            ix = iK + round(100 * p[0] / d[ktoryZakres]);
//            //printf("\n(100*p[0]/d) %2.5f, round(100*p[0]/d) %2.5f",(100*p[0]/d),round(100*p[0]/d));
//            iy = iK + round(100 * p[1] / d[ktoryZakres]);
//
//            iz = iK + round(100 * p[2] / d[ktoryZakres]);
//
//            nrKom[i] = (pS[ktoryZakres] + iz * iK * iK * 4 + iy * iK * 2 + ix);
//        }
//        printf("\n ktoryZakres %d, nrKom[ ] %d",ktoryZakres,nrKom[ktoryZakres]);
//    }
//
//    //przed uzupełnieniem listy, trzeba ją wyzerować
//    //a może nie trzeba? bo wcześniej jest zerowana
//    //punktyWszystkichGwiazd.clear();
//    //znając numery komórek możemy dowiedzieć się, które gwiazdy do nich należą
//    //lGwiazdyWkomorkach;//zawierać będzie iteratory do wyszystkich gwiazd ułożonych kolejno komórkami
//    int ostatniZakres=ileZakresowWidac;
////    int ostatniZakres=ileGranic;
//    int i=0;//pierwszy zakres
//    for (i; i <=ostatniZakres ; i++) {//kolejne i oznaczają numery zakresów z tablicy zasiegWidzenia (float)
////        
////        if(pozaZakresem && nrKom[i]>0)break;
////        if(pozaZakresem && nrKom[i]==0)ostatniZakres=i-1;//w tym przypadku ma tylko raz przejść pętlę
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
//
//    }
//}

//int PodzialNaKomorki::KtorePunktyWidacZapas(float * p, std::deque<double>& punktyWszystkichGwiazd, std::list<int>& listaHip) {
//    int nrKom[ileGranic + 1]; //numery komórek z kolejnych poziomów dokładności
//    //int d; //długość boków komórki kolejnych poziomów dokładności w jednostkach lata świetlne
//    //int pS = 0; //numer od którego zaczynają się komórki danego poziomu
//    int ix, iy, iz; //która komórka w osi x(y,z) licząc od najdalej wysuniętej w stronę ujemnych do dodatnich
//    int iK; // ile jest komórek danego poziomu w boku o długości podstawowej dPodst
//
//    nrKom[0] = 0;
//
//
//
//    for (int i = 0; i < ileGranic; i++) {
//        //d = dPodst / pow(2, i); //16000 podzielić przez, kolejno 1,2,4,8, czyli 16000 8000 4000 2000
//        iK = dPodst / d[i]; //kolejno 1 2 4 8 
//        //pS += pow(8, i); //czyli kolejno 1+8+64+512
//        ix = iK + round(100 * p[0] / d[i]);
//        //printf("\n(100*p[0]/d) %2.5f, round(100*p[0]/d) %2.5f",(100*p[0]/d),round(100*p[0]/d));
//        iy = iK + round(100 * p[1] / d[i]);
//
//        iz = iK + round(100 * p[2] / d[i]);
//
//        nrKom[i + 1] = (pS[i] + iz * iK * iK * 4 + iy * iK * 2 + ix);
//        
//    }
//
//    //przed uzupełnieniem listy, trzeba ją wyzerować
//    //a może nie trzeba? bo wcześniej jest zerowana
//    //punktyWszystkichGwiazd.clear();
//    //znając numery komórek możemy dowiedzieć się, które gwiazdy do nich należą
//    //lGwiazdyWkomorkach;//zawierać będzie iteratory do wyszystkich gwiazd ułożonych kolejno komórkami
//    for (int i = 0; i <= ileGranic; i++) {//jeśli od 1 to pomijamy strefę największych gwiazd
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
//        //jeżeli punkt widzenia jest poza sześcianem o długości boku dPodst przerywamy pętlę i nie dodajemy gwiazd dla mniejszych komórek
//        if ((100 * p[0] > dPodst) || (100 * p[1] > dPodst) || (100 * p[2] > dPodst))break;
//
//    }
//}


//void PodzialNaKomorki::PrzydzielDoStref(std::list<Gwiazda2>& listaDoRozdzielenia){
//    //utworzenie tablicy o wielkości równej ilości wszystkich komórek
//    
//    poczatkiKomorek=new iteratorLitLG[ileKomorek];
//    iteratorLG it, it1;
//    it = listaDoRozdzielenia.begin();
//    it1=it;
//   if (lGwiazdyWkomorkach.empty()) {
//        //ponieważ czymś trzeba wypełnić powyższą listę, to wypełniamy ją iteratorami kolejnych gwiazd z głównej listy
//        for (int i = 0; i < ileKomorek-1; i++)lGwiazdyWkomorkach.push_back(it1++);
//        iteratorLitLG itlg = lGwiazdyWkomorkach.begin();
//        //a tablicę iteratorami do powyższej listy
//        for (int i = 0; i < ileKomorek; i++)poczatkiKomorek[i] = itlg++; //teoretycznie na ostatnim miejscu powinien być lGwiazdyWkomorkach.end()
//        printf("\nPrzydzielDoStrefPrzestrzennych wypelniono tablice poczatkiKomorek[%d]",ileKomorek);
//    }else
//        printf("\nPrzydzielDoStrefPrzestrzennych podczas zwrotu do bazy");
//    int licznik[ilePoziomow+1];
//    int bledy=0;
//    //wyzerowanie licznika
//    for(int i=0;i<=ilePoziomow;i++)licznik[i]=0;
//    while(it != listaDoRozdzielenia.end()){
//        
//        int nrKom = 0;
//        
//        bool dopasowane=0;
//        float M = it->JM();
//        float odl = it->Odleglosc();
//        
//        if(M<=graniczneWielkosci[ilePoziomow-1] || odl>=dMaks){
//            nrKom=ileKomorek-2;//w przedostatniej komórce
//            dopasowane=1;
//            licznik[ilePoziomow]++;
//            //printf("granice %2.2f <= %2.2f",M,graniczneWielkosci[ilePoziomow-1]);
//            //nrKom jest już ustawione na zero
//        }
//        if(!dopasowane){
//            
//            int ix, iy, iz, d, iK, pozycjaPoczatkowaZakresu; //d - długość boku komórki, ile komórek w boku o długości maksymalnej w lś, 
//            
//            d=dPodst;
//            pozycjaPoczatkowaZakresu=0;
//            double x, y, z;
//                x = (double) odl * it->X();
//                y = (double) odl * it->Y();
//                z = (double) odl * it->Z();
//
//        //przedział dla najmniejszych gwiazd, więc długością boku komórki jest długość podstawowa
//            if(M>graniczneWielkosci[0]){
//                //d=500
//                //pozycjaKomorki=1
//                
//                dopasowane=1;
//                licznik[0]++;
//                //printf("granice %2.2f > %2.2f",M,graniczneWielkosci[0]);
//            }
//            //przedział pośredni
//            int pierwszaGranica=0;
//            int drugaGranica=1;
//            int wykladnik=1;
//            while(!dopasowane || drugaGranica!=ilePoziomow){
//                //pozycjaKomorki jest zwiększana o liczbę wszystkich sześcianów o boku długości d, które mieszczą się w sześcianie o długości boku 2*dMaks
//                //d=500
//                //pozycjaKomorki=(8000/500)^3=4096
//                
//                pozycjaPoczatkowaZakresu+=pow(2*dMaks/d,3);//4096+512+64
//                //printf("\n while: pozycjaPoczatkowaZakresu %d",pozycjaPoczatkowaZakresu);
//               d=dPodst*pow(mnoznik,wykladnik++);//1000,2000,4000
//               //printf(" zmiana d %d",d);
//                   //d=500*2^1=1000
//                  
//               if(graniczneWielkosci[pierwszaGranica]>=M && M>graniczneWielkosci[drugaGranica]){
//                   dopasowane=1;
//                   licznik[drugaGranica]++;
//                   //printf("granice %2.2f >= %2.2f > %2.2f",graniczneWielkosci[pierwszaGranica],M,graniczneWielkosci[drugaGranica]);
//               }
//               pierwszaGranica++;
//               drugaGranica++;//1,2,3
//
//            } 
//            //przedział dla największych gwiazd i najdalszych
//
//
//            //po przyporządkowaniu właściwej wielkości komórki znajdujemy lokalizację komórki
//            iK = dMaks / d;
//            ix = iK + round(x / d);
//            iy = iK + round(y / d);
//            iz = iK + round(z / d);
//            
//            nrKom = (pozycjaPoczatkowaZakresu + iz * (iK*2)*(iK*2) + iy * iK * 2 + ix); //numer komórki do której należy dodać daną gwiazdę
//            //printf("\n x %2.3f, y %2.3f, z %2.3f, d %d, M %2.2f",x,y,z,d,M);
//            //printf("\n ix %d, iy %d, iz %d",ix,iy,iz);
////            printf("\nd %d, iK %d, pozPoczatkowaZakresu %d, nrKom %d",d,iK,pozycjaPoczatkowaZakresu,nrKom);
//        }
//        if(nrKom>=ileKomorek-1){
//            bledy++;
//            it++;
//            printf("\nnrKom %d",nrKom);
//            continue;
//        }
//        //sięgamy do odpowieniej tabeli,ażeby znaleźć miejsce gdzie wstawić 
//        iteratorLitLG gdzieWstawic = poczatkiKomorek[nrKom + 1]; //bo wstawiamy przed początkiem następnej komórki
//        //w aktualnej Gwieździe umieszczamy iterator listy iteratorow, zwrócony przez funkcję insert, aby można go było usunąć zlisty właśnie z poziomu gwiazdy
//        
//        it->UstawItWliscieKomorek(lGwiazdyWkomorkach.insert(gdzieWstawic, it));
//        it++;
//    }
//    printf("\nPrzydzielDostref ilosc gwiazd w poszczegolnych poziomach:");
//    for(int i=0;i<=ilePoziomow;i++)printf(" p.%d: %d,",i,licznik[i]);
//    printf(" bledy %d",bledy);
//    
//}
