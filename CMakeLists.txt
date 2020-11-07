cmake_minimum_required(VERSION 3.0)

project(lite-onnx C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Export a JSON file with the compilation commands that external tools can use
# to analyze the source code of the project.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LITEONNX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LITEONNX_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${LITEONNX_SOURCE_DIR})
include_directories(${lITEONNX_SOURCE_DIR}/proto)
include_directories(${LITEONNX_BINARY_DIR})

#find_package(Git)
# Get the commit's short SHA1
#execute_process(COMMAND
#  "${GIT_EXECUTABLE}" log -1 --pretty=format:"%h"
#  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
#  OUTPUT_VARIABLE GIT_SHA1
#  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
#add_definitions("-DGIT_SHA1=${GIT_SHA1}")
# Get the date of the commit
#execute_process(COMMAND
#  "${GIT_EXECUTABLE}" log -1 --pretty=format:"%ad" --date=short
#  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
#  OUTPUT_VARIABLE GIT_DATE
#  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
#add_definitions("-DGIT_DATE=${GIT_DATE}")

add_subdirectory(${PROJECT_SOURCE_DIR}/protobuf/cmake)
set_target_properties(libprotoc PROPERTIES FOLDER "Protobuf")
add_executable(protobuf::protoc ALIAS protoc)
set_target_properties(protoc PROPERTIES FOLDER "Protobuf")
add_library(protobuf::libprotobuf ALIAS libprotobuf)
set(PROTOBUF_LIB libprotobuf)

include(protobuf_function.cmake)

# Generate .h and .cc files from protobuf file
add_library(onnx_proto proto/onnx.proto proto/onnx-operators.proto)
target_include_directories(onnx_proto PUBLIC $<TARGET_PROPERTY:protobuf::libprotobuf,INTERFACE_INCLUDE_DIRECTORIES> ${LITEONNX_BINARY_DIR})
target_compile_definitions(onnx_proto PUBLIC $<TARGET_PROPERTY:protobuf::libprotobuf,INTERFACE_COMPILE_DEFINITIONS>)
onnxruntime_protobuf_generate(APPEND_PATH ${PROJECT_SOURCE_DIR}/proto TARGET onnx_proto)
if(HAS_UNUSED_PARAMETER)
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/onnx.pb.cc PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/onnx-operators.pb.cc PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
endif()

add_definitions(-DONNX_NAMESPACE=onnx)
AUX_SOURCE_DIRECTORY(onnx onnx_SRC)
AUX_SOURCE_DIRECTORY(defs defs_SRC)
AUX_SOURCE_DIRECTORY(shape_inference shape_inference_SRC)

add_library(onnx_lite ${onnx_SRC} ${defs_SRC} ${shape_inference_SRC})
target_link_libraries(onnx_lite PRIVATE onnx_proto)

add_custom_target(dependency_graph
                  "${CMAKE_COMMAND}" "--graphviz=dependency_graph" .
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

