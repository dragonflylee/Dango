#include "StdAfx.h"
#include "MainFrm.h"

int Run(int nCmdShow)
{
    CAtlString szText;
    CMainFrm wndMain;

    HWND hWnd = wndMain.Create(HWND_DESKTOP);
    if (NULL == hWnd)
    {
        szText.Format(IDS_ERROR, ::GetLastError());
        return wndMain.MessageBox(szText, CMainFrm::GetWndCaption(), MB_ICONERROR);
    }

    wndMain.ShowWindow(nCmdShow);
    // 主消息循环:
    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
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

    HANDLE hMutex = ::CreateMutex(NULL, TRUE, TEXT("CDangoHelper"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CAtlString szText;
        szText.LoadString(IDS_RUNNING);
        return ::MessageBox(HWND_DESKTOP, szText, CMainFrm::GetWndCaption(), MB_ICONWARNING);
    }

    HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    ATLASSERT(SUCCEEDED(hr));

    hr = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hr));

    int nRet = Run(nCmdShow);

    _Module.Term();
    ::CoUninitialize();
    ::CloseHandle(hMutex);
    return nRet;
}