#ifndef _LAYERED_WND_H_
#define _LAYERED_WND_H_

typedef CWinTraits<WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW> CLayeredTraits;

/**
* ID2D1RenderTarget ��װ��
*/
class CRenderTargetDC
{
public:
    CRenderTargetDC(ID2D1RenderTarget* pTarget) :
        m_pTarget(pTarget), m_pRenderTarget(NULL), m_hDC(NULL)
    {
        HRESULT hr = pTarget->QueryInterface(IID_PPV_ARGS(&m_pRenderTarget));
        if (SUCCEEDED(hr) && NULL != m_pRenderTarget)
        {
            m_pTarget->BeginDraw();
            m_pRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &m_hDC);
        }
    }
    ~CRenderTargetDC()
    {
        if (NULL != m_pRenderTarget) m_pRenderTarget->ReleaseDC(NULL);
        m_pTarget->EndDraw();
    }
    operator HDC() const { return m_hDC; }

    /**
    * ����GDI��Target����
    */
    const static D2D1_RENDER_TARGET_PROPERTIES mD2DProperties;

private:
    CComPtr<ID2D1RenderTarget> m_pTarget;
    CComPtr<ID2D1GdiInteropRenderTarget> m_pRenderTarget;
    HDC m_hDC;
};

__declspec(selectany) const D2D1_RENDER_TARGET_PROPERTIES CRenderTargetDC::mD2DProperties =
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
* ������ڷ�װ��
*/
class CLayeredInfo
{
public:
    CLayeredInfo()
    {
        ZeroMemory(&m_size, sizeof(m_size));
        ZeroMemory(&m_blend, sizeof(m_blend));
        m_blend.SourceConstantAlpha = 0xFF;
        m_blend.AlphaFormat = AC_SRC_ALPHA;

        if (NULL == m_pD2DFactory) // ���� D2D ����ʵ��
        {
            ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
        }
        m_pD2DFactory->AddRef();
    }

    ~CLayeredInfo()
    {
        if (m_pD2DFactory->Release() == 0) m_pD2DFactory = NULL;
    }

    /**
    * �ı䴰��͸����
    */
    void SetAlpha(BYTE alpha) { m_blend.SourceConstantAlpha = alpha; }

    /**
     * ��ȡ���ڳߴ�
     */
    LONG GetWidth() const { return m_size.cx; }
    LONG GetHeight() const { return m_size.cy; }

    /**
    * ���´���
    */
    HRESULT UpdateLayered(HWND hWnd, IWICBitmap *pBitmap)
    {
        CComPtr<ID2D1RenderTarget> pTarget;
        POINT pt = { 0, 0 };
        UINT uWidth, uHight;

        HRESULT hr = m_pD2DFactory->CreateWicBitmapRenderTarget(pBitmap, CRenderTargetDC::mD2DProperties, &pTarget);
        HR_CHECK(hr);

        hr = pBitmap->GetSize(&uWidth, &uHight);
        HR_CHECK(hr);

        if ((UINT)m_size.cx != uWidth || (UINT)m_size.cy != uHight)
        {
            // ���ڷ����ı�
            m_size.cx = uWidth;
            m_size.cy = uHight;
            BOOL_CHECK(::SetWindowPos(hWnd, NULL, 0, 0, m_size.cx, m_size.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE));
        }
        // ���ƴ���
        BOOL_CHECK(::UpdateLayeredWindow(hWnd, NULL, NULL, &m_size, CRenderTargetDC(pTarget), &pt, 0, &m_blend, ULW_ALPHA));
    exit:
        return hr;
    }

private:
    SIZE m_size;
    BLENDFUNCTION m_blend;
    static ID2D1Factory *m_pD2DFactory;
};

__declspec(selectany) ID2D1Factory * CLayeredInfo::m_pD2DFactory = NULL;

#endif // _LAYERED_WND_H_