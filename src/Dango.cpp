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
    MSG msg = { };
    CAtlString szText;
    CMainFrm wndMain;

    HANDLE hMutex = ::CreateMutex(NULL, TRUE, TEXT("CDangoHelper"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        szText.LoadString(IDS_RUNNING);
        return ::MessageBox(HWND_DESKTOP, szText, CMainFrm::GetWndCaption(), MB_ICONWARNING);
    }
    
    hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    HR_CHECK(hr);

    hr = _Module.Init(NULL, hInstance);
    HR_CHECK(hr);

    HWND hWnd = wndMain.Create(HWND_DESKTOP);
    BOOL_CHECK(hWnd);
    wndMain.ShowWindow(nCmdShow);
    // 主消息循环:
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    _Module.Term();
    ::CoUninitialize();

    ::CloseHandle(hMutex);
exit:
    if (FAILED(hr))
    {
        szText.Format(IDS_ERROR, hr);
        wndMain.MessageBox(szText, CMainFrm::GetWndCaption(), MB_ICONERROR);
    }
    return (int)msg.wParam;
}