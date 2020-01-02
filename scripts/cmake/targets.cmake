
function(k3d_add_lib TARGET)
    cmake_parse_arguments(${TARGET}
        ""
        "FOLDER;PCH;PCH_SRC;PCH_INCLUDE"
        "SRCS;LIBS;INCLUDES"
        ${ARGN}
    )
    if(BUILD_SHARED)
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
    if(BUILD_SHARED)
		target_compile_definitions(Kaleido3D.${TARGET} PRIVATE BUILD_SHARED_LIB=1)
	endif()
    target_include_directories(Kaleido3D.${TARGET} PUBLIC ${${TARGET}_INCLUDES})
    set_target_properties(Kaleido3D.${TARGET} PROPERTIES 
		FOLDER "${${TARGET}_FOLDER}"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin"	
	)
endfunction()
