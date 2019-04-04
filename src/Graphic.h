#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

class LAppManager;

class CGraphic
{
public:
    /*
    * Í¼ÐÎäÖÈ¾
    */
    HRESULT Init(HWND hWnd, int nWidth, int nHeight);

    void Shutdown();

    HRESULT Resize(int nWidth, int nHeight);

    void BeginScene();

    LRESULT EndScene(HWND hWnd, BYTE uAlpha = 0xFF);

    inline ID3D11Device * operator ->() { return pDevice; }

private:
    CComPtr<ID3D11Device> pDevice;
    CComPtr<ID3D11DeviceContext> pContext;
    CComPtr<IDXGISwapChain> pSwapChain;
    ID3D11RenderTargetView *pBackRTV;
    // Live2D
    CAutoPtr<LAppManager> pManager;

public:
    CGraphic();
    ~CGraphic();
};

#endif