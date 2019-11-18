#ifndef __ROBOTARDUINOUNO_H__
#define __ROBOTARDUINOUNO_H__

#define NBR_MOTORS 4 //Cantidad de motores
#define TRIGGER 10   //Pin digital 10 para el TRIGGER del sensor de distancia
#define ECHO 9   //Pin digital 9 para el ECHO del sensor de distancia
#define MAX_BYTE_VAL 255 //Maximo valor del tipo de dato byte

#include <Arduino.h>
#include <AFMotor.h> //Librería para controlar los motores
#include <ConfigComun.h>

/**
	Modo AUTO: seguirá ejecutando la última rutina que recibió.
	Modo MANUAL: ejecuta solamente un giro completo de cada motor.
*/
enum MODO {
	MANUAL,
	AUTO
};

/** Constantes para cada motor */
enum MOTOR {
	DEL_IZQ,
	DEL_DER,
	TRA_DER,
	TRA_IZQ
};

/** Variables globales para control */
extern volatile byte estado;
extern volatile byte distancia; 
extern volatile byte distanciaMinima; 
extern volatile bool puedeLeer;

/**
	Ejecuta la misma orden para todos los motores.
	orden: orden a ejecutar.
*/
void comandoSimpleMotores(byte orden);

/**
	Cambia la velocidad de los motores.
	velocidad: velocidad a la que van a girar los motores.
*/
void cambiarVelocidad(byte velocidad);

/**
	Cambia el modo de control del robot.
	Si cambia a modo MANUAL, detiene el robot y queda a la espera de instrucciones.
	modo: modo en el que quedará el robot.
*/
void cambiarModo(byte modo);

/**
	Detiene por completo al robot, sin importar el modo en el que esté.
	dummy: no se usa este parámetro, se pone para poder ser utilizado dentro de un array de funciones.
*/
void detener(byte dummy);

/**
	Mueve el robot hacia adelante.
	velocidad: velocidad a la que se moverá el robot.
*/
void avanzar(byte velocidad);

/**
	Mueve el robot hacia atrás.
	velocidad: velocidad a la que se moverá el robot.
*/
void retroceder(byte velocidad);

/**
	Gira el robot hacia la izquierda.
	velocidad: velocidad a la que se moverá el robot.
*/
void girarIzquierda(byte velocidad);

/**
	Gira el robot hacia la derecha.
	velocidad: velocidad a la que se moverá el robot.
*/
void girarDerecha(byte velocidad);

/**
	Cambia la distancia mínima a la que se detiene el robot.
	dm: la distancia mínima frente a un obstáculo a la que se va a detener el robot.
*/
void cambiarDistanciaMinima(byte dm);

/**
	Lee la distancia mediante el sensor de ultrasonidos.
	dummy: no se usa este parámetro, se pone para poder ser utilizado dentro de un array de funciones.
*/
void leerDistancia(byte dummy);

/**
	Cambia los comandos a modo manual.
*/
void comandoManual();

/**
	Función que se ejecuta cuando que se reciben datos del Maestro
	siempre que en el Maestro se ejecute la sentencia endTransmission
	recibirá toda la información que hayamos pasado a través de la sentencia Wire.write.
	howMany: cantidad de bytes recibidos.
 */
void receiveEvent(int howMany);

/**
	Envía datos de la Arduino a NodeMCU, para enviar más de uno crear un byte array.
	Envía la distancia solicitada por el Maestro.
*/
void responderMaestro();

/**
	Configura el sensor de distancia.
*/
void configurarSensorDistancia();

#endif