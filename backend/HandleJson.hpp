#ifndef __HANDLEJSON_H_
#define __HANDLEJSON_H_

#include <ArduinoJson.h>

extern StaticJsonDocument<250> json;


String createDeviceJson(String device, String status, float value, float safeDistance,String measureUnit, StaticJsonDocument<250>& jsonObject);
String createJson(String message,StaticJsonDocument<250>& jsonObject);

#endif