#include "Conexion.h"
#include "IBMUtils.h"

/********************* Constantes para la conexión a IBM *********************/
const char SERVER[] = ORG "ORG_URL"; //la dirección asignada por IBM
const char AUTH_METHOD[] = "use-token-auth";
const char CLIENT_ID[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
const short int PORT = 443;

// Conexión segura
WiFiClientSecure clienteWifi;

void setMQTTClient(MQTT_CALLBACK_SIGNATURE){
	client.setServer(SERVER, PORT);
	client.setCallback(callback);
	client.setClient(clienteWifi);
}

void mqttConnect() {
	if (!!!client.connected()) {
		Serial.print("Conectando al cliente MQTT "); Serial.println(SERVER);
		
		//Si no se pudo conectar, espera un momento.
		while (!!!client.connect(CLIENT_ID, AUTH_METHOD, TOKEN)) {
			Serial.print(".");
			delay(500);
		}
	}
}

void suscribir(const char* comando){
	if (client.subscribe(comando)) {
		Serial.print("Suscrito a: "); Serial.println(comando);
	}else{
		Serial.println("Error al suscribirse en: "); Serial.println(comando);
	}
}

void publicar(const char* topic, const char* payload){
	if (client.publish(topic, payload)) {
		Serial.print("Publicado: "); Serial.println(topic);
		Serial.print("Payload: "); Serial.println(payload);
	}
	else {
		Serial.print("Error al publicar: "); Serial.println(topic);
	}
}