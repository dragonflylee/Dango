#include "StdAfx.h"
#include "WICImage.h"
#include "WidgetFrm.h"
#include "MainFrm.h"

#define WM_ICON WM_USER + 180

UINT CMainFrm::WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));
TCHAR CMainFrm::szTitle[MAX_PATH] = { 0 };

// 开机启动相关注册表
LPCTSTR _szRegRun = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
LPCTSTR _szStartup = TEXT("Dango");

LRESULT CMainFrm::OnCreate()
{
    HRESULT hr = S_OK;
    
    // 加载图片
    CWICImage img;
    HR_CHECK(img.Load(m_hInst, MAKEINTRESOURCE(IDR_MAIN), RT_RCDATA));
    HR_CHECK(m_layered.UpdateLayered(m_hWnd, CRenderTarget(img)));

    // 初始化配置文件
    HR_CHECK(CConfig::Init());

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
    // 读取开启启动
    OnStartUp(FALSE);

    // 读取总在最前配置
    HWND hAfter = HWND_TOP;
    if (CConfig::Main().StayOnTop())
    {
        ::CheckMenuItem(m_hMenu, IDM_TOP, MF_CHECKED);
        hAfter = HWND_TOPMOST;
    }
    // 读取坐标
    BOOL_CHECK(::SetWindowPos(m_hWnd, hAfter, CConfig::Main().Left(), CConfig::Main().Top(), 0, 0, SWP_NOSIZE | SWP_NOACTIVATE));
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainFrm::OnDestroy()
{
    RECT rcWnd;
    ::GetWindowRect(m_hWnd, &rcWnd);
    CConfig::Main().Left(rcWnd.left);
    CConfig::Main().Top(rcWnd.top);

    // 删除托盘图标
    ::Shell_NotifyIcon(NIM_DELETE, &m_ncd);

    // 释放菜单资源
    if (NULL != m_hMenu) ::DestroyMenu(m_hMenu);

    ::PostQuitMessage(0);
    return S_OK;
}

LRESULT CMainFrm::OnStayOnTop()
{
    UINT nCheck = ::GetMenuState(m_hMenu, IDM_TOP, MF_BYCOMMAND) ^ MF_CHECKED;
    ::CheckMenuItem(m_hMenu, IDM_TOP, nCheck);
    CConfig::Main().StayOnTop(nCheck);
    return ::SetWindowPos(m_hWnd, nCheck ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

LRESULT CMainFrm::OnStartUp(BOOL bSet)
{
    HRESULT hr = S_OK;
    HKEY hReg = NULL;
    DWORD uOptions = KEY_QUERY_VALUE | KEY_WOW64_64KEY;
    if (bSet) uOptions |= KEY_SET_VALUE;
    HR_CHECK(HRESULT_FROM_WIN32(::RegOpenKeyEx(HKEY_CURRENT_USER, _szRegRun, 0, uOptions, &hReg)));

    hr = HRESULT_FROM_WIN32(::RegQueryValueEx(hReg, _szStartup, 0, NULL, NULL, NULL));
    if (!bSet) 
    {
        ::CheckMenuItem(m_hMenu, IDM_STARTUP, SUCCEEDED(hr) ? MF_CHECKED : MF_UNCHECKED);
    }
    else if (SUCCEEDED(hr))
    {
        HR_CHECK(HRESULT_FROM_WIN32(::RegDeleteValue(hReg, _szStartup)));
        ::CheckMenuItem(m_hMenu, IDM_STARTUP, MF_UNCHECKED);
    }
    else
    {
        TCHAR szPath[MAX_PATH] = { 0 };
        DWORD nSize = ::GetModuleFileName(m_hInst, szPath, _countof(szPath));
        HR_CHECK(HRESULT_FROM_WIN32(::RegSetValueEx(hReg, _szStartup, 0, REG_SZ, (const BYTE*)szPath, nSize * sizeof(TCHAR))));
        ::CheckMenuItem(m_hMenu, IDM_STARTUP, MF_CHECKED);
    }
exit:
    if (NULL != hReg) ::RegCloseKey(hReg);
    return hr;
}

LRESULT CMainFrm::OnOpenImage()
{
    TCHAR szFilter[MAX_PATH], szImage[MAX_PATH] = { 0 };
    ::LoadString(m_hInst, IDS_FILTER, szFilter, _countof(szFilter));
    for (LPTSTR pCurStr = szFilter; *pCurStr != TEXT('\0'); pCurStr++) if (*pCurStr == TEXT('|')) *pCurStr = TEXT('\0');
 
    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrTitle = CMainFrm::GetWndCaption();
    ofn.lpstrFilter = szFilter;
    ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.nMaxFile = _countof(szImage);
    ofn.lpstrFile = szImage;

    if (!::GetOpenFileName(&ofn)) return FALSE;

    CWidgetFrm *pWidget = new CWidgetFrm(szImage);
    HWND hWnd = pWidget->Create(m_hInst, m_hWnd);
    if (NULL != hWnd) return ::ShowWindow(hWnd, SW_SHOW);
    delete pWidget;
    return FALSE;
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
            return ::PostMessage(hWnd, WM_CLOSE, 0, 0L);
        case IDM_ABOUT:
            return ::DialogBoxParam(m_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, CMainFrm::AboutDialogProc, (LPARAM)this);
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
            return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 0), TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
        }
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
    case WM_DISPLAYCHANGE:
    {
        RECT rcWnd;
        ::GetWindowRect(m_hWnd, &rcWnd);
        return m_layered.SetPosition(m_hWnd, (LPPOINT)&rcWnd);
    }
    case WM_CONTEXTMENU:
        // 右键菜单
        return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 0), TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hWnd, NULL);
    case WM_DESTROY:
        return OnDestroy();
    }
    if (CMainFrm::WM_TASKBARCREATED == uMsg)
    {
        return ::Shell_NotifyIcon(NIM_ADD, &m_ncd);
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}