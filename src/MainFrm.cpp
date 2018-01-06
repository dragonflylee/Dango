#include "StdAfx.h"
#include "MainFrm.h"

UINT CMainFrm::WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));
// �����������ע���
LPCTSTR _szRegRun = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
LPCTSTR _szStartup = TEXT("Dango");

LPCTSTR CMainFrm::GetWndCaption()
{
    static CAtlString szTitle;
    if (szTitle.IsEmpty()) szTitle.LoadString(IDR_MAIN);
    return szTitle;
}

LRESULT CMainFrm::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ATL::CRegKey hReg;
    CAtlString szName;
    HRESULT hr = S_OK;

    // ���ô���ͼ��
    HICON hIcon = ::LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(hIcon);
    SetIcon(hIcon);

    // ��ʼ�������ļ�
    HR_CHECK(CConfig::Init(szName));

    // �����Ϸ��ļ�
    ::DragAcceptFiles(m_hWnd, TRUE);

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

    // ��ȡ��������
    HR_CHECK(HRESULT_FROM_WIN32(hReg.Open(HKEY_CURRENT_USER, _szRegRun, KEY_QUERY_VALUE)));
    if (ERROR_SUCCESS == hReg.QueryValue(_szStartup, NULL, NULL, NULL))
    {
        ::CheckMenuItem(m_hMenu, IDM_STARTUP, MF_CHECKED);
    }

    // ��ȡ������ǰ����
    HWND hAfter = HWND_TOP;
    if (CConfig::Main().StayOnTop())
    {
        ::CheckMenuItem(m_hMenu, IDM_TOP, MF_CHECKED);
        hAfter = HWND_TOPMOST;
    }
    BOOL_CHECK(SetWindowPos(hAfter, CConfig::Main().Left(), CConfig::Main().Top(), 0, 0, SWP_NOSIZE | SWP_NOACTIVATE));

    // �����Ӵ���
    for (LPTSTR szImage = szName.GetBuffer(); *szImage; szImage += _tcslen(szImage) + 1)
    {
        if (::PathFileExists(szImage) && CConfig::Widget(szImage).Show())
        {
            POSITION nSize = m_pWidget.AddTail(szImage);
            HWND hWnd = m_pWidget.GetAt(nSize).Create(m_hWnd);
            if (NULL != hWnd) ::ShowWindow(hWnd, SW_SHOW);
        }
    }
exit:
    // ���� -1 ��ʾ���ڴ���ʧ��
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainFrm::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CAtlString szValue;
    RECT rcWnd;
    GetWindowRect(&rcWnd);
    CConfig::Main().Left(rcWnd.left);
    CConfig::Main().Top(rcWnd.top);

    // �Ƴ������Ӵ���
    m_pWidget.RemoveAll();

    // ɾ������ͼ��
    ::Shell_NotifyIcon(NIM_DELETE, &m_ncd);
    // �ͷŲ˵���Դ
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

LRESULT CMainFrm::OnStayOnTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    UINT nCheck = ::GetMenuState(m_hMenu, IDM_TOP, MF_BYCOMMAND) ^ MF_CHECKED;
    ::CheckMenuItem(m_hMenu, IDM_TOP, nCheck);
    CConfig::Main().StayOnTop(nCheck);
    return SetWindowPos(nCheck ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

LRESULT CMainFrm::OnStartup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    ATL::CRegKey hReg;
    hReg.Open(HKEY_CURRENT_USER, _szRegRun, KEY_QUERY_VALUE | KEY_SET_VALUE);
    if (ERROR_SUCCESS == hReg.QueryValue(_szStartup, NULL, NULL, NULL))
    {
        hReg.DeleteValue(_szStartup);
        return ::CheckMenuItem(m_hMenu, IDM_STARTUP, MF_UNCHECKED);
    }
    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(_Module.get_m_hInst(), szPath, _countof(szPath));
    hReg.SetStringValue(_szStartup, szPath);
    return ::CheckMenuItem(m_hMenu, IDM_STARTUP, MF_CHECKED);
}