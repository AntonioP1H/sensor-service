#include <gtest/gtest.h>
#include <pqxx/pqxx>
#include <memory>
#include <string>
#include "../src/repositories/sensor_repository.hpp"
#include "../src/models/sensor_data.hpp"
#include "../src/models/sensor_data_create.hpp"

using namespace sensor_service::repositories;
using namespace sensor_service::models;

std::string getEnvOrDefault(const char* name, const std::string& defaultValue) {
    const char* value = std::getenv(name);
    return value ? std::string(value) : defaultValue;
}

class SensorRepositoryIntegrationTest : public ::testing::Test {
protected:
    std::shared_ptr<SensorRepository> repository;

    void SetUp() override {
        std::string host = getEnvOrDefault("POSTGRES_HOST", "localhost");
        std::string port = getEnvOrDefault("POSTGRES_PORT", "5432");
        std::string db = getEnvOrDefault("POSTGRES_DB", "sensors") + "_test";
        std::string user = getEnvOrDefault("POSTGRES_USER", "postgres");
        std::string password = getEnvOrDefault("POSTGRES_PASSWORD", "postgres");

        std::string connectionString = "host=" + host + " port=" + port +
            " dbname=" + db + " user=" + user +
            " password=" + password;

        repository = std::make_shared<SensorRepository>(connectionString);

        try {
            pqxx::connection c(connectionString);
            pqxx::work txn(c);
            txn.exec("TRUNCATE TABLE sensor_data RESTART IDENTITY");
            txn.commit();
        }
        catch (const std::exception& e) {
            std::cerr << "Warning: Could not truncate test table: " << e.what() << std::endl;
            std::cerr << "Make sure the test database exists and is accessible." << std::endl;
        }
    }

    void TearDown() override {
        try {
            pqxx::connection c(getEnvOrDefault("POSTGRES_HOST", "localhost") + " " +
                getEnvOrDefault("POSTGRES_PORT", "5432") + " " +
                "dbname=" + (getEnvOrDefault("POSTGRES_DB", "sensors") + "_test") + " " +
                "user=" + getEnvOrDefault("POSTGRES_USER", "postgres") + " " +
                "password=" + getEnvOrDefault("POSTGRES_PASSWORD", "postgres"));
            pqxx::work txn(c);
            txn.exec("TRUNCATE TABLE sensor_data RESTART IDENTITY");
            txn.commit();
        }
        catch (...) {
        }
    }
};

TEST_F(SensorRepositoryIntegrationTest, InsertAndGetById) {
    SensorDataCreate createDto{ "test_sensor_1", 23.5, "Celsius" };

    int id = repository->insert(createDto);
    auto result = repository->getById(id);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->sensor_id, "test_sensor_1");
    EXPECT_DOUBLE_EQ(result->value, 23.5);
    EXPECT_EQ(result->unit, "Celsius");
    EXPECT_FALSE(result->timestamp.empty());
}

TEST_F(SensorRepositoryIntegrationTest, GetAll_ReturnsInsertedItems) {
    SensorDataCreate dto1{ "test_sensor_2", 10.0, "Volt" };
    SensorDataCreate dto2{ "test_sensor_3", 15.5, "Ampere" };

    int id1 = repository->insert(dto1);
    int id2 = repository->insert(dto2);
    auto all = repository->getAll();

    EXPECT_EQ(all.size(), 2);
    bool found1 = false, found2 = false;
    for (const auto& sensor : all) {
        if (sensor.sensor_id == "test_sensor_2" &&
            std::abs(sensor.value - 10.0) < 0.001 &&
            sensor.unit == "Volt") {
            found1 = true;
        }
        if (sensor.sensor_id == "test_sensor_3" &&
            std::abs(sensor.value - 15.5) < 0.001 &&
            sensor.unit == "Ampere") {
            found2 = true;
        }
    }
    EXPECT_TRUE(found1);
    EXPECT_TRUE(found2);
}

TEST_F(SensorRepositoryIntegrationTest, UpdateSensorData) {
    SensorDataCreate createDto{ "test_sensor_4", 5.5, "Celsius" };
    int id = repository->insert(createDto);

    SensorData updateData{ id, "test_sensor_4_updated", "2026-08-30T10:00:00Z", 7.5, "Fahrenheit" };

    repository->update(id, updateData);
    auto result = repository->getById(id);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->sensor_id, "test_sensor_4_updated");
    EXPECT_DOUBLE_EQ(result->value, 7.5);
    EXPECT_EQ(result->unit, "Fahrenheit");
    EXPECT_FALSE(result->timestamp.empty());
}

TEST_F(SensorRepositoryIntegrationTest, RemoveSensorData) {
    SensorDataCreate createDto{ "test_sensor_5", 12.3, "Celsius" };
    int id = repository->insert(createDto);

    repository->remove(id);
    auto result = repository->getById(id);

    EXPECT_FALSE(result.has_value());
}

TEST_F(SensorRepositoryIntegrationTest, GetById_NonExistingId_ReturnsNullopt) {
    auto result = repository->getById(99999);

    EXPECT_FALSE(result.has_value());
}