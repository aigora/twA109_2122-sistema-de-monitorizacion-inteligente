#include <DHT.h>


int SENSOR = 2;
int TEMPERATURA;

DHT dht (SENSOR, DHT22);

 void setup(){
   Serial.begin(9600);
   dht.begin();
 }

void loop(){
   TEMPERATURA = dht.readTemperature();
   Serial.print("Temperatura: ");
   Serial.print(TEMPERATURA);
   delay(500); 
}   
