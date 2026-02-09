#include <atomic>
#include <thread>
#include <vector>
#include "vision_gbenchmark.h"
#include <glog/logging.h>

std::once_flag g_glog_vision_init_flag;

// ============================================================================
// 定义日志函数 - 手动展开,不使用 for 循环
// ============================================================================

// 图像采集日志函数 - 0-49
#define VISION_GLOG_IMAGE_LOG_FUNC(num) \
    void log_image_func##num(const VisionImageLog& msg) { \
        LOG(INFO) << "[IMAGE] ts:" << msg.timestamp \
                  << ", cam:" << msg.camera_id \
                  << ", img:" << msg.image_id \
                  << ", " << msg.width << "x" << msg.height \
                  << ", exp:" << msg.exposure_time << "us" \
                  << ", gain:" << msg.gain \
                  << ", status:" << msg.sensor_status; \
    }

VISION_GLOG_IMAGE_LOG_FUNC(0)
VISION_GLOG_IMAGE_LOG_FUNC(1)
VISION_GLOG_IMAGE_LOG_FUNC(2)
VISION_GLOG_IMAGE_LOG_FUNC(3)
VISION_GLOG_IMAGE_LOG_FUNC(4)
VISION_GLOG_IMAGE_LOG_FUNC(5)
VISION_GLOG_IMAGE_LOG_FUNC(6)
VISION_GLOG_IMAGE_LOG_FUNC(7)
VISION_GLOG_IMAGE_LOG_FUNC(8)
VISION_GLOG_IMAGE_LOG_FUNC(9)
VISION_GLOG_IMAGE_LOG_FUNC(10)
VISION_GLOG_IMAGE_LOG_FUNC(11)
VISION_GLOG_IMAGE_LOG_FUNC(12)
VISION_GLOG_IMAGE_LOG_FUNC(13)
VISION_GLOG_IMAGE_LOG_FUNC(14)
VISION_GLOG_IMAGE_LOG_FUNC(15)
VISION_GLOG_IMAGE_LOG_FUNC(16)
VISION_GLOG_IMAGE_LOG_FUNC(17)
VISION_GLOG_IMAGE_LOG_FUNC(18)
VISION_GLOG_IMAGE_LOG_FUNC(19)
VISION_GLOG_IMAGE_LOG_FUNC(20)
VISION_GLOG_IMAGE_LOG_FUNC(21)
VISION_GLOG_IMAGE_LOG_FUNC(22)
VISION_GLOG_IMAGE_LOG_FUNC(23)
VISION_GLOG_IMAGE_LOG_FUNC(24)
VISION_GLOG_IMAGE_LOG_FUNC(25)
VISION_GLOG_IMAGE_LOG_FUNC(26)
VISION_GLOG_IMAGE_LOG_FUNC(27)
VISION_GLOG_IMAGE_LOG_FUNC(28)
VISION_GLOG_IMAGE_LOG_FUNC(29)
VISION_GLOG_IMAGE_LOG_FUNC(30)
VISION_GLOG_IMAGE_LOG_FUNC(31)
VISION_GLOG_IMAGE_LOG_FUNC(32)
VISION_GLOG_IMAGE_LOG_FUNC(33)
VISION_GLOG_IMAGE_LOG_FUNC(34)
VISION_GLOG_IMAGE_LOG_FUNC(35)
VISION_GLOG_IMAGE_LOG_FUNC(36)
VISION_GLOG_IMAGE_LOG_FUNC(37)
VISION_GLOG_IMAGE_LOG_FUNC(38)
VISION_GLOG_IMAGE_LOG_FUNC(39)
VISION_GLOG_IMAGE_LOG_FUNC(40)
VISION_GLOG_IMAGE_LOG_FUNC(41)
VISION_GLOG_IMAGE_LOG_FUNC(42)
VISION_GLOG_IMAGE_LOG_FUNC(43)
VISION_GLOG_IMAGE_LOG_FUNC(44)
VISION_GLOG_IMAGE_LOG_FUNC(45)
VISION_GLOG_IMAGE_LOG_FUNC(46)
VISION_GLOG_IMAGE_LOG_FUNC(47)
VISION_GLOG_IMAGE_LOG_FUNC(48)
VISION_GLOG_IMAGE_LOG_FUNC(49)

