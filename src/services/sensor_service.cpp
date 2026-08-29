#include "sensor_service.hpp"
#include <stdexcept>
#include <memory>

namespace sensor_service {
    namespace services {

        SensorService::SensorService(std::shared_ptr<repositories::ISensorRepository> repository)
            : repository_(std::move(repository)) {
        }

        std::optional<models::SensorData> SensorService::getById(int id) const {
            return repository_->getById(id);
        }

        std::vector<models::SensorData> SensorService::getAll() const {
            return repository_->getAll();
        }

        int SensorService::create(const models::SensorDataCreate& createDto) {
            validateCreateDto(createDto);
            return repository_->insert(createDto);
        }

        void SensorService::update(int id, const models::SensorData& data) {
            validateData(data);
            repository_->update(id, data);
        }

        void SensorService::remove(int id) {
            repository_->remove(id);
        }

        void SensorService::validateCreateDto(const models::SensorDataCreate& dto) const {
            if (dto.sensor_id.empty()) {
                throw std::invalid_argument("sensor_id cannot be empty");
            }
        }

        void SensorService::validateData(const models::SensorData& data) const {
            validateCreateDto({ data.sensor_id, data.value, data.unit });
            if (data.id <= 0) {
                throw std::invalid_argument("ID must be positive");
            }
        }

    }
}