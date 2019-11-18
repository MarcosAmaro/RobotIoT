#ifndef __CONEXION_H__
#define __CONEXION_H__

#include <FS.h> //Este include debe ir siempre primero
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager o en el gestor de librerías poner WiFiManager

/**
	Función para conectar al WIFI.
	En primera instancia usa el AP guardado en la placa.
	Si no se puede conectar, da la opción de elegir el AP,
	conectándose a la AP AutoConnectAP y 
	a través de un navegador yendo a: 192.168.1.4
*/
void conectarWifi();

#endif
