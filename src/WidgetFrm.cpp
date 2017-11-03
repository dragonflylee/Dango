#include "StdAfx.h"
#include "WidgetFrm.h"

CWidgetFrm::CWidgetFrm(LPCWSTR szPath) : m_szPath(szPath), m_bTracking(FALSE), m_uTimer(1)
{
}

LRESULT CWidgetFrm::OnCreate()
{
    if (FAILED(m_image.Load(m_szPath))) return -1;
    m_layered.SetAlpha(0xCC);
    return ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? 0 : -1;
}

LRESULT CWidgetFrm::OnRender()
{
    m_layered.UpdateLayered(m_hWnd, m_image);
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
        m_layered.SetAlpha(0xFF);
        m_layered.UpdateLayered(m_hWnd, m_image);
        m_bTracking = FALSE;
        break;
    case WM_MOUSELEAVE:
        m_layered.SetAlpha(0xCC);
        m_layered.UpdateLayered(m_hWnd, m_image);
        m_bTracking = FALSE;
        break;
    case WM_DISPLAYCHANGE:
    {
        RECT rcWnd;
        ::GetWindowRect(m_hWnd, &rcWnd);
        return SetWindowPos(&rcWnd, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    case WM_DESTROY:
        ::KillTimer(hWnd, m_uTimer);
        return 0;
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}