/* 
 * File:   OdczytWierzcholkow.cpp
 * Author: Mateusz Bielski
 * 
 * Created on 19 lutego 2016, 07:56
 */

#include <vector>

#include "OdczytWierzcholkow.h"

OdczytWierzcholkow::OdczytWierzcholkow() {

}

OdczytWierzcholkow::OdczytWierzcholkow(const char * aP) : OdczytPliku(aP), poziom(0), poczatekAktualnychSrodkowScian(0) {

    double d1 = 0;
    char c1[sizeof (double)];

    //12 wierzchołków
    for (int i = 0; i < 36; i++) {
        ifs.read((char*) &c1, sizeof (double));
        Zamien(c1);
        memcpy(&d1, &c1, sizeof (double));
        wierzcholkiZpliku[i] = d1;
    }
    double d2 = 0;
    //20 środków ścian
    dSrodkiScian.assign(60, 0);
    for (int i = 0; i < 60; i++) {
        ifs.read((char*) &c1, sizeof (double));
        Zamien(c1);
        memcpy(&d2, &c1, sizeof (double));
        dSrodkiScian.at(i) = d2;
    }
    int i1 = 0;
    char c2[sizeof (int)];
    //numeryWierzcholkow.assign(20,0);
    //20 numerów wierzchołków
    for (int i = 0; i < 20; i++) {

        ifs.read((char*) &c2, sizeof (int));
        Zamien(c2,sizeof (int));
        memcpy(&i1, &c2, sizeof (int));
        numeryWierzcholkow[i] = i1;

    }
    //należy wypełnić dWierzcholkiPoczatkowe wartościami dla zerowego poziomu
    vWierzcholkiPoczatkowe.assign(60, 0);
    for (int i = 0; i < 20; i++) {
        int ktoryWierzcholek = numeryWierzcholkow[i];
        //każdą współrzędną oddzielnie skopiować
        for (int j = 0; j < 3; j++)vWierzcholkiPoczatkowe.at(i * 3 + j) = wierzcholkiZpliku[ktoryWierzcholek * 3 + j];

    }
//    printf("\nnumery wierzcholkow");
//    for (int i = 0; i < 20; i++) {
//        printf("\n%d",numeryWierzcholkow[i]);
//    }
    printf("\nodczytano 12 wierzchołkow, srodki 20 scian i numery wierzcholkow rozpoczynajacych podzial ");
    if (ifs.is_open()) {
        ifs.close();
        printf("\nzamknieto plik po odczytaniu wierzchołków");
    }

    //    
}

OdczytWierzcholkow::OdczytWierzcholkow(const OdczytWierzcholkow& orig) {
}

OdczytWierzcholkow::~OdczytWierzcholkow() {
}

void OdczytWierzcholkow::OdczytajWierzcholki(std::ifstream & ifsW) {

}

void OdczytWierzcholkow::WyswietlWartosciWierzcholkow() {
    for (int i = 0; i < 36; i++) {
        printf("\n %d. %1.6f", i, wierzcholkiZpliku[i]);
    }
}

void OdczytWierzcholkow::WyswietlWartosciSrodkowScian() {
    //3 wspolrzedne w jednym wierszu
    int j = 0;
    double pW[]={1.0,0.0,0.0};//poprzedni wektor - do mierzenia kątów
    while (j < dSrodkiScian.size() / 3) {
        double s[3], d[2];
        for (int i = 0; i < 3; i++)s[i] = dSrodkiScian.at(3 * j + i);
        xyz_to_LF(d, s);
        double kat=acos(s[0]*pW[0]+s[1]*pW[1]+s[2]*pW[2]);
        for (int i = 0; i < 3; i++)pW[i]=s[i];
        printf("\n%d r: %4.4f d: %4.4f kat do poprzedniego %4.4f", j++, Rad_To_hRekt(d[0]),Rad_To_Deg(d[1]),Rad_To_Deg(kat));
    }
}
void OdczytWierzcholkow::WyswietlWartosciWierzcholkowPoczatkowych(){
    int j = 0;
    double pW[]={1.0,0.0,0.0};//poprzedni wektor - do mierzenia kątów
    while (j < vWierzcholkiPoczatkowe.size() / 3) {
        double s[3], d[2];
        for (int i = 0; i < 3; i++)s[i] = vWierzcholkiPoczatkowe.at(3 * j + i);
        xyz_to_LF(d, s);
        double kat=acos(s[0]*pW[0]+s[1]*pW[1]+s[2]*pW[2]);
        for (int i = 0; i < 3; i++)pW[i]=s[i];
        printf("\n%d r: %4.4f d: %4.4f kat do poprzedniego %4.4f", j++, Rad_To_hRekt(d[0]),Rad_To_Deg(d[1]),Rad_To_Deg(kat));
    }
}

