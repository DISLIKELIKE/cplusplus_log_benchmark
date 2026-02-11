# 日志库性能基准测试项目

本项目对多个 C/C++ 日志库进行性能基准测试，包括通用测试场景和机器视觉生产线专用场景。

## 目录结构

```
/workspace/
├── gbenchmark/              # 基准测试核心目录
│   ├── benchmark/          # C 语言的日志库测试
│   ├── cpp/                # C++ 的日志库测试
│   │   ├── easyloggingpp/  # easyloggingpp 测试
│   │   ├── fmtlog_block/   # fmtlog (阻塞模式) 测试
│   │   ├── fmtlog_drop/    # fmtlog (丢弃模式) 测试
│   │   ├── glog/           # Google glog 测试
│   │   ├── loguru/         # loguru 测试
│   │   ├── nanolog/        # Nanolog 测试
│   │   ├── quill_bounded_blocking/   # quill (阻塞模式)
│   │   ├── quill_bounded_dropping/  # quill (丢弃模式)
│   │   ├── reckless/       # reckless 测试
│   │   ├── spdlog_sync/    # spdlog (同步模式)
│   │   ├── spdlog_async/   # spdlog (异步模式)
│   │   ├── vision_spdlog/  # 机器视觉场景 spdlog 测试
│   │   └── vision_glog/    # 机器视觉场景 glog 测试
│   ├── log_gbenchmark.h    # 原始测试框架
│   ├── vision_gbenchmark.h # 机器视觉场景测试框架
│   ├── vision_log_msg.h    # 机器视觉日志消息定义
│   └── vision_log_msg.cpp  # 数据生成和序列化实现
├── gen_random_data/        # 原始测试数据生成工具
├── gen_vision_data/        # 机器视觉场景数据生成工具
├── log_msg/               # 原始日志消息定义
├── scripts/               # CPU 频率控制脚本
│   ├── set_cpu_freq.sh     # 固定 CPU 频率
│   ├── monitor_cpu_freq.sh # 监控 CPU 频率
│   └── turbo_boost.sh      # Turbo Boost 控制
├── report/                # 测试报告目录
├── CMakeLists.txt         # CMake 构建配置
├── build.sh / build.bat   # 构建脚本
└── run_benchmark.sh       # 运行基准测试
```

## 测试场景

### 原始通用测试场景

**场景 1: 压力测试 (MinTime 模式)**

- 目的: 测试日志库在高压力下的吞吐量，以及缓冲区紧张情况下的写入效率
- 线程数量: 1 / 2 / 4 / 8
- 测试时间: 最少 3 秒，期间尽可能多地写日志
- 适用: 主要针对异步日志库

**场景 2: 常规测试 (Iterations + Repetitions 模式)**

- 目的: 反映非压力情况下各日志库的表现
- 线程数量: 1, 1/4n, 1/2n, n, 2n (n = CPU 核心数)
- 迭代次数: 2000 次
- 重复次数: 5 次

**单条日志消息结构**:

```cpp
struct LogMsg {
    uint64_t u64;
    uint32_t u32;
    int64_t i64;
    int32_t i32;
    char s[128];
};
```

**日志输出格式**:

```
${Level}|${datetime}|${filename}.${line_no}|${func_name}|${thread_id} - u64: msg.u64, i64: msg.i64, u32: msg.u32, i32: msg.i32, s: msg.s
```

### 机器视觉生产线场景测试

根据机器视觉生产线软件的特点，设计了以下五个典型场景：

| 场景 | 业务描述 | 日志级别 | 数据量 | 测试方式 |
|------|----------|----------|--------|----------|
| **图像采集日志** | 工业相机每秒 500-2000 张图像采集 | INFO | 10000 条 | 1/2/4/8 线程压力测试 |
| **检测结果日志** | AI 模型检测每次结果记录 | INFO | 10000 条 | 1/2/4 线程迭代测试 |
| **设备状态监控** | PLC/传感器/相机定期上报状态 | INFO | 5000 条 | 单线程迭代测试 |
| **异常告警日志** | 缺陷或设备故障告警 | ERROR | 1000 条 | 单线程迭代测试 |
| **生产统计日志** | 产量、良率等统计数据汇总 | INFO | 500 条 | 单线程迭代测试 |

**视觉日志消息结构**:

