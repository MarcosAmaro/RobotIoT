#include <RobotNodeMCU.h>

extern volatile boolean readingJson = false;
extern volatile boolean sendingData = false;
extern volatile boolean leyendo = false;

extern PubSubClient client = PubSubClient();

void setup() {
  Serial.begin(9600);
  conectarWifi();
  setMQTTClient(callback);
  mqttConnect();
  suscribir(cmdControl);
  Wire.begin();
}

void loop() {
	if (!client.loop()) {
		eventoDetener(0);
		mqttConnect();
	}
	else {
		if (!leyendo) {
			pedirDistancia();
		}
	}
}
