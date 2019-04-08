#ifndef _WIDGET_FRM_H_
#define _WIDGET_FRM_H_

#include "WICImage.h"

/* 透明度菜单定义 */
#define WM_ALPHA (WM_USER + IDM_ALPHA)

typedef CWinTraits<WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW> CLayeredTraits;

class CWidgetFrm : public CWindowImpl<CWidgetFrm, CWindow, CLayeredTraits>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoWidget"), CS_DBLCLKS, COLOR_WINDOW);

    BEGIN_MSG_MAP(CWidgetFrm)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_CONTEXTMENU, OnContext)
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
        COMMAND_ID_HANDLER(IDM_EXIT, OnExit)
        COMMAND_RANGE_HANDLER(WM_ALPHA, WM_ALPHA + 0xFF, OnAlpha)
    END_MSG_MAP()

public:
    /**
    * 窗口事件相关
    */
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    /**
    * 右键菜单相关
    */
    LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnAlpha(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    operator LPCTSTR() { return szPath; }

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
    CAtlString szPath;

    static ID2D1Factory *pD2DFactory;

private:
    CWidgetFrm&operator=(const CWidgetFrm&);
    CWidgetFrm(const CWidgetFrm&);

public:
    CWidgetFrm(LPCTSTR szPath = nullptr);
    virtual ~CWidgetFrm();
};

template<> class CElementTraits<CWidgetFrm> : public CElementTraitsBase<CWidgetFrm>
{
public:
    typedef LPCTSTR INARGTYPE;

    static bool CompareElements(LPCTSTR elem1, LPCTSTR elem2) { return _tcscmp(elem1, elem2) == 0; }
};

#endif // _WIDGET_FRM_H_