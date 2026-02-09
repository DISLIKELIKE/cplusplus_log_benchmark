# 完整运行流程指南

本文档详细说明了从零开始到完成测试的完整流程。

## 目录结构总览

```
log_benchmark/                    # 项目根目录
├── _deps/                        # 第1步: 需要创建,存放依赖源码
│   ├── spdlog-src/              # 第1步: 下载 spdlog 源码
│   ├── glog-src/                # 第1步: 下载 glog 源码
│   └── benchmark-src/           # 第1步: 下载 Google Benchmark 源码
├── build/                        # 第3步: 创建的构建目录
│   ├── bin/                     # 第4步: 编译后的可执行文件
│   │   ├── gen_vision_data      # 生成测试数据工具
│   │   ├── gbenchmark_vision_spdlog  # spdlog 视觉场景测试
│   │   └── gbenchmark_vision_glog    # glog 视觉场景测试
│   └── lib/                     # 编译的库文件
├── data/                         # 第5步: 存放生成的测试数据
│   ├── vision_image_logs.bin
│   ├── vision_detection_logs.bin
│   ├── vision_device_logs.bin
│   ├── vision_alarm_logs.bin
│   └── vision_production_logs.bin
├── logs/                         # 第6步: 测试时生成的日志文件
│   ├── vision_spdlog.log
│   └── glog/                    # glog 日志目录
│       └── vision_glog.*.log
└── results/                      # 第8步: 保存测试结果
    ├── results_vision_spdlog.txt
    └── results_vision_glog.txt
```

## 完整流程图

```
第1步: 准备依赖源码
    ↓
    git clone spdlog → _deps/spdlog-src/
    git clone glog → _deps/glog-src/
    git clone benchmark → _deps/benchmark-src/
    ↓
第2步: 验证目录结构
    ↓
    检查 _deps/ 目录下是否有三个源码目录
    ↓
第3步: 配置和编译
    ↓
    mkdir build && cd build
    cmake ..
    make -j$(nproc)
    ↓
第4步: 验证编译结果
    ↓
    检查 build/bin/ 目录下是否有可执行文件
    ↓
第5步: 生成测试数据
    ↓
    mkdir -p data
    ./bin/gen_vision_data
    ↓
    验证 data/ 目录下是否生成了 5 个 .bin 文件
    ↓
第6步: 准备运行环境
    ↓
    mkdir -p logs
    ↓
第7步: 运行 Benchmark 测试
    ↓
    ./bin/gbenchmark_vision_spdlog  → 输出到 logs/vision_spdlog.log
    ./bin/gbenchmark_vision_glog    → 输出到 logs/glog/vision_glog.*.log
    ↓
第8步: 保存测试结果
    ↓
    ./bin/gbenchmark_vision_spdlog > ../results/results_vision_spdlog.txt 2>&1
    ./bin/gbenchmark_vision_glog > ../results/results_vision_glog.txt 2>&1
    ↓
第9步: 分析结果
    ↓
    查看 results/ 目录下的结果文件
    对比性能数据
```

## 详细步骤说明

### 第1步: 准备依赖源码

**目的**: 下载测试所需的所有第三方库源码

**操作**:
```bash
# 进入项目根目录
cd /path/to/log_benchmark

# 创建依赖目录
mkdir -p _deps
cd _deps

# 下载 spdlog (版本 v1.12.0)
git clone https://github.com/gabime/spdlog.git spdlog-src
cd spdlog-src
git checkout v1.12.0
cd ..

# 下载 glog (版本 v0.6.0)
git clone https://github.com/google/glog.git glog-src
cd glog-src
git checkout v0.6.0
cd ..

# 下载 Google Benchmark (版本 v1.8.3)
git clone https://github.com/google/benchmark.git benchmark-src
cd benchmark-src
git checkout v1.8.3
cd ..
```

**验证**:
```bash
cd /path/to/log_benchmark
ls -la _deps/
# 应该看到:
# spdlog-src/
# glog-src/
# benchmark-src/
```

---

