#include "StdAfx.h"

#define WM_ICON WM_USER + 180

UINT CMainFrm::WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));

CMainFrm::CMainFrm() : m_hWnd(NULL), m_hMenu(NULL)
{
}

CMainFrm::~CMainFrm()
{
}

LRESULT CMainFrm::OnCreate(LPCREATESTRUCT lpCreate)
{
    // 初始化托盘图标
    ZeroMemory(&m_ncd, sizeof(m_ncd));
    m_ncd.cbSize = sizeof(m_ncd);
    m_ncd.hIcon = ::LoadIcon(lpCreate->hInstance, MAKEINTRESOURCE(IDR_MAIN));
    m_ncd.hWnd = m_hWnd;
    m_ncd.uID = IDR_MAIN;
    m_ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
    m_ncd.uCallbackMessage = WM_ICON;
    m_ncd.dwInfoFlags = NIIF_USER;
    ::Shell_NotifyIcon(NIM_ADD, &m_ncd);

    // 加载菜单
    m_hMenu = ::LoadMenu(lpCreate->hInstance, MAKEINTRESOURCE(IDR_MAIN));

    // 从获配置文件保存路径
    if (::SHGetSpecialFolderPath(m_hWnd, m_szConfig, CSIDL_APPDATA, TRUE))
    {
        ::PathCombine(m_szConfig, m_szConfig, TEXT("Dango.ini"));
    }

    TCHAR szImage[MAX_PATH] = { 0 };
    if (!::GetPrivateProfileString(TEXT("Main"), TEXT("Image"), NULL, szImage, _countof(szImage), m_szConfig))
    {
        CLayered::Load(lpCreate->hInstance, MAKEINTRESOURCE(IDR_MAIN), RT_RCDATA);
    }
    else if (::PathFileExists(szImage))
    {
        CLayered::Load(szImage);
    }

    // 读取坐标
    UINT top = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Top"), 0, m_szConfig);
    UINT left = ::GetPrivateProfileInt(TEXT("Main"), TEXT("Left"), 0, m_szConfig);
    HWND hAfter = HWND_TOP;

    // 读取总在最前配置
    if (::GetPrivateProfileInt(TEXT("Main"), TEXT("TopMost"), 0, m_szConfig) == TRUE)
    {
        ::CheckMenuItem(m_hMenu, IDM_TOPMOST, MF_CHECKED);
        hAfter = HWND_TOPMOST;
    }
    return ::SetWindowPos(m_hWnd, hAfter, left, top, CLayered::GetWidth(), CLayered::GetHeight(), 0);
}

