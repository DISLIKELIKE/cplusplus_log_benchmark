#include <iostream>
#include <cstdio>
#include <string>
#include <sys/stat.h>

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
#endif

#include "../gbenchmark/vision_log_msg.h"

int main() {
    printf("Generating vision production line log test data...\n");

    // 创建 data 目录
    #ifdef _WIN32
        _mkdir("data");
    #else
        mkdir("data", 0755);
    #endif

    // 生成各种类型的测试数据
    std::vector<VisionImageLog> image_logs;
    GenVisionImageLogs(VISION_IMAGE_LOG_COUNT, image_logs);
    DumpsVisionImageLogs(image_logs, "data/vision_image_logs.bin");
    printf("Generated %u image logs\n", (uint32_t)image_logs.size());

    std::vector<VisionDetectionLog> detection_logs;
    GenVisionDetectionLogs(VISION_DETECTION_LOG_COUNT, detection_logs);
    DumpsVisionDetectionLogs(detection_logs, "data/vision_detection_logs.bin");
    printf("Generated %u detection logs\n", (uint32_t)detection_logs.size());

    std::vector<VisionDeviceLog> device_logs;
    GenVisionDeviceLogs(VISION_DEVICE_LOG_COUNT, device_logs);
    DumpsVisionDeviceLogs(device_logs, "data/vision_device_logs.bin");
    printf("Generated %u device logs\n", (uint32_t)device_logs.size());

    std::vector<VisionAlarmLog> alarm_logs;
    GenVisionAlarmLogs(VISION_ALARM_LOG_COUNT, alarm_logs);
    DumpsVisionAlarmLogs(alarm_logs, "data/vision_alarm_logs.bin");
    printf("Generated %u alarm logs\n", (uint32_t)alarm_logs.size());

    std::vector<VisionProductionLog> production_logs;
    GenVisionProductionLogs(VISION_PRODUCTION_LOG_COUNT, production_logs);
    DumpsVisionProductionLogs(production_logs, "data/vision_production_logs.bin");
    printf("Generated %u production logs\n", (uint32_t)production_logs.size());

    printf("\nAll test data generated successfully in 'data' directory.\n");
    printf("\nYou can now run the benchmarks:\n");
    printf("  - build/bin/gbenchmark_vision_spdlog\n");
    printf("  - build/bin/gbenchmark_vision_glog\n");

    return 0;
}
