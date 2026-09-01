#pragma once
#include <string>
#include <ostream>

namespace sensor_service {
    namespace models {
        struct SensorData {
            int id;
            std::string sensor_id;
            std::string timestamp;
            double value;
            std::string unit;

            bool operator==(const SensorData& other) const {
                return id == other.id && sensor_id == other.sensor_id &&
                    timestamp == other.timestamp && value == other.value && unit == other.unit;
            }

            friend std::ostream& operator<<(std::ostream& os, const SensorData& obj) {
                return os << "{ id: " << obj.id << ", sensor_id: " << obj.sensor_id
                    << ", timestamp: " << obj.timestamp << ", value: " << obj.value
                    << ", unit: " << obj.unit << " }";
            }
        };
    }
}