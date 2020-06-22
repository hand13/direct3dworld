#pragma once
#include "base_direct.h"
#include <memory>
#include "imgui.h"
struct Texture{
    ComPtr<ID3D11ShaderResourceView> textureView;
    UINT width;
    UINT height;
};
class ImDirectWorld : public DirectWorld{
    private:
    ImVec4 clearColor;
    Texture imageTexture;
    public:
    ImDirectWorld(ImVec4 clearColor);
    virtual void draw();
    virtual bool init(HWND hwnd);
    virtual bool initResource();
    void setClearColor(ImVec4 clearColor){
        this->clearColor = clearColor;
    }
    bool isInited();
};