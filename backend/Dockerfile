#* ###################################
#* BUILD
#* ###################################

FROM ubuntu:24.04 AS backend_builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    git \
    cmake \
    g++ \
    make \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    openssl \
    libssl-dev

# Drogon
RUN git clone https://github.com/drogonframework/drogon.git && \
    cd drogon && \
    git submodule update --init && \
    mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install && \
    ldconfig

WORKDIR /app

COPY . .

# Build da API
RUN mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)

#* ###################################
#* RUNTIME
#* ###################################

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Apenas libs runtime
RUN apt update && apt install -y \
    libjsoncpp25 \
    uuid-runtime \
    zlib1g \
    openssl \
    libssl3 && \
    apt clean

# Copia libs (Drogon) e executavel
COPY --from=backend_builder /usr/local /usr/local
COPY --from=backend_builder /app/build/backend_api /backend_api

EXPOSE 8080

CMD ["/backend_api"]