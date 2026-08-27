-- Schema for sensor_service database. PostgreSQL DB.

  CREATE TABLE IF NOT EXISTS sensor_data ( id SERIAL PRIMARY KEY, sensor_id VARCHAR(50) NOT NULL, timestamp TIMESTAMPTZ NOT NULL DEFAULT NOW(), value DOUBLE PRECISION NOT NULL, unit VARCHAR(20));

  CREATE INDEX IF NOT EXISTS idx_sensor_data_sensor_id ON sensor_data(sensor_id);

  CREATE INDEX IF NOT EXISTS idx_sensor_data_timestamp ON sensor_data(timestamp);