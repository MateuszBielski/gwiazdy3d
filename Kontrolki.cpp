/* 
 * File:   Kontrolki.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 6 lipca 2016, 11:39
 */



#include "Kontrolki.h"

Kontrolki::Kontrolki(const Kontrolki& orig) {
}

Kontrolki::~Kontrolki() {
}

bool Kontrolki::Utworz( WNDPROC procedura) {
    szerokoscEkranu=GetSystemMetrics(SM_CXSCREEN);
    wysokoscEkranu=GetSystemMetrics(SM_CYSCREEN);
    WNDCLASSEX wcDoKontrolek;
    char nazwaKlasyOknaDoKontrolek[] = "OknoDoKontrolek";

    wcDoKontrolek.cbSize = sizeof (wcDoKontrolek);
    wcDoKontrolek.style = CS_VREDRAW|CS_SAVEBITS; //styl okna CS_HREDRAW | 
    wcDoKontrolek.lpfnWndProc = procedura; //procedura okna
    wcDoKontrolek.cbClsExtra = 0; //dodatkowe bajty zarezerwowane za klas� okna
    wcDoKontrolek.cbWndExtra = 0; //dodatkowe bajty zarezerwowane za instancj� okna
    wcDoKontrolek.hInstance = uchwytAplikacji; //instancja aplikacji
    wcDoKontrolek.hIcon = NULL; //uchwyt ikony
    wcDoKontrolek.hIconSm = NULL; //uchwyt ikony
    wcDoKontrolek.hCursor = LoadCursor(NULL, IDC_ARROW); //uchwyt kursora
    wcDoKontrolek.hbrBackground = 0; //uchwyt p�dzla t�a
	wcDoKontrolek.lpszMenuName = NULL; //nazwa menu
    wcDoKontrolek.lpszClassName = nazwaKlasyOknaDoKontrolek; //nazwa klasy okna

    //rejestracja
    if (!RegisterClassEx(&wcDoKontrolek)) return 0;
    czcionkaZwykla=CreateFont(12,5,0,0,0,0,0,0,0,0,0,0,0,"Tahoma");
    czcionkaZwezona=CreateFont(12,3,0,0,0,0,0,0,0,0,0,0,0,"Tahoma");
    czcionkaGruba=CreateFont(12,5,0,0,700,0,0,0,0,0,0,0,0,"Tahoma");
	uOknaKontrolekG = CreateWindowEx(
            0,
            nazwaKlasyOknaDoKontrolek, //nazwa klasy okna
            "okno Kontrolek", //nazwa okna
            WS_CHILD, //styl okna
            margines, margines, //po�o�enie okna(x,y)
            szerokoscEkranu-2*margines,80+4*margines, //rozmiar okna (szeroko�c, wysoko��)
            uOknaRodzica, //uchwyt okna nadrz�dnego (parent)
            NULL, //uchwyt menu
            uchwytAplikacji, //uchwyt instancji aplikacji
            NULL//parametr komunikatu informuj�cego o utworzeniu okna
            );
    uOknaKontrolekD = CreateWindowEx(
            0,
            nazwaKlasyOknaDoKontrolek, //nazwa klasy okna
            "okno Kontrolek", //nazwa okna
            WS_CHILD, //styl okna
            margines,wys-margines-20, //po�o�enie okna(x,y)
            360,40, //rozmiar okna (szeroko�c, wysoko��)
            uOknaRodzica, //uchwyt okna nadrz�dnego (parent)
            NULL, //uchwyt menu
            uchwytAplikacji, //uchwyt instancji aplikacji
            NULL//parametr komunikatu informuj�cego o utworzeniu okna
            );
    if (uOknaKontrolekG == NULL ||uOknaKontrolekD == NULL) return false;
	
	//kontrolka
    InitCommonControls();

    ShowWindow(uOknaKontrolekG, SW_SHOW);
    ShowWindow(uOknaKontrolekD, SW_SHOW);
    zainicjowane=true;
    
    return true;
}

