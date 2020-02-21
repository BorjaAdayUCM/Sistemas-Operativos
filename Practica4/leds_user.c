#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int largo_cadena(char cadena[]);

void modulo();

int menu();

void imprimirNumeroBinario(int numero, int descr);

void binario(int descr);

void contador(int descr);

void gray(int descr);

void generadorDeSecuencia(int descr);

void ledsVelocidades(int descr);

int main() {
	int descr = open("/dev/leds", O_WRONLY);
	write(descr, "0", largo_cadena("0"));
	int op = menu();
	while(op != 0) {
		if (op == 1) modulo(descr);
		else if (op == 2) binario(descr);
		else if (op == 3) gray(descr);
		else if (op == 4) contador(descr);
		else if(op == 5) generadorDeSecuencia(descr);
		else if (op == 6) ledsVelocidades(descr);
		op = menu();
	}
	close(descr);
}

int menu(){
	int op = 0;
	printf("\n0 - Salir.\n");
	printf("1 - Conversor a modulos.\n");
	printf("2 - Conversor a binario.\n");
	printf("3 - Conversor a gray.\n");
	printf("4 - Contador ascendente o descendente.\n");
	printf("5 - Generador de secuencias.\n");
	printf("6 - Velocidades en leds.\n");
	printf("Seleccione una opcion: ");
	scanf("%d", &op);
	printf("\n");
	return op;
}

void modulo(int descr) {
	int numero, modulo;
	printf("¿A que modulo desea convertir (2 al 8)? ");
	scanf("%d", &modulo);
	printf("¿Que numero desea convertir? ");
	scanf("%d", &numero);
	imprimirNumeroBinario(numero % modulo, descr);
}

void binario(int descr) {
	int numero;
	printf("¿Que numero desea convertir a binario (0 al 7)? ");
	scanf("%d", &numero);
	imprimirNumeroBinario(numero, descr);
}

void contador(int descr) {
	int sentido, veces, numero, i;
	printf("¿Ascendente (1) o descendente (-1)? ");
	scanf("%d", &sentido);
	numero = (sentido == 1)? 0: 7;
	printf("¿Cuantas veces quieres contar hasta 7? ");
	scanf("%d", &veces);
	for(i = 0; i < 8 * veces; i++) {
		imprimirNumeroBinario(numero,descr);
		usleep(1 * 1000000);
		if(sentido == -1 && numero == 0) numero = 7;
		else numero = (numero+sentido) % 8;
	}
}

void imprimirNumeroBinario(int numero, int descr) {
	switch (numero) {
		case 0: write(descr, "0", largo_cadena("0")); break;
		case 1: write(descr, "3", largo_cadena("3")); break;
		case 2: write(descr, "2", largo_cadena("2")); break;
		case 3: write(descr, "23", largo_cadena("23")); break;
		case 4: write(descr, "1", largo_cadena("1")); break;
		case 5: write(descr, "13", largo_cadena("13")); break;
		case 6: write(descr, "12", largo_cadena("12")); break;
		case 7: write(descr, "123", largo_cadena("123")); break;
		default: write(descr, "0", largo_cadena("0")); break;
	}
}

void gray(int descr) {
	int numero;
	printf("¿Que numero desea convertir a gray (0 al 7)? ");
	scanf("%d", &numero);

	switch (numero) {
		case 0: write(descr, "0", largo_cadena("0")); break;
		case 1: write(descr, "3", largo_cadena("3")); break;
		case 2: write(descr, "23", largo_cadena("23")); break;
		case 3: write(descr, "2", largo_cadena("2")); break;
		case 4: write(descr, "12", largo_cadena("12")); break;
		case 5: write(descr, "123", largo_cadena("123")); break;
		case 6: write(descr, "13", largo_cadena("13")); break;
		case 7: write(descr, "1", largo_cadena("1")); break;
		default: write(descr, "0", largo_cadena("0")); break;
	}
}

void generadorDeSecuencia(int descr) {
	char secuencia[100][100];
	int numElementos = 0, veces = 0, i = 0, j = 0;
	double tiempo;
	printf("¿Cuantos elementos quieres que tenga tu secuencia (maximo 100)? ");
	scanf("%d", &numElementos);
	for(i = 0; i < numElementos; i++) {
		printf("¿Que leds quiere encender en %d instante? ", i + 1);
		scanf("%s", secuencia[i]);
	}
	printf("¿Cuanto tiempo de espera entre elementos desea? ");
	scanf("%le", &tiempo);
	printf("¿Cuantas veces quieres repetir la secuencia? ");
	scanf("%d", &veces);
	for(i = 0; i < veces; i++) {
		for(j = 0; j < numElementos; j++) {
			write(descr, secuencia[j], largo_cadena(secuencia[j]));
			usleep(tiempo * 1000000);
		}
	}
}

void ledsVelocidades(int descr) {
	int sentido;
	double velocidad, diferencia;
	printf("¿Quiere velocidad ascendente (1) o descendente (-1)? ");
	scanf("%d", &sentido);
	printf("¿Que velocidad inicial quiere usar? ");
	scanf("%le", &velocidad);
	printf("¿Cuanto quiere que varie la velocidad a cada iteracion? ");
	scanf("%le", &diferencia);
	if (sentido == 1) {
		while (velocidad >= 0) {
			imprimirNumeroBinario(7, descr);
			usleep(0.1 * 1000000);
			imprimirNumeroBinario(0, descr);
			usleep(velocidad * 1000000);
			velocidad -= diferencia;
		}
	}
	else {
		while (velocidad <= 3) {
			imprimirNumeroBinario(7, descr);
			usleep(0.1 * 1000000);
			imprimirNumeroBinario(0, descr);
			usleep(velocidad * 1000000);
			velocidad += diferencia;
		}
	}
}

int largo_cadena(char cadena[]) {
	int largo = 0;
	while(cadena[largo]!='\0') largo++;
	return largo + 1;
}