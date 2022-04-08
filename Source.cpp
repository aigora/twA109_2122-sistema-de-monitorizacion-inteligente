#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include "SerialClass/SerialClass.h"

#define MAX_BUFFER 200
#define PAUSA_MS 200 

int menu_ppal(void);
void verifica_Temperatura(Serial*, char*);
void comprobar_mensajes(Serial*);
float leer_sensor_Temperatura(Serial*);
int Enviar_y_Recibir(Serial*, const char*, char*);
float float_from_cadena(char* cadena);

int menu_ppal(void) {
	static int opcion = -1;
	if (opcion != 0) {

		printf("MiNevera\n");
		printf("-------------------\n");
		printf("Elija una opcion:\n");
		printf("1-Estado de los componentes\n");
		printf("2-Temperatura\n");
		printf("3-Alarmas\n");
		printf("4-Control de los sensores\n");
		printf("5-Registros\n");
		printf("6-Salir\n");
		printf("opcion:");
	}

if (_kbhit())
{
	opcion = (int)_getch() - '0';
	printf("%d\n", opcion);
}
else
opcion = 0;
return opcion;

}

int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM8"; // Puerto serie al que est� conectado Arduino
	int opcion_menu;
	setlocale(LC_ALL, "es-ES");
	Arduino = new Serial((char*)puerto);
	do
	{
		opcion_menu = menu_ppal();
		switch (opcion_menu)
		{
		case 1:
			printf("todav�a estamos trabajando en ello,disculpe las molestias.\n");
			break;
		case 2:
			verifica_Temperatura(Arduino, puerto);
			break;
		case 3:
			printf("todav�a estamos trabajando en ello,disculpe las molestias.\n");
			break;
		case 4:
			printf("todav�a estamos trabajando en ello,disculpe las molestias.\n");
			break;
		case 5:
			printf("todav�a estamos trabajando en ello,disculpe las molestias.\n");
			break;
		case 6:
			break;
		//default: printf("\nOpci�n incorrecta\n");
		}
	} while (opcion_menu != 6);
	return 0;
}

	void verifica_Temperatura(Serial * Arduino, char* port)
	{
		float temperatura;
		if (Arduino->IsConnected())
		{
			temperatura = leer_sensor_Temperatura(Arduino);
			if (temperatura != -1)
				printf("\nTemperatura: %f\n", temperatura);
		}
		else
		{
			printf("\nNo se ha podido conectar con Arduino.\n");
			printf("Revise la conexi�n, el puerto %s y desactive el monitor serie delIDE de Arduino.\n",port);
		}
	}
	float leer_sensor_Temperatura(Serial* Arduino)
	{
		float temperatura;
		int bytesRecibidos;
		char mensaje_recibido[MAX_BUFFER];

		bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_TEMPERATURA\n", mensaje_recibido);
		if (bytesRecibidos <= 0)
		{
			printf("\nNo se ha recibido respuesta a la petici�n\n");
			temperatura = -1;
		}
		else
		{
			printf("\nLa respuesta recibida tiene %d bytes. Recibido=%s\n", bytesRecibidos,
				mensaje_recibido);
			temperatura = float_from_cadena(mensaje_recibido);
		}
		return temperatura;
	}
	int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
	{
		int bytes_recibidos = 0, total = 0;
		int intentos = 0, fin_linea = 0;
		Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
		Sleep(PAUSA_MS);
		bytes_recibidos = Arduino->ReadData(mensaje_recibir, sizeof(char) * MAX_BUFFER - 1);
		while ((bytes_recibidos > 0 || intentos < 5) && fin_linea == 0)
		{
			if (bytes_recibidos > 0)
			{
				total += bytes_recibidos;
				if (mensaje_recibir[total - 1] == 13 || mensaje_recibir[total - 1] == 10)
					fin_linea = 1;
			}
			else
				intentos++;
			Sleep(PAUSA_MS);
			bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);
		}
		if (total > 0)
			mensaje_recibir[total - 1] = '\0';
		return total;
	}

	float float_from_cadena(char* cadena)
	{
		float numero = 0;
		int i, divisor = 10, estado = 0;


		for (i = 0; cadena[i] != '\0' && estado != 3 && i < MAX_BUFFER; i++)
			switch (estado)
			{
			case 0:// Antes del n�mero
				if (cadena[i] >= '0' && cadena[i] <= '9')
				{
					numero = cadena[i] - '0';
					estado = 1;
				}
				break;
			case 1:// Durante el n�mero
				if (cadena[i] >= '0' && cadena[i] <= '9')
					numero = numero * 10 + cadena[i] - '0';
				else
					if (cadena[i] == '.' || cadena[i] == ',')
						estado = 2;
					else
						estado = 3;
				break;
			case 2: // Parte decimal
				if (cadena[i] >= '0' && cadena[i] <= '9')
				{
					numero = numero + (float)(cadena[i] - '0') / divisor;
					divisor *= 10;
				}
				else
					estado = 3;
				break;
			}
		return numero;
	}
	void monitorizar_sensor_distancia(Serial* Arduino)
	{
		float frecuencia, temperatura;
		char tecla;
		do
		{
			printf("Establezca frecuencia de muestreo (0,5 Hz - 2,0 Hz):");
			scanf_s("%f", &frecuencia);
		} while (frecuencia < 0.5 || frecuencia>2.0);
		printf("Pulse una tecla para finalizar la monitorizaci�n\n");
		do
		{
			if (Arduino->IsConnected())
			{
				temperatura = leer_sensor_Temperatura(Arduino);
				if (temperatura != -1)
					printf("%.2f ", temperatura);
				else
					printf("XXX ");
			}
			else
				printf("\nNo se ha podido conectar con Arduino.\n");
			if ((1 / frecuencia) * 1000 > PAUSA_MS)
				Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
		} while (_kbhit() == 0);
		tecla = _getch();
		return;
	}