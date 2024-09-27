PROJECT(mmc)

list(APPEND SOURCE
    main.cpp
    MainFrame.cpp
    ChildFrame.cpp
    AddRemoveDialog.cpp
    NodeManager.cpp
    precomp.h
)

add_rc_deps(mmc.rc ${CMAKE_CURRENT_SOURCE_DIR}/resources/mmc.ico)
add_executable(mmc ${SOURCE} mmc.rc)
set_module_type(mmc win32gui UNICODE)
target_link_libraries(mmc comsupp cpprt atl_classes cppstl)
add_importlibs(mmc user32 comdlg32 advapi32 ole32 comctl32 msvcrt kernel32 ntdll)
add_pch(mmc precomp.h SOURCE)
add_cd_file(TARGET mmc DESTINATION reactos/system32 FOR all)
