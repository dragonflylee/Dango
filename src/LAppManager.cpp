/**
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#include "StdAfx.h"
#include "LAppManager.h"
#include "ModelLoader.h"
#include "Live2DFramework.h"
#include <util/UtDebug.h>

#define TIMER_ID 1

using namespace live2d;
using namespace live2d::framework;

LAppManager::LAppManager() : _device(nullptr), _deviceContext(nullptr)
{
    Live2D::init();
    Live2DFramework::setPlatformManager(this);

    ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wicFactory));
}

LAppManager::~LAppManager()
{
    Live2D::dispose();

    _wicFactory->Release();
}

void LAppManager::setGraphicsContext(ID3D11Device* device, ID3D11DeviceContext* context)
{
    _device = device;
    _deviceContext = context;
    // デバイスコンテキストを設定(モデル間で共有される)
    Live2DModelD3D11::setGraphicsContext(device, context);
}

unsigned char *LAppManager::loadBytes(const char *path, size_t *size)
{
    return ModelLoader::loadFile(path, (int *)size);
}

void LAppManager::releaseBytes(void *data)
{
    ModelLoader::releaseBuffer(data);
}

ALive2DModel *LAppManager::loadLive2DModel(const char *path)
{
    size_t size;
    unsigned char *buf = loadBytes(path, &size);
    //Create Live2D Model Instance
    ALive2DModel *live2DModel = Live2DModelD3D11::loadModel(buf, (int)size);
    ModelLoader::releaseBuffer(buf);
    return live2DModel;
}

class LAppTextureDesc : public live2d::framework::L2DTextureDesc
{
public:
    LAppTextureDesc(ID3D11ShaderResourceView *tex)
    {
        data = tex;
    }
    virtual ~LAppTextureDesc()
    {
        if (data) data->Release();
    }

private:
    ID3D11ShaderResourceView *data;
};

//-------------------------------------------------------------------------------------
// WIC Pixel Format Translation Data
//-------------------------------------------------------------------------------------
struct WICTranslate
{
    GUID                wic;
    DXGI_FORMAT         format;
};

const WICTranslate g_WICFormats[] =
{
    { GUID_WICPixelFormat128bppRGBAFloat, DXGI_FORMAT_R32G32B32A32_FLOAT },

    { GUID_WICPixelFormat64bppRGBAHalf, DXGI_FORMAT_R16G16B16A16_FLOAT },
    { GUID_WICPixelFormat64bppRGBA, DXGI_FORMAT_R16G16B16A16_UNORM },

    { GUID_WICPixelFormat32bppRGBA, DXGI_FORMAT_R8G8B8A8_UNORM },
    { GUID_WICPixelFormat32bppBGRA, DXGI_FORMAT_B8G8R8A8_UNORM }, // DXGI 1.1
    { GUID_WICPixelFormat32bppBGR, DXGI_FORMAT_B8G8R8X8_UNORM }, // DXGI 1.1

    { GUID_WICPixelFormat32bppRGBA1010102XR, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM }, // DXGI 1.1
    { GUID_WICPixelFormat32bppRGBA1010102, DXGI_FORMAT_R10G10B10A2_UNORM },

    { GUID_WICPixelFormat16bppBGRA5551, DXGI_FORMAT_B5G5R5A1_UNORM },
    { GUID_WICPixelFormat16bppBGR565, DXGI_FORMAT_B5G6R5_UNORM },

    { GUID_WICPixelFormat32bppGrayFloat, DXGI_FORMAT_R32_FLOAT },
    { GUID_WICPixelFormat16bppGrayHalf, DXGI_FORMAT_R16_FLOAT },
    { GUID_WICPixelFormat16bppGray, DXGI_FORMAT_R16_UNORM },
    { GUID_WICPixelFormat8bppGray, DXGI_FORMAT_R8_UNORM },

    { GUID_WICPixelFormat8bppAlpha, DXGI_FORMAT_A8_UNORM },
};

L2DTextureDesc *LAppManager::loadTexture(ALive2DModel *model, int no, const char *path)
{
    ID3D11ShaderResourceView *texture = nullptr;
    CComPtr<IWICStream> stream;
    CComPtr<IWICBitmapDecoder> decoder;
    CComPtr<IWICBitmapFrameDecode> frame;
    CComPtr<ID3D11Texture2D> tex;
    WICPixelFormatGUID pixelFormat;
    D3D11_SUBRESOURCE_DATA initData = {};
    D3D11_TEXTURE2D_DESC desc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC srd = {};
    UINT support = 0, bpp = 0;
    HRESULT hr = S_OK;
    int wicSize = 0;
    unsigned char * wicData = ModelLoader::loadFile(path, &wicSize);
    
    HR_CHECK(_wicFactory->CreateStream(&stream));

    HR_CHECK(stream->InitializeFromMemory(wicData, wicSize));

    HR_CHECK(_wicFactory->CreateDecoderFromStream(stream, nullptr,
        WICDecodeMetadataCacheOnLoad, &decoder));

    HR_CHECK(decoder->GetFrame(0, &frame));

    HR_CHECK(frame->GetSize(&desc.Width, &desc.Height));

    HR_CHECK(frame->GetPixelFormat(&pixelFormat));

    bpp = _WICBitsPerPixel(pixelFormat);

    for (size_t i = 0; i < _countof(g_WICFormats); ++i)
        if (memcmp(&g_WICFormats[i].wic, &pixelFormat, sizeof(GUID)) == 0)
            desc.Format = g_WICFormats[i].format;

    hr = _device->CheckFormatSupport(desc.Format, &support);
    if (FAILED(hr) || !(support & D3D11_FORMAT_SUPPORT_TEXTURE2D))
    {
        // Fallback to RGBA 32-bit format which is supported by all devices
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    }

    initData.SysMemPitch = (desc.Width * bpp + 7u) / 8u;
    initData.SysMemSlicePitch = initData.SysMemPitch * desc.Height;
    initData.pSysMem = malloc(initData.SysMemSlicePitch);

    HR_CHECK(frame->CopyPixels(nullptr, initData.SysMemPitch,
        initData.SysMemSlicePitch, (BYTE*)initData.pSysMem));

    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    HR_CHECK(_device->CreateTexture2D(&desc, &initData, &tex));

    srd.Format = desc.Format;
    srd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srd.Texture2D.MipLevels = 1;

    HR_CHECK(_device->CreateShaderResourceView(tex, &srd, &texture));
exit:
    if (initData.pSysMem) free((void*)initData.pSysMem);

    ModelLoader::releaseBuffer(wicData);
    ((Live2DModelD3D11 *)model)->setTexture(no, texture); // テクスチャとモデルを結びつける
    return new LAppTextureDesc(texture);
}

void LAppManager::log(const char *txt)
{
    UtDebug::print(txt);
}

UINT LAppManager::_WICBitsPerPixel(REFGUID targetGuid)
{
    CComPtr<IWICComponentInfo> ci;
    if (FAILED(_wicFactory->CreateComponentInfo(targetGuid, &ci))) return 0;

    WICComponentType ct;
    if (FAILED(ci->GetComponentType(&ct))) return 0;
    if (ct != WICPixelFormat) return 0;

    CComPtr<IWICPixelFormatInfo> pfi;
    if (FAILED(ci->QueryInterface(IID_PPV_ARGS(&pfi)))) return 0;

    UINT bpp = 0;
    if (FAILED(pfi->GetBitsPerPixel(&bpp))) return 0;
    return bpp;
}