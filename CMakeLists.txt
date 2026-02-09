cmake_minimum_required (VERSION 3.18.6)
project(log_benchmark)

################################
# general config
################################

message("-- use c compiler ${CMAKE_C_COMPILER}")
message("-- use c++ compiler ${CMAKE_CXX_COMPILER}")

# set compile parameter
if (${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif (${CMAKE_C_COMPILER_ID} MATCHES Clang)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-missing-field-initializers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-missing-field-initializers")
elseif (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 -D_UNICODE -DUNICODE)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/std:c++20>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("-- c compiler support features: ")
foreach(feature ${CMAKE_C_COMPILE_FEATURES})
	message("support c feature: ${feature}")
endforeach()

message("-- c++ compiler support features: ")
foreach(feature ${CMAKE_CXX_COMPILE_FEATURES})
	message("support c++ feature: ${feature}")
endforeach()

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# for vim plugin
if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# set use folder in vs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################
# Local library paths (请修改此处路径！)
################################
# 请将以下路径修改为您电脑上 spdlog、glog 和 benchmark 源码目录的绝对路径

set(DEPS_DIR "${CMAKE_SOURCE_DIR}/_deps")
set(SPDLOG_ROOT "${DEPS_DIR}/spdlog-src" CACHE PATH "Path to local spdlog source")
set(GLOG_ROOT "${DEPS_DIR}/glog-src" CACHE PATH "Path to local glog source")
set(BENCHMARK_ROOT "${DEPS_DIR}/benchmark-src" CACHE PATH "Path to local google benchmark source")

################################
# options
################################

set(benchmark_easyloggingpp OFF CACHE BOOL "generate benchmark: easyloggingpp")
set(benchmark_fmtlog OFF CACHE BOOL "generate benchmark: fmtlog")
set(benchmark_glog ON CACHE BOOL "generate benchmark: glog")
set(benchmark_haclog OFF CACHE BOOL "generate benchmark: haclog")
set(benchmark_loguru OFF CACHE BOOL "generate benchmark: loguru")
set(benchmark_nanolog OFF CACHE BOOL "generate benchmark: nanolog")
set(benchmark_quill OFF CACHE BOOL "generate benchmark: quill")
set(benchmark_reckless OFF CACHE BOOL "generate benchmark: reckless")
set(benchmark_spdlog ON CACHE BOOL "generate benchmark: spdlog")

# 机器视觉生产线场景测试选项
set(benchmark_vision_spdlog ON CACHE BOOL "generate vision benchmark: spdlog")
set(benchmark_vision_glog ON CACHE BOOL "generate vision benchmark: glog")

################################
# log message
################################

FILE(GLOB_RECURSE log_msg_h "${CMAKE_CURRENT_LIST_DIR}/log_msg/*.h")
FILE(GLOB_RECURSE log_msg_cpp "${CMAKE_CURRENT_LIST_DIR}/log_msg/*.cpp")
add_library(log_msg STATIC
	${log_msg_h}
	${log_msg_cpp})
set_target_properties(log_msg PROPERTIES LINKER_LANGUAGE CXX)

FILE(GLOB_RECURSE gen_data_h "${CMAKE_CURRENT_LIST_DIR}/gen_random_data/*.h")
FILE(GLOB_RECURSE gen_data_cpp "${CMAKE_CURRENT_LIST_DIR}/gen_random_data/*.cpp")
add_executable(gen_random_data
	${gen_data_h}
	${gen_data_cpp})
add_dependencies(gen_random_data log_msg)
target_include_directories(gen_random_data PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/log_msg)
target_link_libraries(gen_random_data log_msg)

################################
# vision log data generation
################################

FILE(GLOB_RECURSE vision_log_msg_cpp "${CMAKE_CURRENT_LIST_DIR}/gbenchmark/vision_log_msg.cpp")
add_library(vision_log_msg STATIC
	${CMAKE_CURRENT_LIST_DIR}/gbenchmark/vision_log_msg.h
	${vision_log_msg_cpp})
set_target_properties(vision_log_msg PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(vision_log_msg PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/gbenchmark)

FILE(GLOB_RECURSE gen_vision_data_cpp "${CMAKE_CURRENT_LIST_DIR}/gen_vision_data/*.cpp")
add_executable(gen_vision_data
	${CMAKE_CURRENT_LIST_DIR}/gen_vision_data/gen_vision_data.cpp)
add_dependencies(gen_vision_data vision_log_msg)
target_include_directories(gen_vision_data PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/gbenchmark)
target_link_libraries(gen_vision_data vision_log_msg)

################################
# google benchmark (local source)
################################

message("# Using local google benchmark at ${BENCHMARK_ROOT}")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "")
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
add_subdirectory(${BENCHMARK_ROOT} benchmark EXCLUDE_FROM_ALL)

find_package(Threads)

function(add_gbench name folder)
	message("add gbenchmark ${name} ${folder}")
	set(name gbenchmark_${name})

	file(GLOB_RECURSE tmp_h ${folder}/*.h)
	file(GLOB_RECURSE tmp_c ${folder}/*.c)
	file(GLOB_RECURSE tmp_cpp ${folder}/*.cpp)
	file(GLOB_RECURSE tmp_cc ${folder}/*.cc)

	if (MSVC OR MINGW)
		add_executable(${name} ${tmp_h} ${tmp_c} ${tmp_cpp} ${tmp_cc})
		set_target_properties(${name}
			PROPERTIES
			FOLDER "example"
			VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
		)
	else()
		add_executable(${name} ${tmp_c} ${tmp_cpp} ${tmp_cc})
		if (APPLE)
			set_target_properties(${name}
				PROPERTIES
				INSTALL_RPATH "@executable_path/../lib"
			)
		elseif (UNIX)
			set_target_properties(${name}
				PROPERTIES
				INSTALL_RPATH "\$ORIGIN/../lib"
			)
		endif()
	endif(MSVC OR MINGW)

	add_dependencies(${name} benchmark log_msg)
	target_include_directories(${name} PRIVATE
		${folder}
		${BENCHMARK_ROOT}/include
		${CMAKE_CURRENT_LIST_DIR})
	target_link_libraries(${name} benchmark log_msg)

	# save temp object file, such as: asm code
	if ((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR
		(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
		target_compile_options(${name} PUBLIC -save-temps=obj)
	endif()
endfunction()

################################
# logging libraries (local sources)
################################

if (benchmark_glog)
    message("# Using local glog at ${GLOG_ROOT}")
    set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${GLOG_ROOT} glog EXCLUDE_FROM_ALL)
endif()

if (benchmark_spdlog)
    message("# Using local spdlog at ${SPDLOG_ROOT}")
    add_subdirectory(${SPDLOG_ROOT} spdlog EXCLUDE_FROM_ALL)
endif()

################################
# original gbenchmark tests
################################

set(gbenchmark_dir ${CMAKE_CURRENT_LIST_DIR}/gbenchmark)

if (benchmark_glog)
	add_gbench(glog ${gbenchmark_dir}/cpp/glog)
	add_dependencies(gbenchmark_glog glog)
	target_link_libraries(gbenchmark_glog glog)
endif()

if (benchmark_spdlog)
	add_gbench(spdlog_sync ${gbenchmark_dir}/cpp/spdlog_sync)
	add_dependencies(gbenchmark_spdlog_sync spdlog)
	target_link_libraries(gbenchmark_spdlog_sync spdlog)

	add_gbench(spdlog_async ${gbenchmark_dir}/cpp/spdlog_async)
	add_dependencies(gbenchmark_spdlog_async spdlog)
	target_link_libraries(gbenchmark_spdlog_async spdlog)
endif()

################################
# vision production line scenario tests
################################

if (benchmark_spdlog)
    if (benchmark_vision_spdlog)
        message("# add vision benchmark: spdlog")
        add_gbench(vision_spdlog ${gbenchmark_dir}/cpp/vision_spdlog)
        add_dependencies(gbenchmark_vision_spdlog spdlog)
        add_dependencies(gbenchmark_vision_spdlog vision_log_msg)
        target_link_libraries(gbenchmark_vision_spdlog spdlog vision_log_msg)
    endif()
endif()

if (benchmark_glog)
    if (benchmark_vision_glog)
        message("# add vision benchmark: glog")
        add_gbench(vision_glog ${gbenchmark_dir}/cpp/vision_glog)
        add_dependencies(gbenchmark_vision_glog glog)
        add_dependencies(gbenchmark_vision_glog vision_log_msg)
        target_link_libraries(gbenchmark_vision_glog glog vision_log_msg)
    endif()
endif()

################################
# summary
################################

message("# --------------------------------")
message("# benchmark easyloggingpp: ${benchmark_easyloggingpp}")
message("# benchmark fmtlog: ${benchmark_fmtlog}")
message("# benchmark glog: ${benchmark_glog}")
message("# benchmark haclog: ${benchmark_haclog}")
message("# benchmark loguru: ${benchmark_loguru}")
message("# benchmark nanolog: ${benchmark_nanolog}")
message("# benchmark reckless: ${benchmark_reckless}")
message("# benchmark spdlog: ${benchmark_spdlog}")
message("# --------------------------------")
message("# vision benchmark spdlog: ${benchmark_vision_spdlog}")
message("# vision benchmark glog: ${benchmark_vision_glog}")
message("# --------------------------------")
