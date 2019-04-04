/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#include "StdAfx.h"
#include "LAppRenderer.h"
#include "LAppDefine.h"

LAppRenderer::LAppRenderer()
{
    // 表示範囲の設定
    viewMatrix.identity();
    viewMatrix.setMaxScale(VIEW_MAX_SCALE); // 限界拡大率
    viewMatrix.setMinScale(VIEW_MIN_SCALE); // 限界縮小率

    // 表示できる最大範囲
    viewMatrix.setMaxScreenRect(
        VIEW_LOGICAL_MAX_LEFT,
        VIEW_LOGICAL_MAX_RIGHT,
        VIEW_LOGICAL_MAX_BOTTOM,
        VIEW_LOGICAL_MAX_TOP);
}

/***********************************************************
    Live2D描画の標準的な座標系は画面中心を 0,0
    左上を(-1,1) , 右下を(1,-1)とする座標として設定する
    ************************************************************/
void LAppRenderer::setDeviceSize(int width, int height)
{
    live2d::UtDebug::println(" set Device size : %d , %d", width, height);

    // 表示範囲
    float ratio = (float)height / width;
    float left = VIEW_LOGICAL_LEFT;
    float right = VIEW_LOGICAL_RIGHT;
    float bottom = -ratio;
    float top = ratio;
    viewMatrix.setScreenRect(left, right, bottom, top); // デバイスに対応する画面の範囲。 Xの左端, Xの右端, Yの下端, Yの上端

    float screenW = abs(left - right);

    deviceToScreen.identity();
    deviceToScreen.multTranslate(-width / 2.0f, -height / 2.0f);
    deviceToScreen.multScale(screenW / width, -screenW / width);
}

void LAppRenderer::draw(LAppModel *model, size_t count)
{
    // 画面のLeft Top (-1,1) , Right Bottom (1,-1) , z = 0 となるViewを前提にLive2Dを描画
    // そのための投影変換行列を定義する
    DirectX::XMMATRIX ortho = DirectX::XMMatrixOrthographicOffCenterLH(
        viewMatrix.getScreenLeft(),
        viewMatrix.getScreenRight(),
        viewMatrix.getScreenBottom(),
        viewMatrix.getScreenTop(), -1.0f, 1.0f);
    DirectX::XMMATRIX view(viewMatrix.getArray());

    for (size_t i = 0; i < count; i++)
    {
        model[i].update();
        model[i].draw(view, ortho);
    }
}

// マウスホイール時
void LAppRenderer::mouseWheel(float scale, int x, int y)
{
    float x_onScreen = deviceToScreen.transformX((float)x);
    float y_onScreen = deviceToScreen.transformY((float)y);


    // 画面の拡大縮小、移動の設定
    updateViewMatrix(0, 0, x_onScreen, y_onScreen, scale);
}

void LAppRenderer::scaleView(float cx, float cy, float scale)
{
    viewMatrix.adjustScale(cx, cy, scale);
}

void LAppRenderer::translateView(float shiftX, float shiftY)
{
    viewMatrix.adjustTranslate(shiftX, shiftY);
}

void LAppRenderer::updateViewMatrix(float dx, float dy, float cx, float cy, float scale)
{
    bool isMaxScale = viewMatrix.isMaxScale();
    bool isMinScale = viewMatrix.isMinScale();

    // 拡大縮小
    viewMatrix.adjustScale(cx, cy, scale);

    // 移動(ホイールの場合は移動を伴わないので0,0(何もしない)。タッチの場合は移動＋拡大になる）
    viewMatrix.adjustTranslate(dx, dy);

    // 画面が最大になったときのイベント
    if (!isMaxScale)
    {
        if (viewMatrix.isMaxScale())
        {
            // 最大表示になった時に何らかのイベント（アクション等）を行う場合はここに記述
            if (LAppDefine::DEBUG_LOG)
                live2d::UtDebug::println("max scale");
        }
    }
    // 画面が最小になったときのイベント
    if (!isMinScale)
    {
        if (viewMatrix.isMinScale())
        {
            // 最小表示になった時に何らかのイベント（アクション等）を行う場合はここに記述
            if (LAppDefine::DEBUG_LOG)
                live2d::UtDebug::println("min scale");
        }
    }
}

float LAppRenderer::transformDeviceToViewX(float deviceX)
{
    float screenX = deviceToScreen.transformX(deviceX); // 論理座標変換した座標を取得。
    return viewMatrix.invertTransformX(screenX);        // 拡大、縮小、移動後の値。
}

float LAppRenderer::transformDeviceToViewY(float deviceY)
{
    float screenY = deviceToScreen.transformY(deviceY); // 論理座標変換した座標を取得。
    return viewMatrix.invertTransformY(screenY);        // 拡大、縮小、移動後の値。
}
