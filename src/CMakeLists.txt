cmake_minimum_required(VERSION 3.25)
project(Sidekick VERSION 0.1.0)
set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sidekick)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE src_list LIST_DIRECTORIES false RELATIVE 
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c" 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.rc"
)

add_executable(${PROJECT_NAME} WIN32 ${src_list} ${hdr_list})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_list})

target_include_directories(${PROJECT_NAME} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-2.13.3/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}../../bin"
)