#include "StdAfx.h"
#include "MainFrm.h"
#include "Live2DFrm.h"

UINT CMainFrm::WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));
UINT CMainFrm::WM_WIDGETDESTROYED = WM_USER + WM_DESTROY;
// 开机启动相关注册表
LPCTSTR _szRegRun = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
LPCTSTR _szStartup = TEXT("Dango");

CMainFrm::CMainFrm() : hMenu(nullptr)
{

}

LRESULT CMainFrm::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    ATL::CRegKey hReg;
    CAtlString szName;
    LPCREATESTRUCT pParam = reinterpret_cast<LPCREATESTRUCT>(lParam);
    CLive2DFrm *pL2D = nullptr;

    HRESULT hr = S_OK;
    // 初始化配置文件
    HR_CHECK(CConfig::Init(szName));
    // 初始化托盘图标
    ZeroMemory(&ncd, sizeof(ncd));
    ncd.cbSize = sizeof(ncd);
    ncd.hIcon = ::LoadIcon(pParam->hInstance, MAKEINTRESOURCE(IDR_MAIN));
    ncd.hWnd = m_hWnd;
    ncd.uID = IDR_MAIN;
    ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
    ncd.uCallbackMessage = WM_ICON;
    ncd.dwInfoFlags = NIIF_USER;
    BOOL_CHECK(::Shell_NotifyIcon(NIM_ADD, &ncd));
    // 加载菜单
    hMenu = ::GetSubMenu(::LoadMenu(pParam->hInstance, MAKEINTRESOURCE(IDR_MAIN)), 0);
    BOOL_CHECK(hMenu);
    // 读取开启启动
    HR_CHECK(HRESULT_FROM_WIN32(hReg.Open(HKEY_CURRENT_USER, _szRegRun, KEY_QUERY_VALUE)));
    if (ERROR_SUCCESS == hReg.QueryValue(_szStartup, nullptr, nullptr, nullptr))
    {
        ::CheckMenuItem(hMenu, IDM_STARTUP, MF_CHECKED);
    }

    pL2D = new CLive2DFrm(TEXT("Live2D"));
    ::ShowWindow(pL2D->Create(m_hWnd), SW_SHOW);
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainFrm::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // 删除托盘图标
    ::Shell_NotifyIcon(NIM_DELETE, &ncd);
    // 释放菜单资源
    if (nullptr != hMenu) ::DestroyMenu(hMenu);
    ::PostQuitMessage(0);
    return S_OK;
}

LRESULT CMainFrm::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    CAtlString szText;
    szText.LoadString(IDS_EXIT);
    if (MessageBox(szText, CMainFrm::GetWndCaption(), MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
        bHandled = FALSE;
    return S_OK;
}

LRESULT CMainFrm::OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    return ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hWnd, nullptr);
}

LRESULT CMainFrm::OnIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    switch (lParam)
    {
    case WM_CONTEXTMENU:
    case WM_RBUTTONUP:
    {
        POINT pt;
        ::GetCursorPos(&pt);
        return ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, m_hWnd, nullptr);
    }
    case WM_LBUTTONUP:
        ShowWindow(SW_SHOW);
        return ::OpenIcon(m_hWnd);
    default:
        bHandled = FALSE;
    }
    return 0;
}

LRESULT CMainFrm::OnTaskbarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return ::Shell_NotifyIcon(NIM_ADD, &ncd);
}

LRESULT CMainFrm::OnWidgetDestroyed(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    return S_OK;
}

LRESULT CMainFrm::OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    return PostMessage(WM_CLOSE);
}

LRESULT CMainFrm::OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    ATL::CSimpleDialog<IDD_ABOUT> dlgAbout;
    return dlgAbout.DoModal(m_hWnd);
}

LRESULT CMainFrm::OnOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CAtlString szFilter;
    szFilter.LoadString(IDS_FILTER);
    szFilter.Replace(TEXT('|'), TEXT('\0'));

    TCHAR szImage[MAX_PATH] = { 0 };
    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = szFilter;
    ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.nMaxFile = _countof(szImage);
    ofn.lpstrFile = szImage;
    if (!::GetOpenFileName(&ofn)) return FALSE;
    return S_OK;
}

LRESULT CMainFrm::OnStayOnTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    UINT nCheck = ::GetMenuState(hMenu, IDM_TOP, MF_BYCOMMAND) ^ MF_CHECKED;
    ::CheckMenuItem(hMenu, IDM_TOP, nCheck);
    CConfig::Main().StayOnTop(nCheck);
    return S_OK;
}

LRESULT CMainFrm::OnStartup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    ATL::CRegKey hReg;
    hReg.Open(HKEY_CURRENT_USER, _szRegRun, KEY_QUERY_VALUE | KEY_SET_VALUE);
    if (ERROR_SUCCESS == hReg.QueryValue(_szStartup, nullptr, nullptr, nullptr))
    {
        hReg.DeleteValue(_szStartup);
        return ::CheckMenuItem(hMenu, IDM_STARTUP, MF_UNCHECKED);
    }
    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(_Module.get_m_hInst(), szPath, _countof(szPath));
    hReg.SetStringValue(_szStartup, szPath);
    return ::CheckMenuItem(hMenu, IDM_STARTUP, MF_CHECKED);
}