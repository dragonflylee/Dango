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

    int MessageBox(LPCTSTR lpText, UINT uType)
    {
        return ::MessageBox(m_hWnd, lpText, m_szTitle, uType);
    }

protected:
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
