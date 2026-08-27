#include "sensor_repository.hpp"
#include <stdexcept>
#include <sstream>

namespace sensor_service {
    namespace repositories {

        SensorRepository::SensorRepository(const std::string& connectionString)
            : connectionString_(connectionString) {
        }

        std::optional<models::SensorData> SensorRepository::getById(int id) const {
            try {
                pqxx::connection c(connectionString_);
                pqxx::work txn(c);
                pqxx::result r = txn.exec_params(
                    "SELECT id, sensor_id, timestamp, value, unit FROM sensor_data WHERE id = $1",
                    id
                );
                txn.commit();

                if (r.empty()) {
                    return std::nullopt;
                }

                auto row = r[0];
                return models::SensorData{
                    row["id"].as<int>(),
                    row["sensor_id"].as<std::string>(),
                    row["timestamp"].as<std::string>(),
                    row["value"].as<double>(),
                    row["unit"].as<std::string>()
                };
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Database error in getById: " + std::string(e.what()));
            }
        }

        std::vector<models::SensorData> SensorRepository::getAll() const {
            std::vector<models::SensorData> results;
            try {
                pqxx::connection c(connectionString_);
                pqxx::work txn(c);
                pqxx::result r = txn.exec("SELECT id, sensor_id, timestamp, value, unit FROM sensor_data");
                txn.commit();

                for (const auto& row : r) {
                    results.push_back(models::SensorData{
                        row["id"].as<int>(),
                        row["sensor_id"].as<std::string>(),
                        row["timestamp"].as<std::string>(),
                        row["value"].as<double>(),
                        row["unit"].as<std::string>()
                        });
                }
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Database error in getAll: " + std::string(e.what()));
            }
            return results;
        }

        int SensorRepository::insert(const models::SensorDataCreate& createDto) {
            try {
                pqxx::connection c(connectionString_);
                pqxx::work txn(c);
                pqxx::result r = txn.exec_params(
                    "INSERT INTO sensor_data (sensor_id, timestamp, value, unit) "
                    "VALUES ($1, NOW()::timestamptz, $2, $3) "
                    "RETURNING id",
                    createDto.sensor_id,
                    createDto.value,
                    createDto.unit
                );
                txn.commit();

                return r[0]["id"].as<int>();
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Database error in insert: " + std::string(e.what()));
            }
        }

        void SensorRepository::update(int id, const models::SensorData& data) {
            try {
                pqxx::connection c(connectionString_);
                pqxx::work txn(c);
                pqxx::result r = txn.exec_params(
                    "UPDATE sensor_data SET sensor_id = $1, timestamp = $2, value = $3, unit = $4 "
                    "WHERE id = $5",
                    data.sensor_id,
                    data.timestamp,
                    data.value,
                    data.unit,
                    id
                );
                txn.commit();

                if (r.affected_rows() == 0) {
                    throw std::runtime_error("No sensor data found with id " + std::to_string(id));
                }
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Database error in update: " + std::string(e.what()));
            }
        }

        void SensorRepository::remove(int id) {
            try {
                pqxx::connection c(connectionString_);
                pqxx::work txn(c);
                pqxx::result r = txn.exec_params(
                    "DELETE FROM sensor_data WHERE id = $1",
                    id
                );
                txn.commit();

                if (r.affected_rows() == 0) {
                    throw std::runtime_error("No sensor data found with id " + std::to_string(id));
                }
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Database error in remove: " + std::string(e.what()));
            }
        }

    }
}