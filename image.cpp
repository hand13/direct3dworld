#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <WICTextureLoader.h>
#include "screenutils.h"
bool LoadTextureFromFile(const wchar_t * filename,::ID3D11Device * pd3dDevice,ID3D11ShaderResourceView ** out_srv,UINT * out_width,UINT * out_height) {
  ID3D11Resource * texture = nullptr;
  HRESULT hr = DirectX::CreateWICTextureFromFile(pd3dDevice,filename,&texture,out_srv);
  if(SUCCEEDED(hr)){
    ID3D11Texture2D * tmptexture = nullptr;
    if(texture->QueryInterface(&tmptexture) == S_OK){
      D3D11_TEXTURE2D_DESC desc;
      tmptexture->GetDesc(&desc);
      *out_width = desc.Width;
      *out_height = desc.Height;
      tmptexture->Release();
    }
    if(texture != nullptr) {
      texture->Release();
    }
  }else {
    return false;
  }
  return true;
}

//reverse
//BRG
bool LoadTextureFromRawMemory(Buffer buffer,::ID3D11Device * pd3dDevice,ID3D11ShaderResourceView ** out_srv,UINT * out_width,UINT * out_height) {
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = buffer.width;
    desc.Height = buffer.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D *pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = buffer.data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();
    *out_width = buffer.width;
    *out_height = buffer.height;
    return true;
}
