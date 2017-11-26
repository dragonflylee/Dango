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

    // ���ô���ͼ��
    HICON hIcon = ::LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(hIcon);
    SetIcon(hIcon);

    // ��ʼ������ͼ��
    ZeroMemory(&m_ncd, sizeof(m_ncd));
    m_ncd.cbSize = sizeof(m_ncd);
    m_ncd.hIcon = ::LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN));
    m_ncd.hWnd = m_hWnd;
    m_ncd.uID = IDR_MAIN;
    m_ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
    m_ncd.uCallbackMessage = WM_ICON;
    m_ncd.dwInfoFlags = NIIF_USER;
    BOOL_CHECK(::Shell_NotifyIcon(NIM_ADD, &m_ncd));

    // ���ز˵�
    m_hMenu = ::LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(m_hMenu);

    // �ӻ������ļ�����·��
    BOOL_CHECK(::SHGetSpecialFolderPath(m_hWnd, m_config, CSIDL_APPDATA, TRUE));
    BOOL_CHECK(::PathCombine(m_config, m_config, TEXT("Dango.ini")))

    // ��ȡ����
    RECT rcWnd;
    rcWnd.left = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Left"), 0, m_config);
    rcWnd.top = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Top"), 0, m_config);
    
    // ��ȡ������ǰ����
    HWND hAfter = HWND_TOP;
    if (::GetPrivateProfileInt(TEXT("Main"), TEXT("TopMost"), 0, m_config))
    {
        ::CheckMenuItem(m_hMenu, IDM_TOPMOST, MF_CHECKED);
        hAfter = HWND_TOPMOST;
    }
    BOOL_CHECK(SetWindowPos(hAfter, &rcWnd, SWP_NOSIZE | SWP_NOACTIVATE));
exit:
    // ���� -1 ��ʾ���ڴ���ʧ��
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

    // �Ƴ������Ӵ���
    m_pWidget.RemoveAll();

    // ɾ������ͼ��
    ::Shell_NotifyIcon(NIM_DELETE, &m_ncd);
    // �ͷŲ˵���Դ
    if (NULL != m_hMenu) ::DestroyMenu(m_hMenu);
    ::PostQuitMessage(0);
    return TRUE;
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
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    ClientToScreen(&pt);
    return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 0), TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
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

LRESULT CMainFrm::OnOpenImage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

    int nSize = m_pWidget.Add(szImage);
    HWND hWnd = m_pWidget[nSize].Create(m_hWnd);
    if (NULL == hWnd) return FALSE;
    return ::ShowWindow(hWnd, SW_SHOW);
}

LRESULT CMainFrm::OnTopMost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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