#include <Ultrasonic.h>
#include <iostream>

#include "Server.hpp"
#include "HandleJson.hpp"

// Define os pinos de trigger e echo do sensor
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);

// Define a porta onde o servidor ira escutar as requisiçoes
#define DEFAULT_SERVER_PORT 3333

// Cria um novo servidor web
WebServer server(DEFAULT_SERVER_PORT);

// Cria um novo objeto JSON
StaticJsonDocument<250> json;

typedef struct Sensor
{
    String deviceName;
    String status;
    float distance;
    float safeDistance;
    String measureUnit;
    boolean alert;
} Sensor;

extern Sensor sensor = {"device1", "online", 0.0, 30.0, "cm", 0};

// Responsavel pelo gerenciamento do sensor
void sensorDaemon()
{
    if (sensor.status == "online")
    {
        float distance = calculateDistance();
        sensor.distance = distance;

        (sensor.distance < sensor.safeDistance)
            ? sensor.alert = 1
            : sensor.alert = 0;
        Serial.print("[DEBUG]Sensor -> ");
        Serial.println(createDeviceJson(sensor.deviceName, sensor.status,
                                        sensor.distance, sensor.safeDistance, sensor.measureUnit, json));
    }
    else
    {
        Serial.print("[ERROR]Sensor desligado!");
    }
}

/*
 * Aqui configuramos o sensor conforme o objeto JSON enviado pelo cliente.
 * As informações contem: deviceName, status, safeDistance e measureUnit
 *
 */
void setDevice()
{
    // Atraves do metodo server.arg("plain"), obtemos o objeto JSON
    String jsonContent = server.arg("plain");

    // Criamos um objeto JSON que sera usado para a descontrução do objeto
    DynamicJsonDocument document(512);
    DeserializationError error = deserializeJson(document, jsonContent);
    String buf;

    if (error)
    {
        server.send(400, "application/json", createJson(error.c_str(), json));
    }

    JsonObject postObject = document.as<JsonObject>();

    if (server.method() == HTTP_POST)
    {
        if (postObject.containsKey("device") && postObject.containsKey("safeDistance"))
        {
            String buf;

            sensor.deviceName = document["device"].as<String>();
            sensor.safeDistance = document["safeDistance"];
            sensor.alert = (calculateDistance() < sensor.safeDistance) ? 1 : 0;

            serializeJson(document, buf);
            server.send(201, "application/json", buf);
            getClientStatus("/api/v1/reportaenchente/settings/setdevice");
            buf.clear();
        }
    }else{
        document["Erro"] = "Formato invalido";
        serializeJson(document, buf);
        server.send(400, "application/json", buf);
    }
}

/*
 * Função de calulo e conversão de centimetros por segundo para centimetros, já
 * que o sensor trabalha com pulsos por microsegundos.
 */
float calculateDistance()
{
    float cmMsec = 0;
    if (sensor.status == "online")
    {
        long microsec = ultrasonic.timing();
        cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
        return cmMsec;
    }
    return cmMsec;
}

// Função usada para retornar o tipo de requisição e a rota
void getClientStatus(String path)
{
    String statusCode;
    if(server.method() == HTTP_POST){
        statusCode = "POST";
    }else if(server.method() == HTTP_GET){
        statusCode = "GET";
    }else if(server.method() == HTTP_PUT){
        statusCode = "PUT";
    }else{
        statusCode = "UNKNOWN";
    }
    Serial.print("[" + statusCode + "]" + "");
    Serial.print(path);
    Serial.print(" IP:  ");
    Serial.println(server.client().remoteIP());
}

// Função da rota principal
void getStatus()
{
    getClientStatus("/api/v1/reportaenchente/stats");
    server.send(200, "application/json", createDeviceJson(sensor.deviceName, sensor.status, calculateDistance(), sensor.safeDistance, sensor.measureUnit, json));
}

// Função de pagina 404
void notFound()
{
    getClientStatus("/404");
    server.send(404, "application/json", createJson("Path not found", json));
}

// Cria as rotas de acesso ao servidor REST
void setRoutes()
{
    server.enableCORS();
    server.on("/api/v1/reportaenchente/stats", getStatus);
    server.on("/api/v1/reportaenchente/setdevice", HTTP_POST, setDevice);
    server.onNotFound(notFound);
    server.begin();
}

void handleConnections()
{
    server.handleClient();
}