### 第2步: 验证目录结构

**目的**: 确保所有源文件都在正确的位置

**操作**:
```bash
# 检查关键文件是否存在
cd /path/to/log_benchmark
ls CMakeLists.txt
ls gbenchmark/vision_log_msg.h
ls gbenchmark/cpp/vision_spdlog/
ls gbenchmark/cpp/vision_glog/
ls gen_vision_data/gen_vision_data.cpp
```

**预期输出**: 所有命令都应该显示相应的文件或目录

---

### 第3步: 配置和编译

**目的**: 使用 CMake 配置项目并编译

**操作**:
```bash
# 进入项目根目录
cd /path/to/log_benchmark

# 创建构建目录
mkdir build
cd build

# 运行 CMake 配置
cmake ..
```

**预期 CMake 输出**:
```
-- use c compiler /usr/bin/cc
-- use c++ compiler /usr/bin/c++
-- c compiler support features:
support c feature: c_std_99
...
-- c++ compiler support features:
support c++ feature: cxx_std_17
...
# Using local google benchmark at ../_deps/benchmark-src
# Using local glog at ../_deps/glog-src
# Using local spdlog at ../_deps/spdlog-src
# add gbenchmark glog ../gbenchmark/cpp/glog
# add gbenchmark spdlog_sync ../gbenchmark/cpp/spdlog_sync
# add gbenchmark spdlog_async ../gbenchmark/cpp/spdlog_async
# add vision benchmark: spdlog
# add gbenchmark vision_spdlog ../gbenchmark/cpp/vision_spdlog
# add vision benchmark: glog
# add gbenchmark vision_glog ../gbenchmark/cpp/vision_glog
# --------------------------------
# benchmark easyloggingpp: OFF
# benchmark fmtlog: OFF
# benchmark glog: ON
# benchmark haclog: OFF
# benchmark loguru: OFF
# benchmark nanolog: OFF
# benchmark quill: OFF
# benchmark reckless: OFF
# benchmark spdlog: ON
# --------------------------------
# vision benchmark spdlog: ON
# vision benchmark glog: ON
# --------------------------------
-- Configuring done
-- Generating done
```

**编译**:
```bash
# 使用多线程编译(加快速度)
make -j$(nproc)
```

**预期编译输出**:
```
[  1%] Building CXX object CMakeFiles/log_msg.dir/log_msg/log_msg.cpp.o
[  2%] Linking CXX static library liblog_msg.a
...
[ 15%] Building CXX object CMakeFiles/gen_vision_data.dir/gen_vision_data/gen_vision_data.cpp.o
[ 16%] Linking CXX executable gen_vision_data
...
[ 80%] Building CXX object gbenchmark_vision_spdlog.dir/gbenchmark/cpp/vision_spdlog/vision_spdlog_benchmark.cpp.o
[ 81%] Linking CXX executable gbenchmark_vision_spdlog
...
[ 90%] Building CXX object gbenchmark_vision_glog.dir/gbenchmark/cpp/vision_glog/vision_glog_benchmark.cpp.o
[ 91%] Linking CXX executable gbenchmark_vision_glog
...
[100%] Built target gbenchmark_vision_glog
```

---

### 第4步: 验证编译结果

**目的**: 确保所有可执行文件都成功编译

**操作**:
```bash
cd /path/to/log_benchmark/build

# 查看编译生成的可执行文件
ls -lh bin/
```

**预期输出**:
```
-rwxr-xr-x 1 user user 2.5M Jan 15 10:30 gen_vision_data
-rwxr-xr-x 1 user user 3.2M Jan 15 10:31 gbenchmark_vision_spdlog
-rwxr-xr-x 1 user user 3.1M Jan 15 10:31 gbenchmark_vision_glog
-rwxr-xr-x 1 user user 2.8M Jan 15 10:28 gbenchmark_spdlog_async
-rwxr-xr-x 1 user user 2.7M Jan 15 10:28 gbenchmark_spdlog_sync
-rwxr-xr-x 1 user user 2.9M Jan 15 10:27 gbenchmark_glog
```

