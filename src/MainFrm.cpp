#include "StdAfx.h"
#include "MainFrm.h"

UINT CMainFrm::WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));

LPCTSTR CMainFrm::GetWndCaption()
{
    static CAtlString szTitle;
    if (szTitle.IsEmpty()) szTitle.LoadString(IDR_MAIN);
    return szTitle;
}

LRESULT CMainFrm::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = S_OK;

    // 设置窗体图标
    HICON hIcon = ::LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(hIcon);
    SetIcon(hIcon);

    // 允许拖放文件
    ::DragAcceptFiles(m_hWnd, TRUE);

    // 初始化托盘图标
    ZeroMemory(&m_ncd, sizeof(m_ncd));
    m_ncd.cbSize = sizeof(m_ncd);
    m_ncd.hIcon = ::LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN));
    m_ncd.hWnd = m_hWnd;
    m_ncd.uID = IDR_MAIN;
    m_ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
    m_ncd.uCallbackMessage = WM_ICON;
    m_ncd.dwInfoFlags = NIIF_USER;
    BOOL_CHECK(::Shell_NotifyIcon(NIM_ADD, &m_ncd));

    // 加载菜单
    m_hMenu = ::LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(m_hMenu);

    // 从获配置文件保存路径
    BOOL_CHECK(::SHGetSpecialFolderPath(m_hWnd, m_config, CSIDL_APPDATA, TRUE));
    BOOL_CHECK(::PathCombine(m_config, m_config, TEXT("Dango.ini")));

    // 读取坐标
    RECT rcWnd;
    rcWnd.left = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Left"), 0, m_config);
    rcWnd.top = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Top"), 0, m_config);

    // 读取总在最前配置
    HWND hAfter = HWND_TOP;
    if (::GetPrivateProfileInt(TEXT("Main"), TEXT("TopMost"), 0, m_config))
    {
        ::CheckMenuItem(m_hMenu, IDM_TOPMOST, MF_CHECKED);
        hAfter = HWND_TOPMOST;
    }
    BOOL_CHECK(SetWindowPos(hAfter, &rcWnd, SWP_NOSIZE | SWP_NOACTIVATE));
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainFrm::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CAtlString szValue;
    RECT rcWnd;
    GetWindowRect(&rcWnd);
    szValue.Format(TEXT("%d"), rcWnd.top);
    ::WritePrivateProfileString(TEXT("Main"), TEXT("Top"), szValue, m_config);
    szValue.Format(TEXT("%d"), rcWnd.left);
    ::WritePrivateProfileString(TEXT("Main"), TEXT("Left"), szValue, m_config);

    // 移除所有子窗口
    m_pWidget.RemoveAll();

    // 删除托盘图标
    ::Shell_NotifyIcon(NIM_DELETE, &m_ncd);
    // 释放菜单资源
    if (NULL != m_hMenu) ::DestroyMenu(m_hMenu);
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

LRESULT CMainFrm::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return PostMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
}

LRESULT CMainFrm::OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 0), TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hWnd, NULL);
}

LRESULT CMainFrm::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HDROP hDrop = reinterpret_cast<HDROP>(wParam);
    UINT uCount = ::DragQueryFile(hDrop, (UINT)-1, NULL, 0);
    TCHAR szPath[MAX_PATH];

    for (UINT iFile = 0; iFile < uCount; iFile++)
    {
        if (::DragQueryFile(hDrop, iFile, szPath, _countof(szPath)))
        {
            ATLTRACE(TEXT("%s\n"), szPath);
        }
    }
    return S_OK;
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
        return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 0), TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
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
    return ::Shell_NotifyIcon(NIM_ADD, &m_ncd);
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

    POSITION nSize = m_pWidget.AddTail(szImage);
    HWND hWnd = m_pWidget.GetAt(nSize).Create(m_hWnd);
    if (NULL == hWnd) return FALSE;
    return ::ShowWindow(hWnd, SW_SHOW);
}

LRESULT CMainFrm::OnTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    UINT nCheck = ::GetMenuState(m_hMenu, IDM_TOPMOST, MF_BYCOMMAND) ^ MF_CHECKED;
    ::CheckMenuItem(m_hMenu, IDM_TOPMOST, nCheck);
    ::WritePrivateProfileString(TEXT("Main"), TEXT("TopMost"), nCheck ? TEXT("1") : TEXT("0"), m_config);
    return SetWindowPos(nCheck ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}