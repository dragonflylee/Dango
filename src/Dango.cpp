#include "StdAfx.h"
#include "MainFrm.h"

int Run(HINSTANCE hInst, int nCmdShow)
{
    TCHAR szText[MAX_PATH];
    CMainFrm wndMain;
    HWND hWnd = wndMain.Create(hInst, HWND_DESKTOP);
    if (nullptr == hWnd)
    {
        TCHAR szFormat[MAX_PATH];
        DWORD dwError = ::GetLastError();
        ::LoadString(hInst, IDS_ERROR, szFormat, _countof(szFormat));
        _stprintf_s(szText, _countof(szText), szFormat, dwError);
        return wndMain.MessageBox(szText, MB_ICONERROR);
    }
    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);
    // 主消息循环:
    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    ::LoadString(hInstance, IDR_MAIN, CMainFrm::szTitle, _countof(CMainFrm::szTitle));

    HANDLE hMutex = ::CreateMutex(nullptr, TRUE, TEXT("CDangoHelper"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        TCHAR szText[MAX_PATH];
        ::LoadString(hInstance, IDS_RUNNING, szText, _countof(szText));
        return ::MessageBox(HWND_DESKTOP, szText, CMainFrm::GetWndCaption(), MB_ICONWARNING);
    }
    
    ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
   
    int nRet = Run(hInstance, nCmdShow);

    ::CoUninitialize();
    ::CloseHandle(hMutex);
    return nRet;
}