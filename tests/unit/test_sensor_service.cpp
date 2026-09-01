#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include "../src/services/sensor_service.hpp"
#include "../src/repositories/isensor_repository.hpp"
#include "../src/models/sensor_data.hpp"
#include "../src/models/sensor_data_create.hpp"

using namespace sensor_service;
using namespace sensor_service::repositories;
using namespace sensor_service::models;
using namespace sensor_service::services;
using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::Invoke;

class MockSensorRepository : public ISensorRepository {
public:
    MOCK_METHOD(std::optional<SensorData>, getById, (int id), (const, override));
    MOCK_METHOD(std::vector<SensorData>, getAll, (), (const, override));
    MOCK_METHOD(int, insert, (const SensorDataCreate& createDto), (override));
    MOCK_METHOD(void, update, (int id, const SensorData& data), (override));
    MOCK_METHOD(void, remove, (int id), (override));
};

TEST(SensorServiceTest, CreateSensorData_ValidDto_ReturnsId) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    SensorDataCreate dto{ "sensor1", 25.5, "Celsius" };
    EXPECT_CALL(*mockRepo, insert(dto))
        .WillOnce(Return(42));

    int id = service.create(dto);

    EXPECT_EQ(id, 42);
}

TEST(SensorServiceTest, CreateSensorData_EmptySensorId_ThrowsInvalidArgument) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    SensorDataCreate dto{ "", 25.5, "Celsius" };

    EXPECT_THROW(service.create(dto), std::invalid_argument);
}

TEST(SensorServiceTest, GetById_ExistingId_ReturnsSensorData) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    SensorData expected{ 1, "sensor1", "2026-08-30T10:00:00Z", 25.5, "Celsius" };
    EXPECT_CALL(*mockRepo, getById(1))
        .WillOnce(Return(std::make_optional<SensorData>(expected)));

    auto result = service.getById(1);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, 1);
    EXPECT_EQ(result->sensor_id, "sensor1");
    EXPECT_EQ(result->timestamp, "2026-08-30T10:00:00Z");
    EXPECT_DOUBLE_EQ(result->value, 25.5);
    EXPECT_EQ(result->unit, "Celsius");
}

TEST(SensorServiceTest, GetById_NonExistingId_ReturnsNullopt) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    EXPECT_CALL(*mockRepo, getById(999))
        .WillOnce(Return(std::nullopt));

    auto result = service.getById(999);

    EXPECT_FALSE(result.has_value());
}

TEST(SensorServiceTest, GetAll_ReturnsVectorOfSensorData) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    std::vector<SensorData> expected = {
        {1, "sensor1", "2026-08-30T10:00:00Z", 25.5, "Celsius"},
        {2, "sensor2", "2026-08-30T10:05:00Z", 26.0, "Celsius"}
    };
    EXPECT_CALL(*mockRepo, getAll())
        .WillOnce(Return(expected));

    auto result = service.getAll();

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].id, 1);
    EXPECT_EQ(result[1].id, 2);
}

TEST(SensorServiceTest, UpdateSensorData_ValidData_UpdatesSuccessfully) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    SensorData data{ 1, "sensor1", "2026-08-30T10:00:00Z", 26.0, "Celsius" };
    EXPECT_CALL(*mockRepo, update(1, data))
        .Times(1);

    service.update(1, data);

}

TEST(SensorServiceTest, RemoveSensorData_ExistingId_RemovesSuccessfully) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    EXPECT_CALL(*mockRepo, remove(1))
        .Times(1);

    service.remove(1);

}

TEST(SensorServiceTest, UpdateSensorData_InvalidData_ThrowsInvalidArgument) {
    auto mockRepo = std::make_shared<MockSensorRepository>();
    SensorService service(mockRepo);

    SensorData data{ 1, "", "2026 - 08 - 30T10:00 : 00Z",25.5, "Celsius"};

    EXPECT_THROW(service.update(1, data), std::invalid_argument);
}