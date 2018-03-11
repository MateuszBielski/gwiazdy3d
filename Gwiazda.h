/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Gwiazda.h
 * Author: BCO
 *
 * Created on 18 lutego 2016, 14:49
 */

#ifndef GWIAZDA_H
#define GWIAZDA_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <list>
#include <cstring>
#include <sstream>
#include <string>
#include <deque>
#include <exception>
#include <stdexcept>

#define HIP 1
#define MAG 2
#define W_ABS 4
#define ODL 8
#define WLK_DO_WYSWIETLENIA 16
#define ABS_WIELKOSC_DO_WYSWIETLENIA 32
#define B_V 64
#define DYSTANS 128
#define SYMBOL 128

class xNullPointerException :public std::exception{
    public: 
    xNullPointerException(char * doCzego){
        printf("\nNullPointerException - pusty wskaznik %s",doCzego);
    };
	private:
};
class xBrakPliku : public std::exception{
public:
    xBrakPliku(){};
    xBrakPliku(const char * adres){printf("\nbrak pliku %s",adres); };
    char * what(){}
};
class xBladOdczytu : public std::exception{
public:
    xBladOdczytu(int nr){printf("\nprzerwano odczyt %d linii z powodu bledu",nr);};
};
class Gwiazda {
public:

    Gwiazda(){};
    Gwiazda(int i):hip(i),comp_id(i),x0(i),x1(i),b_v(i),mag(i),sp_int(i),dx0(i),dx1(i),plx(i){};

     ~Gwiazda() {
    };
    void Ustaw(Gwiazda *);
    void WyswietlHip();
    void WyswietlParametry(short *);
    int Hip(){return hip;};
    int Comp_id(){return comp_id;};
    int X0(){return x0;};
    int X1(){return x1;};
    int B_v(){return b_v;};
    int Mag(){return mag;};
    int Sp_int(){return sp_int;};
    int Dx0(){return dx0;};
    int Dx1(){return dx1;};
    int Plx(){return plx;};
    int OdlegloscDoCenrtum();
    int Odleglosc();
    void WyzerujX0(){x0=0;};
    void WyzerujX1(){x1=0;};
 
private:
    unsigned hip : 24;
    unsigned comp_id : 8;
    int x0 ;
    int x1 ;
    unsigned b_v : 8;
    unsigned mag : 8;
    unsigned sp_int : 16;
    int dx0;
    int dx1;
    int plx;
};


class Gwiazda2 :public Gwiazda{
    
public:
    Gwiazda2():Gwiazda(0),odleglosc(0),magnitudo(0),Magnitudo(0){};
    Gwiazda2(Gwiazda *);//zakładam, że kopiuje podstawowe parametry
    Gwiazda2(Gwiazda & g,float m,float M):Gwiazda(g),magnitudo(m),Magnitudo(M){};
    ~Gwiazda2(){};
    void Ustaw(Gwiazda *,float,float,float);
    void Ustaw(float,float,float,float,float);//w tej wersji część Gwiazda jest już ustawiona
    void UstawNazwe(std::string& );
    void UstawSymbol(std::string& );
    void NrPorzadkowyX(int x){nrPorzadkowyX=x;};
    void NrPorzadkowyY(int y){nrPorzadkowyY=y;};
    void NrPorzadkowyZ(int z){nrPorzadkowyZ=z;};
    int KtoryNrPorzadkowyX(){return nrPorzadkowyX;};
    int KtoryNrPorzadkowyY(){return nrPorzadkowyY;};
    int KtoryNrPorzadkowyZ(){return nrPorzadkowyZ;};
    int Parametry(std::string&, char );//zwraca nr hip
    int Parametry(std::deque<std::string>&, char );//zwraca nr hip
    float JM(){return Magnitudo;};
    float Jm(){return magnitudo;};
    float Odleglosc(){return odleglosc;};
    float DystansDo(float,float,float);
    float KwadratDystansuDo(float,float,float);
    std::list<std::list<Gwiazda2>::iterator>::iterator ItWliscieKomorek(){return itWliscieKomorek;};
    void UstawWspolrzedne(double *,double);//wskaźnik na tablicę z trzema współrzędnymi, i dzielnik zmniejszający współrzędne
    void UstawItWliscieKomorek(std::list<std::list<Gwiazda2>::iterator>::iterator  it){itWliscieKomorek=it;};
    void PokazWspolrzedneXYZ();
    double WielkoscDoWyswietlenia();//ewentualnie do zmiany, jeśli chcemy uwzglednić zewnętrzną zmienną skalę
    double AbsolutnaWlkDoWyswietlenia();
    std::string Nazwa(){return nazwa;};
    std::string Symbol(){return symbol;};
    double X(){return x;};
    double Y(){return y;};
    double Z(){return z;};
    float OX(){return ox;};
    float OY(){return oy;};
    float OZ(){return oz;};
    float SkupienieNumerowPorzadkowych();
    int numeracjaPierwotna;//lista z gwiazdami będzie sortowana potrzebna jest numeracja, która odtworzy pierwotny porządek
private:
    std::string nazwa;
    std::string symbol;
    float odleglosc;
    float magnitudo;
    float Magnitudo;
    float wielkoscDoWyswietlenia;
    float absolutnaWielkoscDoWyswietlenia;
    double x,y,z;
    float ox,oy,oz;
    int nrPorzadkowyX,nrPorzadkowyY,nrPorzadkowyZ;
    std::list<std::list<Gwiazda2>::iterator>::iterator itWliscieKomorek;//do optymalizacji wyświetlania obszar jest podzielony na komórki, jest jedna lista, która zawiera posegregowane iteratory 
                                    //do zasadniczej listy gwiazd itWliscieKomorek, jest polem które umożliwi "wyrejestrowanie się" danej gwiazdy z listy
};
typedef std::list<Gwiazda2>::iterator iteratorLG;
typedef std::list<std::list<Gwiazda2>::iterator> listaItLG;
typedef std::list<std::list<Gwiazda2>::iterator>::iterator iteratorLitLG;

//klasa na potrzeby obsługi przycisków
class Gwiazdozbior {
public:
    Gwiazdozbior();
    Gwiazdozbior(std::string,std::string );
    ~Gwiazdozbior(){};
    std::string NazwaSkrocona(){return nazwaSkrocona;};
    std::string Nazwa(){return nazwa;};
    void UstawNumery(int nP,int ile){numerPorzadkowy=nP;ilePozycji=ile;};
    int KodPrzycisku(){return 1000*ilePozycji+numerPorzadkowy;};
    int NumerPorzadkowy(){return numerPorzadkowy;};
    int IlePozycji(){return ilePozycji;};
    
private:
    std::string nazwaSkrocona;
    std::string nazwa;
    int numerPorzadkowy,ilePozycji;//ilePozycji np. orion zawiera 9
};
#include "Funkcje.h"
#endif /* GWIAZDA_H */

