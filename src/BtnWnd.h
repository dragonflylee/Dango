#ifndef _BUTTON_WND_H_
#define _BUTTON_WND_H_

#include "Layered.h"
#include "WICImage.h"

template <class T, UINT nID = IDR_MAIN, int nState = 3>
class CButtonWnd
{
public:
    BEGIN_MSG_MAP(CButtonWnd)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)
        MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()

protected:
    enum BtnState { Normal = 0, Hover, Press };

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        HRESULT hr = S_OK;
        // 加载图片
        m_btn.SetCount(nState);
        for (UINT i = 0; i < nState; i++)
        {
            HR_CHECK(m_btn[i].Load(_Module.GetModuleInstance(), MAKEINTRESOURCE(nID + i), RT_RCDATA));
        }
        HR_CHECK(m_layered.UpdateLayered(static_cast<T*>(this)->m_hWnd, m_btn[Normal]));
        bHandled = FALSE;
    exit:
        return SUCCEEDED(hr) ? 0 : -1;
    }
    
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        bHandled = FALSE;
        m_btn.RemoveAll();
        return S_OK;
    }

    LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        if (!m_bTracking)
        {
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.dwHoverTime = 1; //1ms 立即显示  
            tme.hwndTrack = static_cast<T*>(this)->m_hWnd;
            //激活WM_MOUSEHOVER消息  
            m_bTracking = ::TrackMouseEvent(&tme);
        }
        bHandled = FALSE;
        return TRUE;
    }

    LRESULT OnMouseHover(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        m_bTracking = TRUE;
        return m_layered.UpdateLayered(static_cast<T*>(this)->m_hWnd, m_btn[Hover]);
    }

    LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        m_bTracking = FALSE;
        return m_layered.UpdateLayered(static_cast<T*>(this)->m_hWnd, m_btn[Normal]);
    }

    LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        bHandled = FALSE;
        return m_layered.UpdateLayered(static_cast<T*>(this)->m_hWnd, m_btn[Press]);
    }

private:
    BOOL m_bTracking;
    CLayeredInfo m_layered;
    CAtlArray<CWICImage> m_btn;

public:
    CButtonWnd() : m_bTracking(FALSE) {}
};

#endif // _BUTTON_WND_H_