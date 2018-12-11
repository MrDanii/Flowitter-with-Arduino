# Flowitter-with-Arduino

## Descripción del proyecto

El presente proyecto consiste en un sistema de riego automatico el cuál cuenta con un sensor de humedad del suelo, un sensor de temperatura y un sensor de luz para establecer cuando activar y desactivar una bomba de agua que riega una planta.

El sitema además cuenta con conexión a internet para hacer una publicación en Twitter con los resultados de la temperatura, la luz, y la humedad.

## Información general de los autores

> **Universidad:** Instituto Tecnológico de León
>
> **Carrera:** Ingeniería en Sistemas Computacionales
>
> **Materia:** Sistemas Programables
>
> **Periodo:** Agosto-Diciembre 2018
>
> **Profesor:** Ing. Levy Rojas Carlos Rafael
> 
 ### Integrantes:
 * Campos Magallanes Ana Luisa - [AniluCampos](https://github.com/AniluCampos)
 * Dávalos Romero Daniel - [MrDanii](https://github.com/MrDanii)
 * Esparza Guevara Nestor Paul - [PaulEsparza](https://github.com/PaulEsparza)

## Pre-requisitos 
### Materiales
> * Protoboard
> * Arduino UNO
> * Sensor de humedad del suelo
> * Fotoresistencia
> * Sensor temperatura
> * Mini bomba de agua
> * Manguera de nivel
> * Driver L298 Dual Puente-H Motor
> * Controlador de ethernet ENC28J60
> * Cable ethernet

![](/Imagenes/materiales.jpg)
 
 ### Librerías Arduino
 **<EtherCard.h>** para módulo de ethernet - disponible [aquí](https://github.com/njh/EtherCard)
 
 ## Procedimiento
 
 **1. Conectar el módulo ethernet de la siguiente manera**

 ![](/Imagenes/moduloesquema.png)
 ![](/Imagenes/moduloesquema2.jpg)

 **2. Conectar el cable ethernet al modulo y al router**

 **3. Conectar la bomba de agua en el pin digital 4 de la siguiente manera**

 ![](/Imagenes/bombaesquema.jpg)

 **4. Conectar a la bomba la manguera de nivel y sumergirla en agua**

 **5. Conectar con ayuda del protoboard la resistencia al pin A0, el sensor de tierra al pin A1 y el sensor de temperatura al pin A2**

 **6. Insertar en la maceta el sensor de humedad y la manguera de nivel**

 **7. Importar la libreria en el IDE de arduino**
 
 **8. Descargar y ejecutar el código SensoresConActuador.ino en el IDE de arduino**

## Resultados

 ![](/Imagenes/fotofinal.jpg)
 ![](/Imagenes/capturafinal.jpg)

