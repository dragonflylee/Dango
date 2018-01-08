#include "StdAfx.h"
#include "WidgetFrm.h"
#include "MainFrm.h"

/* 透明度菜单定义 */
static const UINT _nStartAlpha = IDM_TRANSPARENT + 1000;
static const UINT _nEndAlpha = _nStartAlpha + 0xFF;

CWidgetFrm::CWidgetFrm(LPCTSTR szPath) :
m_hMenu(NULL),
m_uTimer(1),
m_layered((BYTE)CConfig::Widget(szPath).Alpha())
{
    _tcscpy_s(m_szPath, _countof(m_szPath), szPath);
}

LRESULT CWidgetFrm::OnCreate()
{
    HRESULT hr = S_OK;
    TCHAR szMenu[MAX_PATH] = { 0 };
    MENUITEMINFO info = { sizeof(MENUITEMINFO) };
    // 加载菜单
    m_hMenu = ::LoadMenu(m_hInst, MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(m_hMenu);
    // 生成透明度菜单
    info.fMask = MIIM_SUBMENU;
    info.hSubMenu = ::CreateMenu();
    for (UINT uAlpha = _nStartAlpha + 0x20; uAlpha < _nEndAlpha; uAlpha += 0x20)
    {
        _stprintf_s(szMenu, TEXT("%d%%"), 100 * (_nEndAlpha - uAlpha) / 0x100);
        ::InsertMenu(info.hSubMenu, 0, MF_BYCOMMAND, uAlpha, szMenu);
    }
    ::SetMenuItemInfo(m_hMenu, IDM_TRANSPARENT, FALSE, &info);
    ::CheckMenuRadioItem(m_hMenu, _nStartAlpha, _nEndAlpha, _nStartAlpha + m_layered.GetAlpha(), MF_BYCOMMAND);
    // 加载图像
    HR_CHECK(m_image.Load(m_szPath));
    BOOL_CHECK(::SetWindowPos(m_hWnd, NULL, CConfig::Widget(m_szPath).Left(), CConfig::Widget(m_szPath).Top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER));
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CWidgetFrm::OnDestroy()
{
    RECT rcWnd;
    ::GetWindowRect(m_hWnd, &rcWnd);
    CConfig::Widget(m_szPath).Left(rcWnd.left);
    CConfig::Widget(m_szPath).Top(rcWnd.top);

    // 删除定时器
    ::KillTimer(m_hWnd, m_uTimer);

    // 释放菜单资源
    if (NULL != m_hMenu) ::DestroyMenu(m_hMenu);

    return ::PostMessage(::GetParent(m_hWnd), CMainFrm::WM_WIDGETDESTROYED, 0, reinterpret_cast<LPARAM>(this));
}

LRESULT CWidgetFrm::OnRender()
{
    m_layered.UpdateLayered(m_hWnd, CRenderTarget(m_image));
    if (m_image.GetFrameCount() <= 1) return S_OK;
    m_uTimer = ::SetTimer(m_hWnd, m_uTimer, m_image.GetFrameDelay(), NULL);
    return m_image.NextFrame();
}

LRESULT CWidgetFrm::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (IDM_EXIT == LOWORD(wParam))
        {
            CConfig::Widget(m_szPath).Show(FALSE);
            return ::PostMessage(hWnd, WM_CLOSE, 0, 0L);
        }
        if (::CheckMenuRadioItem(m_hMenu, _nStartAlpha, _nEndAlpha, (UINT)wParam, MF_BYCOMMAND))
        {
            wParam -= _nStartAlpha;
            CConfig::Widget(m_szPath).Alpha((int)wParam);
            return m_layered.SetAlpha(m_hWnd, (BYTE)(wParam));
        }
    case WM_LBUTTONDOWN:
        // 鼠标点击拖动窗口
        ::PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        break;
    case WM_CONTEXTMENU:
        // 右键菜单
        return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 1), TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hWnd, NULL);
    case WM_SHOWWINDOW:
        if (TRUE == wParam)
        {
            CConfig::Widget(m_szPath).Show(TRUE);
            return OnRender();
        }
        ::KillTimer(hWnd, m_uTimer);
        break;
    case WM_TIMER:
        if (wParam == m_uTimer)
        {
            return OnRender();
        }
        break;
    case WM_DISPLAYCHANGE:
    {
        RECT rcWnd;
        ::GetWindowRect(m_hWnd, &rcWnd);
        return m_layered.SetPosition(m_hWnd, (LPPOINT)&rcWnd);
    }
    case WM_DESTROY:
        return OnDestroy();
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}