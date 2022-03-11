#include <stdio.h>
#include <stdlib.h>

int menu_ppal(int);

//int menu_ppal(int respuesta)
int main(void)
{
	int respuesta;
	printf("Bienvenido al menu principal.\n");
	printf("=============================\n");
	printf("Escoja la opción que desea.\n");
	printf("Presione 1 si desea saber sobre la temperatura de su nevera.\n");
	printf("Presione 2 si desea saber sobre los posibles avisos que puede realizar su nevera.\n");
	printf("Presione 3 si desea comprobar los registros de la nevera'.\n");

	printf("Presione 0 si desea salir del programa.\n");
	scanf_s("%d", &respuesta);

	while (respuesta != 0)
	{
		(respuesta == 1);
		{
			printf("Has escogido la opción 1.\n");
			printf("=========================\n");
		}
		(respuesta == 2);
		{
			printf("Has escogido la opción 2.\n");
			printf("=========================\n");
		}
		(respuesta == 3);
		{
			printf("Has escogido la opción 3.\n");
			printf("=========================\n");
		}
		(respuesta > 3);
		{
			printf("la opcíón %d no esta disponble.\n", respuesta);
			printf("Porfavor introduzca de nuevo la opcion.\n");
			scanf_s("%d", &respuesta);
		}
	}

	return 0;
}
