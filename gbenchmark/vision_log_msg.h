#ifndef MACHINE_VISION_LOG_MSG_H_
#define MACHINE_VISION_LOG_MSG_H_

#include <stdint.h>
#include <string>
#include <vector>

// ============================================================================
// 机器视觉生产线日志消息类型
// ============================================================================

// 图像采集日志
struct VisionImageLog {
    uint64_t timestamp;           // 时间戳(微秒)
    uint32_t camera_id;          // 相机ID
    uint32_t image_id;           // 图像ID
    uint32_t width;              // 图像宽度
    uint32_t height;             // 图像高度
    uint64_t exposure_time;      // 曝光时间(微秒)
    float gain;                  // 增益
    char sensor_status[32];      // 传感器状态
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

// ============================================================================
// 生成测试数据
// ============================================================================

// 生成图像采集日志数据
void GenVisionImageLogs(uint32_t count, std::vector<VisionImageLog>& logs);

// 生成检测结果日志数据
void GenVisionDetectionLogs(uint32_t count, std::vector<VisionDetectionLog>& logs);

// 生成设备状态日志数据
void GenVisionDeviceLogs(uint32_t count, std::vector<VisionDeviceLog>& logs);

// 生成异常告警日志数据
void GenVisionAlarmLogs(uint32_t count, std::vector<VisionAlarmLog>& logs);

// 生成生产统计日志数据
void GenVisionProductionLogs(uint32_t count, std::vector<VisionProductionLog>& logs);

// ============================================================================
// 序列化/反序列化
// ============================================================================

// 保存到文件
bool DumpsVisionImageLogs(const std::vector<VisionImageLog>& logs, const char* filename);
bool DumpsVisionDetectionLogs(const std::vector<VisionDetectionLog>& logs, const char* filename);
bool DumpsVisionDeviceLogs(const std::vector<VisionDeviceLog>& logs, const char* filename);
bool DumpsVisionAlarmLogs(const std::vector<VisionAlarmLog>& logs, const char* filename);
bool DumpsVisionProductionLogs(const std::vector<VisionProductionLog>& logs, const char* filename);

// 从文件加载
bool LoadVisionImageLogs(std::vector<VisionImageLog>& logs, const char* filename);
bool LoadVisionDetectionLogs(std::vector<VisionDetectionLog>& logs, const char* filename);
bool LoadVisionDeviceLogs(std::vector<VisionDeviceLog>& logs, const char* filename);
bool LoadVisionAlarmLogs(std::vector<VisionAlarmLog>& logs, const char* filename);
bool LoadVisionProductionLogs(std::vector<VisionProductionLog>& logs, const char* filename);

#endif // !MACHINE_VISION_LOG_MSG_H_
