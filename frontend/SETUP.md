# SETUP

## 📦 Instalação de Dependências

### DNF (Fedora/RHEL)

```bash
sudo dnf install -y nodejs npm
```

### Verificar Instalação

```bash
node -v
npm -v
```

---

## 📚 Instalação de Bibliotecas

> **⚠️ Nota:** Ignore os arquivos existentes durante a criação do projeto

```bash
npm create vite@latest . -- --template react
npm install
```

---

## ▶️ Executar Aplicação

```bash
npm run dev
```

---

## 🐳 Build e Deploy com Docker

### Build da Imagem

```bash
docker build -t frontend-react .
```

### Executar Container

```bash
docker run --detach --name frontend-react-c -p 3000:80 frontend-react
```
