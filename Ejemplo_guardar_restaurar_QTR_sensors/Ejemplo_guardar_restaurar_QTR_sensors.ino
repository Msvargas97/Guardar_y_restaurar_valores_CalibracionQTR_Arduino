/*
"Conocimiento que no se comparte, se pudre."

https://github.com/Msvargas97/Guardar_y_restaurar_valores_CalibracionQTR_Arduino

Usted puede modificar libremente y compartir el código, siempre y cuando mantenga el enlace
de referencia y este comentario inicial...

Guardar y restaurar calibracion en sensores QTR By Michael Vargas
https://github.com/Msvargas97
*/

//Añadir librerias necesarias
#include "EEPROMex.h"
#include "EEPROMVar.h"
#include <QTRSensors.h>

// Configurar según el tipo de sensores
#define NUM_SENSORS   8    
#define TIMEOUT       2500  
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     

QTRSensorsRC qtrrc((unsigned char[]) { 
  2, 3, 4, 5, 6, 7, 8, 9 }
  ,
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 

unsigned int sensorValues[NUM_SENSORS];

void guardarCalibracion(){

  for (int i = 0; i < NUM_SENSORS; i++)
  {                                                     //Gracias al uso de punteros por parte de la libreria se puede realizar lo siguiente
    EEPROM.writeInt(i*2,qtrrc.calibratedMinimumOn[i]);  //Guardar los valores en la EEPROM cada 2 slots gracias a la libreria 
    EEPROM.writeInt((i*2)+(NUM_SENSORS*2),qtrrc.calibratedMaximumOn[i]); //ya que en Arduino el int es de 2 Bytes y la EEMPROM almacena 1 byte por cada slot
  }

}

void monitorSerial(){
// Imprimir en pantalla el resultado
  Serial.println();
  Serial.print("Valores Minimos guardados en EEPROM-> ");
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(EEPROM.readInt(i*2));
    Serial.print(' ');
  }
  Serial.println(' ');
  Serial.print("Valores Maximos guardados en EEPROM-> ");

  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(EEPROM.readInt((i*2)+(NUM_SENSORS*2)));
    Serial.print(' ');
  }

  Serial.println();
  Serial.println();

  Serial.print("Valores minimos de QTR-> ");
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println(' ');
  Serial.print("Valores maximos de QTR-> ");

  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();

}
void restaurarCalibracion(){  

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    qtrrc.calibratedMinimumOn[i] = EEPROM.readInt(i*2); //Asignar los valores guardados en la EEPROM en los valores de calibracion
    qtrrc.calibratedMaximumOn[i] = EEPROM.readInt((i*2)+(NUM_SENSORS*2));
  }

}

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    
  for (int i = 0; i < 200; i++) // Se calibran los sensores durante 5 segundos
  {
    qtrrc.calibrate();     
  }
  digitalWrite(13, LOW);     
  Serial.println("");
  
  guardarCalibracion();
  Serial.println("Se guardan y reinicia los valores de calibracion: ");
  qtrrc.resetCalibration(); // Reiniciamos la calibracion dandole un valor de 0 para comprobar que si se restauran los valores desde la EEPROM
  monitorSerial();
  delay(3000);
  Serial.println("\nSe restauran los valores de calibracion para comprobar que se restauran desde la EEPROM de Arduino: \n ");
  restaurarCalibracion(); // se puede usar con algun boton 
  monitorSerial();
  delay(5000);
}

void loop()
{

// Verificar que se lee los valores con normalidad
  unsigned int position = qtrrc.readLine(sensorValues);

  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }

  Serial.println(position);

  delay(250);
}


