#include "StdAfx.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ::CreateMutex(NULL, TRUE, TEXT("CDangoHelper"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        TCHAR szText[MAX_PATH], szTitle[MAX_PATH];
        ::LoadString(hInstance, IDS_SINGLE, szText, _countof(szText));
        ::LoadString(hInstance, IDR_MAIN, szTitle, _countof(szTitle));
        return ::MessageBox(HWND_DESKTOP, szText, szTitle, MB_ICONWARNING | MB_TOPMOST);
    }

    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    ::DefWindowProc(NULL, 0, 0, 0L);

    CMainFrm wndMain;
    HWND hWnd = wndMain.Create(hInstance, HWND_DESKTOP);
    if (NULL == hWnd) return S_FALSE;

    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    HACCEL hAccMain = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN));

    MSG msg;
    // 主消息循环:
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(hWnd, hAccMain, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    ::CoUninitialize();

    return (int)msg.wParam;
}