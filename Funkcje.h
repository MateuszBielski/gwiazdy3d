/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Funkcje.h
 * Author: BCO
 *
 * Created on 18 lutego 2016, 14:43
 */

#ifndef FUNKCJE_H
#define FUNKCJE_H
#include "Gwiazda.h"

 bool wPlx(Gwiazda &,Gwiazda &);//według paralaxy
    bool wHip(Gwiazda &,Gwiazda &);//według nazwy
    bool wOdlDoCentrum(Gwiazda &,Gwiazda &);//według odległości jw
    bool wMag(Gwiazda2 &,Gwiazda2 &);
    bool wMagIteratorow(iteratorLG & , iteratorLG & );
    bool wmag(Gwiazda2 & , Gwiazda2 & );
    bool wOdl(Gwiazda2 &,Gwiazda2 &);
    bool wgPierwotnejNumeracji(Gwiazda2 &,Gwiazda2 &);
    bool wgSkupienia();
    bool WzdluzX(Gwiazda2 &,Gwiazda2 &);//
    bool WzdluzY(Gwiazda2 &,Gwiazda2 &);//
    bool WzdluzZ(Gwiazda2 &,Gwiazda2 &);//
    bool WgNazwy(Gwiazdozbior & ,Gwiazdozbior &);
    void Zamien(char *);
    void Zamien(char * z,short rozmiar);
    int Minimum(float *,int *,int,int);
    int Sortuj(float *,int *,int,int);//tablica porównywanych wartości, tablica zamienianych indeksów, ilość elementów, zwraca ilość dokonanych zamian
    int SortujIndeksy(float * ,int * ,int ,int );
    //obliczenia geometryczne
    double Rad_To_Deg(double );
    double Rad_To_hRekt(double r);
    double Deg_To_Rad(double );
    void xyz_to_LF(double *,double*);
    bool LF_to_xyz(double *,double*);
    void ObrocPunktd(double & ,double &,double & ,double * katIobrot);
    void NormujWektord(double * wektor);
    void KatIWektorObrotuMiedzyWektoramid(double * wynik,double * wektor1,double * wektor2);


#endif /* FUNKCJE_H */

