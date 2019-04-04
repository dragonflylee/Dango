/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#pragma once

#include <DirectXMath.h>
#include "L2DBaseModel.h"
#include "ModelSetting.h"

class LAppModel : public live2d::framework::L2DBaseModel
{
private:
    ModelSetting *modelSetting; // モデルの設定
    std::string modelHomeDir;

public:
    LAppModel();
    ~LAppModel();

    void load(const char *path);
    void update();
    void draw(const DirectX::XMMATRIX &view, const DirectX::XMMATRIX &projection);

    int startMotion(const char name[], int no, int priority);
    int startRandomMotion(const char name[], int priority);

    void setExpression(const char name[]);
    void setRandomExpression();

    void preloadMotionGroup(const char name[]);
    bool hitTest(const char pid[], float testX, float testY);
    void deviceLost();
};
