#include<stdio.h>
#include<stdlib.h>

int menu_ppal(int);


int menu_ppal(int opcion) {
	int opcion;

	do {
		printf("MiNevera");
		printf("-------------------");
		printf("Elija una opcion");
		printf("1-Estado de los componentes");
		printf("2-Temperatura");
		printf("3-Alarmas");
		printf("4-Control de los sensores");
		printf("5-Registros");
		printf("6-Salir");
		scanf_s("%d", &opcion);

	} while ((opcion != 1) && (opcion != 2) && (opcion != 3) && (opcion != 4) && (opcion != 5));
	return opcion;
}
