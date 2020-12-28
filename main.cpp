#include <iostream>
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
//#include <math.h>
#include "resource.h"
#include <cmath>

using namespace std;
HINSTANCE hInst;
HBRUSH brush;
HPEN greenPen;
COLORREF COLOR;
double DISTANCIA_M=999;
bool ONE = 1;
int ABCD = 65;
string HEX="J",HEY="A";

class NODO;
class HEURISTCA;
void DIBUJAR_CIRCULO_R(HDC,double,double,double,int);
void DIBUJAR_PUNTOS(HDC,double,double,int);
void DIBUJAR_UNION_PUNTOS(HDC,double,double,double,double,int);
void DIBUJAR_TABLA_COORD(HDC);

string MAYUSCULAS(string CADENA){
     for(int i = 0;CADENA[i];i++){
          if((int)CADENA[i]>96&&(int)CADENA[i]<123){
            CADENA[i] = CADENA[i]-32;
          }
     }
     return CADENA;
}

int CONVERTIR_STRING_A_CHAR(string H,char **CAD){int TAM=0,i=0;
    for(;H[TAM];TAM++);TAM+=1;
    *CAD = new char [TAM];
    for(;H[i];i++){
        *(*(CAD)+i) = H[i];
    }*(*(CAD)+i) = '\0';
    return i;
}

int CONCATENAR(string A,string B,char **R){int TAM=0,i=0;
    for(;A[TAM];TAM++);
    for(;B[i];i++,TAM++);TAM+=1;
    *R = new char [TAM];TAM=0;
    for(i=0;A[i];i++,TAM++){
        *(*(R)+TAM)=A[i];
    }
    for(i=0;B[i];i++,TAM++){
        *(*(R)+TAM)=B[i];
    }*(*(R)+TAM) = '\0';
    return TAM;
}

int CONVERTIR_DOUBLE_A_CAD(char **R,double DOBL,int PFLOAT){int NINT=0,DECI=0,i=0,TAM=0,DFLOAT=1;
    NINT=DOBL;for(;PFLOAT;PFLOAT--){DFLOAT*=10;}
    if(NINT==0){*R = new char [2];*(*(R)+0)='0';*(*(R)+1)='\0';return 1;}
    DECI=(DOBL-NINT)*DFLOAT;
    for(int AUX=NINT;AUX!=0;i++,AUX/=10);
    for(int AUX=DECI;AUX!=0;i++,AUX/=10);i+=1;
    *R = new char [i];TAM=i;*(*(R)+i)='\0';i-=1;
    for(;DECI;i--,DECI/=10){
        *(*(R)+i)=(char)(DECI%10+48);
    }*(*(R)+i)='.';i-=1;
    for(;NINT;i--,NINT/=10){
        *(*(R)+i)=(char)(NINT%10+48);
    }
    return TAM;
}

double DISTANCIA_PUNTOS(double x1,double y1,double x2,double y2){
    return sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
}

class NODO{
    public:
    string LNODO;//NOMBRE DE NODO
    double PX,PY;//POSISION EN EL ESPACIO
    double VALOR_HEURISTICO;

    NODO *SIG,*DR[10];//ENLACES
    double DISTANCIA[10];
    NODO(string N="",double x=0,double y=0){
        LNODO = N;
        PX = x;
        PY = y;
        SIG = NULL;
        DR[0] = NULL;
        DR[1] = NULL;
        DR[2] = NULL;
        DR[3] = NULL;
        DR[4] = NULL;
        DR[5] = NULL;
        DR[6] = NULL;
        DR[7] = NULL;
        DR[8] = NULL;
        DR[9] = NULL;
        VALOR_HEURISTICO = 999;
    }
};

class HEURISTCA{
    public:
    NODO *R1;
    HEURISTCA(){
        R1=NULL;
    }
    void INSERTAR_NODO(double x,double y){string N;N=ABCD;ABCD++;
//        if(ABCD<=90){
            INS(N,x,y,R1);
//        }else{
//            cout<<"CARACTERES MAXIMOS PERMITIDOS!";
//        }
    }
    void INS(string N,double x,double y,NODO *&R){
        if(R){
            INS(N,x,y,R->SIG);
        }else{
            R = new NODO(N,x,y);
        }
    }
    void ENLACE(string a,string b){
        E(R1,a,b);
    }
    void E(NODO *R,string a,string b){
        while(R){
            if(R->LNODO==a){
                F(R1,b,R);
                break;
            }
            R=R->SIG;
        }
    }
    void F(NODO *R,string b,NODO *S){
        while(R){
            if(R->LNODO==b){
                for(int i=0;i<10;i++){
                    if(!S->DR[i]){
                        S->DR[i]=R;
                        break;
                    }
                }
                break;
            }
            R=R->SIG;
        }
    }
    void ASIGNAR_DISTANCIA(){
        ASGD(R1);
    }
    void ASGD(NODO *R){
        while(R){
            for(int i=0;R->DR[i];i++){
                R->DISTANCIA[i]=DISTANCIA_PUNTOS(R->PX,R->PY,R->DR[i]->PX,R->DR[i]->PY);
            }
            R=R->SIG;
        }
    }