// 检测结果日志函数 - 0-49
#define VISION_GLOG_DETECTION_LOG_FUNC(num) \
    void log_detection_func##num(const VisionDetectionLog& msg) { \
        LOG(INFO) << "[DETECTION] ts:" << msg.timestamp \
                  << ", img:" << msg.image_id \
                  << ", rois:" << msg.roi_count \
                  << ", defects:" << msg.defect_count \
                  << ", conf:" << msg.confidence \
                  << ", model:" << msg.model_name \
                  << ", time:" << msg.processing_time << "ms" \
                  << ", status:" << msg.result_status; \
    }

VISION_GLOG_DETECTION_LOG_FUNC(0)
VISION_GLOG_DETECTION_LOG_FUNC(1)
VISION_GLOG_DETECTION_LOG_FUNC(2)
VISION_GLOG_DETECTION_LOG_FUNC(3)
VISION_GLOG_DETECTION_LOG_FUNC(4)
VISION_GLOG_DETECTION_LOG_FUNC(5)
VISION_GLOG_DETECTION_LOG_FUNC(6)
VISION_GLOG_DETECTION_LOG_FUNC(7)
VISION_GLOG_DETECTION_LOG_FUNC(8)
VISION_GLOG_DETECTION_LOG_FUNC(9)
VISION_GLOG_DETECTION_LOG_FUNC(10)
VISION_GLOG_DETECTION_LOG_FUNC(11)
VISION_GLOG_DETECTION_LOG_FUNC(12)
VISION_GLOG_DETECTION_LOG_FUNC(13)
VISION_GLOG_DETECTION_LOG_FUNC(14)
VISION_GLOG_DETECTION_LOG_FUNC(15)
VISION_GLOG_DETECTION_LOG_FUNC(16)
VISION_GLOG_DETECTION_LOG_FUNC(17)
VISION_GLOG_DETECTION_LOG_FUNC(18)
VISION_GLOG_DETECTION_LOG_FUNC(19)
VISION_GLOG_DETECTION_LOG_FUNC(20)
VISION_GLOG_DETECTION_LOG_FUNC(21)
VISION_GLOG_DETECTION_LOG_FUNC(22)
VISION_GLOG_DETECTION_LOG_FUNC(23)
VISION_GLOG_DETECTION_LOG_FUNC(24)
VISION_GLOG_DETECTION_LOG_FUNC(25)
VISION_GLOG_DETECTION_LOG_FUNC(26)
VISION_GLOG_DETECTION_LOG_FUNC(27)
VISION_GLOG_DETECTION_LOG_FUNC(28)
VISION_GLOG_DETECTION_LOG_FUNC(29)
VISION_GLOG_DETECTION_LOG_FUNC(30)
VISION_GLOG_DETECTION_LOG_FUNC(31)
VISION_GLOG_DETECTION_LOG_FUNC(32)
VISION_GLOG_DETECTION_LOG_FUNC(33)
VISION_GLOG_DETECTION_LOG_FUNC(34)
VISION_GLOG_DETECTION_LOG_FUNC(35)
VISION_GLOG_DETECTION_LOG_FUNC(36)
VISION_GLOG_DETECTION_LOG_FUNC(37)
VISION_GLOG_DETECTION_LOG_FUNC(38)
VISION_GLOG_DETECTION_LOG_FUNC(39)
VISION_GLOG_DETECTION_LOG_FUNC(40)
VISION_GLOG_DETECTION_LOG_FUNC(41)
VISION_GLOG_DETECTION_LOG_FUNC(42)
VISION_GLOG_DETECTION_LOG_FUNC(43)
VISION_GLOG_DETECTION_LOG_FUNC(44)
VISION_GLOG_DETECTION_LOG_FUNC(45)
VISION_GLOG_DETECTION_LOG_FUNC(46)
VISION_GLOG_DETECTION_LOG_FUNC(47)
VISION_GLOG_DETECTION_LOG_FUNC(48)
VISION_GLOG_DETECTION_LOG_FUNC(49)

// 设备状态日志函数 - 0-49
#define VISION_GLOG_DEVICE_LOG_FUNC(num) \
    void log_device_func##num(const VisionDeviceLog& msg) { \
        LOG(INFO) << "[DEVICE] ts:" << msg.timestamp \
                  << ", id:" << msg.device_id \
                  << ", type:" << msg.device_type \
                  << ", code:" << msg.status_code \
                  << ", temp:" << msg.temperature << "C" \
                  << ", volt:" << msg.voltage << "V" \
                  << ", curr:" << msg.current << "A" \
                  << ", mode:" << msg.operation_mode; \
    }

