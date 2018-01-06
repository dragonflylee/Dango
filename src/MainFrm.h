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
    * ��Ϣ����
    */
    LRESULT OnCreate();
    LRESULT OnDestroy();

    /**
    * �Ҽ��˵��¼�
    */
    LRESULT OnStayOnTop();
    LRESULT OnOpenImage();
    LRESULT OnStartUp(BOOL bSet);

    /**
    * ������Ϣѭ��
    */
    LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    /**
    * ���ڶԻ���
    */
    static INT_PTR CALLBACK AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HMENU m_hMenu;
    CLayeredInfo m_layered;
    NOTIFYICONDATA m_ncd;
    // ������������Ϣ
    static UINT WM_TASKBARCREATED;
    static TCHAR szTitle[MAX_PATH];

public:
    CMainFrm() : m_hMenu(NULL) {}
};

#endif // _MAIN_FRM_H_