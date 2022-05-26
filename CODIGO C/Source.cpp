#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include "SerialClass/SerialClass.h"
#include<math.h>
#define M1 0
#define M2 1

#define NO 0
#define SI 1
#define N 100
#define MAX_BUFFER 200
#define PAUSA_MS 200 
#define TAM 30
#define LONGCAD 50
#define length 16
#define FICHERO "sensores.txt"


typedef struct
{
	char motivo[LONGCAD];
	char mes[LONGCAD];
	char dia[LONGCAD];
	char hora[LONGCAD];
	char minuto[LONGCAD];
	char segundo[LONGCAD];

}Alarma;


struct NV
{
	Alarma alarma;

	struct NV* conexion;
};
typedef struct NV Alarmas;

typedef struct
{
	
	int humedad;
	int temperatura;
} sensores;

int crono(void);

//conexion con visual

float leer_sensor_Temperatura(Serial* Arduino);
float leer_sensor_Humedad(Serial* Arduino);
float float_from_cadena(char* cadena);
int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir);
void monitorizar_sensor_temperatura(Serial* Arduino);
int abrir_nevera(Serial* Arduino);
int cerrar_nevera(Serial* Arduino);
void estado_componentes(Serial* Arduino);




void mostrar_datos_temperatura(sensores u);

int menu_ppal2(void);

int alta_alarma(Alarmas** pagenda);
Alarma leer(void);
void mostrar_titulos_alarma(void);
int modificacion_alarma(Alarmas* agenda);
void mostrar_datos_alarma(Alarma u);
int consulta_alarma(Alarmas*);
int baja_alarma(Alarmas** agenda);
Alarmas* posicion_alarmas(Alarmas*, char*);
Alarmas* leer_fichero_agenda(void);
int escribir_fichero_agenda(Alarmas* agenda);
void escribir_fichero_sensores(sensores* data);

int guardar_datos(void);
void creadores();
void info();
void record();


