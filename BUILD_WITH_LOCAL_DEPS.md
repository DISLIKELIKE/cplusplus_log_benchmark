# 使用本地源码构建指南

本说明文档介绍如何使用本地下载的源码来构建机器视觉生产线日志场景测试。

## 前提条件

1. 已下载以下源码到本地:
   - spdlog: https://github.com/gabime/spdlog
   - glog: https://github.com/google/glog
   - benchmark: https://github.com/google/benchmark

2. 已有 CMake (版本 >= 3.18.6) 和 C++17 兼容的编译器

## 目录结构

```
log_benchmark/
├── _deps/                          # 依赖库源码目录 (需要创建)
│   ├── spdlog-src/                # spdlog 源码
│   ├── glog-src/                  # glog 源码
│   └── benchmark-src/             # Google Benchmark 源码
├── gbenchmark/                     # Benchmark 测试代码
│   ├── vision_log_msg.h          # 视觉日志消息定义
│   ├── vision_log_msg.cpp        # 数据生成实现
│   ├── vision_gbenchmark.h       # Benchmark 框架
│   └── cpp/
│       ├── vision_spdlog/        # spdlog 视觉场景测试
│       └── vision_glog/         # glog 视觉场景测试
├── gen_vision_data/               # 数据生成工具
│   └── gen_vision_data.cpp
├── log_msg/                      # 原始日志消息 (兼容原有测试)
└── CMakeLists.txt                # CMake 配置文件
```

## 步骤 1: 准备源码

### 1.1 创建依赖目录

```bash
cd log_benchmark
mkdir -p _deps
```

### 1.2 下载源码

```bash
# 下载 spdlog
cd _deps
git clone https://github.com/gabime/spdlog.git spdlog-src
cd spdlog-src
git checkout v1.12.0
cd ..

# 下载 glog
git clone https://github.com/google/glog.git glog-src
cd glog-src
git checkout v0.6.0
cd ..

# 下载 Google Benchmark
git clone https://github.com/google/benchmark.git benchmark-src
cd benchmark-src
git checkout v1.8.3
cd ..
```

## 步骤 2: 配置路径

编辑 `CMakeLists.txt`,确保以下路径配置正确:

```cmake
set(DEPS_DIR "${CMAKE_SOURCE_DIR}/_deps")
set(SPDLOG_ROOT "${DEPS_DIR}/spdlog-src" CACHE PATH "Path to local spdlog source")
set(GLOG_ROOT "${DEPS_DIR}/glog-src" CACHE PATH "Path to local glog source")
set(BENCHMARK_ROOT "${DEPS_DIR}/benchmark-src" CACHE PATH "Path to local google benchmark source")
```

**注意**: 如果您的源码在其他位置,请修改上述路径为绝对路径。

## 步骤 3: 构建项目

### 3.1 创建构建目录

```bash
cd log_benchmark
mkdir build
cd build
```

### 3.2 配置 CMake

```bash
cmake ..
```

您应该看到类似的输出:

```
-- Using local google benchmark at ../_deps/benchmark-src
-- Using local glog at ../_deps/glog-src
-- Using local spdlog at ../_deps/spdlog-src
-- add gbenchmark glog ../gbenchmark/cpp/glog
-- add gbenchmark spdlog_sync ../gbenchmark/cpp/spdlog_sync
-- add gbenchmark spdlog_async ../gbenchmark/cpp/spdlog_async
-- add vision benchmark: spdlog
-- add gbenchmark vision_spdlog ../gbenchmark/cpp/vision_spdlog
-- add vision benchmark: glog
-- add gbenchmark vision_glog ../gbenchmark/cpp/vision_glog
```

### 3.3 编译

```bash
# Linux/macOS
make -j$(nproc)

# Windows (使用 Visual Studio)
cmake --build . --config Release
```

### 3.4 验证编译结果

检查 `build/bin/` 目录,应该包含以下可执行文件:

```
bin/
├── gen_vision_data          # 生成测试数据工具
├── gbenchmark_spdlog_sync   # 原始 spdlog 同步测试
├── gbenchmark_spdlog_async  # 原始 spdlog 异步测试
├── gbenchmark_glog          # 原始 glog 测试
├── gbenchmark_vision_spdlog # 视觉场景 spdlog 测试
└── gbenchmark_vision_glog   # 视觉场景 glog 测试
```

## 步骤 4: 生成测试数据

### 4.1 创建 data 目录

```bash
cd log_benchmark
mkdir -p data
```

### 4.2 运行数据生成工具

```bash
cd build
./bin/gen_vision_data
```

预期输出:

```
Generating vision production line log test data...
Generated 10000 image logs
Generated 10000 detection logs
Generated 5000 device logs
Generated 1000 alarm logs
Generated 500 production logs

All test data generated successfully in 'data' directory.

You can now run the benchmarks:
  - build/bin/gbenchmark_vision_spdlog
  - build/bin/gbenchmark_vision_glog
```

