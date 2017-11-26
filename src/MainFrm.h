#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

#include "BtnWnd.h"
#include "WidgetFrm.h"

#define WM_ICON WM_USER + 180

class CMainFrm : public CWindowImpl<CMainFrm, CWindow, CLayeredTraits>, public CButtonWnd<CMainFrm>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CMainFrm"), 0, COLOR_WINDOW + 1)

    BEGIN_MSG_MAP(CMainFrm)
        CHAIN_MSG_MAP(CButtonWnd<CMainFrm>)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_RBUTTONUP, OnContext)
        MESSAGE_HANDLER(WM_ICON, OnIcon)
        MESSAGE_HANDLER(CMainFrm::WM_TASKBARCREATED, OnTaskbarCreated)
        COMMAND_ID_HANDLER(IDM_EXIT, OnExit)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER(IDM_OPENIMG, OnOpenImage)
        COMMAND_ID_HANDLER(IDM_TOPMOST, OnTopMost)
    END_MSG_MAP()

public:
    static LPCTSTR GetWndCaption();
    /**
    * �����¼����
    */
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnTaskbarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    /**
    * �Ҽ��˵����
    */
    LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOpenImage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnTopMost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
    HMENU m_hMenu;
    NOTIFYICONDATA m_ncd;
    TCHAR m_config[MAX_PATH];
    // ������������Ϣ
    static UINT WM_TASKBARCREATED;

    CAtlArray<CWidgetFrm> m_pWidget;

public:
    CMainFrm() : m_hMenu(NULL) {}
};

#endif // _MAIN_FRM_H_