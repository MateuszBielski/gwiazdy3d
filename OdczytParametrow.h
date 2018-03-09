/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OdczytParametrow.h
 * Author: BCO
 *
 * Created on 20 kwietnia 2016, 08:24
 */

#ifndef ODCZYTPARAMETROW_H
#define ODCZYTPARAMETROW_H
#include "OdczytPliku.h"


class OdczytParametrow : public OdczytPliku {
public:
    OdczytParametrow();
    OdczytParametrow(const char * );
    OdczytParametrow(const OdczytParametrow& orig);
    virtual ~OdczytParametrow();
    void WypiszParametry();
    int OdczytajParametr(std::string&,float *&);//na podstawie nazwy odczytuje wymagane parametry jednego rodzaju, *& - bo referencja wskaźnika
    int OdczytajParametr(std::string&,int& );//na podstawie nazwy odczytuje wymagane parametry jednego rodzaju
    int OdczytajParametr(std::string& ,float& );
private:
       

};

#endif /* ODCZYTPARAMETROW_H */

