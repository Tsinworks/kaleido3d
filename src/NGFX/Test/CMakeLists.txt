add_ngfx_test(main SRCS vk_ngfx.cpp)

set(test_gui_srcs base/test_gui.h base/test_gui.cc)
set(test_utils_srcs base/test_utils.h base/test_utils.cc)

add_ngfx_test(device_creation SRCS ngfx_device_creation.cpp)
add_ngfx_test(texture_creation SRCS ngfx_texture_creation.cpp)
add_ngfx_test(render_encoder GUI TRUE SRCS ngfx_render_encoder.cpp ${test_gui_srcs} ${test_utils_srcs})
add_ngfx_test(compute_encoder SRCS ngfx_compute_encoder.cpp ${test_gui_srcs} ${test_utils_srcs})
add_ngfx_test(pipeline_creation SRCS ngfx_pipeline_creation.cpp ${test_gui_srcs} ${test_utils_srcs})
add_ngfx_test(bindgroup_management SRCS ngfx_bindgroup_management.cpp ${test_gui_srcs} ${test_utils_srcs})
add_ngfx_test(buffer_creation SRCS ngfx_buffer_creation.cpp)
add_ngfx_test(raytracing_resource_creation SRCS ngfx_raytracing_resource_creation.cpp)