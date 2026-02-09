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
// 日志函数宏定义
// ============================================================================

#define VISION_DEFINE_LOG_FUNCS(type, count) \
    typedef void (*fn_##type##_log_func)(const type& msg); \
    \
    void log_##type##_func0(const type& msg); \
    void log_##type##_func1(const type& msg); \
    void log_##type##_func2(const type& msg); \
    void log_##type##_func3(const type& msg); \
    void log_##type##_func4(const type& msg); \
    void log_##type##_func5(const type& msg); \
    void log_##type##_func6(const type& msg); \
    void log_##type##_func7(const type& msg); \
    void log_##type##_func8(const type& msg); \
    void log_##type##_func9(const type& msg); \
    void log_##type##_func10(const type& msg); \
    void log_##type##_func11(const type& msg); \
    void log_##type##_func12(const type& msg); \
    void log_##type##_func13(const type& msg); \
    void log_##type##_func14(const type& msg); \
    void log_##type##_func15(const type& msg); \
    void log_##type##_func16(const type& msg); \
    void log_##type##_func17(const type& msg); \
    void log_##type##_func18(const type& msg); \
    void log_##type##_func19(const type& msg); \
    void log_##type##_func20(const type& msg); \
    void log_##type##_func21(const type& msg); \
    void log_##type##_func22(const type& msg); \
    void log_##type##_func23(const type& msg); \
    void log_##type##_func24(const type& msg); \
    void log_##type##_func25(const type& msg); \
    void log_##type##_func26(const type& msg); \
    void log_##type##_func27(const type& msg); \
    void log_##type##_func28(const type& msg); \
    void log_##type##_func29(const type& msg); \
    void log_##type##_func30(const type& msg); \
    void log_##type##_func31(const type& msg); \
    void log_##type##_func32(const type& msg); \
    void log_##type##_func33(const type& msg); \
    void log_##type##_func34(const type& msg); \
    void log_##type##_func35(const type& msg); \
    void log_##type##_func36(const type& msg); \
    void log_##type##_func37(const type& msg); \
    void log_##type##_func38(const type& msg); \
    void log_##type##_func39(const type& msg); \
    void log_##type##_func40(const type& msg); \
    void log_##type##_func41(const type& msg); \
    void log_##type##_func42(const type& msg); \
    void log_##type##_func43(const type& msg); \
    void log_##type##_func44(const type& msg); \
    void log_##type##_func45(const type& msg); \
    void log_##type##_func46(const type& msg); \
    void log_##type##_func47(const type& msg); \
    void log_##type##_func48(const type& msg); \
    void log_##type##_func49(const type& msg); \
    \
    static fn_##type##_log_func log_##type##_funcs[] = { \
        log_##type##_func0, log_##type##_func1, log_##type##_func2, log_##type##_func3, log_##type##_func4, \
        log_##type##_func5, log_##type##_func6, log_##type##_func7, log_##type##_func8, log_##type##_func9, \
        log_##type##_func10, log_##type##_func11, log_##type##_func12, log_##type##_func13, log_##type##_func14, \
        log_##type##_func15, log_##type##_func16, log_##type##_func17, log_##type##_func18, log_##type##_func19, \
        log_##type##_func20, log_##type##_func21, log_##type##_func22, log_##type##_func23, log_##type##_func24, \
        log_##type##_func25, log_##type##_func26, log_##type##_func27, log_##type##_func28, log_##type##_func29, \
        log_##type##_func30, log_##type##_func31, log_##type##_func32, log_##type##_func33, log_##type##_func34, \
        log_##type##_func35, log_##type##_func36, log_##type##_func37, log_##type##_func38, log_##type##_func39, \
        log_##type##_func40, log_##type##_func41, log_##type##_func42, log_##type##_func43, log_##type##_func44, \
        log_##type##_func45, log_##type##_func46, log_##type##_func47, log_##type##_func48, log_##type##_func49 \
    }; \
    const int n_##type##_funcs = sizeof(log_##type##_funcs) / sizeof(log_##type##_funcs[0]);

// ============================================================================
// Benchmark 宏定义
// ============================================================================

