
#ifndef OPENGL_H
#define OPENGL_H

//#define _USE_MATH_DEFINES
//
#include <windows.h>

//#include <commctrl.h>
//

#include <math.h>
#include <cstdio>
#include <deque>
#include <exception>

#include "Varia.h"

//#include "C:\Users\BCO\Documents\NetBeansProjects\CppApplication_2\ZbiorGwiazd.h"
#include "OdczytajGwiazdy.h"
#include "OdczytWierzcholkow.h"
#include "ObliczeniaGwiazd.h"
#include "OdczytNazw.h"
#define ARCBALL
#ifdef ARCBALL
#include "ArcBall.h"
#endif
#define BEZ_SWAPBUFFERS 1
#define BEZ_CLEARBUFFERS 2
#define BEZ_ODSWIEZANIA_KONTROLEK 4
#define WYSWIETL_GWIAZDOZBIORY 1
#define OD_NOWA_PRZELICZ 2
#include "RysujGwiazdy.h"
#include "Kontrolki.h"
//#include "UkladyPunktowMaterialnych.h"
//#define UTWORZ_OKNO_A 11
//#define UTWORZ_OKNO_B 12
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProceduraOknaGL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProceduraOknaDoKontrolek(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class COkno {
public:
    //COkno():{};
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool Init(HINSTANCE, POINT, POINT);
    WPARAM Run();
    HWND uOkna1,uOknaPodst;//,uOknaKontrolek,hButton ; //,uOkna2,uOkna3,uOkna4,uOkna5,uOkna6,uOkna7,uORysowania;
    //std::deque<HWND> uchwytyKontrolek;
    long szerokoscObszaruUzytkownika;
    long wysokoscObszaruUzytkownika;
protected:
    HINSTANCE uAplikacji;
    Kontrolki * kontrolki;
    
    //      
};
//


class COknoGL : public COkno {
    //      
 public:
    COknoGL() : COkno(), uchwytRC(NULL), uchwytDC(NULL),
    poczatkowaPozycjaKursoraMyszy(POINT()), kameraR(10),kameraX(0),kameraY(0),kameraZ(0),wRG(0),nearDoKorektySceny(0.2)
    //,wspKam(1),coPokazac(LINIE|NA_SFERZE|W_PRZESTRZENI),oBaza(0),idWybranegoPunktu(0),idWybranegoGwiazdozbioru(-1)
#ifdef ARCBALL
    , ArcBall(NULL)
#endif
    {
#ifdef ARCBALL
        InicjujArcBall();
#endif
    };
    void InicjujArcBall();
    ~COknoGL();//usuwanie obiektu ArcBall
    void UstawWskaznikRG(RysujGwiazdy* rg){wRG=rg;};
    int WyoborPunktu(POINT);
    void UstawienieSceny(bool);
    void KorektaSceny(double);//ma zmieniać kąt patrzenia kamery argumentem jest  parametr near do funkcji glFrustum
    void RysujScene(int);//w argumencie flagi wyłączenia, np bez funkcji swapbuffers, bez czyszczenia buforów
    
    LRESULT CALLBACK ProceduraZasadnicza(HWND, UINT, WPARAM, LPARAM); 
    LRESULT CALLBACK ProceduraOknaGL(HWND, UINT, WPARAM, LPARAM); 
    LRESULT CALLBACK ProceduraOknaKontrolek(HWND, UINT, WPARAM, LPARAM); 
private:
    HGLRC uchwytRC; //uchwyt kontekstu renderingu
    HDC uchwytDC; //uchwyt prywatnego kontekstu urz�dzenia GDI 
    RysujGwiazdy * wRG;
    bool UstalFormatPikseli(HDC) const;
    bool InitWGL(HWND);
    void UsunWGL();
    void WydzielGzZklawiatury();//wywoływana po wciśnięciu klawisza z, czyli oczekuje na wprowadzenie danych i dopiero działa
    void OdNowaPrzeliczaj();
    POINT poczatkowaPozycjaKursoraMyszy;
    float poprzedniaPozycjaKursoraMyszy3D[3];
    std::deque<double> wierzcholki;
    float kameraX,kameraY,kameraZ, kameraR,nearDoKorektySceny;
    
    
#ifdef ARCBALL
    Matrix4fT Transform;
    Matrix3fT LastRot;
    Matrix3fT ThisRot;
    ArcBallT* ArcBall;
    Point2fT MousePt;
#endif
 
};
void __cdecl WatekOkna(void * );
class ParametryWatkuOkna{
public:
    COknoGL * wOkno;
    RysujGwiazdy * wRG;
    LPARAM parametryKomunikatu;
    int jakaFunkcja;
    int kamera[4];
    int zmiennaInt;
};


#endif
