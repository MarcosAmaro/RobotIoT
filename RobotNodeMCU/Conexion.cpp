#include "Conexion.h"

void conectarWifi() {
	//Inicio local, una vez que se conecta no hace falta dejarlo.
	WiFiManager wifiManager;
	wifiManager.setBreakAfterConfig(true);

	/*
		Acá si no hay conexiones te crea un nuevo AP donde hay que conectarse por celu o compu y ahí
		usamos el navegador para escanear el wifi y elegir a cuál conectarse
	*/
	if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
		delay(3000);
		ESP.reset();
		delay(5000);
	}
}