VISION_GLOG_DEVICE_LOG_FUNC(0)
VISION_GLOG_DEVICE_LOG_FUNC(1)
VISION_GLOG_DEVICE_LOG_FUNC(2)
VISION_GLOG_DEVICE_LOG_FUNC(3)
VISION_GLOG_DEVICE_LOG_FUNC(4)
VISION_GLOG_DEVICE_LOG_FUNC(5)
VISION_GLOG_DEVICE_LOG_FUNC(6)
VISION_GLOG_DEVICE_LOG_FUNC(7)
VISION_GLOG_DEVICE_LOG_FUNC(8)
VISION_GLOG_DEVICE_LOG_FUNC(9)
VISION_GLOG_DEVICE_LOG_FUNC(10)
VISION_GLOG_DEVICE_LOG_FUNC(11)
VISION_GLOG_DEVICE_LOG_FUNC(12)
VISION_GLOG_DEVICE_LOG_FUNC(13)
VISION_GLOG_DEVICE_LOG_FUNC(14)
VISION_GLOG_DEVICE_LOG_FUNC(15)
VISION_GLOG_DEVICE_LOG_FUNC(16)
VISION_GLOG_DEVICE_LOG_FUNC(17)
VISION_GLOG_DEVICE_LOG_FUNC(18)
VISION_GLOG_DEVICE_LOG_FUNC(19)
VISION_GLOG_DEVICE_LOG_FUNC(20)
VISION_GLOG_DEVICE_LOG_FUNC(21)
VISION_GLOG_DEVICE_LOG_FUNC(22)
VISION_GLOG_DEVICE_LOG_FUNC(23)
VISION_GLOG_DEVICE_LOG_FUNC(24)
VISION_GLOG_DEVICE_LOG_FUNC(25)
VISION_GLOG_DEVICE_LOG_FUNC(26)
VISION_GLOG_DEVICE_LOG_FUNC(27)
VISION_GLOG_DEVICE_LOG_FUNC(28)
VISION_GLOG_DEVICE_LOG_FUNC(29)
VISION_GLOG_DEVICE_LOG_FUNC(30)
VISION_GLOG_DEVICE_LOG_FUNC(31)
VISION_GLOG_DEVICE_LOG_FUNC(32)
VISION_GLOG_DEVICE_LOG_FUNC(33)
VISION_GLOG_DEVICE_LOG_FUNC(34)
VISION_GLOG_DEVICE_LOG_FUNC(35)
VISION_GLOG_DEVICE_LOG_FUNC(36)
VISION_GLOG_DEVICE_LOG_FUNC(37)
VISION_GLOG_DEVICE_LOG_FUNC(38)
VISION_GLOG_DEVICE_LOG_FUNC(39)
VISION_GLOG_DEVICE_LOG_FUNC(40)
VISION_GLOG_DEVICE_LOG_FUNC(41)
VISION_GLOG_DEVICE_LOG_FUNC(42)
VISION_GLOG_DEVICE_LOG_FUNC(43)
VISION_GLOG_DEVICE_LOG_FUNC(44)
VISION_GLOG_DEVICE_LOG_FUNC(45)
VISION_GLOG_DEVICE_LOG_FUNC(46)
VISION_GLOG_DEVICE_LOG_FUNC(47)
VISION_GLOG_DEVICE_LOG_FUNC(48)
VISION_GLOG_DEVICE_LOG_FUNC(49)

// 异常告警日志函数 - 0-49
#define VISION_GLOG_ALARM_LOG_FUNC(num) \
    void log_alarm_func##num(const VisionAlarmLog& msg) { \
        const char* severity_str = ""; \
        switch (msg.severity) { \
            case 1: severity_str = "INFO"; break; \
            case 2: severity_str = "WARNING"; break; \
            case 3: severity_str = "ERROR"; break; \
            case 4: severity_str = "CRITICAL"; break; \
        } \
        LOG(ERROR) << "[ALARM] ts:" << msg.timestamp \
                  << ", id:" << msg.alarm_id \
                  << ", severity:" << severity_str \
                  << ", type:" << msg.alarm_type \
                  << ", msg:" << msg.alarm_message \
                  << ", code:" << msg.error_code; \
    }

