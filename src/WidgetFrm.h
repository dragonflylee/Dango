#ifndef _WIDGET_FRM_H_
#define _WIDGET_FRM_H_

#include "Frame.h"
#include "Layered.h"
#include "WICImage.h"

class CWidgetFrm : public CFrameWnd<CWidgetFrm, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoWidget"), 0, COLOR_WINDOW + 1);
    /**
    * ���崴���¼�
    */
    LRESULT OnCreate();
    /**
    * ������Ϣѭ��
    */
    LRESULT DefWindowProc(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

private:
    /**
    * �������
    */
    LRESULT OnRender();

private:
    LPCWSTR m_szPath;
    BOOL m_bTracking;
    UINT_PTR m_uTimer;
    CLayeredInfo m_layered;
    CWICImage m_image;

private:
    CWidgetFrm&operator=(const CWidgetFrm&);
    CWidgetFrm(const CWidgetFrm&);

public:
    CWidgetFrm(LPCWSTR szPath = NULL) : m_szPath(szPath), m_bTracking(FALSE), m_uTimer(1), m_layered(0xCC) {}
};

#endif // _WIDGET_FRM_H_