```cpp
// 图像采集日志
struct VisionImageLog {
    uint64_t timestamp;           // 时间戳(微秒)
    uint32_t camera_id;           // 相机ID
    uint32_t image_id;            // 图像ID
    uint32_t width;               // 图像宽度
    uint32_t height;              // 图像高度
    uint64_t exposure_time;       // 曝光时间(微秒)
    float gain;                   // 增益
    char sensor_status[32];       // 传感器状态
};

// 检测结果日志
struct VisionDetectionLog {
    uint64_t timestamp;           // 时间戳(微秒)
    uint32_t image_id;           // 图像ID
    uint32_t roi_count;          // ROI数量
    uint32_t defect_count;       // 缺陷数量
    float confidence;             // 检测置信度
    char model_name[64];         // 模型名称
    float processing_time;       // 处理耗时(毫秒)
    char result_status[32];      // 结果状态
};

// 设备状态日志
struct VisionDeviceLog {
    uint64_t timestamp;           // 时间戳(微秒)
    uint32_t device_id;          // 设备ID
    char device_type[32];        // 设备类型
    uint32_t status_code;        // 状态码
    float temperature;           // 温度(摄氏度)
    float voltage;              // 电压(V)
    float current;              // 电流(A)
    char operation_mode[32];    // 运行模式
};

// 异常告警日志
struct VisionAlarmLog {
    uint64_t timestamp;           // 时间戳(微秒)
    uint32_t alarm_id;           // 告警ID
    uint32_t severity;           // 严重程度: 1-信息,2-警告,3-错误,4-严重
    uint32_t camera_id;          // 相机ID
    char alarm_type[64];         // 告警类型
    char alarm_message[256];     // 告警消息
    uint32_t error_code;         // 错误码
};

// 生产统计日志
struct VisionProductionLog {
    uint64_t timestamp;           // 时间戳(微秒)
    uint32_t shift_id;           // 班次ID
    uint32_t total_count;        // 总产量
    uint32_t pass_count;         // 合格数
    uint32_t fail_count;         // 不合格数
    float pass_rate;            // 良率(%)
    float avg_cycle_time;        // 平均周期时间(秒)
    float throughput;           // 吞吐量(件/分钟)
};
```

## 被测日志库

| 库名称 | 语言 | 特点 |
|--------|------|------|
| easyloggingpp | C++ | 功能强大，可扩展，线程和类型安全 |
| fmtlog | C++ | fmt 风格，纳秒级延迟 |
| glog | C++ | Google 官方实现 |
| haclog | C | 纯 C 实现，极快 |
| loguru | C++ | 轻量级日志库 |
| Nanolog | C++ | 纳秒级，仅支持 Linux |
| quill | C++ | 异步低延迟日志库 |
| reckless | C++ | 低延迟高吞吐异步日志 |
| spdlog | C++ | 快速 C++ 日志库 |

## 构建和运行

### 构建项目

```bash
# Linux
./build.sh

# Windows
build.bat
```

构建过程中会自动下载被测日志库。

### 运行基准测试

```bash
./run_benchmark.sh
```

生成的测试报告位于 `build` 目录下，名为 `benchmark_*.txt`。

### 生成机器视觉测试数据

```bash
cd build
cmake ..
make gen_vision_data
./bin/gen_vision_data
```

### 运行机器视觉场景测试

```bash
cd build

# 运行 spdlog 视觉场景测试
./bin/gbenchmark_vision_spdlog

# 运行 glog 视觉场景测试
./bin/gbenchmark_vision_glog
```

### CPU 频率控制

测试前建议固定 CPU 频率以获得稳定结果：

```bash
# 设置 CPU 频率为 3.2GHz
./scripts/set_cpu_freq.sh

# 监控 CPU 频率
./scripts/monitor_cpu_freq.sh
```

## 性能测试结论

根据基准测试结果，各日志库表现如下：

**场景 1 (压力测试)** - 较快的库:
- fmtlog
- Nanolog
- quill

**场景 2 (常规测试)** - 较快的库:
- fmtlog
- haclog
- Nanolog
- quill

### 缓冲区满时的处理策略

异步日志库在缓冲区满时有三种处理策略：

| 策略 | 说明 | 采用的库 |
|------|------|----------|
| **阻塞** | 生产者线程阻塞，等待缓冲区有足够空间 | haclog, Nanolog |
| **丢弃** | 放弃写入本条日志或直接套圈写入 | fmtlog (可选), quill (可选) |
| **扩展** | 动态增加缓冲区长度 | quill (可选) |

### 各日志库优缺点分析

**fmtlog**
- 优点: 两种场景下速度良好，支持阻塞和丢弃模式，fmt 格式化风格
- 缺点: drop 模式下可能出现日志丢失

**haclog**
- 优点: 场景 2 表现良好，缓冲区满时阻塞不丢日志，纯 C 实现
- 缺点: 当吞吐量超过阈值时效率大幅下跌

**Nanolog**
- 优点: 两种场景下都表现优秀，保证不丢日志的同时吞吐量极高
- 缺点: 日志无法直接阅读需要解码，仅支持 Linux

**quill**
- 优点: 两种场景下速度良好且稳定，fmt 格式化风格，支持多种缓冲区策略
- 缺点: BoundedDropping 模式会丢日志，BoundedBlocking 模式无法完成场景 1 测试

## 场景参数配置

在 `vision_gbenchmark.h` 中可调整以下参数：

```cpp
// 测试数据量
#define VISION_IMAGE_LOG_COUNT 10000      // 图像采集日志数量
#define VISION_DETECTION_LOG_COUNT 10000   // 检测结果日志数量
#define VISION_DEVICE_LOG_COUNT 5000       // 设备状态日志数量
#define VISION_ALARM_LOG_COUNT 1000       // 异常告警日志数量
#define VISION_PRODUCTION_LOG_COUNT 500   // 生产统计日志数量

// 测试时间配置
#define VISION_MIN_TIME 3                 // 压力测试最小时间(秒)
#define VISION_ITER_COUNT 500             // 迭代测试次数
#define VISION_REPEAT_COUNT 5             // 重复测试次数
```

