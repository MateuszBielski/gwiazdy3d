/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Testy.cpp
 * Author: BCO
 * 
 * Created on 18 lutego 2016, 13:54
 */

#include <math.h>

//#include "OdczytPliku.h"
#include "Testy.h"

//PrzyjmijWierzcholki();
void Testy::PorownajWierzcholki(OdczytWierzcholkow & ow){
    
    PrzyjmijWierzcholki();
    printf("\nporównanie wierzchołków");
    for (int i=0;i<60;i++){
        double diff=wierzcholki[i]-ow.WierzcholkiZpliku(i);
        printf("\n %d. %0.17f",i,diff);
    }
}
void Testy::ZapiszWierzcholkiDoPliku() {
    std::ofstream ofs; //("Data2.bin", std::ios::binary); // otwieramy plik do zapisu binarnego
    ofs.open("C:\\Users\\BCO\\Documents\\NetBeansProjects\\CppApplication_2\\dist\\Debug\\MinGW-Windows\\Data2.bin", std::ios::binary | std::ios::out | std::ios::ate);
    if (!ofs.is_open()) {
        printf("\nproblem z dostępem do pliku i otwarciem do zapisu");
        return;
    }
    for (int i = 0; i < 60; i++) {
        double * pDouble = &(wierzcholki[i]);
        ofs.write((char*) pDouble, sizeof (double));
        printf("\nzapisano %2.5e:", *pDouble);
    }
    ofs.close();
}
void Testy::ZapiszOdczytaneGwiazdyDoPliku(OdczytajGwiazdy & og){
    std::ofstream ofs; //("Data2.bin", std::ios::binary); // otwieramy plik do zapisu binarnego
    ofs.open("C:\\Users\\BCO\\Documents\\NetBeansProjects\\CppApplication_2\\dist\\Debug\\MinGW-Windows\\stars_2_0v0_5wyzerowane.cat", std::ios::binary | std::ios::out | std::ios::ate);
    if (!ofs.is_open()) {
        printf("\nproblem z dostępem do pliku i otwarciem do zapisu");
        return;
    }
    //zapis nagłówka
    ofs.write((char*)og.Nag(),sizeof(Naglowek2));
    //zapis stref
    for(int i=0;i<og.IleStref();i++){
        printf("\n%d",og.IleGwiazdWstrefach()[i]);
        ofs.write((char*)(og.IleGwiazdWstrefach()+i),sizeof(int));
    }
    for(int i=0;i<og.IleGwiazd();i++){
        ofs.write((char*)(og.pGwiazda()+i),sizeof(Gwiazda));
    }
//    for (int i = 0; i < 60; i++) {
//        double * pDouble = &(wierzcholki[i]);
//        ofs.write((char*) pDouble, sizeof (double));
//        printf("\nzapisano %2.5e:", *pDouble);
//    }
    ofs.close();
}
void Testy::PrzyjmijWierzcholki() {
    wierzcholki[0] = 0.9347189918913746;
    wierzcholki[1] = 0.0033783564700513025;
    wierzcholki[2] = -0.35537162647731696;
    wierzcholki[3] = 0.9336112107246358;
    wierzcholki[4] = 0.003391326384809268;
    wierzcholki[5] = 0.3582716931528806;
    wierzcholki[6] = 0.57854149816318;
    wierzcholki[7] = -0.5752440392145618;
    wierzcholki[8] = 0.5782594834944077;
    wierzcholki[9] = -1.5722161847938265E-4;
    wierzcholki[10] = -0.35680276955767143;
    wierzcholki[11] = 0.9341797251692727;
    wierzcholki[12] = 0.5803339257430609;
    wierzcholki[13] = -0.5752650249774713;
    wierzcholki[14] = -0.576439663511557;
    wierzcholki[15] = 0.360204128591284;
    wierzcholki[16] = -0.9328733321624529;
    wierzcholki[17] = 5.760954052597966E-4;
    wierzcholki[18] = -0.3534345066000544;
    wierzcholki[19] = -0.9354591209199521;
    wierzcholki[20] = -5.316314951646084E-4;
    wierzcholki[21] = -0.5761500692614923;
    wierzcholki[22] = -0.5794279333119232;
    wierzcholki[23] = 0.5764671437192685;
    wierzcholki[24] = 0.0027429871281403617;
    wierzcholki[25] = -0.35683672523533916;
    wierzcholki[26] = -0.9341627414668894;
    wierzcholki[27] = -0.5743576416816114;
    wierzcholki[28] = -0.5794489190748328;
    wierzcholki[29] = -0.5782320032866961;
    wierzcholki[30] = -0.9336112107246358;
    wierzcholki[31] = -0.003391326384809268;
    wierzcholki[32] = -0.3582716931528806;
    wierzcholki[33] = -0.9347189918913746;
    wierzcholki[34] = -0.0033783564700513025;
    wierzcholki[35] = 0.35537162647731696;
    wierzcholki[36] = 1.5722161847938265E-4;
    wierzcholki[37] = 0.35680276955767143;
    wierzcholki[38] = -0.9341797251692727;
    wierzcholki[39] = -0.57854149816318;
    wierzcholki[40] = 0.5752440392145618;
    wierzcholki[41] = -0.5782594834944077;
    wierzcholki[42] = -0.360204128591284;
    wierzcholki[43] = 0.9328733321624529;
    wierzcholki[44] = -5.760954052597966E-4;
    wierzcholki[45] = -0.5803339257430609;
    wierzcholki[46] = 0.5752650249774713;
    wierzcholki[47] = 0.576439663511557;
    wierzcholki[48] = 0.5761500692614923;
    wierzcholki[49] = 0.5794279333119232;
    wierzcholki[50] = -0.5764671437192685;
    wierzcholki[51] = 0.3534345066000544;
    wierzcholki[52] = 0.9354591209199521;
    wierzcholki[53] = 5.316314951646084E-4;
    wierzcholki[54] = 0.5743576416816114;
    wierzcholki[55] = 0.5794489190748328;
    wierzcholki[56] = 0.5782320032866961;
    wierzcholki[57] = -0.0027429871281403617;
    wierzcholki[58] = 0.35683672523533916;
    wierzcholki[59] = 0.9341627414668894;
    printf("\nprzyjęto wartości skopiowane z javy");
}
//void Testy::ObliczGwiazdy2() {
//    Gwiazda * gwiazda2 = new Gwiazda2[ileGwiazd];
//    //sprawdzenie adrsów
//    printf("\n adresy gwiazda2:");
//    int prev = 0;
//    int diff = 0;
//    for (int y = 0; y < 200; y++) {
//        diff = (int) (gwiazda2 + y) - prev;
//        printf("\n %d. %d różnica: %d", y, (gwiazda2 + y), diff);
//        prev = (int) (gwiazda2 + y);
//    }
//}
