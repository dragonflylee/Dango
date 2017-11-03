#include "StdAfx.h"
#include "WICImage.h"
#include "WidgetFrm.h"
#include "MainFrm.h"

#define WM_ICON WM_USER + 180

UINT CMainFrm::WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));

LRESULT CMainFrm::OnCreate()
{
    HRESULT hr = S_OK;
    CWICImage img;

    // 初始化托盘图标
    ZeroMemory(&m_ncd, sizeof(m_ncd));
    m_ncd.cbSize = sizeof(m_ncd);
    m_ncd.hIcon = ::LoadIcon(m_hInst, MAKEINTRESOURCE(IDR_MAIN));
    m_ncd.hWnd = m_hWnd;
    m_ncd.uID = IDR_MAIN;
    m_ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
    m_ncd.uCallbackMessage = WM_ICON;
    m_ncd.dwInfoFlags = NIIF_USER;
    BOOL_CHECK(::Shell_NotifyIcon(NIM_ADD, &m_ncd));

    // 加载菜单
    m_hMenu = ::LoadMenu(m_hInst, MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(m_hMenu);

    // 从获配置文件保存路径
    BOOL_CHECK(::SHGetSpecialFolderPath(m_hWnd, m_config, CSIDL_APPDATA, TRUE));
    BOOL_CHECK(::PathCombine(m_config, m_config, TEXT("Dango.ini")));

    // 加载图片
    HR_CHECK(img.Load(m_hInst, MAKEINTRESOURCE(IDR_MAIN), RT_RCDATA));
    HR_CHECK(m_layered.UpdateLayered(m_hWnd, img));
   
    // 读取坐标
    RECT rcWnd;
    rcWnd.left = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Left"), 0, m_config);
    rcWnd.top = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Top"), 0, m_config);
    rcWnd.right = rcWnd.left + m_layered.GetWidth();
    rcWnd.bottom = rcWnd.top + m_layered.GetHeight();

    // 读取总在最前配置
    HWND hAfter = HWND_TOP;
    if (::GetPrivateProfileInt(TEXT("Main"), TEXT("TopMost"), 0, m_config))
    {
        ::CheckMenuItem(m_hMenu, IDM_TOPMOST, MF_CHECKED);
        hAfter = HWND_TOPMOST;
    }
    HR_CHECK(SetWindowPos(&rcWnd, SWP_NOSIZE, hAfter));
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainFrm::OnDestroy()
{
    TCHAR szValue[10];
    RECT rc;
    ::GetWindowRect(m_hWnd, &rc);
    _stprintf_s(szValue, _countof(szValue), TEXT("%d"), rc.top);
    ::WritePrivateProfileString(TEXT("Main"), TEXT("Top"), szValue, m_config);
    _stprintf_s(szValue, _countof(szValue), TEXT("%d"), rc.left);
    ::WritePrivateProfileString(TEXT("Main"), TEXT("Left"), szValue, m_config);

    // 删除托盘图标
    ::Shell_NotifyIcon(NIM_DELETE, &m_ncd);

    // 释放菜单资源
    if (NULL != m_hMenu) ::DestroyMenu(m_hMenu);

    ::PostQuitMessage(0);
    return S_OK;
}

LRESULT CMainFrm::OnContext()
{
    POINT pt;
    ::GetCursorPos(&pt);
    return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 0), TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
}

LRESULT CMainFrm::OnTopMost()
{
    if (::GetMenuState(m_hMenu, IDM_TOPMOST, MF_BYCOMMAND) & MF_CHECKED)
    {
        ::CheckMenuItem(m_hMenu, IDM_TOPMOST, MF_UNCHECKED);
        ::WritePrivateProfileString(TEXT("Main"), TEXT("TopMost"), TEXT("0"), m_config);
        return ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    else
    {
        ::CheckMenuItem(m_hMenu, IDM_TOPMOST, MF_CHECKED);
        ::WritePrivateProfileString(TEXT("Main"), TEXT("TopMost"), TEXT("1"), m_config);
        return ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
}

LRESULT CMainFrm::OnChangeImage()
{
    TCHAR szFilter[MAX_PATH], szImage[MAX_PATH] = { 0 };
    ::LoadString(m_hInst, IDS_FILTER, szFilter, _countof(szFilter));
    for (LPTSTR pCurStr = szFilter; *pCurStr != TEXT('\0'); pCurStr++) if (*pCurStr == TEXT('|')) *pCurStr = TEXT('\0');
 
    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrTitle = m_szTitle;
    ofn.lpstrFilter = szFilter;
    ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.nMaxFile = _countof(szImage);
    ofn.lpstrFile = szImage;

    if (!::GetOpenFileName(&ofn)) return FALSE;

    m_pWidget = new CWidgetFrm(szImage);
    HWND hWnd = m_pWidget->Create(m_hInst, m_hWnd);
    if (NULL == hWnd) return FALSE;
    return ::ShowWindow(hWnd, SW_SHOW);
}

INT_PTR CALLBACK CMainFrm::AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        // 设置对话框的图标
        HICON hIcon = ::LoadIcon(((CMainFrm *)lParam)->m_hInst, MAKEINTRESOURCE(IDR_MAIN));
        return ::PostMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
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
            return ::PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        case IDM_ABOUT:
            return ::DialogBoxParam(m_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, CMainFrm::AboutDialogProc, (LPARAM)this);
        case IDM_OPENIMG:
            return OnChangeImage();
        case IDM_TOPMOST:
            return OnTopMost();
        }
        break;
    case WM_ICON:
        switch (lParam)
        {
        case WM_CONTEXTMENU:
        case WM_RBUTTONUP:
            return OnContext();
        case WM_LBUTTONUP:
            ::ShowWindow(hWnd, SW_SHOW);
            return ::OpenIcon(hWnd);
        }
        break;
    case WM_CLOSE:
    {
        TCHAR szText[MAX_PATH];
        ::LoadString(NULL, IDS_EXIT, szText, _countof(szText));
        if (MessageBox(szText, MB_ICONQUESTION | MB_OKCANCEL) == IDOK) break;
        return 0;
    }
    case WM_LBUTTONDOWN:
        // 鼠标点击拖动窗口
        ::PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        break;
    case WM_RBUTTONUP:
        // 右键菜单
        return OnContext();
    case WM_DISPLAYCHANGE:
    {
        RECT rcWnd;
        ::GetWindowRect(m_hWnd, &rcWnd);
        return SetWindowPos(&rcWnd, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    case WM_DESTROY:
        return OnDestroy();
    }
    if (CMainFrm::WM_TASKBARCREATED == uMsg)
    {
        return ::Shell_NotifyIcon(NIM_ADD, &m_ncd);
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}