LRESULT CMainFrm::OnDestroy()
{
    TCHAR szValue[10];
    RECT rc;
    ::GetWindowRect(m_hWnd, &rc);
    _stprintf_s(szValue, _countof(szValue), TEXT("%d"), rc.top);
    ::WritePrivateProfileString(TEXT("Main"), TEXT("Top"), szValue, m_szConfig);
    _stprintf_s(szValue, _countof(szValue), TEXT("%d"), rc.left);
    ::WritePrivateProfileString(TEXT("Main"), TEXT("Left"), szValue, m_szConfig);

    // 删除托盘图标
    ::Shell_NotifyIcon(NIM_DELETE, &m_ncd);

    // 释放菜单资源
    if (NULL != m_hMenu) ::DestroyMenu(m_hMenu);

    CLayered::Release();
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
        ::WritePrivateProfileString(TEXT("Main"), TEXT("TopMost"), TEXT("0"), m_szConfig);
        return ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    else
    {
        ::CheckMenuItem(m_hMenu, IDM_TOPMOST, MF_CHECKED);
        ::WritePrivateProfileString(TEXT("Main"), TEXT("TopMost"), TEXT("1"), m_szConfig);
        return ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
}

LRESULT CMainFrm::OnChangeImage()
{
    TCHAR szFilter[MAX_PATH], szImage[MAX_PATH] = { 0 };
    ::LoadString(NULL, IDS_OPENFILTER, szFilter, _countof(szFilter));
    for (LPTSTR pCurStr = szFilter; *pCurStr != TEXT('\0'); pCurStr++) if (*pCurStr == TEXT('|')) *pCurStr = TEXT('\0');
 
    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrTitle = m_szTitle;
    ofn.lpstrFilter = szFilter;
    ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.nMaxFile = _countof(szImage);
    ofn.lpstrFile = szImage;

    if (!::GetOpenFileName(&ofn)) return FALSE;
    if (FAILED(CLayered::Load(szImage))) return FALSE;
    ::WritePrivateProfileString(TEXT("Main"), TEXT("Image"), szImage, m_szConfig);
    return ::SetWindowPos(m_hWnd, NULL, 0, 0, CLayered::GetWidth(), CLayered::GetHeight(), SWP_NOMOVE | SWP_NOZORDER);
}

HWND CMainFrm::Create(HINSTANCE hInst, HWND hParent /*= HWND_DESKTOP*/)
{
    // 注册窗口类
    static WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = CMainFrm::StartWindowProc;
    wc.hInstance = hInst;
    wc.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAIN));
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = TEXT("CDangoWnd");

    ATOM atom = ::RegisterClassEx(&wc);
    if (!atom) return NULL;

    // 加载标题
    ::LoadString(hInst, IDR_MAIN, m_szTitle, _countof(m_szTitle));

    return ::CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, MAKEINTATOM(atom), m_szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hParent, NULL, hInst, this);
}

INT_PTR CALLBACK CMainFrm::AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        // 设置对话框的图标
        HICON hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAIN));
        ::PostMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        return TRUE;
    }
    case WM_COMMAND:
        return EndDialog(hWnd, LOWORD(wParam));
    }
    return 0;
}

LRESULT CALLBACK CMainFrm::StartWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WM_CREATE == uMsg)
    {
        LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
        CMainFrm *pT = (CMainFrm *)lpCreate->lpCreateParams;
        pT->m_hWnd = hWnd;
        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pT);
        return pT->OnCreate(lpCreate);
    }

    CMainFrm *pT = (CMainFrm *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_EXIT:
            return ::PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        case IDM_ABOUT:
            return ::DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_ABOUT), hWnd, CMainFrm::AboutDialogProc, NULL);
        case IDM_OPENIMG:
            return pT->OnChangeImage();
        case IDM_TOPMOST:
            return pT->OnTopMost();
        }
        break;
    case WM_ICON:
        switch (lParam)
        {
        case WM_CONTEXTMENU:
        case WM_RBUTTONUP:
            return pT->OnContext();
        case WM_LBUTTONUP:
            ::ShowWindow(hWnd, SW_SHOW);
            return ::OpenIcon(hWnd);
        }
        break;
    case WM_CLOSE:
    {
        TCHAR szText[MAX_PATH];
        ::LoadString(NULL, IDS_EXIT, szText, _countof(szText));
        if (::MessageBox(hWnd, szText, pT->m_szTitle, MB_ICONQUESTION | MB_OKCANCEL) == IDOK) break;
        return 0;
    }
    case WM_SHOWWINDOW:
        ::KillTimer(hWnd, pT->m_uTimer);
        if (TRUE == wParam)
        {
            return pT->Render();
        }
        break;
    case WM_TIMER:
        if ((INT_PTR)wParam == pT->m_uTimer)
        {
            ::KillTimer(hWnd, pT->m_uTimer);
            return pT->Render();
        }
        break;
    case WM_LBUTTONDOWN:
        // 鼠标点击拖动窗口
        return ::DefWindowProc(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
    case WM_RBUTTONUP:
        // 右键菜单
        return pT->OnContext();
    case WM_DESTROY:
        return pT->OnDestroy();
    }
    if (CMainFrm::WM_TASKBARCREATED == uMsg)
    {
        return ::Shell_NotifyIcon(NIM_ADD, &pT->m_ncd);
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}