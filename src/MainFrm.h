#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

#include "Frame.h"
#include "Layered.h"

class CWidgetFrm;

class CMainFrm : public CFrameWnd<CMainFrm, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW>, CLayeredInfo
{
public:
    DECLARE_WND_CLASS_EX(TEXT("CDangoFrm"), 0, COLOR_WINDOW + 1);
    /**
    * 消息处理
    */
    LRESULT OnCreate();
    LRESULT OnDestroy();

    /**
    * 右键菜单事件
    */
    LRESULT OnTopMost();
    LRESULT OnChangeImage();

    /**
    * 窗体消息循环
    */
    LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    /**
    * 关于对话框
    */
    static INT_PTR CALLBACK AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HMENU m_hMenu;
    CWidgetFrm *m_pWidget;
    TCHAR m_config[MAX_PATH];
    CLayeredInfo m_layered;
    NOTIFYICONDATA m_ncd;
    // 任务栏重启消息
    static UINT WM_TASKBARCREATED;

public:
    CMainFrm() : m_hMenu(NULL), m_pWidget(NULL) {}
};

#endif // _MAIN_FRM_H_