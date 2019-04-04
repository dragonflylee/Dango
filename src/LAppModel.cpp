/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#include "StdAfx.h"
//Live2D Application
#include "LAppModel.h"
#include "LAppDefine.h"
#include "ModelSettingJson.h"
#include "ModelLoader.h"

#include <util\UtSystem.h>
#include <L2DStandardID.h>
#include <sstream>

using namespace std;
using namespace live2d;
using namespace live2d::framework;

LAppModel::LAppModel() : L2DBaseModel(), modelSetting(NULL)
{
    if (LAppDefine::DEBUG_LOG)  mainMotionMgr->setMotionDebugMode(true);
}

LAppModel::~LAppModel()
{
    if (LAppDefine::DEBUG_LOG)
        UtDebug::print("delete model\n");
    delete modelSetting;
}

void LAppModel::load(const char *path)
{
    if (LAppDefine::DEBUG_LOG)
        UtDebug::print("load model : %s\n", path);
    updating = true;
    initialized = false;

    int size = 0;
    unsigned char *data = ModelLoader::loadFile(path, &size);
    modelSetting = new ModelSettingJson((char *)data, size);
    ModelLoader::releaseBuffer(data);

    // JSONの入っているフォルダをmodelHomeDirにセット
    modelHomeDir = path;
    modelHomeDir.erase(modelHomeDir.rfind('/') + 1);

    if (LAppDefine::DEBUG_LOG)
        UtDebug::print("create model : %s\n", modelSetting->getModelName());
    updating = true;
    initialized = false;

    // モデルのロード
    if (strcmp(modelSetting->getModelFile(), "") != 0)
    {
        string path = modelSetting->getModelFile();
        path = modelHomeDir + path;
        loadModelData(path.c_str());

        int len = modelSetting->getTextureNum();
        for (int i = 0; i < len; i++)
        {
            string texturePath = modelSetting->getTextureFile(i);
            texturePath = modelHomeDir + texturePath;
            loadTexture(i, texturePath.c_str());
        }
    }

    if (live2DModel == NULL)
    {
        return;
    }

    //Expression
    if (modelSetting->getExpressionNum() > 0)
    {
        int len = modelSetting->getExpressionNum();
        for (int i = 0; i < len; i++)
        {
            string name = modelSetting->getExpressionName(i);
            string file = modelSetting->getExpressionFile(i);
            file = modelHomeDir + file;
            loadExpression(name.c_str(), file.c_str());
        }
    }

    //Physics
    if (strcmp(modelSetting->getPhysicsFile(), "") != 0)
    {
        string path = modelSetting->getPhysicsFile();
        path = modelHomeDir + path;
        loadPhysics(path.c_str());
    }

    //Pose
    if (strcmp(modelSetting->getPoseFile(), "") != 0)
    {
        string path = modelSetting->getPoseFile();
        path = modelHomeDir + path;
        loadPose(path.c_str());
    }

    // 目パチ
    if (eyeBlink == NULL)
    {
        eyeBlink = new L2DEyeBlink();
    }

    //Layout
    map<string, float> layout;
    modelSetting->getLayout(layout);
    modelMatrix->setupLayout(layout);

    for (int i = 0; i < modelSetting->getInitParamNum(); i++)
    {
        live2DModel->setParamFloat(modelSetting->getInitParamID(i), modelSetting->getInitParamValue(i));
    }

    for (int i = 0; i < modelSetting->getInitPartsVisibleNum(); i++)
    {
        live2DModel->setPartsOpacity(modelSetting->getInitPartsVisibleID(i), modelSetting->getInitPartsVisibleValue(i));
    }

    live2DModel->saveParam();
    preloadMotionGroup(MOTION_GROUP_IDLE);
    mainMotionMgr->stopAllMotions();

    updating = false;   // 更新状態の完了
    initialized = true; // 初期化完了
}

