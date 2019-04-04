/**
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#pragma once

#include "IPlatformManager.h"

class LAppManager : public live2d::framework::IPlatformManager
{
public:
    LAppManager();
    ~LAppManager();

    void setGraphicsContext(ID3D11Device* device, ID3D11DeviceContext* context);
    unsigned char *loadBytes(const char *path, size_t *size);
    void releaseBytes(void *data);
    live2d::ALive2DModel *loadLive2DModel(const char *path);
    live2d::framework::L2DTextureDesc *loadTexture(live2d::ALive2DModel *model, int no, const char *path);
    void log(const char *txt);

private:
    UINT _WICBitsPerPixel(REFGUID targetGuid);

private:
    ID3D11Device *_device;
    ID3D11DeviceContext *_deviceContext;
    IWICImagingFactory *_wicFactory;
};
