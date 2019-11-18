#include "RobotNodeMCU.h"

/********************* Setup para JSON *********************/
const size_t TAM_BUFFER_JSON = 255;
DynamicJsonBuffer jsonBuffer(TAM_BUFFER_JSON);

/********************* Control de envío de datos *********************/
volatile byte distanciaAnterior = 1;
volatile byte distanciaMinima = 0;
volatile byte conObstaculo = 0; //Envia solo cuando cambió o se leyó por encima del umbral.
const byte UMBRAL_DISTANCIA = 10; //Debe superar el umbral para no mandar distancia cada rato
/**
	None.
*/
void dummy(byte dmy);

// Punteros a las funciones del NodeMCU
void(*fncAccion[ACCION_TAM])(byte) = {
	eventoDetener,
	eventoAvanzar,
	eventoRetroceder,
	eventoGirarIzquierda,
	eventoGirarDerecha,
	eventoCambiarModo,
	eventoCambiarDistanciaMinima,
	dummy,
	ejecutarDelay
};

// Los comandos que se van a aceptar desde la plataforma.
String comandos[ACCION_TAM] = {
	"detener",
	"avanzar",
	"retroceder",
	"girarIzquierda",
	"girarDerecha",
	"cambiarModo",
	"distancia",
	"pedirDistancia",
	"delay"
};

/**
	Función auxiliar para obtener el comando a ejecutar en caso de haber recibido uno válido.
	orden: el comando recibido desde la plataforma.
*/
byte obtenerOrden(String orden);

void callback(char* topic, byte* payload, unsigned int payloadLength) {
	if (!readingJson) {
		String json;
		for (int i = 0; i < payloadLength; i++) {
			json.concat((char)payload[i]);
		}

		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			Serial.println("Error al parsear el mensaje JSON");
		}
		else {
			readingJson = true;
			for (JsonObject::iterator it = root.begin(); it != root.end(); ++it)
			{
				String key = it->key;
				byte i = obtenerOrden(key);
				if (i != -1) {
					fncAccion[i](root[key].as<byte>());
					delay(200);
				}
			}

			readingJson = false;
		}
		
		jsonBuffer.clear();
	}
}

void pedirDistancia() {
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	// Enviamos un byte, será la orden de leer la distancia
	Wire.write(LEER_DISTANCIA);
	Wire.write(0);

	// Paramos la transmisión
	Wire.endTransmission();
	delay(100);

	// Pido la distancia leída
	Wire.begin();
	Wire.requestFrom(ARDUINO_UNO, 1);
	while (Wire.available()) {
		byte b = Wire.read();

		// Si la distancia es 1, no enviará nada. Las anteriores órdenes todavía están vigentes.
		if (b != 1 && distanciaMinima > 0) {
			bool enviar = false;

			//Si sigue por debajo de la distancia minima
			if (distanciaAnterior <= distanciaMinima && b <= distanciaMinima){
				if (conObstaculo == 0){
					enviar = true;
				}else if (conObstaculo > UMBRAL_DISTANCIA){
					enviar = true;
					conObstaculo = 0;
				}
				conObstaculo++;
			}
			
			//Si antes estaba por encima y ahora por debajo
			if (distanciaAnterior > distanciaMinima && b <= distanciaMinima) {
				enviar = true;
				conObstaculo++;
			}

			//Si antes estaba por debajo y ahora por encima
			if (distanciaAnterior <= distanciaMinima && b > distanciaMinima) {
				enviar = true;
				conObstaculo = 0;
			}

			distanciaAnterior = b;

			if (enviar) {
				enviarDistancia(b);
			}
		}
	}
	
	leyendo = sendingData;
}

void eventoDetener(byte dummy) {
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	Wire.write(DETENER);
	Wire.write(0);

	// Paramos la transmisión
	Wire.endTransmission();
}

void eventoAvanzar(byte velocidad) {
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	Wire.write(AVANZAR);
	Wire.write(velocidad);

	// Paramos la transmisión
	Wire.endTransmission();
}

void eventoRetroceder(byte velocidad) {
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	Wire.write(RETROCEDER);
	Wire.write(velocidad);

	// Paramos la transmisión
	Wire.endTransmission();
}

void eventoGirarIzquierda(byte velocidad) {
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	Wire.write(GIRAR_IZQUIERDA);
	Wire.write(velocidad);

	// Paramos la transmisión
	Wire.endTransmission();
}

void eventoGirarDerecha(byte velocidad) {
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	Wire.write(GIRAR_DERECHA);
	Wire.write(velocidad);

	// Paramos la transmisión
	Wire.endTransmission();
}

void eventoCambiarModo(byte modo) {
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	Wire.write(CAMBIAR_MODO);
	Wire.write(modo);

	// Paramos la transmisión
	Wire.endTransmission();
}

void eventoCambiarDistanciaMinima(byte dm) {
	distanciaMinima = dm;
	// Comenzamos la transmisión al dispositivo Arduino
	Wire.beginTransmission(ARDUINO_UNO);

	Wire.write(DISTANCIA);
	Wire.write(dm);

	// Paramos la transmisión
	Wire.endTransmission();
}

void ejecutarDelay(byte d) {
	delay((d*100));
}

void enviarDistancia(byte dist) {
	sendingData = true;

	String payload = "{\"d\":{\"distancia\":";
	payload += String(dist, DEC);
	payload += "}}";
	
	publicar(distanceTopic, payload.c_str());

	sendingData = false;
}

byte obtenerOrden(String orden) {
	byte i = -1;

	bool encontrado = false;
	byte j = 0;
	while (j < ACCION_TAM && !encontrado) {
		if (comandos[j] == orden) {
			i = j;
			encontrado = true;
		}

		j++;
	}

	return i;
}

/**
	None.
*/
void dummy(byte dmy){
}