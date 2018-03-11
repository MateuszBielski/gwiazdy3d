/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OdczytNazw.h
 * Author: BCO
 *
 * Created on 1 marca 2016, 09:09
 */

#ifndef ODCZYTNAZW_H
#define ODCZYTNAZW_H

#include "OdczytPliku.h"
#include <string>
#include <vector>
#include <map>
#include <time.h>

class OdczytNazw :public OdczytPliku {
public:
    OdczytNazw();
    OdczytNazw(const char *);
    OdczytNazw(const char *,char);//w drugim parametrze informacja, że binarnie
    OdczytNazw(const OdczytNazw& orig);
    virtual ~OdczytNazw();
    int UzupelnijIndeksyHipSymboliNazw(std::vector<int>&,std::vector<std::string>&,std::deque<int>&,std::deque<int>&);
    int IleOdczytanychLinii();
    int UzupelnijNazwyGwiazdozbiorow(std::vector<std::string>& );
    void UzupelnijPelneNazwyGwiazdozbiorow(std::deque<Gwiazdozbior>&,std::map<std::string,std::string>& );
    int UzupelnijNazwyGwiazd(iteratorLG *,std::map<std::string,std::string>& );//iterator daje szybki dostęp do żądanej gwiazdy,a mapa powinna zapewnić tłumaczenie
    
    void WypiszOddzieloneNazwy();
    std::string NazwaWgNumeru(int i);
    
private:
    std::deque<std::string> nazwyOddzielone;
    std::string PodzialNazw(std::string& ,int & ,std::string& );//podział na numer, symbol i gwiazdozbiór;
    void PodzialNazwGwiazd(std::string&  ,iteratorLG *,std::map<std::string,std::string>& );//z danej linii odczytuje numer gwiazdy, skacze do odpowiedniej gwiazdy, poprzez iterator i ustawia jej nazwe
    Gwiazdozbior PodzialNazwGwiazdozbiorow(std::string&  ,std::map<std::string,std::string>& );

};

#endif /* ODCZYTNAZW_H */

