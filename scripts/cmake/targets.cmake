
function(k3d_add_lib TARGET)
    cmake_parse_arguments(${TARGET}
        ""
        "FOLDER;PCH;PCH_SRC;PCH_INCLUDE"
        "SRCS;LIBS;INCLUDES"
        ${ARGN}
    )
    if(BUILD_SHARED)
        add_definitions(-DBUILD_SHARED_LIB)
        if(IOS)
            add_ios_framework(Kaleido3D.${TARGET} SRCS ${${TARGET}_SRCS} LIBS ${${TARGET}_LIBS})
        else()
            add_library(Kaleido3D.${TARGET} SHARED ${${TARGET}_SRCS})
            target_link_libraries(Kaleido3D.${TARGET} ${${TARGET}_LIBS})
        endif(IOS)
    else()
        add_library(Kaleido3D.${TARGET} STATIC ${${TARGET}_SRCS})
    endif()
    if(${TARGET}_PCH AND ${TARGET}_PCH_SRC AND ${TARGET}_PCH_INCLUDE)
        if(WIN32)
            add_precompiled_header(Kaleido3D.${TARGET} "${${TARGET}_PCH}" "${${TARGET}_PCH_INCLUDE}" "${${TARGET}_PCH_SRC}")
            target_compile_definitions(Kaleido3D.${TARGET} PUBLIC K3DPLATFORM_OS_WIN=1)
        endif()
    endif()
    target_include_directories(Kaleido3D.${TARGET} PUBLIC ${${TARGET}_INCLUDES})
    set_target_properties(Kaleido3D.${TARGET} PROPERTIES FOLDER "${${TARGET}_FOLDER}")
endfunction()
