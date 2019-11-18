#ifndef __ROBOTNODEMCU_H__
#define __ROBOTNODEMCU_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include "ConfigComun.h"
#include "IBMUtils.h"

/********************* Eventos y comandos del dispositivo IOT en la plataforma *********************/
const char distanceTopic[] = CMD "evt/distancia" FMT;
const char cmdControl[] = CMD "cmd/control" FMT;

/********************* Control para envío de ordenes *********************/
extern volatile boolean readingJson;
extern volatile boolean sendingData;
extern volatile boolean leyendo;

/**
	Procesa el mensaje que llega desde la plataforma IoT. 
	topic: El tópico del mensaje.
	payload: El mensaje recibido.
	payloadLength: La Longitud del mensaje.
*/
void callback(char* topic, byte* payload, unsigned int payloadLength);

/**
	Solicita la lectura de la distancia a la ARDUINO UNO.
*/
void pedirDistancia();

/**
	Envía la órden de detener el robot a la ARDUINO UNO.
	dummy: no se usa este parámetro, se pone para poder ser utilizado dentro de un array de funciones.
*/
void eventoDetener(byte dummy);

/**
	Envía la órden de avanzar el robot a la ARDUINO UNO.
	velocidad: la velocidad a la que va a avanzar el robot.
*/
void eventoAvanzar(byte velocidad);

/**
	Envía la órden de retroceder el robot a la ARDUINO UNO.
	velocidad: la velocidad a la que va a retroceder el robot.
*/
void eventoRetroceder(byte velocidad);

/**
	Envía la órden de girar el robot hacia la izquierda a la ARDUINO UNO.
	El giro es de aproximadamente 45 grados.
	velocidad: la velocidad a la que va a girar el robot.
*/
void eventoGirarIzquierda(byte velocidad);

/**
	Envía la órden de girar el robot hacia la derecha a la ARDUINO UNO.
	El giro es de aproximadamente 45 grados.
	velocidad: la velocidad a la que va a girar el robot.
*/
void eventoGirarDerecha(byte velocidad);

/**
	Envía la órden de cambiar el modo del robot a la ARDUINO UNO.
	Los modos disponibles son MANUAL (el robot ejecuta las órdenes una vez)
	y modo AUTOMATICO (el robot recibe la orden y la sigue ejecutando automáticamente).
	modo: el modo en que se va a ejecutar el robot.
*/
void eventoCambiarModo(byte modo);

/**
	Envía la órden a la ARDUINO UNO de cambiar la distancia mínima
	a la que el robot se va a detener frente a un obstáculo.
	dm: la distancia mínima frente a un obstáculo a la que se va a detener el robot.
*/
void eventoCambiarDistanciaMinima(byte dm);

/**
	Ejecuta un delay dentro de la NodeMCU para esperar a ejecutar el
	próximo comando recibido desde la plataforma.
	d: el delay expresado en centenas.
*/
void ejecutarDelay(byte d);

/**
	Envía la distancia detectada por el robot hacia la plataforma IoT.
	dist: la distancia a la que el robot se encuentra frente a un obstáculo.
*/
void enviarDistancia(byte dist);

#endif