bool OdczytWierzcholkow::WyznaczSrodkiKolejnegoPoziomu() {
    //koncepcja jest taka, żeby przy wyznaczaniu środków kolejnych mniejszych trójkątów od razu wskazywać wektor, który będzie rozpoczynał następny podział
    //wyznaczanie środków kolejnego poziomu
    //zaczniemy od pierwszego poziomu, zerowy jest zrobiony, zapisany w pliku

    int ileStref = vWierzcholkiPoczatkowe.size() / 3;
    int ileWspolrzednychObliczonoWczesniej=dSrodkiScian.size();
    poziom++;
    std::vector<double> vNoweWp(ileStref * 3 * 4); //tymczasowy wektor na wierzchołki początkowe do następnego podziału,czterokrotnie większy od poprzedniego
    
    for (int i = 0; i < ileStref; i++) {
        try{
            
        PodzielStrefe(i, vNoweWp); /*funkcja wyznacza trzy wektory nowe i wraz z pierwszym dodaje do kolejki*/
        }catch(std::out_of_range& oor){
            printf("\nWyznaczSrodkiKolejnegoPoziomu problem z podzialem strefy %d",i);
        }

    }
    poczatekAktualnychSrodkowScian =  ileWspolrzednychObliczonoWczesniej;/*wskazuje na element z kolejki dSrodkiScian, od którego zaczynają się
                                                           *współrzędne aktualnego poziomu, uwzględniając wszystkie do tej pory wyliczone 
                                                           * współrzędne środków ścian, potrzebne do metody PodzielStefe*/
    //w tym miejscu należy vektor vWierzcholkiPoczatkowe przygotować do nowego poziomu
    vWierzcholkiPoczatkowe.clear();
    vWierzcholkiPoczatkowe = vNoweWp; //
    printf("\nWyznaczSrodkiKolejnegoPoziomu poziom %d,obliczono %d stref, vWierzcholkiPoczatkowe dla nastepnego podzialu rozmiar %d",poziom,ileStref,vWierzcholkiPoczatkowe.size());
    return true;
}
void OdczytWierzcholkow::PodzielStrefe(int ktoraStrefa, std::vector<double>& vNwP) {
    //srodek indeks pierwszej wspolrzednej środka dzielonej strefy
    double nW[3],wierzch[3],nWp[3]; /*nowy wektor do obracania i zapisywania,wierzchołek pierwszy, od którego zaczyna się podział,
                                    * nowy wierzchołek  pierwszy do następnego podziału*/
    double katIobrot[4],katIobrot2[4]; //kat 120 i współrzędne pierwotnego środka, drugi kąt będzie do obracania nowych wierzchołków pierwszych
    katIobrot[0] =-k120st;
    katIobrot2[0] =-k120st/2;
    //vektor z nowymi wierzcholkami poczatkowymi najlepiej wypełnić w sposób natępujący:
    //mamy pierwszy nowy środek
    //kolejne nowe środki nowe środki znajdują się w połowie boków dzielonego trójkąta

    //wyznaczenie środka pierwszego z czterech trójkatów
    
    for (int i = 0; i < 3; i++) {
        katIobrot[i + 1] = dSrodkiScian.at(poczatekAktualnychSrodkowScian+3*ktoraStrefa + i);/*trzeba dodać wartość poczatekAktualnychSrodkowScian, która uwzględnia wszystkie współrzędne
                                                               * wyliczone w poprzednim podziale*/
        katIobrot2[i+1]=katIobrot[i + 1];
        wierzch[i]=vWierzcholkiPoczatkowe.at(3*ktoraStrefa + i);
        nW[i] = (katIobrot[i + 1] + wierzch[i]) / 2;
        
    }
        
    NormujWektord(nW);
    //dodać trzeba pierwszy wierzchołek nowego podziału
    for(int i=0;i<3;i++)vNwP.at(3*4*ktoraStrefa + i)=wierzch[i];//4 - bo w następnym podziale jest 4 razy wiecej trójkątów
    //od środka pierwszego podzielonego trójkąta zaczniemy obliczać nowe wierzchołki pierwsze na następny podział
    for(int i=0;i<3;i++)nWp[i]=nW[i];
    //drugi nowy wierzcholek (po obrocie o 60st)
    ObrocPunktd(nWp[0], nWp[1], nWp[2], katIobrot2);
    katIobrot2[0]=k120st;

    for(int j=0;j<3;j++){
        //dodanie drugiego i następnego nowego wierzchołka dla następnego podziału
        for(int i=0;i<3;i++)vNwP.at(3*4*ktoraStrefa+3+3*j + i)=nWp[i];//4 - bo w następnym podziale jest 4 razy wiecej trójkątów
        ObrocPunktd(nWp[0], nWp[1], nWp[2], katIobrot2);
        //ustalanie kolejnych trójkątów
        for (int k = 0; k < 3; k++)dSrodkiScian.push_back(nW[k]); //dodanie współrzędnych do kolejki
        ObrocPunktd(nW[0], nW[1], nW[2], katIobrot);
    }
    //na koniec dodajemy czwarty wektor,  czyli pierwotny, bo tak wynika z analizy stellarium
    for (int i = 1; i < 4; i++)dSrodkiScian.push_back(katIobrot[i]);
}


