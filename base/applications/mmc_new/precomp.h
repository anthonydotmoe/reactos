#pragma once

#include <tchar.h>
#include <stdarg.h>

#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>

#ifdef _WIN32_WINNT
#define MYOLD_WIN32_WINNT_WOAH _WIN32_WINNT
#define _WIN32_WINNT 0x0600

/* UH OH! This whole thing was to make RegLoadMUIStringW work, but it won't! */

#include <winreg.h>

#define REG_MUI_STRING_TRUNCATE 0x00000001

#define _WIN32_WINNT MYOLD_WIN32_WINNT_WOAH
#undef MYOLD_WIN32_WINNT_WOAH

#endif

#include <commctrl.h>
#include <commdlg.h>

#include <atlbase.h>
#include <atlwin.h>

#include <objbase.h>

#include <list>

#include "resource.h"