int main(void)
{
	
	
	Alarmas* agenda = NULL;
	sensores* data = NULL;
	Alarmas* p, * q;
	agenda = leer_fichero_agenda();
	Serial* Arduino;
	char intro;
	int res;
	int opcion;
	char puerto[] = "COM3"; // Puerto serie al que está conectado Arduino
	int  opcion_menu2;
	setlocale(LC_ALL, "es-ES");
	sensores Sensores;
	sensores* memo;
	Arduino = new Serial((char*)puerto);

	do {
		opcion = menu_ppal2();
		res = -1;
		switch (opcion)
		{
		case 1:
			system("cls");
			estado_componentes(Arduino);
			break;
		case 2:
			system("cls");
			do {
				printf(" ACCESO A  LA TEMPERATURA Y HUMEDAD DE LA NEVERA");
				printf("\n  ==========");
				printf("\n  1 - ver tempertaura actual");
				printf("\n  2 - ver humedad");
				printf("\n  3 - volver");
				printf("\n\n\t  OPCIÓN ?: ");
				scanf_s("%d", &opcion);

				if (opcion != 1 && opcion != 2 && opcion != 3)
				{
					system("cls");
					printf("\n Opción no valida\n\n");
				}
			} while (opcion != 1 && opcion != 2 && opcion != 3);
			switch (opcion) {
			case 1:

			
				int i, n;
				printf(" Numero de muestras de la temperatura que desea tomar : ");
				scanf_s("%d", &n);
				memo = (sensores*)malloc(sizeof(sensores) * n);
				if (memo == NULL)
					printf("Memoria insuficiente para leer el fichero");
				else {
					for (i = 0; i < n; i++) {
						Sensores.temperatura = leer_sensor_Temperatura(Arduino);
						Sleep(1500);
						memo->temperatura = Sensores.temperatura;
						Sleep(1500);
						printf("\n Sensor temperatura: %d", memo->temperatura);
						escribir_fichero_sensores(memo);
						
					}
				}
				
			
				break;
			case 2:
				printf(" Numero de muestras de la humedad que desea tomar : ");
				scanf_s("%d", &n);
				
				memo = (sensores*)malloc(sizeof(sensores) * n);
				if (memo == NULL)
					printf("Memoria insuficiente para leer el fichero");
				else
				{
					for (i = 0; i < n; i++) {
						Sensores.humedad = leer_sensor_Humedad(Arduino);
						Sleep(1500);
						memo->humedad = Sensores.humedad;
						Sleep(1500);
						printf("\n Sensor humedad: %d", memo->humedad);
						escribir_fichero_sensores(memo);
						
					}
				}
				
				break;
			case 3:
				break;
			}
			break;

		case 3:
			system("cls");
			record();
			res = alta_alarma(&agenda);
			system("cls");

			break;
		case 4:
			
			system("cls");
			do {
				printf(" ACCESO A PUERTA");
				printf("\n  ==========");
				printf("\n  1 - Abrir puertas");
				printf("\n  2 - Cerrar puertas");
				printf("\n  3 - volver");
				printf("\n\n\t  OPCIÓN ?: ");
				scanf_s("%d", &opcion);

				if (opcion != 1 && opcion != 2 && opcion != 3)
				{
					system("cls");
					printf("\n Opción no valida\n\n");
				}
			} while (opcion != 1 && opcion != 2 && opcion != 3);
			switch (opcion)
			{
			case 1:
				res=abrir_nevera(Arduino);
				scanf_s("%c", &intro);
				break;
			case 2:
				res=cerrar_nevera(Arduino);
				scanf_s("%c", &intro);
				break;	
			case 3:
				break;
			}

			break;
		case 5:
			creadores();
			break;
		case 6:
			system("cls");
			do {
				printf(" REGISTROS");
				printf("\n  ==========");
				printf("\n  1 - Ver recordatorios programadados");
				printf("\n  2 - volver");
				printf("\n\n\t  OPCIÓN ?: ");
				scanf_s("%d", &opcion);
				if (opcion != 1 && opcion != 2 )
				{
					system("cls");
					printf("\n Opción no valida\n\n");
				}
			} while (opcion != 1 && opcion != 2 );
			switch (opcion)
			{
			while (opcion != 4) {
			case 1:
				scanf_s("%c", &intro);
				consulta_alarma(agenda);
				
				do {
					printf("\n  ===========");
					printf("\n  ==========");
					printf("\n  1 - Eliminar recordatorio");
					printf("\n  2 - Modificar recordatorio ");
					printf("\n  3 - volver");
					printf("\n\n\t  OPCIÓN ?: ");
					scanf_s("%d", &opcion);
					if (opcion != 1 && opcion != 2 && opcion != 3 && opcion != 4)
					{
						system("cls");
						printf("\n Opción no valida\n\n");
					}
				} while (opcion != 1 && opcion != 2 && opcion != 3 && opcion != 4);
				switch (opcion)
				{
					while (opcion != 3) {
				case 1:
					scanf_s("%c", &intro);
					res = baja_alarma(&agenda);
					
					break;
				case 2:
					scanf_s("%c", &intro);
					res = modificacion_alarma(agenda);
					scanf_s("%c", &intro);
					//scanf_s("%c", &intro);
					break;

					}
				}

				break;
			case 2:
				
				break;

				}
			}

			break;
		case 7:
			break;
		}
		if (res == 0)
			escribir_fichero_agenda(agenda);
	} while (opcion != 7);

	return 0;
}

