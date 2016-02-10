#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#include <enet/enet.h>

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"
#endif

#ifdef SERVER
    // OpenCV includes

#else
    // WxWidgets includes
    #include <wx/wxprec.h>

    #ifndef WX_PRECOMP
        #include <wx/wx.h>
    #endif
#endif
