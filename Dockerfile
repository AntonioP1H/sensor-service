  FROM ubuntu:22.04

  RUN apt-get update && apt-get install -y \
      build-essential \
      cmake \
      git \
      libpqxx-dev \
      libgtest-dev \
      && rm -rf /var/lib/apt/lists/*

  RUN git clone https://github.com/ipkn/crow.git && \
      cd crow && \
      mkdir build && cd build && \
      cmake .. && \
      make install

  WORKDIR /app

  COPY . .

  RUN mkdir -p build && \
      cd build && \
      cmake .. && \
      make

  ENTRYPOINT ["./build/SensorService"]