#include <WiFi.h>       // Biblioteca para conexão Wi-Fi (ESP32/ESP8266)
#include <HTTPClient.h> // Biblioteca para requisições HTTP

// Configurações do Wi-Fi
const char* ssid = "ECOLIFE";          // Nome da rede Wi-Fi
const char* password = "Eco123";    // Senha da rede Wi-Fi

// Endereço do servidor Node-RED (IP + porta)
const char* serverURL = "http://4.228.62.162:1880"; // Endereço do Node-RED

// Pino do sensor
#define SIG_PIN 8

void setup() {
  // Configuração inicial do Arduino
  pinMode(SIG_PIN, OUTPUT);
  Serial.begin(115200);

  // Conexão Wi-Fi
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Enviar pulso ultrassônico
  pinMode(SIG_PIN, OUTPUT);
  digitalWrite(SIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SIG_PIN, LOW);
  
  // Receber pulso refletido
  pinMode(SIG_PIN, INPUT);
  long duration = pulseIn(SIG_PIN, HIGH);
  
  // Calcular distância (em cm)
  long distance = duration * 0.034 / 2;
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Enviar os dados ao servidor Node-RED
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Iniciar conexão com o servidor Node-RED
    http.begin(serverURL); // Conexão sem endpoint, diretamente no IP e porta
    http.addHeader("Content-Type", "application/json"); // Definir o tipo de conteúdo

    // Criar um payload JSON com os dados
    String payload = "{\"distance\": " + String(distance) + "}";

    // Enviar requisição POST
    int httpResponseCode = http.POST(payload);

    // Verificar a resposta do servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Resposta do servidor: ");
      Serial.println(response);
    } else {
      Serial.print("Erro na conexão: ");
      Serial.println(httpResponseCode);
    }

    // Finalizar conexão
    http.end();
  } else {
    Serial.println("Wi-Fi desconectado!");
  }

  delay(5000); // Aguardar 5 segundos antes de repetir
}
