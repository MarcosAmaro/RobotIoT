#include "RobotArduinoUno.h"

AF_DCMotor motor[NBR_MOTORS] = { AF_DCMotor(1),AF_DCMotor(2), AF_DCMotor(3),AF_DCMotor(4) }; //Motores
const byte PESO = 20; //Agrega velocidad por el peso trasero

/**
	None.
*/
void dummy(byte dmy);

// Punteros a las funciones del Arduino UNO
void(*fncAccion[ACCION_TAM])(byte) = {
	detener,
	avanzar,
	retroceder,
	girarIzquierda,
	girarDerecha,
	cambiarModo,
	cambiarDistanciaMinima,
	leerDistancia,
	dummy
};

void comandoSimpleMotores(byte orden) {
	for (byte i = 0; i < NBR_MOTORS; i++) {
		motor[i].run(orden);
	}
}

void cambiarVelocidad(byte velocidad) {
	for (byte i = 0; i < (NBR_MOTORS - 2); i++) {
		motor[i].setSpeed(velocidad);
	}
	
	for (byte i = 2; i < NBR_MOTORS; i++) {
		motor[i].setSpeed((velocidad + PESO));
	}
}

void cambiarModo(byte modo) {
	estado = modo;
	if (modo == MANUAL) {
		detener(0);
	}
}

void detener(byte dummy) {
	cambiarVelocidad(0);
	comandoSimpleMotores(RELEASE);
}

void avanzar(byte velocidad) {
	if (distancia > distanciaMinima) {
		cambiarVelocidad(velocidad);
		comandoSimpleMotores(FORWARD);
		comandoManual();
	}
}

void retroceder(byte velocidad) {
	cambiarVelocidad(velocidad);
	comandoSimpleMotores(BACKWARD);
	comandoManual();
}

void girarDerecha(byte velocidad) {
	detener(0);
	motor[DEL_DER].setSpeed(velocidad);
	motor[TRA_DER].setSpeed((velocidad + PESO));
	motor[DEL_DER].run(BACKWARD);
	motor[TRA_DER].run(BACKWARD);
	comandoManual();
}

void girarIzquierda(byte velocidad) {
	detener(0);
	motor[DEL_IZQ].setSpeed(velocidad);
	motor[TRA_IZQ].setSpeed((velocidad + PESO));
	motor[DEL_IZQ].run(BACKWARD);
	motor[TRA_IZQ].run(BACKWARD);
	comandoManual();
}

void cambiarDistanciaMinima(byte dm) {
	distanciaMinima = dm;
}

void comandoManual() {
	if (estado == MANUAL) {
		delay(50000); //Esperar al último giro
		detener(0);
	}
}

void receiveEvent(int howMany) {
	if (howMany >= 1) {
		puedeLeer = false;
		byte accion = Wire.read();
		if (accion < ACCION_TAM) {
			fncAccion[accion](Wire.read());
		}

		puedeLeer = true;
	}
}

void responderMaestro() {
	if (puedeLeer) {
		Wire.write(distancia);
	}
	else {
		Wire.write(1);
	}
}

void configurarSensorDistancia() {
	pinMode(TRIGGER, OUTPUT); //pin como salida
	pinMode(ECHO, INPUT);  //pin como entrada
	digitalWrite(TRIGGER, LOW);//Inicializamos el pin con 0
	leerDistancia(0);
}

void leerDistancia(byte dummy) {
	long t; //tiempo que tarda en recibir el echo
	long d; //distancia en centimetros
   
	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10); //Se envía pulso de 10us
	digitalWrite(TRIGGER, LOW);

	t = pulseIn(ECHO, HIGH); //se obtiene el tiempo del echo
	d = t * 10 / 294/ 2; //calcula la distancia
	delay(100); //Pausa sanitaria

	//Para enviar solo un byte, si la distancia es mayor al máximo valor del byte, enviamos el máximo valor.
	if (d > MAX_BYTE_VAL) {
		distancia = MAX_BYTE_VAL;
	}else {
		distancia = (byte)d;
	}

	// Detiene el robot si está a la distancia mínima o menos.
	if (distancia <= distanciaMinima) {
		detener(0);
	}
}

/**
	None.
*/
void dummy(byte dmy){
}