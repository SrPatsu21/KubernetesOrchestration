# AngelCorp - Kubernetes Orchestration

Este projeto tem como objetivo realizar a conteinerização e orquestração de uma aplicação utilizando Docker e Kubernetes.

A aplicação é composta por:

- Frontend desenvolvido em React + Vite;
- Backend desenvolvido em C++ utilizando o framework Drogon;
- Banco de dados PostgreSQL.

Toda a infraestrutura é gerenciada por manifests YAML do Kubernetes, utilizando Deployments, Secrets e Persistent Volumes.

## Estrutura do Projeto

```text
.
├── backend
│   ├── build
│   ├── CMakeLists.txt
│   ├── Dockerfile
│   ├── drogon
│   ├── SETUP.md
│   └── src
├── docker-compose.yml
├── frontend
│   ├── Dockerfile
│   ├── index.html
│   ├── nginx.conf
│   ├── node_modules
│   ├── package.json
│   ├── package-lock.json
│   ├── SETUP.md
│   ├── src
│   └── vite.config.js
├── k8s
│   ├── backend-deployment.yaml
│   ├── backend-service.yaml
│   ├── configmap.yaml
│   ├── db-deployment.yaml
│   ├── db-pvc.yaml
│   ├── db-pv.yaml
│   ├── db-service.yaml
│   ├── frontend-deployment.yaml
│   ├── frontend-service.yaml
│   ├── secrets.yaml
└── README.md
```

## Tecnologias Utilizadas

- Docker
- Docker Compose
- Kubernetes
- Kind
- React + Vite
- Drogon Framework (C++)
- PostgreSQL 17
- Nginx

## Funcionalidades

A API disponibiliza os seguintes endpoints REST:

| Método | Endpoint      | Descrição            |
| ------ | ------------- | -------------------- |
| GET    | `/items`      | Lista todos os itens |
| POST   | `/items`      | Cria um novo item    |
| DELETE | `/items/{id}` | Remove um item       |

O frontend permite:

- Listar itens cadastrados;
- Inserir novos itens;
- Remover itens existentes.

# Comunicação entre os Serviços

Os serviços utilizam o DNS interno do Kubernetes para comunicação.

Frontend → Backend

```text
http://backend-service:8080
```

Backend → PostgreSQL

```text
postgres-service:5432
```

Nenhum endereço IP fixo é utilizado na comunicação entre os contêineres.

## Configurações

As configurações não sensíveis são armazenadas em um **ConfigMap**.
As informações sensíveis são armazenadas em um **Secret**.

---

## Persistência de Dados

O PostgreSQL utiliza armazenamento persistente através de:

- PersistentVolume (PV);
- PersistentVolumeClaim (PVC).

# Docker Compose

Para facilitar o desenvolvimento local, o projeto disponibiliza um arquivo `docker-compose.yml`.

Este ambiente executa:

- Frontend;
- Backend;
- PostgreSQL.

Pode ser iniciado pelo comando `docker compose up -d --build`. Vale resaltar que nesse build os conteiners rodam em modo de produção.

Para usar o modo de devenvolvimento existe um arquivo `SETUP.md` no frontend como no backend. Esse arquivo instrue a instalação local das dependencias.

## Kind Setup

### Instalar Kind no host

```shell
curl -Lo ./kind https://kind.sigs.k8s.io/dl/v0.30.0/kind-linux-amd64

chmod +x kind

sudo mv kind /usr/local/bin/
```

#### verificar

```shell
kubectl version --client
```

### Criar cluster

```shell
kind create cluster --name angelcorp
```

#### verificar

```shell
kubectl get nodes
```

### Construir imagens

```shell
docker build -t angelcorp-backend:latest ./backend

docker build -t angelcorp-frontend:latest ./frontend
```

### Carregar no Kind

```shell
kind load docker-image angelcorp-backend:latest --name angelcorp

kind load docker-image angelcorp-frontend:latest --name angelcorp
```

### Aplicar manifests

```shell
kubectl apply -f secrets.yaml

kubectl apply -f configmap.yaml

kubectl apply -f db-pv.yaml

kubectl apply -f db-pvc.yaml

kubectl apply -f db-deployment.yaml

kubectl apply -f db-service.yaml

kubectl apply -f backend-deployment.yaml

kubectl apply -f backend-service.yaml

kubectl apply -f frontend-deployment.yaml

kubectl apply -f frontend-service.yaml
```

#### Test

```shell
kubectl get pods

kubectl get svc

kubectl get pvc
```

### Restart deployment

```shell
kubectl rollout restart deployment frontend
kubectl rollout restart deployment backend
```

### Stop

```shell
docker stop angelcorp-control-plane
```

### Enable frontend port

```shell
kubectl port-forward service/frontend-service 3000:80
```
