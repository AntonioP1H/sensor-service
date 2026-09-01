cmake_minimum_required(VERSION 3.20)
project(SensorService LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

find_package(libpqxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_library(sensor_core STATIC
    src/controllers/sensor_controller.cpp
    src/services/sensor_service.cpp
    src/repositories/sensor_repository.cpp
)
target_include_directories(sensor_core PUBLIC src/)
target_link_libraries(sensor_core PUBLIC libpqxx::pqxx Crow::Crow)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sensor_core)

enable_testing()
add_subdirectory(tests)