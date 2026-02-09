#include "vision_log_msg.h"
#include <cstdio>
#include <cstring>
#include <random>
#include <chrono>

// 随机数生成器
static std::mt19937 g_rng(std::chrono::steady_clock::now().time_since_epoch().count());
static std::uniform_int_distribution<uint64_t> g_timestamp_dist(1000000000ULL, 9999999999ULL);

// ============================================================================
// 生成测试数据实现
// ============================================================================

void GenVisionImageLogs(uint32_t count, std::vector<VisionImageLog>& logs) {
    logs.clear();
    logs.reserve(count);

    std::uniform_int_distribution<uint32_t> camera_id_dist(1, 8);
    std::uniform_int_distribution<uint32_t> width_dist(1920, 4096);
    std::uniform_int_distribution<uint32_t> height_dist(1080, 2160);
    std::uniform_int_distribution<uint64_t> exposure_dist(100, 10000);
    std::uniform_real_distribution<float> gain_dist(0.0f, 12.0f);

    for (uint32_t i = 0; i < count; i++) {
        VisionImageLog log;
        log.timestamp = g_timestamp_dist(g_rng);
        log.camera_id = camera_id_dist(g_rng);
        log.image_id = i;
        log.width = width_dist(g_rng);
        log.height = height_dist(g_rng);
        log.exposure_time = exposure_dist(g_rng);
        log.gain = gain_dist(g_rng);
        strncpy(log.sensor_status, "OK", sizeof(log.sensor_status) - 1);

        logs.push_back(log);
    }
}

void GenVisionDetectionLogs(uint32_t count, std::vector<VisionDetectionLog>& logs) {
    logs.clear();
    logs.reserve(count);

    std::uniform_int_distribution<uint32_t> roi_dist(5, 50);
    std::uniform_int_distribution<uint32_t> defect_dist(0, 10);
    std::uniform_real_distribution<float> confidence_dist(0.85f, 0.99f);
    std::uniform_real_distribution<float> processing_dist(5.0f, 50.0f);

    const char* models[] = {"ResNet50", "YOLOv8", "EfficientNet", "CustomNet"};
    const char* statuses[] = {"PASS", "FAIL", "REVIEW", "UNKNOWN"};

    for (uint32_t i = 0; i < count; i++) {
        VisionDetectionLog log;
        log.timestamp = g_timestamp_dist(g_rng);
        log.image_id = i;
        log.roi_count = roi_dist(g_rng);
        log.defect_count = defect_dist(g_rng);
        log.confidence = confidence_dist(g_rng);
        strncpy(log.model_name, models[i % 4], sizeof(log.model_name) - 1);
        log.processing_time = processing_dist(g_rng);
        strncpy(log.result_status, statuses[i % 4], sizeof(log.result_status) - 1);

        logs.push_back(log);
    }
}

void GenVisionDeviceLogs(uint32_t count, std::vector<VisionDeviceLog>& logs) {
    logs.clear();
    logs.reserve(count);

    std::uniform_int_distribution<uint32_t> device_id_dist(1, 20);
    std::uniform_int_distribution<uint32_t> status_code_dist(0, 5);
    std::uniform_real_distribution<float> temp_dist(20.0f, 80.0f);
    std::uniform_real_distribution<float> voltage_dist(220.0f, 240.0f);
    std::uniform_real_distribution<float> current_dist(0.5f, 15.0f);

    const char* device_types[] = {"CAMERA", "LIGHTING", "PLC", "SENSOR", "ACTUATOR"};
    const char* modes[] = {"AUTO", "MANUAL", "MAINTENANCE", "EMERGENCY"};

    for (uint32_t i = 0; i < count; i++) {
        VisionDeviceLog log;
        log.timestamp = g_timestamp_dist(g_rng);
        log.device_id = device_id_dist(g_rng);
        strncpy(log.device_type, device_types[i % 5], sizeof(log.device_type) - 1);
        log.status_code = status_code_dist(g_rng);
        log.temperature = temp_dist(g_rng);
        log.voltage = voltage_dist(g_rng);
        log.current = current_dist(g_rng);
        strncpy(log.operation_mode, modes[i % 4], sizeof(log.operation_mode) - 1);

        logs.push_back(log);
    }
}

