#pragma once
#include <memory>
#include <windows.h>
struct Buffer {
    UINT width;
    UINT height;
    UINT8 * data;
    int size;
};
Buffer getbuffer(RECT rect);