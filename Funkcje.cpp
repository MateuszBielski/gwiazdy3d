/*
 *Autor: Mateusz Bielski
 */

#include "Funkcje.h"

bool wHip(Gwiazda & g1, Gwiazda & g2) {
    bool wynik = 0;
    if (g1.Hip() == g2.Hip())return wynik;
    wynik = (g1.Hip() > g2.Hip()); //powinno sortować rosnąco
    return wynik;
}

bool wPlx(Gwiazda & g1, Gwiazda & g2) {
    if (g1.Plx() == g2.Plx())return g1.Mag() < g2.Mag();
    return g1.Plx() < g2.Plx();
}

bool wOdlDoCentrum(Gwiazda & g1, Gwiazda & g2) {
    if (g1.OdlegloscDoCenrtum() == g2.OdlegloscDoCenrtum())return g1.Mag() < g2.Mag();
    return g1.OdlegloscDoCenrtum() < g2.OdlegloscDoCenrtum();
}

bool wMag(Gwiazda2 & g1, Gwiazda2 & g2) {
    if (g1.JM() == g2.JM())return g1.Mag() < g2.Mag();
    return g1.JM() < g2.JM();
}
bool wMagIteratorow(iteratorLG & itg1, iteratorLG & itg2) {
    if (itg1->JM() == itg2->JM())return itg1->Mag() < itg2->Mag();
    return itg1->JM() < itg2->JM();
}
bool wmag(Gwiazda2 & g1, Gwiazda2 & g2) {
    if (g1.Jm() == g2.Jm())return g1.JM() < g2.JM();
    return g1.Jm() < g2.Jm();
}

bool wOdl(Gwiazda2 & g1, Gwiazda2 & g2) {
    if (g1.Odleglosc() == g2.Odleglosc())return g1.Mag() < g2.Mag();
    return g1.Odleglosc() > g2.Odleglosc();
}
bool wgPierwotnejNumeracji(Gwiazda2 & g1,Gwiazda2 & g2){
    return g1.numeracjaPierwotna<g2.numeracjaPierwotna;
}
bool WzdluzX(Gwiazda2 & g1, Gwiazda2 & g2){
   
   if(g1.OX()==g2.OX())return g1.OY()<g2.OY();
   return g1.OX()<g2.OX();
}

bool WzdluzY(Gwiazda2 & g1, Gwiazda2 & g2){
    if(g1.OY()==g2.OY())return g1.OZ()<g2.OZ();
   return g1.OY()<g2.OY();
}

bool WzdluzZ(Gwiazda2 & g1, Gwiazda2 & g2){
    if(g1.OZ()==g2.OZ())return g1.OX()<g2.OX();
   return g1.OZ()<g2.OZ();
}
bool WgNazwy(Gwiazdozbior & gz1,Gwiazdozbior & gz2){
    return gz1.Nazwa()<gz2.Nazwa();
}