void GenVisionAlarmLogs(uint32_t count, std::vector<VisionAlarmLog>& logs) {
    logs.clear();
    logs.reserve(count);

    std::uniform_int_distribution<uint32_t> alarm_id_dist(1000, 9999);
    std::uniform_int_distribution<uint32_t> severity_dist(1, 4);
    std::uniform_int_distribution<uint32_t> camera_id_dist(1, 8);
    std::uniform_int_distribution<uint32_t> error_code_dist(0, 9999);

    const char* alarm_types[] = {
        "IMAGE_TIMEOUT",
        "DETECTION_FAILED",
        "SENSOR_DISCONNECTED",
        "OVER_TEMPERATURE",
        "LOW_LIGHT",
        "FOCUS_DRIFT",
        "COMMUNICATION_ERROR",
        "PLC_TIMEOUT"
    };

    const char* messages[] = {
        "Camera %u failed to acquire image within timeout",
        "Detection model failed to process image %u",
        "Sensor %u disconnected from system",
        "Temperature exceeded threshold: %.1f°C",
        "Light intensity too low: %.2f lux",
        "Auto-focus system failed to converge",
        "Communication with PLC device lost",
        "PLC response timeout after %u ms"
    };

    for (uint32_t i = 0; i < count; i++) {
        VisionAlarmLog log;
        log.timestamp = g_timestamp_dist(g_rng);
        log.alarm_id = alarm_id_dist(g_rng);
        log.severity = severity_dist(g_rng);
        log.camera_id = camera_id_dist(g_rng);
        strncpy(log.alarm_type, alarm_types[i % 8], sizeof(log.alarm_type) - 1);

        // 生成告警消息
        uint32_t msg_idx = i % 8;
        char temp_msg[512];
        switch (msg_idx) {
            case 0:
                snprintf(temp_msg, sizeof(temp_msg), messages[0], log.camera_id);
                break;
            case 1:
                snprintf(temp_msg, sizeof(temp_msg), messages[1], i);
                break;
            case 2:
                snprintf(temp_msg, sizeof(temp_msg), messages[2], i);
                break;
            case 3:
                snprintf(temp_msg, sizeof(temp_msg), messages[3], 75.0f + i % 10);
                break;
            case 4:
                snprintf(temp_msg, sizeof(temp_msg), messages[4], 100.0f + i % 50);
                break;
            case 5:
                snprintf(temp_msg, sizeof(temp_msg), "%s", messages[5]);
                break;
            case 6:
                snprintf(temp_msg, sizeof(temp_msg), "%s", messages[6]);
                break;
            case 7:
                snprintf(temp_msg, sizeof(temp_msg), messages[7], 5000 + i % 100);
                break;
        }
        strncpy(log.alarm_message, temp_msg, sizeof(log.alarm_message) - 1);
        log.error_code = error_code_dist(g_rng);

        logs.push_back(log);
    }
}

void GenVisionProductionLogs(uint32_t count, std::vector<VisionProductionLog>& logs) {
    logs.clear();
    logs.reserve(count);

    std::uniform_int_distribution<uint32_t> shift_id_dist(1, 3);
    std::uniform_int_distribution<uint32_t> total_dist(1000, 10000);
    std::uniform_real_distribution<float> rate_dist(95.0f, 99.9f);
    std::uniform_real_distribution<float> cycle_dist(0.5f, 3.0f);
    std::uniform_real_distribution<float> throughput_dist(20.0f, 120.0f);

    for (uint32_t i = 0; i < count; i++) {
        VisionProductionLog log;
        log.timestamp = g_timestamp_dist(g_rng);
        log.shift_id = shift_id_dist(g_rng);
        log.total_count = total_dist(g_rng);
        float pass_rate = rate_dist(g_rng);
        log.pass_count = static_cast<uint32_t>(log.total_count * pass_rate / 100.0f);
        log.fail_count = log.total_count - log.pass_count;
        log.pass_rate = pass_rate;
        log.avg_cycle_time = cycle_dist(g_rng);
        log.throughput = throughput_dist(g_rng);

        logs.push_back(log);
    }
}

// ============================================================================
// 序列化/反序列化实现
// ============================================================================

#define DUMPS_IMPL(type, logs, filename) \
    do { \
        FILE* fp = fopen(filename, "wb"); \
        if (!fp) return false; \
        size_t count = logs.size(); \
        fwrite(&count, sizeof(count), 1, fp); \
        fwrite(logs.data(), sizeof(type), count, fp); \
        fclose(fp); \
        return true; \
    } while(0)

#define LOAD_IMPL(type, logs, filename) \
    do { \
        FILE* fp = fopen(filename, "rb"); \
        if (!fp) return false; \
        size_t count = 0; \
        fread(&count, sizeof(count), 1, fp); \
        logs.resize(count); \
        fread(logs.data(), sizeof(type), count, fp); \
        fclose(fp); \
        return true; \
    } while(0)

bool DumpsVisionImageLogs(const std::vector<VisionImageLog>& logs, const char* filename) {
    DUMPS_IMPL(VisionImageLog, logs, filename);
}

bool LoadVisionImageLogs(std::vector<VisionImageLog>& logs, const char* filename) {
    LOAD_IMPL(VisionImageLog, logs, filename);
}

bool DumpsVisionDetectionLogs(const std::vector<VisionDetectionLog>& logs, const char* filename) {
    DUMPS_IMPL(VisionDetectionLog, logs, filename);
}

bool LoadVisionDetectionLogs(std::vector<VisionDetectionLog>& logs, const char* filename) {
    LOAD_IMPL(VisionDetectionLog, logs, filename);
}

bool DumpsVisionDeviceLogs(const std::vector<VisionDeviceLog>& logs, const char* filename) {
    DUMPS_IMPL(VisionDeviceLog, logs, filename);
}

bool LoadVisionDeviceLogs(std::vector<VisionDeviceLog>& logs, const char* filename) {
    LOAD_IMPL(VisionDeviceLog, logs, filename);
}

bool DumpsVisionAlarmLogs(const std::vector<VisionAlarmLog>& logs, const char* filename) {
    DUMPS_IMPL(VisionAlarmLog, logs, filename);
}

bool LoadVisionAlarmLogs(std::vector<VisionAlarmLog>& logs, const char* filename) {
    LOAD_IMPL(VisionAlarmLog, logs, filename);
}

bool DumpsVisionProductionLogs(const std::vector<VisionProductionLog>& logs, const char* filename) {
    DUMPS_IMPL(VisionProductionLog, logs, filename);
}

bool LoadVisionProductionLogs(std::vector<VisionProductionLog>& logs, const char* filename) {
    LOAD_IMPL(VisionProductionLog, logs, filename);
}
