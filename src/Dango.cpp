#include "StdAfx.h"
#include "MainFrm.h"

int Run(int nCmdShow)
{
    CMainFrm wndMain;
    HWND hWnd = wndMain.Create(HWND_DESKTOP);
    if (nullptr == hWnd) return ::GetLastError();
    // 主消息循环:
    MSG msg = {};
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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

    HANDLE hMutex = ::CreateMutex(nullptr, TRUE, TEXT("CDangoHelper"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CAtlString szText;
        szText.LoadString(IDS_RUNNING);
        return ::MessageBox(HWND_DESKTOP, szText, CMainFrm::GetWndCaption(), MB_ICONWARNING);
    }

    HRESULT hr = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    ATLASSERT(SUCCEEDED(hr));

    hr = _Module.Init(nullptr, hInstance);
    ATLASSERT(SUCCEEDED(hr));

    int nRet = Run(nCmdShow);

    _Module.Term();
    ::CoUninitialize();
    ::CloseHandle(hMutex);
    return nRet;
}