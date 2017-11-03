#ifndef _LAYERED_WND_H_
#define _LAYERED_WND_H_

#include <wincodec.h>
#include <d2d1.h>
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")

#define SAFE_RELEASE(p) if (NULL != (p)) { p->Release(); p = NULL; }
#define HR_CHECK(_hr_) hr = _hr_; if (FAILED(hr)) { goto exit; }
#define BOOL_CHECK(_hr_) if (!(_hr_)) { hr = HRESULT_FROM_WIN32(::GetLastError()); goto exit; }

template <class T>
class CLayered
{
protected:
    class CRenderTargetDC
    {
    public:
        CRenderTargetDC(ID2D1GdiInteropRenderTarget* pTarget) :
            m_pTarget(pTarget), m_hDC(NULL)
        {
            pTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &m_hDC);
        }
        ~CRenderTargetDC()
        {
            m_pTarget->ReleaseDC(NULL);
        }
        operator HDC() const { return m_hDC; }

    private:
        ID2D1GdiInteropRenderTarget* m_pTarget;
        HDC m_hDC;
    };

protected:
    CLayered() : m_uFrames(0), m_uNextFrame(0), m_uTimer(1),
        m_pDecoder(NULL), m_pIWICFactory(NULL), m_pD2DFactory(NULL)
    {
        ZeroMemory(&m_blend, sizeof(m_blend));
        m_blend.SourceConstantAlpha = 0xFF;
        m_blend.AlphaFormat = AC_SRC_ALPHA;

        m_d2dprop = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(
            DXGI_FORMAT_B8G8R8A8_UNORM,
            D2D1_ALPHA_MODE_PREMULTIPLIED),
            0.0f, // default dpi
            0.0f, // default dpi
            D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

        // Create WIC factory
        ::CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pIWICFactory));
        // Create D2D factory
        ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    }

    void Release()
    {
        T* pT = static_cast<T*>(this);
        ::KillTimer(pT->m_hWnd, m_uTimer);

        SAFE_RELEASE(m_pDecoder);
        SAFE_RELEASE(m_pIWICFactory);
        SAFE_RELEASE(m_pD2DFactory);
    }

    inline int GetWidth() { return m_size.cx; }
    inline int GetHeight() { return m_size.cy; }
    /**
    * 从资源中加载图像
    */
    HRESULT Load(HMODULE hModule, LPCTSTR szName, LPCTSTR szType)
    {
        HRESULT hr = S_OK;
        IStream *pStream = NULL;
        IWICBitmapDecoder *pDecoder = NULL;

        HRSRC hSrc = ::FindResource(hModule, szName, szType);
        BOOL_CHECK(hSrc);

        DWORD cbSize = SizeofResource(hModule, hSrc);
        HGLOBAL hRes = ::LoadResource(hModule, hSrc);
        BOOL_CHECK(hRes);

        LPBYTE pData = (LPBYTE)::LockResource(hRes);
        pStream = ::SHCreateMemStream(pData, cbSize);
        ::FreeResource(hRes);

        hr = m_pIWICFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
        HR_CHECK(hr);

        SAFE_RELEASE(m_pDecoder);
        m_pDecoder = pDecoder;
        pDecoder = NULL;
        LoadMetadata();
    exit:
        SAFE_RELEASE(pDecoder);
        SAFE_RELEASE(pStream);
        return hr;
    }

    /**
    * 从文件中加载图像
    */
    HRESULT Load(LPCWSTR wzFilename)
    {
        IWICBitmapDecoder *pDecoder = NULL;
        HRESULT hr = m_pIWICFactory->CreateDecoderFromFilename(wzFilename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
        if (FAILED(hr) || NULL == pDecoder)
        {
            SAFE_RELEASE(pDecoder);
            return hr;
        }

        SAFE_RELEASE(m_pDecoder);
        m_pDecoder = pDecoder;
        LoadMetadata();
        return Render();
    }

    /**
    * 绘制图像
    */
    HRESULT Render()
    {
        HRESULT hr = S_OK;
        UINT uFrameDelay = 0;
        IWICBitmapFrameDecode *pWicFrame = NULL;
        IWICFormatConverter *pConverter = NULL;
        ID2D1GdiInteropRenderTarget *pRenderTarget = NULL;
        IWICMetadataQueryReader *pMetaQuery = NULL;
        ID2D1RenderTarget *pTarget = NULL;
        IWICBitmap *pBitmap = NULL;
        POINT pt = { 0, 0 };
        T* pT = static_cast<T*>(this);

        hr = m_pDecoder->GetFrame(m_uNextFrame, &pWicFrame);
        HR_CHECK(hr);

        if (m_uFrames <= 1)
        {
            UINT uWidth, uHight;
            hr = pWicFrame->GetSize(&uWidth, &uHight);
            HR_CHECK(hr);

            m_size.cx = uWidth;
            m_size.cy = uHight;
        }
        else
        {
            hr = pWicFrame->GetMetadataQueryReader(&pMetaQuery);
            if (SUCCEEDED(hr) && NULL != pMetaQuery)
            {
                PROPVARIANT propValue;
                ::PropVariantInit(&propValue);

                // 获取图像帧率
                hr = pMetaQuery->GetMetadataByName(L"/grctlext/Delay", &propValue);
                if (SUCCEEDED(hr) && propValue.vt == VT_UI2)
                {
                    // Convert the delay retrieved in 10 ms units to a delay in 1 ms units
                    hr = ::UIntMult(propValue.uiVal, 10, &uFrameDelay);
                    ::PropVariantClear(&propValue);

                    if (SUCCEEDED(hr) && uFrameDelay < 90)
                    {
                        uFrameDelay = 90;
                    }

                    ::SetTimer(pT->m_hWnd, m_uTimer, uFrameDelay, NULL);
                    m_uNextFrame = (++m_uNextFrame) % m_uFrames;
                }
            }
        }

        hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
        HR_CHECK(hr);

        hr = pConverter->Initialize(pWicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
        HR_CHECK(hr);

        hr = m_pIWICFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnLoad, &pBitmap);
        HR_CHECK(hr);

        hr = m_pD2DFactory->CreateWicBitmapRenderTarget(pBitmap, m_d2dprop, &pTarget);
        HR_CHECK(hr);

        hr = pTarget->QueryInterface(IID_PPV_ARGS(&pRenderTarget));
        HR_CHECK(hr);

        pTarget->BeginDraw();
        ::UpdateLayeredWindow(pT->m_hWnd, NULL, NULL, &m_size, CRenderTargetDC(pRenderTarget), &pt, 0, &m_blend, ULW_ALPHA);
        pTarget->EndDraw();

    exit:
        SAFE_RELEASE(pWicFrame);
        SAFE_RELEASE(pConverter);
        SAFE_RELEASE(pRenderTarget);
        SAFE_RELEASE(pMetaQuery);
        SAFE_RELEASE(pTarget);
        SAFE_RELEASE(pBitmap);
        return hr;
    }

private:
    /**
    * 加载图像格式信息
    */
    void LoadMetadata()
    {
        T* pT = static_cast<T*>(this);
        ::KillTimer(pT->m_hWnd, m_uTimer);
        m_uTimer++;
        m_uFrames = 0;
        m_uNextFrame = 0;

        IWICMetadataQueryReader *pMetaQuery = NULL;
        // 获取图像格式信息
        HRESULT hr = m_pDecoder->GetMetadataQueryReader(&pMetaQuery);
        if (SUCCEEDED(hr) && NULL != pMetaQuery)
        {
            PROPVARIANT propValue;
            ::PropVariantInit(&propValue);
            // 获取图像宽度
            hr = pMetaQuery->GetMetadataByName(L"/logscrdesc/Width", &propValue);
            if (SUCCEEDED(hr) && propValue.vt == VT_UI2)
            {
                m_size.cx = propValue.uiVal;
                ::PropVariantClear(&propValue);
            }
            // 获取图像高度
            hr = pMetaQuery->GetMetadataByName(L"/logscrdesc/Height", &propValue);
            if (SUCCEEDED(hr) && propValue.vt == VT_UI2)
            {
                m_size.cy = propValue.uiVal;
                ::PropVariantClear(&propValue);
            }
            pMetaQuery->Release();
        }
        // 读取帧数
        m_pDecoder->GetFrameCount(&m_uFrames);
    }
protected:
    SIZE m_size;
    BLENDFUNCTION m_blend;
    UINT m_uFrames;
    UINT m_uNextFrame;
    INT_PTR m_uTimer;
    D2D1_RENDER_TARGET_PROPERTIES m_d2dprop;

    IWICBitmapDecoder *m_pDecoder;
    IWICImagingFactory *m_pIWICFactory;
    ID2D1Factory *m_pD2DFactory;
};

#endif // _LAYERED_WND_H_