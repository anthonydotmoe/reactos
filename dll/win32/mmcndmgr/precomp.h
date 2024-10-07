#ifndef _MMCNDMGR_PCH_
#define _MMCNDMGR_PCH_

#pragma once

#define WIN32_NO_STATUS
//#define _INC_WINDOWS
#define COM_NO_WINDOWS_H

#include <windef.h>
#include <winbase.h>
#include <winreg.h>

#include <tchar.h>
#include <strsafe.h>

#include <atlbase.h>
#include <atlcom.h>

#include <mmc.h>

#include "Logger.h"
#include "resource.h"
#include <string>

#include "mmcndmgr_classes.h"

#include "FolderSnapin.h"


#endif // _MMCNDMGR_PCH_