int Kontrolki::PrzyciskiZlisty(std::list<std::string>& lista) {
    if(!zainicjowane){
        printf("\nkontrolki nie zainicjowane");
        return 0;
    }
    std::list<std::string>::iterator it = lista.begin();
    int szerKontrolki = 80;
    int wysKontrolki = 20;
    int ktory = 0;
    while (it != lista.end()) {

        HWND hButton = CreateWindowEx(0
                , WC_BUTTON, (*it).c_str()
                , WS_CHILD, ktory * (szerKontrolki + odstep)
                , 0
                , szerKontrolki
                , wysKontrolki
                , uOknaKontrolekG, (HMENU) (1 + ktory)
                , uchwytAplikacji, 0
                );
        it++;
        if (hButton == NULL)continue;
        uchwytyKontrolek.push_back(hButton);
        ShowWindow(hButton, SW_SHOW);
        ktory++;
    }
    return ktory;
}
int Kontrolki::PrzyciskiZlisty(std::deque<Gwiazdozbior>& listaGwiazdozbiorow){//std::deque<Gwiazdozbior>& listaGwiazdozbiorow
    if(!uOknaKontrolekG)return 0;//gdyby inne okno próbowało wywołać utworzenie
    int ilePrzyciskow=listaGwiazdozbiorow.size();
    //ustalenie szerokosci przyciskow:
    int szerokoscOkna=(szerokoscEkranu-2*margines);
    int ileRzedow=4;
    int ilePrzyciskowWrzedzie=ilePrzyciskow/ileRzedow;
    int szerokoscPrzycisku=szerokoscOkna/ilePrzyciskowWrzedzie-margines;
    int wysokoscPrzycisku=20;
    printf("\nprzyciski gwiazdozbiorow, liczba przyciskow %d",ilePrzyciskow);
    int ktoryPrzycisk=0;
    int pozX=margines;
    int pozY=margines;
    std::deque<Gwiazdozbior>::iterator itGwZb=listaGwiazdozbiorow.begin();

    while(itGwZb!=listaGwiazdozbiorow.end()){
        //ustalenie pozycji
        int ktoryRzad=(int)(ileRzedow*(float)ktoryPrzycisk/ilePrzyciskow);
        int ktoraKolumna=ktoryPrzycisk%ilePrzyciskowWrzedzie;
        pozY=margines+(wysokoscPrzycisku+margines)*ktoryRzad;
        pozX=margines+(szerokoscPrzycisku+margines)*ktoraKolumna;
        char nazwa[itGwZb->Nazwa().length()]; 
        strcpy(nazwa,itGwZb->Nazwa().c_str());
        //próba konwersji na polskie znaki
        wchar_t wideChar[ 100 ];
        
        MultiByteToWideChar( CP_UTF8, 0, nazwa,-1, wideChar, 100 );
        HWND hButton = CreateWindowExW(WS_EX_APPWINDOW
                    , L"BUTTON" ,wideChar//L"BUTTON"   (LPCWSTR)WC_BUTTON
                    , WS_CHILD, pozX
                    , pozY
                    , szerokoscPrzycisku
                    , 20
                    , uOknaKontrolekG, (HMENU)(ktoryPrzycisk+1000)//itGwZb->KodPrzycisku()
                    , uchwytAplikacji, 0
                    );
         uchwytyKontrolek.push_back(hButton);
         HFONT hFont=czcionkaZwykla;
         if(itGwZb->Nazwa().length()>14)hFont=czcionkaZwezona;
         SendMessage(hButton,WM_SETFONT,(WPARAM)hFont,1);
         itGwZb++;
         ktoryPrzycisk++;
         if(hButton==0){
             printf("\ninna kontrolka, okno NIE utworzone");
             continue;
         }
     ShowWindow(hButton, SW_SHOW);
    }

     return 1;
}
int Kontrolki::PrzyciskiWgNumerowPolecen(std::list<int>& lista) {
    std::list<int>::iterator it = lista.begin();
    int szerKontrolki = 40;
    int wysKontrolki = 20;
    int ktory = 0;
    while (it != lista.end()) {
        std::ostringstream os; 
        os<<(*it); 
        HWND hButton = CreateWindowEx(0
                , WC_BUTTON, os.str().c_str()
                , WS_CHILD, ktory * (szerKontrolki + odstep)
                , 0
                , szerKontrolki
                , wysKontrolki
                , uOknaKontrolekG, (HMENU) (*it)
                , uchwytAplikacji, 0
                );
        it++;
        if (hButton == NULL)continue;
        uchwytyKontrolek.push_back(hButton);
        ShowWindow(hButton, SW_SHOW);
        ktory++;
    }
     return ktory;
}

