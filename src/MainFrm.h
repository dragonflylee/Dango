#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

class CMainFrm : public CLayered<CMainFrm>
{
private:
    /**
    * 消息处理
    */
    LRESULT OnCreate(LPCREATESTRUCT /*lpCreate*/);
    LRESULT OnDestroy();
    LRESULT OnContext();

    /**
    * 右键菜单事件
    */
    LRESULT OnTopMost();
    LRESULT OnChangeImage();

    /**
    * 窗体消息循环
    */
    static LRESULT CALLBACK StartWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    /**
    * 关于对话框
    */
    static INT_PTR CALLBACK AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hWnd;
    HMENU m_hMenu;
    NOTIFYICONDATA m_ncd;
    TCHAR m_szTitle[MAX_PATH];
    TCHAR m_szConfig[MAX_PATH];
    // 任务栏重启消息
    static UINT WM_TASKBARCREATED;

    friend class CLayered<CMainFrm>;

public:
    CMainFrm();
    virtual ~CMainFrm();
    /**
    * 创建窗体
    */
    HWND Create(HINSTANCE hInst, HWND hParent = HWND_DESKTOP);
};

#endif // _MAIN_FRM_H_