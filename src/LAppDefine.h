/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#pragma once

// 画面
static const float VIEW_MAX_SCALE = 4.0f;
static const float VIEW_MIN_SCALE = 0.1f;

static const float VIEW_LOGICAL_LEFT = -1;
static const float VIEW_LOGICAL_RIGHT = 1;

static const float VIEW_LOGICAL_MAX_LEFT = -2;
static const float VIEW_LOGICAL_MAX_RIGHT = 2;
static const float VIEW_LOGICAL_MAX_BOTTOM = -2;
static const float VIEW_LOGICAL_MAX_TOP = 2;

// 外部定義ファイル(json)と合わせる
static const char MOTION_GROUP_IDLE[] = "idle";// アイドリング
static const char MOTION_GROUP_TAP_BODY[] = "tap_body";// 体をタップしたとき

// 外部定義ファイル(json)と合わせる
static const char HIT_AREA_HEAD[] = "head";
static const char HIT_AREA_BODY[] = "body";

// モーションの優先度定数
static const int PRIORITY_NONE = 0;
static const int PRIORITY_IDLE = 1;
static const int PRIORITY_NORMAL = 2;
static const int PRIORITY_FORCE = 3;

class LAppDefine
{
public:
    static const bool DEBUG_LOG = true;         // デバッグ用ログの表示
    static const bool DEBUG_TOUCH_LOG = false;  // タッチしたときの冗長なログの表示
};

