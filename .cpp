// WindowsProject1.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject1.h"
#include <valarray>
#include <cmath>


#define MAX_LOADSTRING 100
const int waitMs = 100;
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
namespace Gamemain {
    const double PI = 3.1415926535897932;
    struct speed_deg {
        double deg, sp;
    };
    struct ball {
        double x;
        double y;
        speed_deg speed;
        COLORREF color;
        wchar_t buff[256];//=L"..."
    };
    int ballsize;
    double ballspeeddown;
    ball bl[16];//黑球,1~15
    bool unused[16];
    static double sq(double d) {
        return d * d;
    }
    static double getx(speed_deg a) {
        return cos(a.deg * PI / 180) * a.sp;
    }
    static double gety(speed_deg a) {
        return sin(a.deg * PI / 180) * a.sp;
    }
    static bool eqi(double a, double b) {
        return fabs(a - b) < 0.001;
    }
    static bool AbigB(double a, double b) {
        return a > b || eqi(a, b);
    }
    static double checkdeg(double a) {
        if (a < 0)return 360 + a;
        return a;
    }
    static speed_deg minus(speed_deg a, speed_deg b) {
        double rex = getx(a) - getx(b);
        double rey = gety(a) - gety(b);
        speed_deg n;
        n.deg = checkdeg(atan2(rey, rex) * 180 / PI);
        n.sp = sqrt(sq(rex) + sq(rey));
        return n;
    }
    static speed_deg plus(speed_deg a, speed_deg b) {
        double rex = getx(a) + getx(b);
        double rey = gety(a) + gety(b);
        speed_deg n;
        n.deg = checkdeg(atan2(rey, rex) * 180 / PI);
        n.sp = sqrt(sq(rex) + sq(rey));
        return n;
    }
    static bool impact(int counta,int countb) {
        double x_ = bl[counta].x - bl[countb].x;
        double y_ = bl[counta].y - bl[countb].y;
        if (sq(2*ballsize) - sq(x_) - sq(y_) > 0 ) {
            speed_deg tmp = minus(bl[counta].speed, bl[countb].speed);
            double t = checkdeg(atan2(y_, x_));
            double degree_ = checkdeg(tmp.deg - t);
            if (AbigB(degree_, 90) && AbigB(270, degree_))return false;
            speed_deg a_pl; a_pl.deg = (degree_ < 180 ? 90 : 270), a_pl.sp = cos(degree_) * tmp.sp;
            speed_deg b_pl; b_pl.deg = 0, b_pl.sp = sin(degree_) * tmp.sp;
            a_pl = plus(bl[countb].speed, a_pl);
            b_pl = plus(bl[countb].speed, b_pl);
            bl[counta].speed = a_pl;
            bl[countb].speed = b_pl;
            //Sleep(waitMs);
            return true;
        }
        else {
            return false;
        }
    }
    static COLORREF inttocol(int n) {//[0,1529]
        if (n <= 255)return RGB(255, n, 0);
        n -= 255;
        if (n <= 255)return RGB(255 - n, 255, 0);
        n -= 255;
        if (n <= 255)return RGB(0, 255, n);
        n -= 255;
        if (n <= 255)return RGB(0, 255 - n, 255);
        n -= 255;
        if (n <= 255)return RGB(n, 0, 255);
        n -= 255;
        return RGB(255, 0, 255 - n);
    }
    static void ResetBall() {
        bl[0].color = RGB(255, 255, 255);
        bl[8].color = RGB(0, 0, 0);
        int siz = 1530,bls=7;
        for (int i = 1; i <= bls; ++i)bl[i].color = inttocol(siz / bls * (i - 1));
        for (int i = 1; i <= bls; ++i)bl[i+bls+1].color = inttocol(siz / bls * (i - 1));
        ballsize = 50;
        for (int i = 0; i <= 15; ++i) {
            bl[i].x = (i * 2 + 8) * ballsize, bl[i].y = 800;
            unused[i] = false;
        }
        ballspeeddown = 1;
    }
    static void DrawBall(HDC hdc, int size, int count) {

        HBRUSH hBrush1 = CreateSolidBrush(bl[count].color);

        SelectObject(hdc, hBrush1);
        Ellipse(hdc, bl[count].x - ballsize, bl[count].y - ballsize, bl[count].x + ballsize, bl[count].y + ballsize);
        HFONT hFont1 = CreateFontW(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
        SelectObject(hdc, hFont1);
        TextOut(hdc, bl[count].x - 10, bl[count].y - 24, bl[count].buff, wcslen(bl[count].buff));
    }
    static bool check() {
        bool k = false;
        for (int i = 0; i <= 15; ++i) {
            if (unused[i])continue;
            if (bl[i].speed.sp <= ballspeeddown)bl[i].speed.sp = 0;
            else bl[i].speed.sp -= ballspeeddown, k = true;
            bl[i].x += getx(bl[i].speed);
            bl[i].y += gety(bl[i].speed);
            if (bl[i].x < 0 || bl[i].y < 0 || bl[i].x>2000 || bl[i].y>500)unused[i] = true;
            //Sleep(waitMs);
        }
        if (k == false)return false;
        for (int i = 0; i <= 15; ++i) {
            if (unused[i])continue;
            //Sleep(waitMs);
            for (int j = i + 1; j <= 15; ++j) {
                if (unused[j])continue;
                impact(i, j);
            }
        }
        return true;
    }
    static void clear(HDC hdc, int a, int b) {
        HBRUSH __hBrush1 = CreateSolidBrush(RGB(200, 200, 200));
        SelectObject(hdc, __hBrush1);
        Rectangle(hdc, -1, -1, a, b);
    }
    static bool FlushBall(HDC hdc) {
        bool b = check(); if (b == 0)return false;
        clear(hdc, 2000, 2000);
        for (int i = 0; i <= 15; ++i) {
            DrawBall(hdc, ballsize, i);
        }
        return true;
    }
    static void PowerBall(speed_deg sp, int& b17, int& b815, bool getdowna,bool getdownb) {
        
    }
}
using namespace Gamemain;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                //DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Sleep(3000);
        bl[0].speed.sp = 100;
        bl[0].speed.deg = 30;
        ResetBall();
        while (1)FlushBall(hdc);
        ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
