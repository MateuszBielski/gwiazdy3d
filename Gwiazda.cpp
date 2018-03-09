/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <deque>

#include "Gwiazda.h"

void Gwiazda::WyswietlHip() {
    printf("\nhip %d", hip);
}

int Gwiazda::OdlegloscDoCenrtum() {
    return (int) round(sqrt(x0 * x0 + x1 * x1));
}

int Gwiazda::Odleglosc() {
    int wynik = -1;
    if (plx != 0)wynik = (int) round(326212 / plx);
    return wynik;
}


void Gwiazda::WyswietlParametry(short * par) {

    short ile = 0;
    while (par[ile] >= 0) ile++; //zmierzyć ilość parametrów

    //wskaźniki do funkcji wewnętrznych
    int (Gwiazda::*wi[12])();
    wi[0] = &Gwiazda::Hip;
    wi[1] = &Gwiazda::Comp_id;
    wi[2] = &Gwiazda::X0;
    wi[3] = &Gwiazda::X1;
    wi[4] = &Gwiazda::B_v;
    wi[5] = &Gwiazda::Mag;
    wi[6] = &Gwiazda::Sp_int;
    wi[7] = &Gwiazda::Dx0;
    wi[8] = &Gwiazda::Dx1;
    wi[9] = &Gwiazda::Plx;
    wi[10] = &Gwiazda::OdlegloscDoCenrtum;
    wi[11] = &Gwiazda::Odleglosc;
    /***********dopisując kolejny wskaźnik trzeba zwiększyć rozmiar tablicy*/
    for (int k = 0; k < ile; k++) {
        printf(", %d", (this->*wi[par[k]])()); //metody pokazujące kolejne parametry
    }
}

void Gwiazda::Ustaw(Gwiazda *g) {
    hip = g->Hip();
    comp_id = g->Comp_id();
    x0 = g->X0();
    x1 = g->X1();
    b_v = g->B_v();
    mag = g->Mag();
    sp_int = g->Sp_int();
    dx0 = g->Dx0();
    dx1 = g->Dx1();
    plx = g->Plx();
}
Gwiazda2::Gwiazda2(Gwiazda * g):nrPorzadkowyX(0),nrPorzadkowyY(0),nrPorzadkowyZ(0){
    Gwiazda::Ustaw(g);
//    itWliscieKomorek=0;
}
int Gwiazda2::Parametry(std::string& dW, char parametry){//dW - do wypełnienia
    std::ostringstream osH,osM,osA,osO,osWlk,osAwlk,osBV;
//strs << dbl;
//std::string str = strs.str();
    
    
    
    if(parametry & HIP){dW+=" Hip ";osH<<Hip();dW+=osH.str();}
    if(parametry & MAG){dW+=" mag ";osM<<Jm();dW+=osM.str(); }
    if(parametry & WLK_DO_WYSWIETLENIA){dW+=" wielkosc punktu";osWlk<<WielkoscDoWyswietlenia();dW+=osWlk.str(); }
    if(parametry & W_ABS){dW+=" wielkosc Abs ";osA<<JM();dW+=osA.str(); }
    if(parametry & ODL){dW+=" odleglosc ";osO<<Odleglosc();dW+=osO.str(); }
    if(parametry & ABS_WIELKOSC_DO_WYSWIETLENIA){dW+=" Absolutna wlk punkt ";osAwlk<<AbsolutnaWlkDoWyswietlenia();dW+=osAwlk.str(); }
    if(parametry & B_V){dW+=" Barwa ";osBV<<B_v();dW+=osBV.str(); }
    return Hip();
}
int Gwiazda2::Parametry(std::deque<std::string>& dParametry, char parametry){//dW - do wypełnienia
    dParametry.clear();
    std::ostringstream osH,osM,osA,osO,osWlk,osAwlk,osBV,osDYS;
    std::string linia;
    
    if(parametry & HIP){linia=" Hip ";osH<<Hip();dParametry.push_back(linia+osH.str());}
    if(parametry & MAG){linia=" mag ";osM<<Jm();dParametry.push_back(linia+osM.str()); }
    if(parametry & WLK_DO_WYSWIETLENIA){linia=" wielkosc punktu";osWlk<<WielkoscDoWyswietlenia();dParametry.push_back(linia+osWlk.str()); }
    if(parametry & W_ABS){linia=" wielkosc Abs ";osA<<JM();dParametry.push_back(linia+osA.str()); }
    if(parametry & ODL){linia=" odleglosc ";osO<<Odleglosc();dParametry.push_back(linia+osO.str()); }
    if(parametry & ABS_WIELKOSC_DO_WYSWIETLENIA){linia=" Absolutna wlk punkt ";osAwlk<<AbsolutnaWlkDoWyswietlenia();dParametry.push_back(linia+osAwlk.str()); }
    if(parametry & B_V){linia=" Barwa ";osBV<<B_v();dParametry.push_back(linia+osBV.str()); }
    //jeśli jest nazwa
    if(nazwa.size())dParametry.push_front(nazwa);
//    if(parametry & B_V){linia=" Dystans do środka ";osBV<<B_v();dParametry.push_back(linia+osBV.str()); }
    return Hip();
}

