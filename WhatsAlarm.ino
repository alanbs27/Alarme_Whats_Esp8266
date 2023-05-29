//-----------------------------------------------------------------------------------------------------------------------------------------
// Área de Inclusão de arquivos.
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>                     
#include <UrlEncode.h>                                                  // Para enviar a mensagem via post deve ser no formato URL
#include <ESP8266WiFi.h>

 
//-----------------------------------------------------------------------------------------------------------------------------------------
// Definições de Hardware

#define Sensor  D3                                                      // Sensor digital ligado do GPIO 12
#define Led1    D2                                                      // Led1 ligado ao GPIO 15

//-----------------------------------------------------------------------------------------------------------------------------------------
// Protótipos de função
void WhatsAppMessages(String message);                                  // Função que envia uma mensagem para o whatsapp

//-----------------------------------------------------------------------------------------------------------------------------------------
// Dados do celular e chave do whatsapp

bool flag = 0;                                                          // Flag para uso na logica do sensor
String phoneNumber = "SEU NUMERO DE TELEFONE";                                  // Meu número de celular com formato internacional
String apiKey = "SUA APIKEY";                                               // A chave que o bot do whatsapp enviou

//-----------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
    pinMode(Sensor, INPUT_PULLUP);                                      // Configura pino do sensor como entrada e em pull-up
    pinMode(Led1, OUTPUT);                                              // Configura pino do led1 como saida
    Serial.begin(115200);                                              // Inicia a serial com 115200 bps
    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP");
     
    Serial.println("connected...yeey :)");                                       // Envia pela serial que esta conectando
    Serial.println("local ip");
    Serial.println(WiFi.localIP());
    WhatsAppMessages("Testando WhatsApp Bot !");
   
}

//-----------------------------------------------------------------------------------------------------------------------------------------

void loop() 
{
    int Porta = digitalRead(Sensor);                                    // Faz a leitura do sensor

    if (Porta == 1)                                                     // Porta acionada?
    {
        if (flag)                                                       // Primeira vez acionado?
        {
            Serial.println("ALARME ACIONADO !");                        // Sim, então escreve na serial que foi acionado
            WhatsAppMessages("ALARME ACIONADO, A PORTA FOI ABERTA");                      // Envia a mensagem para o Whatsapp que foi acionado
            flag = 0;                                                   // Zera o flag
        }
    } 
    else if(flag == 0)
    {
        Serial.println("");                                                 // Pula uma linha
        Serial.println("TUDO NORMAL !");
        WhatsAppMessages("A PORTA FOI FECHADA");                                // Sim, então escreve na serial que esta tudo normal
        flag = 1;                                                       // Não esta acionada, então reinicia o flag
    }
    delay(250);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// Definição das funções

void WhatsAppMessages(String message)
{
    // Dados a serem enviados pelo método Post suportado pelo protocolo HTTP
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
    WiFiClient client;
    HTTPClient https;                                                            // Cria um objeto HTTP
    https.begin(client, url);                                                            // Inicia o objeto HTTP passando a string acima
    
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");        // Adiciona um cabeçalho
  
    int httpResponseCode = https.POST(url);                                 // Envia o método POST HTTP de requisição e lê o resultado
  
    if (httpResponseCode == 200)                                                // Deu tudo certo?
    {
        digitalWrite(Led1, HIGH);                                               // Liga o led1
        Serial.print("Mensagem enviada com sucesso!");                          // Envia mensagem na serial que deu certo
        delay(1000);                                                            // Espera 1s
        digitalWrite(Led1, LOW);                                                // Desliga o led1
    }
    else                                                                        // Caso contrário, se não foi possivel enviar a mensagem
    {
        Serial.println("Erro ao tentar enviar a mensagem!");                    // Envia para a serial a mensagem de erro
        Serial.print("Código HTTP: ");                                          // Vai enviar para a serial o código de erro que retornou
        Serial.println(httpResponseCode);                                       // Envia o código de erro
    }
    https.end();                                                                 // Finaliza o objeto http
}

