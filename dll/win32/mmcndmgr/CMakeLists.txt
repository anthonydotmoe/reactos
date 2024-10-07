remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x601 -DWINVER=0x601 -D_USRDLL)

spec2def(mmcndmgr.dll mmcndmgr.spec)

list(APPEND SOURCE
    dllmain.cpp
    FolderSnapin.cpp
    Logger.cpp
    precomp.h
    #${CMAKE_CURRENT_BINARY_DIR}/mmcndmgr_stubs.c
)

list(APPEND PCH_SKIP_SOURCE
    guid.c
)

list(APPEND mmcndmgr_rc_deps
    ${CMAKE_CURRENT_SOURCE_DIR}/mmcndmgr_classes.rgs
)

set_source_files_properties(mmcndmgr.rc PROPERTIES OBJECT_DEPENDS "${mmcndmgr_rc_deps}")

add_library(mmcndmgr MODULE
    ${SOURCE}
    ${PCH_SKIP_SOURCE}
    mmcndmgr.rc
    ${CMAKE_CURRENT_BINARY_DIR}/mmcndmgr.def
)

# target_include_directories(mmcndmgr PRIVATE ${REACTOS_SOURCE_DIR}/sdk/lib/atl)

add_idl_headers(mmcndmgr_idlheader mmcndmgr_classes.idl)
add_dependencies(mmcndmgr mmcndmgr_idlheader stdole2)
set_module_type(mmcndmgr win32dll UNICODE)
target_link_libraries(mmcndmgr uuid comsupp cpprt cppstl atl_classes)
add_importlibs(mmcndmgr user32 advapi32 ole32 oleaut32 msvcrt kernel32 ntdll)
add_pch(mmcndmgr precomp.h "${PCH_SKIP_SOURCE}")
add_cd_file(TARGET mmcndmgr DESTINATION reactos/system32 FOR all)
