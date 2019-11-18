#include <RobotArduinoUno.h>

volatile byte estado = MANUAL;
volatile byte distancia = 10; //Tomará la distancia del sensor de ultrasonido, inicio a 10 para que no se detenga en la primer lectura.
volatile byte distanciaMinima = 0; //Distancia mínima para detenerse
volatile bool puedeLeer = true; //Lee mientras no esté usando el I2C, si no muere el I2C.

void setup() {
  // Iniciamos el monitor serie para monitorear la comunicación
  Serial.begin(9600);
  
  // Inicia el sensor de distancia
  configurarSensorDistancia();

  // Unimos este dispositivo al bus I2C con dirección 1
  Wire.begin(ARDUINO_UNO);

  // Registramos el evento al recibir datos
  Wire.onReceive(receiveEvent);

  // Evento para mandar datos al Master
  Wire.onRequest(responderMaestro);
}

void loop() {
}
