#include <crow.h>
#include <iostream>
#include <cstdlib>
#include <memory>
#include "services/sensor_service.hpp"
#include "repositories/sensor_repository.hpp"
#include "models/sensor_data.hpp"
#include "models/sensor_data_create.hpp"

using namespace sensor_service;

std::string getEnvOrDefault(const char* name, const std::string& defaultValue) {
    const char* value = std::getenv(name);
    return value ? std::string(value) : defaultValue;
}

int main() {
    try {
        std::string host = getEnvOrDefault("POSTGRES_HOST", "localhost");
        std::string port = getEnvOrDefault("POSTGRES_PORT", "5432");
        std::string db = getEnvOrDefault("POSTGRES_DB", "sensors");
        std::string user = getEnvOrDefault("POSTGRES_USER", "postgres");
        std::string password = getEnvOrDefault("POSTGRES_PASSWORD", "antonio");

        std::string connectionString = "host=" + host + " port=" + port +
            " dbname=" + db + " user=" + user +
            " password=" + password;

        auto repository = std::make_shared<repositories::SensorRepository>(connectionString);
        auto service = std::make_shared<services::SensorService>(repository);

        crow::SimpleApp app;

        CROW_ROUTE(app, "/")
            ([]() {
            return "Sensor Service is running";
                });

        CROW_ROUTE(app, "/api/sensors").methods("POST"_method)
            ([service](const crow::request& req) {
            try {
                auto data = crow::json::load(req.body);
                if (!data) {
                    return crow::response(400, "Invalid JSON");
                }

                std::string sensor_id = data["sensor_id"].s();
                double value = data["value"].d();
                std::string unit = data.has("unit") ? std::string(data["unit"].s()) : std::string("");

                models::SensorDataCreate createDto{ sensor_id, value, unit };
                int id = service->create(createDto);

                crow::json::wvalue result;
                result["id"] = id;
                return crow::response(201, result);
            }
            catch (const std::invalid_argument& e) {
                return crow::response(400, e.what());
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Internal server error: ") + e.what());
            }
                });

        CROW_ROUTE(app, "/api/sensors/<int>")
            ([service](int id) {
            try {
                auto sensor = service->getById(id);
                if (!sensor) {
                    return crow::response(404, "Sensor not found");
                }

                crow::json::wvalue result;
                result["id"] = sensor->id;
                result["sensor_id"] = sensor->sensor_id;
                result["timestamp"] = sensor->timestamp;
                result["value"] = sensor->value;
                result["unit"] = sensor->unit;
                return crow::response(200, result);
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Internal server error: ") + e.what());
            }
                });

        CROW_ROUTE(app, "/api/sensors").methods("GET"_method)
            ([service]() {
            try {
                auto sensors = service->getAll();

                std::vector<crow::json::wvalue> items;
                for (const auto& s : sensors) {
                    crow::json::wvalue item;
                    item["id"] = s.id;
                    item["sensor_id"] = s.sensor_id;
                    item["timestamp"] = s.timestamp;
                    item["value"] = s.value;
                    item["unit"] = s.unit;
                    items.push_back(std::move(item));
                }

                crow::json::wvalue result(items);
                return crow::response(200, result);
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Internal server error: ") + e.what());
            }
                });

        CROW_ROUTE(app, "/api/sensors/<int>").methods("PUT"_method)
            ([service](const crow::request& req, int id) {
            try {
                auto data = crow::json::load(req.body);
                if (!data) {
                    return crow::response(400, "Invalid JSON");
                }

                auto existing = service->getById(id);
                if (!existing) {
                    return crow::response(404, "Sensor not found");
                }

                std::string sensor_id = data["sensor_id"].s();
                double value = data["value"].d();
                std::string unit = data.has("unit") ? std::string(data["unit"].s()) : std::string("");

                models::SensorData updateDto{
                    id,
                    sensor_id,
                    existing->timestamp,
                    value,
                    unit
                };

                service->update(id, updateDto);

                return crow::response(200, "Sensor updated");
            }
            catch (const std::invalid_argument& e) {
                return crow::response(400, e.what());
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Internal server error: ") + e.what());
            }
                });

        CROW_ROUTE(app, "/api/sensors/<int>").methods("DELETE"_method)
            ([service](int id) {
            try {
                service->remove(id);
                return crow::response(200, "Sensor deleted");
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Internal server error: ") + e.what());
            }
                });

        int serverPort = std::stoi(getEnvOrDefault("SERVER_PORT", "8080"));

        app.port(serverPort)
            .multithreaded()
            .run();

    }
    catch (const std::exception& e) {
        std::cerr << "Failed to start application: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}