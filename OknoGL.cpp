

//void COknoGL::Oswietlenie()
//{
//     glEnable(GL_LIGHTING);//w��czenie systemu o�wietlenia
//     
//     const float kolor_tla[]={natezenie_swiatla_tla,
//                              natezenie_swiatla_tla,
//                              natezenie_swiatla_tla};
//     //glLightModelfv(GL_LIGHT_MODEL_AMBIENT,kolor_tla); 
//     
//     //materia�
//     glEnable(GL_COLOR_MATERIAL);
//     glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
//     //glPushMatrix();
//     //glLoadIdentity();
//     ZrodlaSwiatla();
//     //glPopMatrix();                                   
// }
//float COknoGL::WyswietlKorektaMacierzyModelWidok(int pozycja)
//{
//      if (pozycja>=0 && pozycja<16)
//      {return korektaMacierzyModelWidok[pozycja];}
//      else
//      {return 0;}
//}
//bool  COknoGL::DialogUtworzNoweOkno()
//{
//      
//    uOD[1]=CreateWindowEx(0,
//            WC_STATIC,//nazwa klasy okna
//            "Utw�rz nowe okno:",
//            WS_POPUP|WS_BORDER|WS_CAPTION,
//            312,284,400,200,uOkna1,
//            NULL,
//            uchwytAplikacji,NULL);
//    uOD[2]=CreateWindowEx(0,
//            WC_STATIC,//nazwa klasy okna
//            "Szeroko�� :",
//            WS_CHILD|WS_VISIBLE,
//            10,50,80,20,uOD[1],
//            NULL,
//            uchwytAplikacji,NULL);
//    uOD[3]=CreateWindowEx(WS_EX_CLIENTEDGE,
//            WC_EDIT,//nazwa klasy okna
//            "800",
//            WS_CHILD|WS_VISIBLE,
//            100,50,80,20,uOD[1],
//            NULL,
//            uchwytAplikacji,NULL);
//    uOD[4]=CreateWindowEx(0,
//            WC_STATIC,//nazwa klasy okna
//            "Wysoko�� :",
//            WS_CHILD|WS_VISIBLE,
//            10,80,80,20,uOD[1],
//            NULL,
//            uchwytAplikacji,NULL);
//    uOD[5]=CreateWindowEx(WS_EX_CLIENTEDGE,
//            WC_EDIT,//nazwa klasy okna
//            "600",
//            WS_CHILD|WS_VISIBLE,
//            100,80,80,20,uOD[1],
//            NULL,
//            uchwytAplikacji,NULL);
//    uOD[6]=CreateWindowEx(0,
//            WC_BUTTON,//nazwa klasy okna
//            "Utw�rz",
//            WS_CHILD|WS_VISIBLE,
//            160,140,80,20,uOD[1],
//            (HMENU)UTWORZ_OKNO_B,
//            uchwytAplikacji,NULL);
//    ShowWindow(uOD[1],5);  
//}
//bool  COknoGL::TworzenieNowegoOkna()
//{
//      char szerokoscO[6],wysokoscO[6];
//      HWND uOD[7];
//      GetWindowText(uOD[3],szerokoscO,5);
//      GetWindowText(uOD[5],wysokoscO,5);
//      uOD[7]=CreateWindowEx(WS_EX_CLIENTEDGE,
//            WC_STATIC,//nazwa klasy okna
//            szerokoscO,
//            WS_CHILD|WS_VISIBLE,
//            250,140,80,20,uOkna1,
//            NULL,
//            uchwytAplikacji,NULL);
//      ShowWindow(uOD[7],8);
//   //jeszcze wyzerowa� wszystkie wska�niki   
//}
//#include "MojeOkno.cpp"
