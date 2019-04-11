﻿/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#pragma once

#include <string>
#include <map>

class ModelSetting
{
public:
    virtual ~ModelSetting() {}

    // モデルデータについて
    virtual const char *getModelName() = 0;
    virtual const char *getModelFile() = 0;

    // テクスチャについて
    virtual int getTextureNum() = 0;
    virtual const char *getTextureFile(int n) = 0;

    // 初期パラメータについて
    virtual int getInitParamNum() = 0;
    virtual float getInitParamValue(int n) = 0;
    virtual const char *getInitParamID(int n) = 0;

    // 初期パーツ表示について
    virtual int getInitPartsVisibleNum() = 0;
    virtual float getInitPartsVisibleValue(int n) = 0;
    virtual const char *getInitPartsVisibleID(int n) = 0;

    // あたり判定について
    virtual int getHitAreasNum() = 0;
    virtual const char *getHitAreaID(int n) = 0;
    virtual const char *getHitAreaName(int n) = 0;

    // 物理演算、パーツ切り替え、表情ファイルについて
    virtual const char *getPhysicsFile() = 0;
    virtual const char *getPoseFile() = 0;

    virtual int getExpressionNum() = 0;
    virtual const char *getExpressionName(int n) = 0;
    virtual const char *getExpressionFile(int n) = 0;

    // モーションについて
    virtual int getMotionNum(const char *name) = 0;
    virtual const char *getMotionFile(const char *name, int n) = 0;
    virtual const char *getMotionSound(const char *name, int n) = 0;
    virtual int getMotionFadeIn(const char *name, int n) = 0;
    virtual int getMotionFadeOut(const char *name, int n) = 0;

    virtual bool getLayout(std::map<std::string, float> &layout) = 0;
};