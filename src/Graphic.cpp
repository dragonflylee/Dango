#include "StdAfx.h"
#include "Graphic.h"
#include "LAppManager.h"

CGraphic::CGraphic() : pBackRTV(nullptr),
pManager(new LAppManager())
{
    
}

CGraphic::~CGraphic()
{
    Shutdown();
}

HRESULT CGraphic::Init(HWND hWnd, int nWidth, int nHeight)
{  
    DXGI_SWAP_CHAIN_DESC sd;
    HRESULT hr = S_OK;

    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = nWidth;
    sd.BufferDesc.Height = nHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

    // Cubism SDK for DirectX11 は Feature Level 10.0以上をサポート
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    HR_CHECK(::D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &sd,
        &pSwapChain, &pDevice, nullptr, &pContext));

    HR_CHECK(Resize(nWidth, nHeight));

    pManager->setGraphicsContext(pDevice, pContext);

exit:
    return hr;
}

void CGraphic::Shutdown()
{
    if (pContext != nullptr) pContext->ClearState();
    if (pBackRTV != nullptr) pBackRTV->Release();

    pBackRTV = nullptr;
    pContext.Release();
    pDevice.Release();
    pSwapChain.Release();
}

HRESULT CGraphic::Resize(int nWidth, int nHeight)
{
    CComPtr<ID3D11Texture2D> tex;
    HRESULT hr = S_OK;

    if (pBackRTV)
    {
        DXGI_SWAP_CHAIN_DESC sd;
        HR_CHECK(pSwapChain->GetDesc(&sd));

        pBackRTV->Release();
        pBackRTV = nullptr;

        HR_CHECK(pSwapChain->ResizeBuffers(1, nWidth, nHeight, sd.BufferDesc.Format, sd.Flags));
    }

    // Create a render target view
    HR_CHECK(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&tex)));

    HR_CHECK(pDevice->CreateRenderTargetView(tex, nullptr, &pBackRTV));

    pContext->OMSetRenderTargets(1, &pBackRTV, nullptr);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    ZeroMemory(&vp, sizeof(vp));
    vp.Width = static_cast<float>(nWidth);
    vp.Height = static_cast<float>(nHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1, &vp);

exit:
    return hr;
}

void CGraphic::BeginScene()
{
    float color[4] = { 0, 0, 0, 0 };
    pContext->ClearRenderTargetView(pBackRTV, color);
}

LRESULT CGraphic::EndScene(HWND hWnd, BYTE uAlpha)
{
    CComPtr<IDXGISurface1> pSurface;
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, uAlpha, AC_SRC_ALPHA };
    POINT pt = { 0, 0 };
    SIZE sz = { 0, 0 };
    HDC hDC = nullptr;
    HRESULT hr = S_OK;

    HR_CHECK(pSwapChain->Present(0, 0));

    HR_CHECK(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pSurface)));

    DXGI_SURFACE_DESC desc;
    HR_CHECK(pSurface->GetDesc(&desc));

    sz.cx = desc.Width;
    sz.cy = desc.Height;

    HR_CHECK(pSurface->GetDC(FALSE, &hDC));

    BOOL_CHECK(::UpdateLayeredWindow(hWnd, nullptr, nullptr, &sz, hDC, &pt, RGB(0, 0, 0), &blend, ULW_ALPHA));

    HR_CHECK(pSurface->ReleaseDC(nullptr));

    pContext->OMSetRenderTargets(1, &pBackRTV, nullptr);

exit:
    return hr;
}