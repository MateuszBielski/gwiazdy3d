/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OdczytKolorowBMP.h
 * Author: BCO
 *
 * Created on 4 maja 2016, 07:46
 */

#ifndef ODCZYTKOLOROWBMP_H
#define ODCZYTKOLOROWBMP_H
#include "OdczytPliku.h"
#pragma pack(push, 1)
class BitmapFileHeader
{ 
public:
    short bfType; 
  int bfSize; 
  short bfReserved1; 
  short bfReserved2; 
  int bfOffBits; 
};
 
class BitmapInfoHeader
{
public:
  int biSize; 
  int biWidth; 
  int biHeight; 
  short biPlanes; 
  short biBitCount; 
  int biCompression; 
  int biSizeImage; 
  int biXPelsPerMeter; 
  int biYPelsPerMeter; 
  int biClrUsed; 
  int biClrImportant; 
};
#pragma pack(pop)
class OdczytKolorowBMP : public OdczytPliku{
public:
    OdczytKolorowBMP();
    OdczytKolorowBMP(const char *);
    OdczytKolorowBMP(const OdczytKolorowBMP& orig);
    virtual ~OdczytKolorowBMP();
    void PrzeczytajNaglowki();
    unsigned char * TablicaKolorowChar();
    int DlugoscTablicy(){return dlugoscTablicy;};
private:
    BitmapFileHeader * bfh;
    BitmapInfoHeader * bih;
    unsigned char * tablicaKolorow;
    int dlugoscTablicy;

};

#endif /* ODCZYTKOLOROWBMP_H */