// 场景1: 图像采集日志 - 多线程高吞吐
#define VISION_BENCHMARK_IMAGE(fixture, name) \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) { \
        static thread_local int idx = 0; \
        const int n = n_##type##_funcs; \
        for (auto _ : state) { \
            log_image_funcs[idx % n](image_logs[idx]); \
            idx = (idx + 1) % image_logs.size(); \
        } \
    } \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(1)->MinTime(VISION_MIN_TIME)->Teardown(DoTeardown); \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(2)->MinTime(VISION_MIN_TIME)->Teardown(DoTeardown); \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(4)->MinTime(VISION_MIN_TIME)->Teardown(DoTeardown); \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(8)->MinTime(VISION_MIN_TIME)->Teardown(Doardown);

// 场景2: 检测结果日志 - 实时记录
#define VISION_BENCHMARK_DETECTION(fixture, name) \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) { \
        static thread_local int idx = 0; \
        const int n = n_detection_funcs; \
        for (auto _ : state) { \
            log_detection_funcs[idx % n](detection_logs[idx]); \
            idx = (idx + 1) % detection_logs.size(); \
        } \
    } \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(1)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT); \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(2)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT); \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(4)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);

// 场景3: 设备状态日志 - 周期性上报
#define VISION_BENCHMARK_DEVICE(fixture, name) \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) { \
        static thread_local int idx = 0; \
        const int n = n_device_funcs; \
        for (auto _ : state) { \
            log_device_funcs[idx % n](device_logs[idx]); \
            idx = (idx + 1) % device_logs.size(); \
        } \
    } \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(1)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);

// 场景4: 异常告警日志 - 偶发记录
#define VISION_BENCHMARK_ALARM(fixture, name) \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) { \
        static thread_local int idx = 0; \
        const int n = n_alarm_funcs; \
        for (auto _ : state) { \
            log_alarm_funcs[idx % n](alarm_logs[idx]); \
            idx = (idx + 1) % alarm_logs.size(); \
        } \
    } \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(1)->Iterations(VISION_ITER_COUNT / 10)->Repetitions(VISION_REPEAT_COUNT);

// 场景5: 生产统计日志 - 周期性汇总
#define VISION_BENCHMARK_PRODUCTION(fixture, name) \
    BENCHMARK_DEFINE_F(fixture, name)(benchmark::State & state) { \
        static thread_local int idx = 0; \
        const int n = n_production_funcs; \
        for (auto _ : state) { \
            log_production_funcs[idx % n](production_logs[idx]); \
            idx = (idx + 1) % production_logs.size(); \
        } \
    } \
    BENCHMARK_REGISTER_F(fixture, name) \
        ->Threads(1)->Iterations(VISION_ITER_COUNT / 20)->Repetitions(VISION_REPEAT_COUNT);

// ============================================================================
// Fixture 基类宏
// ============================================================================

#define VISION_FIXTURE_BASE(class_name) \
class class_name : public benchmark::Fixture { \
public: \
    class_name() { \
        LoadTestLogs(); \
    } \
    \
    void LoadTestLogs() { \
        if (!LoadVisionImageLogs(image_logs, "data/vision_image_logs.bin")) { \
            fprintf(stderr, "Failed to load image logs\n"); \
            exit(EXIT_FAILURE); \
        } \
        if (!LoadVisionDetectionLogs(detection_logs, "data/vision_detection_logs.bin")) { \
            fprintf(stderr, "Failed to load detection logs\n"); \
            exit(EXIT_FAILURE); \
        } \
        if (!LoadVisionDeviceLogs(device_logs, "data/vision_device_logs.bin")) { \
            fprintf(stderr, "Failed to load device logs\n"); \
            exit(EXIT_FAILURE); \
        } \
        if (!LoadVisionAlarmLogs(alarm_logs, "data/vision_alarm_logs.bin")) { \
            fprintf(stderr, "Failed to load alarm logs\n"); \
            exit(EXIT_FAILURE); \
        } \
        if (!LoadVisionProductionLogs(production_logs, "data/vision_production_logs.bin")) { \
            fprintf(stderr, "Failed to load production logs\n"); \
            exit(EXIT_FAILURE); \
        } \
    } \
    \
public: \
    std::vector<VisionImageLog> image_logs; \
    std::vector<VisionDetectionLog> detection_logs; \
    std::vector<VisionDeviceLog> device_logs; \
    std::vector<VisionAlarmLog> alarm_logs; \
    std::vector<VisionProductionLog> production_logs; \
};

#endif // !VISION_GBENCHMARK_H_
