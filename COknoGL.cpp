/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "COknoGL.h"

LRESULT COkno::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            //zmiana rozmiaru okna
            RECT rect;
            GetClientRect(hWnd, &rect);
            szerokoscObszaruUzytkownika = rect.right - rect.left;
            wysokoscObszaruUzytkownika = rect.bottom - rect.top;
            //                 UstawienieSceny(0);
            break;

        default:
            //automatyczne przetwarzanie komunikatów
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return 0L;
}
//

bool COkno::Init(HINSTANCE uchwytAplikacji, POINT polozenieOkna, POINT rozmiarOkna) {
    /*************klasa okna**********************/
    char nazwaKlasyOknaGL[] = "OknoGL";
    char nazwaKlasyOknaPodstawowego[] = "OknoPodstawowe";

    uAplikacji = uchwytAplikacji;
    WNDCLASSEX wc, wcPodstawowe; //, wcDoKontrolek;

    wc.cbSize = sizeof (wc);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //styl okna
    wc.lpfnWndProc = (WNDPROC) ::ProceduraOknaGL; //procedura okna
    wc.cbClsExtra = 0; //dodatkowe bajty zarezerwowane za klas� okna
    wc.cbWndExtra = 0; //dodatkowe bajty zarezerwowane za instancj� okna
    wc.hInstance = uchwytAplikacji; //instancja aplikacji
    wc.hIcon = NULL; //uchwyt ikony
    wc.hIconSm = NULL; //uchwyt ikony
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); //uchwyt kursora
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND; //uchwyt p�dzla t�a
    wc.lpszMenuName = NULL; //nazwa menu
    wc.lpszClassName = nazwaKlasyOknaGL; //nazwa klasy okna

    wcPodstawowe.cbSize = sizeof (wcPodstawowe);
    wcPodstawowe.style = CS_HREDRAW | CS_VREDRAW; //styl okna
    wcPodstawowe.lpfnWndProc = (WNDPROC) ::WndProc; //procedura okna
    wcPodstawowe.cbClsExtra = 0; //dodatkowe bajty zarezerwowane za klas� okna
    wcPodstawowe.cbWndExtra = 0; //dodatkowe bajty zarezerwowane za instancj� okna
    wcPodstawowe.hInstance = uchwytAplikacji; //instancja aplikacji
    wcPodstawowe.hIcon = NULL; //uchwyt ikony
    wcPodstawowe.hIconSm = NULL; //uchwyt ikony
    wcPodstawowe.hCursor = LoadCursor(NULL, IDC_ARROW); //uchwyt kursora
    wcPodstawowe.hbrBackground = (HBRUSH) COLOR_BACKGROUND; //uchwyt p�dzla t�a
    wcPodstawowe.lpszMenuName = NULL; //nazwa menu
    wcPodstawowe.lpszClassName = nazwaKlasyOknaPodstawowego; //nazwa klasy okna

    //Rejestracja klas okna
    if (!RegisterClassEx(&wc) || !RegisterClassEx(&wcPodstawowe)) {
        //cout<<"rejestracja nie powiodla sie\n";                           
        return 0;
    }
    uOknaPodst = CreateWindowEx(
            0,
            nazwaKlasyOknaPodstawowego, //nazwa klasy okna
            "okno Podstawowe", //nazwa okna
            WS_OVERLAPPEDWINDOW, //styl okna
            polozenieOkna.x, polozenieOkna.y, //po�o�enie okna(x,y)
            rozmiarOkna.x, rozmiarOkna.y, //rozmiar okna (szeroko�c, wysoko��)
            NULL, //uchwyt okna nadrz�dnego (parent)
            NULL, //uchwyt menu
            uchwytAplikacji, //uchwyt instancji aplikacji
            NULL//parametr komunikatu informuj�cego o utworzeniu okna
            );
    if (uOknaPodst == NULL) return false;
    RECT rect;
    GetClientRect(uOknaPodst, &rect);
    int szerOpenGL = rect.right - rect.left;
    int wysOpenGL = rect.bottom - rect.top;
    uOkna1 = CreateWindowEx(
            0,
            nazwaKlasyOknaGL, //nazwa klasy okna
            "okno GL", //nazwa okna
            WS_CHILD, //styl okna
            0, 20, //po�o�enie okna(x,y)
            szerOpenGL, wysOpenGL, //rozmiar okna (szeroko�c, wysoko��)
            uOknaPodst, //uchwyt okna nadrz�dnego (parent)
            NULL, //uchwyt menu
            uchwytAplikacji, //uchwyt instancji aplikacji
            NULL//parametr komunikatu informuj�cego o utworzeniu okna
            );
    if (uOkna1 == NULL) return false;

    ShowWindow(uOknaPodst, SW_MINIMIZE);
    ShowWindow(uOkna1, SW_SHOW);
    kontrolki = new Kontrolki(uchwytAplikacji, uOkna1, szerOpenGL, wysOpenGL);
    //argumentem funkcji poniżej ma być wskaźnik do funkcji
    if (!kontrolki->Utworz((WNDPROC)::ProceduraOknaDoKontrolek)) {
        printf("\nNie utworzono kontrolek");
        return false;
    }
