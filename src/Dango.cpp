#include "StdAfx.h"
#include "MainFrm.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HRESULT hr = S_OK;
    MSG msg = {};
    TCHAR szText[MAX_PATH];
    CMainFrm wndMain;

    HANDLE hMutex = ::CreateMutex(NULL, TRUE, TEXT("CDangoHelper"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        ::LoadString(hInstance, IDS_RUNNING, szText, _countof(szText));
        return ::MessageBox(HWND_DESKTOP, szText, NULL, MB_ICONWARNING);
    }
    
    hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    HR_CHECK(hr);

    HWND hWnd = wndMain.Create(hInstance, HWND_DESKTOP);
    BOOL_CHECK(hWnd);

    ::ShowWindow(hWnd, nCmdShow);

    HACCEL hAccMain = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN));
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
    ::CloseHandle(hMutex);
exit:
    if (FAILED(hr))
    {
        TCHAR szFormat[MAX_PATH];
        ::LoadString(hInstance, IDS_ERROR, szFormat, _countof(szFormat));
        _stprintf_s(szText, _countof(szText), szFormat, hr);
        wndMain.MessageBox(szText, MB_ICONERROR);
    }
    return (int)msg.wParam;
}