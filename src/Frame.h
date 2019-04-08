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
* 基本窗口选件类
*/
template <class T, DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwStyleEx = WS_EX_APPWINDOW, UINT nID = IDR_MAIN>
class CFrameWnd
{
public:
    DECLARE_WND_CLASS_EX(TEXT("DangoWnd"), 0, COLOR_WINDOW);

    CFrameWnd() : hWnd(nullptr) {}

    virtual ~CFrameWnd() {}

    /**
    * 获取窗体标题
    */
    static LPCTSTR GetWndCaption() { return nullptr; }

    /**
    * 创建窗体
    */
    HWND Create(HINSTANCE hInst, HWND hParent = HWND_DESKTOP)
    {
        // 注册窗口类
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
    * 消息框
    */
    int MessageBox(LPCTSTR lpText, UINT uType)
    {
        return ::MessageBox(hWnd, lpText, T::GetWndCaption(), uType);
    }

protected:
    /**
    * 窗体创建事件
    */
    LRESULT OnCreate(LPCREATESTRUCT /*pParam*/) { return S_OK; }

    /**
    * 窗体消息循环
    */
    LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    /**
    * 窗体消息循环
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