## 注意事项

1. 由于部分日志库不支持所有平台，仅在 Linux 能够测试所有日志库
2. 测试结果受 CPU 频率动态调节影响，建议测试前固定频率
3. 磁盘 IO 速度会影响测试结果，建议使用 SSD
4. 多线程下的 Time 值是按吞吐量和时间平均的结果，不能简单理解为单次耗时
5. quill 在某些模式下可能导致进程卡死，已跳过相关测试

## 扩展其他日志库

如需添加其他日志库的测试，参考以下步骤：

1. 在 `gbenchmark/cpp/` 下创建新目录，如 `vision_xxxlog/`
2. 复制 `vision_spdlog_benchmark.cpp` 作为模板
3. 修改日志库的初始化和日志输出语句
4. 更新 `CMakeLists.txt` 添加构建配置
5. 重新编译并测试

## 许可证

本项目基于各自的日志库许可证进行测试。

---

## 测试场景详细分析

### 一、通用日志测试场景（2种测试模式）

#### 场景 1：压力测试（MinTime 模式）
- **目的**：测试日志库在高压力下的吞吐量和前端写入效率
- **配置**：设定最小测试时间（默认 3 秒），在此时间内尽可能多地写入日志
- **线程数**：1 / 2 / 4 / 8 线程
- **适用**：主要针对异步日志库

#### 场景 2：非压力测试（Iterations + Repetitions 模式）
- **目的**：测试日志库在正常负载下的性能表现
- **配置**：设定迭代次数（默认 2000 次）和重复次数（默认 5 次）
- **线程数**：1, 1/4n, 1/2n, n, 2n（n 为硬件并发线程数）

### 二、机器视觉生产线专用测试场景（5种业务场景）

| 场景名称 | 业务场景 | 日志内容 | 数据量 | 测试方式 |
|---------|---------|---------|--------|---------|
| **图像采集日志** | 工业相机每秒采集 500-2000 张图像 | 时间戳、相机ID、图像ID、分辨率、曝光时间、增益、传感器状态 | 10000 条 | 1/2/4/8 线程压力测试 |
| **检测结果日志** | AI 模型检测后记录每次结果 | 时间戳、图像ID、ROI数量、缺陷数量、置信度、模型名称、处理耗时、结果状态 | 10000 条 | 1/2/4 线程迭代测试 |
| **设备状态监控** | PLC、传感器、相机定期上报状态 | 时间戳、设备ID、设备类型、状态码、温度、电压、电流、运行模式 | 5000 条 | 单线程迭代测试 |
| **异常告警日志** | 检测到缺陷或设备故障时记录 | 时间戳、告警ID、严重程度、相机ID、告警类型、告警消息、错误码 | 1000 条 | 单线程迭代测试（迭代次数较少） |
| **生产统计日志** | 定期输出产量、良率等统计数据 | 时间戳、班次ID、总产量、合格数、不合格数、良率、平均周期时间、吞吐量 | 500 条 | 单线程迭代测试（迭代次数最少） |

### 三、测试的日志库（9种）

| 日志库 | 测试文件 | 模式 |
|-------|---------|------|
| **spdlog** | `gbenchmark_spdlog_sync.cpp`, `gbenchmark_spdlog_async.cpp` | 同步/异步 |
| **glog** | `gbenchmark_glog.cpp` | 同步 |
| **quill** | `gbenchmark_quill_bounded_dropping.cpp`, `gbenchmark_quill_bounded_blocking.cpp` | BoundedDropping/BoundedBlocking |
| **fmtlog** | `gbenchmark_fmtlog_drop.cpp`, `gbenchmark_fmtlog_block.cpp` | Drop/Block |
| **nanolog** | `gbenchmark_nanolog.cpp` | 异步 |
| **loguru** | `gbenchmark_loguru.cpp` | - |
| **easyloggingpp** | `gbenchmark_easyloggingpp.cpp` | - |
| **reckless** | `gbenchmark_reckless.cpp` | - |
| **haclog** | (C 语言实现) | 异步 |

### 四、测试框架封装

项目使用 `RUN_GBENCHMARK` 宏简化测试代码编写，定义在 `gbenchmark/log_gbenchmark.h`：

- 使用 `thread_local` 索引轮询不同的日志函数和消息
- 自动生成迭代/重复测试和压力测试用例
- 支持 5 种不同线程数的测试配置

### 五、关键测试维度总结

1. **性能维度**：吞吐量、延迟、CPU 占用
2. **并发维度**：单线程、多线程（1/2/4/8 线程）
3. **压力维度**：正常负载 vs 极限压力
4. **缓冲区策略**：阻塞、丢弃、扩展
5. **业务维度**：通用日志 vs 机器视觉专用场景
