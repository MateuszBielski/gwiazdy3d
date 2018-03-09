#ifndef VARIA_H
#define VARIA_H
#include <windows.h>
#include <math.h>
#include <GL\gl.h>
#include <GL\glu.h>
//#include <stdio.h>
#include <stdexcept>


//Funkcje r�ne dodatkowe


float* Roznica3fv(float punkt1[3],float punkt2[3],float wynik[3]);
float* IloczynWektorowy3fv(float [3],float [3], float [3]);
float DlugoscWektora3fv(float [3]);
float* NormujWektor3fv(float [3]);
float* JednostkowyWektorNormalny3fv(float [3],float [3],float [3],float [3]);
float* TransformujPikselDoPrzestrzeniSceny(POINT pozycjaPiksela,float glebokosc,float* pozycjaPunktu3D);
unsigned int StworzCzcionke(bool czyCzcionka3D, HWND uchwytOkna, char* NazwaCzcionki,
                            int wysokoscWPikselach,bool Pogrubiona,bool Kursywa,
                            long kodPierwszegoZnaku,long kodOstatniegoZnaku);
void Pisz(char* napis,int iloscLiter,unsigned int czcionka,int kodPierwszegoZnaku);
void PiszZnaki(unsigned int );
void Wyswietlaj(unsigned int czcionka,int kodPierwszegoZnaku, const char * napis, ...);
bool filetest(const char* filename);
unsigned long* WczytajTeksture(HWND uchwytOkna, char* nazwaPliku, int& teksturaSzer,
                               int& teksturaWys, unsigned char alfa);
//#include "Varia.cpp"
#endif
