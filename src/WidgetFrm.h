#ifndef _WIDGET_FRM_H_
#define _WIDGET_FRM_H_

#include "Frame.h"
#include "Layered.h"
#include "WICImage.h"

class CWidgetFrm : public CFrameWnd<CWidgetFrm, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoWidget"), 0, COLOR_WINDOW);
    /**
    * 消息处理
    */
    LRESULT OnCreate();
    LRESULT OnDestroy();
    /**
    * 窗体消息循环
    */
    LRESULT DefWindowProc(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

private:
    /**
    * 窗体更新
    */
    LRESULT OnRender();

private:
    HMENU m_hMenu;
    UINT_PTR m_uTimer;
    CLayeredInfo m_layered;
    CWICImage m_image;
    TCHAR m_szPath[MAX_PATH];

private:
    CWidgetFrm&operator=(const CWidgetFrm&);
    CWidgetFrm(const CWidgetFrm&);

public:
    CWidgetFrm(LPCTSTR szPath);
};

#endif // _WIDGET_FRM_H_