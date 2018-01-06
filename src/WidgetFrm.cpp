#include "StdAfx.h"
#include "WidgetFrm.h"

CWidgetFrm::CWidgetFrm(LPCTSTR szPath) :
m_hMenu(NULL),
m_uTimer(1),
m_layered((BYTE)CConfig::Widget(szPath).Alpha()),
m_szPath(szPath)
{
}

LRESULT CWidgetFrm::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = S_OK;
    TCHAR szMenu[MAX_PATH] = { 0 };
    MENUITEMINFO info = { sizeof(MENUITEMINFO) };
    // ���ز˵�
    m_hMenu = ::LoadMenu(_Module.get_m_hInstResource(), MAKEINTRESOURCE(IDR_MAIN));
    BOOL_CHECK(m_hMenu);
    // ����͸���Ȳ˵�
    info.fMask = MIIM_SUBMENU;
    info.hSubMenu = ::CreateMenu();
    for (WORD uAlpha = _nStartAlpha + 0x20; uAlpha < _nEndAlpha; uAlpha += 0x20)
    {
        _stprintf_s(szMenu, TEXT("%d%%"), 100 * (_nEndAlpha - uAlpha) / 0x100);
        ::InsertMenu(info.hSubMenu, 0, MF_BYCOMMAND, uAlpha, szMenu);
    }
    ::SetMenuItemInfo(m_hMenu, IDM_TRANSPARENT, FALSE, &info);
    ::CheckMenuRadioItem(m_hMenu, _nStartAlpha, _nEndAlpha, _nStartAlpha + m_layered.GetAlpha(), MF_BYCOMMAND);
    // ����ͼ��
    HR_CHECK(m_image.Load(m_szPath));
    BOOL_CHECK(::SetWindowPos(m_hWnd, NULL, CConfig::Widget(m_szPath).Left(), CConfig::Widget(m_szPath).Top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER));
exit:
    // ���� -1 ��ʾ���ڴ���ʧ��
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CWidgetFrm::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    RECT rcWnd;
    ::GetWindowRect(m_hWnd, &rcWnd);
    CConfig::Widget(m_szPath).Left(rcWnd.left);
    CConfig::Widget(m_szPath).Top(rcWnd.top);

    // ɾ����ʱ��
    ::KillTimer(m_hWnd, m_uTimer);

    // �ͷŲ˵���Դ
    if (NULL != m_hMenu) ::DestroyMenu(m_hMenu);

    CConfig::Widget(m_szPath).Show(FALSE);
    return S_OK;
}

LRESULT CWidgetFrm::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return PostMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
}

LRESULT CWidgetFrm::OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    return ::TrackPopupMenu(::GetSubMenu(m_hMenu, 1), TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hWnd, NULL);
}

LRESULT CWidgetFrm::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (TRUE == wParam)
    {
        CConfig::Widget(m_szPath).Show(wParam);
        return OnRender();
    }
    bHandled = FALSE;
    return KillTimer(m_uTimer);
}

LRESULT CWidgetFrm::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (wParam == m_uTimer)
    {
        return OnRender();
    }
    bHandled = FALSE;
    return 0;
}

LRESULT CWidgetFrm::OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    return PostMessage(WM_CLOSE);
}

LRESULT CWidgetFrm::OnTransparent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!::CheckMenuRadioItem(m_hMenu, _nStartAlpha, _nEndAlpha, wID, MF_BYCOMMAND)) return S_FALSE;
    WORD wAlpah = wID - _nStartAlpha;
    CConfig::Widget(m_szPath).Alpha(wAlpah);
    return m_layered.SetAlpha(m_hWnd, LOBYTE(wAlpah));
}


LRESULT CWidgetFrm::OnRender()
{
    m_layered.UpdateLayered(m_hWnd, CRenderTarget(m_image));
    if (m_image.GetFrameCount() <= 1) return S_OK;
    m_uTimer = SetTimer(m_uTimer, m_image.GetFrameDelay());
    return m_image.NextFrame();
}