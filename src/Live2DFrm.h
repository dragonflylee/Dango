#ifndef _WIDGET_FRM_H_
#define _WIDGET_FRM_H_

#include "Graphic.h"

/* 透明度菜单定义 */
#define WM_ALPHA (WM_USER + IDM_ALPHA)

class LAppModel;
class LAppRenderer;

typedef CWinTraits<WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW> CLayeredTraits;

class CLive2DFrm : public CWindowImpl<CLive2DFrm, CWindow, CLayeredTraits>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoWidget"), CS_DBLCLKS, COLOR_WINDOW);

    BEGIN_MSG_MAP(CLive2DFrm)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
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
    LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
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

    HRESULT OnFrame();

private:
    HMENU hMenu;
    BYTE uAlpha;
    UINT_PTR uTimer;
    LPCTSTR szPath;

    CGraphic pGraphic;
    CAtlArray<LAppModel> pModel;
    CAutoPtr<LAppRenderer> pRender;

private:
    CLive2DFrm&operator=(const CLive2DFrm&);
    CLive2DFrm(const CLive2DFrm&);

public:
    CLive2DFrm(LPCTSTR szPath = nullptr);
    virtual ~CLive2DFrm();
};

template<> class CElementTraits<CLive2DFrm> : public CElementTraitsBase<CLive2DFrm>
{
public:
    typedef LPCTSTR INARGTYPE;

    static bool CompareElements(LPCTSTR elem1, LPCTSTR elem2) { return _tcscmp(elem1, elem2) == 0; }
};

#endif // _WIDGET_FRM_H_