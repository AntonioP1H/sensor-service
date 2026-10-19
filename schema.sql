CREATE TABLE IF NOT EXISTS sensor_data (
    id SERIAL PRIMARY KEY,
    sensor_id VARCHAR(255) NOT NULL,
    value DOUBLE PRECISION NOT NULL,
    unit VARCHAR(50) NOT NULL,
    timestamp TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_sensor_id ON sensor_data(sensor_id);
CREATE INDEX IF NOT EXISTS idx_timestamp ON sensor_data(timestamp);