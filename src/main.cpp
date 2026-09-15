#include <crow.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include "services/sensor_service.hpp"
#include "repositories/sensor_repository.hpp"
#include "models/sensor_data.hpp"
#include "models/sensor_data_create.hpp"

using namespace sensor_service;

std::string getEnvOrDefault(
    const char* name,
    const std::string& defaultValue
) {
    const char* value = std::getenv(name);

    return value
        ? std::string(value)
        : defaultValue;
}

int main() {
    try {

        const std::string host =
            getEnvOrDefault(
                "POSTGRES_HOST",
                "localhost"
            );

        const std::string port =
            getEnvOrDefault(
                "POSTGRES_PORT",
                "5432"
            );

        const std::string db =
            getEnvOrDefault(
                "POSTGRES_DB",
                "sensors"
            );

        const std::string user =
            getEnvOrDefault(
                "POSTGRES_USER",
                "postgres"
            );

        const std::string password =
            getEnvOrDefault(
                "POSTGRES_PASSWORD",
                "postgres"
            );


        const std::string connectionString =
            "host=" + host +
            " port=" + port +
            " dbname=" + db +
            " user=" + user +
            " password=" + password;

        auto repository =
            std::make_shared<
            repositories::SensorRepository
            >(connectionString);


        auto service =
            std::make_shared<
            services::SensorService
            >(repository);


        crow::SimpleApp app;

        CROW_ROUTE(app, "/")
            (
                []() {
                    return "Sensor Service is running";
                }
                );

        CROW_ROUTE(app, "/api/sensors")
            .methods("POST"_method)
            (
                [service](const crow::request& req) {

                    try {

                        auto data =
                            crow::json::load(req.body);


                        if (!data) {
                            return crow::response(
                                400,
                                "Invalid JSON"
                            );
                        }

                        if (!data.has("sensor_id")) {
                            return crow::response(
                                400,
                                "Missing required field: sensor_id"
                            );
                        }

                        if (!data.has("value")) {
                            return crow::response(
                                400,
                                "Missing required field: value"
                            );
                        }


                        std::string sensor_id =
                            std::string(
                                data["sensor_id"].s()
                            );


                        double value =
                            data["value"].d();


                        std::string unit =
                            data.has("unit")
                            ? std::string(
                                data["unit"].s()
                            )
                            : std::string("");


                        models::SensorDataCreate createDto{
                            sensor_id,
                            value,
                            unit
                        };


                        int id =
                            service->create(
                                createDto
                            );


                        crow::json::wvalue result;

                        result["id"] = id;


                        return crow::response(
                            201,
                            result
                        );
                    }

                    catch (const std::invalid_argument& e) {
                        return crow::response(
                            400,
                            e.what()
                        );
                    }

                    catch (const std::exception& e) {
                        return crow::response(
                            500,
                            std::string(
                                "Internal server error: "
                            ) + e.what()
                        );
                    }
                }
                );

        CROW_ROUTE(app, "/api/sensors/<int>")
            (
                [service](int id) {

                    try {

                        auto sensor =
                            service->getById(id);


                        if (!sensor) {
                            return crow::response(
                                404,
                                "Sensor not found"
                            );
                        }


                        crow::json::wvalue result;

                        result["id"] =
                            sensor->id;

                        result["sensor_id"] =
                            sensor->sensor_id;

                        result["timestamp"] =
                            sensor->timestamp;

                        result["value"] =
                            sensor->value;

                        result["unit"] =
                            sensor->unit;


                        return crow::response(
                            200,
                            result
                        );
                    }

                    catch (const std::exception& e) {
                        return crow::response(
                            500,
                            std::string(
                                "Internal server error: "
                            ) + e.what()
                        );
                    }
                }
                );

        CROW_ROUTE(app, "/api/sensors")
            .methods("GET"_method)
            (
                [service]() {

                    try {

                        auto sensors =
                            service->getAll();


                        std::vector<
                            crow::json::wvalue
                        > items;


                        items.reserve(
                            sensors.size()
                        );


                        for (const auto& sensor : sensors) {

                            crow::json::wvalue item;

                            item["id"] =
                                sensor.id;

                            item["sensor_id"] =
                                sensor.sensor_id;

                            item["timestamp"] =
                                sensor.timestamp;

                            item["value"] =
                                sensor.value;

                            item["unit"] =
                                sensor.unit;


                            items.push_back(
                                std::move(item)
                            );
                        }


                        crow::json::wvalue result(
                            items
                        );


                        return crow::response(
                            200,
                            result
                        );
                    }

                    catch (const std::exception& e) {
                        return crow::response(
                            500,
                            std::string(
                                "Internal server error: "
                            ) + e.what()
                        );
                    }
                }
                );


        CROW_ROUTE(app, "/api/sensors/<int>")
            .methods("PUT"_method)
            (
                [service](
                    const crow::request& req,
                    int id
                    ) {

                        try {

                            auto data =
                                crow::json::load(req.body);


                            if (!data) {
                                return crow::response(
                                    400,
                                    "Invalid JSON"
                                );
                            }


                            if (!data.has("sensor_id")) {
                                return crow::response(
                                    400,
                                    "Missing required field: sensor_id"
                                );
                            }

                            if (!data.has("value")) {
                                return crow::response(
                                    400,
                                    "Missing required field: value"
                                );
                            }


                            auto existing =
                                service->getById(id);


                            if (!existing) {
                                return crow::response(
                                    404,
                                    "Sensor not found"
                                );
                            }


                            std::string sensor_id =
                                std::string(
                                    data["sensor_id"].s()
                                );


                            double value =
                                data["value"].d();


                            std::string unit =
                                data.has("unit")
                                ? std::string(
                                    data["unit"].s()
                                )
                                : std::string("");


                            models::SensorData updateDto{
                                id,
                                sensor_id,
                                existing->timestamp,
                                value,
                                unit
                            };


                            service->update(
                                id,
                                updateDto
                            );


                            return crow::response(
                                200,
                                "Sensor updated"
                            );
                        }

                        catch (const std::invalid_argument& e) {
                            return crow::response(
                                400,
                                e.what()
                            );
                        }

                        catch (const std::exception& e) {
                            return crow::response(
                                500,
                                std::string(
                                    "Internal server error: "
                                ) + e.what()
                            );
                        }
                }
                );

        CROW_ROUTE(app, "/api/sensors/<int>")
            .methods("DELETE"_method)
            (
                [service](int id) {

                    try {

                        service->remove(id);


                        return crow::response(
                            200,
                            "Sensor deleted"
                        );
                    }

                    catch (const std::exception& e) {
                        return crow::response(
                            500,
                            std::string(
                                "Internal server error: "
                            ) + e.what()
                        );
                    }
                }
                );
            
        const int serverPort =
            std::stoi(
                getEnvOrDefault(
                    "SERVER_PORT",
                    "8080"
                )
            );


        app
            .port(serverPort)
            .multithreaded()
            .run();
    }

    catch (const std::exception& e) {

        std::cerr
            << "Failed to start application: "
            << e.what()
            << std::endl;

        return 1;
    }


    return 0;
}