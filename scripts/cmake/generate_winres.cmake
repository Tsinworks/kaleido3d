
string(TIMESTAMP YEAR "%Y")
set(COPY_RIGHT "Copyright (C) ${YEAR} Tsin Studio.")

function(generate_win_res TARGET)
    cmake_parse_arguments(${TARGET}
        ""
        "VER;DESC"
        ""
        ${ARGN}
    )
    set(__TEMPLATE_RC "\
    \#include <windows.h>\r\n\
    VS_VERSION_INFO VERSIONINFO\r\n\
        FILEVERSION 1,1,0,0\r\n\
        PRODUCTVERSION 1,1,0,0\r\n\
        FILEFLAGSMASK 0x3fL\r\n\
    \#ifdef _DEBUG\r\n\
        FILEFLAGS VS_FF_DEBUG\r\n\
    \#else\r\n\
        FILEFLAGS 0x0L\r\n\
    \#endif\r\n\
        FILEOS VOS__WINDOWS32\r\n\
        FILETYPE VFT_DLL\r\n\
        FILESUBTYPE 0x0L\r\n\
        BEGIN\r\n\
            BLOCK \"StringFileInfo\"\r\n\
            BEGIN\r\n\
                BLOCK \"040904b0\"\r\n\
                BEGIN\r\n\
                    VALUE \"CompanyName\", \"{COMPANY}.\\0\"\r\n\
                    VALUE \"FileDescription\", \"{DESCRIPTION}.\\0\"\r\n\
                    VALUE \"FileVersion\", \"{_VERSION_}\\0\"\r\n\
                    VALUE \"LegalCopyright\", \"{COPYRIGHT}\\0\"\r\n\
                    VALUE \"ProductName\", \"{PRODUCT}\\0\"\r\n\
                    VALUE \"ProductVersion\", \"{_VERSION_}\\0\"\r\n\
                END\r\n\
            END\r\n\
            BLOCK \"VarFileInfo\"\r\n\
            BEGIN\r\n\
                VALUE \"Translation\", 0x0409, 1200\r\n\
            END\r\n\
        END\r\n\
    ")
    string(REPLACE "{_VERSION_}" "${${TARGET}_VER}" __TEMPLATE_RC ${__TEMPLATE_RC})
    string(REPLACE "{DESCRIPTION}" "${${TARGET}_DESC}" __TEMPLATE_RC ${__TEMPLATE_RC})
    string(REPLACE "{COPYRIGHT}" ${COPY_RIGHT} __TEMPLATE_RC ${__TEMPLATE_RC})
    string(REPLACE "{PRODUCT}" "${TARGET}" __TEMPLATE_RC ${__TEMPLATE_RC})
    string(REPLACE "{COMPANY}" "Tsin Studio" __TEMPLATE_RC ${__TEMPLATE_RC})
    set(${TARGET}_RC "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${TARGET}_WinRes.rc" PARENT_SCOPE)
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${TARGET}_WinRes.rc" "${__TEMPLATE_RC}")
endfunction()