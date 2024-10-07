/* DO NOT USE THE PRECOMPILED HEADER FOR THIS FILE! */

#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H

#include <windef.h>
#include <winbase.h>
#include <objbase.h>

#include <initguid.h>

#include <unknwn.h>
#include <comcat.h>
#include <mmc.h>

#include "mmcndmgr_classes.h"
DEFINE_GUID(NODETYPE_STATICFOLDERNODE, 0xC96401CE, 0x0E17, 0x11D3, 0x88, 0x5B, 0x00, 0xC0, 0x4F, 0x72, 0xFF, 0xFF);

/* NO CODE HERE, THIS IS JUST REQUIRED FOR THE GUID DEFINITIONS */