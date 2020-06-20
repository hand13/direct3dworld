#pragma once
#include <windows.h>
#include <d3d11.h>
#include <directxmath.h>
#include <wrl/client.h>
#include <memory>
#include <SpriteFont.h>
#include <SpriteBatch.h>
using Microsoft::WRL::ComPtr;
class DirectWorld {
  private :
    ComPtr<ID3D11Device>  m_device;
    ComPtr<ID3D11DeviceContext>  m_context;
    ComPtr<IDXGISwapChain>  m_swapChain;
    ComPtr<ID3D11RenderTargetView>  m_mainRenderTargetView;
    std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    bool inited ;
  public:
    DirectWorld();
    ~DirectWorld();
    virtual void createRenderTarget();
    virtual bool init(HWND hwnd);
    virtual void draw();
    virtual void setViewport();
    HRESULT compileD3DFile(const WCHAR * filename,LPCSTR entryPoint,LPCSTR profile,ID3DBlob ** outData);
    bool createVertexShaderFromFile(const WCHAR * filename,LPCSTR entryPoint,ID3D11VertexShader ** outShader);
    bool createPixelShaderFromFile(const WCHAR  * filename,LPCSTR entryPoint,ID3D11PixelShader ** outShader);
    ComPtr<ID3D11Device>  getDevice();
    ComPtr<ID3D11DeviceContext>  getContext();
    ComPtr<IDXGISwapChain>  getSwapChain();
    ComPtr<ID3D11RenderTargetView>  getMainRenderTargetView();
};