bool Kontrolki::ZmienStanPrzycisku(int nrPrzycisku,bool zaznaczyc) {
    HFONT hFont=zaznaczyc?czcionkaGruba:czcionkaZwykla;
    if(SendMessage(uchwytyKontrolek.at(nrPrzycisku),WM_SETFONT,(WPARAM)hFont,1))return true;
    else return false;
}


void Kontrolki::OdswiezOkno(int ktore) {
    if(!czyWidoczne)return;
    switch(ktore){
        case WSZYSTKIE_KONTROLKI:
        InvalidateRect(uOknaKontrolekG, NULL, true);
        InvalidateRect(uOknaKontrolekD, NULL, true);
        break;
            
    }
}

void Kontrolki::PokazUkryj() {
    czyWidoczne=!czyWidoczne;
    int stanWidocznosci;
    stanWidocznosci=czyWidoczne?SW_SHOW:SW_HIDE; 
    ShowWindow(uOknaKontrolekG,stanWidocznosci);
    ShowWindow(uOknaKontrolekD,stanWidocznosci);
    OdswiezOkno(WSZYSTKIE_KONTROLKI);
}

void Kontrolki::PoleWyszukiwania() {
    uOknaWyszukiwania = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, "HIP lub nr Gwiazdozbioru"//WS_EX_CLIENTEDGE
            , WS_CHILD | WS_VISIBLE,0
            , 0
            , 165
            , 20
            , uOknaKontrolekD, 0
            , uchwytAplikacji, 0
            );
    uPrzyciskSzukaj = CreateWindowEx(0
                , WC_BUTTON,"szukaj HIP"
                , WS_CHILD,170
                , 0
                , 80
                , 20
                , uOknaKontrolekD,(HMENU)SZUKAJ
                , uchwytAplikacji, 0
            );
    
    uZaznaczGwiazdozbior = CreateWindowEx(0
                , WC_BUTTON,"Gwiazdozbior"
                , WS_CHILD,255
                , 0
                , 100
                , 20
                , uOknaKontrolekD,(HMENU)SZUKAJ_KONSTELACJI
                , uchwytAplikacji, 0
            );
    
    SendMessage(uOknaWyszukiwania,WM_SETFONT,(WPARAM)czcionkaZwykla,1);
    SendMessage(uPrzyciskSzukaj,WM_SETFONT,(WPARAM)czcionkaZwykla,1);
    SendMessage(uZaznaczGwiazdozbior,WM_SETFONT,(WPARAM)czcionkaZwykla,1);
    ShowWindow(uOknaWyszukiwania, SW_SHOW);
    ShowWindow(uPrzyciskSzukaj, SW_SHOW);
    ShowWindow(uZaznaczGwiazdozbior, SW_SHOW);
}

int Kontrolki::OdczytajWpisanyNumer() {
    char numer[7];
GetWindowText(uOknaWyszukiwania,numer,7);
return atoi(numer);
}

void Kontrolki::WypiszTekst(std::string& tekst) {
    SetWindowText(uOknaWyszukiwania,tekst.c_str());
}