//    printf("\nprzed listaNazwPrzyciskow");
//    std::list<std::string> listaNazwPrzyciskow;
//    listaNazwPrzyciskow.push_back("Uran");
//    listaNazwPrzyciskow.push_back("Neptun");
//    listaNazwPrzyciskow.push_back("Pluton");
//    kontrolki->PrzyciskiZlisty(listaNazwPrzyciskow);
//    std::list<int> listaNumerowPolecen;
//    listaNumerowPolecen.push_back(1129);
//    listaNumerowPolecen.push_back(1187);
//    listaNumerowPolecen.push_back(1182);
//    listaNumerowPolecen.push_back(1137);
//    kontrolki->PrzyciskiWgNumerowPolecen(listaNumerowPolecen);
    
    kontrolki->PoleWyszukiwania();

    return true;

}

WPARAM COkno::Run() {

    //    Petla główna - obsługa komunikatów
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
    //    MSG msgKomunikat;
    //    msgKomunikat.message = WM_NULL;
    //    while (msgKomunikat.message != WM_QUIT) {
    //        if (PeekMessage(&msgKomunikat, NULL, 0, 0, PM_REMOVE)) {
    //            TranslateMessage(&msgKomunikat);
    //            DispatchMessage(&msgKomunikat);
    //        }
    //    }

}

bool COknoGL::UstalFormatPikseli(HDC uchwytDC) const {
    PIXELFORMATDESCRIPTOR opisFormatuPikseli;
    ZeroMemory(&opisFormatuPikseli, sizeof (opisFormatuPikseli));
    opisFormatuPikseli.nVersion = 1;
    //w oknie, podw�jne buforowanie
    opisFormatuPikseli.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    opisFormatuPikseli.iPixelType = PFD_TYPE_RGBA; //typ RGB
    opisFormatuPikseli.cColorBits = 32; //jako�� koloru 4 bajty
    opisFormatuPikseli.cDepthBits = 16; //g��boko�� bufora Z (z-buffer)
    opisFormatuPikseli.iLayerType = PFD_MAIN_PLANE;
    int formatPikseli = ChoosePixelFormat(uchwytDC, &opisFormatuPikseli);
    if (formatPikseli == 0) return false;
    if (!SetPixelFormat(uchwytDC, formatPikseli, &opisFormatuPikseli)) return false;
    return true;
}

bool COknoGL::InitWGL(HWND uchwytOkna) {
    uchwytDC = ::GetDC(uchwytOkna);
    //Utworzenie kontekstu renderowania i uczynienie go aktywnym
    if (!UstalFormatPikseli(uchwytDC)) return false;
    uchwytRC = wglCreateContext(uchwytDC);
    if (uchwytRC == NULL)return false;
    if (!wglMakeCurrent(uchwytDC, uchwytRC)) return false;
    return true;

}

void COknoGL::UsunWGL() {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(uchwytRC);
    ::ReleaseDC(uOkna1, uchwytDC);
}