**说明**:
- `gen_vision_data` - 生成测试数据的工具
- `gbenchmark_vision_spdlog` - 视觉场景 spdlog 测试
- `gbenchmark_vision_glog` - 视觉场景 glog 测试
- 其他三个是原始的通用测试(可选)

---

### 第5步: 生成测试数据

**目的**: 生成所有视觉场景所需的测试数据

**操作**:
```bash
# 创建数据目录
cd /path/to/log_benchmark
mkdir -p data

# 运行数据生成工具
cd build
./bin/gen_vision_data
```

**预期输出**:
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

**验证**:
```bash
cd /path/to/log_benchmark
ls -lh data/
```

**预期输出**:
```
-rw-r--r-- 1 user user 1.8M Jan 15 10:32 vision_alarm_logs.bin
-rw-r--r-- 1 user user  28M Jan 15 10:32 vision_detection_logs.bin
-rw-r--r-- 1 user user  14M Jan 15 10:32 vision_device_logs.bin
-rw-r--r-- 1 user user  21M Jan 15 10:32 vision_image_logs.bin
-rw-r--r-- 1 user user 3.5M Jan 15 10:32 vision_production_logs.bin
```

**说明**:
- `vision_image_logs.bin` - 10000 条图像采集日志
- `vision_detection_logs.bin` - 10000 条检测结果日志
- `vision_device_logs.bin` - 5000 条设备状态日志
- `vision_alarm_logs.bin` - 1000 条异常告警日志
- `vision_production_logs.bin` - 500 条生产统计日志

---

### 第6步: 准备运行环境

**目的**: 创建日志输出目录

**操作**:
```bash
# 创建日志目录
cd /path/to/log_benchmark
mkdir -p logs

# 验证目录创建成功
ls -la logs/
```

**预期输出**:
```
total 0
drwxr-xr-x 2 user user 4096 Jan 15 10:33 .
drwxr-xr-x 20 user user 4096 Jan 15 10:33 ..
```

---

### 第7步: 运行 Benchmark 测试

**目的**: 执行性能测试并收集结果

#### 7.1 运行 spdlog 视觉场景测试

```bash
cd /path/to/log_benchmark/build

# 运行测试(输出到控制台)
./bin/gbenchmark_vision_spdlog
```

**预期输出示例**:
```
[2025-01-15 10:35:00.123] [main] [info] Vision spdlog benchmark initialized
Running ./bin/gbenchmark_vision_spdlog
Run on (16 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 4096 KiB (x8)
  Load Average: 0.05, 0.04, 0.05
--------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations
--------------------------------------------------------------------------------------------------
VisionSpdlogFixture/image_acquisition/1           12345 ns        12345 ns        100000
VisionSpdlogFixture/image_acquisition/2            6789 ns         6789 ns        100000
VisionSpdlogFixture/image_acquisition/4            3456 ns         3456 ns        100000
VisionSpdlogFixture/image_acquisition/8            1789 ns         1789 ns        100000
VisionSpdlogFixture/detection_result/1            23456 ns        23456 ns         50000
VisionSpdlogFixture/detection_result/2             12345 ns        12345 ns         50000
VisionSpdlogFixture/detection_result/4             8901 ns         8901 ns         50000
VisionSpdlogFixture/device_status/1                5678 ns         5678 ns         50000
VisionSpdlogFixture/alarm_log/1                   8901 ns         8901 ns          5000
VisionSpdlogFixture/production_stats/1             45678 ns        45678 ns          2500
```

**说明**:
- 测试将同时运行 5 个场景
- 每个场景可能有多个线程配置
- 日志会写入到 `logs/vision_spdlog.log`

#### 7.2 运行 glog 视觉场景测试

```bash
# 运行测试(输出到控制台)
./bin/gbenchmark_vision_glog
```

