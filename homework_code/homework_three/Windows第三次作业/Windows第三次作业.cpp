// Windows第三次作业.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Windows第三次作业.h"

#define MAX_LOADSTRING 100

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
    LoadStringW(hInstance, IDC_WINDOWS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWS));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /*
        1、cxclient： 客户区的宽度；
        2、cylient：  客户区的高度；
        3、x_top：    矩形的上边；
        4、x_buttom： 矩形的下边；
        5、y_left：   矩形的左边；
        6、y_right：  矩形的右边；
        7、flag：     这里是由于我起初运行代码时，无法获取屏幕的宽度、高度；所以我设置了flag变量用于以下处理：
                        1）起初运行，flag为true，客户区宽度、高度为自己设置的初始值；
                        2）当改变客户区大小，会触发WM_SIZE消息，从而获取客户区的真实宽度、高度，更新flag为flase；
                        3）如果没有上述问题的同学，可以不做此处理。
        8、temp_x：   临时存储客户区宽度；
        9、temp_y：   临时存储客户区高度；
        10、添加temp_x、temp_y的目的是为了能够实现改变客户区大小时也能改变矩形大小，而不是改变了客户区大小后，才会更新矩形大小
    */
    static int cxclient;
    static int cyclient;
    static double x_top;
    static double x_buttom;
    static double y_left;
    static double y_right;
    static bool flag;
    static int temp_x;
    static int temp_y;

    switch (message)
    {
    case WM_CREATE:
        cxclient = 1050;
        cyclient = 560;
        x_top = 0;
        x_buttom = cyclient / 10;
        y_left = cxclient * 7 / 16;
        y_right = cxclient * 9 / 16;
        flag = true;
        temp_x = cxclient;
        temp_y = cyclient;
        break;

    // 当客户区大小改变时，触发WM_SIZE消息，更新cxclient、cyclient、flag值
    case WM_SIZE:
        cxclient = LOWORD(lParam);
        cyclient = HIWORD(lParam);
        flag = false;
        break;

    // 响应按键消息
    case WM_KEYDOWN:
        flag = true;
        switch (wParam)
        {
        case VK_UP:
            // x_top = max(0, x_top - 5);
            // 注意当矩形移动到客户区边缘时，进行如下4种判断
            if (x_top - 5 >= 0) {
                x_top -= 5;
                x_buttom -= 5;
            }
            // InvalidateRect函数可以使整个客户区无效，从而除法WM_PAINT消息，善用！
            InvalidateRect(hWnd, NULL, true);
            break;
        case VK_DOWN:
            // x_buttom = min(cyclient, x_buttom + 5);
            if (x_buttom + 5 <= cyclient) {
                x_buttom += 5;
                x_top += 5;
            }
            InvalidateRect(hWnd, NULL, true);
            break;
        case VK_LEFT:
            // y_left = max(0, y_left - 5);
            if (y_left - 5 >= 0) {
                y_left -= 5;
                y_right -= 5;
            }
            InvalidateRect(hWnd, NULL, true);
            break;
        case VK_RIGHT:
            //y_right = min(cxclient, y_right + 5);
            if (y_right + 5 <= cxclient) {
                y_right += 5;
                y_left += 5;
            }
            InvalidateRect(hWnd, NULL, true);
            break;
        default:
            break;
        }
        /*
            1、此处需要注意（以我的电脑为例）：
            2、当我按下TAB/SHIFT键时，此时TAB/SHIFT键的状态为被按下（一直被按下），只要我按了另外一个键
            3、比如：我先按下TAB键，松开手指，在按下SHIFT键，那么系统会判定为两个键均为按下状态
            4、所以矩形会一直被更新在初始位置，所以需要快速、同时按下两个键，并松开
            5、代码书写可以按照自己的喜好来
        */
        if (GetKeyState(VK_TAB) && GetKeyState(VK_SHIFT)) {
        //if (GetKeyState(VK_TAB)) {
            if(GetKeyState(VK_SHIFT)) {
            x_top = 0;
            x_buttom = cyclient / 10;
            y_left = cxclient * 7 / 16;
            y_right = cxclient * 9 / 16;
            InvalidateRect(hWnd, NULL, true);
        }
        }
        break;

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
                DestroyWindow(hWnd);
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

            // 画矩形
            if (flag == true) {
                Rectangle(hdc, y_left, x_top, y_right, x_buttom);
            }
            // 
            else {
                y_left = y_left * cxclient / temp_x;
                y_right = y_right * cxclient / temp_x;
                x_top = x_top * cyclient / temp_y;
                x_buttom = x_buttom * cyclient / temp_y;
                Rectangle(hdc, y_left, x_top, y_right, x_buttom);
                temp_x = cxclient;
                temp_y = cyclient;
            }
            EndPaint(hWnd, &ps);
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
