#pragma once
#include <string>

namespace sensor_service {
    namespace models {
        struct SensorDataCreate {
            std::string sensor_id;  // Required: unique sensor identifier
            double value;           // Required: sensor reading value
            std::string unit;       // Optional: measurement unit (empty string if none)
        };

    }
}