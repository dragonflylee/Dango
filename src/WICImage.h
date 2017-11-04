#ifndef _WIC_IMAGE_H_
#define _WIC_IMAGE_H_

/*
/**
* CWICImage 封装类
*/
class CWICImage
{
public:
    CWICImage() : m_uFrames(0), m_pDecoder(NULL), m_uNextFrame(0), m_uFrameDelay(0), m_pBitmap(NULL)
    {
        if (NULL == m_pIWICFactory) // 创建 WIC 工厂实例
        {
            ::CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pIWICFactory));
        }
        else
        {
            m_pIWICFactory->AddRef();
        }
    }

    ~CWICImage()
    {
        SAFE_RELEASE(m_pBitmap);
        SAFE_RELEASE(m_pDecoder);
        if (m_pIWICFactory->Release() == 0) m_pIWICFactory = NULL;
    }

    /**
    * 从资源中加载图像
    */
    HRESULT Load(HMODULE hModule, LPCTSTR szName, LPCTSTR szType)
    {
        IStream *pStream = NULL;
        HGLOBAL hRes = NULL;
        HRESULT hr = S_OK;

        HRSRC hSrc = ::FindResource(hModule, szName, szType);
        BOOL_CHECK(hSrc);

        hRes = ::LoadResource(hModule, hSrc);
        BOOL_CHECK(hSrc);

        DWORD cbSize = SizeofResource(hModule, hSrc);
        LPBYTE pData = (LPBYTE)::LockResource(hRes);
        pStream = ::SHCreateMemStream(pData, cbSize);
        BOOL_CHECK(pStream);

        m_uFrames = 0;
        m_uNextFrame = 0;
        m_uFrameDelay = 0;
        SAFE_RELEASE(m_pDecoder);

        hr = m_pIWICFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &m_pDecoder);
        HR_CHECK(hr);

        // 读取帧数
        hr = m_pDecoder->GetFrameCount(&m_uFrames);
        HR_CHECK(hr);

        // 读取第一帧
        hr = NextFrame();
    exit:
        if (NULL != hRes) ::FreeResource(hRes);
        SAFE_RELEASE(pStream);
        return hr;
    }

    /**
    * 从文件中加载图像
    */
    HRESULT Load(LPCWSTR wzFilename)
    {
        m_uFrames = 0;
        m_uNextFrame = 0;
        m_uFrameDelay = 0;
        SAFE_RELEASE(m_pDecoder);

        HRESULT hr = m_pIWICFactory->CreateDecoderFromFilename(wzFilename, NULL,
            GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_pDecoder);
        HR_CHECK(hr);

        // 读取帧数
        hr = m_pDecoder->GetFrameCount(&m_uFrames);
        HR_CHECK(hr);

        // 读取第一帧
        hr = NextFrame();
    exit:
        return hr;
    }

    /**
    * 获取当前帧图像
    */
    inline operator IWICBitmap *() const { return m_pBitmap; }
    /**
    * 获取总帧数
    */
    inline UINT GetFrameCount() const { return m_uFrames; }
    /**
    * 获取帧延时
    */
    inline UINT GetFrameDelay() const { return m_uFrameDelay; }

    /**
    * 移动到下一帧
    */
    HRESULT NextFrame()
    {
        HRESULT hr = S_OK;
        IWICBitmapFrameDecode *pWicFrame = NULL;
        IWICFormatConverter *pConverter = NULL;
        IWICMetadataQueryReader *pMetaQuery = NULL;

        hr = m_pDecoder->GetFrame(m_uNextFrame, &pWicFrame);
        HR_CHECK(hr);

        hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
        HR_CHECK(hr);

        hr = pConverter->Initialize(pWicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
        HR_CHECK(hr);

        SAFE_RELEASE(m_pBitmap);
        hr = m_pIWICFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnLoad, &m_pBitmap);
        HR_CHECK(hr);

        if (m_uFrames > 1)
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
                    hr = ::UIntMult(propValue.uiVal, 10, &m_uFrameDelay);
                    ::PropVariantClear(&propValue);

                    if (SUCCEEDED(hr) && m_uFrameDelay < 90) m_uFrameDelay = 90;
                    m_uNextFrame = (++m_uNextFrame) % m_uFrames;
                }
            }
        }
    exit:
        SAFE_RELEASE(pWicFrame);
        SAFE_RELEASE(pConverter);
        SAFE_RELEASE(pMetaQuery);
        return hr;
    }

private:
    UINT m_uFrames;
    IWICBitmapDecoder *m_pDecoder;
    static IWICImagingFactory *m_pIWICFactory;
    // 当前帧状态
    SIZE m_size;
    UINT m_uNextFrame;
    UINT m_uFrameDelay;
    IWICBitmap *m_pBitmap;
};

__declspec(selectany) IWICImagingFactory * CWICImage::m_pIWICFactory = NULL;

#endif // _WIC_IMAGE_H_