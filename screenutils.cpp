#include "screenutils.h"
#include <windows.h>
Buffer getbuffer(RECT rect) {
    Buffer result;
    HDC screenHDC = GetDC(nullptr);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    HBITMAP bitMap = CreateCompatibleBitmap(screenHDC,width,height);
    HDC memoryHDC = CreateCompatibleDC(screenHDC);
    SelectObject(memoryHDC,bitMap);
    BitBlt(memoryHDC,0,0,width,height,screenHDC,rect.left,0,SRCCOPY);
    BITMAP bmpScreen;
    GetObject(bitMap,sizeof(BITMAP),&bmpScreen);
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = bmpScreen.bmWidth;    
    bi.biHeight = -bmpScreen.bmHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;
    result.width = bmpScreen.bmWidth;
    result.height = bmpScreen.bmHeight;
    DWORD dwBmpSize = ((bmpScreen.bmWidth * 32 + 31) / 32) * 4 * bmpScreen.bmHeight;
    UINT8 * data = new UINT8[dwBmpSize];
    GetDIBits(memoryHDC, bitMap, 0,(UINT)bmpScreen.bmHeight,data,
    (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    result.data = data;
    result.size = dwBmpSize;
    ReleaseDC(nullptr,memoryHDC);
    ReleaseDC(nullptr,screenHDC);
    return result;
}