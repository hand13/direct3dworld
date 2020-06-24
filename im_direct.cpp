#include "im_direct.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <iostream>
#include "image.h"
#include "screenutils.h"

ImDirectWorld::ImDirectWorld(ImVec4 clearColor):DirectWorld() {
    this->clearColor = clearColor;
}
bool ImDirectWorld::init(HWND hwnd){

    DirectWorld::init(hwnd);
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if(inited) {
        if(!initResource()) {
            inited = false;
        }
    }
    return inited;
}

bool ImDirectWorld::initResource() {
    //bool ret = LoadTextureFromFile(L"e:/test.jpg",this->getDevice().Get()
    //, imageTexture.textureView.GetAddressOf(), &imageTexture.width, &imageTexture.height);
    RECT rect;
    rect.left = 0;
    rect.right = 1366;
    rect.top = 0;
    rect.bottom = 768;
    //Buffer buffer = getbuffer(rect);
    //bool ret = LoadTextureFromRawMemory(buffer,this->getDevice().Get()
    //, imageTexture.textureView.GetAddressOf(), &imageTexture.width, &imageTexture.height);
    return TRUE;
}

void ImDirectWorld::switchImage(const wchar_t * image_path) {
    if(imageTexture.textureView != nullptr) {
        imageTexture.textureView = nullptr;
    }
    ID3D11ShaderResourceView * tmp = nullptr;
    bool ret = LoadTextureFromFile(image_path,this->getDevice().Get()
    , &tmp, &imageTexture.width, &imageTexture.height);

    ImageRender imageRender(this->getDevice(),this->getContext());
    imageRender.init_resource(L"vertex.hlsl",L"pixel.hlsl",ComPtr<ID3D11ShaderResourceView>(tmp));
    imageRender.render();
    this->imageTexture.textureView = imageRender.output();

}


void ImDirectWorld::draw() {
    if(!inited) {
        std::cout<<"no initialize"<<std::endl;
        return;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Hello, world!");
        ImGui::InputText("image_path",this->image_path_buffer,BUFFERSIZE);
        if(ImGui::Button("show image")) {
            wchar_t * tmpbuffer = new wchar_t[128];
            if(MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,this->image_path_buffer,-1,tmpbuffer,128)>0) {
                switchImage(tmpbuffer);
            }
            delete [] tmpbuffer;
        }
        if(imageTexture.textureView != nullptr) {
            ImGui::Image((void*)imageTexture.textureView.Get(), ImVec2(ImGui::GetWindowWidth(),ImGui::GetWindowHeight()));
        }
        ImGui::End();
    }
    ImGui::Render();
    auto context = this->getContext();
    auto renderTargetView = this->getMainRenderTargetView();
    context->OMSetRenderTargets(1,renderTargetView.GetAddressOf(), NULL);
    context->ClearRenderTargetView(renderTargetView.Get(), reinterpret_cast<float*>(&clearColor));
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    this->getSwapChain()->Present(1, 0); // Present with vsync
}

bool ImDirectWorld::isInited() {
    return inited;
}



/**
 *  imagerender
**/

void ImageRender::init_resource(const wchar_t* vertex_path,const wchar_t *pixel_path,ComPtr<ID3D11ShaderResourceView> in_resource_view) {

    const D3D11_INPUT_ELEMENT_DESC basicVertexLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
        };
    DirectWorld::createVertexShaderAndInputLayoutFromFile(m_device,vertex_path,"main",&vertex_shader
    ,basicVertexLayoutDesc,ARRAYSIZE(basicVertexLayoutDesc),&input_layout
    );

    DirectWorld::createPixelShaderFromFile(m_device,pixel_path,"main",&pixelShader);
    this->in_resource_view = in_resource_view;
    ID3D11Resource * resource = nullptr;
    ID3D11Texture2D * texture2d = nullptr;
    in_resource_view->GetResource(&resource);
    if(SUCCEEDED(resource->QueryInterface(&texture2d))) {
        D3D11_TEXTURE2D_DESC desc;  
        texture2d->GetDesc(&desc);
        width = desc.Width;
        height = desc.Height;
        texture2d->Release();
    }
    resource->Release();
    this->createTargetView();
    this->init_buffer();
}

void ImageRender::createTargetView() {
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Width = this->width;
    desc.Height = this->height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D * texture2d;
    m_device->CreateTexture2D(&desc,nullptr,&texture2d);

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format             = desc.Format;
    renderTargetViewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    m_device->CreateRenderTargetView(texture2d,&renderTargetViewDesc,&this->target_view);

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    shaderResourceViewDesc.Format                       = desc.Format;
    shaderResourceViewDesc.ViewDimension                = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip    = 0;
    shaderResourceViewDesc.Texture2D.MipLevels          = 1;

    m_device->CreateShaderResourceView(texture2d, &shaderResourceViewDesc, &this->out_resource_view);

    texture2d->Release();
}

void ImageRender::init_buffer() {
    float vertices[] = {
        -1.0,-1.0,0.0   ,0.0,1.0f,
        1.0,-1.0,0.0     ,1.0,1.0f,
        1.0,1.0,0.0    ,1.0,0.0f,
        -1.0,1.0,0.0   ,0.0,0.0f};

    unsigned short triangleIndices[] = {
        0,3,2,
        2,1,0,};
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc,sizeof(vertexBufferDesc));
    vertexBufferDesc.ByteWidth = sizeof(float) * ARRAYSIZE(vertices);
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    vertexBufferData.pSysMem = vertices;
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    m_device->CreateBuffer(
        &vertexBufferDesc,
        &vertexBufferData,
        &vertex_buffer
        );
    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(triangleIndices);
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA indexBufferData;
    indexBufferData.pSysMem = triangleIndices;
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    m_device->CreateBuffer(
        &indexBufferDesc,
        &indexBufferData,
        &index_buffer
        );
}

void ImageRender::render() {
    m_context->OMSetRenderTargets(
                1,
                target_view.GetAddressOf(),
                nullptr // Use no depth stencil.
                );
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_context->ClearRenderTargetView(
                    target_view.Get(),
                    clearColor
                    );
    set_viewport();
    m_context->IASetInputLayout(input_layout.Get());
    UINT stride = 20 ;
    UINT offset = 0;
    m_context->IASetVertexBuffers(0,1,vertex_buffer.GetAddressOf(),&stride,&offset);
    m_context->IASetIndexBuffer(index_buffer.Get(),DXGI_FORMAT_R16_UINT,0);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->VSSetShader(vertex_shader.Get(),NULL,0);
    m_context->PSSetShader(pixelShader.Get(),NULL,0);
    m_context->PSSetShaderResources(0,1,in_resource_view.GetAddressOf());
    m_context->DrawIndexed(6,0,0);
}
ComPtr<ID3D11ShaderResourceView> ImageRender::output() {
    return this->out_resource_view;
}

void ImageRender::set_viewport() {
      D3D11_VIEWPORT viewport;
      viewport.TopLeftX = 0.0f;
      viewport.TopLeftY = 0.0f;
      viewport.Width = static_cast<float>(this->width);
      viewport.Height = static_cast<float>(this->height);
      viewport.MinDepth = D3D11_MIN_DEPTH;
      viewport.MaxDepth = D3D11_MAX_DEPTH;
      m_context->RSSetViewports(1, &viewport);
}