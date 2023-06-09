# A partir de uma imagem do Node.js existente
FROM node:latest

# Configura o diretório de trabalho no container
WORKDIR /app

# Copia qualquer arquivo que comece com 'package' e termine com '.json' da máquina local para a pasta do container
COPY package*.json ./

# Instala as dependências necessárias no container
RUN npm install

# Copia o restante dos arquivos da aplicação para a pasta do container
COPY . .

# Comando executado para quando o container iniciar
CMD [ "npm", "start" ]