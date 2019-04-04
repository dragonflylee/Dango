#include "StdAfx.h"
#include "Live2DFrm.h"
#include "MainFrm.h"
#include "LAppRenderer.h"
#include "LAppDefine.h"

CLive2DFrm::CLive2DFrm(LPCTSTR szFile) :
hMenu(nullptr), uTimer(1), szPath(szFile),
pRender(new LAppRenderer())
{
    uAlpha = (BYTE)CConfig::Widget(szFile).Alpha();
}

CLive2DFrm::~CLive2DFrm()
{

}

LRESULT CLive2DFrm::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = S_OK;
    TCHAR szMenu[MAX_PATH] = { 0 };
    SIZE szWnd = { 400, 350 };
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

    HR_CHECK(pGraphic.Init(m_hWnd, szWnd.cx, szWnd.cy));

    pRender->setDeviceSize(szWnd.cx, szWnd.cy);

    pModel.GetAt(pModel.Add()).load("http://live2d.fghrsh.net/api/get/?id=5-0");

    BOOL_CHECK(SetWindowPos(nullptr, CConfig::Widget(szPath).Left(), CConfig::Widget(szPath).Top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER));

    uTimer = SetTimer(uTimer, 10);
exit:
    // 返回 -1 表示窗口创建失败
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CLive2DFrm::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    RECT rcWnd;
    GetWindowRect(&rcWnd);
    CConfig::Widget(szPath).Left(rcWnd.left);
    CConfig::Widget(szPath).Top(rcWnd.top);
    KillTimer(uTimer);

    pRender.Free();
    pModel.RemoveAll();
    pGraphic.Shutdown();

    // 释放菜单资源
    if (nullptr != hMenu) ::DestroyMenu(hMenu);
    // 通知主窗口
    return ::PostMessage(GetParent(), CMainFrm::WM_WIDGETDESTROYED, 0, (LPARAM)(LPCTSTR)szPath);
}

LRESULT CLive2DFrm::OnMouseWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // ホイール
    RECT rc;
    float delta = GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? 0.9f : 1.1f;
    GetWindowRect(&rc);

    int nWidth = static_cast<int>((rc.right - rc.left) * delta);
    int nHeight = static_cast<int>((rc.bottom - rc.top) * delta);
    pRender->setDeviceSize(nWidth, nHeight);

    return pGraphic.Resize(nWidth, nHeight);
}

LRESULT CLive2DFrm::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    float vx = pRender->transformDeviceToViewX(static_cast<float>(GET_X_LPARAM(lParam)));
    float vy = pRender->transformDeviceToViewY(static_cast<float>(GET_Y_LPARAM(lParam)));

    for (size_t i = 0; i < pModel.GetCount(); i++)
    {
        if (pModel[i].hitTest(HIT_AREA_HEAD, vx, vy))
        {
            pModel[i].setRandomExpression();
        }
        else if (pModel[i].hitTest(HIT_AREA_BODY, vx, vy))
        {
            pModel[i].startRandomMotion(MOTION_GROUP_TAP_BODY, PRIORITY_NORMAL);
        }
    }
    // 鼠标点击拖动窗口
    return PostMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
}

LRESULT CLive2DFrm::OnContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    return ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_hWnd, nullptr);
}

LRESULT CLive2DFrm::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (TRUE == wParam && pModel.GetCount() > 0)
    {
        CConfig::Widget(szPath).Show((int)wParam);
        return OnFrame();
    }
    bHandled = FALSE;
    return KillTimer(uTimer);
}

LRESULT CLive2DFrm::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = pGraphic->GetDeviceRemovedReason();
    switch (hr)
    {
    case S_OK:
        break;
    case DXGI_ERROR_DEVICE_HUNG:
    case DXGI_ERROR_DEVICE_RESET:
        KillTimer(wParam);
        pGraphic.Shutdown();
        return pGraphic.Init(m_hWnd, 0, 0);
    }
    return OnFrame();
}

LRESULT CLive2DFrm::OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CConfig::Widget(szPath).Show(FALSE);
    return PostMessage(WM_CLOSE);
}

LRESULT CLive2DFrm::OnAlpha(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!::CheckMenuRadioItem(hMenu, WM_ALPHA, WM_ALPHA + 0xFF, wID, MF_BYCOMMAND)) return S_FALSE;
    uAlpha = static_cast<BYTE>(wID - WM_ALPHA);
    CConfig::Widget(szPath).Alpha(uAlpha);
    return S_OK;
}

HRESULT CLive2DFrm::OnFrame()
{
    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);
    ScreenToClient(&pt);

    float vx = pRender->transformDeviceToViewX(static_cast<float>(pt.x));
    float vy = pRender->transformDeviceToViewY(static_cast<float>(pt.y));

    for (size_t i = 0; i < pModel.GetCount(); i++) pModel[i].setDrag(vx, vy);

    pGraphic.BeginScene();

    pRender->draw(pModel.GetData(), pModel.GetCount());

    return pGraphic.EndScene(m_hWnd, uAlpha);
}