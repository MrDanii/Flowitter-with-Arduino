/*
 ********************************************************************************************
 *  Este codigo permite obtener la lectura de diferentes sensores y tomar acciones en       *
 *  en consecuencia utilizando un actuador                                                  *
 *                                                                                          *
 *  @autor Campos Magallanes Ana Luisa                                                      *
 *  @autor Davalos Romero Daniel                                                            *
 *  @autor Esparza Guevara Nestor Paul                                                      *
 ********************************************************************************************
*/

int valorActualLuz = 0; // Variable donde se almacena el valor de luminosidad
int valorActualHumedad = 0; // Variable donde se almacena el valor de la humedad
int valorActualTemperatura = 0; // Variable donde se almacena el valor de la temperatura

float luz; // Variable que nos servira para guardar el valor calculado de luminosidad que se tenga actualmente
float humedad; // Variable que nos servira para guardar el valor calculado de la humedad que se tenga actualmente
float temperatura; // Variable que nos servira para guardar el valor calculado de la temperatura que se tenga actualmente

// Variables utilizadas para lectura de datos de la fotorresistencia
const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int RC = 10;       //Resistencia calibracion en KΩ

#define sensorLuz A0     //Pin analogico de entrada del LDR (Fotorresistencia)
#define sensorHumedad A1 //Pin analogico de entrada del sensor de humedad de suelo
#define sensorTemperatura A2 //Pin analogico de entrada del sensor de temperatura LM35
#define motorRiego 4 //Pin digital de entrada del actuador

void setup() {
  pinMode(motorRiego, OUTPUT); //Definimos el tipo (entrada - salida) de nuestro actuador
  Serial.begin(9600); // Inicializamos nuestra comunicacion con el puerto serial a 9600 Baudios
}

/*
   Dentro del ciclo mandamos a llamar a los diferentes metodos para que cumplan
   con sus respectivas funcionalidades especificadas en cada metodo para lo que
   fueron programadas
*/
void loop() {
  calculaLuminosidad(); // Metodo que permite obtener el valor actual del sensor de la fotorresistencia
  calculaHumedad(); // Metodo que permite obtener el valor actual del sensor de humedad de suelo
  calculaTemperatura(); // Metodo que permite obtener el valor actual del sensor de temperatura LM35
  verificaActuador(); // Metodo que permite evalua los valores de los sensores utilizados
  delay(2000); // Esperamos un tiempo para repetir el loop
}

void calculaHumedad() {
  valorActualHumedad = analogRead(sensorHumedad); // Leemos el valor de la humedad actual

  humedad = (100 * valorActualHumedad) / 1024; // Calculamos el valor de la humedad actual con la formula
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" %");
  Serial.println();
}

void calculaLuminosidad() {
  /*
      Leemos el valor relativo de luminosidad actual y lo guardamos en una variable.
      El valor leido (desde Vcc) aumenta de manera proporcional con respecto a la luz recibida
  */
  valorActualLuz = analogRead(sensorLuz); // Leemos el valor de la luminosidad actual

  luz = ((long)valorActualLuz * A * 10) / ((long)B * RC * (1024 - valorActualLuz)); // Formula utilizada porque el LDR esta entre A0 y Vcc
  Serial.print("Luminosidad: ");
  Serial.print(luz);
  Serial.print(" Lux");
  Serial.println();
}

void calculaTemperatura() {
  valorActualTemperatura = analogRead(sensorTemperatura); // Leemos el valor de la temperatura actual

  temperatura = (5 * valorActualTemperatura * 100) / 1024; // Calculamos el valor de la temperatura actual con la formula
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C");
  Serial.println();
}

/*
   Metodo que nos permite activar o desactivar nuestro actuador a traves de condicionales
   despues de obtener la lectura de los valores actuales de nuestros sensores
*/
void verificaActuador() {
  if (humedad <= 40 && luz < 60 && temperatura < 20) {
    digitalWrite(motorRiego, HIGH); // Activamos el motor
    Serial.println("Regando");
  } else {
    digitalWrite(motorRiego, LOW); // Desactivamos el motor
    Serial.println("No regando");
  }

  if (humedad <= 30 && luz > 80 && temperatura > 25) {
    digitalWrite(motorRiego, HIGH); // Activamos el motor
    Serial.println("Regando2");
  } else {
    digitalWrite(motorRiego, LOW); // Desactivamos el motor
    Serial.println("No regando2");
  }
}
