#include "screenutils.h"
int main() {
    RECT r;
    r.left = 0;
    r.top = 0;
    r.right = 100;
    r.bottom = 100;
    Buffer buffer = getbuffer(r);
    return 0;
}