/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
 
/* 
 * File:   main.cpp
 * Author: BCO
 *
 * Created on 21 lutego 2016, 13:23
 */

#include "COknoGL.h"
//#define _WIN32_WINNT 0x0500//dla Get Console Window
#include <windows.h>
#include "OdczytajGwiazdy.h"
#include "ObliczeniaGwiazd.h"
#include "RysujGwiazdy.h"


COknoGL okno;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow)//główna funkcja winAPI
{
    //ukrycie okna konsoli
    HWND uKonsoli =GetConsoleWindow();
//   ShowWindow( uKonsoli, SW_HIDE );
    RysujGwiazdy * rysujGwiazdy;
    rysujGwiazdy = new RysujGwiazdy();//wskaźnik ten jest odczytywany przez COknoGL w COknoGL::WndProc case : WM_CREATE za pomocą metody statycznej
    
//    for(int z=-128; z<255; z++)
//    {
//            std::cout << z << " " << char(z) << "." << std::endl;
//    }
//    system("pause");
    
    rysujGwiazdy->Inicjuj();
    
    POINT rozmiarOkna = {640, 480};
        POINT polozenieOkna = {(GetSystemMetrics(SM_CXSCREEN) - rozmiarOkna.x) / 2,( GetSystemMetrics(SM_CYSCREEN) - rozmiarOkna.y) / 2};
    if (!okno.Init(hInstance, polozenieOkna, rozmiarOkna)) {
        MessageBox(NULL, "Inicjacja okna nie powiod�a si�", "Aplikacja OpenGL",
                MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    } else return okno.Run();
   delete rysujGwiazdy;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return okno.ProceduraZasadnicza(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK ProceduraOknaGL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return okno.ProceduraOknaGL(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK ProceduraOknaDoKontrolek(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return okno.ProceduraOknaKontrolek(hWnd, message, wParam, lParam);
}

