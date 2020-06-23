#pragma once
#include <d3d11.h>
#include "screenutils.h"
bool LoadTextureFromFile(const wchar_t * filename,ID3D11Device * pd3dDevice,ID3D11ShaderResourceView ** out_srv,UINT * out_width,UINT * out_height);
bool LoadTextureFromRawMemory(Buffer buffer,::ID3D11Device * pd3dDevice,ID3D11ShaderResourceView ** out_srv,UINT * out_width,UINT * out_height);