    void ASIGNAR_HEURISTICA(string I,string F){
        ASGH(R1,I,F);
    }
    void ASGH(NODO *R,string I,string F){

        while(R){
            if(I==R->LNODO){
                ASGH2(R1,R->PX,R->PY);
                break;
            }
            R=R->SIG;
        }
    }
    void ASGH2(NODO *B,double XX,double YY){
        while(B){
            B->VALOR_HEURISTICO=DISTANCIA_PUNTOS(XX,YY,B->PX,B->PY);
            B=B->SIG;
        }
    }
    void RECORRIDO_HEURISTICO(HDC hdc,string I,string F){
        ASIGNAR_HEURISTICA(I,F);
        REC_INI(hdc,I,F,R1);
    }
    bool REC_INI(HDC hdc,string I,string F,NODO *R){NODO *B=R;
        while(R){
            if(F==R->LNODO){
                if(!R->DR[0]){cout<<"No Tiene Enlace:\n";return 0;}
                 while(B){
                    if(I==B->LNODO){
                        if(!B->DR[0]){cout<<"No Tiene Enlace:\n";return 0;}
                    }
                    B=B->SIG;
                }
                return REC_H(R,I,F,hdc);
            }
            R=R->SIG;
        }return 0;
    }
    bool REC_H(NODO *R,string I,string F,HDC hdc){NODO *B=R,*ANTERIOR=R;
        while(R){DISTANCIA_M=999;
            cout<<"NODO: "<<R->LNODO<<"\t";
            for(int i=0;R->DR[i];i++){
                if(ANTERIOR->LNODO!=R->DR[i]->LNODO){
                    if(DISTANCIA_M>R->DR[i]->VALOR_HEURISTICO){
                        DISTANCIA_M = R->DR[i]->VALOR_HEURISTICO;
                        B=R->DR[i];
                    }
                }

            }
            if(DISTANCIA_M==0){
                cout<<" -> "<<B->LNODO<<" FIN!\n\n";
                DIBUJAR_UNION_PUNTOS(hdc,R->PX,R->PY,B->PX,B->PY,1);
                return 1;
            }
            DIBUJAR_UNION_PUNTOS(hdc,R->PX,R->PY,B->PX,B->PY,1);
            cout<<" -> "<<B->LNODO<<"\n";
            ANTERIOR=R;
            R=B;
        }
        return 0;
    }

    void MOSTRAR_LINEAS(HDC hdc){
        ML(R1,hdc);
    }

    void ML(NODO *R,HDC hdc){
        while(R){
            for(int i=0;R->DR[i];i++){
                DIBUJAR_UNION_PUNTOS(hdc,R->PX,R->PY,R->DR[i]->PX,R->DR[i]->PY,2);
            }
            DIBUJAR_PUNTOS(hdc,R->PX,R->PY,1);
            DIBUJAR_CIRCULO_R(hdc,R->PX,R->PY,0.3,0);
            R=R->SIG;
        }
    }

    void MOSTRAR_DATOS(HDC hdc){
        MDTS(R1,hdc);
    }
    void MDTS(NODO *R,HDC hdc){
        while(R){
            for(int i=0;R->DR[i];i++){char *CAD;
                int T = CONVERTIR_DOUBLE_A_CAD(&CAD,R->DISTANCIA[i],1);
                COLOR = SetTextColor(hdc, RGB(0,255,255));
                TextOut(hdc,(((R->PX+R->DR[i]->PX)/2)*20),(((R->PY+R->DR[i]->PY)/2)*-20)+615,CAD,T);
            }
            char *CAD1,*CAD2;
            int T1 = CONVERTIR_STRING_A_CHAR(R->LNODO,&CAD1);
            int T2 = CONVERTIR_DOUBLE_A_CAD(&CAD2,R->VALOR_HEURISTICO,1);
            COLOR = SetTextColor(hdc, RGB(255,0,0));
            TextOut(hdc,(R->PX*20)-5,(R->PY*-20)+605,CAD1,T1);
            COLOR = SetTextColor(hdc, RGB(255,100,255));
            TextOut(hdc,(R->PX*20)+10,(R->PY*-20)+605,CAD2,T2);
            R=R->SIG;
        }

        SetTextColor(hdc, COLOR);
    }
    void MOSTRAR_CMD(){
        M_CMD(R1);
    }
    void M_CMD(NODO *R){
        cout<<"DISTANCIA DE NODOS:\n";
        while(R){
            cout<<"NODO: "<<R->LNODO<<" ("<<R->PX<<","<<R->PY<<")     \t";
            for(int i=0;R->DR[i];i++){
                cout<<"CON "<<R->DR[i]->LNODO<<" d: ("<<R->DISTANCIA[i]<<"), ";
            }
            cout<<"\n";
            R=R->SIG;
        }
    }
}A;

