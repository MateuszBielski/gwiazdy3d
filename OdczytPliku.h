/* 
 * File:   OdczytPliku.h
 * Author: Mateusz Bielski
 *
 * Created on 5 lutego 2016, 11:45
 */

#ifndef ODCZYTPLIKU_H
#define ODCZYTPLIKU_H
#include "Gwiazda.h"
#include <deque>

#define BINARNIE 0
#define TEKSTOWO 1
class OdczytPliku {
public:
    OdczytPliku();
    OdczytPliku(const char * );
    OdczytPliku(const char * ,char);
    OdczytPliku(const OdczytPliku& orig);
    OdczytPliku& operator=(OdczytPliku& );
    virtual ~OdczytPliku();
    char * adresPliku;
private:
protected:  
    std::ifstream ifs;
    int OdczytajKolejneLinie(std::ifstream &);
    std::deque<std::string> linieOdczytane;//bez analizy i podziałów
};
#endif /* ODCZYTPLIKU_H */

