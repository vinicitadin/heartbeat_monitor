# Heartbeat Monitor

Projeto de monitoramento de batimentos cardíacos utilizando ESP32, sensor KY-039, conexão Wi-Fi e envio de dados via MQTT.

## Visão Geral

Este projeto realiza a leitura do sensor KY-039 para monitorar batimentos cardíacos. Os dados coletados são enviados periodicamente para um servidor MQTT, permitindo acompanhamento remoto dos dados de frequência cardíaca. Também possui integração com um buzzer para avisos sonoros e pode receber comandos do backend via MQTT.

## Hardware

- ESP32 ou similar  
- Sensor de batimentos cardíacos KY-039  
- Buzzer  
- Conexão Wi-Fi

## Funcionalidades

- Leitura contínua do batimento cardíaco pelo sensor KY-039.
- Envio periódico dos dados para um broker MQTT definido.
- Recepção de comandos MQTT para controle remoto do dispositivo.
- Alerta sonoro (buzzer) em condições configuráveis.
- Parâmetros facilmente configuráveis por macros (pins, thresholds, tópicos, etc).

## Configurações Importantes (configs.h)

| Macro                    | Descrição                                   | Valor                               |
|--------------------------|---------------------------------------------|-------------------------------------|
| CONFIG_KY039_PIN         | Pino do sensor KY-039                       | 34                                  |
| CONFIG_BUZZER_PIN        | Pino do buzzer                              | 25                                  |
| CONFIG_TAB_LENGTH        | Tamanho do buffer de leituras               | 4                                   |
| CONFIG_RISE_THRESHOLD    | Threshold para detecção de batimento        | 5                                   |
| CONFIG_CALIB_OFFSET      | Offset de calibração                        | 0                                   |
| CONFIG_SENSOR_TIMER_READ | Intervalo de leitura do sensor (ms)         | 20                                  |
| CONFIG_WIFI_TIMEOUT_MS   | Timeout da conexão Wi-Fi (ms)               | 10000                               |
| CONFIG_MQTT_SERVER       | Endereço do broker MQTT                     | Definido em `Credentials.h`         |
| CONFIG_MQTT_PORT         | Porta MQTT                                  | 1883                                |
| CONFIG_MQTT_PUB_TOPIC    | Tópico para publicação dos dados            | "unesciot/heartbeat/monitor"        |
| CONFIG_MQTT_SUB_TOPIC    | Tópico para comandos                        | "unesciot/heartbeat/response"       |
| CONFIG_MQTT_INTERVAL_TIME| Intervalo de envio ao broker (ms)           | 20000                               |
| CONFIG_MQTT_RECONNECT_INTERVAL_MS | Intervalo para reconexão MQTT (ms) | 5000                                |

As credenciais Wi-Fi e MQTT são protegidas e, por padrão, vêm do arquivo `Credentials.h`.

## Como usar

1. **Clone o repositório**  
   ```bash
   git clone https://github.com/vinicitadin/heartbeat_monitor.git
   ```

2. **Configure as credenciais:**  
   Crie o arquivo `Credentials.h` e informe:
   ```c
   #define WIFI_SSID "<sua-rede>"
   #define WIFI_PASSWORD "<sua-senha>"
   #define MQTT_SERVER "<endereco-broker>"
   ```

3. **Compile e grave no ESP32**  
   Use a plataforma de sua preferência (ex: [PlatformIO](https://platformio.org/) ou [Arduino IDE](https://www.arduino.cc/en/software)).

4. **Conecte o hardware**
   - Sensor KY-039: GPIO conforme `CONFIG_KY039_PIN`
   - Buzzer: GPIO conforme `CONFIG_BUZZER_PIN`

5. **Monitore e envie comandos**
   - Monitoramento via tópico MQTT: `unesciot/heartbeat/monitor`
   - Comandos para o dispositivo via MQTT: `unesciot/heartbeat/response`

## Protocolos e Tópicos MQTT

- **Publicação** (`unesciot/heartbeat/monitor`): envia dados do sensor periodicamente.
- **Assinatura** (`unesciot/heartbeat/response`): recebe comandos para o buzzer ou outras ações.
