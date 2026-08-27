cmake_minimum_required(VERSION 3.20)
project(SensorService LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(libpqxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/controllers/sensor_controller.cpp
    src/services/sensor_service.cpp
    src/repositories/sensor_repository.cpp
)

# Application Executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        libpqxx::pqxx
        Crow::Crow
)

enable_testing()
add_subdirectory(tests)