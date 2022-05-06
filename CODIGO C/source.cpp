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

//int menu_ppal1(void);
typedef struct
{
	char nombre[LONGCAD];
	char apellidos[LONGCAD];
	char login[LONGCAD];
	char password[LONGCAD];

}Usuario;

typedef struct
{
	char motivo[LONGCAD];
	char dia[LONGCAD];
	char hora[LONGCAD];

}Alarma;



struct item
{
	Usuario usuario;
	struct item* enlace;
};
typedef struct item Nodo;

struct NV
{
	Alarma alarma;

	struct NV* conexion;
};
typedef struct NV Alarmas;

//conexion con visual

float leer_sensor_Temperatura(Serial* Arduino);
float float_from_cadena(char* cadena);
int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir);
void monitorizar_sensor_temperatura(Serial* Arduino);

//menus

int menu_ppal1(void);
int menu_ppal2(void);

int alta_alarma(Alarmas** pagenda);
Alarma leer_datos_alarma(char operacion);
int alta_usuario(Nodo**);
int iniciar_sesion(Nodo**);
void mostrar_titulos_alarma(void);
void mostrar_titulos_usuario(void);
void mostrar_datos_usuario(Usuario u);
void mostrar_datos_alarma(Alarma u);
int baja_usuario(Nodo**);
int consulta_usuario(Nodo*);
int consulta_alarma(Alarmas*);
int modificacion_usuario(Nodo* lista);
Nodo* posicion_usuario(Nodo*, char*);
Alarmas* posicion_alarmas(Alarmas*, char*);
int escribir_fichero_usuarios(Nodo* lista);
Nodo* leer_fichero_usuarios(void);
void estado_componentes(Serial* Arduino);
void creadores();
void info();

int main(void)
{

	Nodo* lista = NULL;
	lista = leer_fichero_usuarios();
	Serial* Arduino;
	char intro;
	int res;
	char puerto[] = "COM3"; // Puerto serie al que está conectado Arduino
	int opcion_menu1, opcion_menu2;
	setlocale(LC_ALL, "es-ES");
	Arduino = new Serial((char*)puerto);


	do {
		opcion_menu1 = menu_ppal1();
		res = -1;
		switch (opcion_menu1)
		{
		case 1:
			system("cls");
			res = alta_usuario(&lista);
			system("cls");
			break;
		case 2:
			system("cls");
			res = iniciar_sesion(&lista);

			break;
		case 3:
			system("cls");
			res = baja_usuario(&lista);
		
			break;
		case 4:
			info();
			break;
		case 5:
			break;
		}

	} while (opcion_menu1 != 5);

	return 0;
}

