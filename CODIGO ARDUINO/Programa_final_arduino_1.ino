#include <DHT.h>
#include<DHT_u.h>
#include <Servo.h>
int estado = 0;                // 0=cerrado 1=abierto
Servo servo13;                 // servo para la cerradura
Servo servo12;                 // servo para la puerta
int SENSOR = 2;
int TEMPERATURA;
int HUMEDAD;
String mensaje_entrada;
String mensaje_salida;
const int pinBuzzer = 6;
const int LED=3;
byte salida = 255;
int f = 0;


DHT dht (SENSOR, DHT22);

void setup() {
  Serial.begin(9600);
  dht.begin();
  servo13.attach(13);  
  servo12.attach(12); // Asocia el servo1 al pin 11, define el min y max del ancho del pulso
  servo13.write(3);
  servo12.write(0);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {



 
  if (Serial.available() > 0)
  {
    
    
    mensaje_entrada = Serial.readStringUntil('\n');
    if (mensaje_entrada.compareTo("GET_TEMPERATURA") == 0)
    {
      float grad = ping(SENSOR);
      Serial.println(grad);
       Serial.print("ºC ");
      mensaje_salida = String("TEMPERATURA=" + String(grad, 3));
    }
    if (mensaje_entrada.compareTo("GET_HUMEDAD") == 0)
    {
      float hum= ping(SENSOR);
      Serial.println(hum);
       Serial.print("% ");
      mensaje_salida = String("HUMEDAD=" + String(hum, 3));
    }
    
    if(mensaje_entrada.compareTo("ABRIR_PUERTA") == 0)
    {
        abierto();
       sonar();   // Ponemos en bajo(0V) el pin del buzzer
        analogWrite(LED,127); 
        
      mensaje_salida=String("PUERTA ABIERTA");
      delay(500);
    }

    if(mensaje_entrada.compareTo("CERRAR_PUERTA") == 0)
    {
      cerrado();
      sonar();    // Ponemos en bajo(0V) el pin del buzzer
      digitalWrite(LED,0);                    // Esperamos 4 segundos
      mensaje_salida=String("PUERTA CERRADA");
      delay(500);
    }
     Serial.println(mensaje_salida);
  }
  
}
float ping(int SENSOR) {
              int TEMPERATURA;
              int HUMEDAD;
              TEMPERATURA = dht.readTemperature();
              HUMEDAD=dht.readHumidity();
              return TEMPERATURA, HUMEDAD;

            }

 void abierto(){
              
              //lcd.setCursor(0,1);
              //lcd.print("Abierto            ");  // imprime en el LCD que esta abierta
              servo13.write(90);
              // Gira el servo a 30 grados  abre la puerta
              servo12.write(120);
            }
    
     void cerrado(){
             
              //lcd.setCursor(0,1);
              //lcd.print("Cerrado              ");  // imprime en el LCD que esta cerrada
              servo12.write(0);
            
              servo13.write(0);                // Gira el servo a 150 grados  cierra la puerta
            }
            
void sonar(){
      digitalWrite(pinBuzzer, HIGH);   // Ponemos en alto(5V) el pin del buzzer
      delay(1000);                     // Esperamos 1 segundo
      digitalWrite(pinBuzzer, LOW); 
      delay(1000); 
      digitalWrite(pinBuzzer, HIGH);
      delay(1000);
      digitalWrite(pinBuzzer, LOW);// Ponemos en bajo(0V) el pin del buzzer
}
 
