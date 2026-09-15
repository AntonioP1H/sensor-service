CREATE TABLE IF NOT EXISTS sensor_data (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    sensor_id VARCHAR(50) NOT NULL,
    timestamp TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    value DOUBLE PRECISION NOT NULL,
    unit VARCHAR(20),

    CONSTRAINT sensor_data_sensor_id_not_blank
        CHECK (btrim(sensor_id) <> ''),

    CONSTRAINT sensor_data_value_finite
        CHECK (
            value <> 'NaN'::double precision
            AND value <> 'Infinity'::double precision
            AND value <> '-Infinity'::double precision
        )
);

CREATE INDEX IF NOT EXISTS idx_sensor_data_sensor_id
    ON sensor_data(sensor_id);

CREATE INDEX IF NOT EXISTS idx_sensor_data_timestamp
    ON sensor_data(timestamp);