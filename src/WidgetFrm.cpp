#include "StdAfx.h"
#include "WidgetFrm.h"

LRESULT CWidgetFrm::OnCreate()
{
    if (FAILED(m_image.Load(m_szPath))) return -1;
    return ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? 0 : -1;
}

LRESULT CWidgetFrm::OnRender()
{
    m_layered.UpdateLayered(m_hWnd, CRenderTarget(m_image));
    m_uTimer = ::SetTimer(m_hWnd, m_uTimer, m_image.GetFrameDelay(), NULL);
    return m_image.NextFrame();
}

LRESULT CWidgetFrm::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        // 鼠标点击拖动窗口
        ::PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        break;
    case WM_SHOWWINDOW:
        if (TRUE == wParam)
        {
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
    case WM_MOUSEMOVE:
    {
        if (!m_bTracking)
        {
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.dwHoverTime = 1; //1ms 立即显示  
            tme.hwndTrack = m_hWnd;
            m_bTracking = ::TrackMouseEvent(&tme);
        }
        break;
    }
    case WM_MOUSEHOVER:
        m_bTracking = TRUE;
        return m_layered.SetAlpha(m_hWnd, 0xFF);
    case WM_MOUSELEAVE:
        m_bTracking = FALSE;
        return m_layered.SetAlpha(m_hWnd, 0xCC);
    case WM_DISPLAYCHANGE:
    {
        RECT rcWnd;
        ::GetWindowRect(m_hWnd, &rcWnd);
        return m_layered.SetPosition(m_hWnd, (LPPOINT)&rcWnd);
    }
    case WM_DESTROY:
        return ::KillTimer(hWnd, m_uTimer);
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}