#pragma once
#include <string>

namespace sensor_service {
    namespace models {
        struct SensorData {
            int id;                         // Database-generated primary key
            std::string sensor_id;          // Unique sensor identifier
            std::string timestamp;          // ISO 8601 timestamp
            double value;                   // Sensor reading value
            std::string unit;               // Measurement unit (e.g., "Celsius", "V");
        };

    }
}