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
    HRESULT Load(HMODULE hModule, LPCTSTR szName, LPCTSTR szType)
    {
        IWICStream *pStream = nullptr;
        HGLOBAL hRes = nullptr;
        HRESULT hr = S_OK;

        HRSRC hSrc = ::FindResource(hModule, szName, szType);
        BOOL_CHECK(hSrc);

        hRes = ::LoadResource(hModule, hSrc);
        BOOL_CHECK(hRes);

        DWORD cbSize = SizeofResource(hModule, hSrc);
        LPBYTE pData = (LPBYTE)::LockResource(hRes);

        HR_CHECK(pWICFactory->CreateStream(&pStream));

        HR_CHECK(pStream->InitializeFromMemory(pData, cbSize));

        Clear();

        HR_CHECK(pWICFactory->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder));

        // 读取帧数
        HR_CHECK(pDecoder->GetFrameCount(&uFrames));

        // 读取第一帧
        hr = NextFrame();
    exit:
        if (nullptr != hRes) ::FreeResource(hRes);
        SafeRelease(pStream);
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
        hr = NextFrame();
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
        SafeRelease(pBitmap);
        SafeRelease(pDecoder);
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
        IWICBitmapFrameDecode *pWicFrame = nullptr;
        IWICFormatConverter *pConverter = nullptr;
        IWICMetadataQueryReader *pMetaQuery = nullptr;

        HR_CHECK(pDecoder->GetFrame(uNextFrame, &pWicFrame));

        HR_CHECK(pWICFactory->CreateFormatConverter(&pConverter));

        HR_CHECK(pConverter->Initialize(pWicFrame, GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom));

        SafeRelease(pBitmap);
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
        SafeRelease(pWicFrame);
        SafeRelease(pConverter);
        SafeRelease(pMetaQuery);
        return hr;
    }

private:
    UINT uFrames;
    IWICBitmapDecoder *pDecoder;
    static IWICImagingFactory *pWICFactory;
    UINT uNextFrame;
    UINT uFrameDelay;
    IWICBitmap *pBitmap;

private:
    CWICImage&operator=(const CWICImage&);
    CWICImage(const CWICImage&);

public:
    CWICImage() : uFrames(0), pDecoder(nullptr), uNextFrame(0), uFrameDelay(0), pBitmap(nullptr)
    {
        if (nullptr == pWICFactory) ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
        pWICFactory->AddRef();
    }

    ~CWICImage()
    {
        Clear();
        if (pWICFactory->Release() == 0) pWICFactory = nullptr;
    }
};

__declspec(selectany) IWICImagingFactory * CWICImage::pWICFactory = nullptr;

#endif // _WIC_IMAGE_H_