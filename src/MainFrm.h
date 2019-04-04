#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

#define WM_ICON WM_USER + 1

class CLive2DFrm;

class CMainFrm : public CWindowImpl<CMainFrm, CWindow, CWinTraits<WS_POPUP>>
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CMainFrm"), CS_DBLCLKS, COLOR_WINDOW)

    BEGIN_MSG_MAP(CMainFrm)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_CONTEXTMENU, OnContext)
        MESSAGE_HANDLER(WM_ICON, OnIcon)
        MESSAGE_HANDLER(CMainFrm::WM_TASKBARCREATED, OnTaskbarCreated)
        MESSAGE_HANDLER(CMainFrm::WM_WIDGETDESTROYED, OnWidgetDestroyed)
        COMMAND_ID_HANDLER(IDM_EXIT, OnExit)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER(IDM_OPEN, OnOpen)
        COMMAND_ID_HANDLER(IDM_TOP, OnStayOnTop)
        COMMAND_ID_HANDLER(IDM_STARTUP, OnStartup)
    END_MSG_MAP()

public:
    static LPCTSTR GetWndCaption() { return TEXT("Dango"); }
    /**
    * 窗口事件相关
    */
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnTaskbarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnWidgetDestroyed(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    /**
    * 右键菜单相关
    */
    LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnStayOnTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnStartup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
    HMENU hMenu;
    NOTIFYICONDATA ncd;
    // 任务栏重启消息
    static UINT WM_TASKBARCREATED;

public:
    CMainFrm();
    // 子窗口销毁消息
    static UINT WM_WIDGETDESTROYED;
};

#endif // _MAIN_FRM_H_