**预期输出示例**:
```
I0115 10:36:00.123456   12345 vision_glog.cpp:45] Vision glog benchmark initialized
Running ./bin/gbenchmark_vision_glog
Run on (16 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 4096 KiB (x8)
  Load Average: 0.06, 0.05, 0.05
--------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations
--------------------------------------------------------------------------------------------------
VisionGlogFixture/image_acquisition/1           45678 ns        45678 ns        50000
VisionGlogFixture/image_acquisition/2            28901 ns        28901 ns        50000
VisionGlogFixture/image_acquisition/4            17890 ns        17890 ns        50000
VisionGlogFixture/image_acquisition/8            12345 ns        12345 ns        50000
VisionGlogFixture/detection_result/1            56789 ns        56789 ns         25000
VisionGlogFixture/detection_result/2             34567 ns        34567 ns         25000
VisionGlogFixture/detection_result/4             23456 ns        23456 ns         25000
VisionGlogFixture/device_status/1                12345 ns        12345 ns         25000
VisionGlogFixture/alarm_log/1                   23456 ns        23456 ns          2500
VisionGlogFixture/production_stats/1             67890 ns        67890 ns          1250
```

**说明**:
- glog 的日志会写入到 `logs/glog/vision_glog.*.log`
- glog 采用滚动日志,可能生成多个文件

#### 7.3 查看生成的日志文件

```bash
# 查看 spdlog 日志
cd /path/to/log_benchmark/logs
tail -20 vision_spdlog.log
```

**预期日志内容**:
```
[2025-01-15 10:35:00.123] [main] [info] Vision spdlog benchmark initialized
[2025-01-15 10:35:01.456] [1405623456789] [info] [IMAGE] ts:1234567890, cam:3, img:0, 1920x1080, exp:2500us, gain:2.5, status:OK
[2025-01-15 10:35:01.457] [1405623456789] [info] [DETECTION] ts:1234567890, img:0, rois:15, defects:0, conf:0.92, model:ResNet50, time:12.34ms, status:PASS
[2025-01-15 10:35:01.458] [1405623456789] [info] [DEVICE] ts:1234567890, id:5, type:CAMERA, code:0, temp:45.2C, volt:230.1V, curr:2.3A, mode:AUTO
[2025-01-15 10:35:01.459] [1405623456789] [error] [ALARM] ts:1234567890, id:1234, severity:ERROR, type:IMAGE_TIMEOUT, msg:Camera 3 failed to acquire image within timeout, code:5001
[2025-01-15 10:35:01.460] [1405623456789] [info] [PRODUCTION] ts:1234567890, shift:1, total:5000, pass:4950, fail:50, rate:99.00%, cycle:1.2s, throughput:50.0/min
```

---

### 第8步: 保存测试结果

**目的**: 将测试结果保存到文件以便后续分析

**操作**:
```bash
# 创建结果目录
cd /path/to/log_benchmark
mkdir -p results

# 保存 spdlog 测试结果
cd build
./bin/gbenchmark_vision_spdlog > ../results/results_vision_spdlog.txt 2>&1

# 保存 glog 测试结果
./bin/gbenchmark_vision_glog > ../results/results_vision_glog.txt 2>&1
```

**说明**:
- `>` 将标准输出重定向到文件
- `2>&1` 将标准错误也重定向到同一文件

---

### 第9步: 分析结果

**目的**: 对比两个日志库的性能表现

#### 9.1 查看原始结果

```bash
cd /path/to/log_benchmark/results

# 查看 spdlog 结果
cat results_vision_spdlog.txt | grep "image_acquisition/8"
```

**预期输出**:
```
VisionSpdlogFixture/image_acquisition/8           1789 ns         1789 ns        100000
```

```bash
# 查看 glog 结果
cat results_vision_glog.txt | grep "image_acquisition/8"
```

**预期输出**:
```
VisionGlogFixture/image_acquisition/8           12345 ns        12345 ns        50000
```

#### 9.2 创建对比表

手工对比关键指标:

