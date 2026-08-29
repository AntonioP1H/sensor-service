#pragma once
#include <vector>
#include <optional>
#include "../models/sensor_data.hpp"
#include "../models/sensor_data_create.hpp"

namespace sensor_service {
    namespace services {
        class ISensorService {
        public:
            virtual ~ISensorService() = default;
            virtual std::optional<models::SensorData> getById(int id) const = 0;
            virtual std::vector<models::SensorData> getAll() const = 0;
            virtual int create(const models::SensorDataCreate& createDto) = 0;
            virtual void update(int id, const models::SensorData& data) = 0;
            virtual void remove(int id) = 0;
        };

    }
}