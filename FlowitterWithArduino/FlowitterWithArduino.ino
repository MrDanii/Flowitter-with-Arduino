/*
 ********************************************************************************************
    Este codigo permite obtener la lectura de diferentes sensores y tomar acciones en
    en consecuencia utilizando un actuador
 *                                                                                          *
    @autor Campos Magallanes Ana Luisa
    @autor Dávalos Romero Daniel
    @autor Esparza Guevara Nestor Paul
 ********************************************************************************************
*/

#include <EtherCard.h> //Libreria para módulo ethernet

int valorActualLuz = 0; // Variable donde se almacena el valor de luminosidad
int valorActualHumedad = 0; // Variable donde se almacena el valor de la humedad
int valorActualTemperatura = 0; // Variable donde se almacena el valor de la temperatura

float luz; // Variable que nos servira para guardar el valor calculado de luminosidad que se tenga actualmente
float humedad; // Variable que nos servira para guardar el valor calculado de la humedad que se tenga actualmente
float temperatura; // Variable que nos servira para guardar el valor calculado de la temperatura que se tenga actualmente
int contador = 1; //Variable que controla el número de mensaje
boolean regando = false; //Variable que comprueba si se regó la planta

// Variables para concatenar las cadenas (con el valor de cada sensor) en el mensaje de Twitter
const char *tempText = "Temperatura: ";
const char *luzText = " Luminocidad: ";
const char *humText = " Humedad: ";
char tempChar[8];
char luzChar[8];
char humChar[8];
char contChar[16];

// Variables utilizadas para lectura de datos de la fotorresistencia
const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int RC = 10;       //Resistencia calibracion en KΩ

#define sensorLuz A0     //Pin analogico de entrada del LDR (Fotorresistencia)
#define sensorHumedad A1 //Pin analogico de entrada del sensor de humedad de suelo
#define sensorTemperatura A2 //Pin analogico de entrada del sensor de temperatura LM35
#define motorRiego 4 //Pin digital de entrada del actuador

// Definimos la clave única que obtumidos desde http://arduino-tweet.appspot.com/
#define TOKEN   "1069784063752499200-3xJvG60Z82BbuFln2qRmPivV1GTaDE"

// establecemos la dirección MAC
byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };

const char website[] PROGMEM = "arduino-tweet.appspot.com";

static byte session;            //variable para almacenar el ID de la sesión

byte Ethernet::buffer[700];     //buuffer de datos del mensaje de twitter
Stash stash;    //objeto usado para la creacion del mensaje a twitter



/*
   Método con la inicialización del módulo ethernet y el puerto Serial
*/
void setup() {
  pinMode(motorRiego, OUTPUT); //Definimos el tipo (entrada - salida) de nuestro actuador
  Serial.begin(9600); // Inicializamos nuestra comunicacion con el puerto serial a 9600 Baudios

  //Comprueba que se inicializa el módulo ethernet
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  //Imprime la información sobre la IP, el Gateway y el DNS
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  //Mensaje de error
  if (!ether.dnsLookup(website))
    Serial.println(F("DNS failed"));

  ether.printIp("SRV: ", ether.hisip);
}

/*
   Dentro del ciclo mandamos a llamar a los diferentes metodos encargados de
   vigilar cada sensor y el actuador que activa la bomba de agua
   vigilando la conexión a twitter
*/
void loop() {
  //Comprueba la conexión
  ether.packetLoop(ether.packetReceive());
  const char* reply = ether.tcpReply(session);

  //Imprime la respuesta de conexión, solo cuando se recibe respuesta de un twitt enviado por el arduino
  if (reply != 0) {
    Serial.println("Got a response!");
    Serial.println(reply);
  }

  calculaLuminosidad(); // Metodo que permite obtener el valor actual del sensor de la fotorresistencia
  calculaHumedad(); // Metodo que permite obtener el valor actual del sensor de humedad de suelo
  calculaTemperatura(); // Metodo que permite obtener el valor actual del sensor de temperatura LM35
  verificaActuador(); // Metodo que permite evalua los valores de los sensores utilizados y envía un twitt si la planta se está regando
  delay(2000); // Esperamos un tiempo para repetir el loop
}