void Gwiazda2::Ustaw(Gwiazda * g, float m, float M, float odl) {
    //ustawić trzeba parametry klasy bazowej
    Gwiazda::Ustaw(g);
    //oraz magnituda wyliczone i inne później
    odleglosc = odl;
    magnitudo = m;
    Magnitudo = M;
}
void Gwiazda2::Ustaw(float m, float M, float odl,float wlk,float AbsWlk) {
    //
    //oraz magnituda wyliczone i inne później
    odleglosc = odl;
    magnitudo = m;
    Magnitudo = M;
    wielkoscDoWyswietlenia = wlk;
    absolutnaWielkoscDoWyswietlenia = AbsWlk;
}
void Gwiazda2::UstawWspolrzedne(double * w,double zmniejszenie){
    /*      opis próby*/
    /* pionową osią obrotu jest Y*/
    
//    w[0];
//    w[1];
    double r;
    double katIobrot1[4]={-w[1],1,0,0};//deklinacja
    double katIobrot2[4]={w[0],0,1,0};//rektascencja
    //long lz=888945722*4;//2888945722;
 //   z=2.588945722e+9;
    z=2.888945722e+9;
    x=-this->X0()/zmniejszenie;//bo trójkąty się zmniejszają w miarę odczytu dlaszych plików
    y=this->X1()/zmniejszenie;
    r=sqrt(x*x+y*y+z*z);
    x=x/r;y=y/r;z=z/r;
    ObrocPunktd(x,y,z,katIobrot1);
    ObrocPunktd(x,y,z,katIobrot2);
    //od razu można zapisać położenie w przestrzeni
    ox=x*odleglosc;
    oy=y*odleglosc;
    oz=z*odleglosc;
    
    //w współrzędne biegunowe
    //z x0 i x1 utworzyć wektor, znormalizować i obrócić o kąty wynikłe ze wspó
    
}
void Gwiazda2::UstawNazwe(std::string& nazwaG){
    nazwa=nazwaG;
}
void Gwiazda2::UstawSymbol(std::string& symbolG){
    symbol=symbolG;
}
void Gwiazda2::PokazWspolrzedneXYZ(){
    printf("\nx=%2.4f, y=%2.4f, z=%2.4f",x,y,z);
}
double Gwiazda2::WielkoscDoWyswietlenia(){
    
    return (double)wielkoscDoWyswietlenia;
//    return (double)(1+pow((13-(this->Jm()))*0.22,2));
//    return (double)(1.8*(6-this->Jm()));
   
}
double Gwiazda2::AbsolutnaWlkDoWyswietlenia(){
    //największa gwiazda -10, najmniejsza 5
    //największy wyświetlany punkt 20 - wykorzystamy 15
   return (double)absolutnaWielkoscDoWyswietlenia;
//   return (double)(1+pow((15.0-this->JM())*0.15,2));
//   return (double)(6.0-this->JM());
}
float Gwiazda2::DystansDo(float cx,float cy,float cz){
//    float gx=(float)x*odleglosc;
//    float gy=(float)y*odleglosc;
//    float gz=(float)z*odleglosc;
//    
//    float c1x=cx;
//    float c1y=cy;
//    float c1z=cz;
//    //printf("\nGwiazda2::DystansDo gx %2.3f, gy %2.3f, gz %2.3f, cx %2.3f, cy %2.3f, cz %2.3f",gx,gy,gz,c1x,c1y,c1z);
//    float kwx=pow(c1x-gx,2);
//    float kwy=pow(c1y-gy,2);
//    float kwz=pow(c1z-gz,2);
//    return sqrt(kwx+kwy+kwz);
   
    float kwx=pow(cx-ox,2);
    float kwy=pow(cy-oy,2);
    float kwz=pow(cz-oz,2);
    return sqrt(kwx+kwy+kwz);
}

float Gwiazda2::KwadratDystansuDo(float cx, float cy, float cz) {
    float kwx=pow(cx-ox,2);
    float kwy=pow(cy-oy,2);
    float kwz=pow(cz-oz,2);
    return kwx+kwy+kwz;
    
}

float Gwiazda2::SkupienieNumerowPorzadkowych(){
    int suma=0;
    suma+=abs(nrPorzadkowyX-nrPorzadkowyY);
    suma+=abs(nrPorzadkowyX-nrPorzadkowyY);
    suma+=abs(nrPorzadkowyZ-nrPorzadkowyY);
    return (float)suma/3.0;
}
Gwiazdozbior::Gwiazdozbior(std::string nK,std::string n):nazwaSkrocona(nK),nazwa(n){
    if(nazwa.length()<3)nazwa=nazwaSkrocona;
    //printf("\n%s %s",nazwaSkrocona.c_str(),nazwa.c_str());
}

