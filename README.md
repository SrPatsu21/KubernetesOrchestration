## Setup

### Instalar Kind no host

curl -Lo ./kind https://kind.sigs.k8s.io/dl/v0.30.0/kind-linux-amd64

chmod +x kind

sudo mv kind /usr/local/bin/

#### verificar

kubectl version --client

### Criar cluster

kind create cluster --name angelcorp

#### verificar

kubectl get nodes

### Construir imagens

docker build -t angelcorp-backend:latest ./backend

docker build -t angelcorp-frontend:latest ./frontend

### Carregar no Kind

kind load docker-image angelcorp-backend:latest --name angelcorp

kind load docker-image angelcorp-frontend:latest --name angelcorp

### Aplicar manifests

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

#### Test

kubectl get pods

kubectl get svc

kubectl get pvc

### Restart deployment

kubectl rollout restart deployment frontend
kubectl rollout restart deployment backend

### Stop

docker stop angelcorp-control-plane

### Enable frontend port

kubectl port-forward service/frontend-service 3000:80
