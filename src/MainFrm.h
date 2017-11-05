#ifndef _MAIN_FRM_H_
#define _MAIN_FRM_H_

class CMainFrm : public CLayered<CMainFrm>
{
private:
    /**
    * ��Ϣ����
    */
    LRESULT OnCreate(LPCREATESTRUCT /*lpCreate*/);
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
    static LRESULT CALLBACK StartWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    /**
    * ���ڶԻ���
    */
    static INT_PTR CALLBACK AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hWnd;
    HMENU m_hMenu;
    NOTIFYICONDATA m_ncd;
    TCHAR m_szTitle[MAX_PATH];
    TCHAR m_szConfig[MAX_PATH];
    // ������������Ϣ
    static UINT WM_TASKBARCREATED;

    friend class CLayered<CMainFrm>;

public:
    CMainFrm();
    virtual ~CMainFrm();
    /**
    * ��������
    */
    HWND Create(HINSTANCE hInst, HWND hParent = HWND_DESKTOP);
};

#endif // _MAIN_FRM_H_