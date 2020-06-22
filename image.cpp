#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <WICTextureLoader.h>
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
