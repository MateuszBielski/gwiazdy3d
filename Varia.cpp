//#include "OknoGL.h"
#include <stdio.h>//potrzebne do funkcji vsprint
#include "Varia.h"

float* Roznica3fv(float punkt1[3],float punkt2[3],float wynik[3])
{
       for (int i=0;i<3;i++)
       {wynik[i]=punkt2[i]-punkt1[i];}
       return wynik;
}

float* IloczynWektorowy3fv(float a[3],float b[3], float wynik[3])
{
       const int x=0;
       const int y=1;
       const int z=2;
       
       wynik[x]= a[y]*b[z]-a[z]*b[y];
       wynik[y]= -(a[x]*b[z]-a[z]*b[x]);
       wynik[z]=a[x]*b[y]-a[y]*b[x];
       return wynik;
}

#define SQR(x) ((x)*(x))


float DlugoscWektora3fv(float wektor[3])
{
      float dlugosc=0;
      for (int i=0;i<3;i++){dlugosc+=SQR(wektor[i]);}
      return sqrt(dlugosc);
}

float* NormujWektor3fv(float wektor[3])
{
      float wsp=DlugoscWektora3fv(wektor);
      for (int i=0;i<3;i++){wektor[i]/=wsp;}
      return wektor;
}

float* JednostkowyWektorNormalny3fv(float punkt1[3],float punkt2[3],float punkt3[3],float wynik[3])
{
       float wektor12[3],wektor13[3];
       return NormujWektor3fv(IloczynWektorowy3fv(Roznica3fv(punkt1,punkt2,wektor12),
                                                  Roznica3fv(punkt1,punkt3,wektor13),wynik));
}

float* TransformujPikselDoPrzestrzeniSceny(POINT pozycjaPiksela,float glebokosc,float*  pozycjaPunktu3D)
{
       //viewport i macierze
       int viewport[4]; glGetIntegerv(GL_VIEWPORT,viewport);
       double macierzModelWidok[16];
       glGetDoublev(GL_MODELVIEW_MATRIX,macierzModelWidok);
       double macierzRzutowania[16];
       glGetDoublev(GL_PROJECTION_MATRIX,macierzRzutowania);
       
       //wsp�rz�dne sceny
       double wspolrzednePunktu3D[3];
       
       //transformacja
       gluUnProject((double)pozycjaPiksela.x,(double)( viewport[3]-pozycjaPiksela.y),
                   glebokosc,macierzModelWidok,macierzRzutowania,viewport,
                    &wspolrzednePunktu3D[0],
                    &wspolrzednePunktu3D[1],
                    &wspolrzednePunktu3D[2]);
                    
       //konwersja z double na float
       for (int i=0;i<3;i++)
       {pozycjaPunktu3D[i]=(float)wspolrzednePunktu3D[i];}
       return pozycjaPunktu3D;
} 

