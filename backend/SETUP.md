# SETUP

## 📦 Instalação de Dependências

### APT (Debian/Ubuntu)

```bash
sudo apt update

sudo apt install -y \
    git \
    cmake \
    g++ \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    openssl \
    libpq-dev \
    libssl-dev
```

### DNF (Fedora/RHEL)

```bash
sudo dnf install -y \
    git \
    cmake \
    gcc-c++ \
    jsoncpp-devel \
    libuuid-devel \
    libpq-dev \
    zlib-devel \
    openssl-devel
```

---

## 🔧 Instalação da Biblioteca Drogon

### Clone e Preparação

```bash
git clone https://github.com/drogonframework/drogon.git
cd drogon
git submodule update --init
```

### Build e Instalação

```bash
mkdir build
cd build

cmake ..
make -j$(nproc)

sudo make install
```

### Atualizar Cache de Bibliotecas

```bash
sudo ldconfig
```

### Verificar Instalação

```bash
drogon_ctl
```

---

## 🏗️ Build do Servidor

```bash
mkdir build
cd build

cmake ..
make
```

---

## 🐳 Criar Imagem Docker

```bash
docker build -t backend-api:latest .
```

## 🐳 Rodar Imagem Docker

```bash
docker run --detach --name backend-api-c -p 8080:8080 backend-api:latest
```
