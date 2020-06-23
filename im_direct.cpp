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
    bool ret = LoadTextureFromFile(image_path,this->getDevice().Get()
    , imageTexture.textureView.GetAddressOf(), &imageTexture.width, &imageTexture.height);
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
            ImGui::Image((void*)imageTexture.textureView.Get(), ImVec2(ImGui::GetWindowWidth()/2.0f,ImGui::GetWindowHeight()/2.0f));
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