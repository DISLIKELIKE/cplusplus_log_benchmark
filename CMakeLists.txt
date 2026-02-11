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
# 请将以下路径修改为您电脑上日志库源码目录的绝对路径
# 如果某个库不存在，可以将对应选项设置为 OFF

set(DEPS_DIR "${CMAKE_SOURCE_DIR}/_deps")
set(SPDLOG_ROOT "${DEPS_DIR}/spdlog-src" CACHE PATH "Path to local spdlog source")
set(GLOG_ROOT "${DEPS_DIR}/glog-src" CACHE PATH "Path to local glog source")
set(BENCHMARK_ROOT "${DEPS_DIR}/benchmark-src" CACHE PATH "Path to local google benchmark source")
set(FMTLOG_ROOT "${DEPS_DIR}/fmtlog-src" CACHE PATH "Path to local fmtlog source")
set(QUILL_ROOT "${DEPS_DIR}/quill-src" CACHE PATH "Path to local quill source")
set(HACLOG_ROOT "${DEPS_DIR}/haclog-src" CACHE PATH "Path to local haclog source")
set(EASYLOGGINGPP_ROOT "${DEPS_DIR}/easyloggingpp-src" CACHE PATH "Path to local easyloggingpp source")
set(LOGURU_ROOT "${DEPS_DIR}/loguru-src" CACHE PATH "Path to local loguru source")

################################
# options
################################

# 原始通用测试选项 (全部开启)
set(benchmark_easyloggingpp ON CACHE BOOL "generate benchmark: easyloggingpp")
set(benchmark_fmtlog ON CACHE BOOL "generate benchmark: fmtlog")
set(benchmark_glog ON CACHE BOOL "generate benchmark: glog")
set(benchmark_haclog ON CACHE BOOL "generate benchmark: haclog")
set(benchmark_loguru ON CACHE BOOL "generate benchmark: loguru")
set(benchmark_nanolog OFF CACHE BOOL "generate benchmark: nanolog")
set(benchmark_quill ON CACHE BOOL "generate benchmark: quill")
set(benchmark_reckless OFF CACHE BOOL "generate benchmark: reckless")
set(benchmark_spdlog ON CACHE BOOL "generate benchmark: spdlog")

# 机器视觉生产线场景测试选项 (全部开启)
set(benchmark_vision_spdlog ON CACHE BOOL "generate vision benchmark: spdlog")
set(benchmark_vision_spdlog_async ON CACHE BOOL "generate vision benchmark: spdlog async")
set(benchmark_vision_glog ON CACHE BOOL "generate vision benchmark: glog")
set(benchmark_vision_quill_drop ON CACHE BOOL "generate vision benchmark: quill drop")
set(benchmark_vision_quill_block ON CACHE BOOL "generate vision benchmark: quill block")
set(benchmark_vision_haclog ON CACHE BOOL "generate vision benchmark: haclog")
set(benchmark_vision_fmtlog_drop ON CACHE BOOL "generate vision benchmark: fmtlog drop")
set(benchmark_vision_fmtlog_block ON CACHE BOOL "generate vision benchmark: fmtlog block")
set(benchmark_vision_easyloggingpp ON CACHE BOOL "generate vision benchmark: easyloggingpp")
set(benchmark_vision_loguru ON CACHE BOOL "generate vision benchmark: loguru")

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

