#ifndef _WIC_IMAGE_H_
#define _WIC_IMAGE_H_

/*
/**
* CWICImage 封装类
*/
class CWICImage
{
public:
    /**
    * 从资源中加载图像
    */
    HRESULT Load(_U_STRINGorID nID, LPCTSTR szType)
    {
        CComPtr<IWICStream> pStream;
        HGLOBAL hRes = nullptr;
        HRESULT hr = S_OK;

        HRSRC hSrc = ::FindResource(_Module.GetModuleInstance(), nID.m_lpstr, szType);
        BOOL_CHECK(hSrc);

        hRes = ::LoadResource(_Module.GetModuleInstance(), hSrc);
        BOOL_CHECK(hRes);

        HR_CHECK(pWICFactory->CreateStream(&pStream));

        DWORD cbSize = SizeofResource(_Module.GetModuleInstance(), hSrc);
        LPBYTE pData = (LPBYTE)::LockResource(hRes);

        HR_CHECK(pStream->InitializeFromMemory(pData, cbSize));

        Clear();

        HR_CHECK(pWICFactory->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder));

        // 读取帧数
        HR_CHECK(pDecoder->GetFrameCount(&uFrames));

        // 读取第一帧
        HR_CHECK(NextFrame());
    exit:
        if (nullptr != hRes) ::FreeResource(hRes);
        return hr;
    }

    /**
    * 从文件中加载图像
    */
    HRESULT Load(LPCWSTR wzFilename)
    {
        HRESULT hr = S_OK;

        Clear();

        HR_CHECK(pWICFactory->CreateDecoderFromFilename(wzFilename, nullptr,
            GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder));

        // 读取帧数
        HR_CHECK(pDecoder->GetFrameCount(&uFrames));

        // 读取第一帧
        HR_CHECK(NextFrame());
    exit:
        return hr;
    }

    /**
    * 清除已加载的图像
    */
    void Clear()
    {
        uFrames = 0;
        uNextFrame = 0;
        uFrameDelay = 0;
        pDecoder = nullptr;
        pBitmap = nullptr;
    }

    /**
    * 获取当前帧图像
    */
    inline operator IWICBitmap *() const { return pBitmap; }
    /**
    * 获取总帧数
    */
    inline UINT GetFrameCount() const { return uFrames; }
    /**
    * 获取帧延时
    */
    inline UINT GetFrameDelay() const { return uFrameDelay; }

    /**
    * 移动到下一帧
    */
    HRESULT NextFrame()
    {
        HRESULT hr = S_OK;
        CComPtr<IWICBitmapFrameDecode> pWicFrame;
        CComPtr<IWICFormatConverter> pConverter;
        CComPtr<IWICMetadataQueryReader> pMetaQuery;

        HR_CHECK(pDecoder->GetFrame(uNextFrame, &pWicFrame));

        HR_CHECK(pWICFactory->CreateFormatConverter(&pConverter));

        HR_CHECK(pConverter->Initialize(pWicFrame, GUID_WICPixelFormat32bppPBGRA, 
            WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom));

        pBitmap.Release();
        HR_CHECK(pWICFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnLoad, &pBitmap));

        if (uFrames > 1 && SUCCEEDED(pWicFrame->GetMetadataQueryReader(&pMetaQuery)) && nullptr != pMetaQuery)
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

                if (SUCCEEDED(hr) && uFrameDelay < 90) uFrameDelay = 90;
                uNextFrame = (++uNextFrame) % uFrames;
            }
        }
    exit:
        return hr;
    }

private:
    UINT uFrames;
    CComPtr<IWICBitmapDecoder> pDecoder;
    static IWICImagingFactory *pWICFactory;
    // 当前帧状态
    UINT uNextFrame;
    UINT uFrameDelay;
    CComPtr<IWICBitmap> pBitmap;

private:
    CWICImage&operator=(const CWICImage&);
    CWICImage(const CWICImage&);

public:
    CWICImage() : uFrames(0), uNextFrame(0), uFrameDelay(0)
    {
        if (nullptr == pWICFactory) ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
        ATLASSERT(pWICFactory);
        pWICFactory->AddRef();
    }

    ~CWICImage()
    {
        if (pWICFactory->Release() == 0) pWICFactory = nullptr;
    }
};

template<> class CElementTraits<CWICImage> : public CElementTraitsBase<CWICImage>
{
public:
    typedef IWICBitmap * OUTARGTYPE;
};

__declspec(selectany) IWICImagingFactory * CWICImage::pWICFactory = nullptr;

#endif // _WIC_IMAGE_H_