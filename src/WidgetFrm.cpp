#include "StdAfx.h"
#include "WidgetFrm.h"

LRESULT CWidgetFrm::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    m_close.Create(m_hWnd);
    if (FAILED(m_image.Load(m_szPath))) return -1;
    return SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? 0 : -1;
}

LRESULT CWidgetFrm::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return KillTimer(m_uTimer);
}

LRESULT CWidgetFrm::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return m_close.SetWindowPos(NULL, LOWORD(lParam), HIWORD(lParam), 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

LRESULT CWidgetFrm::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return PostMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
}

LRESULT CWidgetFrm::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return ::PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
}

LRESULT CWidgetFrm::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (TRUE == wParam)
    {
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

LRESULT CWidgetFrm::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (!m_bTracking)
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = 1; //1ms ¡¢º¥œ‘ æ  
        tme.hwndTrack = m_hWnd;
        m_bTracking = ::TrackMouseEvent(&tme);
    }
    bHandled = FALSE;
    return 0;
}

LRESULT CWidgetFrm::OnMouseHover(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    RECT rc;
    m_bTracking = TRUE;
    GetWindowRect(&rc);
    m_close.ShowWindow(SW_SHOW);
    return m_layered.SetAlpha(m_hWnd, 0xFF);
}

LRESULT CWidgetFrm::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    m_bTracking = FALSE;
    m_close.ShowWindow(SW_HIDE);
    return m_layered.SetAlpha(m_hWnd, 0xCC);
}

LRESULT CWidgetFrm::OnRender()
{
    m_layered.UpdateLayered(m_hWnd, CRenderTarget(m_image));
    m_uTimer = SetTimer(m_uTimer, m_image.GetFrameDelay());
    return m_image.NextFrame();
}