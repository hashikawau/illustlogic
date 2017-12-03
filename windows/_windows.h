

#ifndef __Windows_windows
#define __Windows_windows


#define UNICODE
#include "windows.h"
#include <iostream.h>
#include <iomanip.h>
MSG msg;

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif  //ifndef WM_MOUSEWHEEL

#ifdef MOUSE_MOVED
#undef MOUSE_MOVED
#endif

#endif //ifndef __Windows_windows

