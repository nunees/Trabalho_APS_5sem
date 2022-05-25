#include "HandleJson.hpp"

char buffer[250];

 // Cria o objeto JSON que sera enviado ao cliente baseado nos parametros do sensor
 String createDeviceJson(String device, String status, float value, float safeDistance, 
        String measureUnit, StaticJsonDocument<250>& jsonObject){
    jsonObject.clear();
    jsonObject["device"] = device;
    jsonObject["status"]  = status;
    jsonObject["distance"] = value;
    jsonObject["safeDistance"] = safeDistance;
    jsonObject["measureUnit"] = measureUnit;
    jsonObject["alert"] = (value < safeDistance) ? 1 : 0;
    serializeJson(jsonObject, buffer);
    return buffer;
}

// Cria um objeto JSON baseado em uma mensagem e envia ao cliente
String createJson(String message, StaticJsonDocument<250>& jsonObject){
    char buffer[250];
    jsonObject.clear();
    jsonObject["Error"] = message;
    serializeJson(jsonObject, buffer);
    return buffer;
}