int menu_ppal2(void)
{
	static int opcion = -1;
	if (opcion != 0)
	{
		system("cls");
		printf("\nMiAlmacen\n");
		printf("-------------------\n");
		printf("Elija una opcion:\n");
		printf("1-Estado de los componentes\n");
		printf("2-Temperatura\n");
		printf("3-recordatorios\n");
		printf("4-Abrir almacen\n");
		printf("5-Informacion del proyecto y participantes\n");
		printf("6-Registros\n");
		printf("7-Salir\n");
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

float leer_sensor_Temperatura(Serial* Arduino)
{
	char opcion;
	float temperatura;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_TEMPERATURA\n", mensaje_recibido);
	temperatura = float_from_cadena(mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		printf("\nNo se ha recibido respuesta a la petición\n");
		temperatura = -1;
	}
	else
	{
		printf("\nLa respuesta recibida tiene %d bytes. Recibido=%s\n", bytesRecibidos,
			mensaje_recibido);
		temperatura = float_from_cadena(mensaje_recibido);
	}
	scanf_s("%c", &opcion);
	return temperatura;
}
float leer_sensor_Humedad(Serial* Arduino)
{
	char opcion;
	float humedad;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_HUMEDAD\n", mensaje_recibido);
	humedad = float_from_cadena(mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		printf("\nNo se ha recibido respuesta a la petición\n");
		humedad = -1;
	}
	else
	{
		printf("\nLa respuesta recibida tiene %d bytes. Recibido=%s\n", bytesRecibidos,
			mensaje_recibido);
		humedad = float_from_cadena(mensaje_recibido);
	}
	scanf_s("%c", &opcion);
	return humedad;
}


float float_from_cadena(char* cadena)
{
	float numero = 0;
	int i, divisor = 10, estado = 0;


	for (i = 0; cadena[i] != '\0' && estado != 3 && i < MAX_BUFFER; i++)
		switch (estado)
		{
		case 0:// Antes del número
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = cadena[i] - '0';
				estado = 1;
			}
			break;
		case 1:// Durante el número
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
int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
{
	int bytes_recibidos = 0, total = 0;
	int intentos = 0, fin_linea = 0;
	Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
	Sleep(200);
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
		Sleep(200);
		bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);
	}
	if (total > 0)
		mensaje_recibir[total - 1] = '\0';
	return total;
}

void estado_componentes(Serial* Arduino)
{
	char intro;
	float distancia, temperatura, correcto;

	system("cls");

	
	temperatura = leer_sensor_Temperatura(Arduino);
	correcto = abrir_nevera(Arduino);
	correcto = cerrar_nevera(Arduino);

	system("cls");
	system("cls");
	printf("\n");
	printf("   ESTADO DE LOS COMPONENTES\n");

	if (temperatura == -500)
		printf("\n  Error, revise el sensor de temperatura\n");
	if (temperatura != -500)
		printf("\n  El sensor de temperatura funciona correctamente\n");
	if (correcto == 0)
		printf("\n  Error revise el servo \n");
	if (correcto == 1)
		printf("\n  El servo funciona correctamente\n");
	printf("\n\n pulsa enter:");
	scanf_s("%c", &intro);
	system("cls");
}

Alarma leer(void)
{
	
	Alarma alarma;
	char intro;
	printf("Introduzca nueva alarma\n");
	printf("========================\n");
	printf("motivo:");
	gets_s(alarma.motivo, LONGCAD);
	printf("mes:");
	gets_s(alarma.mes, LONGCAD);
	printf("dia:");
	gets_s(alarma.dia, LONGCAD);
	printf("\n");
	intro = getchar();
	return alarma;
}
int alta_alarma(Alarmas** pagenda)
{
		char intro;
		Alarmas* cab = *pagenda;
		Alarmas* p;
		int cod_error = 0;


		p = (Alarmas*)malloc(sizeof(Alarmas));
		if (p == NULL)
		{
			printf("No se ha podido reservar memoria para un nuevo usuario\n");
			cod_error = -1;
		}
		else
		{
			p->alarma = leer();
			p->conexion = (struct NV*)cab;
			cab = p;
			printf("Alarma registrada correctamente");
			scanf_s("%c", &intro);
		}
		*pagenda = cab;
	
	return cod_error;
}
Alarmas* posicion_alarmas(Alarmas* agenda, char* motivo)
{
	Alarmas* p;

	for (p = agenda; p != NULL; p = p->conexion)
		if (strcmp(motivo, p->alarma.motivo) == 0)
			return p;
	return p;
}

int consulta_alarma(Alarmas* agenda)
{
	char motivo[LONGCAD];
	char intro;
	Alarmas* alarmas;
	int cod_error = 0;
	printf("Introduzca el recordatorio a buscar:");
	gets_s(motivo, LONGCAD);
	alarmas = posicion_alarmas(agenda, motivo);
	
	if (alarmas == NULL)
	{

		printf("No existe ninguna recordatorio programado\n");

		cod_error = -1;

	}
	else {


		mostrar_titulos_alarma();
		mostrar_datos_alarma(alarmas->alarma);
	}
	return cod_error;
}


int modificacion_alarma(Alarmas* agenda) {
	char motivo[LONGCAD];
	Alarmas* alarmas;
	Alarma nuevos_datos;
	int cod_error = 0;
	printf("Introduzca recordatorio a buscar ( busquela por el motivo que fue creada):");
	gets_s(motivo, LONGCAD);
	alarmas = posicion_alarmas(agenda, motivo);
	if (alarmas == NULL)
	{
		printf("No existe ninguna alarma con ese motivo\n");
		cod_error = -1;
	}
	else
	{
		mostrar_titulos_alarma();
		mostrar_datos_alarma(alarmas->alarma);
		printf("\n");
		nuevos_datos = leer();
		if (strcmp(nuevos_datos.motivo, "") != 0)
			strcpy_s(alarmas->alarma.motivo, LONGCAD, nuevos_datos.motivo);
		if (strcmp(nuevos_datos.mes, "") != 0)
			strcpy_s(alarmas->alarma.mes, LONGCAD, nuevos_datos.mes);
		if (strcmp(nuevos_datos.dia, "") != 0)
			strcpy_s(alarmas->alarma.dia, LONGCAD, nuevos_datos.dia);
	}
	return cod_error;
}
void mostrar_titulos_alarma(void)
{
	const char titulos[][LONGCAD] = { "motivo","mes ","dia",};
	int i;

	for (i = 0; i < 3; i++)
		printf("%-15s", titulos[i]);
	printf("\n");
	for (i = 0; i < 15 *5; i++)
		printf("=");
	printf("\n");
}
void mostrar_datos_alarma(Alarma u)
{
	printf("%-15s%-15s%-15s\n", u.motivo, u.mes, u.dia);

}

int baja_alarma(Alarmas** agenda) {
	char motivo[LONGCAD], respuesta[2];
	int encontrado = NO;
	Alarmas* p, * q;
	int cod_error = 0;
	char intro;
	if (*agenda == NULL)
	{

		printf("\n\n\n\n\t\t\t\t\t\t=========================\n");
		printf("\n\n\n\n\t\t\t\t\t\t  La lista está vacía\n");
		printf("\n\n\n\n\t\t\t\t\t\t=========================\n");
		scanf_s("%c", &intro);
		system("cls");
		cod_error = -1;
	}
	else
	{
		printf("Introduzca recordatorio a eliminar:");
		gets_s(motivo, LONGCAD);
		q = *agenda;
		for (p = q; p != NULL && encontrado == NO; p = p->conexion)
		{
			if (strcmp(motivo, p->alarma.motivo) == 0)
			{
				printf("Datos del usuario encontrado:\n");
				mostrar_titulos_alarma();
				mostrar_datos_alarma(p->alarma);
				printf("Desea borra esta alarma (S/N):");
				gets_s(respuesta, 2);
				if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
				{
					encontrado = SI;
					if (p == q) // Es el primero
						*agenda = p->conexion;
					else
						q->conexion = p->conexion;
					free(p);
					printf("\n\n -Se ha eliminado la alarma.");

					break;
				}
				else
					cod_error = -1;
			}
			else
				if (p != q)
					q = q->conexion;
		}
		if (encontrado == NO)
		{
			printf("-No se ha encontrado la alarma\n");
			cod_error = -1;
		}
		scanf_s("%c", &intro);
		system("cls");
	}



	return cod_error;
}
void creadores() {
	char intro;
	scanf_s("%c", &intro);
	system("cls");
	printf("                                realizado por:\n\n");
	printf("                           - Manuel Vallina Vides\n\n");
	printf("                           - Javier Serrano Molina \n\n");
	printf("                           - Daniel Vázquez Chuvieco");
	scanf_s("%c", &intro);
	system("cls");
}

void record() {
	char intro;
	system("cls");
	printf("\n\n");
	printf("        En este apartado del programa se le permitirá guardar recodatorios\n");
	printf("        personalizados con el motivo desado.                              \n");
	printf("        Introduzca la fecha del momento en el que ha sido guardado para   \n");
	printf("        localizar facilmente el recordatorio.                              \n");
	printf("        Acceda al apartado de registros para ver los recordatiorios.     \n\n");
	printf("        Esperemos que le haya sido útil.");
	scanf_s("%c", &intro);
	system("cls");
}
Alarmas* leer_fichero_agenda(void)
{

	Alarmas* p, * cab = NULL;
	Alarma u;
	FILE* fichero; // Puntero para manipular el fichero
	errno_t cod_error; // C�digo de error tras el proceso de apertura.

	cod_error = fopen_s(&fichero, "Alarmas.bin", "rb"); // Se intenta abrir el fichero 
	if (cod_error != 0)  // Si el fichero no se ha podido abrir
		cab = NULL; // La lista estar� vac�a
	else  // Si el fichero ha podido abrirse 
	{
		fread_s(&u, sizeof(Alarma), sizeof(Alarma), 1, fichero);
		while (!feof(fichero))
		{
			p = (Alarmas*)malloc(sizeof(Alarmas));
			
			if (p == NULL)
			{
				printf("Memoria insuficiente para leer el fichero\n");
				break;
			}
			else
			{
			
				p->alarma = u;
				p->conexion = cab;
				cab = p;
			}
			fread_s(&u, sizeof(Alarma), sizeof(Alarma), 1, fichero);
		}
		fclose(fichero);
	}
	return cab;
}


int escribir_fichero_agenda(Alarmas* agenda)
{
	FILE* fichero;
	errno_t err;

	err = fopen_s(&fichero, "Alarmas.bin", "wb");
	if (err == 0) // Si el fichero se ha podido crear
	{
		while (agenda != NULL)
		{
			fwrite(&agenda->alarma, sizeof(Alarma), 1, fichero);
			agenda = agenda->conexion;
		}
		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar el fichero de alarmas\n");
	return err;
}
int abrir_nevera(Serial* Arduino) {
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];
	int correcto = 1;

	bytesRecibidos = Enviar_y_Recibir(Arduino, "ABRIR_PUERTA\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		correcto = 1;
	}
	return correcto;
}
int cerrar_nevera(Serial* Arduino) {
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];
	int correcto = 1;
	bytesRecibidos = Enviar_y_Recibir(Arduino, "CERRAR_PUERTA\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		correcto = 1;
	}
	return correcto;
}

void escribir_fichero_sensores(sensores* Sensores)
{

	FILE* file;
	errno_t err;

	err = fopen_s(&file, FICHERO, "a+");
	
	if (err == 0) 
	{
		fwrite(&Sensores->humedad, sizeof(sensores),1, file);
		fwrite(&Sensores->temperatura, sizeof(sensores),1 ,file);


		fclose(file);
	}
	else printf("Se ha producido un error al abrir el fichero de datos.\n");
}

void leer_fichero_sensores() {
	FILE* file;
	sensores* cab,* p = NULL;
	errno_t err;
	char  humedad[LONGCAD];
	char temperatura[LONGCAD], trash[LONGCAD];
	err = fopen_s(&file, FICHERO, "a+");
	if (err != 0)  // Si el fichero no se ha podido abrir
		cab = NULL; // La lista estar� vac�a
	err = fopen_s(&file, FICHERO, "a+");
	if (err == 0) 
	{
		while (!feof(file)) {
			fgets(trash, TAM, file);
			fgets(humedad, TAM, file);
			fgets(trash, TAM, file);
			fgets(temperatura, TAM, file);

			printf("\nHumedad: %s\nTemperatura: %s\n\n", humedad, temperatura);
		}
	}
}
