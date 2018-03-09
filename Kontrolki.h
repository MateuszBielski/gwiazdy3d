/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Kontrolki.h
 * Author: BCO
 *
 * Created on 6 lipca 2016, 11:39
 */

#ifndef KONTROLKI_H
#define KONTROLKI_H
#define WSZYSTKIE_KONTROLKI 0
#define SZUKAJ 205
#define SZUKAJ_KONSTELACJI 206
#include <windows.h>
#include <commctrl.h>
#include <sstream>
//#include <stdio.h>
//#include <stdlib.h>
#include <cstdio>
#include <deque>
#include <string>
#include <list>
#include <C:\Users\BCO\Documents\NetBeansProjects\CppApplication_2\Gwiazda.h>

class Kontrolki {
    friend class COknoGL;
public:
    Kontrolki(HINSTANCE uAplikacji,HWND uRodzica,int szerO,int wysO):uchwytAplikacji(uAplikacji),uOknaRodzica(uRodzica),
                                                                    czyWidoczne(true),zainicjowane(false),szer(szerO),wys(wysO){};
    Kontrolki(const Kontrolki& orig);
    virtual ~Kontrolki();
    bool Utworz(WNDPROC);/*dopisuje do kolejki uchwyt okna z kontrolkami, 
                                                 * 
                                                 * pobiera wskaźnik do procedury, tworzy przezroczyste okno, do utworzenia na nim kontrolek,
                                                 *  rejestruje uprzednio klasę*/
    int PrzyciskiZlisty(std::list<std::string>&);
    int PrzyciskiZlisty(std::deque<Gwiazdozbior>&);//
    int PrzyciskiWgNumerowPolecen(std::list<int>&);
    bool ZmienStanPrzycisku(int,bool);//true jeśli zaznaczyć
    void PoleWyszukiwania();
    void OdswiezOkno(int);
    void PokazUkryj();
    int OdczytajWpisanyNumer();
    void WypiszTekst(std::string&);
private:
    int margines=5;
    int odstep=5;
    int szer,wys;
    int szerokoscEkranu,wysokoscEkranu;
    HINSTANCE uchwytAplikacji;
    bool czyWidoczne;
    bool zainicjowane;
    //uchwyty okien
    HWND uOknaRodzica;
    HWND uOknaKontrolekG=0,uOknaKontrolekD=0;
    HWND uOknaWyszukiwania,uPrzyciskSzukaj,uZaznaczGwiazdozbior;
    std::deque<HWND> uchwytyKontrolek;
    //czcionki
    HFONT czcionkaZwykla,czcionkaZwezona,czcionkaGruba;

};

#endif /* KONTROLKI_H */

