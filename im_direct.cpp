#include "im_direct.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <iostream>
#include "image.h"

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
    bool ret = LoadTextureFromFile(L"e:/test.jpg",this->getDevice().Get()
    , imageTexture.textureView.GetAddressOf(), &imageTexture.width, &imageTexture.height);
    return ret;
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
        ImGui::Image((void*)imageTexture.textureView.Get(), ImVec2(imageTexture.width/4.0f,imageTexture.height/4.0f));
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