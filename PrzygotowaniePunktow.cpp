/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PrzygotowaniePunktow.cpp
 * Author: BCO
 * 
 * Created on 11 kwietnia 2016, 10:06
 */

#include "PrzygotowaniePunktow.h"

PrzygotowaniePunktow::PrzygotowaniePunktow():coPokazac(LINIE|NA_SFERZE|W_PRZESTRZENI|ETYKIETY) {
    wspKam=pow(2*10,0.25);// w drodze doświadczenia ustalono, - 10 zamienić później na 
        wK[0]=0.2;
        wK[1]=0.2;
        wK[2]=-1.8;
        //ustawienie tablicy kolorów, gdyby nie udało się odczytać
        for(int i=0;i<128;i++){
            r[i]=i/127.0;
            g[i]=i/127.0;
            b[i]=i/127.0;
        }
}

PrzygotowaniePunktow::PrzygotowaniePunktow(const PrzygotowaniePunktow& orig) {
}

PrzygotowaniePunktow::~PrzygotowaniePunktow() {

}
void PrzygotowaniePunktow::PrzygotujPunktyDoWyswietlenia(int CoDoZmiany) {
    std::list<int> listaHip;
    //listaItLG iteratoryNajwiekszych;
    if (CoDoZmiany & WSZYSTKIE_W_PRZESTRZENI) {
        //najpierw czyścimy
        punktyWszystkichGwiazd.clear();
        iteratoryWszystkichDoWyswietlenia.clear();
        //i jeżeli ma być widoczne, przypisujemy nową wartość
        if (coPokazac & WSZYSTKIE_W_PRZESTRZENI){
            oBaza->KtorePunktyWidac(wK, punktyWszystkichGwiazd, listaHip,iteratoryWszystkichDoWyswietlenia);
        }
         //funkcja KtrorePunktyWidac powinno zwrócić jakąś listę, lub w inny sposób przekazać dane do przygotowania Nazw do wyświetlenia
	}
    //ponieważ poniższe metody generalnie dodają nowe punkty, to trzeba wprowadzić warunek, że jeśli kontenery nie są puste to nie robią
    // pozostałe w pętlach, bo gwiazdozbiorów może być kilka
    int ileGZ = voGwiazdozbior.size();
    if (CoDoZmiany & LINIE) {
        punktyLinii.clear();
        if (coPokazac & LINIE)for (int i = 0; i < ileGZ; i++)(voGwiazdozbior.at(i)->ListaWspolrzednychParPunktow(punktyLinii));
    }
    if (CoDoZmiany & NA_SFERZE) {
        punktyGwiazdNaSferze.clear();
        if (coPokazac & NA_SFERZE)for (int i = 0; i < ileGZ; i++)(voGwiazdozbior.at(i)->ListaWspolrzednych(0, punktyGwiazdNaSferze));
    }
    if (CoDoZmiany & W_PRZESTRZENI) {
        punktyGwiazdWprzestrzeni.clear();
        iteratoryWybranychDoWyswietlenia.clear();
        if (coPokazac & W_PRZESTRZENI){
            for (int i = 0; i < ileGZ; i++)
            voGwiazdozbior.at(i)->ListaWspolrzednychZOdleglosciami(punktyGwiazdWprzestrzeni,iteratoryWybranychDoWyswietlenia);
        }
             
    }
    //CoDoZmiany - informuje w której części listy należy przeprowadzić ingerencję
    /*w tym miejscu można ustawić listę np 20 największych gwiazd z tych , które są przeznaczone do wyświetlenia na ekranie.
     * vo.Gwiazdozbior udostepniaja takie listy, natomiat oBaza->KtorePunktyWidac należy dodać argument do przekazania do obiektu podział listę bądź tablicę z
     * iteratorami gwiazd. Zbiory te należy połączyć, posortować i pierwsze 20 używać w metodzie rysuj scenę
    */

    PrzygotujNazwyDoWyswietlania(CoDoZmiany, listaHip);
}
void PrzygotowaniePunktow::PrzygotujNazwyDoWyswietlania(int CoZmieniono, std::list<int>& listaDoWstawienia) {
    tIdoListyHip[0] = hipWyswietlanychGwiazd.begin(); //przypisanie jest poźniej powtórzone, gdyby się okazało, że jest zmiana
    //jeżeli wszystkie gwiazdy nie są do wyświetlenia, to nie ma ich nazw, a iterator powinien wskazywać koniec listy
    tIdoListyHip[1] = hipWyswietlanychGwiazd.end();
    if ((CoZmieniono & WSZYSTKIE_W_PRZESTRZENI) && (coPokazac & WSZYSTKIE_W_PRZESTRZENI)) {
        if (listaDoWstawienia.empty())printf("\nPrzygotujNazwyDoWyswietlania brak numerów hip wszystkich gwiazd, pomimo zmiany WSZYSTKIE_W_PRZESTRZENI");
        else {
            int ileDodano=listaDoWstawienia.size();
            //na początek wykasować poprzednie hipy
            hipWyswietlanychGwiazd.erase(tIdoListyHip[1], hipWyswietlanychGwiazd.end());
            //otrzymaną listę trzeba wstawić na końcu 
            hipWyswietlanychGwiazd.insert(hipWyswietlanychGwiazd.end(),listaDoWstawienia.begin(),listaDoWstawienia.end());
            //i ustawić tIdoListyHip[1] na początek wskazanych danych
            tIdoListyHip[1]=hipWyswietlanychGwiazd.end();
            //poprzez dekrementację
            for(int i=0;i<ileDodano;i++)tIdoListyHip[1]--;
        }

    }
    //wstawić dane wszystkich gwiazd, bedą one na końcu
	//ponieważ tIdoListyHip[1] zawiera iterator do pierwszego elementu danych oBaza->ListaHip to przed nim mozemy wstawić
    //dane grupy wydzielonych gwiazdozbiorów
    if (CoZmieniono & (LINIE | NA_SFERZE | W_PRZESTRZENI)) {
        //zerujemy to, co dotyczy gwiazdozbiorów
        hipWyswietlanychGwiazd.erase(tIdoListyHip[0], tIdoListyHip[1]);
        for (int i = 0; i < voGwiazdozbior.size(); i++)voGwiazdozbior.at(i)->ListaHip(hipWyswietlanychGwiazd, tIdoListyHip[1]);
        tIdoListyHip[0] = hipWyswietlanychGwiazd.begin(); //dane pierwszej grupy zaczynają się od początku
    }
   
}

