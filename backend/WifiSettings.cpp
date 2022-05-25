#include <HardwareSerial.h>
#include <WiFi.h>
 
#include "WifiSettings.hpp"
#include "Server.hpp"


void connectToNetwork(){
  Serial.print("Conectando-se a rede ");
  Serial.print(DEFAULT_SSID_NAME);


  WiFi.begin(DEFAULT_SSID_NAME, DEFAULT_SSID_KEY);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }

  Serial.println("");
  Serial.println("Conectado com sucesso!");
  displayConnectionStatus();

}


// Somente para depuracao do codigo via console serial
void displayConnectionStatus(){
  Serial.println("===========================");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println();
  Serial.print("NetMask: ");
  Serial.print(WiFi.subnetMask());
  Serial.println();
  Serial.print("Gateway: ");
  Serial.print(WiFi.gatewayIP());
  Serial.println();
  Serial.println("===========================");
}