LRESULT CALLBACK COknoGL::ProceduraZasadnicza(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    long wynik = COkno::WndProc(hWnd, message, wParam, lParam);
    //        
    switch (message) {
        case WM_SIZE:
            //Zmiana rozmiaru okna
            //ValidateRect(uOkna1, NULL);
            UstawienieSceny(0);
            MoveWindow(uOkna1, 0, 0, szerokoscObszaruUzytkownika, wysokoscObszaruUzytkownika, true);
            //MoveWindow(kontrolki->uOknaKontrolekD, 0, wysokoscObszaruUzytkownika-25, 360, 40, true);
            //SetWindowPos(kontrolki->uOknaKontrolekD,uOkna1,0,wysokoscObszaruUzytkownika-25,0,0,SWP_NOSIZE|SWP_NOZORDER);
            //            UpdateWindow(uOkna1);
            break;

        case WM_KEYDOWN:
        {
            //            ObliczeniaGwiazd * pTemp = 0;
            switch (wParam) {
                case 'Z'://pokazuje gwiazdozbiory wg wprowadzonych parametrów, ukrywając wcześniej wyświetlone
//                    wRG->WyswietlGwiazdozbiory(WPISANE_OD_NOWA);
                    WydzielGzZklawiatury();//przekazuje wykonanie do innego wątku
                    break;
                case 'X'://ma pokazać gwiazdozbiór wybrany przez wskazanie gwiazdy
                    wRG->WyswietlGwiazdozbiory(ZAZNACZONE_OD_NOWA);
                    break;
                case 'V'://nie ukrywając poprzednio wyswietlanych ma pokazać następny, żądany gwiazdozbiór
                    wRG->WyswietlGwiazdozbiory(ZAZNACZONE_DORYSUJ);
                    
                    break;
                case 'B':
                    wRG->WyswietlGwiazdozbiory(WPISANE_DORYSUJ);
                    break;
                case 'L':
                    wRG->PokazUkryj(LINIE);
                    break;
                case 'S':
                    wRG->PokazUkryj(NA_SFERZE);
                    break;
                case 'P':
                    wRG->PokazUkryj(W_PRZESTRZENI);
                    break;
                case 'W':
                    wRG->PokazUkryj(WSZYSTKIE_W_PRZESTRZENI);
                    break;
                case'E':
                    wRG->PokazUkryj(ETYKIETY);
                    break;
                case 'C':
                    wRG->UstawKameraXYZf(kameraX, kameraY, kameraZ);
                    wRG->PrzygotujPunkty(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
                    break;
                case 'N'://wyświetlanie najbliższych gwiazd
                    try{//może się zdarzyć, że zbiór najbliższych nie istnieje
                        wRG->WydzielWybraneGwiazdozbiory(196, 1); //nie dorysowuje do istniejących, bo nie ma i nie tworzy z zaznaczonej, bo nie zaznaczona
                        wRG->PrzygotujPunkty(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
                    }catch(std::exception &e){
                        printf("\nnie mozna pokazac najblizszych gwiazd");
                    }
                    //                    wRG->WyswietlGwiazdozbiory(196,1,WPISANE_OD_NOWA);
                    break;
                    //wRG->PokazNajblizsze();
                case 'F'://szukanie gwiazdy wg nr hip
                {
                    int wprowadzoneDane[3];
                    wprowadzoneDane[0] = 2;
                    //printf("\nwcisnieto F");
                    if (wRG->PobierzNrIloscZklawiatury(wprowadzoneDane, "numer HIP"))
                        wRG->ZaznaczGwiazde(kameraX, kameraY, kameraZ, wprowadzoneDane[1]);
                    wRG->UstawKameraXYZf(kameraX, kameraY, kameraZ);
                }
                    break;
                case 'K'://pokazanie i ukrycie kontrolek
                    kontrolki->PokazUkryj();
                   break;
                case 'R':
                    OdNowaPrzeliczaj();//tworzy nowy obiekt Rysuj Gwiazdy, pobiera dane i przelicza w nowym watku, użyteczne przy zmianie parametrów, czy innych plików
                    break;
                case'M'://widok ze środka sfery
                    
                    static float kameraNieZerowa[4];
                    if(kameraR==0){
                        kameraX=kameraNieZerowa[0];
                        kameraY=kameraNieZerowa[1];
                        kameraZ=kameraNieZerowa[2];
                        kameraR=kameraNieZerowa[3];
                        wRG->UstawWspolczynnikKamery(5.0);//wielkość punktów od razu się zmienia
                    }
                    else{
                        kameraNieZerowa[0]=kameraX;
                        kameraNieZerowa[1]=kameraY;
                        kameraNieZerowa[2]=kameraZ;
                        kameraNieZerowa[3]=kameraR;
                        kameraX=0.0;
                        kameraY=0.0;
                        kameraZ=0.0;
                        kameraR=0.0;
                        //skala=1.0;
                    }
                    break;
            }
        }
            RysujScene(0);
            break;

        case WM_MOUSEWHEEL:
            const float czuloscMyszy = 10.0f;
            short ZmianaPozycjiRolki = (short) HIWORD(wParam);
            //zmiana odleg�o�ci kamery od pocz�tku uk�adu wsp�rz�dnych
            kameraR *= 1.0f + ZmianaPozycjiRolki / abs(ZmianaPozycjiRolki) / czuloscMyszy;
            //jeśli patrzymy z centrum sfery, zmiana skali a nie odsuwanie od środka
            static double poprzedniNear;
            if(kameraR==0)nearDoKorektySceny *= 1.0f + ZmianaPozycjiRolki / abs(ZmianaPozycjiRolki) /5.0;
            nearDoKorektySceny=kameraR!=0?0.2:nearDoKorektySceny;
            if(poprzedniNear!=nearDoKorektySceny)KorektaSceny(nearDoKorektySceny);//wywoływanie tylko gdy jest zmiana 
            poprzedniNear=nearDoKorektySceny;
//            printf("\nskala %2.3f",skala);
            //float kameraRdoWspolczynnika=(kameraR!=0)?kameraR:5.0;//w przypadku patrzenia z centrum
//            wRG->UstawWspolczynnikKamery(kameraRdoWspolczynnika);
            wRG->UstawWspolczynnikKamery((kameraR!=0)?kameraR:2.0);
            wRG->PobierzUstawieniaMacierzy();
            //wspKam = pow(2 * kameraR, 0.25);
            //if(pauza) 
            //printf("\nkameraR %2.4f",kameraR);
            RysujScene(0);
            wynik = 0;
            break;
    }
    return wynik;
}

LRESULT CALLBACK COknoGL::ProceduraOknaGL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    long wynik = COkno::WndProc(hWnd, message, wParam, lParam);
    //    long wynik = 0;
    switch (message) {
        case WM_CREATE:
            //Utworzenie okna           
            InitWGL(hWnd);

            UstawWskaznikRG(RysujGwiazdy::WskaznikRysujGwiazdy());
//            wRG = RysujGwiazdy::WskaznikRysujGwiazdy();
            if (wRG == 0) {
                printf("\nWM_CREATE: nie zainicjowano RysujGwiazdy");
                return wynik;
            }
            
            //oBaza=wRG->OBaza();
            UstawienieSceny(0);
            unsigned short pierwszyZnak,ostatniZnak;
            pierwszyZnak=0xc582;//x82c5;
            ostatniZnak=0xc5b9;
            printf("\nPrzed stworz czcionke kody: %d %d",pierwszyZnak,ostatniZnak);
            wRG->CzcionkaBitmapowa(StworzCzcionke(false, uOkna1, "Verdana", 16, false, false,(long) pierwszyZnak,(long) ostatniZnak)); //Verdana
//            wRG->CzcionkaBitmapowa(StworzCzcionke(false, uOkna1, "Verdana", 16, false, false, 32, 255)); //Verdana
            wRG->WydzielWybraneGwiazdozbiory(129, 9); //nie dorysowuje do istniejących, bo nie ma i nie tworzy z zaznaczonej, bo nie zaznaczona
            wRG->PrzygotujPunkty(WSZYSTKIE_W_PRZESTRZENI | LINIE | NA_SFERZE | W_PRZESTRZENI);
            //            this->WydzielWybraneGwiazdozbiory(187, 2);

            break;
        case WM_DESTROY:
            UsunWGL();
            break;
        
        case WM_PAINT:
            //okno wymaga odrysowania
            RysujScene(0);
            //ValidateRect(hWnd, NULL);
            
            break;

        case WM_RBUTTONDOWN:
        {
            
            POINT pozycjaMyszy;
            pozycjaMyszy.x = LOWORD(lParam);
            pozycjaMyszy.y = HIWORD(lParam);
            try {
                wRG->ZaznaczGwiazde(kameraX, kameraY, kameraZ, WyoborPunktu(pozycjaMyszy));
            } catch (std::exception& e) {
                printf("\nnie zaznaczon gwiazdy z powodu bledu %s", e.what());
            }
            RysujScene(0);
            
        }
            break;
        case WM_LBUTTONDOWN:
            //printf("\naktywne okno %d",GetActiveWindow());
#ifdef ARCBALL
            MousePt.s.X = (GLfloat) LOWORD(lParam);
            MousePt.s.Y = (GLfloat) HIWORD(lParam);
            LastRot = ThisRot;
            ArcBall->click(&MousePt);
#endif
            poczatkowaPozycjaKursoraMyszy.x = LOWORD(lParam);
            poczatkowaPozycjaKursoraMyszy.y = HIWORD(lParam);
            wynik = 0;
            break;
        case WM_MBUTTONDOWN:
            poczatkowaPozycjaKursoraMyszy.x = LOWORD(lParam);
            poczatkowaPozycjaKursoraMyszy.y = HIWORD(lParam);
            TransformujPikselDoPrzestrzeniSceny(poczatkowaPozycjaKursoraMyszy, 1, poprzedniaPozycjaKursoraMyszy3D);
            wynik = 0;
            break;

            //                    
        case WM_MOUSEMOVE:
            if (wParam & (MK_LBUTTON | MK_MBUTTON)) {
                POINT biezacaPozycjaKursoraMyszy = {LOWORD(lParam), HIWORD(lParam)};
                POINT przesuniecieKursoraMyszy = {biezacaPozycjaKursoraMyszy.x - poczatkowaPozycjaKursoraMyszy.x,
                    biezacaPozycjaKursoraMyszy.y - poczatkowaPozycjaKursoraMyszy.y};
                if (przesuniecieKursoraMyszy.x == 0 && przesuniecieKursoraMyszy.y == 0) break;
                //obliczenie osi i k�ta obrotu
                if (wParam & (MK_LBUTTON)) {

                    //#ifdef ARCBALL
                    MousePt.s.X = (GLfloat) biezacaPozycjaKursoraMyszy.x;
                    MousePt.s.Y = (GLfloat) biezacaPozycjaKursoraMyszy.y;
                    Quat4fT ThisQuat;
                    //wskaźnik do funkcji (w przypadku spojrzenia z centrum sfery, obroty muszą być w drugą stronę)
                    pDrag wskaznikDrag=&ArcBall_t::drag;//przypisanie nazwy funkcji do wskaźnika
                    if(!kameraR)wskaznikDrag=&ArcBall_t::dragRev;//lub ewentualnie inna funkcja
                    (ArcBall->*wskaznikDrag)(&MousePt, &ThisQuat);
                    //użycie pierwotne ArcBall->drag(&MousePt, &ThisQuat);
                    Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
                    Matrix3fMulMatrix3f(&ThisRot, &LastRot);
                    Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
                }
                if (wParam & (MK_MBUTTON)) {
                    //metoda zakłada wyznaczenie w przestrzeni punktu
                    //wskazywanego przez mysz, punkt ten leży na płaszczyźnie równoległej do ekranu
                    float czulosc = 400 / kameraR;
                    float biezacaPozycjaKursoraMyszy3D[3];
                    TransformujPikselDoPrzestrzeniSceny(
                            biezacaPozycjaKursoraMyszy, 1,
                            biezacaPozycjaKursoraMyszy3D);
                    kameraX += (biezacaPozycjaKursoraMyszy3D[0] - poprzedniaPozycjaKursoraMyszy3D[0]) / czulosc;
                    kameraY += (biezacaPozycjaKursoraMyszy3D[1] - poprzedniaPozycjaKursoraMyszy3D[1]) / czulosc;
                    kameraZ += (biezacaPozycjaKursoraMyszy3D[2] - poprzedniaPozycjaKursoraMyszy3D[2]) / czulosc;
                    //                    
                    for (int i = 0; i < 3; i++) {
                        poprzedniaPozycjaKursoraMyszy3D[i] = biezacaPozycjaKursoraMyszy3D[i];
                    }
                }
                /*to jest miejsce, w którym należy dokonać korekty listy wyświetlanych punktów i ich nazw (identyfukatorów) z grupy wszystkie */
                //              //if(coPokazac & WSZYSTKIE_W_PRZESTRZENI)oBaza->KtorePunktyWidac(wK,punktyWszystkichGwiazd);//docelowo, powinno też pobierać listę punktów i nazw do aktualizacji


                wRG->PobierzPrzeksztalcenia(kameraX, kameraY, kameraZ, Transform);
                wRG->PobierzUstawieniaMacierzy();
                wRG->PrzygotujPunkty(WSZYSTKIE_W_PRZESTRZENI);
                RysujScene(0);//BEZ_ODSWIEZANIA_KONTROLEK
                //UpdateWindow(hButton);
            }
            //                        wynik=0;
            break;

    }
    return wynik;
}

LRESULT CALLBACK COknoGL::ProceduraOknaKontrolek(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    long wynik = COkno::WndProc(hWnd, message, wParam, lParam);
    std::string tekst;
    switch (message) {
        case WM_CREATE:
            kontrolki->PrzyciskiZlisty(wRG->ListaGwiazdozbiorow());//wRG->OBaza()->dGwiazdozbiorDoPrzycisku
            break;
        case WM_COMMAND:
            if(wParam>=1000 && wParam<1200){
                try{
                    int ilePozycji;//=(int)wParam/1000;
                    int nrPorzadkowy;//=(int)wParam-ilePozycji*1000;
                    int nrKomunikatu=(int)wParam-1000;
                    nrPorzadkowy=wRG->ListaGwiazdozbiorow().at(nrKomunikatu).NumerPorzadkowy();
                    ilePozycji=wRG->ListaGwiazdozbiorow().at(nrKomunikatu).IlePozycji();
                    printf("\n.at(nr) %d, nrPorz %d, ilePoz %d",nrKomunikatu,nrPorzadkowy,ilePozycji);
//                    
                    kontrolki->ZmienStanPrzycisku(nrKomunikatu,wRG->PokazUkryjGwiazdozbior(nrPorzadkowy,ilePozycji)); 
                }catch(std::exception &e) {
                    tekst = "nieprawidlowa wartosc";
                    tekst += e.what();
                    kontrolki->WypiszTekst(tekst);
                }
                    SetFocus(uOknaPodst);
            }
            switch (wParam) {
                case SZUKAJ:
                    try {
                        wRG->ZaznaczGwiazde(kameraX, kameraY, kameraZ, kontrolki->OdczytajWpisanyNumer());
                        wRG->UstawKameraXYZf(kameraX, kameraY, kameraZ);
                        tekst+"HIP";
                    } catch (std::exception &e) {
                        tekst = "nieprawidlowa wartosc";
                        tekst += e.what();
                    }
                    kontrolki->WypiszTekst(tekst);
                    SetFocus(uOknaPodst);
                    break;
                case SZUKAJ_KONSTELACJI:
                    try {
                        wRG->WyswietlGwiazdozbiorZprzyciskow(kontrolki->OdczytajWpisanyNumer());
                        tekst+"HIP";
                    } catch (std::exception &e) {
                        tekst = "nieprawidlowa wartosc";
                        tekst += e.what();
                    }
                    kontrolki->WypiszTekst(tekst);
                    SetFocus(uOknaPodst);
                    break;
            }

            RysujScene(0);
            break;
            //        case WM_KEYDOWN:
            //        
            //            //            ObliczeniaGwiazd * pTemp = 0;
            //            switch (wParam) {
            //                case 'Z'://pokazuje gwiazdozbiory wg wprowadzonych parametrów, ukrywając wcześniej wyświetlone
            //                    printf("\nwpisano Z");
            //                    break;
            //            }
            //            break;
        case WM_LBUTTONDOWN:
            //wyczyścić pole wpisywania
            tekst="";
            kontrolki->WypiszTekst(tekst);
            break;
        case WM_RBUTTONDOWN:
        {
            POINT pozycjaMyszy;
            pozycjaMyszy.x = LOWORD(lParam);
            pozycjaMyszy.y = HIWORD(lParam);
            try {
                wRG->ZaznaczGwiazde(kameraX, kameraY, kameraZ, WyoborPunktu(pozycjaMyszy));
            } catch (std::exception& e) {
                printf("\nnie zaznaczon gwiazdy z powodu bledu %s", e.what());
            }
        }
            RysujScene(0);
            break;
    }

    return wynik;
}

int COknoGL::WyoborPunktu(POINT pozycjaKursoraMyszy) {
    clock_t czas[5];
    czas[0] = clock();
    //przygotowanie bufora zaznaczenia
    const int rozmiarBuforaZaznaczenia = 1024;
    unsigned buforZaznaczenia[rozmiarBuforaZaznaczenia];
    ZeroMemory(buforZaznaczenia, rozmiarBuforaZaznaczenia);
    glSelectBuffer(rozmiarBuforaZaznaczenia, buforZaznaczenia);

    //przygotowanie promienia pod myszką 
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czy�ci bufory
    glLoadIdentity();
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPickMatrix(pozycjaKursoraMyszy.x, wysokoscObszaruUzytkownika - pozycjaKursoraMyszy.y, 8, 8, viewport);
    czas[1]=clock();
//    printf("\nwyborPunktu odczytane parametry:");
//    printf("\npozMyszy X: %d, viewport %d, %d, %d, %d,",(int)pozycjaKursoraMyszy.x,viewport[0],viewport[1],viewport[2],viewport[3]);
    float wsp = wysokoscObszaruUzytkownika / (float) szerokoscObszaruUzytkownika;
    if (1)
        glFrustum(-0.1, 0.1, wsp*-0.1, wsp * 0.1, nearDoKorektySceny, 400.0);
    else
        glOrtho(-3, 3, wsp*-3, wsp * 3, 0.3, 100.0);
    glMatrixMode(GL_MODELVIEW);
    //przełączanie w tryb selekcji i renderowanie sceny
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czy�ci bufory
    glRenderMode(GL_SELECT); //umieść znak komemtarza przed tym poleceniem, żeby zobaczyć co widzi myszka
    RysujScene(BEZ_SWAPBUFFERS);
    czas[2]=clock();
    //powrót do norlamlnego trybu renderowania
    int ileTrafien = glRenderMode(GL_RENDER);
    czas[3]=clock();
    printf("\nWyoborPunktu ileTrafien= %d, zawartosc bufora: \n ", ileTrafien);
    for (int j = 0; j < 5 * ileTrafien + 10; j++)printf(" %d,", buforZaznaczenia[j]);
    //przywracanie oryginalnej macierzy rzutowania
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    //interpretacja zawartosci bufora zaznaczenia
    int wynik=-1;
    if (ileTrafien > 0) {
        //        //zwracam obiekt najbliższy kamery
        unsigned indeksNajblizszegoPunktu = buforZaznaczenia[4];
        unsigned odlegloscNajblizszegoPunktu = buforZaznaczenia[1];
        int biezacyIndeks = 0;
        for (int i = 0; i < ileTrafien; i++) {
            if (buforZaznaczenia[biezacyIndeks + 1] > odlegloscNajblizszegoPunktu) {
                odlegloscNajblizszegoPunktu = buforZaznaczenia[biezacyIndeks + 1];
                if (buforZaznaczenia[biezacyIndeks] > 0)
                    indeksNajblizszegoPunktu = buforZaznaczenia[biezacyIndeks + 4];
            }
            biezacyIndeks += 5;
        }
        wynik=indeksNajblizszegoPunktu;

    }
    czas[4] = clock();
    long delta[5]; //= (long) (koniec - start);
    printf("\n czasy:");
    for(int i=0;i<4;i++){
        delta[i]=(long)(czas[i+1]-czas[i]);
        printf("\n%d. %d ms",i,delta[i]);
    }
    delta[4]=(long)(czas[4]-czas[0]);
    printf("\ncalosc %d ms",delta[4]);
    return wynik;
    
}

void COknoGL::UstawienieSceny(bool rzutowanieIzometryczne) //warto�� domy�lna =false
{ //okno OpenGL = wn�trze formy (domy�lnie)
    glViewport(0, 0, szerokoscObszaruUzytkownika, wysokoscObszaruUzytkownika);
#ifdef ARCBALL
    ArcBall->setBounds((float) szerokoscObszaruUzytkownika, (float) wysokoscObszaruUzytkownika);
#endif
    //ustawienie punktu projekcji
    glMatrixMode(GL_PROJECTION); //prze��cznie na macierz projekcji
    glLoadIdentity();

    float wsp = wysokoscObszaruUzytkownika / (float) szerokoscObszaruUzytkownika;
    if (!rzutowanieIzometryczne)

        //mno�enie macierzy rzutowania przez macierz perspektywy - ustalanie frustum
        glFrustum(-0.1, 0.1, wsp*-0.1, wsp * 0.1, nearDoKorektySceny, 400.0);
    else
        glOrtho(-3, 3, wsp*-3, wsp * 3, 0.3, 100.0);
    glMatrixMode(GL_MODELVIEW); //powr�t do macierzy widoku modelu
    glEnable(GL_DEPTH_TEST); //z-buffer aktywny = ukrywanie niewidocznych powierzchni
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);

    //    glDrawBuffer(GL_BACK_LEFT);                                      //obraz dla oka lewego
    //glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    //glFrustum(left+dx, right+dx, bottom, top, near, far);
    //glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //glTranslatef(EyeSep/2.0, 0.0, 0.0);


    // równanie mieszania kolorów
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //    GLfloat sizes[2];
    //    GLfloat step;
    //    glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
    //    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
    //    printf("\nzakres wielkości: %2.4f, %2.4f, krok %0.5f",sizes[0],sizes[1],step);
    wRG->PobierzUstawieniaMacierzy();

}
void COknoGL::KorektaSceny(double nearVal){
   // printf("\n farVal %2.3f",farVal);
    glMatrixMode(GL_PROJECTION); //prze��cznie na macierz projekcji
    glLoadIdentity();
    float wsp = wysokoscObszaruUzytkownika / (float) szerokoscObszaruUzytkownika;
    
    glFrustum(-0.1, 0.1, wsp*-0.1, wsp * 0.1, nearVal, 400.0);
    //glFrustum(-0.1, 0.1, wsp*-0.1, wsp * 0.1, 0.2, 400.0);
    glMatrixMode(GL_MODELVIEW);
}
float sinDf(double deg) {
    return (float) sin((double) 3.14159 * deg / 180.0f);
}