unsigned int StworzCzcionke(bool czyCzcionka3D, HWND uchwytOkna, char* NazwaCzcionki,
                            int wysokoscWPikselach,bool Pogrubiona,bool Kursywa,
                            long kodPierwszegoZnaku,long kodOstatniegoZnaku)
{
  //tworzy list� na pe�en zestaw czcionek
    unsigned int indeksPierwszejListy = glGenLists(kodOstatniegoZnaku+1-kodPierwszegoZnaku);

    //funkcja WinAPI (GDI); tworzy obiekt czcionki
    HFONT uchwytCzcionki = CreateFont(
                           wysokoscWPikselach,//wysoko�� czcionki
                           0,//u�ywam szeroko�ci czcionki proporcjonalnej do wysoko�ci
                           20,//nachylenie czcionek
                           15,//k�t
                           Pogrubiona?FW_BOLD:FALSE,//pogrubienie
                           Kursywa?TRUE:FALSE,//kursywa
                           FALSE,//podkre�lenie
                           FALSE,//przekre�lenie
                           DEFAULT_CHARSET,//zbi�r liter ANSI // ANSI_CHARSET
                           OUT_TT_PRECIS,//czcionki true type
                           CLIP_DEFAULT_PRECIS,//domy�lna precyzja przycianania rozmiaru
                           ANTIALIASED_QUALITY,//antyaliasing przy tworzeniu obraz�w
                           FF_DONTCARE|DEFAULT_PITCH,//styl i dekoracja czcionki
                           NazwaCzcionki);//nazwa czcionki
    
    HDC uchwytDC=GetDC(uchwytOkna);
    HFONT uchwytCzcionkiDC=(HFONT)SelectObject(uchwytDC,uchwytCzcionki);
    long ileZnakow=kodOstatniegoZnaku+1-kodPierwszegoZnaku;
    printf("\nStworz czcionke ile znakow %d",ileZnakow);
    //Funkcja WGL (specyficzna dla Windows)
    if (!czyCzcionka3D)
    {
       wglUseFontBitmaps(uchwytDC,kodPierwszegoZnaku,
                         ileZnakow,
                         indeksPierwszejListy);
       
    }
    else
    {
        wglUseFontOutlines(uchwytDC,
                           kodPierwszegoZnaku,
                           kodOstatniegoZnaku+1-kodPierwszegoZnaku,
                           indeksPierwszejListy,
                           0.0f,//precyzja budowania czcionki
                           0.2f,//g��boko�� czcionki
                           WGL_FONT_POLYGONS,
                           NULL);//informacje o rozmiarach czcionek
    }
    SelectObject(uchwytDC,uchwytCzcionkiDC);  //wyb�r czcionki, kt�r� stworzyli�my
    DeleteObject(uchwytCzcionki);             //Usuwanie pomocniczego(?)

    //czcionkaZainicjowana=true;    
    return indeksPierwszejListy;              //=indeks tablicy list
}
void Pisz(char* napis,int iloscLiter,unsigned int czcionka,int kodPierwszegoZnaku)
{
     if (napis==NULL||napis==" ") return;
     
     glPushAttrib(GL_LIST_BIT);                      //Odk�ada na stos atrybuty wyswietlania
     glListBase(czcionka-kodPierwszegoZnaku);        //Ustawia podstaw� znak�w
     glCallLists(iloscLiter,GL_UNSIGNED_BYTE,napis); //Wy�wietla kolejno listy liter(napis)
     glPopAttrib();                                  //przywraca ze stosu atrybuty wy�wietlania
}
void PiszZnaki(unsigned int czcionka){
     glPushAttrib(GL_LIST_BIT);                      //Odk�ada na stos atrybuty wyswietlania
     glListBase(czcionka-32);
     int pocz,kon,i;
     pocz=32;
     kon=255;
     i=pocz;
     unsigned int litery[kon-pocz];
     while(i<kon)litery[i-pocz]=i++;
     glCallLists(kon-pocz,GL_UNSIGNED_INT,litery); //Wy�wietla kolejno listy liter(napis)
//     char litery[]={"dfgę"};
//     glCallLists(4,GL_UNSIGNED_BYTE,litery); //Wy�wietla kolejno listy liter(napis)
//     glCallLists(98);
     glPopAttrib();
}
void Wyswietlaj(unsigned int czcionka,int kodPierwszegoZnaku, const char * napis, ...)
{
     char tekst[248];
     va_list ap;
     if (napis==NULL||napis==" ") return;
     
     va_start(ap,napis);
     vsprintf(tekst,napis,ap);
     va_end(ap);
     
     Pisz(tekst,strlen(tekst),czcionka, kodPierwszegoZnaku);
     return;
}

