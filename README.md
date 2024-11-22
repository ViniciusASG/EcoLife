# EcoLife
EcoLife GS EDGE FIAP

Vinicius Augusto RM 557065
JOAO PEDRO RM 558375

Aqui está um exemplo de um README.md que documenta o processo de configuração do seu projeto, desde o uso do Arduino até a comunicação com o servidor Node-RED na Azure. O README inclui etapas sobre configuração de hardware, software, e comunicação entre o dispositivo e o servidor.

Projeto IoT com Arduino e Node-RED na Azure
Este projeto utiliza um Arduino para coletar dados de sensores e os envia para um servidor Node-RED hospedado na Azure. O objetivo é capturar dados em tempo real (como distância de objetos) e enviá-los para visualização em um dashboard no Node-RED.

Sumário
Hardware necessário
Configuração do Arduino
Configuração do Node-RED
Configuração do Servidor na Azure
Código do Arduino
Teste de Conexão e Solução de Problemas
Referências
Hardware necessário
Arduino Uno ou qualquer placa Arduino compatível.
Sensor Ultrassônico HC-SR04: usado para medir a distância de objetos.
Fonte de alimentação (Bateria 9V): para alimentar o Arduino.
Placa Solar (opcional): para recarregar a bateria.
Esquema de conexão:
Sensor Ultrassônico:

GND -> GND do Arduino
VCC -> 5V do Arduino
SIG -> Pino Digital 9 do Arduino
Bateria 9V:

Conectar o terminal positivo (+) ao pino Vin do Arduino.
Conectar o terminal negativo (-) ao pino GND do Arduino.
Placa Solar (opcional):

Pode ser conectada à entrada de alimentação do Arduino, dependendo da tensão de saída da placa solar.
Configuração do Arduino
Para conectar o Arduino ao servidor Node-RED, você precisa configurar o código no Arduino para ler dados dos sensores e enviá-los via HTTP para o servidor.

Instale a biblioteca WiFi ou Ethernet, dependendo da sua conexão.
Conecte o Arduino à rede Wi-Fi ou use um módulo Ethernet.
Envie os dados coletados (distância) via requisição HTTP para o servidor Node-RED.
Exemplo de código do Arduino (atualizado para uso do IP público):
cpp
Copiar código
#include <WiFi.h>
#include <HTTPClient.h>

// Configurações de WiFi
const char* ssid = "NOME_DA_REDE_WIFI";
const char* password = "SENHA_DA_REDE_WIFI";

// Configurações do Sensor Ultrassônico
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi.");
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Calcula a distância
  
  // Envia os dados para o servidor Node-RED
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://4.228.62.162:1880/dados"); // Endereço do servidor Node-RED
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"distancia\": " + String(distance) + "}";
    int httpResponseCode = http.POST(jsonData);
    
    if (httpResponseCode > 0) {
      Serial.println("Dados enviados com sucesso");
    } else {
      Serial.println("Erro ao enviar dados");
    }
    
    http.end();
  }
  
  delay(10000);  // Envia a cada 10 segundos
}
Configuração do Node-RED
Instalar o Node-RED em uma máquina virtual Linux na Azure.
Criar um fluxo no Node-RED para receber os dados do Arduino.
Configurar um servidor HTTP no Node-RED para receber requisições HTTP.
Criar um dashboard para visualizar os dados em tempo real.
Passos de configuração:
Instale o Node-RED em uma máquina virtual na Azure, rodando Ubuntu 22.04.
Configure a máquina virtual para aceitar conexões na porta 1880 (ou a porta que você escolher).
Crie um fluxo no Node-RED para receber as requisições HTTP e armazenar os dados recebidos.
Exemplo de fluxo:
Use um nó HTTP In para definir a URL (por exemplo, /dados).
Use um nó JSON para converter os dados recebidos em JSON.
Use um nó Debug para exibir os dados no console do Node-RED.
Use um nó Dashboard para exibir os dados em um painel de controle.
Configuração do Servidor na Azure
Crie uma máquina virtual (VM) no Azure.
Defina o IP público (no caso: 4.228.62.162).
Habilite a porta 1880 no grupo de segurança de rede para permitir o tráfego HTTP.
Instale o Node-RED na VM (Ubuntu 22.04).
Passos:
Conecte-se à máquina via SSH:
bash
Copiar código
ssh username@4.228.62.162
Instale o Node-RED:
bash
Copiar código
bash <(curl -sL https://nodered.org/setup.sh)
Execute o Node-RED:
bash
Copiar código
node-red-start
Teste de Conexão e Solução de Problemas
Problemas Comuns:
Erro de Conexão ("Couldn't connect to server")

Verifique as regras de firewall na Azure para garantir que a porta 1880 está aberta.
Verifique se o Node-RED está rodando corretamente na VM.
Erro de Timeout

Verifique se o servidor está ouvindo na porta correta.
Verifique se a conexão Wi-Fi do Arduino está ativa.
Referências
Node-RED Documentation
Arduino Documentation
Azure Documentation