function(add_gbench_with_lib name folder lib_root)
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
		${lib_root}
		${CMAKE_CURRENT_LIST_DIR})
	target_link_libraries(${name} benchmark log_msg)

	# save temp object file, such as: asm code
	if ((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR
		(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
		target_compile_options(${name} PUBLIC -save-temps=obj)
	endif()
endfunction()

################################
# original gbenchmark tests (local sources)
################################

# glog
if (benchmark_glog OR benchmark_vision_glog)
    message("# Using local glog at ${GLOG_ROOT}")
    set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${GLOG_ROOT} glog EXCLUDE_FROM_ALL)
endif()

# spdlog
if (benchmark_spdlog OR benchmark_vision_spdlog OR benchmark_vision_spdlog_async)
    message("# Using local spdlog at ${SPDLOG_ROOT}")
    add_subdirectory(${SPDLOG_ROOT} spdlog EXCLUDE_FROM_ALL)
endif()

# fmtlog
if (benchmark_fmtlog OR benchmark_vision_fmtlog_drop OR benchmark_vision_fmtlog_block)
    message("# Using local fmtlog at ${FMTLOG_ROOT}")
    add_subdirectory(${FMTLOG_ROOT} fmtlog EXCLUDE_FROM_ALL)
endif()

# quill
if (benchmark_quill OR benchmark_vision_quill_drop OR benchmark_vision_quill_block)
    message("# Using local quill at ${QUILL_ROOT}")
    add_subdirectory(${QUILL_ROOT} quill EXCLUDE_FROM_ALL)
endif()

# haclog
if (benchmark_haclog OR benchmark_vision_haclog)
    message("# Using local haclog ")
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/_deps/haclog-src haclog EXCLUDE_FROM_ALL)
endif()

# easyloggingpp
if (benchmark_easyloggingpp OR benchmark_vision_easyloggingpp)
    message("# Using local easyloggingpp at ${EASYLOGGINGPP_ROOT}")
    add_subdirectory(${EASYLOGGINGPP_ROOT} easyloggingpp EXCLUDE_FROM_ALL)
endif()

# loguru
if (benchmark_loguru OR benchmark_vision_loguru)
    message("# Using local loguru at ${LOGURU_ROOT}")
    add_subdirectory(${LOGURU_ROOT} loguru EXCLUDE_FROM_ALL)
endif()

################################
# original gbenchmark tests
################################

set(gbenchmark_dir ${CMAKE_CURRENT_LIST_DIR}/gbenchmark)

# easyloggingpp
if (benchmark_easyloggingpp)
    add_gbench(easyloggingpp ${gbenchmark_dir}/cpp/easyloggingpp)
    add_dependencies(gbenchmark_easyloggingpp easyloggingpp)
    target_link_libraries(gbenchmark_easyloggingpp easyloggingpp)
    target_include_directories(gbenchmark_easyloggingpp PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/_deps/easyloggingpp-src/src)
endif()

# fmtlog
if (benchmark_fmtlog)
    add_gbench(fmtlog_drop ${gbenchmark_dir}/cpp/fmtlog_drop)
    add_dependencies(gbenchmark_fmtlog_drop fmtlog-static fmt)
    target_link_libraries(gbenchmark_fmtlog_drop fmtlog-static fmt)
    target_include_directories(gbenchmark_fmtlog_drop PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/_deps/fmtlog-src/
        ${CMAKE_CURRENT_LIST_DIR}/_deps/fmtlog-src/fmt/include
    )

    add_gbench(fmtlog_block ${gbenchmark_dir}/cpp/fmtlog_block)
    add_dependencies(gbenchmark_fmtlog_block fmtlog-static fmt)
    target_link_libraries(gbenchmark_fmtlog_block fmtlog-static fmt)
    target_include_directories(gbenchmark_fmtlog_block PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/_deps/fmtlog-src/
        ${CMAKE_CURRENT_LIST_DIR}/_deps/fmtlog-src/fmt/include
    )
endif()

# glog
if (benchmark_glog)
    add_gbench_with_lib(glog ${gbenchmark_dir}/cpp/glog ${GLOG_ROOT})
    target_link_libraries(gbenchmark_glog glog)
endif()

# haclog
if (benchmark_haclog)
    add_gbench(haclog ${gbenchmark_dir}/c/haclog)
     
	add_dependencies(gbenchmark_haclog haclog)
    target_link_libraries(gbenchmark_haclog haclog)
endif()

# loguru
if (benchmark_loguru)
    add_gbench_with_lib(loguru ${gbenchmark_dir}/cpp/loguru ${LOGURU_ROOT})
    target_link_libraries(gbenchmark_loguru loguru)
endif()

# quill
if (benchmark_quill)
    add_gbench_with_lib(quill_bounded_dropping ${gbenchmark_dir}/cpp/quill_bounded_dropping ${QUILL_ROOT})
    target_link_libraries(gbenchmark_quill_bounded_dropping quill)

    add_gbench_with_lib(quill_bounded_blocking ${gbenchmark_dir}/cpp/quill_bounded_blocking ${QUILL_ROOT})
    target_link_libraries(gbenchmark_quill_bounded_blocking quill)
endif()

# spdlog
if (benchmark_spdlog)
    add_gbench_with_lib(spdlog_sync ${gbenchmark_dir}/cpp/spdlog_sync ${SPDLOG_ROOT})
    target_link_libraries(gbenchmark_spdlog_sync spdlog)

    add_gbench_with_lib(spdlog_async ${gbenchmark_dir}/cpp/spdlog_async ${SPDLOG_ROOT})
    target_link_libraries(gbenchmark_spdlog_async spdlog)
endif()

################################
# vision production line scenario tests
################################

# spdlog (sync)
if (benchmark_spdlog)
    if (benchmark_vision_spdlog)
        message("# add vision benchmark: spdlog")
        add_gbench(vision_spdlog ${gbenchmark_dir}/cpp/vision_spdlog)
        target_link_libraries(gbenchmark_vision_spdlog spdlog vision_log_msg)
    endif()
endif()

# spdlog (async)
if (benchmark_spdlog)
    if (benchmark_vision_spdlog_async)
        message("# add vision benchmark: spdlog async")
        add_gbench(vision_spdlog_async ${gbenchmark_dir}/cpp/vision_spdlog_async)
        target_link_libraries(gbenchmark_vision_spdlog_async spdlog vision_log_msg)
    endif()
endif()

# glog
if (benchmark_glog)
    if (benchmark_vision_glog)
        message("# add vision benchmark: glog")
        add_gbench(vision_glog ${gbenchmark_dir}/cpp/vision_glog)
        target_link_libraries(gbenchmark_vision_glog glog vision_log_msg)
    endif()
endif()

# quill (drop)
if (benchmark_quill)
    if (benchmark_vision_quill_drop)
        message("# add vision benchmark: quill drop")
        add_gbench(vision_quill_drop ${gbenchmark_dir}/cpp/vision_quill_drop)
        target_link_libraries(gbenchmark_vision_quill_drop quill vision_log_msg)
    endif()
endif()

# quill (block)
if (benchmark_quill)
    if (benchmark_vision_quill_block)
        message("# add vision benchmark: quill block")
        add_gbench(vision_quill_block ${gbenchmark_dir}/cpp/vision_quill_block)
        target_link_libraries(gbenchmark_vision_quill_block quill vision_log_msg)
    endif()
endif()

# haclog
if (benchmark_haclog)
    if (benchmark_vision_haclog)
        message("# add vision benchmark: haclog")
        add_gbench(vision_haclog ${gbenchmark_dir}/cpp/vision_haclog)
        target_link_libraries(gbenchmark_vision_haclog haclog vision_log_msg)
    endif()
endif()

# fmtlog (drop)
if (benchmark_fmtlog)
    if (benchmark_vision_fmtlog_drop)
        message("# add vision benchmark: fmtlog drop")
        add_gbench(vision_fmtlog_drop ${gbenchmark_dir}/cpp/vision_fmtlog_drop)
        target_link_libraries(gbenchmark_vision_fmtlog_drop fmtlog vision_log_msg)
    endif()
endif()

# fmtlog (block)
if (benchmark_fmtlog)
    if (benchmark_vision_fmtlog_block)
        message("# add vision benchmark: fmtlog block")
        add_gbench(vision_fmtlog_block ${gbenchmark_dir}/cpp/vision_fmtlog_block)
        target_link_libraries(gbenchmark_vision_fmtlog_block fmtlog vision_log_msg)
    endif()
endif()

# easyloggingpp
if (benchmark_easyloggingpp)
    if (benchmark_vision_easyloggingpp)
        message("# add vision benchmark: easyloggingpp")
        add_gbench(vision_easyloggingpp ${gbenchmark_dir}/cpp/vision_easyloggingpp)
        target_link_libraries(gbenchmark_vision_easyloggingpp easyloggingpp vision_log_msg)
    endif()
endif()

# loguru
if (benchmark_loguru)
    if (benchmark_vision_loguru)
        message("# add vision benchmark: loguru")
        add_gbench(vision_loguru ${gbenchmark_dir}/cpp/vision_loguru)
        target_link_libraries(gbenchmark_vision_loguru loguru vision_log_msg)
    endif()
endif()

################################
# summary
################################

message("# ========================================")
message("# Original General Benchmark Tests")
message("# ========================================")
message("# benchmark easyloggingpp: ${benchmark_easyloggingpp}")
message("# benchmark fmtlog:        ${benchmark_fmtlog}")
message("# benchmark glog:         ${benchmark_glog}")
message("# benchmark haclog:       ${benchmark_haclog}")
message("# benchmark loguru:       ${benchmark_loguru}")
message("# benchmark nanolog:      ${benchmark_nanolog}")
message("# benchmark reckless:      ${benchmark_reckless}")
message("# benchmark spdlog:       ${benchmark_spdlog}")
message("# ========================================")
message("# Vision Production Line Benchmark Tests")
message("# ========================================")
message("# vision spdlog (sync):     ${benchmark_vision_spdlog}")
message("# vision spdlog (async):    ${benchmark_vision_spdlog_async}")
message("# vision glog:              ${benchmark_vision_glog}")
message("# vision quill (drop):      ${benchmark_vision_quill_drop}")
message("# vision quill (block):     ${benchmark_vision_quill_block}")
message("# vision haclog:            ${benchmark_vision_haclog}")
message("# vision fmtlog (drop):     ${benchmark_vision_fmtlog_drop}")
message("# vision fmtlog (block):    ${benchmark_vision_fmtlog_block}")
message("# vision easyloggingpp:     ${benchmark_vision_easyloggingpp}")
message("# vision loguru:            ${benchmark_vision_loguru}")
message("# ========================================")