int INICIALIZACION(){
    A.INSERTAR_NODO(4.8,9.1);
    A.INSERTAR_NODO(2.5,8);
    A.INSERTAR_NODO(3,4);
    A.INSERTAR_NODO(7,6);
    A.INSERTAR_NODO(6,1);
    A.INSERTAR_NODO(14,9);
    A.INSERTAR_NODO(10,7);
    A.INSERTAR_NODO(17,6);
    A.INSERTAR_NODO(12,3);
    A.INSERTAR_NODO(10,0.5);

    A.ENLACE("A","B");
    A.ENLACE("A","F");

    A.ENLACE("B","A");
    A.ENLACE("B","C");
    A.ENLACE("B","D");

    A.ENLACE("C","B");
    A.ENLACE("C","D");
    A.ENLACE("C","E");

    A.ENLACE("D","B");
    A.ENLACE("D","C");
    A.ENLACE("D","E");

    A.ENLACE("E","I");
    A.ENLACE("E","J");
    A.ENLACE("E","D");
    A.ENLACE("E","C");

    A.ENLACE("F","A");
    A.ENLACE("F","G");
    A.ENLACE("F","H");

    A.ENLACE("G","F");
    A.ENLACE("G","I");

    A.ENLACE("H","F");
    A.ENLACE("H","I");

    A.ENLACE("I","G");
    A.ENLACE("I","H");
    A.ENLACE("I","E");
    A.ENLACE("I","J");

    A.ENLACE("J","E");
    A.ENLACE("J","I");

//    A.MOSTRAR_CMD();

    return 0;
}


///GRAFICA

void DIBUJAR_CIRCULO_R(HDC hdc,double X,double Y,double RADIO,int COLOR){     ///GRAFICAR RADIO
        if(COLOR==0){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 0, 0));}
        else if(COLOR==1){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 0));}
        else if(COLOR==2){greenPen=CreatePen(PS_SOLID, 3, RGB(0, 0, 255));}
        else{
           greenPen=CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        }
        SelectObject(hdc,greenPen);

        X*=20;
        Y*=-20;
        RADIO*=20;

        Arc(hdc, 10+X-RADIO, 625+Y-RADIO, 10+X+RADIO, 625+Y+RADIO, 10, 10, 10, 10);

        DeleteObject(greenPen);
}

void DIBUJAR_PUNTOS(HDC hdc,double X,double Y,int COLOR){                     ///GRAFICAR PUNTOS
    if(COLOR==0){greenPen=CreatePen(PS_SOLID, 10, RGB(255, 0, 0));}
    else if(COLOR==1){greenPen=CreatePen(PS_SOLID, 10, RGB(255, 255, 0));}
    else if(COLOR==2){greenPen=CreatePen(PS_SOLID, 10, RGB(0, 0, 255));}
   // else if(color==4){greenPen=CreatePen(PS_SOLID, 15, RGB(255, 255, 255));}
    else{
       greenPen=CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
    }
    /// x 250-1190
    /// y 10-625

    X*=20;
    Y*=-20;

    SelectObject(hdc,greenPen);
    MoveToEx(hdc,10+X,625+Y,NULL);
    LineTo(hdc,10+X,625+Y);
    DeleteObject(greenPen);
}

void DIBUJAR_UNION_PUNTOS(HDC hdc,double X_INI,double Y_INI,double X,double Y,int COLOR){                     ///GRAFICAR PUNTOS
    if(COLOR==0){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 0, 0));}
    else if(COLOR==1){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 0));}
    else if(COLOR==2){greenPen=CreatePen(PS_SOLID, 3, RGB(0, 0, 255));}
   // else if(color==4){greenPen=CreatePen(PS_SOLID, 15, RGB(255, 255, 255));}
    else{
       greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    }
    /// x 250-1190
    /// y 10-625

    X*=20;
    Y*=-20;
    X_INI*=20;
    Y_INI*=-20;

    SelectObject(hdc,greenPen);
    MoveToEx(hdc,10+X_INI,625+Y_INI,NULL);
    LineTo(hdc,10+X,625+Y);
    DeleteObject(greenPen);
}

