set (NGFX_SRCS
Private/Vulkan/vk_commands.cpp
Private/Vulkan/vk_log.cpp
Private/Vulkan/vk_low_level_mem.cpp
Private/Vulkan/vk_ray_tracing.cpp
Private/Vulkan/vk_resource.cpp
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

k3d_add_lib(NGFX SRCS ${NGFX_SRCS} LIBS Kaleido3D.Core FOLDER "Runtime" INCLUDES Public ../Core)

set(NGFX_SHELL_SRCS
Public/ngfx_shell.h
Private/ngfx_shell.cpp)
source_group(sh FILES ${NGFX_SHELL_SRCS})
k3d_add_lib(NGFXSHELL SRCS ${NGFX_SHELL_SRCS} LIBS Kaleido3D.Core FOLDER "Runtime" INCLUDES Public ../Core)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/AddNGFXTest.cmake)

add_subdirectory(Test)