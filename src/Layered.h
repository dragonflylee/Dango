#ifndef _LAYERED_WND_H_
#define _LAYERED_WND_H_

/**
* RenderTarget封装类
*/
class CRenderTarget
{

public:
    CRenderTarget(IWICBitmap *pBitmap = NULL) : m_pTarget(NULL)
    {
        if (NULL == m_pD2DFactory) ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
        m_pD2DFactory->AddRef();
        if (NULL != pBitmap) m_pD2DFactory->CreateWicBitmapRenderTarget(pBitmap, &RenderProperties, &m_pTarget);
    }

    ~CRenderTarget()
    {
        SAFE_RELEASE(m_pTarget);
        if (0 == m_pD2DFactory->Release()) m_pD2DFactory = NULL;
    }

    operator ID2D1RenderTarget *() { return m_pTarget; }

private:
    ID2D1RenderTarget* m_pTarget;
    static const D2D1_RENDER_TARGET_PROPERTIES RenderProperties;
    static ID2D1Factory *m_pD2DFactory;

private:
    CRenderTarget(const CRenderTarget&);
    CRenderTarget&operator=(const CRenderTarget&);
};

__declspec(selectany) ID2D1Factory * CRenderTarget::m_pD2DFactory = NULL;
__declspec(selectany) const D2D1_RENDER_TARGET_PROPERTIES CRenderTarget::RenderProperties =
    D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(
            DXGI_FORMAT_B8G8R8A8_UNORM,
            D2D1_ALPHA_MODE_PREMULTIPLIED
        ),
        0.0f, // default dpi
        0.0f, // default dpi
        D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE
    );

/**
* 层叠窗口封装类
*/
class CLayeredInfo
{
public:
    BYTE GetAlpha() { return m_blend.SourceConstantAlpha; }

    /**
    * 改变窗体透明度
    */
    HRESULT SetAlpha(HWND hWnd, BYTE alpha)
    {
        m_blend.SourceConstantAlpha = alpha;
        if (::UpdateLayeredWindow(hWnd, NULL, NULL, NULL, NULL, NULL, 0, &m_blend, ULW_ALPHA)) return S_OK;
        return HRESULT_FROM_WIN32(::GetLastError());
    }

    HRESULT SetPosition(HWND hWnd, LPPOINT ptDst)
    {
        // 获取显示器信息
        MONITORINFO minfo = { sizeof(MONITORINFO) };
        HMONITOR hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        if (::GetMonitorInfo(hMonitor, &minfo))
        {
            // 处理窗体超出屏幕
            if (ptDst->x + m_size.cx > minfo.rcWork.right) ptDst->x = minfo.rcWork.right - m_size.cx;
            if (ptDst->x < minfo.rcWork.left) ptDst->x = minfo.rcWork.left;
            if (ptDst->y + m_size.cy > minfo.rcWork.bottom) ptDst->y = minfo.rcWork.bottom - m_size.cy;
            if (ptDst->y < minfo.rcWork.top) ptDst->y = minfo.rcWork.top;
        }
        if (::UpdateLayeredWindow(hWnd, NULL, ptDst, NULL, NULL, NULL, 0, NULL, 0)) return S_OK;
        return HRESULT_FROM_WIN32(::GetLastError());
    }

    /**
    * 更新窗体
    */
    HRESULT UpdateLayered(HWND hWnd, ID2D1RenderTarget *pTarget)
    {
        ID2D1GdiInteropRenderTarget *pGdiTarget = NULL;
        POINT pt = { 0, 0 };
        HDC hDC = NULL;

        // 设置绘图大小
        D2D1_SIZE_U size = pTarget->GetPixelSize();
        m_size.cx = size.width;
        m_size.cy = size.height;

        pTarget->BeginDraw();
        HRESULT hr = pTarget->QueryInterface(IID_PPV_ARGS(&pGdiTarget));
        HR_CHECK(hr);
        // 获取DC
        hr = pGdiTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
        HR_CHECK(hr);
        // 绘制窗体
        BOOL_CHECK(::UpdateLayeredWindow(hWnd, NULL, NULL, &m_size, hDC, &pt, 0, &m_blend, ULW_ALPHA));
    exit:
        if (NULL != pGdiTarget && NULL != hDC) pGdiTarget->ReleaseDC(NULL);
        pTarget->EndDraw();
        SAFE_RELEASE(pGdiTarget);
        return hr;
    }

private:
    SIZE m_size;
    BLENDFUNCTION m_blend;

public:
    CLayeredInfo(BYTE alpha = 0xFF)
    {
        ZeroMemory(&m_size, sizeof(m_size));
        ZeroMemory(&m_blend, sizeof(m_blend));
        m_blend.SourceConstantAlpha = alpha;
        m_blend.AlphaFormat = AC_SRC_ALPHA;
    }
};

#endif // _LAYERED_WND_H_