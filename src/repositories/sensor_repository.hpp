#pragma once
#include <pqxx/pqxx>
#include <vector>
#include <optional>
#include "../models/sensor_data.hpp"
#include "../models/sensor_data_create.hpp"
#include "isensor_repository.hpp"

namespace sensor_service {
    namespace repositories {
        class SensorRepository : public ISensorRepository {
        public:
            explicit SensorRepository(const std::string& connectionString);
            std::optional<models::SensorData> getById(int id) const override;
            std::vector<models::SensorData> getAll() const override;
            int insert(const models::SensorDataCreate& createDto) override;
            void update(int id, const models::SensorData& data) override;
            void remove(int id) override;
        private:
            std::string connectionString_;
        };
    }
}