检查 `data/` 目录,应该包含:

```
data/
├── vision_image_logs.bin
├── vision_detection_logs.bin
├── vision_device_logs.bin
├── vision_alarm_logs.bin
└── vision_production_logs.bin
```

## 步骤 5: 运行 Benchmark

### 5.1 运行视觉场景测试

```bash
# 创建日志输出目录
mkdir -p logs

# 运行 spdlog 视觉场景测试
./bin/gbenchmark_vision_spdlog

# 运行 glog 视觉场景测试
./bin/gbenchmark_vision_glog
```

### 5.2 保存结果

```bash
# 保存 spdlog 结果
./bin/gbenchmark_vision_spdlog > results_vision_spdlog.txt 2>&1

# 保存 glog 结果
./bin/gbenchmark_vision_glog > results_vision_glog.txt 2>&1
```

### 5.3 运行原始测试(可选)

如果需要运行原始的通用测试:

```bash
# 原始 spdlog 测试
./bin/gbenchmark_spdlog_sync
./bin/gbenchmark_spdlog_async

# 原始 glog 测试
./bin/gbenchmark_glog
```

## 自定义选项

可以通过 CMake 选项控制构建哪些测试:

```bash
# 只构建 spdlog 相关测试
cmake -Dbenchmark_glog=OFF -Dbenchmark_spdlog=ON ..

# 只构建 glog 相关测试
cmake -Dbenchmark_glog=ON -Dbenchmark_spdlog=OFF ..

# 只构建视觉场景测试
cmake -Dbenchmark_vision_spdlog=ON -Dbenchmark_vision_glog=ON ..
```

## 常见问题

### Q1: 找不到依赖库源码

**错误信息**:
```
CMake Error: Could not find ...
```

**解决方案**:
检查 `_deps/` 目录下是否正确下载了源码,并确认 `CMakeLists.txt` 中的路径配置正确。

### Q2: 编译错误 - 找不到头文件

**错误信息**:
```
fatal error: spdlog/spdlog.h: No such file or directory
```

**解决方案**:
1. 确认 spdlog 源码已正确下载
2. 检查 CMake 配置时的路径输出是否正确
3. 清理构建目录后重新配置: `rm -rf build && mkdir build && cd build && cmake ..`

### Q3: glog 编译失败

**错误信息**:
```
CMake Error at glog-src/CMakeLists.txt:...
```

**解决方案**:
确保 glog 版本兼容,推荐使用 v0.6.0 版本。

### Q4: 运行测试时找不到数据文件

**错误信息**:
```
Failed to load image logs
```

**解决方案**:
1. 确保已运行 `gen_vision_data` 生成测试数据
2. 确认 `data/` 目录在正确的位置
3. 检查数据文件是否存在: `ls -la data/`

### Q5: 性能差异很大

**可能原因**:
- 不同的磁盘 IO 性能 (建议使用 SSD)
- CPU 频率动态调整 (建议固定 CPU 频率)
- 系统负载差异 (建议在低负载环境下测试)

## 性能对比分析

运行完测试后,可以通过以下方式对比性能:

### 1. 直接对比输出

```
# 查看关键指标
grep "image_acquisition/8" results_vision_spdlog.txt
grep "image_acquisition/8" results_vision_glog.txt
```

### 2. 提取关键指标

创建一个简单的脚本来提取关键指标:

```python
#!/usr/bin/env python3
import re
import sys

def extract_benchmark_time(filename, pattern):
    with open(filename, 'r') as f:
        content = f.read()
        match = re.search(pattern, content)
        if match:
            return match.group(1)
    return "N/A"

# 提取图像采集场景(8线程)的性能
pattern = r'image_acquisition/8\s+(\d+)\s+ns'
spdlog_time = extract_benchmark_time('results_vision_spdlog.txt', pattern)
glog_time = extract_benchmark_time('results_vision_glog.txt', pattern)

print(f"Image Acquisition (8 threads):")
print(f"  spdlog: {spdlog_time} ns")
print(f"  glog:   {glog_time} ns")
if spdlog_time != "N/A" and glog_time != "N/A":
    ratio = float(glog_time) / float(spdlog_time)
    print(f"  ratio:  {ratio:.2f}x")
```

## 下一步

完成基准测试后,您可以根据结果:

1. **选择合适的日志库**: 根据实际业务场景的性能表现选择
2. **优化日志配置**: 调整缓冲区大小、线程数等参数
3. **进行更深入的测试**: 在实际生产环境中进行长时间稳定性测试

## 参考资料

- [Google Benchmark 文档](https://github.com/google/benchmark)
- [spdlog 文档](https://github.com/gabime/spdlog)
- [glog 文档](https://github.com/google/glog)
- [视觉场景测试说明](./gbenchmark/VISION_SCENARIO_README.md)