void LAppModel::preloadMotionGroup(const char group[])
{
    int len = modelSetting->getMotionNum(group);
    for (int i = 0; i < len; i++)
    {
        std::stringstream ss;

        //ex) idle_0
        ss << group << "_" << i;

        string name = ss.str();
        string path = modelSetting->getMotionFile(group, i);
        path = modelHomeDir + path;

        if (LAppDefine::DEBUG_LOG)
            UtDebug::print("load motion name:%s ", name.c_str());

        loadMotion(name.c_str(), path.c_str());
    }
}

void LAppModel::update()
{
    dragMgr->update();
    dragX = dragMgr->getX();
    dragY = dragMgr->getY();

    //-----------------------------------------------------------------
    live2DModel->loadParam(); // 前回セーブされた状態をロード
    if (mainMotionMgr->isFinished())
    {
        // モーションの再生がない場合、待機モーションの中からランダムで再生する
        startRandomMotion(MOTION_GROUP_IDLE, PRIORITY_IDLE);
    }
    else if (!mainMotionMgr->updateParam(live2DModel))
    {
        // メインモーションの更新がないとき
        eyeBlink->setParam(live2DModel); // 目パチ
    }

    live2DModel->saveParam(); // 状態を保存
    //-----------------------------------------------------------------

    if (expressionMgr != NULL)
        expressionMgr->updateParam(live2DModel); //  表情でパラメータ更新（相対変化）

    // ドラッグによる変化
    // ドラッグによる顔の向きの調整
    live2DModel->addToParamFloat(PARAM_ANGLE_X, dragX * 30, 1); // -30から30の値を加える
    live2DModel->addToParamFloat(PARAM_ANGLE_Y, dragY * 30, 1);
    live2DModel->addToParamFloat(PARAM_ANGLE_Z, (dragX * dragY) * -30, 1);

    // ドラッグによる体の向きの調整
    live2DModel->addToParamFloat(PARAM_BODY_ANGLE_X, dragX * 10, 1); // -10から10の値を加える

    // ドラッグによる目の向きの調整
    live2DModel->addToParamFloat(PARAM_EYE_BALL_X, dragX, 1); // -1から1の値を加える
    live2DModel->addToParamFloat(PARAM_EYE_BALL_Y, dragY, 1);

    // 呼吸など
    LDint64 timeMSec = UtSystem::getUserTimeMSec() - startTimeMSec;
    double t = (timeMSec / 1000.0) * 2 * 3.14159; //2*Pi*t

    live2DModel->addToParamFloat(PARAM_ANGLE_X, (float)(15 * sin(t / 6.5345)), 0.5f); // -15 ~ +15 まで周期的に加算。周期は他とずらす。
    live2DModel->addToParamFloat(PARAM_ANGLE_Y, (float)(8 * sin(t / 3.5345)), 0.5f);
    live2DModel->addToParamFloat(PARAM_ANGLE_Z, (float)(10 * sin(t / 5.5345)), 0.5f);
    live2DModel->addToParamFloat(PARAM_BODY_ANGLE_X, (float)(4 * sin(t / 15.5345)), 0.5f);
    live2DModel->setParamFloat(PARAM_BREATH, (float)(0.5f + 0.5f * sin(t / 3.2345)), 1); // 0~1 まで周期的に設定。モーションを上書き。

    if (physics != NULL)
        physics->updateParam(live2DModel); // 物理演算でパラメータ更新

    // リップシンクの設定
    if (lipSync)
    {
        float value = 0; // リアルタイムでリップシンクを行う場合、システムから音量を取得して0～1の範囲で入力してください。
        live2DModel->setParamFloat(PARAM_MOUTH_OPEN_Y, value, 0.8f);
    }

    // ポーズの設定
    if (pose != NULL)
        pose->updateParam(live2DModel);

    live2DModel->update();
}

