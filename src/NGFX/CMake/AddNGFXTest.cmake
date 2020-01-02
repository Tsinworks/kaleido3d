function(add_ngfx_test TARGET)
    cmake_parse_arguments(${TARGET}
        ""
        "FOLDER;PCH;PCH_SRC;PCH_INCLUDE;GUI"
        "SRCS;LIBS;INCLUDES;SHADERS"
        ${ARGN}
    )
    if(WIN32)
        add_executable(ngfx.test.${TARGET} WIN32        ${${TARGET}_SRCS})
    else()
        add_executable(ngfx.test.${TARGET}				${${TARGET}_SRCS})
    endif()
	target_include_directories(ngfx.test.${TARGET}	PRIVATE ${KCORE_INCLUDE_DIR} ${KNGFX_INCLUDE_DIR} ${${TARGET}_INCLUDES})
	target_link_libraries(ngfx.test.${TARGET}		Kaleido3D.Core Kaleido3D.NGFXSHELL ${${TARGET}_LIBS})
    set_target_properties(ngfx.test.${TARGET} PROPERTIES 
		FOLDER "Tests"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin")

endfunction()