#pragma once
#include "base_direct.h"
#include <memory>
#include <windows.h>
#include "imgui.h"
#include <vector>
#define BUFFERSIZE 128
struct Texture{
    ComPtr<ID3D11ShaderResourceView> textureView;
    UINT width;
    UINT height;
};
class ImDirectWorld : public DirectWorld{
    private:
    ImVec4 clearColor;
    Texture imageTexture;
    void switchImage(const wchar_t * img_path);
    char image_path_buffer[BUFFERSIZE];
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
class ImageRender {
    private:
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11RenderTargetView> target_view;
    ComPtr<ID3D11ShaderResourceView> in_resource_view;
    ComPtr<ID3D11ShaderResourceView> out_resource_view;
    ComPtr<ID3D11VertexShader> vertex_shader;
    ComPtr<ID3D11InputLayout> input_layout;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11Buffer> vertex_buffer;
    ComPtr<ID3D11Buffer> index_buffer;
    bool inited;
    UINT width;
    UINT height;
    public:
    ImageRender(ComPtr<ID3D11Device> device,ComPtr<ID3D11DeviceContext> context) {
        this->m_context = context;
        this->m_device = device;
        inited = false;
    }
    void init_resource(const wchar_t* vertex_path,const wchar_t *pixel_path,ComPtr<ID3D11ShaderResourceView> in_resource_view);
    void render();
    void createTargetView();
    void init_buffer();
    void set_viewport();
    ComPtr<ID3D11ShaderResourceView> ImageRender::output();
};