float cosDf(double deg) {
    return (float) cos((double) 3.14159 * deg / 180.0f);
}

void COknoGL::RysujScene(int wylaczenia) {
    if(!(wylaczenia & BEZ_ODSWIEZANIA_KONTROLEK))kontrolki->OdswiezOkno(WSZYSTKIE_KONTROLKI);

    //    //tworzenie czcionki bitmapowej
    //    static unsigned int czcionkaBitmapowa=NULL;
    //    if(czcionkaBitmapowa==NULL)czcionkaBitmapowa=StworzCzcionke(false,uOkna1,"Arial CE",10,true,false,32,255);
    //Przygotowanie bufora
    if(!(wylaczenia & BEZ_CLEARBUFFERS))glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czy�ci bufory
    glLoadIdentity(); //macierz model-widok = macierz jednostkowa
    glTranslatef(0.0, 0, -10);
    //wypisanie tekstu


    //    glPushMatrix();
#ifdef ARCBALL
    glMultMatrixf(Transform.M);
    //
#endif

    //osie układu:
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(1.5, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 1.5, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 1.5);
    glEnd();

    //glPopMatrix();
    glLoadIdentity();
#ifdef ARCBALL
    glTranslatef(0.0, 0, -kameraR);
    glMultMatrixf(Transform.M);
    glTranslatef(kameraX, kameraY, kameraZ);
   
    //glScalef(10.0,10.0,10.0);


#endif
    /*w celu przyspieszenia działania programu należy zredukować ilość wyswietlanych gwiazd
     należy określić współrzędne wektora od środka układu współrzędnych w kierunku dali prostopadle do ekranu
     * bedzie on decydował, które prostopdłościany - strefy będą widoczne - innych gwiazd nie bedzie w obliczeniach */
    //101';.sfera


    //        glColor3f(0.4, 0.3, 0.2);
    //        glPushMatrix();
    //        
    //        GLUquadricObj * kwadryka=gluNewQuadric();
    //        gluQuadricDrawStyle(kwadryka,GLU_FILL);
    //        gluSphere(kwadryka,0.25,100,100);
    //        gluDeleteQuadric(kwadryka);
    //        
    //        glPopMatrix();
    wRG->rgRysujScene();
    //z bufora na ekran
    
    if(!(wylaczenia & BEZ_SWAPBUFFERS))SwapBuffers(uchwytDC);


}
#ifdef ARCBALL

