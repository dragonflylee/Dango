#include "StdAfx.h"
#include "MainFrm.h"
#include "WidgetFrm.h"

ID2D1Factory * CWidgetFrm::pD2DFactory = nullptr;

const D2D1_RENDER_TARGET_PROPERTIES rtProp = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
    0.0f, // default dpi
    0.0f, // default dpi
    D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

CWidgetFrm::CWidgetFrm(LPCTSTR szFile) :
hMenu(nullptr), uTimer(1), szPath(szFile)
{
    if (nullptr == pD2DFactory)
        ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    else
        pD2DFactory->AddRef();

    uAlpha = (BYTE)CConfig::Widget(szFile).Alpha();
}

CWidgetFrm::~CWidgetFrm()
{
    if (0 == pD2DFactory->Release()) pD2DFactory = nullptr;
}

LRESULT CWidgetFrm::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = S_OK;
    TCHAR szMenu[MAX_PATH] = { 0 };
    MENUITEMINFO mi = { sizeof(MENUITEMINFO) };
    // 加载菜单
    hMenu = ::GetSubMenu(::LoadMenu(_Module.get_m_hInstResource(), MAKEINTRESOURCE(IDR_MAIN)), 1);
    BOOL_CHECK(hMenu);
    // 生成透明度菜单
    mi.fMask = MIIM_SUBMENU;
    mi.hSubMenu = ::CreateMenu();
    for (WORD i = 21; i < 256; i += 26)
    {
        _stprintf_s(szMenu, TEXT("%d%%"), 100 * (i + 5) / 260);
        ::InsertMenu(mi.hSubMenu, 0, i == uAlpha ? MF_CHECKED | MF_USECHECKBITMAPS : MF_UNCHECKED, WM_ALPHA + i, szMenu);
    }
    ::SetMenuItemInfo(hMenu, IDM_ALPHA, FALSE, &mi);
    // 加载图像
    HR_CHECK(wImage.Load(szPath));

    BOOL_CHECK(SetWindowPos(nullptr, CConfig::Widget(szPath).Left(), CConfig::Widget(szPath).Top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER));
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CWidgetFrm::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    RECT rcWnd;
    GetWindowRect(&rcWnd);
    CConfig::Widget(szPath).Left(rcWnd.left);
    CConfig::Widget(szPath).Top(rcWnd.top);

    KillTimer(uTimer);
    // 释放菜单资源
    if (nullptr != hMenu) ::DestroyMenu(hMenu);
    // 通知主窗口
    return ::PostMessage(GetParent(), CMainFrm::WM_WIDGETDESTROYED, 0, (LPARAM)(LPCTSTR)szPath);
}

LRESULT CWidgetFrm::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return PostMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
}

LRESULT CWidgetFrm::OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    return ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hWnd, nullptr);
}

LRESULT CWidgetFrm::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (TRUE == wParam)
    {
        CConfig::Widget(szPath).Show((int)wParam);
        return OnRender();
    }
    bHandled = FALSE;
    return KillTimer(uTimer);
}

LRESULT CWidgetFrm::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (wParam == uTimer)
    {
        return OnRender();
    }
    bHandled = FALSE;
    return 0;
}

LRESULT CWidgetFrm::OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CConfig::Widget(szPath).Show(FALSE);
    return PostMessage(WM_CLOSE);
}

LRESULT CWidgetFrm::OnAlpha(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!::CheckMenuRadioItem(hMenu, WM_ALPHA, WM_ALPHA + 0xFF, wID, MF_BYCOMMAND)) return S_FALSE;
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, (BYTE)(wID - WM_ALPHA), AC_SRC_ALPHA };
    CConfig::Widget(szPath).Alpha(uAlpha = blend.SourceConstantAlpha);
    return ::UpdateLayeredWindow(m_hWnd, nullptr, nullptr, nullptr, nullptr, nullptr, 0, &blend, ULW_ALPHA);
}


LRESULT CWidgetFrm::OnRender()
{
    CComPtr<ID2D1RenderTarget> pTarget;
    CComPtr<ID2D1GdiInteropRenderTarget> pGdiTarget;
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, uAlpha, AC_SRC_ALPHA };
    POINT pt = { 0, 0 };
    HDC hDC = nullptr;
    HRESULT hr = S_OK;

    HR_CHECK(pD2DFactory->CreateWicBitmapRenderTarget(wImage, &rtProp, &pTarget));

    // 设置绘图大小
    SIZE sz;
    D2D1_SIZE_U size;
    size = pTarget->GetPixelSize();
    sz.cx = size.width;
    sz.cy = size.height;

    pTarget->BeginDraw();

    HR_CHECK(pTarget->QueryInterface(IID_PPV_ARGS(&pGdiTarget)));
    // 获取DC
    HR_CHECK(pGdiTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC));
    // 绘制窗体
    BOOL_CHECK(::UpdateLayeredWindow(m_hWnd, nullptr, nullptr, &sz, hDC, &pt, 0, &blend, ULW_ALPHA));

    HR_CHECK(pGdiTarget->ReleaseDC(nullptr));
    HR_CHECK(pTarget->EndDraw());
;
    if (wImage.GetFrameCount() > 1)
        uTimer = SetTimer(uTimer, wImage.GetFrameDelay());

    HR_CHECK(wImage.NextFrame());
exit:
    return hr;
}