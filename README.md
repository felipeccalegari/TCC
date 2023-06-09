# Trabalho de Conclusão de Curso - Estudo da Utilização da Plataforma Docker em Aplicações Mecatrônicas

## Introdução
Este é o meu trabalho de conclusão de curso da Engenharia Mecatrônica pelo Instituto Federal de Santa Catarina.
O tema é "Estudo da Utilização da Plataforma Docker em Aplicações Mecatrônicas", onde realizo uma abordagem do desenvolvimento de contêineres em um projeto no contexto da mecatrônica.

## Projeto solução
A proposta de solução foi um sistema de monitoramento de temperatura ambiente envolvendo tecnologias IoT. Os componentes utilizados foram: ESP32 e sensor de temperatura DHT22. O sensor envia dados de temperatura ambiente para o ESP32 e este é responsável por enviar para um broker MQTT os dados de temperatura atual em Celsius, o estado do alarme de temperatura alta e o estado do alarme de temperatura baixa (3 tópicos configurados).

Da parte de software, foram utilizados o framework Node.js para desenvolver o backend responsável por receber os dados do MQTT e enviar ao banco de dados, o banco de dados PostgreSQL para armazenar os dados tratados pelo backend e um sistema de visualização gráfica a partir do Grafana. Foi utilizado o Docker para desenvolver os contêineres de cada módulo da aplicação.

## Circuito Elétrico - ESP32 + DHT22

![alt text](https://github.com/felipeccalegari/TCC/blob/master/esp32_dht11_2023/tcc_esquematico_dht22.png?raw=true)
## Utilização

- Alterar o arquivo *.ino* com as credenciais de rede wi-fi e MQTT.
- Alterar o arquivo *index.js* com as credenciais MQTT e de banco de dados.
- Alterar o Docker Compose com suas devidas credenciais.
- Alterar o arquivo *datasources.yml* com as credenciais do banco de dados.
- Inicializar os contêineres com o comando `docker-compose up -d`.