unsigned long* WczytajTeksture(HWND uchwytOkna, char* nazwaPliku, int& teksturaSzer,
                               int& teksturaWys, unsigned char alfa)
{
//nie trzeba zwaraca� informacji o obrazie, bo zawsze zwracamy tablice RGBA
      try
      {
           //czytanie bitmapy z pliku
           if (!filetest(nazwaPliku)) throw std::invalid_argument("Brak pliku");
           
           HBITMAP uchwytObrazu = (HBITMAP)LoadImage(GetModuleHandle(NULL),nazwaPliku,IMAGE_BITMAP,0,0,
                                   LR_LOADFROMFILE | LR_CREATEDIBSECTION);
           //informacja o bitmapie
           
           BITMAP obraz;
           GetObject(uchwytObrazu,sizeof(obraz),&obraz);
           teksturaSzer=obraz.bmWidth;
           teksturaWys=obraz.bmHeight;
           
           
           unsigned long* tekstura= new unsigned long[teksturaSzer*teksturaWys];
           
           switch(obraz.bmBitsPixel)
           {
                case 24:
                     {
                     unsigned char* tekstura24bppBRG=new unsigned char[obraz.bmWidthBytes*teksturaWys];
                     memcpy(tekstura24bppBRG,obraz.bmBits,teksturaSzer*teksturaWys*3);
                     //gdybym zwr�ci� tekstura24bppBRG zrzutowane na unsigned long*,
                     //to powinienem u�ywa� do wy�wietlania GL_BGR_EXT
                     
                     //konwersja na RGBA
                     for (int ih=0;ih<teksturaWys;ih++)
                         {
                         for (int iw=0;iw<teksturaSzer;iw++)
                             {
                                  int i=3*iw+(ih*obraz.bmWidthBytes);//uwzgl�dnia uzupe�nianie do WORD
                                  unsigned char A=alfa;
                                  unsigned char B=tekstura24bppBRG[i];
                                  unsigned char G=tekstura24bppBRG[i+1];
                                  unsigned char R=tekstura24bppBRG[i+2];
                                  tekstura[iw+(ih*teksturaSzer)]=(A<<24)+(B<<16)+(G<<8)+(R);                                 
                                  
                             }
                          }
                     delete [] tekstura24bppBRG;
                     tekstura24bppBRG=0;
                     }
                     break;
                     
                case 1://monochromatyczne
                     {
                     unsigned char* tekstura1bppMONO=new unsigned char[obraz.bmWidthBytes*teksturaWys];
                     memcpy(tekstura1bppMONO,obraz.bmBits,teksturaSzer*teksturaWys);
                                          
                     //konwersja na RGBA
                     for (int ih=0;ih<teksturaWys;ih++)
                         {
                         for (int iw=0;iw<teksturaSzer;iw++)
                             {
                                  int i=iw/8+(ih*obraz.bmWidthBytes);//uwzgl�dnia uzupe�nianie do WORD
                                  int numerBitu=iw % 8;                                  
                                  unsigned char A=alfa;
                                  bool bitZapalony=((tekstura1bppMONO[i]<<numerBitu)&128)==128;
                                  //ignorujemy palete i tworzymy obraz czarnobia�y
                                  unsigned char B=bitZapalony?255:0;
                                  unsigned char G=bitZapalony?255:0;
                                  unsigned char R=bitZapalony?255:0;
                                  tekstura[iw+(ih*teksturaSzer)]=(A<<24)+(B<<16)+(G<<8)+(R);                                 
                                  
                             }
                          }
                     delete [] tekstura1bppMONO;
                     }
                     break;
                
                case 8://256 kolor�w, wymaga palety barw (tabeli kolor�w)
                     {
                        unsigned char* tekstura8bppPalette=new unsigned char[obraz.bmWidthBytes*teksturaWys];
                        memcpy(tekstura8bppPalette,obraz.bmBits,obraz.bmWidthBytes*teksturaWys);
                        //pobranie tebeli kolor�w (pomijamy czytanie BITMAPINFO)
                        HDC uchwyt=CreateCompatibleDC(GetDC(uchwytOkna));
                        SelectObject (uchwyt,uchwytObrazu);
                        RGBQUAD tabelaKolorow[256];
                        GetDIBColorTable(uchwyt,0,256,tabelaKolorow);
                        
                         for (int ih=0;ih<teksturaWys;ih++)
                         {
                         for (int iw=0;iw<teksturaSzer;iw++)
                             {
                                  int i=iw+(ih*obraz.bmWidthBytes);//uwzgl�dnia uzupe�nienie do WORD
                                  
                                  unsigned char R=255-tabelaKolorow[tekstura8bppPalette[i]].rgbRed; 
                                  unsigned char G=255-tabelaKolorow[tekstura8bppPalette[i]].rgbGreen;
                                  unsigned char B=255-tabelaKolorow[tekstura8bppPalette[i]].rgbBlue;
                                  unsigned char A=(R+G+B)/3;
                                  tekstura[iw+(ih*teksturaSzer)]=(A<<24)+(B<<16)+(G<<8)+(R);  
                             }
                         }
                         delete[] tekstura8bppPalette;   
                     }
                     break;
                case 4://16 kolor�w (paleta barw jak w Paint)
                     {
                        unsigned char* tekstura4bppPalette=new unsigned char[obraz.bmWidthBytes*teksturaWys];
                        memcpy(tekstura4bppPalette,obraz.bmBits,obraz.bmWidthBytes*teksturaWys);
                        //pobranie tebeli kolor�w (pomijamy czytanie BITMAPINFO)
                        HDC uchwyt=CreateCompatibleDC(GetDC(uchwytOkna));
                        SelectObject (uchwyt,uchwytObrazu);
                        RGBQUAD tabelaKolorow[16];
                        GetDIBColorTable(uchwyt,0,16,tabelaKolorow);
                        
                         for (int ih=0;ih<teksturaWys;ih++)
                         {
                         for (int iw=0;iw<teksturaSzer;iw++)
                             {
                                  int i=iw/2+(ih*obraz.bmWidthBytes);//uwzgl�dnia uzupe�nienie do WORD
                                  bool pierwszaPolowaBajtu=!(iw%2);
                                  unsigned char A=alfa;
                                  int numerKoloruZPalety=(pierwszaPolowaBajtu)?((tekstura4bppPalette[i] & 0xF0)>>4):
                                                                               (tekstura4bppPalette[i] & 0x0F);
                                  
                                  unsigned char R=tabelaKolorow[numerKoloruZPalety].rgbRed; 
                                  unsigned char G=tabelaKolorow[numerKoloruZPalety].rgbGreen;
                                  unsigned char B=tabelaKolorow[numerKoloruZPalety].rgbBlue;
                                  tekstura[iw+(ih*teksturaSzer)]=(A<<24)+(B<<16)+(G<<8)+(R);  
                             }
                         }
                         delete[] tekstura4bppPalette;   
                     }
                     break;
                default: throw std::exception(); break;
           } 
            
           DeleteObject(uchwytObrazu);
           return tekstura;
                
      } 
      catch (const std::exception& exc)
      {
            char komunikat[256]="B��d podczas pobierania tekstury:\n";
            strcat(komunikat,exc.what());
            MessageBox(NULL,komunikat,"B��d teksturowania",MB_OK | MB_ICONERROR);
            return NULL;
            }                              
}

bool filetest(const char* filename)
{
     FILE* plik_test;
     if ((plik_test=fopen(filename,"r"))==NULL)return false;
     else fclose(plik_test);
     return true;
 }
