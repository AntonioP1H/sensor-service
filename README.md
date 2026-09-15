# Sensor Service

[![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?style=flat&logo=c%2B%2B)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C?style=flat&logo=cmake)](https://cmake.org/)
[![PostgreSQL](https://img.shields.io/badge/PostgreSQL-14%2B-4169E1?style=flat&logo=postgresql)](https://www.postgresql.org/)
[![Docker](https://img.shields.io/badge/Docker-Supported-2496ED?style=flat&logo=docker)](https://www.docker.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A multithreaded RESTful backend built in C++20 for ingesting and querying time-series sensor data. Built using a layered architecture to decouple HTTP handling, core business rules, and database execution—ensuring high testability and straightforward maintenance.

---

## Key Features

- **RESTful API**: Async HTTP ingestion and querying powered by Crow.
- **Persistent Storage**: Raw PostgreSQL connection handling via `libpqxx` with automated checks and indexing.
- **Layered Architecture**: Decoupled Controllers, Services, Repositories, and Models.
- **Testing**: Unit tests with Google Mock/Google Test alongside isolated integration tests against a dedicated live test database.
- **Containerized**: Fully configured with Docker and Docker Compose for instant environment reproduction.
- **Automated CI**: GitHub Actions workflow covering automated builds, dependency setups, and test executions.

---

## Tech Stack

| Category | Technology |
| :--- | :--- |
| **Language** | C++20 |
| **HTTP Framework** | Crow |
| **Database** | PostgreSQL 14+ |
| **DB Driver** | `libpqxx` |
| **Build System** | CMake 3.20+ |
| **Package Manager**| `vcpkg` |
| **Testing** | GoogleTest (gtest) & GoogleMock (gmock) |
| **DevOps** | Docker, Docker Compose, GitHub Actions |

---

## System Architecture

```text
  +------------------+
  |    Controllers   |  <-- HTTP handlers & routing (Crow)
  +------------------+
         |
  +------------------+
  |     Services     |  <-- Business logic, validation, orchestration
  +------------------+
         |
  +------------------+
  |   Repositories   |  <-- Data access layer (PostgreSQL via libpqxx)
  +------------------+
         |
  +------------------+
  |      Models      |  <-- Data Transfer Objects (DTOs) & entities
  +------------------+
```

### Architectural Rationale
* **Separation of Concerns**: HTTP parsing stays in Controllers; database operations stay in Repositories.
* **Testability**: Services depend on repository interfaces, allowing unit tests to mock database access effortlessly via Google Mock.
* **Low Overhead**: Strict abstraction boundaries without unnecessary performance hits—keeping services lightweight and repository queries optimized.

---

## Project Structure

```text
sensor-service/
├── src/
│   ├── controllers/       # HTTP route handlers
│   ├── services/          # Core business logic & validation
│   ├── repositories/      # Database abstraction layer
│   ├── models/            # DTOs and domain entities
│   └── main.cpp           # App entry point & dependency wiring
├── database/
│   └── schema.sql         # PostgreSQL schema, indexes & constraints
├── tests/
│   ├── unit/              # Unit tests (with mock repositories)
│   └── integration/       # Integration tests (against active Postgres)
├── Dockerfile             # Multi-stage image build
├── docker-compose.yml     # Service & database orchestration
├── .env.example           # Environment configuration template
├── CMakeLists.txt         # Root build script
└── .github/
    └── workflows/
        └── ci.yml         # GitHub Actions workflow configuration
```

---

## API Reference

### Endpoints

| Method | Endpoint | Description |
| :--- | :--- | :--- |
| `GET` | `/` | Service health check |
| `POST` | `/api/sensors` | Submit a new sensor reading |
| `GET` | `/api/sensors` | Retrieve all sensor readings |
| `GET` | `/api/sensors/:id` | Fetch a specific reading by ID |
| `PUT` | `/api/sensors/:id` | Update an existing reading |
| `DELETE` | `/api/sensors/:id` | Remove a reading |

### Request Payload Example (`POST` / `PUT`)

```json
{
  "sensor_id": "temp_sensor_01",
  "value": 23.5,
  "unit": "Celsius"
}
```

---

## Database

The application utilizes PostgreSQL with a schema optimized for time-series sensor ingestion. 
Key schema features include:
* Auto-generated primary keys and timestamps.
* Indexes on `sensor_id` and timestamps for fast querying.
* Database-level check constraints for data integrity (e.g., rejecting blank sensor IDs or non-finite values).

---

## Getting Started

### Prerequisites

* **C++ Compiler**: GCC, Clang, or MSVC with C++20 support
* **Build Tools**: CMake 3.20+ and Git
* **Package Manager**: `vcpkg` (for managing `libpqxx`, `crow`, `gtest`, `gmock`)
* **Database**: PostgreSQL 14+ (or Docker)

### Local Development Setup

1. **Install Dependencies via vcpkg**:
   ```bash
   vcpkg install libpqxx crow gtest gmock
   ```

2. **Database Setup**:
   Create the primary and testing databases:
   ```bash
   psql -h localhost -p 5432 -U postgres -c "CREATE DATABASE sensors;"
   psql -h localhost -p 5432 -U postgres -c "CREATE DATABASE sensors_test;"
   
   psql -h localhost -p 5432 -U postgres -d sensors -f database/schema.sql
   psql -h localhost -p 5432 -U postgres -d sensors_test -f database/schema.sql
   ```

3. **Environment Configuration**:
   Create a `.env` file or export your environment variables:
   ```bash
   # Example PowerShell configuration (or use .env file reference)
   $env:POSTGRES_HOST = "localhost"
   $env:POSTGRES_PORT = "5432"
   $env:POSTGRES_DB = "sensors"
   $env:POSTGRES_USER = "postgres"
   $env:POSTGRES_PASSWORD = "your-password"
   $env:SERVER_PORT = "8080"
   ```

4. **Build via CMake**:
   ```bash
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
   cmake --build build --config Release
   ```

---

## Docker Deployment

To launch both the service and a pre-configured PostgreSQL instance with zero host dependencies:

```bash
docker-compose up --build
```

* **Service URL**: `http://localhost:8080`
* **PostgreSQL Port**: `localhost:5432`

---

## Running Tests

### Unit Tests
Unit tests run locally with mocked repositories and require no active database connection:
```bash
ctest --test-dir build --output-on-failure -L unit
```

### Integration Tests
Integration tests execute against your active PostgreSQL `sensors_test` instance:
```bash
ctest --test-dir build --output-on-failure -L integration
```

---

## Design Decisions & Trade-offs

1. **Direct SQL (`libpqxx`) over ORM**:
   Chosen to eliminate the performance overhead of full ORMs while maintaining explicit control over SQL queries, transactions, and indexing strategies.
2. **Repository Interface & Dependency Injection**:
   The service depends on a repository interface rather than direct database logic, enabling seamless unit testing with Google Mock.
3. **Layered Architecture over Single-File Simplicity**:
   Adding discrete layer boundaries introduces minor boilerplate, but guarantees isolated unit testing and clean separation between HTTP parsing and database queries.

---

## License

Distributed under the MIT License. See `LICENSE` for more information.