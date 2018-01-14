#ifndef _WIC_IMAGE_H_
#define _WIC_IMAGE_H_

/*
/**
* CWICImage ��װ��
*/
class CWICImage
{
public:
    /**
    * ����Դ�м���ͼ��
    */
    HRESULT Load(_U_STRINGorID nID, LPCTSTR szType)
    {
        CComPtr<IStream> pStream;
        HGLOBAL hRes = NULL;
        HRESULT hr = S_OK;

        HRSRC hSrc = ::FindResource(_Module.GetModuleInstance(), nID.m_lpstr, szType);
        BOOL_CHECK(hSrc);

        hRes = ::LoadResource(_Module.GetModuleInstance(), hSrc);
        BOOL_CHECK(hRes);

        DWORD cbSize = SizeofResource(_Module.GetModuleInstance(), hSrc);
        LPBYTE pData = (LPBYTE)::LockResource(hRes);
        pStream = ::SHCreateMemStream(pData, cbSize);
        BOOL_CHECK(pStream);

        Clear();

        hr = m_pIWICFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &m_pDecoder);
        HR_CHECK(hr);

        // ��ȡ֡��
        hr = m_pDecoder->GetFrameCount(&m_uFrames);
        HR_CHECK(hr);

        // ��ȡ��һ֡
        hr = NextFrame();
    exit:
        if (NULL != hRes) ::FreeResource(hRes);
        return hr;
    }

    /**
    * ���ļ��м���ͼ��
    */
    HRESULT Load(LPCWSTR wzFilename)
    {
        Clear();

        HRESULT hr = m_pIWICFactory->CreateDecoderFromFilename(wzFilename, NULL,
            GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_pDecoder);
        HR_CHECK(hr);

        // ��ȡ֡��
        hr = m_pDecoder->GetFrameCount(&m_uFrames);
        HR_CHECK(hr);

        // ��ȡ��һ֡
        hr = NextFrame();
    exit:
        return hr;
    }

    /**
    * ����Ѽ��ص�ͼ��
    */
    void Clear()
    {
        m_uFrames = 0;
        m_uNextFrame = 0;
        m_uFrameDelay = 0;
        m_pDecoder = NULL;
        m_pBitmap = NULL;
    }

    /**
    * ��ȡ��ǰ֡ͼ��
    */
    inline operator IWICBitmap *() const { return m_pBitmap; }
    /**
    * ��ȡ��֡��
    */
    inline UINT GetFrameCount() const { return m_uFrames; }
    /**
    * ��ȡ֡��ʱ
    */
    inline UINT GetFrameDelay() const { return m_uFrameDelay; }

    /**
    * �ƶ�����һ֡
    */
    HRESULT NextFrame()
    {
        HRESULT hr = S_OK;
        CComPtr<IWICBitmapFrameDecode> pWicFrame;
        CComPtr<IWICFormatConverter> pConverter;

        hr = m_pDecoder->GetFrame(m_uNextFrame, &pWicFrame);
        HR_CHECK(hr);

        hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
        HR_CHECK(hr);

        hr = pConverter->Initialize(pWicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
        HR_CHECK(hr);

        m_pBitmap = NULL;
        hr = m_pIWICFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnLoad, &m_pBitmap);
        HR_CHECK(hr);

        if (m_uFrames > 1)
        {
            CComPtr<IWICMetadataQueryReader> pMetaQuery;
            hr = pWicFrame->GetMetadataQueryReader(&pMetaQuery);
            if (SUCCEEDED(hr) && NULL != pMetaQuery)
            {
                PROPVARIANT propValue;
                ::PropVariantInit(&propValue);

                // ��ȡͼ��֡��
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
        return hr;
    }

private:
    UINT m_uFrames;
    CComPtr<IWICBitmapDecoder> m_pDecoder;
    static IWICImagingFactory *m_pIWICFactory;
    // ��ǰ֡״̬
    UINT m_uNextFrame;
    UINT m_uFrameDelay;
    CComPtr<IWICBitmap> m_pBitmap;

private:
    CWICImage&operator=(const CWICImage&);
    CWICImage(const CWICImage&);

public:
    CWICImage() : m_uFrames(0), m_uNextFrame(0), m_uFrameDelay(0)
    {
        if (NULL == m_pIWICFactory) ::CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pIWICFactory));
        ATLASSERT(m_pIWICFactory);
        m_pIWICFactory->AddRef();
    }

    ~CWICImage()
    {
        if (m_pIWICFactory->Release() == 0) m_pIWICFactory = NULL;
    }
};

template<> class CElementTraits<CWICImage> : public CElementTraitsBase<CWICImage>
{
public:
    typedef IWICBitmap * OUTARGTYPE;
};

__declspec(selectany) IWICImagingFactory * CWICImage::m_pIWICFactory = NULL;

#endif // _WIC_IMAGE_H_