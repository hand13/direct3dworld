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
  protected:
    bool inited ;
  public:
    DirectWorld();
    virtual ~DirectWorld();
    virtual void createRenderTarget();
    virtual bool init(HWND hwnd);
    virtual void draw() = 0;
    virtual void setViewport();
    virtual void changeTargetSize(UINT width,UINT height);
    static HRESULT compileD3DFile (const WCHAR * filename,LPCSTR entryPoint,LPCSTR profile,ID3DBlob ** outData);
    static bool createVertexShaderAndInputLayoutFromFile(ComPtr<ID3D11Device> device
    ,const WCHAR * filename,LPCSTR entryPoint,ID3D11VertexShader ** outShader
    ,const D3D11_INPUT_ELEMENT_DESC * input_desc,int input_size,ID3D11InputLayout ** inputLayout);
    static bool createPixelShaderFromFile (ComPtr<ID3D11Device> device,const WCHAR  * filename,LPCSTR entryPoint,ID3D11PixelShader ** outShader);
    ComPtr<ID3D11Device>  getDevice() const ;
    ComPtr<ID3D11DeviceContext>  getContext() const ;
    ComPtr<IDXGISwapChain>  getSwapChain() const ;
    ComPtr<ID3D11RenderTargetView>  getMainRenderTargetView()const ;
};