VISION_GLOG_ALARM_LOG_FUNC(0)
VISION_GLOG_ALARM_LOG_FUNC(1)
VISION_GLOG_ALARM_LOG_FUNC(2)
VISION_GLOG_ALARM_LOG_FUNC(3)
VISION_GLOG_ALARM_LOG_FUNC(4)
VISION_GLOG_ALARM_LOG_FUNC(5)
VISION_GLOG_ALARM_LOG_FUNC(6)
VISION_GLOG_ALARM_LOG_FUNC(7)
VISION_GLOG_ALARM_LOG_FUNC(8)
VISION_GLOG_ALARM_LOG_FUNC(9)
VISION_GLOG_ALARM_LOG_FUNC(10)
VISION_GLOG_ALARM_LOG_FUNC(11)
VISION_GLOG_ALARM_LOG_FUNC(12)
VISION_GLOG_ALARM_LOG_FUNC(13)
VISION_GLOG_ALARM_LOG_FUNC(14)
VISION_GLOG_ALARM_LOG_FUNC(15)
VISION_GLOG_ALARM_LOG_FUNC(16)
VISION_GLOG_ALARM_LOG_FUNC(17)
VISION_GLOG_ALARM_LOG_FUNC(18)
VISION_GLOG_ALARM_LOG_FUNC(19)
VISION_GLOG_ALARM_LOG_FUNC(20)
VISION_GLOG_ALARM_LOG_FUNC(21)
VISION_GLOG_ALARM_LOG_FUNC(22)
VISION_GLOG_ALARM_LOG_FUNC(23)
VISION_GLOG_ALARM_LOG_FUNC(24)
VISION_GLOG_ALARM_LOG_FUNC(25)
VISION_GLOG_ALARM_LOG_FUNC(26)
VISION_GLOG_ALARM_LOG_FUNC(27)
VISION_GLOG_ALARM_LOG_FUNC(28)
VISION_GLOG_ALARM_LOG_FUNC(29)
VISION_GLOG_ALARM_LOG_FUNC(30)
VISION_GLOG_ALARM_LOG_FUNC(31)
VISION_GLOG_ALARM_LOG_FUNC(32)
VISION_GLOG_ALARM_LOG_FUNC(33)
VISION_GLOG_ALARM_LOG_FUNC(34)
VISION_GLOG_ALARM_LOG_FUNC(35)
VISION_GLOG_ALARM_LOG_FUNC(36)
VISION_GLOG_ALARM_LOG_FUNC(37)
VISION_GLOG_ALARM_LOG_FUNC(38)
VISION_GLOG_ALARM_LOG_FUNC(39)
VISION_GLOG_ALARM_LOG_FUNC(40)
VISION_GLOG_ALARM_LOG_FUNC(41)
VISION_GLOG_ALARM_LOG_FUNC(42)
VISION_GLOG_ALARM_LOG_FUNC(43)
VISION_GLOG_ALARM_LOG_FUNC(44)
VISION_GLOG_ALARM_LOG_FUNC(45)
VISION_GLOG_ALARM_LOG_FUNC(46)
VISION_GLOG_ALARM_LOG_FUNC(47)
VISION_GLOG_ALARM_LOG_FUNC(48)
VISION_GLOG_ALARM_LOG_FUNC(49)

// 生产统计日志函数 - 0-19
#define VISION_GLOG_PRODUCTION_LOG_FUNC(num) \
    void log_production_func##num(const VisionProductionLog& msg) { \
        LOG(INFO) << "[PRODUCTION] ts:" << msg.timestamp \
                  << ", shift:" << msg.shift_id \
                  << ", total:" << msg.total_count \
                  << ", pass:" << msg.pass_count \
                  << ", fail:" << msg.fail_count \
                  << ", rate:" << msg.pass_rate << "%" \
                  << ", cycle:" << msg.avg_cycle_time << "s" \
                  << ", throughput:" << msg.throughput << "/min"; \
    }