int LAppModel::startMotion(const char group[], int no, int priority)
{
    if (priority == PRIORITY_FORCE)
    {
        mainMotionMgr->setReservePriority(priority);
    }
    else if (!mainMotionMgr->reserveMotion(priority))
    {
        if (LAppDefine::DEBUG_LOG)
            UtDebug::print("can't start motion.\n");
        return -1;
    }

    const char *fileName = modelSetting->getMotionFile(group, no);
    std::stringstream ss;

    //ex) idle_0
    ss << group << "_" << no;

    string name = ss.str();
    AMotion *motion = motions[name.c_str()];
    bool autoDelete = false;
    if (motion == NULL)
    {
        // 読み込み
        string path = fileName;
        path = modelHomeDir + path;
        motion = loadMotion(NULL, path.c_str());

        autoDelete = true; // 終了時にメモリから削除
    }

    motion->setFadeIn(modelSetting->getMotionFadeIn(group, no));
    motion->setFadeOut(modelSetting->getMotionFadeOut(group, no));

    if (LAppDefine::DEBUG_LOG)
        UtDebug::print("start motion ( %s : %d )", group, no);
    return mainMotionMgr->startMotionPrio(motion, autoDelete, priority);
}

int LAppModel::startRandomMotion(const char name[], int priority)
{
    if (modelSetting->getMotionNum(name) == 0)
        return -1;
    int no = rand() % modelSetting->getMotionNum(name);

    return startMotion(name, no, priority);
}

/*
 * モデルを描画する。
 * プラットフォームごとの固有設定も行う。
 * モデルが設定されてない場合は何もしない。
 */
void LAppModel::draw(const DirectX::XMMATRIX &view, const DirectX::XMMATRIX &projection)
{
    if (live2DModel == NULL)
        return;

    DirectX::XMMATRIX trans(modelMatrix->getArray());
    DirectX::XMFLOAT4X4 wvp;
    // wvp = model x view x projection
    DirectX::XMStoreFloat4x4(&wvp,
        DirectX::XMMatrixMultiplyTranspose(
        DirectX::XMMatrixMultiply(trans, view), //  モデルの変換を適用
        projection));                           // 投影変換を適用

    // 変換行列をモデルに設定
    ((Live2DModelD3D11 *)live2DModel)->setMatrix(reinterpret_cast<float *>(wvp.m));

    //  Live2Dを描画
    live2DModel->draw();
}

/*
 * 当たり判定との簡易テスト。
 * 指定IDの頂点リストからそれらを含む最大の矩形を計算し、点がそこに含まれるか判定
 */
bool LAppModel::hitTest(const char pid[], float testX, float testY)
{
    if (alpha < 1)
        return false; // 透明時は当たり判定なし。
    int len = modelSetting->getHitAreasNum();
    for (int i = 0; i < len; i++)
    {
        if (strcmp(modelSetting->getHitAreaName(i), pid) == 0)
        {
            const char *drawID = modelSetting->getHitAreaID(i);
            return hitTestSimple(drawID, testX, testY);
        }
    }
    return false; // 存在しない場合はfalse
}

void LAppModel::setExpression(const char expressionID[])
{
    AMotion *motion = expressions[expressionID];
    if (LAppDefine::DEBUG_LOG)
        UtDebug::print("expression[%s]\n", expressionID);
    if (motion != NULL)
    {
        expressionMgr->startMotion(motion, false);
    }
    else
    {
        if (LAppDefine::DEBUG_LOG)
            UtDebug::print("expression[%s] is null \n", expressionID);
    }
}

void LAppModel::setRandomExpression()
{
    int no = rand() % expressions.size();
    map<string, AMotion *>::const_iterator map_ite;
    int i = 0;
    for (map_ite = expressions.begin(); map_ite != expressions.end(); map_ite++)
    {
        if (i == no)
        {
            string name = (*map_ite).first;
            setExpression(name.c_str());
            return;
        }
        i++;
    }
}

void LAppModel::deviceLost()
{
    ((Live2DModelD3D11 *)live2DModel)->deviceLostD3D();
}
