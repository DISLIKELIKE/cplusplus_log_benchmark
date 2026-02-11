# 机器视觉日志测试运行流程详解

## 一、测试运行流程

### 1. 测试场景是独立并行运行的

spdlog 机器视觉测试会注册 5 个独立的 benchmark，每个场景都是独立的测试用例：

```cpp
// 场景1: 图像采集日志 - 高频
VISION_BENCHMARK_IMAGE(VisionSpdlogFixture, image_acquisition)

// 场景2: 检测结果日志 - 实时
VISION_BENCHMARK_DETECTION(VisionSpdlogFixture, detection_result)

// 场景3: 设备状态日志 - 周期性上报
VISION_BENCHMARK_DEVICE(VisionSpdlogFixture, device_status)

// 场景4: 异常告警日志 - 偶发
VISION_BENCHMARK_ALARM(VisionSpdlogFixture, alarm_log)

// 场景5: 生产统计日志 - 周期性汇总
VISION_BENCHMARK_PRODUCTION(VisionSpdlogFixture, production_stats)
```

### 2. Google Benchmark 会自动执行所有场景

当你运行 `./bin/gbenchmark_vision_spdlog` 时，Google Benchmark 框架会：
- 自动识别所有注册的 benchmark
- 按照你指定的参数执行测试
- 输出每个场景的性能结果

### 3. 日志写入到同一个文件

是的，所有 5 个场景的日志都写入到同一个文件：`logs/vision_spdlog.log`

---

## 二、日志内容示例

### 日志格式

```
[%Y-%m-%d %H:%M:%S.%e] [%t] [%^%l%$] %v
```

### 日志内容示例

```
[2025-02-11 14:30:25.123] [140192] [info] [IMAGE] ts:1739234625123, cam:1, img:4521, 1920x1080, exp:5000us, gain:1.2, status:OK
[2025-02-11 14:30:25.124] [140192] [info] [DETECTION] ts:1739234625125, img:4521, rois:5, defects:0, conf:0.98, model:YOLOv8, time:12.34ms, status:PASS
[2025-02-11 14:30:25.125] [140192] [info] [DEVICE] ts:1739234625125, id:PLC01, type:PLC, code:0, temp:45.5C, volt:24.0V, curr:1.2A, mode:RUNNING
[2025-02-11 14:30:25.126] [140192] [error] [ALARM] ts:1739234625126, id:1024, severity:ERROR, type:OVER_TEMPERATURE, msg:Camera temperature exceeded threshold, code:0x0012
[2025-02-11 14:30:25.127] [140192] [info] [PRODUCTION] ts:1739234625127, shift:2, total:15420, pass:15123, fail:297, rate:98.07%, cycle:2.34s, throughput:423.50/min
```

---

## 三、测试参数解释

### 1. MinTime（压力测试）

```cpp
->MinTime(VISION_MIN_TIME)  // VISION_MIN_TIME = 3 秒
```

**含义**：至少运行 3 秒钟，在这 3 秒内尽可能多地写日志

**线程配置**：
- 1 线程：运行 3 秒
- 2 线程：运行 3 秒
- 4 线程：运行 3 秒
- 8 线程：运行 3 秒

**适用场景**：图像采集日志（高频场景）

**输出示例**：
```
VisionSpdlogFixture/image_acquisition/1            12345 ns       12345 ns    234567
VisionSpdlogFixture/image_acquisition/2             6789 ns        6789 ns    467890
VisionSpdlogFixture/image_acquisition/4             3456 ns        3456 ns    890123
VisionSpdlogFixture/image_acquisition/8             1723 ns        1723 ns    1567890
```

### 2. Iterations（迭代次数）

```cpp
->Iterations(VISION_ITER_COUNT)  // VISION_ITER_COUNT = 500
```

**含义**：固定执行 500 次日志写入操作

**适用场景**：检测结果日志（1/2/4 线程）、设备状态日志（单线程）

### 3. Repetitions（重复次数）

```cpp
->Repetitions(VISION_REPEAT_COUNT)  // VISION_REPEAT_COUNT = 5
```

**含义**：每个测试用例重复运行 5 次，取平均值

**为什么需要重复**：消除单次测试的偶然性，得到更稳定的结果

### 4. 完整示例：图像采集 + 检测结果组合

```
图像采集场景（MinTime=3秒）：
┌─────────────────────────────────────────────┐
│ 线程1: ███████████████████████ │ 3秒，约写 50,000 条
│ 线程2: ███████████████████████ │ 3秒，约写 50,000 条
│ 线程4: ███████████████████████ │ 3秒，约写 50,000 条
│ 线程8: ███████████████████████ │ 3秒，约写 50,000 条
└─────────────────────────────────────────────┘
              并发写入 logs/vision_spdlog.log

检测结果场景（Iterations=500, Repetitions=5）：
┌─────────────────────────────────────────────┐
│ 线程1: ███████████████████████ │ 500次 × 5轮 = 2,500 条
│ 线程2: ███████████████████████ │ 500次 × 5轮 = 2,500 条
│ 线程4: ███████████████████████ │ 500次 × 5轮 = 2,500 条
└─────────────────────────────────────────────┘
              并发写入 logs/vision_spdlog.log
```

---

## 四、各场景配置总结

| 场景 | 测试模式 | 线程数 | 迭代次数 | 重复次数 | 数据量 |
|------|---------|--------|---------|---------|--------|
| **图像采集日志** | MinTime(3秒) | 1/2/4/8 | 无限制 | 10,000 条（轮询） |
| **检测结果日志** | Iterations(500) | 1/2/4 | 5 | 10,000 条（轮询） |
| **设备状态监控** | Iterations(500) | 1 | 5 | 5,000 条（轮询） |
| **异常告警日志** | Iterations(50) | 1 | 5 | 1,000 条（轮询） |
| **生产统计日志** | Iterations(25) | 1 | 5 | 500 条（轮询） |

**注意**：告警和生产统计场景的迭代次数减少（`/10` 和 `/20`），是因为这两个场景的数据量本来就少（1000 条和 500 条），500 次迭代会造成过多重复。