VISION_GLOG_PRODUCTION_LOG_FUNC(0)
VISION_GLOG_PRODUCTION_LOG_FUNC(1)
VISION_GLOG_PRODUCTION_LOG_FUNC(2)
VISION_GLOG_PRODUCTION_LOG_FUNC(3)
VISION_GLOG_PRODUCTION_LOG_FUNC(4)
VISION_GLOG_PRODUCTION_LOG_FUNC(5)
VISION_GLOG_PRODUCTION_LOG_FUNC(6)
VISION_GLOG_PRODUCTION_LOG_FUNC(7)
VISION_GLOG_PRODUCTION_LOG_FUNC(8)
VISION_GLOG_PRODUCTION_LOG_FUNC(9)
VISION_GLOG_PRODUCTION_LOG_FUNC(10)
VISION_GLOG_PRODUCTION_LOG_FUNC(11)
VISION_GLOG_PRODUCTION_LOG_FUNC(12)
VISION_GLOG_PRODUCTION_LOG_FUNC(13)
VISION_GLOG_PRODUCTION_LOG_FUNC(14)
VISION_GLOG_PRODUCTION_LOG_FUNC(15)
VISION_GLOG_PRODUCTION_LOG_FUNC(16)
VISION_GLOG_PRODUCTION_LOG_FUNC(17)
VISION_GLOG_PRODUCTION_LOG_FUNC(18)
VISION_GLOG_PRODUCTION_LOG_FUNC(19)

// ============================================================================
// 函数指针数组
// ============================================================================

typedef void (*fn_VisionImageLog_log_func)(const VisionImageLog& msg);
typedef void (*fn_VisionDetectionLog_log_func)(const VisionDetectionLog& msg);
typedef void (*fn_VisionDeviceLog_log_func)(const VisionDeviceLog& msg);
typedef void (*fn_VisionAlarmLog_log_func)(const VisionAlarmLog& msg);
typedef void (*fn_VisionProductionLog_log_func)(const VisionProductionLog& msg);

static fn_VisionImageLog_log_func log_image_funcs[] = {
    log_image_func0, log_image_func1, log_image_func2, log_image_func3, log_image_func4,
    log_image_func5, log_image_func6, log_image_func7, log_image_func8, log_image_func9,
    log_image_func10, log_image_func11, log_image_func12, log_image_func13, log_image_func14,
    log_image_func15, log_image_func16, log_image_func17, log_image_func18, log_image_func19,
    log_image_func20, log_image_func21, log_image_func22, log_image_func23, log_image_func24,
    log_image_func25, log_image_func26, log_image_func27, log_image_func28, log_image_func29,
    log_image_func30, log_image_func31, log_image_func32, log_image_func33, log_image_func34,
    log_image_func35, log_image_func36, log_image_func37, log_image_func38, log_image_func39,
    log_image_func40, log_image_func41, log_image_func42, log_image_func43, log_image_func44,
    log_image_func45, log_image_func46, log_image_func47, log_image_func48, log_image_func49
};
const int n_image_funcs = sizeof(log_image_funcs) / sizeof(log_image_funcs[0]);

static fn_VisionDetectionLog_log_func log_detection_funcs[] = {
    log_detection_func0, log_detection_func1, log_detection_func2, log_detection_func3, log_detection_func4,
    log_detection_func5, log_detection_func6, log_detection_func7, log_detection_func8, log_detection_func9,
    log_detection_func10, log_detection_func11, log_detection_func12, log_detection_func13, log_detection_func14,
    log_detection_func15, log_detection_func16, log_detection_func17, log_detection_func18, log_detection_func19,
    log_detection_func20, log_detection_func21, log_detection_func22, log_detection_func23, log_detection_func24,
    log_detection_func25, log_detection_func26, log_detection_func27, log_detection_func28, log_detection_func29,
    log_detection_func30, log_detection_func31, log_detection_func32, log_detection_func33, log_detection_func34,
    log_detection_func35, log_detection_func36, log_detection_func37, log_detection_func38, log_detection_func39,
    log_detection_func40, log_detection_func41, log_detection_func42, log_detection_func43, log_detection_func44,
    log_detection_func45, log_detection_func46, log_detection_func47, log_detection_func48, log_detection_func49
};
const int n_detection_funcs = sizeof(log_detection_funcs) / sizeof(log_detection_funcs[0]);

