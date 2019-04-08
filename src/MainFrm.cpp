#include "StdAfx.h"
#include "WICImage.h"
#include "WidgetFrm.h"
#include "MainFrm.h"

#define WM_ICON WM_USER + 180

UINT CMainFrm::WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));
UINT CMainFrm::WM_WIDGETDESTROYED = WM_USER + WM_DESTROY;
TCHAR CMainFrm::szTitle[MAX_PATH] = { 0 };

// 开机启动相关注册表
LPCTSTR _szRegRun = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
LPCTSTR _szStartup = TEXT("Dango");

LRESULT CMainFrm::OnCreate(LPCREATESTRUCT pParam)
{
    HRESULT hr = S_OK;
    LPTSTR hName = nullptr;
    // 初始化配置文件
    HR_CHECK(CConfig::Init(hName));

    hInst = pParam->hInstance;
    // 允许拖放文件
    ::DragAcceptFiles(hWnd, TRUE);
    // 初始化托盘图标
    ZeroMemory(&ncd, sizeof(ncd));
    ncd.cbSize = sizeof(ncd);
    ncd.hIcon = ::LoadIcon(pParam->hInstance, MAKEINTRESOURCE(IDR_MAIN));
    ncd.hWnd = hWnd;
    ncd.uID = IDR_MAIN;
    ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
    ncd.uCallbackMessage = WM_ICON;
    ncd.dwInfoFlags = NIIF_USER;
    BOOL_CHECK(::Shell_NotifyIcon(NIM_ADD, &ncd));
    // 加载菜单
    hMenu = ::GetSubMenu(::LoadMenu(pParam->hInstance, MAKEINTRESOURCE(IDR_MAIN)), 0);
    BOOL_CHECK(hMenu);
    // 读取开启启动
    OnStartUp(FALSE);
    // 处理子窗口
    for (LPTSTR szImage = hName; *szImage; szImage += _tcslen(szImage) + 1)
    {
        if (::PathFileExists(szImage) && CConfig::Widget(szImage).Show())
        {
            CreaateWidget(szImage);
        }
    }
exit:
    if (nullptr != hName) ::LocalFree(hName);
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainFrm::OnDestroy()
{
    // 删除托盘图标
    ::Shell_NotifyIcon(NIM_DELETE, &ncd);
    // 释放菜单资源
    if (nullptr != hMenu) ::DestroyMenu(hMenu);
    ::PostQuitMessage(0);
    return S_OK;
}

LRESULT CMainFrm::OnStayOnTop()
{
    UINT nCheck = ::GetMenuState(hMenu, IDM_TOP, MF_BYCOMMAND) ^ MF_CHECKED;
    ::CheckMenuItem(hMenu, IDM_TOP, nCheck);
    CConfig::Main().StayOnTop(nCheck);
    return ::SetWindowPos(hWnd, nCheck ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

LRESULT CMainFrm::OnStartUp(BOOL bSet)
{
    HRESULT hr = S_OK;
    HKEY hReg = nullptr;
    DWORD uOptions = KEY_QUERY_VALUE | KEY_WOW64_64KEY;
    if (bSet) uOptions |= KEY_SET_VALUE;
    HR_CHECK(HRESULT_FROM_WIN32(::RegOpenKeyEx(HKEY_CURRENT_USER, _szRegRun, 0, uOptions, &hReg)));

    hr = HRESULT_FROM_WIN32(::RegQueryValueEx(hReg, _szStartup, 0, nullptr, nullptr, nullptr));
    if (!bSet)
    {
        ::CheckMenuItem(hMenu, IDM_STARTUP, SUCCEEDED(hr) ? MF_CHECKED : MF_UNCHECKED);
    }
    else if (SUCCEEDED(hr))
    {
        HR_CHECK(HRESULT_FROM_WIN32(::RegDeleteValue(hReg, _szStartup)));
        ::CheckMenuItem(hMenu, IDM_STARTUP, MF_UNCHECKED);
    }
    else
    {
        TCHAR szPath[MAX_PATH] = { 0 };
        DWORD nSize = ::GetModuleFileName(nullptr, szPath, _countof(szPath));
        HR_CHECK(HRESULT_FROM_WIN32(::RegSetValueEx(hReg, _szStartup, 0, REG_SZ, (const BYTE*)szPath, nSize * sizeof(TCHAR))));
        ::CheckMenuItem(hMenu, IDM_STARTUP, MF_CHECKED);
    }
exit:
    if (nullptr != hReg) ::RegCloseKey(hReg);
    return hr;
}

LRESULT CMainFrm::OnOpenImage()
{
    TCHAR szFilter[MAX_PATH], szImage[MAX_PATH] = { 0 };
    ::LoadString(nullptr, IDS_FILTER, szFilter, _countof(szFilter));
    for (LPTSTR pCurStr = szFilter; *pCurStr != TEXT('\0'); pCurStr++) if (*pCurStr == TEXT('|')) *pCurStr = TEXT('\0');

    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = hWnd;
    ofn.lpstrTitle = CMainFrm::GetWndCaption();
    ofn.lpstrFilter = szFilter;
    ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.nMaxFile = _countof(szImage);
    ofn.lpstrFile = szImage;
    if (!::GetOpenFileName(&ofn)) return FALSE;

    return CreaateWidget(szImage);
}

BOOL CMainFrm::CreaateWidget(LPCTSTR szImage)
{
    pWidget = new CWidgetFrm(pWidget, szImage);
    HWND hWidget = pWidget->Create(hInst, hWnd);
    if (nullptr != hWidget) return ::ShowWindow(hWidget, SW_SHOW);

    TCHAR szText[MAX_PATH] = {};
    ::FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
        ::GetLastError(), 0, szText, _countof(szText), NULL);
    return MessageBox(szText, MB_ICONERROR);
}

INT_PTR CALLBACK CMainFrm::AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        // 设置对话框的图标
        HICON hIcon = ::LoadIcon(reinterpret_cast<CMainFrm*>(lParam)->hInst, MAKEINTRESOURCE(IDR_MAIN));
        return ::PostMessage(hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
    }
    case WM_COMMAND:
        return EndDialog(hWnd, LOWORD(wParam));
    }
    return 0;
}

