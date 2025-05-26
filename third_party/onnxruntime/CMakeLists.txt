
set(SRCS
    "test.cpp"
)
add_library(onnxruntime ${SRCS})
target_include_directories(onnxruntime PUBLIC "onnxruntime/include")

target_link_directories(onnxruntime PUBLIC "onnxruntime/lib")
target_link_libraries(onnxruntime PUBLIC onnxruntime.lib onnxruntime_providers_shared.lib)
