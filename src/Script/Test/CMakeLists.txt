set(JSAPP_SRCS
JsApp.h JsApp.cpp
JsAppBinding.h JsAppBinding.cpp
DemoApp.cpp)

source_group(JsApp FILES ${JSAPP_SRCS})

script_add_app(Demo
SRCS ${JSAPP_SRCS})