static fn_VisionDeviceLog_log_func log_device_funcs[] = {
    log_device_func0, log_device_func1, log_device_func2, log_device_func3, log_device_func4,
    log_device_func5, log_device_func6, log_device_func7, log_device_func8, log_device_func9,
    log_device_func10, log_device_func11, log_device_func12, log_device_func13, log_device_func14,
    log_device_func15, log_device_func16, log_device_func17, log_device_func18, log_device_func19,
    log_device_func20, log_device_func21, log_device_func22, log_device_func23, log_device_func24,
    log_device_func25, log_device_func26, log_device_func27, log_device_func28, log_device_func29,
    log_device_func30, log_device_func31, log_device_func32, log_device_func33, log_device_func34,
    log_device_func35, log_device_func36, log_device_func37, log_device_func38, log_device_func39,
    log_device_func40, log_device_func41, log_device_func42, log_device_func43, log_device_func44,
    log_device_func45, log_device_func46, log_device_func47, log_device_func48, log_device_func49
};
const int n_device_funcs = sizeof(log_device_funcs) / sizeof(log_device_funcs[0]);

static fn_VisionAlarmLog_log_func log_alarm_funcs[] = {
    log_alarm_func0, log_alarm_func1, log_alarm_func2, log_alarm_func3, log_alarm_func4,
    log_alarm_func5, log_alarm_func6, log_alarm_func7, log_alarm_func8, log_alarm_func9,
    log_alarm_func10, log_alarm_func11, log_alarm_func12, log_alarm_func13, log_alarm_func14,
    log_alarm_func15, log_alarm_func16, log_alarm_func17, log_alarm_func18, log_alarm_func19,
    log_alarm_func20, log_alarm_func21, log_alarm_func22, log_alarm_func23, log_alarm_func24,
    log_alarm_func25, log_alarm_func26, log_alarm_func27, log_alarm_func28, log_alarm_func29,
    log_alarm_func30, log_alarm_func31, log_alarm_func32, log_alarm_func33, log_alarm_func34,
    log_alarm_func35, log_alarm_func36, log_alarm_func37, log_alarm_func38, log_alarm_func39,
    log_alarm_func40, log_alarm_func41, log_alarm_func42, log_alarm_func43, log_alarm_func44,
    log_alarm_func45, log_alarm_func46, log_alarm_func47, log_alarm_func48, log_alarm_func49
};
const int n_alarm_funcs = sizeof(log_alarm_funcs) / sizeof(log_alarm_funcs[0]);

static fn_VisionProductionLog_log_func log_production_funcs[] = {
    log_production_func0, log_production_func1, log_production_func2, log_production_func3, log_production_func4,
    log_production_func5, log_production_func6, log_production_func7, log_production_func8, log_production_func9,
    log_production_func10, log_production_func11, log_production_func12, log_production_func13, log_production_func14,
    log_production_func15, log_production_func16, log_production_func17, log_production_func18, log_production_func19
};
const int n_production_funcs = sizeof(log_production_funcs) / sizeof(log_production_funcs[0]);

// ============================================================================
// Fixture 类
// ============================================================================

class VisionGlogFixture : public benchmark::Fixture {
public:
    VisionGlogFixture() {
        LoadTestLogs();
    }

    void SetUp(const benchmark::State &) {
        std::call_once(g_glog_vision_init_flag, []() {
            FLAGS_log_dir = "logs";
            FLAGS_logtostderr = false;
            FLAGS_alsologtostderr = false;
            FLAGS_max_log_size = 100;  // MB
            FLAGS_stop_logging_if_full_disk = true;

            google::InitGoogleLogging("vision_glog");
            LOG(INFO) << "Vision glog benchmark initialized";
        });
    }

    void LoadTestLogs() {
        if (!LoadVisionImageLogs(image_logs, "data/vision_image_logs.bin")) {
            fprintf(stderr, "Failed to load image logs\n");
            exit(EXIT_FAILURE);
        }
        if (!LoadVisionDetectionLogs(detection_logs, "data/vision_detection_logs.bin")) {
            fprintf(stderr, "Failed to load detection logs\n");
            exit(EXIT_FAILURE);
        }
        if (!LoadVisionDeviceLogs(device_logs, "data/vision_device_logs.bin")) {
            fprintf(stderr, "Failed to load device logs\n");
            exit(EXIT_FAILURE);
        }
        if (!LoadVisionAlarmLogs(alarm_logs, "data/vision_alarm_logs.bin")) {
            fprintf(stderr, "Failed to load alarm logs\n");
            exit(EXIT_FAILURE);
        }
        if (!LoadVisionProductionLogs(production_logs, "data/vision_production_logs.bin")) {
            fprintf(stderr, "Failed to load production logs\n");
            exit(EXIT_FAILURE);
        }
    }

