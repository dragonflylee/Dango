#ifndef _FRAME_WND_H_
#define _FRAME_WND_H_

#define DECLARE_WND_CLASS_EX(WndClassName, style, bkgnd) \
static WNDCLASSEX& GetWndClassInfo() \
{ \
	static WNDCLASSEX wc = { sizeof(WNDCLASSEX), style, CFrameWnd::StartWindowProc, \
		  0, 0, NULL, NULL, NULL, (HBRUSH)(bkgnd + 1), NULL, WndClassName, NULL }; \
	return wc; \
}

/**
* 基本窗口选件类
*/
template <class T, DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwStyleEx = WS_EX_APPWINDOW, UINT nID = IDR_MAIN>
class CFrameWnd
{
public:
    DECLARE_WND_CLASS_EX(NULL, 0, COLOR_WINDOW + 1);

    CFrameWnd() : m_hWnd(NULL), m_hInst(NULL) {}

    virtual ~CFrameWnd() {}

    /**
    * 创建窗体
    */
    HWND Create(HINSTANCE hInst, HWND hParent = HWND_DESKTOP)
    {
        TCHAR szAutoName[CHAR_BIT * 2];
        m_hInst = hInst;
        // 加载标题
        ::LoadString(hInst, nID, m_szTitle, _countof(m_szTitle));

        // 注册窗口类
        WNDCLASSEX& wc = T::GetWndClassInfo();
        if (NULL == wc.lpszClassName)
        {
            // 自动生成窗口类名
            _stprintf_s(szAutoName, _countof(szAutoName), TEXT("CDango:%p"), &wc);
            wc.lpszClassName = szAutoName;
        }

        if (!::GetClassInfoEx(hInst, wc.lpszClassName, &wc))
        {
            wc.hInstance = hInst;
            wc.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nID));
            wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
            if (!::RegisterClassEx(&wc)) return NULL;
        }

        return ::CreateWindowEx(dwStyleEx, wc.lpszClassName, m_szTitle, dwStyle,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hParent, NULL, hInst, static_cast<T*>(this));
    }

    /**
    * 消息框
    */
    int MessageBox(LPCTSTR lpText, UINT uType)
    {
        return ::MessageBox(m_hWnd, lpText, m_szTitle, uType);
    }

    /**
    * 适配屏幕位置
    */
    BOOL SetWindowPos(LPRECT pRect, DWORD dwFlag = 0, HWND hAfter = HWND_TOP)
    {
        // 获取显示器信息
        MONITORINFO minfo = { sizeof(MONITORINFO) };
        HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
        if (!::GetMonitorInfo(hMonitor, &minfo)) return FALSE;

        int nWidth = pRect->right - pRect->left;
        int nHeight = pRect->bottom - pRect->top;

        // 处理窗体超出屏幕
        if (pRect->right > minfo.rcWork.right) pRect->left = minfo.rcWork.right - nWidth;
        if (pRect->left < minfo.rcWork.left) pRect->left = minfo.rcWork.left;
        if (pRect->bottom > minfo.rcWork.bottom) pRect->top = minfo.rcWork.bottom - nHeight;
        if (pRect->top < minfo.rcWork.top) pRect->top = minfo.rcWork.top;

        return ::SetWindowPos(m_hWnd, hAfter, pRect->left, pRect->top, pRect->left + nWidth, pRect->top + nHeight, dwFlag);
    }

protected:
    /**
    * 窗体创建事件
    */
    LRESULT OnCreate() { return S_OK; }

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
            T *pT = (T *)((LPCREATESTRUCT)lParam)->lpCreateParams;
            pT->m_hWnd = hWnd;
            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pT);
            return pT->OnCreate();
        }
        T *pT = (T *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (NULL == pT) return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
        return pT->DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

protected:
    HWND m_hWnd;
    HINSTANCE m_hInst;
    TCHAR m_szTitle[MAX_PATH];
};

#endif // _FRAME_WND_H_
