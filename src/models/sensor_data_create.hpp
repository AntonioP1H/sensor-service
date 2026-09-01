#pragma once
#include <string>
#include <ostream>

namespace sensor_service {
    namespace models {
        struct SensorDataCreate {
            std::string sensor_id;
            double value;
            std::string unit;

            bool operator==(const SensorDataCreate& other) const {
                return sensor_id == other.sensor_id && value == other.value && unit == other.unit;
            }

            friend std::ostream& operator<<(std::ostream& os, const SensorDataCreate& obj) {
                return os << "{ sensor_id: " << obj.sensor_id << ", value: " << obj.value << ", unit: " << obj.unit << " }";
            }
        };
    }
}