    ~VisionGlogFixture() {
        google::ShutdownGoogleLogging();
    }

public:
    std::vector<VisionImageLog> image_logs;
    std::vector<VisionDetectionLog> detection_logs;
    std::vector<VisionDeviceLog> device_logs;
    std::vector<VisionAlarmLog> alarm_logs;
    std::vector<VisionProductionLog> production_logs;
};

// ============================================================================
// 注册 Benchmark
// ============================================================================

// 场景1: 图像采集日志 - 高频
BENCHMARK_DEFINE_F(VisionGlogFixture, image_acquisition)(benchmark::State & state) {
    static thread_local int idx = 0;
    const int n = n_image_funcs;
    for (auto _ : state) {
        log_image_funcs[idx % n](image_logs[idx]);
        idx = (idx + 1) % image_logs.size();
    }
}
BENCHMARK_REGISTER_F(VisionGlogFixture, image_acquisition)
    ->Threads(1)->MinTime(VISION_MIN_TIME);
BENCHMARK_REGISTER_F(VisionGlogFixture, image_acquisition)
    ->Threads(2)->MinTime(VISION_MIN_TIME);
BENCHMARK_REGISTER_F(VisionGlogFixture, image_acquisition)
    ->Threads(4)->MinTime(VISION_MIN_TIME);
BENCHMARK_REGISTER_F(VisionGlogFixture, image_acquisition)
    ->Threads(8)->MinTime(VISION_MIN_TIME);

// 场景2: 检测结果日志 - 实时
BENCHMARK_DEFINE_F(VisionGlogFixture, detection_result)(benchmark::State & state) {
    static thread_local int idx = 0;
    const int n = n_detection_funcs;
    for (auto _ : state) {
        log_detection_funcs[idx % n](detection_logs[idx]);
        idx = (idx + 1) % detection_logs.size();
    }
}
BENCHMARK_REGISTER_F(VisionGlogFixture, detection_result)
    ->Threads(1)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);
BENCHMARK_REGISTER_F(VisionGlogFixture, detection_result)
    ->Threads(2)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);
BENCHMARK_REGISTER_F(VisionGlogFixture, detection_result)
    ->Threads(4)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);

// 场景3: 设备状态日志 - 周期性
BENCHMARK_DEFINE_F(VisionGlogFixture, device_status)(benchmark::State & state) {
    static thread_local int idx = 0;
    const int n = n_device_funcs;
    for (auto _ : state) {
        log_device_funcs[idx % n](device_logs[idx]);
        idx = (idx + 1) % device_logs.size();
    }
}
BENCHMARK_REGISTER_F(VisionGlogFixture, device_status)
    ->Threads(1)->Iterations(VISION_ITER_COUNT)->Repetitions(VISION_REPEAT_COUNT);

// 场景4: 异常告警日志 - 偶发
BENCHMARK_DEFINE_F(VisionGlogFixture, alarm_log)(benchmark::State & state) {
    static thread_local int idx = 0;
    const int n = n_alarm_funcs;
    for (auto _ : state) {
        log_alarm_funcs[idx % n](alarm_logs[idx]);
        idx = (idx + 1) % alarm_logs.size();
    }
}
BENCHMARK_REGISTER_F(VisionGlogFixture, alarm_log)
    ->Threads(1)->Iterations(VISION_ITER_COUNT / 10)->Repetitions(VISION_REPEAT_COUNT);

// 场景5: 生产统计日志 - 周期性汇总
BENCHMARK_DEFINE_F(VisionGlogFixture, production_stats)(benchmark::State & state) {
    static thread_local int idx = 0;
    const int n = n_production_funcs;
    for (auto _ : state) {
        log_production_funcs[idx % n](production_logs[idx]);
        idx = (idx + 1) % production_logs.size();
    }
}
BENCHMARK_REGISTER_F(VisionGlogFixture, production_stats)
    ->Threads(1)->Iterations(VISION_ITER_COUNT / 20)->Repetitions(VISION_REPEAT_COUNT);

BENCHMARK_MAIN();
