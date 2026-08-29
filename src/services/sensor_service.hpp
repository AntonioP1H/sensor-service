#pragma once
#include "isensor_service.hpp"
#include "../repositories/isensor_repository.hpp"
#include "../models/sensor_data.hpp"
#include "../models/sensor_data_create.hpp"
#include <stdexcept>
#include <memory>

namespace sensor_service {
    namespace services {
        class SensorService : public ISensorService {
        public:
            explicit SensorService(std::shared_ptr<repositories::ISensorRepository> repository);
            std::optional<models::SensorData> getById(int id) const override;
            std::vector<models::SensorData> getAll() const override;
            int create(const models::SensorDataCreate& createDto) override;
            void update(int id, const models::SensorData& data) override;
            virtual void remove(int id) override;

        private:
            void validateCreateDto(const models::SensorDataCreate& dto) const;
            void validateData(const models::SensorData& data) const;

            std::shared_ptr<repositories::ISensorRepository> repository_;
        };

    }
}