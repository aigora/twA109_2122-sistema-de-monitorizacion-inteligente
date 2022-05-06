#include <DHT.h>
#include <Servo.h>
int estado = 0;                // 0=cerrado 1=abierto
Servo servo11;                 // servo para la cerradura
Servo servo12;                 // servo para la puerta
int SENSOR = 2;
int TEMPERATURA;
const int EchoPin = 10; //
const int TriggerPin = 11; //
String mensaje_entrada;
String mensaje_salida;

DHT dht (SENSOR, DHT22);

void setup() {
  Serial.begin(9600);
  dht.begin();
  servo11.attach(11, 660, 1400);      // Asocia el servo1 al pin 11, define el min y max del ancho del pulso
  servo11.write(150);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

void loop() {
  if (Serial.available() > 0)
  {
    mensaje_entrada = Serial.readStringUntil('\n');
    if (mensaje_entrada.compareTo("GET_TEMPERATURA") == 0)
    {
      float grad = ping(SENSOR);
      Serial.println(grad);
      mensaje_salida = String("TEMPERATURA=" + String(grad, 3));
    }
    mensaje_entrada = Serial.readStringUntil('\n');
    if (mensaje_entrada.compareTo("GET_DISTANCIA") == 0)
    {
      float cm = ping(TriggerPin, EchoPin);
      mensaje_salida = String("DISTANCIA=" + String(cm, 3));
   
    }
  }
}
   float ping(int SENSOR) {
              int TEMPERATURA;
              TEMPERATURA = dht.readTemperature();
              return TEMPERATURA;

            }
  float ping(int TriggerPin, int EchoPin)
            {
              long duration, distanceCm;

              digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
              delayMicroseconds(4);
              digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
              delayMicroseconds(10);
              digitalWrite(TriggerPin, LOW);

              duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos

              distanceCm = duration * 10 / 292 / 2;  //convertimos a distancia, en cm
              return distanceCm;
            }
    void abierto(){
              if(estado==0){
              estado=1;
              //lcd.setCursor(0,1);
              //lcd.print("Abierto            ");  // imprime en el LCD que esta abierta
              servo11.write(30);                 // Gira el servo a 30 grados  abre la puerta
              servo12.write(120);
            }
              else{
              estado=0;
              //lcd.setCursor(0,1);
              //lcd.print("Cerrado              ");  // imprime en el LCD que esta cerrada
              servo12.write(120);
              servo11.write(150);                // Gira el servo a 150 grados  cierra la puerta
            }
            }
