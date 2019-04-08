#include "StdAfx.h"
#include "WidgetFrm.h"
#include "MainFrm.h"

/* 透明度菜单定义 */
#define WM_ALPHA (WM_USER + IDM_ALPHA)

ID2D1Factory * CWidgetFrm::pD2DFactory = nullptr;

const D2D1_RENDER_TARGET_PROPERTIES rtProp = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
    0.0f, // default dpi
    0.0f, // default dpi
    D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

CWidgetFrm::CWidgetFrm(CWidgetFrm *pPrev, LPCTSTR szFile) :
hMenu(nullptr), uTimer(1), pNext(pPrev)
{
    if (nullptr == pD2DFactory)
        ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    else
        pD2DFactory->AddRef();

    uAlpha = (BYTE)CConfig::Widget(szFile).Alpha();
    _tcscpy_s(szPath, _countof(szPath), szFile);
}

CWidgetFrm::~CWidgetFrm()
{
    if (0 == pD2DFactory->Release()) pD2DFactory = nullptr;
    if (pNext) delete pNext;
}

LRESULT CWidgetFrm::OnCreate(LPCREATESTRUCT pParam)
{
    HRESULT hr = S_OK;
    TCHAR szMenu[MAX_PATH] = { 0 };
    MENUITEMINFO mi = { sizeof(MENUITEMINFO) };
    // 加载菜单
    hMenu = ::GetSubMenu(::LoadMenu(pParam->hInstance, MAKEINTRESOURCE(IDR_MAIN)), 1);
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

    BOOL_CHECK(::SetWindowPos(hWnd, nullptr, CConfig::Widget(szPath).Left(), CConfig::Widget(szPath).Top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER));
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CWidgetFrm::OnDestroy()
{
    RECT rcWnd;
    ::GetWindowRect(hWnd, &rcWnd);
    CConfig::Widget(szPath).Left(rcWnd.left);
    CConfig::Widget(szPath).Top(rcWnd.top);

    // 删除定时器
    ::KillTimer(hWnd, uTimer);

    // 释放菜单资源
    if (nullptr != hMenu) ::DestroyMenu(hMenu);

    return ::PostMessage(::GetParent(hWnd), CMainFrm::WM_WIDGETDESTROYED, 0, reinterpret_cast<LPARAM>(this));
}

LRESULT CWidgetFrm::OnRender()
{
    ID2D1RenderTarget *pTarget = nullptr;
    ID2D1GdiInteropRenderTarget *pGdiTarget = nullptr;
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
    BOOL_CHECK(::UpdateLayeredWindow(hWnd, nullptr, nullptr, &sz, hDC, &pt, 0, &blend, ULW_ALPHA));

    HR_CHECK(pGdiTarget->ReleaseDC(nullptr));
    HR_CHECK(pTarget->EndDraw());
 
    if (wImage.GetFrameCount() > 1)
        uTimer = ::SetTimer(hWnd, uTimer, wImage.GetFrameDelay(), nullptr);
    HR_CHECK(wImage.NextFrame());
exit:
    SafeRelease(pTarget);
    SafeRelease(pGdiTarget);
    return hr;
}

LRESULT CWidgetFrm::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (IDM_EXIT == LOWORD(wParam))
        {
            CConfig::Widget(szPath).Show(FALSE);
            return ::PostMessage(hWnd, WM_CLOSE, 0, 0L);
        }
        if (::CheckMenuRadioItem(hMenu, WM_ALPHA, WM_ALPHA + 0xFF, (UINT)wParam, MF_BYCOMMAND))
        {
            BLENDFUNCTION blend = { AC_SRC_OVER, 0, (BYTE)(wParam - WM_ALPHA), AC_SRC_ALPHA };
            CConfig::Widget(szPath).Alpha(uAlpha = blend.SourceConstantAlpha);
            return ::UpdateLayeredWindow(hWnd, nullptr, nullptr, nullptr, nullptr, nullptr, 0, &blend, ULW_ALPHA);
        }
    case WM_LBUTTONDOWN:
        // 鼠标点击拖动窗口
        ::PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        break;
    case WM_CONTEXTMENU:
        // 右键菜单
        return ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hWnd, nullptr);
    case WM_SHOWWINDOW:
        if (TRUE == wParam)
        {
            CConfig::Widget(szPath).Show(TRUE);
            return OnRender();
        }
        ::KillTimer(hWnd, uTimer);
        break;
    case WM_TIMER:
        if (wParam == uTimer)
        {
            return OnRender();
        }
        break;
    case WM_DISPLAYCHANGE:
        break;
    case WM_DESTROY:
        return OnDestroy();
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}