void OdczytWierzcholkow::ListaWspolrzednych(std::deque<double> & wierzcholki) {
    for (int i = 0; i < 36; i++)
        wierzcholki.push_back(wierzcholkiZpliku[i]);
}
//void OdczytWierzcholkow::PodzielStrefeZapas(int ktoraStrefa, std::vector<double>& vNwP) {
//    //srodek indeks pierwszej wspolrzednej środka dzielonej strefy
//    double nW[3],wierzch[3],wR[3]; /*nowy wektor do obracania i zapisywania,wierzchołek pierwszy, od którego zaczyna się podział,
//                                    * wektor różnicowy do wyznaczania wierzchołków pierwszych następnego podziału*/
//    double katIobrot[4]; //kat 120 i współrzędne pierwotnego środka
//    katIobrot[0] =-k120st;
//    //vektor z nowymi wierzcholkami poczatkowymi najlepiej wypełnić w sposób natępujący:
//    //mamy pierwszy nowy środek
//    //znajdujemy różnicę do aktualnego wierzcholka początkowego
//    //różnicę tą dodajemy do kolejnych nowych środków
//
//    //wyznaczenie środka pierwszego z czterech trójkatów
//    
//    for (int i = 0; i < 3; i++) {
//        katIobrot[i + 1] = dSrodkiScian.at(poczatekAktualnychSrodkowScian+3*ktoraStrefa + i);/*trzeba dodać wartość poczatekAktualnychSrodkowScian, która uwzględnia wszystkie współrzędne
//                                                               * wyliczone w poprzednim podziale*/
//        wierzch[i]=vWierzcholkiPoczatkowe.at(3*ktoraStrefa + i);
//        nW[i] = (katIobrot[i + 1] + wierzch[i]) / 2;
//        //wektor różnicowy
//        wR[i]=wierzch[i]-katIobrot[i+1];
//    }
//
//    //ustalanie kolejnych trójkątów
//    for (int j = 0; j < 3; j++) {
//        NormujWektord(nW);
//        for (int k = 0; k < 3; k++)dSrodkiScian.push_back(nW[k]); //dodanie współrzędnych do kolejki
//        //dopisać należy również współrzędne wierzchołków początkowych dla następnego podziału
//        for (int k = 0; k < 3; k++)vNwP.at(3*4*ktoraStrefa+3*j + k)=nW[k]+wR[k];
//        printf("\n%d ustawiono nowy wierzcholek %4.4f %4.4f %4.4f",ktoraStrefa*4+j,vNwP.at(3*4*ktoraStrefa+3*j),vNwP.at(3*4*ktoraStrefa+3*j + 1),vNwP.at(3*4*ktoraStrefa+3*j + 2));
//        ObrocPunktd(nW[0], nW[1], nW[2], katIobrot);
//       
//    }
//    //na koniec dodajemy czwarty wektor,  czyli pierwotny, bo tak wynika z analizy stellarium
//    for (int i = 1; i < 4; i++)dSrodkiScian.push_back(katIobrot[i]);
//    //oraz ustalamy czwarty wierzchołek początkowy,
//    for (int k = 0; k < 3; k++)vNwP.at(3*4*ktoraStrefa+9 + k)=katIobrot[k]-wR[k]; 
//    printf("\n%d ustawiono nowy wierzcholek %4.4f %4.4f %4.4f",ktoraStrefa*4+3,vNwP.at(3*4*ktoraStrefa+9),vNwP.at(3*4*ktoraStrefa+9 + 1),vNwP.at(3*4*ktoraStrefa+9 + 2));
//    
////    double w4[3];
////    for(int i=0;i<3;i++){
////        w4[i]=(vNwP.at(3*ktoraStrefa+3 +i)+vNwP.at(3*ktoraStrefa+6 +i))/2;
////    }
////    NormujWektord(w4);
////    //i ustawić w vektorze
////    for(int i=0;i<3;i++){
////        try{
////            
////        vNwP.at(3*ktoraStrefa+9 +i)=w4[i];
////        }catch(std::out_of_range& oor){
////            printf("\nblad dostepu do vNwP.at %s",oor.what());
////        }
////    }
//
//    
//}
