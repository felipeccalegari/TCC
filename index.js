// Configurações PostgreSQL

const { Client } = require("pg")

const client = new Client({
    user: "postgres",
    host: "postgres_dht",
    database: "esp32",
    password: "123456789",
    port: 5432
});

client.connect();

// Criar tabela "temperatura" com as colunas tempC, alta, baixa, simbolizando temperatura em Celsius, alarme de temperatura Alta e alarme de temperatura Baixa, respectivamente

const createTableQuery = `CREATE TABLE IF NOT EXISTS temperatura (
    id SERIAL PRIMARY KEY,
    tempC float,
    alta integer,
    baixa integer,
    hora TIMESTAMP NOT NULL DEFAULT NOW()
)`;

// Query para verificar a criação da tabela "temperatura" definida
client.query(createTableQuery, (error, res) => {
    if (error) {
        console.error(error);
    } else {
        console.log(`Tabela criada: ${JSON.stringify(res)}`);
    }
});



// Configurações MQTT

var mqtt = require('mqtt')
var portMqtt = '1883'
var hostMqtt = 'broker.hivemq.com'
var clientId = "ESP32-felipe"
const connectUrl = `mqtt://${hostMqtt}:${portMqtt}`
var clientMqtt = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: 'felipe', 
    passwordMqtt: '12345',
    reconnectPeriod: 1000,
})

var topicTempC = "dht/tempC"
var topicAlarmeA = "dht/alarmeA"
var topicAlarmeB = "dht/alarmeB"

// Conectando ao MQTT

clientMqtt.on('connect', () => {
    console.log("Conectado ao MQTT!")
})

// Realizando o subscribe/inscrição nos tópicos de temperatura atual, alarme de temperatura alta e alarme de temperatura baixa
clientMqtt.subscribe(topicTempC)
clientMqtt.subscribe(topicAlarmeA)
clientMqtt.subscribe(topicAlarmeB)


// Variáveis para armazenar o payload do MQTT
var tempFloat
var varAlarmeAlta
var varAlarmeBaixa

// Receber os dados do MQTT e enviá-los para a base de dados

clientMqtt.on('message', (topic, payload, packet) => {
    if (topic == topicTempC) { // Verifica se o tópico de mensagem recebida é o de temperatura atual
        tempFloat = parseFloat(payload); // Transforma para um valor do tipo 'float' o valor da temperatura atual
        console.log("Temperatura em Celsius: " + tempFloat);

    } else if (topic == topicAlarmeA) { // Verifica se o tópico de mensagem recebida é o de alarme de temperatura alta
        varAlarmeAlta = payload // payload atual é o estado do alarme de temperatura alta
        if (varAlarmeAlta == 1) { // Verifica o estado recebido do mqtt

            console.log("Estado alarme de temperatura ALTA: LIGADO")

        } else if (varAlarmeAlta == 0) { // Verifica o estado recebido do mqtt

            console.log("Estado alarme de temperatura ALTA: DESLIGADO")

        }

    } else if (topic == topicAlarmeB) { // Verifica se o tópico de mensagem recebida é o de alarme de temperatura baixa
        varAlarmeBaixa = payload // payload atual é o estado do alarme de temperatura baixa
        if (varAlarmeBaixa == 1) { // Verifica o estado recebido do mqtt

            console.log("Estado alarme de temperatura BAIXA: LIGADO")

        } else if (varAlarmeBaixa == 0) { // Verifica o estado recebido do mqtt

            console.log("Estado alarme de temperatura BAIXA: DESLIGADO")

        }
    }
    // Variável com comando para inserir no banco de dados na tabela 'temperatura', nas colunas 'tempC', 'alta', 'baixa' com os valores recebidos anteriormente
    const insertData = `INSERT INTO temperatura (tempC, alta, baixa) VALUES (${tempFloat}, ${varAlarmeAlta}, ${varAlarmeBaixa})`;
    client.query(insertData, (error, res) => { // Função para realizar uma consulta, com os parâmetros da variável de inserir dados (insertData) e uma função de callback em caso de erro
        if (error) { // Caso houver erro
            console.error(error);
        } else { // Caso a consulta for bem sucedida
            console.log(`Dados inseridos: ${JSON.stringify(res)}`);
        }
    })
});