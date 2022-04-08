#include <DHT.h>

int SENSOR = 2;
int TEMPERATURA;
String mensaje_entrada;
String mensaje_salida;

DHT dht (SENSOR, DHT22);

 void setup(){
   Serial.begin(9600);
   dht.begin();
 }

void loop(){
  if(Serial.available()>0)
  {
      mensaje_entrada = Serial.readStringUntil('\n');
        if (mensaje_entrada.compareTo("GET_TEMPERATURA")==0)
        {
                float grad = ping(SENSOR);
                Serial.println(grad);
                mensaje_salida=String("TEMPERATURA="+String(grad,3));
        }
        else
          mensaje_salida="COMANDO DESCONOCIDO";
        Serial.println(mensaje_salida);
  }
}
float ping(int SENSOR){
   int TEMPERATURA;
   TEMPERATURA = dht.readTemperature();
   return TEMPERATURA;

}   
