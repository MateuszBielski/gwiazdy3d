/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZapisDoPliku.h
 * Author: BCO
 *
 * Created on 5 sierpnia 2016, 08:58
 */

#ifndef ZAPISDOPLIKU_H
#define ZAPISDOPLIKU_H
#include <fstream>

using namespace std;

class ZapisDoPliku {
public:
    ZapisDoPliku();
    ZapisDoPliku(const char* , ios_base::openmode );//mode może określić, czy tekstowo, czy binarnie i inne
    ZapisDoPliku(const ZapisDoPliku& orig);
    virtual void ZapiszDane(void *){};
    virtual ~ZapisDoPliku();
    int DlugoscPliku();
protected:
    const char * adresPliku;
    fstream ofs;
private:

};

#endif /* ZAPISDOPLIKU_H */

