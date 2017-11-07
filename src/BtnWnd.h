#ifndef _BUTTON_WND_H_
#define _BUTTON_WND_H_

#include "Layered.h"
#include "WICImage.h"

/*
* 按钮选件类
*/
template <class T>
class CButtonBase
{
public:
    BEGIN_MSG_MAP(CButtonBase)
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
        if (FAILED(OnRender(Normal))) return -1;
        bHandled = FALSE;
        return 0;
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
        return OnRender(Hover);
    }

    LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        m_bTracking = FALSE;
        return OnRender(Normal);
    }

    LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        bHandled = FALSE;
        return OnRender(Press);
    }

protected:
    LRESULT OnRender(BtnState state)
    {
        T* pT = static_cast<T*>(this);
        return m_layered.UpdateLayered(pT->m_hWnd, m_btn[state]);
    }

protected:
    BOOL m_bTracking;
    CLayeredInfo m_layered;
    CAtlArray<CRenderTarget> m_btn;

public:
    CButtonBase(UINT nID, BYTE alpha = 0xFF) : m_bTracking(FALSE), m_layered(alpha)
    {
        CWICImage img;
        // 加载图片
        for (UINT i = 0; i < 3; i++)
        {
            img.Load(nID + i, RT_RCDATA);
            m_btn.Add(img);
        }
    }
};

class CBtnClose : public CWindowImpl<CBtnClose, CWindow, CLayeredTraits>, public CButtonBase<CBtnClose>
{
public:
    BEGIN_MSG_MAP(CBtnClose)
        CHAIN_MSG_MAP(CButtonBase<CBtnClose>)
        MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    END_MSG_MAP()

    LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        return GetParent().PostMessage(WM_CLOSE);
    }

public:
    CBtnClose() : CButtonBase(IDR_MAIN, 0xCC) {}
};


#endif // _BUTTON_WND_H_