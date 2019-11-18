#ifndef __IBMUTILS_H__
#define __IBMUTILS_H__

/**	Seteamos el valor de esta constante antes de importar la librería
	ya que la librería trae un size de 128 lo cual es poco para el uso
	que va a tener el robot.
*/
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 512
#endif

/********************* Dirección I2C de NodeMCU y ARDUINO UNO *********************/
#define NODE_MCU 1
#define ARDUINO_UNO 2

/********************* Credenciales de IBM Watson para el dispositivo *********************/
#define ORG "xxxx" //ORGID que asigna ibm
#define DEVICE_TYPE "nodemcu"
#define DEVICE_ID "nodemcu_veh"
#define TOKEN "xxxxxx" //Token asignado en la configuración del dispositivo en IBM Watson

/********************* URI para Comandos y Eventos *********************/
#define CMD "iot-2/"
#define FMT "/fmt/json"

#include "Conexion.h"
#include <PubSubClient.h>

/********************* Setup del cliente MQTT *********************/
extern PubSubClient client;

/**
	Es la primera función que debe llamarse antes de usar mqttConnect.
	Setea los datos referentes al cliente MQTT.
	MQTT_CALLBACK_SIGNATURE: función callback que procesará los eventos recibidos.
*/
void setMQTTClient(MQTT_CALLBACK_SIGNATURE);

/**
	Antes de llamar a esta función debe llamarse a setMQQTClient.
	Se conecta al MQTT de la plataforma IoT.
*/
void mqttConnect();

/**
	Suscribe a un comando en la MQTT.
	comando: el comando al cual se suscribe.
*/ 
void suscribir(const char* comando);

/**
	Publica un evento en la MQTT
	topic: el topic a publicar.
	payload: el mensaje a publicar.
*/ 
void publicar(const char* topic, const char* payload);

#endif
