# 机器视觉生产线日志场景测试

本目录包含针对机器视觉生产线软件场景的日志库性能测试。

## 业务场景

根据机器视觉生产线软件的特点,我们设计了以下五个典型场景:

### 1. 图像采集日志 (高频场景)
- **业务场景**: 工业相机持续采集图像,每秒 500-2000 张图像
- **日志内容**: 时间戳、相机ID、图像ID、分辨率、曝光时间、增益、传感器状态
- **日志级别**: INFO
- **数据量**: 10000 条
- **测试方式**: 1/2/4/8 线程压力测试 (MinTime模式)

### 2. 检测结果日志 (实时场景)
- **业务场景**: 图像经过AI模型检测,记录每次检测结果
- **日志内容**: 时间戳、图像ID、ROI数量、缺陷数量、置信度、模型名称、处理耗时、结果状态
- **日志级别**: INFO
- **数据量**: 10000 条
- **测试方式**: 1/2/4 线程迭代测试 (Iterations模式)

### 3. 设备状态监控 (周期性场景)
- **业务场景**: PLC、传感器、相机等设备定期上报状态
- **日志内容**: 时间戳、设备ID、设备类型、状态码、温度、电压、电流、运行模式
- **日志级别**: INFO
- **数据量**: 5000 条
- **测试方式**: 单线程迭代测试

### 4. 异常告警日志 (偶发场景)
- **业务场景**: 检测到缺陷或设备故障时记录告警
- **日志内容**: 时间戳、告警ID、严重程度、相机ID、告警类型、告警消息、错误码
- **日志级别**: ERROR
- **数据量**: 1000 条
- **测试方式**: 单线程迭代测试 (迭代次数较少)

### 5. 生产统计日志 (周期性汇总)
- **业务场景**: 定期输出产量、良率等统计数据
- **日志内容**: 时间戳、班次ID、总产量、合格数、不合格数、良率、平均周期时间、吞吐量
- **日志级别**: INFO
- **数据量**: 500 条
- **测试方式**: 单线程迭代测试 (迭代次数最少)

## 项目结构

```
gbenchmark/
├── vision_log_msg.h           # 视觉日志消息类型定义
├── vision_log_msg.cpp         # 数据生成和序列化实现
├── vision_gbenchmark.h        # Benchmark 框架封装
└── cpp/
    ├── vision_spdlog/        # spdlog 视觉场景测试
    │   └── vision_spdlog_benchmark.cpp
    └── vision_glog/         # glog 视觉场景测试
        └── vision_glog_benchmark.cpp

gen_vision_data/
└── gen_vision_data.cpp       # 生成测试数据工具

data/                         # 测试数据目录 (运行后生成)
    ├── vision_image_logs.bin
    ├── vision_detection_logs.bin
    ├── vision_device_logs.bin
    ├── vision_alarm_logs.bin
    └── vision_production_logs.bin
```

## 使用方法

### 1. 生成测试数据

```bash
# 编译数据生成工具
cd build
cmake ..
make gen_vision_data

# 生成测试数据
./bin/gen_vision_data
```

这将在 `data/` 目录下生成 5 个二进制文件,包含所有测试场景的日志数据。

### 2. 运行 Benchmark

```bash
# 编译所有 benchmark
cd build
cmake ..
make -j$(nproc)

# 运行 spdlog 视觉场景测试
./bin/gbenchmark_vision_spdlog

# 运行 glog 视觉场景测试
./bin/gbenchmark_vision_glog
```

### 3. 查看结果

测试结果将输出到控制台,也可以重定向到文件:

```bash
./bin/gbenchmark_vision_spdlog > results_vision_spdlog.txt 2>&1
./bin/gbenchmark_vision_glog > results_vision_glog.txt 2>&1
```

## 场景参数说明

在 `vision_gbenchmark.h` 中可以调整以下参数:

```cpp
// 测试数据量
#define VISION_IMAGE_LOG_COUNT 10000      // 图像采集日志数量
#define VISION_DETECTION_LOG_COUNT 10000   // 检测结果日志数量
#define VISION_DEVICE_LOG_COUNT 5000       // 设备状态日志数量
#define VISION_ALARM_LOG_COUNT 1000        // 异常告警日志数量
#define VISION_PRODUCTION_LOG_COUNT 500    // 生产统计日志数量

// 测试时间配置
#define VISION_MIN_TIME 3                 // 压力测试最小时间(秒)
#define VISION_ITER_COUNT 500             // 迭代测试次数
#define VISION_REPEAT_COUNT 5             // 重复测试次数
```

## 性能指标解读

### Google Benchmark 输出格式

```
Benchmark                        Time           CPU            Iterations
--------------------------------------------------------------------------
VisionSpdlogFixture/image_acquisition/1       12345 ns       12345 ns            100000
VisionSpdlogFixture/image_acquisition/2        6789 ns        6789 ns            100000
...
```

- **Time**: 平均每次日志写入的时间 (纳秒)
- **CPU**: CPU 时间 (纳秒)
- **Iterations**: 迭代次数

### 多线程测试

多线程下的 Time 值是按吞吐量和时间平均的结果:

```
Google Benchmark Time = (sum(每个线程的耗时) ÷ 线程数量) ÷ 总执行次数
```

这个值可以用于横向对比不同库在不同线程数下的性能,但不能简单地理解为单次耗时。

## 与原始测试的区别

| 特性 | 原始测试 | 视觉场景测试 |
|------|----------|-------------|
| 日志内容 | 通用结构 | 生产线专用结构 |
| 场景数量 | 2个场景 | 5个场景 |
| 数据类型 | 单一类型 | 5种不同类型 |
| 业务相关性 | 通用 | 机器视觉专用 |
| 线程配置 | 1/2/4/8/n | 根据场景定制 |

## 测试的日志库

- **spdlog**: 异步日志库,使用 async_factory
- **glog**: Google 官方日志库,同步写入

## 预期性能对比

基于日志库的特性,预期性能表现:

- **图像采集场景 (高频)**: spdlog 异步应该明显优于 glog 同步
- **检测结果场景 (实时)**: spdlog 异步有优势,但差距可能较小
- **设备状态场景 (周期性)**: 两者差距较小
- **异常告警场景 (偶发)**: 两者差距较小
- **生产统计场景 (低频)**: 两者性能接近

## 注意事项

1. **数据依赖**: 运行 benchmark 前必须先生成测试数据
2. **磁盘IO**: 日志写入磁盘的速度会影响测试结果,建议使用 SSD
3. **缓冲区设置**: spdlog 默认队列大小为 8192,可以根据需要调整
4. **CPU频率**: 测试时建议固定 CPU 频率,避免动态调频影响结果

## 扩展说明

如果需要添加其他日志库的测试,参考以下步骤:

1. 在 `gbenchmark/cpp/` 下创建新目录,如 `vision_xxxlog/`
2. 复制 `vision_spdlog_benchmark.cpp` 作为模板
3. 修改日志库的初始化和日志输出语句
4. 更新 `CMakeLists.txt` 添加构建配置
5. 重新编译并测试

## 贡献

如果您有其他机器视觉场景需要测试,欢迎提交 PR 或 Issue。