/**
   Método que guarda el valor de la humedad del suelo en la variable valorActualHumedad, e imprime
   los valores por medio del puerto Serial
*/
float calculaHumedad() {
  valorActualHumedad = analogRead(sensorHumedad); // Leemos el valor de la humedad actual

  //humedad = (100 * valorActualHumedad) / 1024; // Calculamos el valor de la humedad actual con la formula
  Serial.print("Humedad: ");
  Serial.print(valorActualHumedad);
  Serial.print(" %");
  Serial.println();
  return valorActualHumedad;
}

/**
   Método que guarda el valor de la luminosidad en la variable luz, e imprime
   los valores por medio del puerto Serial
*/
float calculaLuminosidad() {
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
  return luz;
}

/**
   Método que guarda el valor de la temperatura en grados centigrados en la variable temperatura, e imprime
   los valores por medio del puerto Serial
*/
float calculaTemperatura() {
  valorActualTemperatura = analogRead(sensorTemperatura); // Leemos el valor de la temperatura actual

  temperatura = (5 * valorActualTemperatura * 100) / 1024; // Calculamos el valor de la temperatura actual con la formula
  temperatura = abs(temperatura);
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C");
  Serial.println();
  return temperatura;
}

/*
   Metodo que nos permite activar o desactivar nuestro actuador (bomba) a traves de condicionales
   despues de obtener la lectura de los valores actuales de nuestros sensores
*/
void verificaActuador() {
  // valores que activan la bomba dependiendo de la luz y de la humedad del suelo actuales
  if (luz > 80 && valorActualHumedad > 600) {
    digitalWrite(motorRiego, HIGH); // Activamos el motor
    Serial.println("Regando");
    regando = true;
  } else {
    if (regando) {

      //Proceso para concatenar las cadenas de arreglos de char
      // *************************************************** //
      char mensajeFinal[100];   //variable que guardará toda la cadena del twitt

      //Obtenemos todos los valores actuales de los sensores
      temperatura = calculaTemperatura();
      luz = calculaLuminosidad();
      humedad = calculaHumedad();

      dtostrf(temperatura, 8, 2, tempChar);

      strcpy(mensajeFinal, tempText);
      strcat(mensajeFinal, tempChar);

      dtostrf(luz, 8, 2, luzChar);

      strcat(mensajeFinal, luzText);
      strcat(mensajeFinal, luzChar);

      dtostrf(humedad, 8, 2, humChar);

      strcat(mensajeFinal, humText);
      strcat(mensajeFinal, humChar);

      Serial.println(mensajeFinal); //imprimimos el mensaje por el puerto serial

      //Enviamos a twitter la cadena almacenada en "mensajeFinal"
      sendToTwitter(mensajeFinal); //Envia el mensaje

      contador++;
      regando = false;
    }
    digitalWrite(motorRiego, LOW); // Desactivamos el motor
    Serial.println("No regando");
  }
}

/*
  Método que se encarga de enviar el mensaje a twitter
  @param tweet arreglo de char con el mensaje a enviar a twitter
*/
static void sendToTwitter (char tweet[]) {
  Serial.println("Sending tweet...");
  byte sd = stash.create();

  //  const char tweet[] = "Texto que se envia a twitter";
  stash.print("token=");
  stash.print(TOKEN);
  stash.print("&status=");
  stash.println(tweet);
  stash.save();
  int stash_size = stash.size();

  // Construimos la petición POST con el protocolo http y
  // adjuntamos el stash previamente creado en el encabezado "sd"
  Stash::prepare(PSTR("POST http://$F/update HTTP/1.0" "\r\n"
                      "Host: $F" "\r\n"
                      "Content-Length: $D" "\r\n"
                      "\r\n"
                      "$H"),
                 website, website, stash_size, sd);

  // enviamos los paquetes - Esto también realiza todos los buffers almcenados una vez que termine
  // Guardamos el ID de la sesión, para así poder visualizarlo en el ciclo loop
  session = ether.tcpSend();
}