void DIBUJAR_TABLA_COORD(HDC hdc){                                  ///GRAFICAR LINEAS TABLA
    for(int i=30,j=0;j<46;j++,i+=20){
        MoveToEx(hdc,i,10,NULL);
        LineTo(hdc,i,625);
    }
    for(int i=605,j=0;j<30;j++,i-=20){
        MoveToEx(hdc,10,i,NULL);
        LineTo(hdc,950,i);
    }
}

void Visual(HDC hdc){
    SetBkMode(hdc, TRANSPARENT);
    greenPen=CreatePen(PS_SOLID, 1, RGB(70, 70, 70));
    SelectObject(hdc,greenPen);

    brush=CreateSolidBrush(RGB(67, 68, 70));    ///FONDO
    SelectObject(hdc,brush);
    Rectangle(hdc,-1,-1,1000,1000);


    brush=CreateSolidBrush(RGB(0, 0, 0));       ///COORDENADAS
    SelectObject(hdc,brush);
    Rectangle(hdc,10,10,950,625);

    DIBUJAR_TABLA_COORD(hdc);

    greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
    SelectObject(hdc,greenPen);
    brush=CreateSolidBrush(RGB(255, 255, 255)); ///L IZQUIERDO
    SelectObject(hdc,brush);
    Rectangle(hdc,10,10,950,105);

    DeleteObject(brush);
    DeleteObject(greenPen);
}

///GRAFICA
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        if(ONE){ONE=0;
            INICIALIZACION();
        }
    }
    return TRUE;
    case WM_PAINT:
        {
            HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);

            Visual(hdc);

            A.ASIGNAR_DISTANCIA();
            A.MOSTRAR_LINEAS(hdc);
            A.RECORRIDO_HEURISTICO(hdc,HEX,HEY);
            A.MOSTRAR_DATOS(hdc);

            EndPaint(hwndDlg,&ps);
        }
    return TRUE;
    case WM_LBUTTONDOWN:
        {
            double xPos = GET_X_LPARAM(lParam);
            double yPos = GET_Y_LPARAM(lParam);
            if(xPos<950&&xPos>10&&yPos<625&&yPos>10){
                A.INSERTAR_NODO(((xPos-10)/20),31-((yPos-10)/20));
                SetDlgItemText(hwndDlg,EDIT1,"INCERTADO");
                InvalidateRect(hwndDlg,NULL,true);
            }
        }
    return TRUE;
    case WM_MOUSEMOVE:
        {
        }
    return TRUE;
    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            char CADE1[10],CADE2[10];int Numx,Numy;

            case BTN_INSERTAR:

                GetDlgItemText(hwndDlg,EDITX,CADE1,10);//capturar evento
                GetDlgItemText(hwndDlg,EDITY,CADE2,10);
                Numx=atof(CADE1);
                Numy=atof(CADE2);

                if(Numx>0&&Numx<47&&Numy>0&&Numy<31){

                    A.INSERTAR_NODO(Numx,Numy);

                    SetDlgItemText(hwndDlg,EDITX,"");
                    SetDlgItemText(hwndDlg,EDITY,"");
                    SetDlgItemText(hwndDlg,EDIT1,"INCERTADO");
                    InvalidateRect(hwndDlg,NULL,true);
                }else{
                    SetDlgItemText(hwndDlg,EDIT1,"LIM EXEDIDO");
                }
            return TRUE;
            case BTN_ENLAZAR:
                GetDlgItemText(hwndDlg,EDITE1,CADE1,10);//capturar evento
                GetDlgItemText(hwndDlg,EDITE2,CADE2,10);

                A.ENLACE(MAYUSCULAS(CADE1),MAYUSCULAS(CADE2));
                A.ENLACE(MAYUSCULAS(CADE2),MAYUSCULAS(CADE1));
                SetDlgItemText(hwndDlg,EDITE1,"");
                SetDlgItemText(hwndDlg,EDITE2,"");
                SetDlgItemText(hwndDlg,EDIT1,"INCERTADO");
                InvalidateRect(hwndDlg,NULL,true);

            return TRUE;
            case BTN_CALCULAR:
                GetDlgItemText(hwndDlg,EDITEE1,CADE1,10);//capturar evento
                GetDlgItemText(hwndDlg,EDITEE2,CADE2,10);
                HEX = MAYUSCULAS(CADE2);
                HEY = MAYUSCULAS(CADE1);
                SetDlgItemText(hwndDlg,EDITEE1,"");
                SetDlgItemText(hwndDlg,EDITEE2,"");
                InvalidateRect(hwndDlg,NULL,true);
            return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
