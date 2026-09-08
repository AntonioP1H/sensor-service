# Sensor Service

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat&logo=c%2B%2B)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C?style=flat&logo=cmake)](https://cmake.org/)
[![PostgreSQL](https://img.shields.io/badge/PostgreSQL-14%2B-4169E1?style=flat&logo=postgresql)](https://www.postgresql.org/)
[![Docker](https://img.shields.io/badge/Docker-Supported-2496ED?style=flat&logo=docker)](https://www.docker.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A multithreaded RESTful backend built in C++17 for ingesting and querying time-series sensor data. Built using a layered architecture to decouple HTTP handling, core business rules, and database execution—ensuring high testability and straightforward maintenance.

---

## Key Features

- **RESTful API**: Async HTTP ingestion and querying powered by Crow.
- **Persistent Storage**: Raw PostgreSQL connection handling via `libpqxx`.
- **Layered Architecture**: Decoupled Controllers, Services, Repositories, and Models.
- **Testing**: Unit tests with Google Mock/Google Test alongside integration tests against a live database.
- **Containerized**: Fully configured with Docker and Docker Compose for instant environment reproduction.
- **Automated CI**: GitHub Actions workflow covering automated builds and test executions.

---

## Tech Stack

| Category | Technology |
| :--- | :--- |
| **Language** | C++17 |
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
  |   Controllers    |  <-- HTTP handlers & routing (Crow)
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
* **Testability**: Services depend on repository interfaces, allowing unit tests to mock database access effortlessly.
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
├── tests/
│   ├── unit/              # Unit tests (with mock repositories)
│   └── integration/       # Integration tests (against active Postgres)
├── schema.sql             # DB initialization & table definitions
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

## Getting Started

### Prerequisites

* **C++ Compiler**: GCC, Clang, or MSVC with C++17 support
* **Build Tools**: CMake 3.20+ and Git
* **Package Manager**: `vcpkg` (for managing `libpqxx`, `crow`, `gtest`)
* **Database**: PostgreSQL 14+ (or Docker)

### Local Development Setup

1. **Database Setup**:
   Install PostgreSQL and initialize a database named `sensors`:
   ```bash
   createdb sensors
   psql -d sensors -f schema.sql
   ```

2. **Environment Configuration**:
   Create a `.env` file in the root directory using the template:
   ```bash
   cp .env.example .env
   ```
   *Adjust credentials inside `.env` to match your local setup.*

3. **Build via CMake**:
   ```bash
   # Clone repository
   git clone https://github.com/yourusername/sensor-service.git
   cd sensor-service

   # Configure & Build
   mkdir build && cd build
   cmake -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake ..
   make

   # Run application
   ./SensorService
   ```

---

## Docker Deployment

To launch both the service and a pre-configured PostgreSQL instance with zero host dependencies:

```bash
docker-compose up --build
```

* **Service URL**: `http://localhost:8080`
* **Health Check**: `http://localhost:8080/`
* **PostgreSQL Port**: `localhost:5432`

---

## Running Tests

### Unit Tests
```bash
cd build
ctest --output-on-failure -R UnitTest
```

### Integration Tests
*Note: Requires a running PostgreSQL instance specified in `.env`.*
```bash
cd build
ctest --output-on-failure -R IntegrationTest
```

---

## Design Decisions & Trade-offs

1. **Direct SQL (`libpqxx`) over ORM**:
   Chosen to eliminate the performance overhead of full ORMs while maintaining explicit control over SQL queries, transactions, and indexing strategies.
2. **Layered Architecture over Single-File Simplicity**:
   Adding discrete layer boundaries introduces minor boilerplate, but guarantees isolated unit testing and clean separation between HTTP parsing and database queries.
3. **Service Layer Validation**:
   Validation logic lives explicitly in business services rather than controllers or SQL constraints, ensuring consistent rule enforcement regardless of the entry point.

---

## License

Distributed under the MIT License. See `LICENSE` for more information.