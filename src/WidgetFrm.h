#ifndef _WIDGET_FRM_H_
#define _WIDGET_FRM_H_

#include "BtnWnd.h"

/* 透明度菜单定义 */
static const WORD _nStartAlpha = IDM_TRANSPARENT + 1000;
static const WORD _nEndAlpha = _nStartAlpha + 0xFF;

class CWidgetFrm : public CWindowImpl<CWidgetFrm, CWindow, CLayeredTraits>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoWidget"), CS_DBLCLKS, COLOR_WINDOW + 1);

    BEGIN_MSG_MAP(CWidgetFrm)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_CONTEXTMENU, OnContext)
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
        COMMAND_ID_HANDLER(IDM_EXIT, OnExit)
        COMMAND_RANGE_HANDLER(_nStartAlpha, _nEndAlpha, OnTransparent)
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
    LRESULT OnTransparent(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    operator LPCTSTR() { return m_szPath; }

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
    CAtlString m_szPath;

private:
    CWidgetFrm&operator=(const CWidgetFrm&);
    CWidgetFrm(const CWidgetFrm&);

public:
    CWidgetFrm(LPCTSTR szPath = NULL);
};

template<> class CElementTraits<CWidgetFrm> : public CElementTraitsBase<CWidgetFrm>
{
public:
    typedef LPCTSTR INARGTYPE;

    static bool CompareElements(LPCTSTR elem1, LPCTSTR elem2) { return _tcscmp(elem1, elem2) == 0; }
};

#endif // _WIDGET_FRM_H_