void COknoGL::InicjujArcBall() {
    Matrix3fSetIdentity(&LastRot);
    Matrix3fSetIdentity(&ThisRot);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Transform.M[i + 4 * j] = (i == j) ? 1.0f : 0.0f;
    ArcBall = new ArcBallT(640.0f, 480.0f);
}

COknoGL::~COknoGL() {
    delete ArcBall;
    delete kontrolki;
}
#endif
void __cdecl WatekOkna(void * p){
    ParametryWatkuOkna * pwa;
    pwa=reinterpret_cast<ParametryWatkuOkna *>(p);
    switch(pwa->jakaFunkcja){
        case WYSWIETL_GWIAZDOZBIORY:
            pwa->wRG->WyswietlGwiazdozbiory(pwa->zmiennaInt);
            break;
        case OD_NOWA_PRZELICZ:
            RysujGwiazdy * noweGwiazdy=new RysujGwiazdy;
            RysujGwiazdy * dotychczasowyWskaznik=pwa->wRG;
            noweGwiazdy->Inicjuj();
            noweGwiazdy->KopiujUstawienia(dotychczasowyWskaznik);
            //tu powinna nastapic zamiana wskaźników i na ten czas zablokowanie sekcji krytycznej
            pwa->wOkno->UstawWskaznikRG(noweGwiazdy);
            delete dotychczasowyWskaznik;
           
            break;
    }
    delete pwa; 
    printf("\nkoniec watku");
    _endthread();    
}
void COknoGL::WydzielGzZklawiatury(){
    ParametryWatkuOkna * pwa = new ParametryWatkuOkna;
    pwa->wRG=this->wRG;
    pwa->zmiennaInt=WPISANE_OD_NOWA;
    pwa->jakaFunkcja=WYSWIETL_GWIAZDOZBIORY;
    HANDLE hThread =( HANDLE ) _beginthread(WatekOkna,0, pwa);
    
}
void COknoGL::OdNowaPrzeliczaj(){
    ParametryWatkuOkna * pwa = new ParametryWatkuOkna;
    pwa->wOkno=this;
    pwa->wRG=this->wRG;
    pwa->jakaFunkcja=OD_NOWA_PRZELICZ;
    HANDLE hThread =( HANDLE ) _beginthread(WatekOkna,0, pwa);
}