#ifndef _FRAME_WND_H_
#define _FRAME_WND_H_

#define DECLARE_WND_CLASS_EX(WndClassName, style, bkgnd) \
static WNDCLASSEX& GetWndClassInfo() \
{ \
	static WNDCLASSEX wc = { sizeof(WNDCLASSEX), style, CFrameWnd::StartWindowProc, \
		  0, 0, nullptr, nullptr, nullptr, (HBRUSH)(bkgnd + 1), nullptr, WndClassName, nullptr }; \
	return wc; \
}

/**
* ��������ѡ����
*/
template <class T, DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwStyleEx = WS_EX_APPWINDOW, UINT nID = IDR_MAIN>
class CFrameWnd
{
public:
    DECLARE_WND_CLASS_EX(TEXT("DangoWnd"), 0, COLOR_WINDOW);

    CFrameWnd() : hWnd(nullptr) {}

    virtual ~CFrameWnd() {}

    /**
    * ��ȡ�������
    */
    static LPCTSTR GetWndCaption() { return nullptr; }

    /**
    * ��������
    */
    HWND Create(HINSTANCE hInst, HWND hParent = HWND_DESKTOP)
    {
        // ע�ᴰ����
        WNDCLASSEX& wc = T::GetWndClassInfo();
        if (!::GetClassInfoEx(hInst, wc.lpszClassName, &wc))
        {
            wc.hInstance = hInst;
            wc.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nID));
            wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
            if (!::RegisterClassEx(&wc)) return nullptr;
        }

        return ::CreateWindowEx(dwStyleEx, wc.lpszClassName, T::GetWndCaption(), dwStyle,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hParent, nullptr, hInst, static_cast<T*>(this));
    }

    /**
    * ��Ϣ��
    */
    int MessageBox(LPCTSTR lpText, UINT uType)
    {
        return ::MessageBox(hWnd, lpText, T::GetWndCaption(), uType);
    }

protected:
    /**
    * ���崴���¼�
    */
    LRESULT OnCreate(LPCREATESTRUCT /*pParam*/) { return S_OK; }

    /**
    * ������Ϣѭ��
    */
    LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    /**
    * ������Ϣѭ��
    */
    static LRESULT CALLBACK StartWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (WM_CREATE == uMsg)
        {
            LPCREATESTRUCT pParam = reinterpret_cast<LPCREATESTRUCT>(lParam);
            T *pT = reinterpret_cast<T *>(pParam->lpCreateParams);
            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pT));
            pT->hWnd = hWnd;
            return pT->OnCreate(pParam);
        }
        T *pT = reinterpret_cast<T *>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (nullptr == pT) return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
        return pT->DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

protected:
    HWND hWnd;
};

#endif // _FRAME_WND_H_