int PrzygotowaniePunktow::UsunZlistyPokazywanychGwiazdozbiorow(int nrGwiazdozbioru) {
    //na której pozycji znajduje się szukany numer?
    std::list<int>::iterator it=numeryUzywanychGwiazdozbiorow.begin();
    int ktoraPozycja=0;
    bool znaleziono=false;
    while(it!=numeryUzywanychGwiazdozbiorow.end()){
        if(nrGwiazdozbioru==*it){
            numeryUzywanychGwiazdozbiorow.erase(it);
            znaleziono=true;
			break;
        }
        ktoraPozycja++;
        it++;
    }
    if (znaleziono)return ktoraPozycja;
    return -1;
}

void PrzygotowaniePunktow::UstawTabliceKolorow(unsigned char * pKolor,int dlugosc){
    if(pKolor==0)return;
    for(int i=0;i<128;i++){
        //długość tablicy może być większa niż 128
        int ktory=i*(int)round((float)dlugosc/128.0);
        b[i]=(float)pKolor[ktory*3]/255.0;//kolory openGL od 0.0 do 1.0
        g[i]=(float)pKolor[1+ktory*3]/255.0;
        r[i]=(float)pKolor[2+ktory*3]/255.0;
        
    }
}

void PrzygotowaniePunktow::PokazEtykietyObiektow(listaItLG& listaNazw,bool * czyMoznaWyswietlic,int coWyswietlic){
    iteratorLitLG itNajwiekszych=listaNazw.begin();
    int rozmiarItNajwiekszych=listaNazw.size();
    int ileWyswietlac=rozmiarItNajwiekszych;
    int licznik=0;
    //równomierne rozmieszczenie napisów
    //polega na tym, że ekran dzielimy na przylegające prostokąty, jeżeli w danym prostokącie jest już napis, to kolejnego napisu w nim nie wyświetlamy
    //na początek siatka 10x10
    
     float pozycjaNapisu[3];//w trzech wymiarach
    double pozycjaNaEkranie[3];
    int ktoraPoziomo,ktoraPionowo,pozycjaWtablicy,kolorDuzych;
    
    
    while(itNajwiekszych!=listaNazw.end()){
        pozycjaNapisu[0]=(*itNajwiekszych)->OZ()/100;
        pozycjaNapisu[1]=(*itNajwiekszych)->OY()/100;
        pozycjaNapisu[2]=(*itNajwiekszych)->OX()/100;
        gluProject((double)pozycjaNapisu[0],(double)pozycjaNapisu[1],(double)pozycjaNapisu[2],
                macierzModelWidok,macierzRzutowania,viewport,&pozycjaNaEkranie[0],&pozycjaNaEkranie[1],&pozycjaNaEkranie[2]);
        //w której komórce się znajduje? sprawdzamy od lewego dolnego rogu najpierw x potem y
        ktoraPoziomo=(int)round((pozycjaNaEkranie[0]*komorkiWpoziomie)/(pozycjaNaEkranie[2]*viewport[2]));
        ktoraPionowo=(int)round((pozycjaNaEkranie[1]*komorkiWpionie)/(pozycjaNaEkranie[2]*viewport[3]));
        //jeżeli poza ekranem to nie wyswietlać
        if(ktoraPoziomo<0 || ktoraPoziomo>=komorkiWpoziomie || ktoraPionowo<0 || ktoraPionowo>=komorkiWpionie){
            itNajwiekszych++;
            continue;
        }
        pozycjaWtablicy=ktoraPionowo*komorkiWpoziomie+ktoraPoziomo;
        if(pozycjaWtablicy>=(komorkiWpionie*komorkiWpoziomie)){
            printf("\nPrzygPktRysujScene nieprawidlowo obliczone miejsce w tablicy %d, %d, %d",ktoraPoziomo,ktoraPionowo,pozycjaWtablicy);
            itNajwiekszych++;
            continue;
        }
        if(!czyMoznaWyswietlic[pozycjaWtablicy]){
            itNajwiekszych++;
            continue;
        }
        kolorDuzych=(*itNajwiekszych)->B_v();
        if(kolorDuzych>127)kolorDuzych=127;
        glColor3f(r[kolorDuzych],g[kolorDuzych],b[kolorDuzych]);
        glRasterPos3f(pozycjaNapisu[0],pozycjaNapisu[1],pozycjaNapisu[2]);
        std::string etykieta;
        etykieta=(*itNajwiekszych)->Nazwa();
        if(coWyswietlic & SYMBOL)etykieta+=" "+(*itNajwiekszych)->Symbol();
        if(etykieta.empty() && (coWyswietlic & HIP))etykieta=(*itNajwiekszych)->Hip();
        Wyswietlaj(czcionkaBitmapowa,32,"%s",etykieta.c_str());
        czyMoznaWyswietlic[pozycjaWtablicy]=false;
    //ilość wyświetlanych powinna zmieniać się w zależności od powiększenia
        if(licznik++>ileWyswietlac)break;
        itNajwiekszych++;
    }     
}
void PrzygotowaniePunktow::PrzygPktRysujScene(){
    
    std::list<int>::iterator itn[4];
    for (int k = 0; k < 3; k++)itn[k] = tIdoListyHip[0]; //trzy wskazują to samo, ale będą inkrementowane oddzielnie
    itn[3] = tIdoListyHip[1]; //wkazuje na hipy z bazy
    //gwiazdy w przestrzeni
    glInitNames();

    //oznaczenie grupy nazw w przestrzeni
    glPushName(1);

    //int identyfikator = 10; //może zmienimy na 100, jak będzie trzeba więcej nazw porządkujących
    int i = 0;
    float wielkosc = 1;
    cX = 0;
    cY = 0;
    cZ = 0;

    glLoadName(15); //od tego miejsca powinna zaczynać się podgrupa nazw
    glPushName(14);
    int idGwP = 0; //identyfikator gwiazd w przestrzeni

    
    while (i < punktyGwiazdWprzestrzeni.size()) {
        idGwP = *itn[0]++; //iterator do odpowiedniego miejsca na liście z aktualnymi hipami gwiazd
        int nrKoloru=(int)punktyGwiazdWprzestrzeni[i++];
        glColor3f(r[nrKoloru],g[nrKoloru],b[nrKoloru]);
        wielkosc = (float) (punktyGwiazdWprzestrzeni[i++]) / wspKam;
        glPointSize(wielkosc);
        glLoadName(idGwP);

        if (idGwP == idWybranegoPunktu) {
            //dzięki temu można centrować na wybraną gwiazdę
            cZ = (float) -punktyGwiazdWprzestrzeni[i++]; //nie wiadomo, dlaczego trzeba zamienić x-z
            cY = (float) -punktyGwiazdWprzestrzeni[i++];
            cX = (float) -punktyGwiazdWprzestrzeni[i--];
            i--;
            glColor3f(0.85, 0.05, 0.75);
        }
        glBegin(GL_POINTS);
        glVertex3d(punktyGwiazdWprzestrzeni[i++], punktyGwiazdWprzestrzeni[i++], punktyGwiazdWprzestrzeni[i++]);
        glEnd();
        glColor3f(1.0, 1.0, 0.8);
    }
    glPopName();

    //gwiazdy na sferze
    glColor3f(0.45, 0.75, 0.45);
    i = 0;
    wielkosc = 1;

    glLoadName(13);
    glPushName(12);
    int idGnS = 0; //identyfikator gwiazd na sferze

    while (i < punktyGwiazdNaSferze.size()) {
        wielkosc = (float) (punktyGwiazdNaSferze[i++]) / wspKam;
        glPointSize(wielkosc);
        try {
            idGnS = *itn[1]++;
        } catch (std::exception& e) {
            printf("\nBlad iteratora Rysuj Scene %s", e.what());
        }
        glLoadName(idGnS);
        if (idGnS == idWybranegoPunktu) {
            cZ = (float) -punktyGwiazdNaSferze[i++]; //nie wiadomo, dlaczego trzeba zamienić x-z
            cY = (float) -punktyGwiazdNaSferze[i++];
            cX = (float) -punktyGwiazdNaSferze[i--];
            i--;
            
            glColor3f(0.95, 0.05, 0.15);
        }
        glBegin(GL_POINTS);
        glVertex3d(punktyGwiazdNaSferze[i++], punktyGwiazdNaSferze[i++], punktyGwiazdNaSferze[i++]);
        glEnd();
        if (idGnS == idWybranegoPunktu)glColor3f(0.45, 0.75, 0.45);
    }

    glPopName();

    //linie łączące gwiazdy
    glLoadName(11);
    glPushName(10);
    int idL = 0; //identyfikator Linii


    glLineWidth(1.0);
    glColor3f(0.3, 0.3, 0.45);
    glBegin(GL_LINES);
    int j = 0;
    while (j < punktyLinii.size()) {
        j++; //wielkość punktu nie potrzebna
        idL = *itn[2]++;
        glLoadName(idL);
        glVertex3d(punktyLinii[j++], punktyLinii[j++], punktyLinii[j++]);
        j++; //powtarzamy, żeby jedna nazwa była do całej linii
        glVertex3d(punktyLinii[j++], punktyLinii[j++], punktyLinii[j++]);
    }
    glEnd();
    glPopName();

    //wszystkie gwiazdy w przestrzeni
    glLoadName(9);
    glPushName(8);
    int idWg = 0; //identyfikator wszystkich gwiazd w przestrzeni

    i = 0;
    while (i < punktyWszystkichGwiazd.size()) {
        int nrKoloru=(int)punktyWszystkichGwiazd[i++];
        glColor3f(0.8*r[nrKoloru],0.8*g[nrKoloru],0.8*b[nrKoloru]);
        wielkosc = (float) (punktyWszystkichGwiazd[i++]) / wspKam;
        glPointSize(wielkosc);
        idWg = *itn[3]++;
        glLoadName(idWg);
        if (idWg == idWybranegoPunktu) {
            cZ = (float) -punktyWszystkichGwiazd[i++];
            cY = (float) -punktyWszystkichGwiazd[i++];
            cX = (float) -punktyWszystkichGwiazd[i--];
            i--;
            glColor3f(0.05, 0.95, 0.05);
        }
        glBegin(GL_POINTS);
        glVertex3d(punktyWszystkichGwiazd[i++], punktyWszystkichGwiazd[i++], punktyWszystkichGwiazd[i++]);
        glEnd();
    }
    glPopName();
   glPopName();
    
    //nazwy największych gwiazd z wybranych gwiazdozbiorów
    //nazwy największych spośród wszystkich pozostałych widocznych
    if(coPokazac & ETYKIETY){
        glNormal3f(0,0,1);
        komorkiWpionie=10;
        komorkiWpoziomie=10;
        bool czyMoznaWyswietlic[komorkiWpionie*komorkiWpoziomie];
        for(int i=0;i<komorkiWpionie*komorkiWpoziomie;i++)czyMoznaWyswietlic[i]=true;
        PokazEtykietyObiektow(iteratoryWybranychDoWyswietlenia,czyMoznaWyswietlic,SYMBOL|HIP);
        PokazEtykietyObiektow(iteratoryWszystkichDoWyswietlenia,czyMoznaWyswietlic,SYMBOL);

    }
    

	//od tego miejsca powinna zaczynać się podgrupa nazw
    //parametry wybranej gwiazdy
    
    if(!dsParametryDoWyswietlenia.empty()){
        glPushMatrix();
        glLoadIdentity();
        if(iKolorTekstu>127)iKolorTekstu=127;//nie możemy przekraczać zakresu tablic r[],g[],b[]
        glColor3f(r[iKolorTekstu],g[iKolorTekstu],b[iKolorTekstu]);
        
        glNormal3f(0,0,1);
        std::deque<std::string>::iterator dIt=dsParametryDoWyswietlenia.begin();
        float fPozY=2.8;//2.8
        while(dIt!=dsParametryDoWyswietlenia.end()){
            
            glRasterPos3f(-5.0f,fPozY-=0.3,-10.0f);
            Wyswietlaj(czcionkaBitmapowa,32,"%s",(*dIt++).c_str());
        }
        glPopMatrix();
    }
    
}

