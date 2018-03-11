/* 
 * File:   OdczytParametrow.h
 * Author: Mateusz Bielski
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

