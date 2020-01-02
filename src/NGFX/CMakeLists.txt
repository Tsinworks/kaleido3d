set (NGFX_SRCS
Private/Vulkan/vk_commands.cpp
Private/Vulkan/vk_commands.h
Private/Vulkan/vk_log.cpp
Private/Vulkan/vk_low_level_mem.cpp
Private/Vulkan/vk_ray_tracing.cpp
Private/Vulkan/vk_resource.cpp
Private/Vulkan/vk_pipeline.cpp
Private/Vulkan/vk_swapchain.cpp
Private/Vulkan/volk.c
Private/Vulkan/vk_instance.cpp
Private/Vulkan/vk_device.cpp
Private/Vulkan/vk_common.h
Public/ngfx.h
Public/ngfx_allocator.h
Public/ngfx_atomic.h
Public/ngfx_container.h
Public/ngfx_hash.h
Public/ngfx_ptr.h
)

source_group(NGFX FILES ${NGFX_SRCS})

set (NGFX_SHADER_COMPILER_SRCS
Private/ShaderCompiler/metal_binary_parser.cpp
Private/ShaderCompiler/metal_binary_parser.h
Private/ShaderCompiler/spirv_parser.cpp
Private/ShaderCompiler/spirv_parser.h
Private/ShaderCompiler/ngfx_shader_compiler.cpp
Private/ShaderFormats/ngfx_shader.cpp
Public/ngfx_shader_compiler.h
Public/ngfx_shader.h
)

source_group(ShaderCompiler FILES ${NGFX_SHADER_COMPILER_SRCS})

set (NGFX_SL_SRCS
Private/ShaderLab/shaderlab.cpp
Private/ShaderLab/sl_parse_pragma.cpp
Private/ShaderLab/sl_parse_prop.cpp
Private/ShaderLab/sl_parse_state.cpp
Private/ShaderLab/sl_parser.cpp
Private/ShaderLab/sl_tokenizer.cpp
Private/ShaderLab/sl_parse_state.cpp
Public/shaderlab.h
)

source_group(ShaderLab FILES ${NGFX_SL_SRCS})

k3d_add_lib(NGFX SRCS ${NGFX_SRCS} ${NGFX_SHADER_COMPILER_SRCS} ${NGFX_SL_SRCS} LIBS Kaleido3D.Core FOLDER "Runtime" INCLUDES Public ../Core)

set(NGFX_SHELL_SRCS
Public/ngfx_shell.h
Private/ngfx_shell.cpp)
source_group(sh FILES ${NGFX_SHELL_SRCS})
k3d_add_lib(NGFXSHELL SRCS ${NGFX_SHELL_SRCS} LIBS Kaleido3D.Core FOLDER "Runtime" INCLUDES Public ../Core)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/AddNGFXTest.cmake)

add_subdirectory(Test)