LRESULT CMainFrm::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_EXIT:
            return ::PostMessage(hWnd, WM_CLOSE, 0, 0L);
        case IDM_ABOUT:
            return ::DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, 
                CMainFrm::AboutDialogProc, reinterpret_cast<LPARAM>(this));
        case IDM_OPEN:
            return OnOpenImage();
        case IDM_TOP:
            return OnStayOnTop();
        case IDM_STARTUP:
            return OnStartUp(TRUE);
        }
        break;
    case WM_ICON:
        switch (lParam)
        {
        case WM_CONTEXTMENU:
        case WM_RBUTTONUP:
        {
            POINT pt;
            ::GetCursorPos(&pt);
            return ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hWnd, nullptr);
        }
        case WM_LBUTTONUP:
            ::ShowWindow(hWnd, SW_SHOW);
            return ::OpenIcon(hWnd);
        }
        break;
    case WM_CLOSE:
    {
        TCHAR szText[MAX_PATH];
        ::LoadString(hInst, IDS_EXIT, szText, _countof(szText));
        if (MessageBox(szText, MB_ICONQUESTION | MB_OKCANCEL) == IDOK) break;
        return 0;
    }
    case WM_LBUTTONDOWN:
        // 鼠标点击拖动窗口
        ::PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        break;
    case WM_CONTEXTMENU:
        // 右键菜单
        return ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hWnd, nullptr);
    case WM_DESTROY:
        return OnDestroy();
    }
    if (CMainFrm::WM_TASKBARCREATED == uMsg)
    {
        return ::Shell_NotifyIcon(NIM_ADD, &ncd);
    }
    else if (CMainFrm::WM_WIDGETDESTROYED == uMsg)
    {
        delete reinterpret_cast<CWidgetFrm *>(lParam);
        return S_OK;
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}