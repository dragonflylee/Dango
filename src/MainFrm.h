#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

#include "Frame.h"
#include "Layered.h"

class CWidgetFrm;

class CMainFrm : public CFrameWnd<CMainFrm, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW>, CLayeredInfo
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoFrm"), 0, COLOR_WINDOW);

    static LPCTSTR GetWndCaption() { return szTitle; }
    /**
    * 消息处理
    */
    LRESULT OnCreate();
    /**
    * 窗体消息循环
    */
    LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    /**
    * 关于对话框
    */
    static INT_PTR CALLBACK AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    /**
    * 消息处理
    */
    LRESULT OnDestroy();
    LRESULT OnDropFiles(HDROP hDrop);
    /**
    * 右键菜单事件
    */
    LRESULT OnStayOnTop();
    LRESULT OnOpenImage();
    LRESULT OnStartUp(BOOL bSet);
    /**
    * 创建挂件
    */
    BOOL CreaateWidget(LPCTSTR /*szImage*/);

private:
    HMENU m_hMenu;
    CLayeredInfo m_layered;
    NOTIFYICONDATA m_ncd;
    // 任务栏重启消息
    static UINT WM_TASKBARCREATED;

public:
    CMainFrm() : m_hMenu(NULL) {}
    // 子窗口销毁消息
    static UINT WM_WIDGETDESTROYED;
    static TCHAR szTitle[MAX_PATH];
};

#endif // _MAIN_FRM_H_