#ifndef _WIDGET_FRM_H_
#define _WIDGET_FRM_H_

#include "Frame.h"
#include "WICImage.h"

class CWidgetFrm : public CFrameWnd<CWidgetFrm, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoWidget"), 0, COLOR_WINDOW);
    /**
    * 消息处理
    */
    LRESULT OnCreate(LPCREATESTRUCT /*pParam*/);
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
    HMENU hMenu;
    BYTE uAlpha;
    UINT_PTR uTimer;
    CWICImage wImage;
    TCHAR szPath[MAX_PATH];
    static ID2D1Factory *pD2DFactory;
    CWidgetFrm *pNext;

private:
    CWidgetFrm&operator=(const CWidgetFrm&);
    CWidgetFrm(const CWidgetFrm&);

public:
    CWidgetFrm(CWidgetFrm *pPrev, LPCTSTR szPath = nullptr);
    virtual ~CWidgetFrm();
};

#endif // _WIDGET_FRM_H_