void Zamien(char * z) {
    short ile = sizeof (double);
    char tz[ile];
    for (int i = 0; i < ile; i++) {
        tz[7 - i] = z[i];
    }
    for (int i = 0; i < ile; i++) {
        z[i] = tz[i];
    }
}
void Zamien(char * z,short rozmiar) {
    char tz[rozmiar];
    for (int i = 0; i < rozmiar; i++) {
        tz[rozmiar-1 - i] = z[i];
    }
    for (int i = 0; i < rozmiar; i++) {
        z[i] = tz[i];
    }
}
int Minimum(float * tablicaWartosci,int * tablicaIndeksow,int lewyBrzeg,int rozmiarTablicy){
    //w zakresie tablicaWarotsci od lewegoBrzegu do konca znaleźć najmniejszą wartość i wskazać na której jest pozycji
    int indeksNajmniejszego=tablicaIndeksow[lewyBrzeg];
    int i=lewyBrzeg;
    float min=tablicaWartosci[indeksNajmniejszego];
    while(++i<rozmiarTablicy){
        if(tablicaWartosci[tablicaIndeksow[i]]<min){
            min=tablicaWartosci[tablicaIndeksow[i]];
            indeksNajmniejszego=i;
            
        }
    }
    //oraz zamienić elementy w tablicy wartości
    int temp=tablicaIndeksow[lewyBrzeg];
    tablicaIndeksow[lewyBrzeg]=indeksNajmniejszego;
    tablicaIndeksow[indeksNajmniejszego]=temp;
    return indeksNajmniejszego;
}
int Sortuj(float * tWartosci,int * tIndeksy,int lewyBrzeg,int rozmiar){
    //sorowanie przez wybieranie
    //na początek należy posortować przestawiając elementy w tablicy wartości, a potem tylko indeksy
    //najpierw szukanie minimum
    //printf("\nfunkcja sortuj");
    int wynik=0;
    int i=lewyBrzeg;
    int indeksNajmniejszego=i;
    float min=tWartosci[i];
    while(++i<rozmiar){
        if(tWartosci[i]<min){
            min=tWartosci[i];
            indeksNajmniejszego=i;
        }
    }
    //jeżeli minimum leżey poza pierwszym elementem, to trzeba je ze sobą zamienić
    if(indeksNajmniejszego!=lewyBrzeg){
        tWartosci[indeksNajmniejszego]=tWartosci[lewyBrzeg];
        tWartosci[lewyBrzeg]=min;
        wynik = 1;
    }
    
    if(++lewyBrzeg<rozmiar-1)wynik+=Sortuj(tWartosci,tIndeksy,lewyBrzeg,rozmiar);
    return wynik;
}
int SortujIndeksy(float * tWartosci,int * rekordyZwartosciami,int nrRekorduPoczatkowego,int rozmiar){
    //sorowanie przez wybieranie
    
    //najpierw szukanie minimum
    //printf("\nfunkcja sortuj");
    int wynik=0;
    int i=nrRekorduPoczatkowego;
    int rekordZnajmniejszaWartoscia=i;
    float min=tWartosci[rekordyZwartosciami[i]];
     while(++i<rozmiar){
        if(tWartosci[rekordyZwartosciami[i]]<min){
            min=tWartosci[rekordyZwartosciami[i]];
            rekordZnajmniejszaWartoscia=i;
        }
    }
    //jeżeli minimum leży poza pierwszym elementem, to trzeba je ze sobą zamienić
  
    if(nrRekorduPoczatkowego!=rekordZnajmniejszaWartoscia){
        int temp=rekordyZwartosciami[nrRekorduPoczatkowego];
        rekordyZwartosciami[nrRekorduPoczatkowego]=rekordyZwartosciami[rekordZnajmniejszaWartoscia];
        rekordyZwartosciami[rekordZnajmniejszaWartoscia]=temp;
        wynik = 1;
    }
	return wynik;
}
double Rad_To_Deg(double r){return r*180/3.14159;}
double Rad_To_hRekt(double r){return r*12/3.14159;} 
double Deg_To_Rad(double d){return d*3.14159/180;}
void xyz_to_LF(double * d, double* s) {
    d[0] = atan2(s[0], s[2]);
    d[1] = asin(s[1]);
    d[0] < 0 ? d[0] = 2 * 3.14159 + d[0] : d[0];
}
void ObrocPunktd(double & x0,double & y0,double & z0,double * katIobrot){
     double w[3];
    w[0]=x0;w[1]=y0;w[2]=z0;
	double sinFi=sin(katIobrot[0]);
	double cosFi=cos(katIobrot[0]);
	double x=katIobrot[1], y=katIobrot[2], z=katIobrot[3];
	double m[9];//macierz obrotu
	m[0]=cosFi+pow(x,2)*(1-cosFi);
	m[1]=x*y*(1-cosFi)-z*sinFi;
	m[2]=x*z*(1-cosFi)+y*sinFi;
	m[3]=y*x*(1-cosFi)+z*sinFi;
	m[4]=cosFi+pow(y,2)*(1-cosFi);
	m[5]=y*z*(1-cosFi)-x*sinFi;
	m[6]=z*x*(1-cosFi)-y*sinFi;
	m[7]=z*y*(1-cosFi)+x*sinFi;
	m[8]=cosFi+pow(z,2)*(1-cosFi);
	double x2,y2,z2;
	x0=m[0]*w[0]+m[1]*w[1]+m[2]*w[2];
	y0=m[3]*w[0]+m[4]*w[1]+m[5]*w[2];
	z0=m[6]*w[0]+m[7]*w[1]+m[8]*w[2];
	
}
void NormujWektord(double * w){
    double dlugosc;
    dlugosc=sqrt(w[0]*w[0]+w[1]*w[1]+w[2]*w[2]);
    w[0]/=dlugosc;
    w[1]/=dlugosc;
    w[2]/=dlugosc;
}
 void KatIWektorObrotuMiedzyWektoramid(double * wynik,double * w1,double * w2)
{
     NormujWektord(w1);
     NormujWektord(w2);
        double kat=acos(w1[0]*w2[0]+w1[1]*w2[1]+w1[2]*w2[2]);//iloczyn skalarny
	double x=w1[1]*w2[2]-w1[2]*w2[1];
	double y=-(w1[0]*w2[2]-w1[2]*w2[0]);
	double z=w1[0]*w2[1]-w1[1]*w2[0];
	double R=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	wynik[0]=kat;
	wynik[1]=x/R;
	wynik[2]=y/R;
	wynik[3]=z/R;
}