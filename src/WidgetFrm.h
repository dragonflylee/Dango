#ifndef _WIDGET_FRM_H_
#define _WIDGET_FRM_H_

#include "Frame.h"
#include "Layered.h"

class CWidgetFrm : public CFrameWnd<CWidgetFrm, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoWidget"), 0, COLOR_WINDOW + 1);

    CWidgetFrm(LPWSTR szPath) : m_uTimer(1) 
    {
        m_image.Load(szPath);
    }
    /**
    * 窗体创建事件
    */
    LRESULT OnCreate()
    {   
        return ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? 0 : -1;
    }

    LRESULT OnRender()
    {
        m_layered.UpdateLayered(m_hWnd, m_image);
        m_uTimer = ::SetTimer(m_hWnd, m_uTimer, m_image.GetFrameDelay(), NULL);
        return m_image.NextFrame();
    }
    /**
    * 窗体消息循环
    */
    LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
        case WM_DESTROY:
            ::KillTimer(hWnd, m_uTimer);
            return 0;
        }
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

private:
    UINT_PTR m_uTimer;
    CLayeredInfo m_layered;
    CWICImage m_image;
};

#endif // _WIDGET_FRM_H_