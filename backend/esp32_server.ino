#include "WifiSettings.hpp"
#include "Server.hpp"
#include "HandleJson.hpp"

// Cria um task handler que ira gerenciar os estados das tarefas
// E as funções que serao passadas ao respectivos cores
TaskHandle_t Task1;
void TaskDaemon(void *pvParameters);
void TaskHandleConnectionLed(void *pvParameters);

void blinkLed(int pin);

void setup()
{
  Serial.begin(19200);
  Serial.flush();
  Serial.println("Iniciando Servidor...");
  Serial.println("Escutando na porta: 3333");
  Serial.println();
  connectToNetwork();
  setRoutes();

  // Led de indicação de status da rede
  pinMode(SERVER_ONLINE, OUTPUT);

  /* Trava as duas funções em nucleos de processamento diferentes.
     O nucleo 0 ira gerenciar a rotina do sensor
     O nucleo 1 ira gerenciar as rotinas de conexão e rotas da api
   */
  xTaskCreatePinnedToCore(TaskDaemon, "HandleDaemon", 10000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskHandleConnectionLed, "HandleConnectionLed", 10000, NULL, 2, NULL, 1);
}


/* Não precisaremos invocar a função já que o processo será gerenciado pela 
   rotina de multithread
 */
void loop()
{ 
  
}


void TaskHandleConnectionLed(void *pvParameters){
  (void)pvParameters;
  for(;;){
    handleConnections();
    blinkLed(SERVER_ONLINE);
  }
  
}

void TaskDaemon(void *pvParameters){
  (void)pvParameters;
  for(;;){
    sensorDaemon();
    vTaskDelay(1000);
  }
  
}

void blinkLed(int pin){
  vTaskDelay(50);
  digitalWrite(pin, HIGH);
  vTaskDelay(50);
  digitalWrite(pin, LOW);
}
