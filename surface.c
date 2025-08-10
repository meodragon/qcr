//
// Created by meo-dragon on 7/30/2025.
//
#include <stdio.h>

#include "surface.h"

#ifdef _WIN32
const char g_szClassName[] = "qcrWindowClass";

LRESULT CALLBACK MSG_CALLBACK(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    SURFACE *ptr;
    if (msg == WM_CREATE) // upon window creation, bind window state with window handle
    {
        CREATESTRUCTW* createStructW = (CREATESTRUCTW*)lparam;
        ptr = (SURFACE*)createStructW->lpCreateParams;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)ptr);
    } else // get window state pointer for every event
    {
        ptr = (SURFACE*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    }

    switch(msg) {
    case WM_LBUTTONDOWN:
        {
            char szFileName[MAX_PATH];
            GetModuleFileName(GetModuleHandle(NULL), szFileName, MAX_PATH);
            printf("this program is: %s\n", szFileName);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        // Draw something here...
        break;
    case WM_SIZE:
        {
            ptr->width = LOWORD(lparam);
            ptr->height = HIWORD(lparam);
            printf("hwnd wxh: %dx%d\n", ptr->width, ptr->height);
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
#else
#endif

// Utils
int message_loop();

int init_surface(SURFACE* surface)
{
#ifdef _WIN32
    WNDCLASSEX wc;

    surface->h_instance = GetModuleHandle(NULL);

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = MSG_CALLBACK;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = surface->h_instance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;

    wc.hIcon  = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_QCR_ICON));
    wc.hIconSm  = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_QCR_ICON), IMAGE_ICON, 16, 16, 0);

    if(!RegisterClassEx(&wc))
    {
        printf("Window Registration Failed! Error!");
        return 1;
    }

    // Step 2: Creating the Window
    surface->hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "qcr",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL, NULL, surface->h_instance, surface);

    if(surface->hwnd == NULL) {
        printf("Window Creation Failed! Error!");
        return 2;
    }

    ShowWindow(surface->hwnd, SW_NORMAL);
    UpdateWindow(surface->hwnd);

    surface->run = message_loop;
    return 0;
#else
    return -1;
#endif
}

void free_surface(SURFACE* surface)
{
#ifdef _WIN32
    DestroyWindow(surface->hwnd);
#else
#endif
}

int message_loop()
{
#ifdef _WIN32
    // Step 3: The Message Loop
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("msg.wParam: %llu\n", msg.wParam);

    if (msg.wParam == 0) printf("window closed\n");

    return 0;
#else
    return -1;
#endif
}