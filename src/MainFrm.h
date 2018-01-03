#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

#include "BtnWnd.h"
#include "WidgetFrm.h"

#define WM_ICON WM_USER + 180

class CMainFrm : public CWindowImpl<CMainFrm, CWindow, CLayeredTraits>, 
    public CButtonBase<CMainFrm>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CMainFrm"), CS_DBLCLKS, COLOR_WINDOW + 1)

    BEGIN_MSG_MAP(CMainFrm)
        CHAIN_MSG_MAP(CButtonBase<CMainFrm>)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_CONTEXTMENU, OnContext)
        MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
        MESSAGE_HANDLER(WM_ICON, OnIcon)
        MESSAGE_HANDLER(CMainFrm::WM_TASKBARCREATED, OnTaskbarCreated)
        COMMAND_ID_HANDLER(IDM_EXIT, OnExit)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER(IDM_OPEN, OnOpen)
        COMMAND_ID_HANDLER(IDM_TOP, OnTop)
        COMMAND_ID_HANDLER(IDM_STARTUP, OnStartup)
    END_MSG_MAP()

public:
    static LPCTSTR GetWndCaption();
    /**
    * 窗口事件相关
    */
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnTaskbarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    /**
    * 右键菜单相关
    */
    LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnStartup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
    HMENU m_hMenu;
    NOTIFYICONDATA m_ncd;
    TCHAR m_config[MAX_PATH];
    // 任务栏重启消息
    static UINT WM_TASKBARCREATED;

    CAtlList<CWidgetFrm> m_pWidget;

public:
    CMainFrm() : CButtonBase(IDR_MAIN), m_hMenu(NULL) {}
};

#endif // _MAIN_FRM_H_