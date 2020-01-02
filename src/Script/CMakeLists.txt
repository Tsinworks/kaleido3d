set(SCRIPT_SRCS
V8Binding.h
V8Binding.cpp)

source_group(V8 FILES ${SCRIPT_SRCS})

k3d_add_lib(V8 
	SRCS ${SCRIPT_SRCS}
	LIBS Kaleido3D.Core 
	FOLDER "Runtime" 
	INCLUDES Public ../Core ${CMAKE_SOURCE_DIR}/.build/third_party/include/v8)
