FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        cmake \
        git \
        libpqxx-dev \
        libboost-all-dev \
        libasio-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/CrowCpp/Crow.git /tmp/crow && \
    cmake \
        -S /tmp/crow \
        -B /tmp/crow/build \
        -DCROW_BUILD_EXAMPLES=OFF \
        -DCROW_BUILD_TESTS=OFF \
        -DCROW_BUILD_DOCS=OFF && \
    cmake \
        --build /tmp/crow/build \
        --config Release \
        --parallel && \
    cmake \
        --install /tmp/crow/build && \
    rm -rf /tmp/crow

WORKDIR /app

COPY . .

RUN cmake \
        -S . \
        -B build \
        -DCMAKE_BUILD_TYPE=Release && \
    cmake \
        --build build \
        --config Release \
        --parallel

ENTRYPOINT ["./build/SensorService"]