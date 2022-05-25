#ifndef __SERVER_H_
#define __SERVER_H_

#include <WebServer.h>

#define SERVER_ONLINE 32
#define PIN_TRIGGER 4
#define PIN_ECHO 5

void sensorDaemon();
void getClientStatus(String path);
void getStatus();
void setRoutes();
void handleConnections();
float calculateDistance();

#endif