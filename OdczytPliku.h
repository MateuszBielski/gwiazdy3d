/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OdczytPliku.h
 * Author: BCO
 *
 * Created on 5 lutego 2016, 11:45
 */

#ifndef ODCZYTPLIKU_H
#define ODCZYTPLIKU_H
#include "Gwiazda.h"
#include <deque>

#define BINARNIE 0
#define TEKSTOWO 1





//sortowanie
   

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

