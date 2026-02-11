#!/bin/bash

origin_dir="$(dirname "$(readlink -f "$0")")"
build_dir=$origin_dir/build

if [ ! -d $build_dir ]; then
	echo "build directory not exists!"
	exit 1
fi

cd $build_dir

echo "generate random data"
./bin/Release/gen_random_data
./bin/Release/gen_vision_data

run_benchmark() {
	local name=$1
	./bin/Release/gbenchmark_$name 2>&1 | tee ../report/benchmark_20260211/benchmark_$name.txt

	echo "sleep for a while, wait CPU temperature drops"
	sleep 15
}

# ============================================================================
# 原始通用测试 (全部开启)
# ============================================================================
run_benchmark easyloggingpp
run_benchmark fmtlog_drop
run_benchmark fmtlog_block
run_benchmark glog
run_benchmark haclog
run_benchmark loguru
# run_benchmark nanolog
run_benchmark quill_bounded_blocking
run_benchmark quill_bounded_dropping
#run_benchmark reckless
run_benchmark spdlog_async
run_benchmark spdlog_sync

# ============================================================================
# 机器视觉场景测试 (全部开启)
# ============================================================================
run_benchmark vision_spdlog
run_benchmark vision_spdlog_async
run_benchmark vision_glog
run_benchmark vision_quill_drop
run_benchmark vision_quill_block
run_benchmark vision_haclog
run_benchmark vision_fmtlog_drop
run_benchmark vision_fmtlog_block
run_benchmark vision_easyloggingpp
run_benchmark vision_loguru
