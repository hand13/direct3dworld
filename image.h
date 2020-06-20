#pragma once
#include <d3d11.h>
bool LoadTextureFromFile(const wchar_t * filename,ID3D11Device * pd3dDevice,ID3D11ShaderResourceView ** out_srv,int * out_width,int * out_height);
