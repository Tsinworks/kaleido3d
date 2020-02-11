set(SCRIPT_SRCS
V8Binding.h
V8Binding.cpp)

source_group(V8 FILES ${SCRIPT_SRCS})

include(third_party_clibs)
include(V8)

set(V8_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/.build/third_party/include/v8)

k3d_add_lib(V8 
	SRCS ${SCRIPT_SRCS}
	LIBS Kaleido3D.Core v8::Base v8::Platform v8::V8
	FOLDER "Runtime" 
	INCLUDES Public ../Core ${V8_INCLUDE_DIR})

function(script_add_app TARGET)
    cmake_parse_arguments(${TARGET}
        ""
        "FOLDER;PCH;PCH_SRC;PCH_INCLUDE;GUI"
        "SRCS;LIBS;INCLUDES;SHADERS"
        ${ARGN}
    )
    if(WIN32)
        add_executable(JsApp.${TARGET} WIN32        ${${TARGET}_SRCS})
    else()
        add_executable(JsApp.${TARGET}				${${TARGET}_SRCS})
    endif()
	target_include_directories(JsApp.${TARGET}	PRIVATE ${KCORE_INCLUDE_DIR} ${KNGFX_INCLUDE_DIR} ${V8_INCLUDE_DIR} ${${TARGET}_INCLUDES})
	target_link_libraries(JsApp.${TARGET}		Kaleido3D.Core Kaleido3D.NGFXSHELL v8::Base v8::Platform v8::V8 ${${TARGET}_LIBS})
    set_target_properties(JsApp.${TARGET} PROPERTIES 
		FOLDER "JavaScript"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin")

endfunction()

add_subdirectory(Test)