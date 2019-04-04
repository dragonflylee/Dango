/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#pragma once

#include "L2DViewMatrix.h"
#include "LAppModel.h"

class LAppRenderer
{
private:
    live2d::framework::L2DViewMatrix viewMatrix;
    live2d::framework::L2DMatrix44 deviceToScreen;

public:
    LAppRenderer();

    void draw(LAppModel *model, size_t count);

    void mouseWheel(float scale, int x, int y);

    void scaleView(float cx, float cy, float scale);
    void translateView(float x, float y);
    void updateViewMatrix(float dx, float dy, float cx, float cy, float scale);

    float transformDeviceToViewX(float deviceX);
    float transformDeviceToViewY(float deviceY);

    void setDeviceSize(int width, int height);

    live2d::framework::L2DViewMatrix *getViewMatrix() { return &viewMatrix; }
};
