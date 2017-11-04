#include "StdAfx.h"
#include "MainFrm.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CMainFrm wndMain;

    ::CreateMutex(NULL, TRUE, TEXT("CDangoHelper"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        TCHAR szText[MAX_PATH];
        ::LoadString(hInstance, IDS_RUNNING, szText, _countof(szText));
        return wndMain.MessageBox(szText, MB_ICONWARNING | MB_TOPMOST);
    }

    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    ::DefWindowProc(NULL, 0, 0, 0L);

    HWND hWnd = wndMain.Create(hInstance, HWND_DESKTOP);
    if (NULL == hWnd)
    {
        DWORD dwError = ::GetLastError();
        TCHAR szFormat[MAX_PATH], szText[MAX_PATH];
        ::LoadString(hInstance, IDS_ERROR, szFormat, _countof(szFormat));
        _stprintf_s(szText, _countof(szText), szFormat, dwError);
        return wndMain.MessageBox(szText, MB_ICONERROR | MB_TOPMOST);
    }

    ::ShowWindow(hWnd, nCmdShow);

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