| 场景 | 线程数 | spdlog 时间 | glog 时间 | 性能对比 |
|-------|---------|-------------|-----------|---------|
| 图像采集 | 1 | 12345 ns | 45678 ns | spdlog 快 3.7x |
| 图像采集 | 8 | 1789 ns | 12345 ns | spdlog 快 6.9x |
| 检测结果 | 4 | 8901 ns | 23456 ns | spdlog 快 2.6x |
| 设备状态 | 1 | 5678 ns | 12345 ns | spdlog 快 2.2x |
| 异常告警 | 1 | 8901 ns | 23456 ns | spdlog 快 2.6x |
| 生产统计 | 1 | 45678 ns | 67890 ns | spdlog 快 1.5x |

**说明**:
- 高频场景(图像采集)中 spdlog 异步优势明显
- 低频场景(生产统计)两者差距较小
- 这符合预期:spdlog 异步在高吞吐场景下表现更好

#### 9.3 绘制性能图表(可选)

使用 Python 等工具绘制性能对比图:

```python
import matplotlib.pyplot as plt

scenarios = ['Image/1', 'Image/8', 'Detection/4', 'Device/1', 'Alarm/1', 'Production/1']
spdlog_times = [12345, 1789, 8901, 5678, 8901, 45678]
glog_times = [45678, 12345, 23456, 12345, 23456, 67890]

plt.figure(figsize=(10, 6))
plt.bar([i*2 for i in range(len(scenarios))], spdlog_times, width=0.8, label='spdlog')
plt.bar([i*2+1 for i in range(len(scenarios))], glog_times, width=0.8, label='glog')
plt.xticks([i*2+0.5 for i in range(len(scenarios))], scenarios, rotation=45)
plt.ylabel('Time (ns)')
plt.title('Log Library Performance Comparison')
plt.legend()
plt.tight_layout()
plt.savefig('../results/performance_comparison.png')
```

---

## 故障排查

### 问题1: CMake 找不到源码

**错误**:
```
CMake Error at CMakeLists.txt:128 (add_subdirectory):
  add_subdirectory given source "../_deps/spdlog-src" which is not an existing directory.
```

**解决**:
```bash
# 检查 _deps 目录
cd /path/to/log_benchmark
ls -la _deps/

# 如果目录不存在,重新下载源码(参考第1步)
```

### 问题2: 编译失败

**错误**:
```
make: *** No rule to make target 'all'.  Stop.
```

**解决**:
```bash
# 清理并重新配置
cd /path/to/log_benchmark/build
rm -rf *
cmake ..
make -j$(nproc)
```

### 问题3: 找不到测试数据

**错误**:
```
Failed to load image logs
```

**解决**:
```bash
# 确认数据文件存在
cd /path/to/log_benchmark
ls -la data/

# 如果不存在,重新生成数据(参考第5步)
cd build
./bin/gen_vision_data
```

### 问题4: 测试结果异常

**现象**: 时间值特别大(> 1ms)

**可能原因**:
1. 磁盘 IO 成为瓶颈(使用 HDD 而非 SSD)
2. CPU 频率被限制
3. 系统负载过高

**解决**:
```bash
# 使用 SSD 存放日志
# 固定 CPU 频率(Linux)
sudo cpupower frequency-set -g performance
# 在低负载环境下测试
```

---

## 总结

完整流程从准备源码到查看结果,共 9 个主要步骤:

1. **准备依赖源码** - 下载所需的第三方库
2. **验证目录结构** - 确保所有文件就位
3. **配置和编译** - 使用 CMake 构建项目
4. **验证编译结果** - 确认可执行文件生成
5. **生成测试数据** - 创建性能测试所需的数据
6. **准备运行环境** - 创建日志输出目录
7. **运行 Benchmark 测试** - 执行 spdlog 和 glog 测试
8. **保存测试结果** - 将结果保存到文件
9. **分析结果** - 对比性能并得出结论

整个流程预计耗时:
- 准备源码: 5-10 分钟(取决于网络速度)
- 编译项目: 5-15 分钟(取决于 CPU 性能)
- 生成数据: < 1 分钟
- 运行测试: 5-10 分钟
- 总计: 约 20-40 分钟
