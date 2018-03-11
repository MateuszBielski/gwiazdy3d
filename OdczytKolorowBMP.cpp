/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OdczytKolorowBMP.cpp
 * Author: BCO
 * 
 * Created on 4 maja 2016, 07:46
 */

#include "OdczytKolorowBMP.h"

OdczytKolorowBMP::OdczytKolorowBMP() {
}
OdczytKolorowBMP::OdczytKolorowBMP(const char * aP):OdczytPliku(aP),bfh(0),bih(0),tablicaKolorow(0),dlugoscTablicy(1) {
}

OdczytKolorowBMP::OdczytKolorowBMP(const OdczytKolorowBMP& orig) {
}

OdczytKolorowBMP::~OdczytKolorowBMP() {
    if(tablicaKolorow) delete[] tablicaKolorow;
    if(bfh) delete bfh;
    if(bih) delete bih;
}
void OdczytKolorowBMP::PrzeczytajNaglowki(){
    if(!ifs.good())return;
    char* temp = new char[sizeof(BitmapFileHeader)];
 ifs.read(temp, sizeof(BitmapFileHeader));
 bfh = (BitmapFileHeader*)(temp);
 
 temp = new char[sizeof(BitmapInfoHeader)];
 ifs.read(temp, sizeof(BitmapInfoHeader));
 bih = (BitmapInfoHeader*)(temp);
 
 //ustalenie szerokości wczytanego obrazka
  dlugoscTablicy = bih->biWidth;
 if(dlugoscTablicy % 4) dlugoscTablicy += 4 - (dlugoscTablicy % 4); // piksele w bitmapie są wyrównywane do 4 bajtów
  printf("\nprzeczytano naglowki BPM, dlugosc tablicy kolorow %d",dlugoscTablicy);
}
unsigned char * OdczytKolorowBMP::TablicaKolorowChar(){
    if(!ifs.good())return tablicaKolorow;
    char * temp = new char[3*dlugoscTablicy];
    ifs.seekg(bfh->bfOffBits, std::ios::beg); // bfOffBits wskazuje początek danych obrazka
    ifs.read(temp, 3*dlugoscTablicy);//czytamy tylko jedną linię
    tablicaKolorow = (unsigned char*)temp;
    return tablicaKolorow;
}