int menu_ppal2(void)
{
	static int opcion = -1;
	if (opcion != 0)
	{
		system("cls");
		printf("\nMiNevera\n");
		printf("-------------------\n");
		printf("Elija una opcion:\n");
		printf("1-Estado de los componentes\n");
		printf("2-Temperatura\n");
		//printf("3-contenido\n"); // controlado mediante un sensor ultrasonidos
		printf("3-Alarmas\n");
		printf("4-Abrir nevera\n");
		printf("5-Informacion del proyecto y participantes\n");
		//printf("6-usuarios registrados(baja de usuario)\n");
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
int menu_ppal1(void) {
	int opcion = -1;
	char intro;

	do {

		printf("introduzca su usuario y contraseña, en el caso de no tener,por favor registrese\n");
		printf("-------------------\n");
		printf("Elija una opcion:\n");
		printf("1-registrarse\n");
		printf("2-Usuarios\n");
		printf("3-Eliminar usuarios\n");
		printf("4-informacion\n");
		printf("5-Salir\n");
		printf("opcion:");
		scanf_s("%d", &opcion);
		intro = getchar();
		if (opcion < 1 || opcion>6)
			printf("\nOpción inexistente.\n\n");
	} while (opcion < 1 || opcion>5);
	printf("\n");

	return opcion;
}


float leer_sensor_Temperatura(Serial* Arduino)
{
	float temperatura;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_TEMPERATURA\n", mensaje_recibido);
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
	return temperatura;
}
float leer_sensor_distancia(Serial* Arduino)
{
	float distancia;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_DISTANCIA\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		distancia = -500;
	}
	else
	{
		distancia = float_from_cadena(mensaje_recibido);
	}
	return distancia;
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
void monitorizar_sensor_temperatura(Serial* Arduino)
{
	float frecuencia, temperatura;
	char tecla;
	do
	{
		printf("Establezca frecuencia de muestreo (0,5 Hz - 2,0 Hz):");
		scanf_s("%f", &frecuencia);
	} while (frecuencia < 0.5 || frecuencia>2.0);
	printf("Pulse una tecla para finalizar la monitorización\n");
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
void monitorizar_sensor_distancia(Serial* Arduino)
{
	float frecuencia, distancia;
	char tecla;
	do
	{
		printf("Establezca frecuencia de muestreo (0,5 Hz - 2,0 Hz):");
		scanf_s("%f", &frecuencia);
	} while (frecuencia < 0.5 || frecuencia>2.0);
	printf("Pulse una tecla para finalizar la monitorización\n");
	do
	{
		if (Arduino->IsConnected())
		{
			distancia = leer_sensor_distancia(Arduino);
			if (distancia != -1)
				printf("%.2f ", distancia);
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
void estado_componentes(Serial* Arduino)
{
	char intro;
	float distancia, temperatura, funciona;

	system("cls");

	distancia = leer_sensor_distancia(Arduino);
	temperatura = leer_sensor_Temperatura(Arduino);
	

	system("cls");
	printf("\n");
	printf("   ESTADO DE LOS COMPONENTES\n");
	

	if (distancia == -500)
		printf("\n  Error, revise el sensor de distancia\n");
	if (distancia != -500)
		printf("\n  El sensor de distancia funciona correctamente\n");

	if (temperatura == -500)
		printf("\n  Error, revise el sensor de temperatura\n");
	if (temperatura != -500)
		printf("\n  El sensor de temperatura funciona correctamente\n");

	scanf_s("%c", &intro);
	system("cls");
}

Usuario leer_datos_usuario(char operacion)
{
	Usuario usuario;
	
	if (operacion == 'A')
	{
		printf("Alta de un nuevo usuario\n");
		printf("=======================\n");
	}
	else
	{
		printf("Modificar usuario (intro mantiene dato previo)\n");
		printf("==============================================\n");
	}
	printf("Nombre:");
	gets_s(usuario.nombre, LONGCAD);
	printf("Apellidos:");
	gets_s(usuario.apellidos, LONGCAD);
	if (operacion == 'A') // En el alta lee el login. En modificaci�n mantiene el anterior.
	{
		printf("Login:");
		gets_s(usuario.login, LONGCAD);
	}
	printf("Password:");
	gets_s(usuario.password, LONGCAD);


	return usuario;
}
Alarma leer_datos_alarma(char operacion)
{
	Alarma alarma;


	if (operacion == 'A')
	{
		printf("Definir nueva alarma\n");
		printf("=======================\n");
	}
	else
	{
		printf("Modificar alarma (intro mantiene dato previo)\n");
		printf("==============================================\n");
	}
	printf("dia:");
	gets_s(alarma.dia, LONGCAD);
	printf("hora:");
	gets_s(alarma.hora, LONGCAD);
	if (operacion == 'A') // En el alta lee el login. En modificaci�n mantiene el anterior.
	{
		printf("Motivo:");
		gets_s(alarma.motivo, LONGCAD);
	}


	return alarma;
}
int alta_usuario(Nodo** plista)
{
	char intro;
	Nodo* cab = *plista;
	Nodo* p;
	int cod_error = 0;


	p = (Nodo*)malloc(sizeof(Nodo));
	if (p == NULL)
	{
		printf("No se ha podido reservar memoria para un nuevo usuario\n");
		cod_error = -1;
	}
	else
	{
		p->usuario = leer_datos_usuario('A');
		p->enlace = (struct item*)cab;
		cab = p;
		printf("se ha registrado correctamente");
		scanf_s("%c", &intro);
	}
	*plista = cab;
	return cod_error;
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
		p->alarma = leer_datos_alarma('A');
		p->conexion = (struct NV*)cab;
		cab = p;
		printf("Alarma registrada correctamente");
		scanf_s("%c", &intro);
	}
	*pagenda = cab;
	return cod_error;
}
Nodo* posicion_usuario(Nodo* lista, char* login)
{
	Nodo* p;

	for (p = lista; p != NULL; p = p->enlace)
		if (strcmp(login, p->usuario.login) == 0)
			return p;
	return p;
}
Alarmas* posicion_alarmas(Alarmas* agenda, char* motivo)
{
	Alarmas* p;

	for (p = agenda; p != NULL; p = p->conexion)
		if (strcmp(motivo, p->alarma.motivo) == 0)
			return p;
	return p;
}
int consulta_usuario(Nodo* lista)
{
	char login[LONGCAD];
	Nodo* nodo;
	int cod_error = 0;
	printf("Introduzca login a buscar:");
	gets_s(login, LONGCAD);
	nodo = posicion_usuario(lista, login);
	if (nodo == NULL)
	{
		printf("No existe ning�n usuario con ese login\n");
		cod_error = -1;
	}
	else
	{
		mostrar_titulos_usuario();
		mostrar_datos_usuario(nodo->usuario);
	}
	return cod_error;
}
int consulta_alarma(Alarmas* agenda)
{
	char motivo[LONGCAD];
	char intro;
	Alarmas* alarmas;
	int cod_error = 0;
	printf("Introduzca la alarma a buscar:");
	gets_s(motivo, LONGCAD);
	alarmas = posicion_alarmas(agenda, motivo);
	if (alarmas == NULL)
	{
		
		printf("No existe ninguna alarma programada\n");
		
		cod_error = -1;

	}
	else
	{
		mostrar_titulos_alarma();
		mostrar_datos_alarma(alarmas->alarma);
	}
	return cod_error;
}
int iniciar_sesion(Nodo** lista) {
	char login[LONGCAD], password[LONGCAD], respuesta[2];
	int encontrado = NO;
	char intro;
	int res;
	int opcion;
	Alarmas* agenda = NULL;
	Nodo* p, * q;
	int cod_error = 0;
	char puerto[] = "COM3";
	int opcion_menu2;
	Serial* Arduino;
	Arduino = new Serial((char*)puerto);


	if (*lista == NULL)
	{

		printf("=========================\n");
		printf("La lista está vacía\n");
		printf("=========================\n");
		scanf_s("%c", &intro);
		system("cls");
		cod_error = -1;
	}
	else {
		printf("introduzca su usuario y contraseña, en el caso de no tener,por favor registrese\n");
		printf("Login:");
		gets_s(login, LONGCAD);
		q = *lista;
		printf("Contraseña:");
		gets_s(password, LONGCAD);
		q = *lista;
		//printf("<pulse la barra espaciadora para volver atras >");
		for (p = q; p != NULL && encontrado == NO; p = p->enlace)
		{
			if (strcmp(login, p->usuario.login) == 0 && strcmp(password, p->usuario.password) == 0)
			{
				printf("Usuario y contraseña correctos\n");
				scanf_s("%c", &intro);
				system("cls");
				//printf("Usuario y contraseña correctos\n");
				menu_ppal2();
				do
				{
					opcion_menu2 = menu_ppal2();
					switch (opcion_menu2)
					{
					case 1:
						estado_componentes(Arduino);
						break;
					case 2:
						system("cls");
						monitorizar_sensor_temperatura(Arduino);
						break;
					case 3:
						system("cls");
						res = alta_alarma(&agenda);
						break;
					case 4:
		 
						break;
					case 5:
						creadores();
						break;
					case 6:
						system("cls");
						do {
							printf(" REGISTROS");
							printf("\n  ==========");
							printf("\n  1 - Ver alarmas programadas");
							printf("\n  2 - Ver usuarios registrados");
							printf("\n  3 - Modificar usuario");
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
							scanf_s("%c", &intro);
							res = consulta_alarma(agenda);
							scanf_s("%c", &intro);
							

							break;
						case 2:
							scanf_s("%c", &intro);
							res = consulta_usuario(*lista);
							scanf_s("%c", &intro);
							break;
							//res = consulta_usuario(lista);
						case 3:
							scanf_s("%c", &intro);
							res = modificacion_usuario(*lista);
							scanf_s("%c", &intro);
							//scanf_s("%c", &intro);
							break;

						}
						break;
					case 7:
						encontrado = SI;
						system("cls");
						printf("\n\n\n\n                  PULSE ENTER DOS VECES PARA VOLVER AL MENU PRINCIPAL");
						scanf_s("%c", &intro);
						system("cls");
						break;

					}

				} while (opcion_menu2 != 7);
				gets_s(respuesta, 2);
				
			}


		}
		if (encontrado == NO) {
			printf("usuario y contraseña no coinciden, intentelo de nuevo\n");
			scanf_s("%c", &intro);
			system("cls");


		}
		return cod_error;
	}
}
int modificacion_usuario(Nodo* lista)
{
	char login[LONGCAD];
	Nodo* nodo;
	Usuario nuevos_datos;
	int cod_error = 0;
	printf("Introduzca login a buscar:");
	gets_s(login, LONGCAD);
	nodo = posicion_usuario(lista, login);
	if (nodo == NULL)
	{
		printf("No existe ning�n usuario con ese login\n");
		cod_error = -1;
	}
	else
	{
		mostrar_titulos_usuario();
		mostrar_datos_usuario(nodo->usuario);
		printf("\n");
		nuevos_datos = leer_datos_usuario('M');
		if (strcmp(nuevos_datos.nombre, "") != 0)
			strcpy_s(nodo->usuario.nombre, LONGCAD, nuevos_datos.nombre);
		if (strcmp(nuevos_datos.apellidos, "") != 0)
			strcpy_s(nodo->usuario.apellidos, LONGCAD, nuevos_datos.apellidos);
		if (strcmp(nuevos_datos.password, "") != 0)
			strcpy_s(nodo->usuario.password, LONGCAD, nuevos_datos.password);
		
	}
	return cod_error;
}
void mostrar_titulos_usuario(void)
{
	const char titulos[][LONGCAD] = { "Nombre","Apellidos","Login","Password" };
	int i;

	for (i = 0; i < 4; i++)
		printf("%-15s", titulos[i]);
	printf("\n");
	for (i = 0; i < 15 * 5; i++)
		printf("=");
	printf("\n");
}
void mostrar_titulos_alarma(void)
{
	const char titulos[][LONGCAD] = { "motivo","dia","hora" };
	int i;

	for (i = 0; i < 3; i++)
		printf("%-15s", titulos[i]);
	printf("\n");
	for (i = 0; i < 15 * 5; i++)
		printf("=");
	printf("\n");
}
void mostrar_datos_usuario(Usuario u)
{
	printf("%-15s%-15s", u.nombre, u.apellidos);
	printf("%-15s%-15s\n", u.login, u.password);

}
void mostrar_datos_alarma(Alarma u)
{
	printf("%-15s%-15s", u.motivo, u.dia);
	printf("%-15s\n", u.hora);

}
int baja_usuario(Nodo** lista)
{
	char login[LONGCAD], respuesta[2];
	int encontrado = NO;
	Nodo* p, * q;
	int cod_error = 0;
	char intro;

	if (*lista == NULL)
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
		printf("Introduzca login a eliminar:");
		gets_s(login, LONGCAD);
		q = *lista;
		for (p = q; p != NULL && encontrado == NO; p = p->enlace)
		{
			if (strcmp(login, p->usuario.login) == 0)
			{
				printf("Datos del usuario encontrado:\n");
				mostrar_titulos_usuario();
				mostrar_datos_usuario(p->usuario);
				printf("Desea borra este usuario (S/N):");
				gets_s(respuesta, 2);
				if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
				{
					encontrado = SI;
					if (p == q) // Es el primero
						*lista = p->enlace;
					else
						q->enlace = p->enlace;
					free(p);
					printf("\n\n -Se ha eliminado el usuario.");

					break;
				}
				else
					cod_error = -1;
			}
			else
				if (p != q)
					q = q->enlace;
		}
		if (encontrado == NO)
		{
			printf("-No se ha encontrado el usuario\n");
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
void info() {
	char intro;
	system("cls");
	printf("\n\n");
	printf("        Bienvenido al manual de instrucciones de como usar el dispositivo\n");
	printf("        En primer lugar debe crear un usuario, tras haberlo creado o si ya\n");
	printf("        existía dicho usuario debe iniciar sesión en el apartado “Usuario”.\n");
	printf("        Tras iniciar sesión de forma correcta se encontrará el menú principal\n");
	printf("        donde podrá acceder a los diferentes servicios de la nevera.\n\n");
	printf("        Esperemos que le haya sido útil.");
	scanf_s("%c", &intro);
	system("cls");
}
void funcionamiento_componentes() {

}
Nodo* leer_fichero_usuarios(void)
{
	Nodo* p, * cab = NULL;
	Usuario u;
	FILE* fichero; // Puntero para manipular el fichero
	errno_t cod_error; // C�digo de error tras el proceso de apertura.

	cod_error = fopen_s(&fichero, "Usuarios.bin", "rb"); // Se intenta abrir el fichero 
	if (cod_error != 0)  // Si el fichero no se ha podido abrir
		cab = NULL; // La lista estar� vac�a
	else  // Si el fichero ha podido abrirse 
	{
		fread_s(&u, sizeof(Usuario), sizeof(Usuario), 1, fichero);
		while (!feof(fichero))
		{
			p = (Nodo*)malloc(sizeof(Nodo));
			if (p == NULL)
			{
				printf("Memoria insuficiente para leer el fichero\n");
				break;
			}
			else
			{
				p->usuario = u;
				p->enlace = cab;
				cab = p;
			}
			fread_s(&u, sizeof(Usuario), sizeof(Usuario), 1, fichero);
		}
		fclose(fichero);
	}
	return cab;
}

int escribir_fichero_usuarios(Nodo* lista)
{
	FILE* fichero;
	errno_t err;

	err = fopen_s(&fichero, "Usuarios.bin", "wb");
	if (err == 0) // Si el fichero se ha podido crear
	{
		while (lista != NULL)
		{
			fwrite(&lista->usuario, sizeof(Usuario), 1, fichero);
			lista = lista->enlace;
		}
		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar el fichero de usuarios\n");
	return err;
}
