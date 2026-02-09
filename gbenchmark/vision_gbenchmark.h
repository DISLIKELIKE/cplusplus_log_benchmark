#ifndef VISION_GBENCHMARK_H_
#define VISION_GBENCHMARK_H_

#include "benchmark/benchmark.h"
#include "vision_log_msg.h"
#include <vector>

// ============================================================================
// 场景配置
// ============================================================================

// 场景1: 高频图像采集日志 (模拟每秒 500-2000 张图像的采集)
#define VISION_IMAGE_LOG_COUNT 10000

// 场景2: 实时检测日志 (模拟实时检测结果记录)
#define VISION_DETECTION_LOG_COUNT 10000

// 场景3: 设备状态监控 (模拟 50 个设备每秒上报状态)
#define VISION_DEVICE_LOG_COUNT 5000

// 场景4: 异常告警 (模拟偶发的异常情况,约 1% 的检测率)
#define VISION_ALARM_LOG_COUNT 1000

// 场景5: 生产统计 (模拟每分钟输出一次统计数据)
#define VISION_PRODUCTION_LOG_COUNT 500

// ============================================================================
// 场景时间配置
// ============================================================================

#ifndef VISION_MIN_TIME
#define VISION_MIN_TIME 3
#endif

#ifndef VISION_ITER_COUNT
#define VISION_ITER_COUNT 500
#define VISION_REPEAT_COUNT 5
#endif

// ============================================================================
// Vision Benchmark 宏定义
// ============================================================================

// 图像采集日志 - 高频场景
#define VISION_BENCHMARK_IMAGE(fixture, name)                      \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) {    \
        static thread_local int idx = 0;                           \
        const int n = n_image_funcs;                                \
        for (auto _ : state) {                                     \
            log_image_funcs[idx % n](image_logs[idx]);             \
            idx = (idx + 1) % image_logs.size();                   \
        }                                                          \
    }                                                              \
    BENCHMARK_REGISTER_F(fixture, name)                              \
        ->Threads(1)->MinTime(VISION_MIN_TIME);                     \
    BENCHMARK_REGISTER_F(fixture, name)                              \
        ->Threads(2)->MinTime(VISION_MIN_TIME);                     \
    BENCHMARK_REGISTER_F(fixture, name)                              \
        ->Threads(4)->MinTime(VISION_MIN_TIME);                     \
    BENCHMARK_REGISTER_F(fixture, name)                              \
        ->Threads(8)->MinTime(VISION_MIN_TIME);

// 检测结果日志 - 实时场景
#define VISION_BENCHMARK_DETECTION(fixture, name)                    \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) {      \
        static thread_local int idx = 0;                             \
        const int n = n_detection_funcs;                             \
        for (auto _ : state) {                                     \
            log_detection_funcs[idx % n](detection_logs[idx]);       \
            idx = (idx + 1) % detection_logs.size();               \
        }                                                          \
    }                                                              \
    BENCHMARK_REGISTER_F(fixture, name)                              \
        ->Threads(1)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT); \
    BENCHMARK_REGISTER_F(fixture, name)                              \
        ->Threads(2)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT); \
    BENCHMARK_REGISTER_F(fixture, name)                              \
        ->Threads(4)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);

// 设备状态日志 - 周期性上报
#define VISION_BENCHMARK_DEVICE(fixture, name)                        \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) {     \
        static thread_local int idx = 0;                              \
        const int n = n_device_funcs;                                \
        for (auto _ : state) {                                       \
            log_device_funcs[idx % n](device_logs[idx]);             \
            idx = (idx + 1) % device_logs.size();                   \
        }                                                            \
    }                                                                \
    BENCHMARK_REGISTER_F(fixture, name)                                \
        ->Threads(1)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);

// 异常告警日志 - 偶发场景
#define VISION_BENCHMARK_ALARM(fixture, name)                         \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) {     \
        static thread_local int idx = 0;                              \
        const int n = n_alarm_funcs;                                 \
        for (auto _ : state) {                                       \
            log_alarm_funcs[idx % n](alarm_logs[idx]);                \
            idx = (idx + 1) % alarm_logs.size();                    \
        }                                                            \
    }                                                                \
    BENCHMARK_REGISTER_F(fixture, name)                                \
        ->Threads(1)->Iterations(VISION_ITER_COUNT / 10)->Repetitions(VISION_REPEAT_COUNT);

// 生产统计日志 - 周期性汇总
#define VISION_BENCHMARK_PRODUCTION(fixture, name)                     \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) {      \
        static thread_local int idx = 0;                               \
        const int n = n_production_funcs;                              \
        for (auto _ : state) {                                         \
            log_production_funcs[idx % n](production_logs[idx]);         \
            idx = (idx + 1) % production_logs.size();                 \
        }                                                              \
    }                                                                 \
    BENCHMARK_REGISTER_F(fixture, name)                                 \
        ->Threads(1)->Iterations(VISION_ITER_COUNT / 20)->Repetitions(VISION_REPEAT_COUNT);

#endif // !VISION_GBENCHMARK_H_
