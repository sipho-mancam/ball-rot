#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <math.h>

#define IDB_START 200
#define IDB_LEFT 201
#define IDB_RIGHT 202
#define IDB_LINES 203
#define ID_TIMER 301

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HPEN hPen;
TCHAR className [] = _T("Main Window");
void DrawBalls(HWND hwnd, int k);
void DrawLines(HWND hwnd, int k);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wclex;


    wclex.hInstance = hInstance;
    wclex.lpfnWndProc = WindowProc;
    wclex.lpszClassName = className;
    wclex.style = CS_DBLCLKS;
    wclex.lpszMenuName = NULL;
    wclex.cbSize = sizeof(WNDCLASSEX);

    //How the class looks and some Icons and cursors
    wclex.hbrBackground = CreateSolidBrush(RGB(108, 180, 180));
    wclex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wclex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wclex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wclex.cbWndExtra = 0;
    wclex.cbClsExtra = 0;

    if(!RegisterClassEx(&wclex))return 2;

    hwnd = CreateWindowEx(0,
                          className,
                          _T("Rotating Balls"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          600, 540,
                          HWND_DESKTOP,
                          (HMENU)NULL,
                          hInstance,
                          NULL);


    ShowWindow(hwnd, nCmdShow);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND control[10];
    static int balls = 2, toggle=1, lines =1;

    switch(msg)
    {
    case WM_CREATE:
        control[0] = CreateWindowW(L"Button", L"Start", WS_CHILD | WS_VISIBLE, 5, 5, 70, 30, hwnd, (HMENU)IDB_START, NULL, NULL);
        control[1] = CreateWindowW(L"Button", L"<", WS_CHILD | WS_VISIBLE, 80, 5, 70, 30, hwnd, (HMENU)IDB_LEFT, NULL, NULL);
        control[2] = CreateWindowW(L"Button", L">", WS_CHILD | WS_VISIBLE, 155, 5, 70, 30, hwnd, (HMENU)IDB_RIGHT, NULL, NULL);
        control[3] = CreateWindowW(L"Button", L"Lines", WS_CHILD | WS_VISIBLE, 235, 5, 70, 30, hwnd, (HMENU)IDB_LINES, NULL, NULL);

        hPen = CreatePen(PS_SOLID, 3, RGB(0,0,250));

        break;
    case WM_COMMAND:
        if(HIWORD(wParam)==BN_CLICKED)
        {
            switch(LOWORD(wParam))
            {
            case IDB_START:
                if(toggle)
                {
                    SetTimer(hwnd, ID_TIMER, 30, (TIMERPROC)NULL);
                    SetWindowTextW(control[0], L"Stop");
                    toggle = 0;
                }
                else
                {
                    toggle = 1;
                    SetWindowTextW(control[0], L"Start");
                    KillTimer(hwnd, ID_TIMER);
                }
                break;
            case IDB_LEFT:
                if(balls> 2)balls -=2;
                break;
            case IDB_RIGHT:
                if(balls<20)balls +=2;
                break;
            case IDB_LINES:
                if(lines)
                {
                    //SetTimer(hwnd, ID_TIMER, 30, (TIMERPROC)NULL);
                    SetWindowTextW(control[3], L"Erase");
                    lines = 0;
                }
                else
                {
                    lines = 1;
                    SetWindowTextW(control[3], L"Lines");
                    //KillTimer(hwnd, ID_TIMER);
                }
                break;
            }
        }
        break;

    case WM_TIMER:

        if(lines)DrawBalls(hwnd, balls);
        else DrawLines(hwnd, balls);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


void DrawBalls(HWND hwnd, int k)
{
    HDC hdc;
    hdc = GetDC(hwnd);
    RECT clearScreen;
    SetRect(&clearScreen, 40, 40, 240, 240);
    FillRect(hdc, &clearScreen, CreateSolidBrush(RGB(108, 180, 180)));
    HBRUSH circleBrush = CreateSolidBrush(RGB(255, 0, 0));

    static int xPos[8], yPos[8], t=0;

    if(t>=100)t=0;

    SelectObject(hdc, circleBrush);

    for(int i=0; i<k; i++)
    {
        xPos[i] = 130 + 80 * cos(t/100.0 * 2*M_PI + i/((k) * 1.0) * M_PI ) * cos(i/((k)*1.0)* M_PI);
        yPos[i] = 130 + 80 * cos(t/100.0 * 2*M_PI + i/((k) * 1.0) * M_PI ) * sin(i/((k)*1.0)* M_PI);

        Ellipse(hdc, xPos[i]-10, yPos[i]-10, xPos[i]+10, yPos[i]+10);
     }

     ReleaseDC(hwnd, hdc);
     t++;

}

void DrawLines(HWND hwnd, int k)
{
    HDC hdc;
    hdc = GetDC(hwnd);
    static int xLine, yLine;

    RECT clearScreen;
    SetRect(&clearScreen, 40, 40, 240, 240);
    FillRect(hdc, &clearScreen, CreateSolidBrush(RGB(108, 180, 180)));

    SelectObject(hdc, hPen);
     for(int j=0; j< 2*k; j++)
     {
         MoveToEx(hdc, 130, 130, NULL);
         xLine = 130 - 80* sin(j/(2*k* 1.0)* 2* M_PI);
         yLine = 130 - 80* cos(j/(2*k* 1.0)* 2* M_PI);

         LineTo(hdc, xLine, yLine);
     }

     HBRUSH circleBrush = CreateSolidBrush(RGB(255, 0, 0));

    static int xPos[8], yPos[8], t=0;

    if(t>=100)t=0;

    SelectObject(hdc, circleBrush);

    for(int i=0; i<k; i++)
    {
        xPos[i] = 130 + 80 * cos(t/100.0 * 2*M_PI + i/((k) * 1.0) * M_PI ) * cos(i/((k)*1.0)* M_PI);
        yPos[i] = 130 + 80 * cos(t/100.0 * 2*M_PI + i/((k) * 1.0) * M_PI ) * sin(i/((k)*1.0)* M_PI);

        Ellipse(hdc, xPos[i]-10, yPos[i]-10, xPos[i]+10, yPos[i]+10);
     }
     t++;
     ReleaseDC(hwnd, hdc);
}
