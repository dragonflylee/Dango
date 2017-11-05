#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

#include "Frame.h"
#include "Layered.h"

class CWidgetFrm;

class CMainFrm : public CFrameWnd<CMainFrm, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW>, CLayeredInfo
{
public:
    /**
    * ��Ϣ����
    */
    LRESULT OnCreate();
    LRESULT OnDestroy();
    LRESULT OnContext();

    /**
    * �Ҽ��˵��¼�
    */
    LRESULT OnTopMost();
    LRESULT OnChangeImage();

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
    CWidgetFrm *m_pWidget;
    TCHAR m_config[MAX_PATH];
    CLayeredInfo m_layered;
    NOTIFYICONDATA m_ncd;
    // ������������Ϣ
    static UINT WM_TASKBARCREATED;

public:
    CMainFrm();
    virtual ~CMainFrm();
};

#endif // _MAIN_FRM_H_